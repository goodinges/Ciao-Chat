/**
 * Copyright (C) 2011, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

//#pragma GCC poison HAVE_PTHREAD_COND_TIMEDWAIT_RELATIVE
#include "AudioTest.h"
#include "utils/Log.h"
#include "utility.h"
#include <media/stagefright/FileSource.h>
#include <media/mediarecorder.h>

#include "WavFile.h"

//#define CALLBACK_PLAYBACK 1
#define THREAD_PLAYBACK 2

//#define CALLBACK_RECORDING
#define THREAD_RECORDING 8

using namespace android;

AudioTest::AudioTest() {
	// TODO Auto-generated constructor stub

}

AudioTest::~AudioTest() {
	// TODO Auto-generated destructor stub
}

static uint16_t WAVE_FORMAT_PCM = 1;

static uint32_t U32_LE_AT(const uint8_t *ptr) {
	return ptr[3] << 24 | ptr[2] << 16 | ptr[1] << 8 | ptr[0];
}

static uint16_t U16_LE_AT(const uint8_t *ptr) {
	return ptr[1] << 8 | ptr[0];
}

int AudioTest::initWav() {
	mValidFormat = false;

	uint8_t header[12];
	if (mDataSource->readAt(0, header, sizeof(header)) < (ssize_t)sizeof(header)) {
		return NO_INIT;
	}
	WaveHeader *w = (WaveHeader *)&header[0];
	Log("WAV--------");
	Log("riff:%c%c%c%c", w->riff[0], w->riff[1], w->riff[2], w->riff[3]);
	Log("size:%u", w->size);
	Log("wave:%c%c%c%c", w->wave[0], w->wave[1], w->wave[2], w->wave[3]);

	if (memcmp(header, "RIFF", 4) || memcmp(&header[8], "WAVE", 4)) {
		return NO_INIT;
	}

	size_t totalSize = U32_LE_AT(&header[4]);

	Log("totalSize = %u", totalSize);
	off_t offset = 12;
	size_t remainingSize = totalSize;
	while (remainingSize >= 8) {
		Log("loop...");
		uint8_t chunkHeader[8];
		ssize_t readBytes = mDataSource->readAt(offset, chunkHeader, 8);
		if ( readBytes < 8) {
			Log("chunkHeader NO_INIT rms %u rdby %u", remainingSize, (int)readBytes);
			return NO_INIT;
		}

		remainingSize -= 8;
		offset += 8;

		uint32_t chunkSize = U32_LE_AT(&chunkHeader[4]);
		Log("chunksize %u...", chunkSize);

		if (chunkSize > remainingSize) {
			Log("chunkSize > remainingSize");
			return NO_INIT;
		}

		if (!memcmp(chunkHeader, "fmt ", 4)) {
			if (chunkSize < 16) {
				Log("chunkSize<16");
				return NO_INIT;
			}

			uint8_t formatSpec[16];
			if (mDataSource->readAt(offset, formatSpec, 16) < 16) {
				Log("formatSpec[16]");
				return NO_INIT;
			}

			uint16_t format = U16_LE_AT(formatSpec);
			if (format != WAVE_FORMAT_PCM) {
				Log("format != WAVE_FORMAT_PCM");
				return ERROR_UNSUPPORTED;
			}

			mNumChannels = U16_LE_AT(&formatSpec[2]);
			if (mNumChannels != 1 && mNumChannels != 2) {
				Log("mNumChannels != 1");
				return ERROR_UNSUPPORTED;
			}

			mSampleRate = U32_LE_AT(&formatSpec[4]);

			if (mSampleRate == 0) {
				Log("mSampleRate != 0");
				return ERROR_MALFORMED;
			}

			mBitsPerSample = U16_LE_AT(&formatSpec[14]);

			if (mBitsPerSample != 8 && mBitsPerSample != 16 && mBitsPerSample != 24) {
				Log("mBitsPerSample != 16");
				return ERROR_UNSUPPORTED;
			}

			mValidFormat = true;
			Log("validFormat");
		} else if (!memcmp(chunkHeader, "data", 4)) {
			Log("chunk is data");
			if (mValidFormat) {
				mDataOffset = offset;
				mDataSize = chunkSize;
				Log("WAV:mDataSize %u", mDataSize);
				return OK;
			}
		}

		offset += chunkSize;
	}

	return NO_INIT;
}

void AudioTest::playTest() {
	F_LOG;
	mCounter = 0;
	mDataSource = new FileSource(m_playFile);

	if (OK!=initWav()) {
		Log("init failed");
		return;
	}
	Log("Bitspersample=%u NumChannels=%u SampleRate=%u", mBitsPerSample, mNumChannels, mSampleRate );
	if (m_playbackRate != mSampleRate) {
		Log("Sampling rate mismatch");
		return;
	}
	mCurrentPos = mDataOffset;

	int streamType = AudioSystem::VOICE_CALL;

	int afSampleRate;
	if (AudioSystem::getOutputSamplingRate(&afSampleRate, streamType) != NO_ERROR) {
		Log("outputSamplingRate error");
		return;
	}
	int afFrameCount;
	if (AudioSystem::getOutputFrameCount(&afFrameCount, streamType) != NO_ERROR) {
		Log("outputFrameCount error");
		return;
	}
	uint32_t afLatency;
	if (AudioSystem::getOutputLatency(&afLatency, streamType) != NO_ERROR) {
		Log("outputLatency error");
		return;
	}

	Log("output sampling rate %u framecount %u latency %u", afSampleRate, afFrameCount, afLatency);

#ifdef CALLBACK_PLAYBACK
	audioTrack = new android::AudioTrack(
		streamType, m_playbackRate, AudioSystem::PCM_16_BIT,
		AudioSystem::CHANNEL_OUT_MONO,
		6 * 441, //60 ms
		0, c_playbackCallback, this
		);
	audioTrack->start();
	timer.start();
	{
		Mutex::Autolock autoLock(mLock);
		Log("waiting for finished signal");
		while((!audioTrack->stopped()) /*&& mCounter < 1000 */)
			mSignalFinished.waitRelative(mLock, 100 * 1000 * 1000);
	}
	Log("track stopped");
#endif
#ifdef THREAD_PLAYBACK
	uint32_t minBufCount = afLatency / ((1000 * afFrameCount)/afSampleRate);
	if (minBufCount < 2) {
		minBufCount = 2;
	}
	int minFrameCount = (afFrameCount*m_playbackRate*minBufCount)/afSampleRate;

	Log("minBufCount %u minFrameCount %u", minBufCount, minFrameCount);

	audioTrack = new android::AudioTrack(
		streamType, m_playbackRate, AudioSystem::PCM_16_BIT,
		AudioSystem::CHANNEL_OUT_MONO,
		0 // defaults to minimum framecount
		);
	audioTrack->start();
	timer.start();
	uint8_t buffer[32768];
	unsigned long sleepTime = 10 * 1000;
	unsigned long long idealClock = systemTime();
	unsigned long long newClock;
	for(;;) {
		mCounter++;
		int size = 441 * 2;
		ssize_t n = mDataSource->readAt(mCurrentPos, buffer, size);
		if (n<1) {
			Log("we are done here");
			break;
		}
		mCurrentPos += n;
		int written = audioTrack->write(buffer, n);
		while(written < 1) {
			usleep(1);
			written = audioTrack->write(buffer, n);
		}

		usleep(sleepTime);
		idealClock += 10 * 1000;

		while((newClock = systemTime()) < idealClock)
			usleep(1);
		while(newClock - idealClock > 10*1000) idealClock+= 10 * 1000;
		sleepTime = 10 * 1000 - (newClock - idealClock);

		if (((mCounter % 10) == 0)) {
			timer.stop();
			long long time = timer.durationUsecs();
			Log("durationms %u ", (uint32_t) (time / 1000));
		}
	}
	audioTrack->stop();
#endif
}

#ifdef CALLBACK_PLAYBACK
void AudioTest::c_playbackCallback(int event, void* user, void *info) {
	static_cast<AudioTest *>(user)->playbackCallback(event, info);
}


void AudioTest::playbackCallback(int event, void *info) {
//	F_LOG;
	if (event == AudioTrack::EVENT_UNDERRUN ) {
		Log("buffer underrun");
		return;
	}
	if (event != AudioTrack::EVENT_MORE_DATA) {
		return;
	}

	const AudioTrack::Buffer *buffer = static_cast<const AudioTrack::Buffer *>(info);
	size_t size = buffer->size;
	unsigned char* pDestBuffer = (unsigned char*) buffer->raw;

	mCounter++;
	if (true || ((mCounter % 16) == 0)) {
		timer.stop();
		long long time = timer.durationUsecs();
		uint32_t timems = time / 1000;
		Log("durationms %u size=%u", timems, size);
	}

	ssize_t n = mDataSource->readAt(mCurrentPos, pDestBuffer, size);

	if (n < 1 ) {
		Log("we are done");
		mSignalFinished.signal();
		audioTrack->stop();
		return;
	}
	mCurrentPos += n;
}

#endif


void AudioTest::recordTest() {
	F_LOG;

	int flags = AudioRecord::RECORD_AGC_ENABLE | AudioRecord::RECORD_NS_ENABLE;
	flags = 0;
	audioRecord = new AudioRecord(android::AUDIO_SOURCE_DEFAULT, m_recordRate, AudioSystem::PCM_16_BIT, AudioSystem::CHANNEL_IN_MONO, 0, flags);
	WaveHeader hdr = {
		{'R','I','F','F'},0, {'W','A','V','E'},
		{'f','m','t',' '}, 16,
		WAVE_FORMAT_PCM, 1, //numChannels
		44100,
		0, //bytes per sec
		0, //block align
		16 // bits per sample
	};
	chunkHeader chunk = {
		{'d','a','t','a'},0
	};

	m_f = fopen(m_recordFile, "wb");
	size_t written = 0;
	fwrite( &hdr, sizeof(hdr), 1, m_f);
	fwrite( &chunk, sizeof(chunk), 1, m_f);
	audioRecord->start();
	timer.start();

	unsigned long sleepTime = 10 * 1000;
	unsigned long long idealClock = systemTime();
	unsigned long long newClock;

	mCounter = 0;
	while(mCounter < 1000) { // 1 sec
		mCounter ++;
		//10ms, 16bit
		uint8_t buffer[16384];
		size_t size = 441 * 2;
		ssize_t read = audioRecord->read(buffer, size);
		if ((size_t)read != size) {
			Log("read: %lu, size: %u", read, size);
		}
		fwrite(buffer, read, 1, m_f);

		usleep(sleepTime);
		idealClock += 10 * 1000;

		while((newClock = systemTime()) < idealClock)
			usleep(1);
		while(newClock - idealClock > 10*1000) idealClock+= 10 * 1000;
		sleepTime = 10 * 1000 - (newClock - idealClock);

		if (((mCounter % 10) == 0)) {
			timer.stop();
			long long time = timer.durationUsecs();
			Log("durationms %u ", (uint32_t) (time / 1000));
		}
	}

	audioRecord->stop();
	written = ftell(m_f);
	Log("bytes written total: %u", written);
	uint32_t fileSize = written-8;
	uint32_t dataSize = written-44;
	fseek(m_f, 4, SEEK_SET);
	fwrite(&fileSize, sizeof(fileSize), 1, m_f);
	fseek(m_f, 40, SEEK_SET);
	fwrite(&dataSize, sizeof(dataSize), 1, m_f);

	Log("setting filesize hdr to %u", fileSize);
	Log("setting datasize hdr to %u", dataSize);

	fclose(m_f);
}
