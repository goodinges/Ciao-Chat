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
#include "H264RtpAnalyzer.hpp"
#include "rtph.hpp"
#include "Logging.hpp"

H264RtpAnalyzer::H264RtpAnalyzer(AnxbFrameCallback *cb, int inHeader, int outHeader, int videoHeader, int log) :
	anxbFrameCb(cb),
	hasRtpHeader(inHeader),
	keepRtpHeader(outHeader),
	onlyVideoHeader(videoHeader),
	anxbPacketBuf(NULL),
	anxbPacketBufLen(0),
	anxbPacketBufUsed(0),
	m_Log(log) {
}

H264RtpAnalyzer::~H264RtpAnalyzer() {
	if (anxbPacketBuf) {
		free(anxbPacketBuf);
		anxbPacketBuf = NULL;
	}
}

void H264RtpAnalyzer::AddNalToAnxbBuf(uint8_t *buffer, uint32_t bufLen, NalHeader *nalHdr, bool skipAnnexbHdr) {
	int maxNeededSize = bufLen + 4 + sizeof(NalHeader);
	if (anxbPacketBufUsed + maxNeededSize > anxbPacketBufLen) {
		anxbPacketBufLen += (anxbPacketBufUsed + maxNeededSize) * 2;
		anxbPacketBuf = (uint8_t *) realloc(anxbPacketBuf, anxbPacketBufLen);
	}

	if (!skipAnnexbHdr) {
		anxbPacketBuf[anxbPacketBufUsed++] = 0;
		anxbPacketBuf[anxbPacketBufUsed++] = 0;
		anxbPacketBuf[anxbPacketBufUsed++] = 0;
		anxbPacketBuf[anxbPacketBufUsed++] = 1;
	}

	if (nalHdr) {
		memcpy(anxbPacketBuf + anxbPacketBufUsed, (void *)nalHdr, sizeof(NalHeader));
		anxbPacketBufUsed += sizeof(NalHeader);
	}

	memcpy(anxbPacketBuf + anxbPacketBufUsed, buffer, bufLen);
	anxbPacketBufUsed += bufLen;
}

void H264RtpAnalyzer::RtpToAnxbFrame(uint8_t *buffer, uint32_t bufLen) {
	rtph *rtpHdr = NULL;
	NalHeader *nalHdr = NULL;
	int nalLen = 0;

	if (hasRtpHeader) {
		rtpHdr = (rtph *) buffer;
		nalHdr = (NalHeader *) rtpHdr->GetDataPtr();
		nalLen = bufLen - sizeof(rtph);
	} else {
		nalHdr = (NalHeader *) buffer;
		nalLen = bufLen;
	}

	if (nalLen < 1) {
		error("Empty RTP packet: size:%u\n", bufLen);
	} else {
		info(m_Log, "nal_unit_type:%d, %s, size:%u\n", nalHdr->GetType(), nal_unit_type[nalHdr->GetType()], bufLen);
		if ((NALU_FU_A == nalHdr->GetType()) || (NALU_FU_B == nalHdr->GetType())) {
			FUAHeader *fuaHdr = (FUAHeader *)nalHdr;
			if (fuaHdr->GetS()) {
				info(m_Log, " Start of fragmented NAL unit\n");
			}
			if (fuaHdr->GetE()) {
				info(m_Log, " End of fragmented NAL unit\n");
			}
		}
	}
	if (hasRtpHeader) {
		info(m_Log, " V:%u, P:%u, X:%u, CC:%u, M:%u, PT:%u, SN:%u, TS:%u, SSRC:%u\n", rtpHdr->GetVersion(), rtpHdr->GetPadding(), rtpHdr->GetExtension(), rtpHdr->GetCSRCCount(), rtpHdr->GetMarker(), rtpHdr->GetPayloadType(), rtpHdr->GetSequence(), rtpHdr->GetTimestamp(), rtpHdr->GetSSRC());
	}

	if (nalLen < 1) {
		error(" Unable to determine NAL unit type.\n");
	}
	else if (nalHdr->GetType() >= NALU_SLICE && nalHdr->GetType() <= NALU_FILLER) { // Single NAL Unit Packet
		info(m_Log, " F:%u, NRI:%u, T:%u\n", nalHdr->GetF(), nalHdr->GetNri(), nalHdr->GetType());

		AddNalToAnxbBuf((uint8_t *)nalHdr, nalLen, NULL, false);

	} else if (NALU_STAP_A == nalHdr->GetType()) { // STAP-A
		uint16_t stapaLen;
		uint8_t *nalDataPtr;
		info(m_Log, " F:%u, NRI:%u, T:%u\n", nalHdr->GetF(), nalHdr->GetNri(), nalHdr->GetType());

		nalDataPtr = nalHdr->GetDataPtr();
		nalLen -= sizeof(NalHeader);
		while (nalLen > 0) {
			nalHdr = (NalHeader *) ((rtph *) &(nalDataPtr[2]));
			uint16_t tmp;
			((uint8_t*)&tmp)[0] = nalDataPtr[0];
			((uint8_t*)&tmp)[1] = nalDataPtr[1];
			stapaLen = ntohs(tmp);
			nalDataPtr += sizeof(stapaLen);

			AddNalToAnxbBuf(nalDataPtr, stapaLen, NULL, false);
			info(m_Log, "  nal_unit_type:%d, %s, NALU size:%u\n", nalHdr->GetType(), nal_unit_type[nalHdr->GetType()], stapaLen);
			info(m_Log, "  F:%u, NRI:%u, T:%u\n", nalHdr->GetF(), nalHdr->GetNri(), nalHdr->GetType());

			nalLen -= sizeof(stapaLen) + stapaLen;
			nalDataPtr += stapaLen;
		}
	} else if (NALU_FU_A == nalHdr->GetType()) { // FU-A
		FUAHeader *fuaHdr = (FUAHeader *)nalHdr;
		info(m_Log, "  nal_unit_type:%d, %s\n", fuaHdr->GetNalType(), nal_unit_type[fuaHdr->GetNalType()]);
		info(m_Log, "  F:%u, NRI:%u, T:%u, S:%u, E:%u, R:%u, T:%u\n", fuaHdr->GetF(), fuaHdr->GetNri(), fuaHdr->GetFuaType(), fuaHdr->GetS(), fuaHdr->GetE(), fuaHdr->GetR(), fuaHdr->GetNalType());

		if (fuaHdr->GetS()) { // Start bit
			NalHeader rebuiltNalHdr;
			rebuiltNalHdr.Reset();
			rebuiltNalHdr.SetNri(fuaHdr->GetNri());
			rebuiltNalHdr.SetType(fuaHdr->GetNalType());
			AddNalToAnxbBuf(fuaHdr->GetDataPtr(), nalLen - sizeof(FUAHeader), &rebuiltNalHdr, false);
		} else {
			AddNalToAnxbBuf(fuaHdr->GetDataPtr(), nalLen - sizeof(FUAHeader), NULL, true);
		}
	} else {
		info(m_Log, "Unknown NAL unit payload type %d\n", nalHdr->GetType());
	}

	if (!anxbFrameCb) {
		fatal("Callback function not set\n");
	}
	if (hasRtpHeader && keepRtpHeader) {
		if (rtpHdr->GetMarker()) {
			anxbPacketBufUsed = 0;
			info(m_Log, "\n");
		}
		if (onlyVideoHeader) {
			buffer += sizeof(rtph);
			bufLen -= sizeof(rtph);
		}
		anxbFrameCb->TransmitAnxbFrame(buffer, bufLen);
	}
	else if (hasRtpHeader && !keepRtpHeader) {
		if (rtpHdr->GetMarker()) {
			if (anxbPacketBufUsed) {
				anxbFrameCb->TransmitAnxbFrame(anxbPacketBuf, anxbPacketBufUsed);
				anxbPacketBufUsed = 0;
			}
			info(m_Log, "\n");
		}
	}
	else if (!hasRtpHeader && keepRtpHeader) {
		anxbFrameCb->TransmitAnxbFrame(buffer, bufLen);
		anxbPacketBufUsed = 0;
	}
	else { // !hasRtpHeader && !keepRtpHeader
		if (anxbPacketBufUsed) {
			anxbFrameCb->TransmitAnxbFrame(anxbPacketBuf, anxbPacketBufUsed);
			anxbPacketBufUsed = 0;
		}
	}
}
