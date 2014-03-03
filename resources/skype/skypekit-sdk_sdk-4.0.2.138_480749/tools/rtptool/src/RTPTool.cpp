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

#include "RTPTool.hpp"
#include "Common.hpp"
#include "rtph.hpp"
#include "rfc3551.hpp"
#include "Logging.hpp"

void ReaderCallback::TransmitFrame(uint8_t* data, int len) {
	if (!m_RTPTool.m_MediaType) {
		// Automatically detect media type (audio or video)
		rtph *rtpHdr = NULL;
		int hasRtpHeader = (m_RTPTool.m_InFormat != FILE_FMT_WIN);
		if (hasRtpHeader) {
			rtpHdr = (rtph *) data;
			if (PT_DYNAMIC == rtpHdr->GetPayloadType()) {
				info(m_RTPTool.m_Log,"Assumed video stream (payload type = %u).\n\n", rtpHdr->GetPayloadType());
				m_RTPTool.m_MediaType = MEDIA_FMT_VIDEO;
			} else {
				info(m_RTPTool.m_Log, "Assumed audio stream (payload type = %u).\n\n", rtpHdr->GetPayloadType());
				m_RTPTool.m_MediaType = MEDIA_FMT_AUDIO;
				if (m_RTPTool.m_OutFormat == FILE_FMT_WIN) {
					fatal("%s: win format not supported for audio streams\n", RTPTOOL);
				}
			}
		} else {
			info(m_RTPTool.m_Log, "Assumed video stream (no RTP headers).\n\n");
			m_RTPTool.m_MediaType = MEDIA_FMT_VIDEO;
		}
	}

	switch (m_RTPTool.m_MediaType) {
	case MEDIA_FMT_VIDEO:
		m_RTPTool.m_RtpToAnxb.RtpToAnxbFrame(data, len);
		break;
	case MEDIA_FMT_AUDIO:
		m_RTPTool.m_RtpToAudio.RtpToAudioFrame(data, len);
		break;
	default:
		fatal("Unsupported input media type %d", m_RTPTool.m_MediaType);
	}
}

RTPTool::RTPTool(int inMedia, int inFormat, const char* inName, int outFormat, const char* outName, int log) :
	m_ReaderCb(*this),
	m_RtpToAudioCb(*this),
	m_RtpToAudio(&m_RtpToAudioCb, (outFormat != FILE_FMT_RAW) && (outFormat != FILE_FMT_BIT), log),
	m_RtpToAnxbCb(*this),
	m_RtpToAnxb(&m_RtpToAnxbCb, (inFormat != FILE_FMT_WIN), (outFormat != FILE_FMT_RAW) && (outFormat != FILE_FMT_BIT), (FILE_FMT_WIN == outFormat), log),
	m_MediaType(inMedia),
	m_InFormat(inFormat),
	m_OutFormat(outFormat),
	m_Log(log) {
	m_pReader = new FileReader(inName, (FileFormat) inFormat, &m_ReaderCb, log);
	if (outName) {
		m_pWriter = new FileWriter(outName, (FileFormat) outFormat, log);
	}
}

RTPTool::~RTPTool() {
	if (m_pReader) {
		m_pReader->StopReading();
		CLEANUP(m_pReader);
	}
	if (m_pWriter) {
		CLEANUP(m_pWriter);
	}
}

void RTPTool::AnalyzeRTP() {
	m_pReader->StartReading();
}

void AudioRtpAnalyzerCb::TransmitAudioFrame(uint8_t* data, uint32_t len) {
	if (m_RTPTool.m_pWriter) {
		m_RTPTool.m_pWriter->WritePacket(data, len);
	}
}

void H264RtpAnalyzerCb::TransmitAnxbFrame(uint8_t* data, uint32_t len) {
	if (m_RTPTool.m_pWriter) {
		m_RTPTool.m_pWriter->WritePacket(data, len);
	}
}
