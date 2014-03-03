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

#include <stdlib.h>
#include "AudioRtpAnalyzer.hpp"
#include "rtph.hpp"
#include "Logging.hpp"

AudioRtpAnalyzer::AudioRtpAnalyzer(AudioFrameCallback *cb, int outHeader, int log) :
	audioFrameCb(cb),
	keepRtpHeader(outHeader),
	m_PayloadType(128),
	m_Timestamp(0),
	m_Log(log) {
}

AudioRtpAnalyzer::~AudioRtpAnalyzer() {
}

void AudioRtpAnalyzer::RtpToAudioFrame(uint8_t *buffer, uint32_t bufLen) {
	rtph *rtpHdr = (rtph *) buffer;
	unsigned char *data = NULL;
	int dataLen = 0;
	int clockRate = 0;

	data = rtpHdr->GetDataPtr();
	dataLen = bufLen - sizeof(rtph);

	if (m_PayloadType != rtpHdr->GetPayloadType()) {
		if (m_PayloadType < 128) {
			info(m_Log, "\nCodec change detected\n");
		}
		m_PayloadType = rtpHdr->GetPayloadType();
	}
	switch (m_PayloadType) {
	case PT_PCMU:
		info(m_Log, "PCMU - G.711 mu-law, size:%u\n", bufLen);
		clockRate = 8000;
		break;
	case PT_PCMA:
		info(m_Log, "PCMA - G.711 A-law, size:%u\n", bufLen);
		clockRate = 8000;
		break;
	case PT_G729:
		info(m_Log, "G729 - G.729 (Annex A), size:%u\n", bufLen);
		clockRate = 8000;
		break;
	case PT_SILK_V3:
		info(m_Log, "SILK_V3 - SILK Super Wideband, size:%u\n", bufLen);
		break;
	case PT_SILK_V3_MB:
		info(m_Log, "SILK_V3_MB - SILK Mediumband, size:%u\n", bufLen);
		break;
	case PT_SILK_V3_NB:
		info(m_Log, "SILK_V3_NB - SILK Narrowband, size:%u\n", bufLen);
		break;
	case PT_SILK_V3_WB:
		info(m_Log, "SILK_V3_WB - SILK Wideband, size:%u\n", bufLen);
		break;
	default:
		error("Unknown payload type %d, size:%u\n", m_PayloadType, bufLen);
	}
	if (rtpHdr->GetMarker()) {
		info(m_Log, " Start of talkspurt\n");
	}
	info(m_Log, " V:%u, P:%u, X:%u, CC:%u, M:%u, PT:%u, SN:%u, TS:%u, SSRC:%u\n", rtpHdr->GetVersion(), rtpHdr->GetPadding(), rtpHdr->GetExtension(), rtpHdr->GetCSRCCount(), rtpHdr->GetMarker(), rtpHdr->GetPayloadType(), rtpHdr->GetSequence(), rtpHdr->GetTimestamp(), rtpHdr->GetSSRC());
	if (clockRate) {
		info(m_Log, "  timestamp difference: %d ms\n", 1000 * (rtpHdr->GetTimestamp() - m_Timestamp) / clockRate);
	} else {
		info(m_Log, "  timestamp difference: %d\n", (rtpHdr->GetTimestamp() - m_Timestamp));
	}
	m_Timestamp = rtpHdr->GetTimestamp();

	if (!audioFrameCb) {
		fatal("Callback function not set\n");
	}

	if (keepRtpHeader) {
		audioFrameCb->TransmitAudioFrame(buffer, bufLen);
	}
	else { // !keepRtpHeader
		audioFrameCb->TransmitAudioFrame(data, dataLen);
	}
}
