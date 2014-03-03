/*
 * Copyright (C) 2007-2009 Skype Technologies S.A. Confidential and proprietary
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
#include "SendVideoWrapper.hpp"
#include "RtpTeLogging.hpp"
#include "PacketDump.hpp"
#include "H264AnxbFileReader.hpp"
#include "PcapFileReader.hpp"

#ifndef MINIMAL_MODE
#include "V4l2Capturer.hpp"
#include "DummyCapturer.hpp"
#include "H264Encoder.hpp"
#include "X11Renderer.hpp"
#endif

void VideoEncCallback::TransmitFrame(VideoData &frame) {
	frame_cnt++;
	PACKET_DBG("From encoder: Frame length %d bytes, frame_cnt: %d", frame.Size(), frame_cnt);

	m_pSend.m_SendFStat.Update();
	if (m_pSend.m_SendFStat.IsValid())
		TI_DBG("Send Framerate: %s", m_pSend.m_SendFStat.GetStatStr());

	/* If sending packets from pcap file then the "encoder" output is rtp packets - no need to pass them to m_AnxbToRtp */
	if (Configuration::GetCfg()->GetIntVal("SendPacketsFromPcapFile")) {
		m_pSend.m_pSendCallback.TransportSendRTPPacket(frame.Data(), frame.Size());
	} else {
		if (Configuration::GetCfg()->GetIntVal("DumpSendAnxbPackets")) {
			m_pSend.m_pSendAnxbDump->StorePacket(frame.Data(), frame.Size());
		}

		if (m_pSend.m_pSendCallback.GetPayloadFmt() == SkypeVideoRTPInterface::FORMAT_FRAMES) {
			m_pSend.m_pSendCallback.TransportSendFrame(frame);
		} else {
			m_pSend.m_AnxbToRtp.AnxbFrameToRtp(frame.Data(), frame.Size(), frame.GetTimestamp(), true);
		}
	}
}

void H264AnxbToRtpCb::TransmitRtpPacket(const uint8_t *data, uint32_t len) {
	if (Configuration::GetCfg()->GetIntVal("DumpSendRtpPackets")) {
		m_pSend.m_pSendRtpDump->StorePacket(data, len);
	}	

	m_pSend.m_pSendCallback.TransportSendRTPPacket(data, len);
}

#define CLEANUP(ptr) if (ptr) { delete ptr; ptr = NULL;}

SendVideoWrapper::SendVideoWrapper(SendPacketCallback &sendCb,
		int initialMaxPacketSize, VideoParams &vidPar) :
	m_pSendCallback(sendCb),
	m_VidEncCb(*this),
	m_AnxbToRtpCb(*this),
	m_AnxbToRtp(&m_AnxbToRtpCb),
	m_SendFStat(3000000),
	m_pSendRtpDump(NULL),
	m_pSendAnxbDump(NULL),
	m_pCapturer(NULL),
	m_pEncoder(NULL),
	m_pPreview(NULL)
{
	m_AnxbToRtp.SetMaxRtpPacketLen(initialMaxPacketSize);

	if (Configuration::GetCfg()->GetIntVal("DumpSendRtpPackets")) {
		char filename[100];
		sprintf(filename, "sent_rtp_packets%dx%d.pcap", vidPar.width, vidPar.height);
		m_pSendRtpDump = new PacketDump(filename, PDUMP_FMT_PCAP);
	}
	
	if (Configuration::GetCfg()->GetIntVal("DumpSendAnxbPackets")) {
		char filename[100];
		sprintf(filename, "sent_anxb_packets%dx%d.264", vidPar.width, vidPar.height);
		m_pSendAnxbDump = new PacketDump(filename, PDUMP_FMT_RAW);
	}	
	
	if (Configuration::GetCfg()->GetIntVal("SendPacketsFromAnxbFile")) {
		strncpy(vidPar.devName, Configuration::GetCfg()->GetStrVal("SendAnxbFileDir"), sizeof(vidPar.devName));
		m_pEncoder = new H264AnxbFileReader(&vidPar, NULL, &m_VidEncCb);
		m_pEncoder->StartEncoding();

	} else if (Configuration::GetCfg()->GetIntVal("SendPacketsFromPcapFile")) {
		strncpy(vidPar.devName, Configuration::GetCfg()->GetStrVal("SendPcapFilename"), sizeof(vidPar.devName));
		m_pEncoder = new PcapFileReader(&vidPar, NULL, &m_VidEncCb);
		m_pEncoder->StartEncoding();

	} else if (Configuration::GetCfg()->GetIntVal("SendVideoFromCamera")) {
#ifndef MINIMAL_MODE
		strncpy(vidPar.devName, Configuration::GetCfg()->GetStrVal("VideoCameraDevName"), sizeof(vidPar.devName));
		m_pPreview = new X11Renderer(&vidPar, "Preview", 150, 150);
		if (Configuration::GetCfg()->GetIntVal("UseDummyCapturer")) {
			m_pCapturer = new DummyCapturer(&vidPar, m_pPreview);
		} else {
			m_pCapturer = new V4l2Capturer(&vidPar, m_pPreview);
		}
		m_pEncoder = new H264Encoder(&vidPar, m_pCapturer, &m_VidEncCb);
		m_pEncoder->StartEncoding();
#endif
	}	
}

SendVideoWrapper::~SendVideoWrapper() {
	if (m_pEncoder)
		m_pEncoder->StopEncoding();

	CLEANUP(m_pSendRtpDump);
	CLEANUP(m_pSendAnxbDump);
	CLEANUP(m_pCapturer);
	CLEANUP(m_pEncoder);
	CLEANUP(m_pPreview);
}

void SendVideoWrapper::SetBitrate(uint32_t bitrate) {

	if (m_pEncoder)
		m_pEncoder->ChangeBitrate(bitrate);
}

void SendVideoWrapper::SetFramerate(uint32_t framerate) {

	if (m_pEncoder)
		m_pEncoder->ChangeFramerate(framerate);
	if (m_pCapturer)
		m_pCapturer->ChangeFramerate(framerate);
}


void SendVideoWrapper::SetMaxPacketSize(uint32_t maxPacketSize) {

	m_AnxbToRtp.SetMaxRtpPacketLen(maxPacketSize);
}

void SendVideoWrapper::RequestKeyFrame() {
	if (m_pEncoder)
		m_pEncoder->GenerateKeyFrame();
}
