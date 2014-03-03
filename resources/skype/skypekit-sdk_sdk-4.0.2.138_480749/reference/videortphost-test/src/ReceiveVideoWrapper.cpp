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
/*! \class H264AnxbToRtpCb
 *  Implements TransmitRtpPacket method (H264 Annex B frame as RTP packet).
 */
#include "ReceiveVideoWrapper.hpp"
#include "RtpTeLogging.hpp"
#include "PacketDump.hpp"

#ifndef MINIMAL_MODE
#include "H264Decoder.hpp"
#include "X11Renderer.hpp"
#endif

void H264RtpToAnxbCb::TransmitAnxbFrame(const uint8_t *data, uint32_t len, int timestamp_ms) {
	m_FrameCnt++;
	PACKET_DBG("To decoder: frame len %d bytes,  received_frame_cnt: %d, timestamp_ms: %d", len, m_FrameCnt, timestamp_ms);

	m_pReceive.m_ReceiveFStat.Update();
	if (m_pReceive.m_ReceiveFStat.IsValid())
		TI_DBG("Receive Framerate: %s", m_pReceive.m_ReceiveFStat.GetStatStr());

	if (Configuration::GetCfg()->GetIntVal("DumpReceiveAnxbPackets")) {
		m_pReceive.m_pReceiveAnxbDump->StorePacket(data, len);
	}

#ifndef MINIMAL_MODE
	if (Configuration::GetCfg()->GetIntVal("RenderVideo")) {
		VideoData frame;
		frame.SetBuf(data, len);
		frame.SetTimestamp(timestamp_ms);
		m_pReceive.m_pDecoder->DecodeFrame(frame);
	}
#endif
}

#define CLEANUP(ptr) if (ptr) { delete ptr; ptr = NULL;}

ReceiveVideoWrapper::ReceiveVideoWrapper(VideoParams &vidPar) :
	m_RtpToAnxbCb(*this),
	m_RtpToAnxb(&m_RtpToAnxbCb),
	m_ReceiveFStat(3000000),
	m_pReceiveRtpDump(NULL),
	m_pReceiveAnxbDump(NULL),
	m_pRenderer(NULL),
	m_pDecoder(NULL)
{
	if (Configuration::GetCfg()->GetIntVal("DumpReceiveRtpPackets")) {
		char filename[100];
		sprintf(filename, "received_rtp_packets%dx%d.pcap", vidPar.width, vidPar.height);
		m_pReceiveRtpDump = new PacketDump(filename, PDUMP_FMT_PCAP);
	}
	
	if (Configuration::GetCfg()->GetIntVal("DumpReceiveAnxbPackets")) {
		char filename[100];
		sprintf(filename, "received_anxb_packets%dx%d.264", vidPar.width, vidPar.height);
		m_pReceiveAnxbDump = new PacketDump(filename, PDUMP_FMT_RAW);
	}
	
	if (Configuration::GetCfg()->GetIntVal("RenderVideo")) {
#ifndef MINIMAL_MODE
		m_pRenderer = new X11Renderer(&vidPar, "Renderer", 450, 150);
		m_pDecoder = new H264Decoder(&vidPar, m_pRenderer);
#endif
	}	
}

ReceiveVideoWrapper::~ReceiveVideoWrapper() {
	CLEANUP(m_pReceiveRtpDump);
	CLEANUP(m_pReceiveAnxbDump);
	CLEANUP(m_pRenderer);
	CLEANUP(m_pDecoder);
}

void ReceiveVideoWrapper::SubmitPacket(const uint8_t *packet, uint32_t packetLen) {
	if (Configuration::GetCfg()->GetIntVal("DumpReceiveRtpPackets")) {
		m_pReceiveRtpDump->StorePacket(packet, packetLen);
	}

	if (Configuration::GetCfg()->GetIntVal("DumpReceiveAnxbPackets") || Configuration::GetCfg()->GetIntVal("RenderVideo")) {
		m_RtpToAnxb.RtpToAnxbFrame(packet, packetLen);
	}	
}

void ReceiveVideoWrapper::SubmitFrame(VideoData &frame) {
	PACKET_DBG("To decoder: frame len %d bytes", frame.Size());

	if (Configuration::GetCfg()->GetIntVal("DumpReceiveAnxbPackets")) {
		m_pReceiveAnxbDump->StorePacket(frame.Data(), frame.Size());
	}

#ifndef MINIMAL_MODE
	if (Configuration::GetCfg()->GetIntVal("RenderVideo")) {
		m_pDecoder->DecodeFrame(frame);
	}
#endif
}
