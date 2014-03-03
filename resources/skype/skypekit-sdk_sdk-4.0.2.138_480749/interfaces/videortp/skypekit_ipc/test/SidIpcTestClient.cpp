
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>


#include <SidPlatform.hpp>
#include <SidVideoRTPInterface.hpp>
#include <SidVideoRTPTypesSkypekit31.hpp>
#include <SidVideoRtpIfConnection.hpp>

#define OK "OK"
#define BAD "FAILED"

#define ERROR(fmt, args...) printf(fmt "\n", ##args)
#define INFO(fmt, args...) printf(fmt "\n", ##args)
#define DBG(fmt, args...) //printf(fmt "\n", ##args)

bool alltestsok = true;

class tv {
	public:
		tv() {};
		tv(const char *name, const char *expvalue, const char *value) {
			append(name, expvalue, value);
		}
		tv(const char *name, int expvalue, int value) {
			append(name, expvalue, value);
		}

		tv &append(const char *name, int expvalue, int value)
		{
			sinamelist.append(name);
			expectedint.append(expvalue);
			gotint.append(value);
			return *this;
		}
		tv &append(const char *name, const char *expvalue, const char *value)
		{
			namelist.append(name);
			expectedlist.append(expvalue);
			gotlist.append(value);
			return *this;
		}
		void print(void) const {
			for(unsigned int i=0;i<(unsigned int)namelist.size();i++) {
				printf("\tExpected %s: '%s', was '%s' => %s\n", (const char *)namelist[i], (const char *)expectedlist[i], (const char *)gotlist[i], (expectedlist[i]==gotlist[i] ? OK : BAD));
				if (expectedlist[i]!=gotlist[i])
					alltestsok = false;
			}
			for(unsigned int i=0;i<(unsigned int)sinamelist.size();i++) {
				printf("\tExpected %s: '%i', was '%i' => %s\n", (const char *)sinamelist[i], expectedint[i], gotint[i], (expectedint[i]==gotint[i] ? OK : BAD));
				if (expectedint[i]!=gotint[i])
					alltestsok = false;
			}
		}

	private:
		Sid::List_String namelist, expectedlist, gotlist;
		Sid::List_String inamelist;
		Sid::List_String sinamelist;
		Sid::List_uint expectedint, gotint;
};
#define t(N, R, P) { \
	bool _retval = R; \
	test(N, _retval, P); }
void test(const char *name, bool ret, const tv &tvalues = tv())
{
	printf(" ** Testing %s **\n", name);
	printf("\tReturn value: %i => %s\n", ret, (ret==1? OK : BAD));
	if (ret != 1)
		alltestsok = false;
	tvalues.print();
}

int asd = 0;
struct MyTransport : public SkypeVideoRTPInterfaceCb {
	MyTransport(int kbps): m_ReceivedCnt(0), m_Kbps(kbps) {}
	void SendRTPPacket(const Sid::Binary &data) {
		m_ReceivedCnt += data.size();
		asd++;
		if (m_ReceivedCnt % (m_Kbps / 8 * 100) == 0) {
			DBG("SendRTPPacket: got %lld bytes, asd: %d\n", m_ReceivedCnt, asd);
		}
	}
	void SendFrame(const Sid::Binary &data, int timestamp_ms) {
		m_ReceivedCnt += data.size();
		asd++;
		if (m_ReceivedCnt % (m_Kbps / 8 * 100) == 0) {
			DBG("SendFarme: got %lld bytes, asd: %d\n", m_ReceivedCnt, asd);
		}
	}
	uint64_t m_ReceivedCnt;
	int m_Kbps;
};

struct MyDisconnectCallback : public Sid::SidVideoRtpIfConnection::DisconnectCallback {
	virtual void Disconnected() {
		DBG("MyDisconnectCallback: DISCONNECTED");
	}
};

void BasicTest() {
	bool ret;
	MyTransport myTrans(500);
	MyDisconnectCallback dc;
	Sid::SidVideoRtpIfConnection conn(myTrans, dc);

	bool connected = conn.EnsureConnection();
	if (!connected) {
		ERROR("Connection failed, exit!");
		return;
	}

	SkypeVideoRTPInterface *intf = conn.Interface();

	test("Init", intf->Init());
	int version;
	t("GetInterfaceVersion", intf->GetInterfaceVersion(version),
			tv("version", 32, version));
	test("GetLastError", intf->GetLastError());
	int codec_count;
	t("GetCodecCount", intf->GetCodecCount(codec_count),
			tv("codec_count", 2, codec_count));

	SkypeVideoRTPInterface::VideoCodec codec;
	t("GetCodec", intf->GetCodec(1, codec),
			tv("codec.fourcc", 1234, codec.fourcc).
			append("codec.recording_capable", 1, codec.recording_capable).
			append("codec.payload_format", SkypeVideoRTPInterface::FORMAT_FRAMES, codec.payload_format));

	if (version >= 32) {
		int reso_count;
		t("GetSenderResolutionCount", intf->GetSenderResolutionCount(reso_count),
				tv("reso_count", 1, reso_count));

		SkypeVideoRTPInterface::FixedRes reso;
		t("GetSenderResolution", intf->GetSenderResolution(1, reso),
				tv("reso.width", 640, reso.width).
				append("reso.height", 480, reso.height).
				append("reso.preference", 1, reso.preference));

		t("GetReceiverResolutionCount", intf->GetReceiverResolutionCount(reso_count),
				tv("reso_count", 1, reso_count)
				);

		SkypeVideoRTPInterface::ResCap res_cap;
		t("GetReceiverResolution", intf->GetReceiverResolution(1, res_cap),
				tv("res_cap.max_width", 0x12345678, res_cap.max_width).
				append("res_cap.max_height", 0x9abcdef0, res_cap.max_height).
				append("res_cap.min_width", 0xaabbccdd, res_cap.min_width).
				append("res_cap.min_height", 0xeeff0011, res_cap.min_height).
				append("res_cap.preference", 0x11223344, res_cap.preference));

		int snd_param_cnt = 0;
		ret = intf->GetIntProperty(
				SkypeVideoRTPInterface::GET_NUMBER_OF_H264_SENDER_PARAMETERS,
				0, snd_param_cnt);
		t("GetIntProperty", ret,
				tv("snd_param_cnt", 2, snd_param_cnt));

		Sid::Binary bin;
		bin.reserve(sizeof(SkypeVideoRTPInterface::H264SenderParameters));
		ret = intf->GetBinaryProperty(
				SkypeVideoRTPInterface::GET_H264_SENDER_PARAMETERS,
				0, bin);
		SkypeVideoRTPInterface::H264SenderParameters *snd_params =
			(SkypeVideoRTPInterface::H264SenderParameters *)bin.data();
		test("GetBinaryProperty(0)", ret,
				tv("size", sizeof(SkypeVideoRTPInterface::H264SenderParameters), bin.size()).
				append("profile_idc", 1, snd_params->profile_idc).
				append("profile_iop", 2, snd_params->profile_iop).
				append("level_idc", 3, snd_params->level_idc)
				);

		//bin.Reset();

		bin.reserve(sizeof(SkypeVideoRTPInterface::H264SenderParameters));
		ret = intf->GetBinaryProperty(
				SkypeVideoRTPInterface::GET_H264_SENDER_PARAMETERS,
				1, bin);
		snd_params = (SkypeVideoRTPInterface::H264SenderParameters *)bin.data();
		test("GetBinaryProperty(1)", ret,
				tv("size", sizeof(SkypeVideoRTPInterface::H264SenderParameters), bin.size()).
				append("profile_idc", 4, snd_params->profile_idc).
				append("profile_iop", 5, snd_params->profile_iop).
				append("level_idc", 6, snd_params->level_idc)
				);

		//bin.Reset();
		bin.reserve(sizeof(SkypeVideoRTPInterface::H264SenderConstraints));
		SkypeVideoRTPInterface::H264SenderConstraints *snd_constr =
					(SkypeVideoRTPInterface::H264SenderConstraints *)bin.data();
		snd_constr->profile_idc = 100;
		snd_constr->profile_iop = (SkypeVideoRTPInterface::H264ProfileIop)101;
		snd_constr->level_idc = 102;
		snd_constr->max_mbps = 103;
		snd_constr->max_fs = 104;
		snd_constr->max_cpb = 105;
		snd_constr->max_dpb = 106;
		snd_constr->max_smbps = 107;
		snd_constr->redundant_pic_cap = 108;
		ret = intf->SetBinaryProperty(
				SkypeVideoRTPInterface::SET_H264_SENDER_CONSTRAINTS,
				0, bin);

		test("SetBinaryProperty", ret);
	} else {
		int snd_param_cnt = 0;
		ret = intf->GetIntProperty(
				SkypeVideoRTPInterface::GET_NUMBER_OF_H264_SENDER_PARAMETERS,
				0, snd_param_cnt);
		t("GetIntProperty", ret,
				tv("snd_param_cnt", 2, snd_param_cnt));

		Sid::Binary bin;
		bin.reserve(sizeof(SkypeVideoRTPInterface::H264SenderParameters));
		ret = intf->GetBinaryProperty(
				(int)SidVideoRTPTypesSkypekit31::GET_H264_SENDER_PARAMETERS,
				0, bin);
		SidVideoRTPTypesSkypekit31::H264SenderParameters *snd_params =
			(SidVideoRTPTypesSkypekit31::H264SenderParameters *)bin.data();
		test("GetBinaryProperty(0)", ret,
				tv("profile_idc", 1, snd_params->profile_idc).
				append("profile_iop", 2, snd_params->profile_iop).
				append("level_idc", 3, snd_params->level_idc).
				append("resolution_caps[0].max_width", 101, snd_params->resolution_caps[0].max_width).
				append("resolution_caps[0].max_height", 102, snd_params->resolution_caps[0].max_height).
				append("resolution_caps[0].min_width", 103, snd_params->resolution_caps[0].min_width).
				append("resolution_caps[0].min_height", 104, snd_params->resolution_caps[0].min_height).
				append("resolution_caps[0].preference", 105, snd_params->resolution_caps[0].preference).
				append("resolution_caps[1].max_width", 201, snd_params->resolution_caps[1].max_width).
				append("resolution_caps[1].max_height", 202, snd_params->resolution_caps[1].max_height).
				append("resolution_caps[1].min_width", 203, snd_params->resolution_caps[1].min_width).
				append("resolution_caps[1].min_height", 204, snd_params->resolution_caps[1].min_height).
				append("resolution_caps[1].preference", 205, snd_params->resolution_caps[1].preference)
				);

		//bin.reset();
		bin.reserve(sizeof(SidVideoRTPTypesSkypekit31::H264SenderParameters));
		ret = intf->GetBinaryProperty(
				(int)SidVideoRTPTypesSkypekit31::GET_H264_SENDER_PARAMETERS,
				1, bin);
		snd_params = (SidVideoRTPTypesSkypekit31::H264SenderParameters *)bin.data();
		test("GetBinaryProperty(1)", ret,
				tv("profile_idc", 4, snd_params->profile_idc).
				append("profile_iop", 5, snd_params->profile_iop).
				append("level_idc", 6, snd_params->level_idc).
				append("resolution_caps[0].max_width", 101, snd_params->resolution_caps[0].max_width).
				append("resolution_caps[0].max_height", 102, snd_params->resolution_caps[0].max_height).
				append("resolution_caps[0].min_width", 103, snd_params->resolution_caps[0].min_width).
				append("resolution_caps[0].min_height", 104, snd_params->resolution_caps[0].min_height).
				append("resolution_caps[0].preference", 105, snd_params->resolution_caps[0].preference).
				append("resolution_caps[1].max_width", 201, snd_params->resolution_caps[1].max_width).
				append("resolution_caps[1].max_height", 202, snd_params->resolution_caps[1].max_height).
				append("resolution_caps[1].min_width", 203, snd_params->resolution_caps[1].min_width).
				append("resolution_caps[1].min_height", 204, snd_params->resolution_caps[1].min_height).
				append("resolution_caps[1].preference", 205, snd_params->resolution_caps[1].preference)
				);

		//bin.reset();
		bin.reserve(sizeof(SidVideoRTPTypesSkypekit31::H264SenderConstraints));
		SidVideoRTPTypesSkypekit31::H264SenderConstraints *snd_constr =
					(SidVideoRTPTypesSkypekit31::H264SenderConstraints *)bin.data();
		snd_constr->profile_idc = 100;
		snd_constr->profile_iop = 101;
		snd_constr->level_idc = 102;
		snd_constr->max_mbps = 103;
		snd_constr->max_fs = 104;
		snd_constr->max_cpb = 105;
		snd_constr->max_dpb = 106;
		snd_constr->max_smbps = 107;
		snd_constr->redundant_pic_cap = 108;
		snd_constr->resolution_caps[0].max_height = 200;
		snd_constr->resolution_caps[0].max_width = 201;
		snd_constr->resolution_caps[0].min_height = 202;
		snd_constr->resolution_caps[0].min_width = 203;
		snd_constr->resolution_caps[0].preference = 204;
		snd_constr->resolution_caps[1].max_height = 300;
		snd_constr->resolution_caps[1].max_width = 301;
		snd_constr->resolution_caps[1].min_height = 302;
		snd_constr->resolution_caps[1].min_width = 303;
		snd_constr->resolution_caps[1].preference = 304;
		ret = intf->SetBinaryProperty(
				(int)SidVideoRTPTypesSkypekit31::SET_H264_SENDER_CONSTRAINTS,
				0, bin);

		test("SetBinaryProperty", ret);
	}


	test("RequestKeyFrame", intf->RequestKeyFrame());
	test("RequestRecoveryFrame", intf->RequestRecoveryFrame(1));
	test("SetBitrate", intf->SetBitrate(11));
	Sid::Binary data((char *)"Packet", 7);
	intf->RtpPacketReceived(data);
	if (version >= 32) {
		intf->FrameReceived(data, 1);
	}
	test("StopPlayback", intf->StopPlayback());
	test("StopRecording", intf->StopRecording());
	test("SetMaxPacketSize", intf->SetMaxPacketSize(1024));
	Sid::String com("command"), resp;
	t("CustomCommand", intf->CustomCommand(com, resp),
			tv("result_str", "response", (const char*)resp));
	SkypeVideoRTPInterface::H264SenderParameters sndpar;

	sndpar.profile_idc = 10;
	sndpar.profile_iop = (SkypeVideoRTPInterface::H264ProfileIop)20;
	sndpar.level_idc = 30;
	test("Uninit", intf->Uninit());

	if (alltestsok)
		INFO(" ====== All tests passed ======");
	else
		INFO(" ====== Some test('s) failed ======");

	conn.Disconnect();
}

uint64_t getUs() {
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

void LoopbackTest(int count, int bitrateKbps, int mtu, int fps, int test_frame) {
	MyTransport myTrans(bitrateKbps);
	MyDisconnectCallback dc;
	Sid::SidVideoRtpIfConnection conn(myTrans, dc);

	bool connected = conn.EnsureConnection();
	if (!connected) {
		ERROR("Connection failed, exit!");
		return;
	}

	SkypeVideoRTPInterface *intf = conn.Interface();
	intf->Init();

	intf->StartRecording(0, 1, 2, 3, 4, 5);

	int frame_size = bitrateKbps * 1000 / 8 / fps;
	Sid::Binary data;
	data.reserve(frame_size);
	INFO("bitrateKbps: %d, mtu: %d, fps: %d, test_frame: %d, frame_size: %d\n", bitrateKbps, mtu, fps, test_frame, frame_size);

	uint64_t startTime = getUs();

	uint64_t sent_size = 0;
	for (int i = 0; i < count; i++) {
		if (test_frame) {
			DBG("send %d\n", data.size());
			intf->FrameReceived(data, i);
			sent_size += frame_size;
		} else {
			int len = frame_size;
			Sid::Binary data;
			while (len > 0) {
				int to_send = len > mtu? mtu:len;
				len -= to_send;
				data.reserve(to_send);
				sent_size += to_send;
				DBG("send %d\n", data.size());
				intf->RtpPacketReceived(data);
			}
		}
		if (bitrateKbps) {
			int expectedUs = sent_size * 8 * 1000 / bitrateKbps;
			int elapsedUs = (int)(getUs() - startTime);
			DBG("exdpectedUs: %d, elapsedUs: %d\n", expectedUs, elapsedUs);
			if (elapsedUs < expectedUs) {
				int sleep_time_us = expectedUs - elapsedUs;
				timespec req = {0, sleep_time_us * 1000}, rem;
				DBG("sleep_time_us: %d", sleep_time_us);
				nanosleep(&req, &rem);
			}
		}
	}

	uint64_t bits_sent = sent_size * 8;
	int millisec = ((getUs() - startTime) / 1000 );
	INFO("count: %d, bits_sent: %lld, millisec: %d, bytes_send: %lld, bytes received: %lld", count, bits_sent, millisec, ((uint64_t)1384 * count), myTrans.m_ReceivedCnt);
	INFO("avg speed: %lld kbps", bits_sent / millisec);
	conn.Disconnect();
}

struct OptionsParser {
	OptionsParser() :
		m_Mode(0),
		m_Count(1000),
		m_Kbps(512),
		m_Mtu(1300),
		m_Fps(30),
		m_Frame(false),
		m_Help(false)
	{}
	void Usage() {
		INFO("usage:");
		INFO("	testprogram --mode=x --count=x --kbps=x --mtu=x --fps=x --frame --help");
		INFO("	mode=1: do basic test");
		INFO("	mode=2: do loopback test. --count --kbps --mtu --fps --frame");
		INFO("	default: mode: 1, count: 1000, kbps: 512");
	}
	int ParseOptions(int argc, const char **argv) {
		int c;

		while (1) {
			int option_index = 0;
			static struct option long_options[] = {
				{ "mode", 1, 0, 0 },
				{ "count", 1, 0, 0 },
				{ "kbps", 1, 0, 0 },
				{ "mtu", 1, 0, 0 },
				{ "fps", 1, 0, 0 },
				{ "frame", 0, 0, 0 },
				{ "help", 0, 0, 0 },
				{ 0, 0, 0, 0 }
			};

			c = getopt_long(argc, (char * const *)argv, "", long_options, &option_index);
			if (c == -1)
				break;

			switch (c) {
			case 0:
				DBG("option %d, %s", option_index, long_options[option_index].name);
				if (optarg)
					DBG(" with arg %s", optarg);

				switch(option_index) {
				case 0:
					m_Mode = atoi(optarg);
					break;
				case 1:
					m_Count = atoi(optarg);
					break;
				case 2:
					m_Kbps = atoi(optarg);
					break;
				case 3:
					m_Mtu = atoi(optarg);
					break;
				case 4:
					m_Fps = atoi(optarg);
					break;
				case 5:
					m_Frame = true;
					break;
				case 6:
					m_Help = 1;
					break;
				default:
					break;
				}
				break;

			default:
				ERROR("?? getopt returned character code 0%o ??\n", c);
			}
		}

		if (!m_Mode || m_Help) {
			Usage();
			return -1;
		}

		return 0;
	}
	int m_Mode;
	int m_Count;
	int m_Kbps;
	int m_Mtu;
	int m_Fps;
	bool m_Frame;
	bool m_Help;
};


int main(const int argc, const char** argv)
{
	OptionsParser p;
	if (p.ParseOptions(argc, argv))
		return -1;

	if (p.m_Mode == 1) {
		BasicTest();
	}  else if (p.m_Mode == 2) {
		LoopbackTest(p.m_Count, p.m_Kbps, p.m_Mtu, p.m_Fps, p.m_Frame);
	} else {
		printf("Invalid test mode\n");
	}
	return 0;
}
