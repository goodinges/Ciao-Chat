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

#include "SendVideoWrapper.hpp"
#include "LoggingCtrl.hpp"

#define CLEANUP(ptr) if (ptr) {delete ptr; ptr = NULL;}

void VideoEncCallback::TransmitFrame(VideoData& frame) {
	NalHeader* nalHdr = NULL;
	bool gotFrame = false;

	m_FrameCnt++;
	PACKET_DBG("From encoder: Frame length %d bytes, frame_cnt: %d", frame.Size(), m_FrameCnt);

	// Transmit SPS, PPS and IDR-frame in the same access unit
	if (Configuration::GetCfg()->GetIntVal("SendPacketsFromAnxbFile") || Configuration::GetCfg()->GetIntVal("SendPacketsFromPcapFile")) {
		gotFrame = true;
	} else {
		nalHdr = (NalHeader *)(frame.Data() + 4);
		if (nalHdr->GetType() >= NALU_SLICE && nalHdr->GetType() <= NALU_SLICE_IDR) {

			gotFrame = true;
		} else {
			gotFrame = false;
		}
	}

	if (Configuration::GetCfg()->GetIntVal("SendPacketsFromPcapFile")) {
		// Sending data from pcap file, already packetized for RTP
		m_pSend.m_pSendCallback.TransportSendRTPPacket(frame.Data(), frame.Size());
	} else {
		if (Configuration::GetCfg()->GetIntVal("DumpSendAnxbPackets")) {
			m_pSend.m_pSendAnxbDump->StorePacket(frame.Data(), frame.Size());
		}

		if (m_pSend.m_pSendCallback.GetPayloadFmt() == SkypeVideoRTPInterface::FORMAT_FRAMES) {
			m_pSend.m_pSendCallback.TransportSendFrame(frame);
		} else {
			m_pSend.m_AnxbToRtp.AnxbFrameToRtp(frame.Data(), frame.Size(), frame.GetTimestamp(), gotFrame);
		}
	}
}

void H264AnxbToRtpCb::TransmitRtpPacket(const uint8_t* data, uint32_t len) {
	if (Configuration::GetCfg()->GetIntVal("DumpSendRtpPackets")) {
		m_pSend.m_pSendRtpDump->StorePacket(data, len);
	}

	m_pSend.m_pSendCallback.TransportSendRTPPacket(data, len);
}

SendVideoWrapper::SendVideoWrapper(SendPacketCallback& sendCb, int initialMaxPacketSize, VideoParams& vidPar) :
	m_pSendCallback(sendCb),
	m_VidEncCb(*this),
	m_AnxbToRtpCb(*this),
	m_AnxbToRtp(&m_AnxbToRtpCb),
	m_pSendRtpDump(NULL),
	m_pSendAnxbDump(NULL),
	m_pCapturer(NULL),
	m_pEncoder(NULL)
{
	m_AnxbToRtp.SetMaxRtpPacketLen(initialMaxPacketSize);

	if (Configuration::GetCfg()->GetIntVal("DumpSendRtpPackets")) {
		m_pSendRtpDump = new PacketDump("sent_rtp_packets.pcap", PDUMP_FMT_PCAP);
	}

	if (Configuration::GetCfg()->GetIntVal("DumpSendAnxbPackets")) {
		m_pSendAnxbDump = new PacketDump("sent_anxb_packets.264", PDUMP_FMT_RAW);
	}

	if (Configuration::GetCfg()->GetIntVal("SendPacketsFromAnxbFile")) {
		strncpy(vidPar.devName, Configuration::GetCfg()->GetStrVal("SendAnxbFileDir"), sizeof(vidPar.devName));
		m_pEncoder = new H264AnxbFileReader(&vidPar, NULL, &m_VidEncCb);
	} else if (Configuration::GetCfg()->GetIntVal("SendPacketsFromPcapFile")) {
		strncpy(vidPar.devName, Configuration::GetCfg()->GetStrVal("SendPcapFilename"), sizeof(vidPar.devName));
		m_pEncoder = new PcapFileReader(&vidPar, NULL, &m_VidEncCb);
	} else if (Configuration::GetCfg()->GetIntVal("SendVideoFromCamera")) {
#ifndef MINIMAL_MODE
		strncpy(vidPar.devName, Configuration::GetCfg()->GetStrVal("VideoCameraDevName"), sizeof(vidPar.devName));
		if (Configuration::GetCfg()->GetIntVal("UseDummyCapturer")) {
			m_pCapturer = new DummyCapturer(&vidPar, NULL);
		} else {
			m_pCapturer = new Capturer(&vidPar, NULL);
		}
		m_pEncoder = new Encoder(&vidPar, m_pCapturer, &m_VidEncCb);
#endif
	} else {
		WARN("No encode instance started");
		return;
	}

	if (m_pCapturer) {
		m_pCapturer->StartCapturing();
	}
	m_pEncoder->StartEncoding();
}

SendVideoWrapper::~SendVideoWrapper() {
	if (m_pEncoder) {
		m_pEncoder->m_pVCap = NULL;
		m_pEncoder->StopEncoding();
	}
	if (m_pCapturer) {
		m_pCapturer->StopCapturing();
	}

	CLEANUP(m_pSendRtpDump);
	CLEANUP(m_pSendAnxbDump);
	CLEANUP(m_pCapturer);
	CLEANUP(m_pEncoder);
}

void SendVideoWrapper::SetBitrate(uint32_t bitrate) {
	if (m_pEncoder) {
		m_pEncoder->ChangeBitrate(bitrate);
	}
}

void SendVideoWrapper::SetFramerate(uint32_t framerate) {
	if (m_pEncoder) {
		m_pEncoder->ChangeFramerate(framerate);
	}
	if (m_pCapturer) {
		m_pCapturer->ChangeFramerate(framerate);
	}
}

void SendVideoWrapper::SetMaxPacketSize(uint32_t maxPacketSize) {
	m_AnxbToRtp.SetMaxRtpPacketLen(maxPacketSize);
}

void SendVideoWrapper::RequestKeyFrame() {
	if (m_pEncoder) {
		m_pEncoder->GenerateKeyFrame();
	}
}
