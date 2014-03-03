/*
 * asd.cpp
 *
 *  Created on: May 3, 2010
 *      Author: lauri
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVTransport.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <pthread.h>
#include <errno.h>

#define SID_LOG_MASK 0x1f
#define SID_DBG_TIMESTAMPS 1

#include "SidDebugging.hpp"
#define MAX_BUFLEN 400000
//#define MAX_BUFLEN 10000
char buf[MAX_BUFLEN + 4];

struct JitterStats {
	int min_delay;
	int max_delay;
	float avg_delay;
	int delay_over05ms;
	int delay_over1ms;
	int delay_over2ms;
	int delay_over5ms;
	int delay_over10ms;
	int delay_over20ms;
	int delay_over40ms;
	int pcnt;
	JitterStats() {
		Reset();
	};
	void Reset() {
		min_delay = 0x7fffffff;
		max_delay = 0;
		avg_delay = 0;
		delay_over05ms = 0;
		delay_over1ms = 0;
		delay_over2ms = 0;
		delay_over5ms = 0;
		delay_over10ms = 0;
		delay_over20ms = 0;
		delay_over40ms = 0;
		pcnt = 0;
	}
	void UpdateStats(int delay) {
		if (min_delay > delay) {
			min_delay = delay;
		}
		if (max_delay < delay) {
			max_delay = delay;
		}
		avg_delay += ((float)(delay - avg_delay) / ++pcnt);

		if (delay > 40000) {
			delay_over40ms++;
		} else if (delay > 20000) {
			delay_over20ms++;
		} else if (delay > 10000) {
			delay_over10ms++;
		} else if (delay > 5000) {
			delay_over5ms++;
		} else if (delay > 2000) {
			delay_over2ms++;
		} else if (delay > 1000) {
			delay_over1ms++;
		} else if (delay > 500) {
			delay_over05ms++;
		}
	}
	void Print(char * prefix) {
		SID_DBG("%s min: %d us", prefix, min_delay);
		SID_DBG("%s max: %d us", prefix, max_delay);
		SID_DBG("%s avg: %.0f us", prefix, avg_delay);
		SID_DBG("%s >0.5ms: %d", prefix, delay_over05ms);
		SID_DBG("%s >1ms: %d", prefix, delay_over1ms);
		SID_DBG("%s >2ms: %d", prefix, delay_over2ms);
		SID_DBG("%s >5ms: %d", prefix, delay_over5ms);
		SID_DBG("%s >10ms: %d", prefix, delay_over10ms);
		SID_DBG("%s >20ms: %d", prefix, delay_over20ms);
		SID_DBG("%s >40ms: %d", prefix, delay_over40ms);
	}
};

struct OptionsParser {
	OptionsParser() :
		m_IpcPrefix("/tmp/"),
		m_Help(false),
		m_TestNr(-1),
		m_ServerMode(-1),
		m_TestCnt(-1),
		m_CheckData(false),
		m_CheckJitter(0),
		m_CheckMultiSyscall(0),
		m_Nice(0),
		m_MsgLen(1920){
	}
	void Usage(const char *executable) {
		SID_INFO("usage:");
		SID_INFO("	%s [options]", executable);
		SID_INFO("	options:");
		SID_INFO("		-h - print help");
		SID_INFO("		-t test_nr - test number: 1 = random read/write, 2 = last message test, 3 - perfo test");
		SID_INFO("			1 - random read/write");
		SID_INFO("			2 - last message test");
		SID_INFO("			3 - perfo test");
		SID_INFO("			4 - thread wakeup test");
		SID_INFO("      -n <nice value> - default 0");
		SID_INFO("		-s server_mode - 1 - act as server, 0 - act as client");
		SID_INFO("		-c test_count - number of tests");
		SID_INFO("		-k ipc key prefix - key prefix for ipc");
		SID_INFO("		-C - check data");
		SID_INFO("		-j - test jitter");
		SID_INFO("		-m <count of writes&reads per message> - test perfo when multiple writes/reads done instead of one");
		SID_INFO("		-l message length in perfo test");
	}
	int ParseOptions(int argc, const char **argv) {
		for (int i = 1; i < argc; i++) {
			const char c = argv[i][0];
			if (c == '-' || c == '/') {
				switch (argv[i][1]) {
				case 'n': {
					if (i + 1 >= argc)
						return -1;
					m_Nice = atoi(argv[++i]);
					break;
				}
				case 'h': {
					m_Help = true;
					break;
				}
				case 't': {
					if (i + 1 >= argc)
						return -1;
					m_TestNr = atoi(argv[++i]);
					break;
				}
				case 's': {
					if (i + 1 >= argc)
						return -1;
					m_ServerMode = atoi(argv[++i]);
					break;
				}
				case 'c': {
					if (i + 1 >= argc)
						return -1;
					m_TestCnt = atoi(argv[++i]);
					break;
				}
				case 'C': {
					m_CheckData = true;
					break;
				}
				case 'k': {
					if (i + 1 >= argc)
						return -1;
					m_IpcPrefix = argv[++i];
					break;
				}
				case 'j': {
					m_CheckJitter = true;
					break;
				}
				case 'l': {
					if (i + 1 >= argc)
						return -1;
					m_MsgLen = atoi(argv[++i]);
					break;
				}
				case 'm': {
					if (i + 1 >= argc)
						return -1;
					m_CheckMultiSyscall = atoi(argv[++i]);
					break;
				}
				default: {
					return -1;
					break;
				}
				}
			} else {
				return -1;
			}
		}

		if (m_TestNr < 0 || m_ServerMode < 0 || m_TestCnt < 0) {
			return -1;
		}

		return 0;
	}
	const char *m_IpcPrefix;
	bool m_Help;
	int m_TestNr;
	bool m_ServerMode;
	int m_TestCnt;
	bool m_CheckData;
	int m_CheckJitter;
	int m_CheckMultiSyscall;
	int m_Nice;
	int m_MsgLen;
};

static uint64_t getTimeUs() {
	struct timeval tv;
	uint64_t ret;
	gettimeofday(&tv, NULL);
	ret = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
	return ret;
}

int RandomSendRecv(OptionsParser &options) {

	printf("server_mode: %d, testcnt: %d\n", options.m_ServerMode, options.m_TestCnt);

	if(options.m_ServerMode) {
		Sid::AVTransport* test = Sid::GetAVTransport();

		if (!test->Connect(options.m_IpcPrefix, true, -1)) {
			SID_ERROR("test->Connect(server) failed");
			return -1;
		}

		for (int i = 0; i < options.m_TestCnt; i++) {
			int buflen = MAX_BUFLEN * (rand() / (float)RAND_MAX);
			*((int *)buf) = buflen;
			for (int j = 0; j < buflen; j++) {
				buf[4 + j] = (char)j;
			}
			SID_VDBG("write buffer: %d\n", buflen);
			int towrite = 4 + buflen;
			int written = 0;
			test->WriteStart();
			do {
				int wrlen;
				if (towrite > 3) {
					wrlen = towrite * (rand() / (float)RAND_MAX);
				} else {
					wrlen = towrite;
				}
				SID_VDBG("write: %d, %d\n", wrlen, towrite);
				test->Write(&buf[written], wrlen);
				towrite -= wrlen;
				written += wrlen;
			} while(towrite);
			test->WriteComplete();
		}
		printf("press enter\n");
		char asd[100];
		gets(asd);
	} else {
		Sid::AVTransport* test = Sid::GetAVTransport();

		if (!test->Connect(options.m_IpcPrefix, false, 1000)) {
			SID_ERROR("test->Connect(client) failed");
			return -1;
		}


		for (int i = 0; i < options.m_TestCnt; i++) {
			test->ReadStart();
			char lenbuf[4];
			test->Read(lenbuf, 4);
			int len = *((int*)lenbuf);
			SID_VDBG("read buffer: %d\n", len);

			int toread = len;
			int read = 0;
			do {
				int rdlen;
				if (toread > 3) {
					rdlen = toread * (rand() / (float)RAND_MAX);
				} else {
					rdlen = toread;
				}
				SID_VDBG("read: %d, %d\n", rdlen, toread);
				test->Read(&buf[read], rdlen);
				toread -= rdlen;
				read += rdlen;
			} while(toread);
			test->ReadComplete();

			for (int j = 0; j < len; j++) {
				if (buf[j] != (char)j) {
					SID_ERROR("error: buf: %d, len: %d, j: %d, char: %d, instead of %d\n", i, len, j, buf[j], (char)j);
				}
			}
		}
		printf("press enter\n");
		char asd[100];
		gets(asd);
	}
	return 0;
}

int LastMsgTest(OptionsParser &options) {

	if(options.m_ServerMode) {
		Sid::AVTransport* test = Sid::GetAVTransport();

		if (!test->Connect(options.m_IpcPrefix, true, -1)) {
			SID_ERROR("test->Connect(server) failed");
			return -1;
		}

		for (int i = 0; i < options.m_TestCnt; i++) {
			int bufcnt = 10 * (rand() / (float)RAND_MAX);
			SID_VDBG("write & read %d buffers\n", bufcnt);
			for (int j = 0; j < bufcnt; j++) {
				int buflen = MAX_BUFLEN * (rand() / (float)RAND_MAX);
				*((int *)buf) = buflen;/*
				for (int j = 0; j < buflen; j++) {
					buf[4 + j] = (char)j;
				}*/
				SID_VDBG("write buffer: %d\n", buflen);
				int towrite = 4 + buflen;
				test->WriteStart();
				test->Write(buf, towrite);
				test->WriteComplete();
			}
			for (int j = 0; j < bufcnt; j++) {
				test->ReadStart();
				test->Read(buf, 4);
				int len = *((int*)buf);
				SID_VDBG("read buffer: %d\n", len);

				test->Read(&buf[4], len);
				test->ReadComplete();
/*
				for (int j = 0; j < len; j++) {
					if (buf[4 + j] != (char)j) {
						SID_ERROR("error: buf: %d, len: %d, j: %d, char: %d, instead of %d\n", i, len, j, buf[j], (char)j);
					}
				}*/
			}
		}
		printf("press enter\n");
		char asd[100];
		gets(asd);
	} else {
		Sid::AVTransport* test = Sid::GetAVTransport();

		if (!test->Connect(options.m_IpcPrefix, false, 1000)) {
			SID_ERROR("test->Connect(client) failed");
			return -1;
		}

		while (1) {
			if (!test->ReadStart())
				break;
			if (!test->Read(buf, 4))
				break;
			int len = *((int*)buf);
			SID_VDBG("read buffer: %d\n", len);

			if (!test->Read(&buf[4], len))
				break;
			if (!test->ReadComplete())
				break;

/*
			for (int j = 0; j < len; j++) {
				if (buf[4 + j] != (char)j) {
					SID_ERROR("error: buf: %d, len: %d, j: %d, char: %d, instead of %d\n", i, len, j, buf[j], (char)j);
				}
			}*/

			SID_VDBG("write buffer: %d\n", len);
			if (!test->WriteStart())
				break;
			if (!test->Write(buf, len + 4))
				break;
			if (!test->WriteComplete())
				break;
		}
		printf("press enter\n");
		char asd[100];
		gets(asd);
	}
	return 0;
}


int PerfoTest(OptionsParser &options) {
	JitterStats jstats;

	printf("server_mode: %d, bufcnt: %d\n", options.m_ServerMode,  options.m_TestCnt);

	if(options.m_ServerMode) {
		Sid::AVTransport* test = Sid::GetAVTransport();

		if (!test->Connect(options.m_IpcPrefix, true, -1)) {
			SID_ERROR("test->Connect(server) failed");
			return -1;
		}

		// Wait for client to be really (tm) initialized
		//usleep(100000);?
		for (int i = 0; i < options.m_TestCnt; i++) {
			int buflen = options.m_MsgLen;
			*((int *)buf) = buflen;
			SID_VDBG("write buffer: %d\n", buflen);
			if (options.m_CheckData) {
				for (int j = 0; j < buflen; j++) {
					buf[4 + j] = (char)j;
				}
			} else if (options.m_CheckJitter) {
				uint64_t *timestamp = (uint64_t *)(buf + 4);
				*timestamp = getTimeUs();
				SID_VDBG("write buffer: timestamp %llu\n", *timestamp);
			}
			int towrite = 4 + buflen;
			test->WriteStart();

			if (options.m_CheckMultiSyscall) {
				char *tmp_buf = buf;
				int chunk_len = towrite / options.m_CheckMultiSyscall;
				int write_len;
				while (towrite > 0) {
					write_len = (towrite < chunk_len? towrite: chunk_len);
					test->Write(tmp_buf, write_len);
					towrite -= write_len;
					tmp_buf += write_len;
				}
			} else {
				test->Write(buf, towrite);
			}

			test->WriteComplete();
			if (options.m_CheckJitter) {
				usleep(10000);
			}
		}
//		printf("press enter\n");
//		char asd[100];
//		gets(asd);
	} else {
		Sid::AVTransport* test = Sid::GetAVTransport();

		if (!test->Connect(options.m_IpcPrefix, false, 1000)) {
			SID_ERROR("test->Connect(client) failed");
			return -1;
		}

		for (int i = 0; i < options.m_TestCnt; i++) {
			test->ReadStart();
			test->Read(buf, 4);
			int len = *((int*)buf);
			SID_VDBG("read buffer: %d\n", len);

			if (options.m_CheckMultiSyscall) {
				char *tmp_buf = &buf[4];
				int to_read = len;
				int chunk_len = len / options.m_CheckMultiSyscall;
				int read_len;
				while (to_read > 0) {
					read_len = (to_read < chunk_len? to_read: chunk_len);
					SID_VDBG("read_len: %d to_read: %d\n", read_len, to_read);
					test->Read(tmp_buf, read_len);
					to_read -= read_len;
					tmp_buf += read_len;
				}
			} else {
				test->Read(&buf[4], len);
			}

			test->ReadComplete();

			if (options.m_CheckData) {
				for (int j = 0; j < len; j++) {
					if (buf[4 + j] != (char)j) {
						SID_ERROR("error: buf: %d, len: %d, j: %d, char: %d, instead of %d\n", i, len, j, buf[j], (char)j);
					}
				}
			} else if (options.m_CheckJitter) {
				uint64_t *timestamp = (uint64_t *)(buf + 4);
				int delay = (int)(getTimeUs() - *timestamp);
				//SID_DBG("r: t %d",  delay);
				jstats.UpdateStats(delay);
			}
		}
		if (options.m_CheckJitter) {
			jstats.Print("ipc delay");
		}
//		printf("press enter\n");
//		char asd[100];
//		gets(asd);
	}
	return 0;
}

class Event
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	timespec makeTimespec(int milliseconds);
public:
	Event(void) {
		if (pthread_mutex_init(&mutex, NULL))
			SID_FATAL("pthread_mutex_init() failed (%s)", strerror(errno));
		if (pthread_cond_init(&cond, NULL))
			SID_FATAL("pthread_cond_init() failed (%s)", strerror(errno));
	}
	~Event(void) {
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
	}
	bool Wait() {
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		pthread_mutex_unlock(&mutex);
	}
	void Trigger(void) {
		pthread_mutex_lock(&mutex);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
};

struct thread_data_t {
	Event *event;
	OptionsParser *options;
	uint64_t timestamp;
};

void *sender_thread(void * const data) {
	thread_data_t *thread_data = (thread_data_t *)data;
	SID_DBG("sender_thread start: cnt %d",  thread_data->options->m_TestCnt);

	for (int i = 0; i < thread_data->options->m_TestCnt; i++) {

		usleep(10000);
		thread_data->timestamp = getTimeUs();
		thread_data->event->Trigger();
	}
	SID_DBG("sender_thread exit:");
}

void *receiver_thread(void * const data) {
	thread_data_t *thread_data = (thread_data_t *)data;
	JitterStats jstats;
	SID_DBG("receiver_thread start: cnt %d",  thread_data->options->m_TestCnt);

	for (int i = 0; i < thread_data->options->m_TestCnt; i++) {
		thread_data->event->Wait();
		int delay = (int)(getTimeUs() - thread_data->timestamp);
		jstats.UpdateStats(delay);
		//SID_DBG("r: t %d",  delay);
	}
	jstats.Print("thread delay");
	SID_DBG("receiver_thread exit:");
}

int ThreadJitterTest(OptionsParser &options) {
	pthread_t thread_id1;
	pthread_t thread_id2;
	pthread_attr_t attr1;
	pthread_attr_t attr2;
	Event event;
	thread_data_t thread_data;
	thread_data.event = &event;
	thread_data.options = &options;

	pthread_attr_init(&attr1);
	pthread_attr_init(&attr2);

	if (pthread_create(&thread_id1, &attr1, sender_thread, (void *)&thread_data))
		SID_FATAL("pthread_create() failed (%s)", strerror(errno));

	if (pthread_create(&thread_id2, &attr2, receiver_thread, (void *)&thread_data))
		SID_FATAL("pthread_create() failed (%s)", strerror(errno));

	pthread_join(thread_id1, NULL);
	pthread_join(thread_id2, NULL);
}

int main(int argc, const char **argv) {

	OptionsParser parser;
	if (parser.ParseOptions(argc, argv) < 0 || parser.m_Help) {
		parser.Usage(argv[0]);
		return -1;
	}

	if (parser.m_Nice) {
		SID_DBG("set nice(%d)", parser.m_Nice);
#if 0
		if (-1 == nice(parser.m_Nice)) {
			SID_ERROR("nice");
		}
#else
/*		if (-1 == setpriority(PRIO_PROCESS, gettid(), parser.m_Nice)) {
			SID_ERROR("nice");
		}*/
#endif	
	}

	if (parser.m_TestNr == 1) {
		return RandomSendRecv(parser);
	} else if (parser.m_TestNr == 2) {
		return LastMsgTest(parser);
	} else if (parser.m_TestNr == 3){
		return PerfoTest(parser);
	} else {
		return ThreadJitterTest(parser);
	}
}
