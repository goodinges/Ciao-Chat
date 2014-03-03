#include "SidVideoRTPInterface.hpp"

#define DBG(fmt, args...) printf(fmt "\n", ##args)
#define FUNCLOG printf("%s\n", __func__)

struct VideoRTPTest :public SkypeVideoRTPInterface {

	VideoRTPTest(SkypeVideoRTPInterfaceCb *callback) : m_Callback(callback) {
		FUNCLOG;
	}

	~VideoRTPTest() {
		FUNCLOG;
	}

	bool GetVersion(Sid::String & version) {
		FUNCLOG;
		version = "RTPLoopback/Test V1.0"; return true;
	}

	int GetLastError() {
		FUNCLOG;
		return true;
	}

	bool Init() {
		FUNCLOG;
		return true;
	}

	bool Uninit() {
		FUNCLOG;
		return true;
	}

	bool GetCodecCount( int &codec_count ) {
		FUNCLOG;
		codec_count = 2;
		return true;
	}

	bool GetCodec(int index, VideoCodec &codec ) {
		FUNCLOG;
		if ( index != 1 )
			return false;
		codec.fourcc = 1234;
		codec.recording_capable = 1;
		codec.payload_format = FORMAT_FRAMES;
		return true;
	}

	bool GetSenderResolutionCount(int &res_cap_count) {
		FUNCLOG;
		res_cap_count = 1;
		return true;
	}

	bool GetSenderResolution(int index, FixedRes & resolution) {
		FUNCLOG;
		if ( index != 1 )
			return false;
		resolution.width = 640;
		resolution.height = 480;
		resolution.preference = 1;
		return true;
	}

	bool GetReceiverResolutionCount(int &resolution_count) {
		FUNCLOG;
		resolution_count = 1;
		return true;
	}

	bool GetReceiverResolution(int index, ResCap & resolution) {
		FUNCLOG;
		if ( index != 1 )
			return false;
		resolution.max_width = 0x12345678;
		resolution.max_height = 0x9abcdef0;
		resolution.min_width = 0xaabbccdd;
		resolution.min_height = 0xeeff0011;
		resolution.preference = 0x11223344;
		return true;
	}

	bool GetIntProperty(int prop_id, int index, int &value) {
		FUNCLOG;
		int ret = false;
		if (index != 0)
			return false;

		switch (prop_id) {
		case GET_NUMBER_OF_H264_SENDER_PARAMETERS:
			value = 2;
			ret = true;
			break;
		default:
			break;
		}
		return ret;
	}

	bool SetIntProperty(int prop_id, int index, int value) {
		FUNCLOG;
		return true;
	}

	bool GetStringProperty(int prop_id, int index, Sid::String &value) {
		FUNCLOG;
		return true;
	}

	bool SetStringProperty(int prop_id, int index, const Sid::String &value) {
		FUNCLOG;
		return true;
	}

	bool GetBinaryProperty(int prop_id, int index, Sid::Binary &value) {
		FUNCLOG;
		bool ret = false;
		switch (prop_id) {
		case GET_H264_SENDER_PARAMETERS:
		{
			DBG("GET_H264_SENDER_PARAMETERS");
			value.reserve(sizeof(H264SenderParameters));
			H264SenderParameters *param = (H264SenderParameters *)value.data();
			if (index == 0) {
				param->profile_idc = 1;
				param->profile_iop = (H264ProfileIop)2;
				param->level_idc = 3;
			} else {
				param->profile_idc = 4;
				param->profile_iop = (H264ProfileIop)5;
				param->level_idc = 6;
			}
			ret = true;
			break;
		}
		default:
			break;
		}
		return ret;
	}

	bool SetBinaryProperty(int prop_id, int index, const Sid::Binary &value) {
		FUNCLOG;
		bool ret = false;
		switch (prop_id) {
		case SET_H264_SENDER_CONSTRAINTS:
		{
			H264SenderConstraints *snd_constr = (H264SenderConstraints *)((const char*)value);
			if (index != 0)
				break;
			if (value.size() != sizeof(H264SenderConstraints)) {
				printf("value.getSize() != sizeof(H264SenderConstraints) %d %d\n", value.size(), sizeof(H264SenderConstraints));
				break;
			}
			if (snd_constr->profile_idc != 100)
				break;
			if (snd_constr->profile_iop != 101)
				break;
			if (snd_constr->level_idc != 102)
				break;
			if (snd_constr->max_mbps != 103)
				break;
			if (snd_constr->max_fs != 104)
				break;
			if (snd_constr->max_cpb != 105)
				break;
			if (snd_constr->max_dpb != 106)
				break;
			if (snd_constr->max_smbps != 107)
				break;
			if (snd_constr->redundant_pic_cap != 108)
				break;
			ret = true;
		}
		default:
			break;
		}

		if (!ret) {
			switch (prop_id) {
			case SET_H264_SENDER_CONSTRAINTS:
				printf("H264SenderConstraints struct broken!");
				break;
			default:
				printf("unknown property set");
				break;
			}
		}
		return ret;
	}

	bool RequestKeyFrame() {
		FUNCLOG;
		return true;
	}

	bool RequestRecoveryFrame( int reserved ) {
		FUNCLOG;
		if ( reserved == 1 )
			return true;
		else
			return false;
	}

	bool SetBitrate(int bitrate) {
		FUNCLOG;
		if ( bitrate == 11 )
			return true;
		else
			return false;
	}

	bool SetFramerate(int framerate_fps) {
		FUNCLOG;
		if ( framerate_fps	== 30 )
			return true;
		else
			return false;
	}

	bool SetMaxPacketSize( int max_packet_size ) {
		if ( max_packet_size != 1024 )
			return false;
		return true;
	}

	bool StartPlayback(int fourcc, int width, int height) {
		FUNCLOG;
		if ( fourcc != 0 )
			return false;
		if ( width != 1 )
			return false;
		if ( height != 2 )
			return false;
		return true;
	}

	void RtpPacketReceived(const Sid::Binary &data) {
		//FUNCLOG;

		m_Callback->SendRTPPacket(data);
	}
	void FrameReceived(const Sid::Binary &data, int timestamp_ms) {
		//FUNCLOG;

		m_Callback->SendFrame(data, timestamp_ms);
	}

	bool StopPlayback() {
		FUNCLOG;
		return true;
	}

	bool StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps) {
		FUNCLOG;

		if ( fourcc != 1234 )
			return false;
		if ( initial_max_packet_size != 10000 )
			return false;
		if ( initial_bitrate_bytes_sec != 10200 )
			return false;
		if ( width != 10300 )
			return false;
		if ( height != 10400 )
			return false;
		width = 2468;
		width = 3579;

		return true;
	}

	bool StopRecording() {
		FUNCLOG;
		return true;
	}

	bool CustomCommand(const Sid::String &cmd, Sid::String &result_str) {
		if ( cmd == "command" ) {
			result_str = "response";
			return true;
		}
		return false;
	}

	bool VideoCallStart() {
		return true;
	}

	bool VideoCallEnd() {
		return true;
	}

	SkypeVideoRTPInterfaceCb *m_Callback;
};

SkypeVideoRTPInterface * SkypeVideoRTPInterfaceGet(SkypeVideoRTPInterfaceCb *callback)
{
	FUNCLOG;
	return new VideoRTPTest(callback);
}

void SkypeVideoRTPInterfaceRelease(SkypeVideoRTPInterface* rtpvideo)
{
	FUNCLOG;
	delete rtpvideo;
}
