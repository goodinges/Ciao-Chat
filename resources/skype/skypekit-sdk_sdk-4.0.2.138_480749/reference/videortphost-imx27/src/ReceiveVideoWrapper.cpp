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

#include "ReceiveVideoWrapper.hpp"
#include "LoggingCtrl.hpp"

#define CLEANUP(ptr) if (ptr) {delete ptr; ptr = NULL;}

/*! \class H264AnxbToRtpCb
 *  Implements TransmitRtpPacket method (H264 Annex B frame as RTP packet).
 */
void H264RtpToAnxbCb::TransmitAnxbFrame(const uint8_t *data, uint32_t len, int timestamp_ms) {
	m_FrameCnt++;
	PACKET_DBG("To decoder: frame len %d bytes, received_frame_cnt: %d, timestamp_ms: %d", len, m_FrameCnt, timestamp_ms);

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

ReceiveVideoWrapper::ReceiveVideoWrapper(VideoParams& vidPar) :
	m_RtpToAnxbCb(*this),
	m_RtpToAnxb(&m_RtpToAnxbCb),
	m_pReceiveRtpDump(NULL),
	m_pReceiveAnxbDump(NULL),
	m_pRenderer(NULL),
	m_pDecoder(NULL)
{
	if (Configuration::GetCfg()->GetIntVal("DumpReceiveRtpPackets")) {
		m_pReceiveRtpDump = new PacketDump("received_rtp_packets.pcap", PDUMP_FMT_PCAP);
	}

	if (Configuration::GetCfg()->GetIntVal("DumpReceiveAnxbPackets")) {
		m_pReceiveAnxbDump = new PacketDump("received_anxb_packets.264", PDUMP_FMT_RAW);
	}

#ifndef MINIMAL_MODE
	if (Configuration::GetCfg()->GetIntVal("RenderVideo")) {
		m_pRenderer = new Renderer(&vidPar);
		m_pDecoder = new Decoder(&vidPar, m_pRenderer);
	}
#endif
}

ReceiveVideoWrapper::~ReceiveVideoWrapper() {
	if (m_pDecoder) {
		m_pDecoder->m_pVRend = NULL;
	}
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

void ReceiveVideoWrapper::SubmitFrame(VideoData& frame) {
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
