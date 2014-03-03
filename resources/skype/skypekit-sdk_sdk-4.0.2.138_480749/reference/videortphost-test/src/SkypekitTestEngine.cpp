
#include "SkypekitTestEngine.hpp"

#include "SendVideoWrapper.hpp"
#include "ReceiveVideoWrapper.hpp"
#include "RtpTeLogging.hpp"
#include "BitrateStat.hpp"
#include "rfc3984.hpp"
#include "rtph.hpp"

#define CLEANUP(ptr) if (ptr) { delete ptr; ptr = NULL;}
#define BPS_TO_KBITPS(bps) bps * 8 / 1024

SkypekitTestEngine::SkypekitTestEngine(SkypeVideoRTPInterfaceCb *pCallback) :
	m_BitRate(0),
	m_pCb(pCallback),
	m_ReceiveBStat(3000000),
	m_SendBStat(3000000),
    m_KeyFrameRequested(0),
	m_RecordingStarted(false),
	m_ReceivePcnt(0),
	m_SendPcnt(0),
	m_ReceiveFcnt(0),
	m_SendFcnt(0),
	m_PayloadFormat(FORMAT_FRAMES),
	m_pSendWrapper(NULL),
	m_pReceiveWrapper(NULL)
{
	TI_FUNCLOG;
	if (!Configuration::GetCfg()->IsInitialized())
		Configuration::GetCfg()->Init("RTPTestEngine.conf");
	m_PayloadFormat = (PayloadFmtType)Configuration::GetCfg()->GetIntVal("PayloadFormat");
}

SkypekitTestEngine::~SkypekitTestEngine()
{
	TI_FUNCLOG;
	CLEANUP(m_pSendWrapper);
	CLEANUP(m_pReceiveWrapper);
}
	
int SkypekitTestEngine::GetLastError() {
	TI_FUNCLOG;
	return 0;
}	
	
bool SkypekitTestEngine::Init() { 
	TI_FUNCLOG;
	return true;
}
	
bool SkypekitTestEngine::Uninit() {
	return true;
}
	
bool SkypekitTestEngine::GetCodecCount( int &codec_count ) {
	TI_FUNCLOG;
	codec_count = 1;
	return true;
}

bool SkypekitTestEngine::GetCodec(int index, VideoCodec &codec ) {
	TI_FUNCLOG;
	if ( index != 0 )
		return false;
	codec.fourcc = MAKEFOURCC('H', '2', '6', '4');
	codec.recording_capable = 1;
	codec.payload_format = m_PayloadFormat;

	return true;
}	

bool SkypekitTestEngine::GetSenderResolutionCount(int &res_cap_count) {
	if (Configuration::GetCfg()->GetIntVal("SendVGAOnly")) {
    	res_cap_count = 1;
    	return true;
    }

	if (Configuration::GetCfg()->GetIntVal("SendPacketsFromAnxbFile")) {
		res_cap_count = 4;
	} else {
		res_cap_count = 3;
	}
	return true;
}

bool SkypekitTestEngine::GetSenderResolution(int index, FixedRes & resolution) {
	switch (index) {
	case 0:
	    if (Configuration::GetCfg()->GetIntVal("SendVGAOnly")) {
	    	resolution.width = 640;
	    	resolution.height = 480;
	    	resolution.preference = 0;
	    } else {
	    	resolution.width = 160;
	    	resolution.height = 120;
	    	resolution.preference = 1;
	    }
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
	case 3:
		if (Configuration::GetCfg()->GetIntVal("SendPacketsFromAnxbFile")) {
	    	resolution.width = 1280;
	    	resolution.height = 720;
	    	resolution.preference = 4;
		} else {
			return false;
		}
        break;
	default:
		return false;
	}
    return true;
}

bool SkypekitTestEngine::GetReceiverResolutionCount(int &resolution_count) {
	resolution_count = 4;
    return true;
}

bool SkypekitTestEngine::GetReceiverResolution(int index, ResCap & resolution) {
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
	case 3:
    	resolution.min_width = 1280;
    	resolution.min_height = 720;
    	resolution.max_width = 1280;
    	resolution.max_height = 720;
    	resolution.preference = 4;
		break;
	default:
		return false;
	}
    return true;
}

bool SkypekitTestEngine::GetIntProperty(int prop_id, int index, int &value) {
	TI_FUNCLOG;
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

bool SkypekitTestEngine::SetIntProperty(int prop_id, int index, int value) {
	TI_FUNCLOG;
	return false;
}

bool SkypekitTestEngine::GetStringProperty(int prop_id, int index, Sid::String &value) {
	TI_FUNCLOG;
	return false;
}

bool SkypekitTestEngine::SetStringProperty(int prop_id, int index, const Sid::String &value) {
	TI_FUNCLOG;
	return false;
}

bool SkypekitTestEngine::GetBinaryProperty(int prop_id, int index, Sid::Binary &value) {
	TI_FUNCLOG;

	bool ret = false;
	switch (prop_id) {
	case GET_H264_SENDER_PARAMETERS:
	{
		value.reserve(sizeof(H264SenderParameters));
		H264SenderParameters *param = (H264SenderParameters *)value.data();
		if (index == 0) {
			memset(param, 0, sizeof(H264SenderParameters));
			param->profile_idc = 66;
			param->profile_iop = (H264ProfileIop)(PROFILE_IOP_CONSTRAINTS_SET0 |
					PROFILE_IOP_CONSTRAINTS_SET1 |
					PROFILE_IOP_CONSTRAINTS_SET2);
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
			param->profile_iop = (H264ProfileIop)(PROFILE_IOP_CONSTRAINTS_SET0 |
					PROFILE_IOP_CONSTRAINTS_SET1 |
					PROFILE_IOP_CONSTRAINTS_SET2);
			param->level_idc = 30;
			param->max_br = Configuration::GetCfg()->GetIntVal("LimitReceiveBitrate");
			ret = true;
		}
		break;
	}
	default:
		break;
	}
	return ret;
}

bool SkypekitTestEngine::SetBinaryProperty(int prop_id, int index, const Sid::Binary &value) {
	TI_FUNCLOG;
	bool ret = false;
	switch (prop_id) {
	case SET_H264_SENDER_CONSTRAINTS:
	{
		H264SenderConstraints *snd_constr = (H264SenderConstraints *)((const char *)value);
		if (index != 0)
			break;
		if (value.size() != sizeof(H264SenderConstraints)) {
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
	
bool SkypekitTestEngine::RequestKeyFrame() {
	TI_FUNCLOG;

	m_KeyFrameRequested = 1;

	if (m_pSendWrapper)
		m_pSendWrapper->RequestKeyFrame();

	return true;
}
	
bool SkypekitTestEngine::RequestRecoveryFrame(int reserved) {
	TI_FUNCLOG;
	return true;
}

bool SkypekitTestEngine::SetFramerate(int fps) {
	TI_FUNCLOG;
	if (Configuration::GetCfg()->GetIntVal("ForceSendFramerate")) {
		TI_DBG("Ignored due to ForceSendFramerate: %d", Configuration::GetCfg()->GetIntVal("ForceSendFramerate"));
		return true;
	}

	TI_DBG("Set framerate: %d fps", fps);
	if (m_pSendWrapper)
		m_pSendWrapper->SetFramerate(fps);
	return true;
}

bool SkypekitTestEngine::SetMaxPacketSize(int maxPacketSize) {
	TI_FUNCLOG;

	TI_DBG("SetMaxPacketSize: %d bytes/sec", maxPacketSize);
	if (m_pSendWrapper)
		m_pSendWrapper->SetMaxPacketSize(maxPacketSize);
	return true;
}
	
bool SkypekitTestEngine::SetBitrate(int bitrate) {
	TI_FUNCLOG;

	if (Configuration::GetCfg()->GetIntVal("ForceSendBitrate")) {
		TI_DBG("Ignored due to ForceSendBitrate: %d", Configuration::GetCfg()->GetIntVal("ForceSendBitrate"));
		return true;
	}

	if (Configuration::GetCfg()->GetIntVal("LimitSendBitrate") && Configuration::GetCfg()->GetIntVal("LimitSendBitrate") < BPS_TO_KBITPS(bitrate)) {
		TI_DBG("Set bitrate %d bytes/sec (%d kbit/s), limiting to: %d kbit/s",  bitrate, BPS_TO_KBITPS(bitrate), Configuration::GetCfg()->GetIntVal("LimitSendBitrate"));
		m_BitRate = Configuration::GetCfg()->GetIntVal("LimitSendBitrate");
	} else {
		TI_DBG("Set bitrate: %d bytes/sec (%d kbit/s)", bitrate, BPS_TO_KBITPS(bitrate));
		m_BitRate = BPS_TO_KBITPS(bitrate);
	}

	if (m_pSendWrapper)
		m_pSendWrapper->SetBitrate(m_BitRate);

	return true;
}
	
bool SkypekitTestEngine::StartPlayback(int decoderfourcc, int width, int height) {
	TI_FUNCLOG;
	VideoParams vidPar;

	if (decoderfourcc != MAKEFOURCC('H', '2', '6', '4')) {
		return false;
	}

	TI_DBG("StartPlayback: width: %d heigth: %d", width, height);

	vidPar.width = width;
	vidPar.height = height;
	vidPar.pixFmt = PIX_FMT_YUV420;

	m_pReceiveWrapper = new ReceiveVideoWrapper(vidPar);

	m_ReceiveBStat.Reset();
	return true;
}
	
void SkypekitTestEngine::RtpPacketReceived(const Sid::Binary &data) {
	PACKET_DBG("Received rpt packet, len %d, count", data.size(), m_ReceivePcnt);

	m_ReceivePcnt++;
	m_ReceiveBStat.Update(data.size());
	if (m_ReceiveBStat.IsValid())
		TI_DBG("Receive bitrate: %s", m_ReceiveBStat.GetStatStr());

	if (Configuration::GetCfg()->GetIntVal("UseLoopback")) {
		// if keyframe was requested then pretend that we send keyframe - otherwise the videolib keeps requesting the keyframes
		if (m_KeyFrameRequested) {
			((NalHeader *)((rtph *)(const char *)data)->GetDataPtr())->SetNri(3);
			if (((rtph *)(const char *)data)->GetMarker())
				m_KeyFrameRequested = 0;
		}
		if (m_RecordingStarted) {
			TransportSendRTPPacket((const char *)data, data.size());
		}

		return;
	}

	if (m_pReceiveWrapper) {
		m_pReceiveWrapper->SubmitPacket((uint8_t *)data.data(), data.getSize());
	}
	return;
}

void SkypekitTestEngine::FrameReceived(const Sid::Binary &data, int timestamp_ms) {
	PACKET_DBG("Received frame, len %d, count", data.size(), m_ReceiveFcnt);
	m_ReceiveFcnt++;

	m_ReceiveBStat.Update(data.getSize());
	if (m_ReceiveBStat.IsValid())
		TI_DBG("Receive bitrate: %s", m_ReceiveBStat.GetStatStr());

	if (Configuration::GetCfg()->GetIntVal("UseLoopback")) {
		if (m_RecordingStarted) {
			VideoData frame;
			frame.SetBuf((const uint8_t*)data.data(), data.size());
			frame.SetTimestamp(timestamp_ms);
			TransportSendFrame(frame);
		}
		return;
	}

	if (m_pReceiveWrapper) {
		VideoData frame;
		frame.SetBuf((uint8_t *)data.data(), data.size());
		frame.SetTimestamp(timestamp_ms);
		m_pReceiveWrapper->SubmitFrame(frame);
	}
	return;
}


bool SkypekitTestEngine::StopPlayback() {
	TI_FUNCLOG;

	CLEANUP(m_pReceiveWrapper);

	return true;
}
	
bool SkypekitTestEngine::StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps) {
	TI_FUNCLOG;

	if (fourcc != MAKEFOURCC('H', '2', '6', '4')) {
		return false;
	}
	m_RecordingStarted = true;

	TI_DBG("StartRecording - mtu: %d, kbps: %d, size: %dx%d, framerate: %d, format: %s", initial_max_packet_size, BPS_TO_KBITPS(initial_bitrate_bytes_sec), width, height, initial_framerate_fps, m_PayloadFormat == FORMAT_FRAMES? "frames": "rtp");

	VideoParams vidPar;
	vidPar.width = width;
	vidPar.height = height;
	if (Configuration::GetCfg()->GetIntVal("ForceSendFramerate"))
		vidPar.framerate = Configuration::GetCfg()->GetIntVal("ForceSendFramerate");
	else
		vidPar.framerate = initial_framerate_fps;
	if (Configuration::GetCfg()->GetIntVal("ForceSendBitrate")) {
		TI_DBG("StartRecording %d bytes/sec (%d kbit/s), forcing to: %d kbit/s",  initial_bitrate_bytes_sec, BPS_TO_KBITPS(initial_bitrate_bytes_sec), Configuration::GetCfg()->GetIntVal("ForceSendBitrate"));
		m_BitRate = Configuration::GetCfg()->GetIntVal("ForceSendBitrate");
	} else if (Configuration::GetCfg()->GetIntVal("LimitSendBitrate") && Configuration::GetCfg()->GetIntVal("LimitSendBitrate") < BPS_TO_KBITPS(initial_bitrate_bytes_sec)) {
		TI_DBG("StartRecording %d bytes/sec (%d kbit/s), limiting to: %d kbit/s",  initial_bitrate_bytes_sec, BPS_TO_KBITPS(initial_bitrate_bytes_sec), Configuration::GetCfg()->GetIntVal("LimitSendBitrate"));
		m_BitRate = Configuration::GetCfg()->GetIntVal("LimitSendBitrate");
	} else {
		TI_DBG("StartRecording: %d bytes/sec (%d kbit/s)", initial_bitrate_bytes_sec, BPS_TO_KBITPS(initial_bitrate_bytes_sec));
		m_BitRate = BPS_TO_KBITPS(initial_bitrate_bytes_sec);
	}

	vidPar.bitrate = m_BitRate;
	vidPar.pixFmt = PIX_FMT_YUV420;

	m_pSendWrapper = new SendVideoWrapper(*this, initial_max_packet_size, vidPar);

	m_SendBStat.Reset();	
	return true;
}

void SkypekitTestEngine::TransportSendFrame(VideoData &frame) {
	Sid::Binary binary((char *)frame.Data(), frame.Size());
	m_pCb->SendFrame(binary, frame.GetTimestamp());

	m_SendFcnt++;
	m_SendBStat.Update(frame.Size());
	if (m_SendBStat.IsValid())
		TI_DBG("Send bitrate: %s", m_SendBStat.GetStatStr());

	PACKET_DBG("TransportSendFrame: packet len %d bytes, count: %d", frame.Size(), m_SendFcnt);
}

void SkypekitTestEngine::TransportSendRTPPacket(const void *data, int len) {
	Sid::Binary binary((char *)data, len);
	m_pCb->SendRTPPacket(binary);

	m_SendPcnt++;
	m_SendBStat.Update(len);
	if (m_SendBStat.IsValid())
		TI_DBG("Send bitrate: %s", m_SendBStat.GetStatStr());

	PACKET_DBG("From Anxb->RTP: packet len %d bytes, count: %d", len, m_SendPcnt);
}

SkypeVideoRTPInterface::PayloadFmtType SkypekitTestEngine::GetPayloadFmt() {
	return m_PayloadFormat;
}

bool SkypekitTestEngine::StopRecording() {
	TI_FUNCLOG;

	m_RecordingStarted = false;
	CLEANUP(m_pSendWrapper);

	if (Configuration::GetCfg()->GetIntVal("TogglePayloadFormat")) {
		// to ensure both payload formats get tested we switch between FRAMES and RTP - this is for testing purposes only
		if (m_PayloadFormat == FORMAT_FRAMES) {
			m_PayloadFormat = FORMAT_RTP;
		} else {
			m_PayloadFormat = FORMAT_FRAMES;
		}
	}

	return true;
}
	
bool SkypekitTestEngine::CustomCommand(const Sid::String &cmd, Sid::String &result_str) {
	TI_FUNCLOG;
	result_str = "Hi! I got your ";
	result_str += cmd;
	TI_DBG("CustomCommand: got %s, answer: %s", cmd.data(), result_str.data());
	return true;
}


bool SkypekitTestEngine::VideoCallStart() {
	TI_FUNCLOG;
	return true;
}

bool SkypekitTestEngine::VideoCallEnd() {
	TI_FUNCLOG;
	return true;
}

SkypeVideoRTPInterface * SkypeVideoRTPInterfaceGet(SkypeVideoRTPInterfaceCb *callback)
{
	TI_FUNCLOG;
	return new SkypekitTestEngine(callback);
}

void SkypeVideoRTPInterfaceRelease(SkypeVideoRTPInterface* rtpvideo)
{
	TI_FUNCLOG;
	delete rtpvideo;
}
