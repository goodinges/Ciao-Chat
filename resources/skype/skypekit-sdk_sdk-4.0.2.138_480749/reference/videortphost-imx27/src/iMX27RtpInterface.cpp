/*
 * Copyright (C) 2010 Skype Technologies S.A. Confidential and proprietary
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained
 * in, relating to, or arising from the internet telephony software of Skype
 * Limited (including its affiliates, "Skype"), including without limitation
 * this source code, Skype API and related material of such software
 * proprietary to Skype and/or its licensors ("IP Rights") are and shall
 * remain the exclusive property of Skype and/or its licensors. The recipient
 * hereby acknowledges and agrees that any unauthorized use of the IP Rights
 * is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and ownership
 * thereof.
 */

#include "iMX27RtpInterface.hpp"

#include "SendVideoWrapper.hpp"
#include "ReceiveVideoWrapper.hpp"
#include "LoggingCtrl.hpp"
#include "BitrateStat.hpp"
#include "rfc3984.hpp"
#include "rtph.hpp"

#define CLEANUP(ptr) if (ptr) {delete ptr; ptr = NULL;}
#define BPS_TO_KBITPS(bps) bps * 8 / 1024

iMX27RtpInterface::iMX27RtpInterface(SkypeVideoRTPInterfaceCb *pCallback) :
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
	RTP_FUNCLOG;
	if (!Configuration::GetCfg()->IsInitialized())
		Configuration::GetCfg()->Init("RTPTestEngine.conf");
	m_PayloadFormat = (PayloadFmtType)Configuration::GetCfg()->GetIntVal("PayloadFormat");
#ifndef MINIMAL_MODE
	if ((Configuration::GetCfg()->GetIntVal("SendPacketsFromAnxbFile") || Configuration::GetCfg()->GetIntVal("SendPacketsFromPcapFile")) && !Configuration::GetCfg()->GetIntVal("RenderVideo")) {
		return;
	}
	vpu_versioninfo vpuVersion;
	RetCode ret = RETCODE_SUCCESS;
	pMutexLock scopedLock(m_VpuMutex); // Scoped lock on VPU mutex

	// Initialize VPU hardware and data structures/resources
	ret = vpu_Init(NULL);
	if (ret != RETCODE_SUCCESS) {
		FATAL("%s: vpu_Init() failed. Return code: %d", ret);
	}

	// Read out the version information running on the system
	ret = vpu_GetVersionInfo(&vpuVersion);
	scopedLock.mutex.Release(); // VPU access - unlock
	if (ret != RETCODE_SUCCESS) {
		if (ret != RETCODE_NOT_INITIALIZED) {
			vpu_UnInit();
		}
		FATAL("vpu_GetVersionInfo() failed. Return code: %d\n", ret);
	}
	RTP_DBG("i.MX%x Silicon Revision: %x.%x", mxc_cpu(), ((mxc_cpu_rev() >> 4) & 0xF), (mxc_cpu_rev() & 0xF));
	RTP_DBG("i.MX%x VPU Firmware version: %d.%d.%d", mxc_cpu(), vpuVersion.fw_major, vpuVersion.fw_minor, vpuVersion.fw_release);
	RTP_DBG("i.MX%x VPU Library version: %d.%d.%d", mxc_cpu(), vpuVersion.lib_major, vpuVersion.lib_minor, vpuVersion.lib_release);
#endif
}

iMX27RtpInterface::~iMX27RtpInterface() {
	RTP_FUNCLOG;
	CLEANUP(m_pSendWrapper);
	CLEANUP(m_pReceiveWrapper);
#ifndef MINIMAL_MODE
	if ((Configuration::GetCfg()->GetIntVal("SendPacketsFromAnxbFile") || Configuration::GetCfg()->GetIntVal("SendPacketsFromPcapFile")) && !Configuration::GetCfg()->GetIntVal("RenderVideo")) {
		return;
	}
	pMutexLock scopedLock(m_VpuMutex);

	// De-initialize VPU hardware and release data structures/resources
	vpu_UnInit();
#endif
}

int iMX27RtpInterface::GetLastError() {
	RTP_FUNCLOG;
	return 0;
}

bool iMX27RtpInterface::Init() {
	RTP_FUNCLOG;
	return true;
}

bool iMX27RtpInterface::Uninit() {
	return true;
}

bool iMX27RtpInterface::GetCodecCount(int& codec_count) {
	RTP_FUNCLOG;
	codec_count = 1;
	return true;
}

bool iMX27RtpInterface::GetCodec(int index, VideoCodec& codec ) {
	RTP_FUNCLOG;
	if ( index != 0 )
		return false;
	codec.fourcc = MAKEFOURCC('H', '2', '6', '4');
	codec.recording_capable = 1;
	codec.payload_format = m_PayloadFormat;

	return true;
}

bool iMX27RtpInterface::GetSenderResolutionCount(int& res_cap_count) {
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

bool iMX27RtpInterface::GetSenderResolution(int index, FixedRes& resolution) {
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
	default:
		return false;
	}
    return true;
}

bool iMX27RtpInterface::GetIntProperty(int prop_id, int index, int& value) {
	RTP_FUNCLOG;
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

bool iMX27RtpInterface::GetReceiverResolutionCount(int& resolution_count) {
	if (Configuration::GetCfg()->GetIntVal("RenderVideo")) {
		resolution_count = 3;
	} else {
		resolution_count = 4;
	}
    return true;
}

bool iMX27RtpInterface::GetReceiverResolution(int index, ResCap& resolution) {
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

bool iMX27RtpInterface::SetIntProperty(int prop_id, int index, int value) {
	RTP_FUNCLOG;
	return false;
}

bool iMX27RtpInterface::GetStringProperty(int prop_id, int index, Sid::String& value) {
	RTP_FUNCLOG;
	return false;
}

bool iMX27RtpInterface::SetStringProperty(int prop_id, int index, const Sid::String& value) {
	RTP_FUNCLOG;
	return false;
}

bool iMX27RtpInterface::GetBinaryProperty(int prop_id, int index, Sid::Binary & value) {
	RTP_FUNCLOG;

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

bool iMX27RtpInterface::SetBinaryProperty(int prop_id, int index, const Sid::Binary & value) {
	RTP_FUNCLOG;
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

bool iMX27RtpInterface::RequestKeyFrame() {
	RTP_FUNCLOG;

	m_KeyFrameRequested = 1;

	if (m_pSendWrapper)
		m_pSendWrapper->RequestKeyFrame();

	return true;
}

bool iMX27RtpInterface::RequestRecoveryFrame(int reserved) {
	RTP_FUNCLOG;
	return true;
}

bool iMX27RtpInterface::SetFramerate(int fps) {
	RTP_FUNCLOG;

	bool doSetFramerate = Configuration::GetCfg()->GetIntVal("EnableSendSetFramerate") != 0;
	const char* actionDesc = doSetFramerate ?
		"" :
		" (ignored due to option)";

	RTP_DBG("Set framerate: %d fps%s", fps, actionDesc);
	if (!doSetFramerate)
	{
		return true;
	}
	if (m_pSendWrapper)
		m_pSendWrapper->SetFramerate(fps);
	return true;
}

bool iMX27RtpInterface::SetMaxPacketSize(int maxPacketSize) {
	RTP_FUNCLOG;

	RTP_DBG("SetMaxPacketSize: %d bytes/sec", maxPacketSize);
	if (m_pSendWrapper)
		m_pSendWrapper->SetMaxPacketSize(maxPacketSize);
	return true;
}

bool iMX27RtpInterface::SetBitrate(int bitrate) {
	RTP_FUNCLOG;

	if (Configuration::GetCfg()->GetIntVal("ForceSendBitrate"))
		return true;

	if (Configuration::GetCfg()->GetIntVal("LimitSendBitrate") && Configuration::GetCfg()->GetIntVal("LimitSendBitrate") < BPS_TO_KBITPS(bitrate)) {
		RTP_DBG("Set bitrate %d bytes/sec (%d kbit/s), limiting to: %d kbit/s",  bitrate, BPS_TO_KBITPS(bitrate), Configuration::GetCfg()->GetIntVal("LimitSendBitrate"));
		m_BitRate = Configuration::GetCfg()->GetIntVal("LimitSendBitrate");
	} else {
		RTP_DBG("Set bitrate: %d bytes/sec (%d kbit/s)", bitrate, BPS_TO_KBITPS(bitrate));
		m_BitRate = BPS_TO_KBITPS(bitrate);
	}

	if (m_pSendWrapper)
		m_pSendWrapper->SetBitrate(m_BitRate);

	return true;
}

bool iMX27RtpInterface::StartPlayback(int decoderfourcc, int width, int height) {
	RTP_FUNCLOG;
	VideoParams vidPar;
	char devName[] = "/dev/video16";

	if (decoderfourcc != MAKEFOURCC('H', '2', '6', '4')) {
		return false;
	}

	RTP_DBG("StartPlayback: width: %d heigth: %d", width, height);

	strncpy(vidPar.devName, devName, strlen(devName) + 1);
	vidPar.width = width;
	vidPar.height = height;
	vidPar.pixFmt = V4L2_PIX_FMT_YUV420;
	vidPar.mutex = &m_VpuMutex;

	m_pReceiveWrapper = new ReceiveVideoWrapper(vidPar);

	m_ReceiveBStat.Reset();
	return true;
}

void iMX27RtpInterface::RtpPacketReceived(const Sid::Binary & data) {
	PACKET_DBG("Received rpt packet, len %d, count", data.size(), m_ReceivePcnt);

	m_ReceivePcnt++;
	m_ReceiveBStat.Update(data.size());
	if (m_ReceiveBStat.IsValid())
		RTP_DBG("Receive bitrate: %s", m_ReceiveBStat.GetStatStr());

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

void iMX27RtpInterface::FrameReceived(const Sid::Binary & data, int timestamp_ms) {
	PACKET_DBG("Received frame, len %d, count", data.size(), m_ReceiveFcnt);
	m_ReceiveFcnt++;

	m_ReceiveBStat.Update(data.getSize());
	if (m_ReceiveBStat.IsValid())
		RTP_DBG("Receive bitrate: %s", m_ReceiveBStat.GetStatStr());

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

bool iMX27RtpInterface::StopPlayback() {
	RTP_FUNCLOG;

	CLEANUP(m_pReceiveWrapper);

	return true;
}

bool iMX27RtpInterface::StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps) {
	RTP_FUNCLOG;

	if (fourcc != MAKEFOURCC('H', '2', '6', '4')) {
		return false;
	}
	m_RecordingStarted = true;

	RTP_DBG("StartRecording - mtu: %d, kbps: %d, size: %dx%d, framerate: %d, format: %s", initial_max_packet_size, BPS_TO_KBITPS(initial_bitrate_bytes_sec), width, height, initial_framerate_fps, m_PayloadFormat == FORMAT_FRAMES? "frames": "rtp");

	VideoParams vidPar;
	vidPar.width = width;
	vidPar.height = height;
	vidPar.framerate = Configuration::GetCfg()->GetIntVal("ReceiveInitialFPS");
	if (Configuration::GetCfg()->GetIntVal("ForceSendBitrate")) {
		RTP_DBG("StartRecording %d bytes/sec (%d kbit/s), forcing to: %d kbit/s",  initial_bitrate_bytes_sec, BPS_TO_KBITPS(initial_bitrate_bytes_sec), Configuration::GetCfg()->GetIntVal("ForceSendBitrate"));
		m_BitRate = Configuration::GetCfg()->GetIntVal("ForceSendBitrate");
	} else if (Configuration::GetCfg()->GetIntVal("LimitSendBitrate") && Configuration::GetCfg()->GetIntVal("LimitSendBitrate") < BPS_TO_KBITPS(initial_bitrate_bytes_sec)) {
		RTP_DBG("StartRecording %d bytes/sec (%d kbit/s), limiting to: %d kbit/s",  initial_bitrate_bytes_sec, BPS_TO_KBITPS(initial_bitrate_bytes_sec), Configuration::GetCfg()->GetIntVal("LimitSendBitrate"));
		m_BitRate = Configuration::GetCfg()->GetIntVal("LimitSendBitrate");
	} else {
		RTP_DBG("StartRecording: %d bytes/sec (%d kbit/s)", initial_bitrate_bytes_sec, BPS_TO_KBITPS(initial_bitrate_bytes_sec));
		m_BitRate = BPS_TO_KBITPS(initial_bitrate_bytes_sec);
	}

	vidPar.bitrate = m_BitRate;
	vidPar.pixFmt = V4L2_PIX_FMT_YUV420;
	vidPar.mutex = &m_VpuMutex;

	m_pSendWrapper = new SendVideoWrapper(*this, initial_max_packet_size, vidPar);

	m_SendBStat.Reset();	
	return true;
}

void iMX27RtpInterface::TransportSendFrame(VideoData& frame) {
	Sid::Binary binary((char *)frame.Data(), frame.Size());
	m_pCb->SendFrame(binary, frame.GetTimestamp());

	m_SendFcnt++;
	m_SendBStat.Update(frame.Size());
	if (m_SendBStat.IsValid())
		RTP_DBG("Send bitrate: %s", m_SendBStat.GetStatStr());

	PACKET_DBG("TransportSendFrame: packet len %d bytes, count: %d", frame.Size(), m_SendFcnt);
}

void iMX27RtpInterface::TransportSendRTPPacket(const void *data, int len) {
	Sid::Binary binary((char *)data, len);
	m_pCb->SendRTPPacket(binary);

	m_SendPcnt++;
	m_SendBStat.Update(len);
	if (m_SendBStat.IsValid())
		RTP_DBG("Send bitrate: %s", m_SendBStat.GetStatStr());

	PACKET_DBG("From Anxb->RTP: packet len %d bytes, count: %d", len, m_SendPcnt);
}

SkypeVideoRTPInterface::PayloadFmtType iMX27RtpInterface::GetPayloadFmt() {
	return m_PayloadFormat;
}

bool iMX27RtpInterface::StopRecording() {
	RTP_FUNCLOG;

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

bool iMX27RtpInterface::CustomCommand(const Sid::String& cmd, Sid::String& result_str) {
	RTP_FUNCLOG;
	result_str = "Hi! I got your ";
	result_str += cmd;
	RTP_DBG("CustomCommand: got %s, answer: %s", cmd.data(), result_str.data());
	return true;
}

bool iMX27RtpInterface::VideoCallStart() {
	RTP_FUNCLOG;
	return true;
}

bool iMX27RtpInterface::VideoCallEnd() {
	RTP_FUNCLOG;
	return true;
}

SkypeVideoRTPInterface * SkypeVideoRTPInterfaceGet(SkypeVideoRTPInterfaceCb *callback)
{
	RTP_FUNCLOG;
	return new iMX27RtpInterface(callback);
}

void SkypeVideoRTPInterfaceRelease(SkypeVideoRTPInterface* rtpvideo)
{
	RTP_FUNCLOG;
	delete rtpvideo;
}
