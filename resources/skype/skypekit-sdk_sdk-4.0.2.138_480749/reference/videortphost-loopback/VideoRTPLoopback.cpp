#include <SidVideoRTPInterface.hpp>

#define DBG(fmt, args...) printf(fmt "\n", args)
#define FUNCLOG printf("%s\n", __func__)

struct VideoRTPLoopback :public SkypeVideoRTPInterface {

	VideoRTPLoopback(SkypeVideoRTPInterfaceCb *callback) : m_Callback(callback) {}

	virtual ~VideoRTPLoopback() {
		FUNCLOG;
	}
	
	virtual bool GetVersion(Sid::String & version) {
		FUNCLOG;
		return true;
	}
	
	virtual int GetLastError() { 
		FUNCLOG;
		return 0;
	}	
	
	virtual bool Init() { 
		FUNCLOG;
		return true;
	}
	
	virtual bool Uninit() {
		FUNCLOG;
		return true;
	}
	
	bool GetCodecCount( int &codec_count ) {
		FUNCLOG;
		codec_count = 1;
		return true;
	}

	bool GetCodec(int index, VideoCodec &codec ) {
		FUNCLOG;
		if ( index != 0 )
			return false;
		codec.fourcc = MAKEFOURCC('H', '2', '6', '4');
		codec.recording_capable = 1;
		return true;
	}

	bool GetSenderResolutionCount(int &res_cap_count) {

		res_cap_count = 3;

		return true;
	}

	bool GetSenderResolution(int index, FixedRes & resolution) {
		switch (index) {
		case 0:
			resolution.width = 160;
			resolution.height = 120;
			resolution.preference = 1;
			break;
		case 1:
			resolution.width = 320;
			resolution.height = 240;
			resolution.preference = 2;
			break;
		case 2:
			resolution.width = 640;
			resolution.height = 480;
			resolution.preference = 3;
			break;
		default:
			return false;
		}
		return true;
	}

	bool GetReceiverResolutionCount(int &resolution_count) {
		resolution_count = 3;
		return true;
	}

	bool GetReceiverResolution(int index, ResCap & resolution) {
		switch (index) {
		case 0:
			resolution.min_width = 160;
			resolution.min_height = 120;
			resolution.max_width = 160;
			resolution.max_height = 120;
			resolution.preference = 1;
			break;
		case 1:
			resolution.min_width = 320;
			resolution.min_height = 240;
			resolution.max_width = 320;
			resolution.max_height = 240;
			resolution.preference = 2;
			break;
		case 2:
			resolution.min_width = 640;
			resolution.min_height = 480;
			resolution.max_width = 640;
			resolution.max_height = 480;
			resolution.preference = 3;
			break;
		default:
			return false;
		}
		return true;
	}
	bool GetIntProperty(int prop_id, int index, int &value) {
		FUNCLOG;
		int ret = false;
		if (index != 0)
			return false;

		switch (prop_id) {
		case GET_NUMBER_OF_H264_SENDER_PARAMETERS:
			value = 1;
			ret = true;
			break;
		case GET_NUMBER_OF_H264_RECEIVER_PARAMETERS:
			value = 1;
			ret = true;
			break;
		default:
			break;
		}
		return ret;
	}

	bool SetIntProperty(int prop_id, int index, int value) {
		FUNCLOG;
		return false;
	}

	bool GetStringProperty(int prop_id, int index, Sid::String &value) {
		FUNCLOG;
		return false;
	}

	bool SetStringProperty(int prop_id, int index, const Sid::String &value) {
		FUNCLOG;
		return false;
	}

	bool GetBinaryProperty(int prop_id, int index, Sid::Binary &value) {
		FUNCLOG;
		bool ret = false;
		switch (prop_id) {
		case GET_H264_SENDER_PARAMETERS:
		{
			value.reserve(sizeof(H264SenderParameters));
			H264SenderParameters *param = (H264SenderParameters *)value.data();
			if (index == 0) {
				param->profile_idc = 66;
				param->profile_iop = (H264ProfileIop)0xe0;
				param->level_idc = 30;
				ret = true;
			}
			break;
		}
		case GET_H264_RECEIVER_PARAMETERS:
		{
			value.reserve(sizeof(H264ReceiverParameters));
			H264ReceiverParameters *param = (H264ReceiverParameters *)value.data();
			if (index == 0) {
				memset(param, 0, sizeof(H264ReceiverParameters));
				param->profile_idc = 66;
				param->profile_iop = (H264ProfileIop)0xe0;
				param->level_idc = 30;
				param->max_br = 400;

				ret = true;
			}
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
			H264SenderConstraints *snd_constr = (H264SenderConstraints *)value.data();
			if (index != 0)
				break;
			if (value.getSize() != sizeof(H264SenderConstraints)) {
				printf("value.getSize() != sizeof(H264SenderConstraints)\n");
				break;
			}

			DBG("snd_constr->profile_idc: %d", snd_constr->profile_idc);
			DBG("snd_constr->profile_iop: %d", snd_constr->profile_iop);
			DBG("snd_constr->level_idc: %d", snd_constr->level_idc);
			DBG("snd_constr->max_mbps: %d", snd_constr->max_mbps);
			DBG("snd_constr->max_fs: %d", snd_constr->max_fs);
			DBG("snd_constr->max_cpb: %d", snd_constr->max_cpb);
			DBG("snd_constr->max_dpb: %d", snd_constr->max_dpb);
			DBG("snd_constr->max_br: %d", snd_constr->max_br);
			DBG("snd_constr->max_smbps: %d", snd_constr->max_smbps);
			DBG("snd_constr->redundant_pic_cap: %d", snd_constr->redundant_pic_cap);
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
	
	virtual bool RequestKeyFrame() {
		FUNCLOG;
		return true;
	}
	
	virtual bool RequestRecoveryFrame(int reserved) {
		FUNCLOG;
		return true;
	}

	virtual bool SetFramerate(int fps) {
		FUNCLOG;
		return true;
	}

	virtual bool SetMaxPacketSize(int bytes) {
		FUNCLOG;
		return true;
	}
	
	virtual bool SetBitrate(int bitrate) {
		FUNCLOG;
		return true;
	}
	
	virtual bool StartPlayback(int decoderfourcc, int width, int height) {
		FUNCLOG;
		return true;
	}
	
	virtual void RtpPacketReceived(const Sid::Binary &data) {
		FUNCLOG;
		if (m_Callback)
			m_Callback->SendRTPPacket(data);
		return;
	}

	void FrameReceived(const Sid::Binary &data, int timestamp_ms) {
		FUNCLOG;
		if (m_Callback)
			m_Callback->SendFrame(data, timestamp_ms);

		return;
	}

	virtual bool StopPlayback() {
		FUNCLOG;
		return true;
	}
	
	virtual bool StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps) {
		FUNCLOG;
		return true;
	}
	
	virtual bool StopRecording() {
		FUNCLOG;
		return true;
	}
	
	virtual bool CustomCommand(const Sid::String &cmd, Sid::String &result_str) {
		FUNCLOG;
		return true;
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
	return new VideoRTPLoopback(callback);
}

void SkypeVideoRTPInterfaceRelease(SkypeVideoRTPInterface* rtpvideo)
{
	delete rtpvideo;
}
