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
#include "H264RtpToAnxb.hpp"

#include <string.h>
#include <stdlib.h>

#include "rtph.hpp"

#define LOG_PREFIX "Rtp2Anxb:"
#include "debugging.hpp"

int TIMESTAMP_RTP90KHZ_TO_MS(unsigned int timestamp_rtp, unsigned int &last_timestamp_rtp, long long &accumulator) {
	long long diff = (int)(timestamp_rtp - last_timestamp_rtp);

	accumulator += diff;
	last_timestamp_rtp = timestamp_rtp;
	return (int)(accumulator/90);
}


H264RtpToAnxb::H264RtpToAnxb(AnxbFrameCallback *cb) :
	anxbFrameCb(cb),
	anxbPacketBuf(0), 
	anxbPacketBufLen(0),
	anxbPacketBufUsed(0),
	lastTimestampRtp(0),
	accumulator(0) {
	
}

H264RtpToAnxb::~H264RtpToAnxb() {
	free(anxbPacketBuf);	
}

void H264RtpToAnxb::AddNalToAnxbBuf(const uint8_t *buffer, uint32_t bufLen, NalHeader *nalHdr, bool skipAnnexbHdr) {
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

void H264RtpToAnxb::RtpToAnxbFrame(const uint8_t *buffer, uint32_t bufLen) {
	rtph *rtpHdr = (rtph *) buffer;
	NalHeader *nalHdr = (NalHeader *) rtpHdr->GetDataPtr();
	int nalLen = bufLen - sizeof(rtph);

	if (nalHdr->GetType() >= NALU_SLICE && nalHdr->GetType() <= NALU_FILLER) {
		VDBG("process packet - single nal unit");

		//single nal unit
		AddNalToAnxbBuf((uint8_t *)nalHdr, nalLen, NULL, false);

	} else if (nalHdr->GetType() == NALU_STAP_A) { //stap-A
		uint16_t stapaLen;
		uint8_t *nalDataPtr;
		VDBG("process packet - stap-A");

		nalDataPtr = nalHdr->GetDataPtr();
		nalLen -= sizeof(NalHeader);
		while (nalLen > 0) {
			uint16_t tmp;
			((uint8_t*)&tmp)[0] = nalDataPtr[0];
			((uint8_t*)&tmp)[1] = nalDataPtr[1];
			stapaLen = ntohs(tmp);
			nalDataPtr += sizeof(stapaLen);

			AddNalToAnxbBuf(nalDataPtr, stapaLen, NULL, false);
			VDBG("found nal: %d", stapaLen);

			nalLen -= sizeof(stapaLen) + stapaLen;
			nalDataPtr += stapaLen;
		}
	} else if (nalHdr->GetType() == NALU_FU_A) { // FU-A
		VDBG("process packet - FU-A");

		FUAHeader *fuaHdr = (FUAHeader *)nalHdr;
		if (fuaHdr->GetS()) { // start bit set
			NalHeader rebuiltNalHdr;
			rebuiltNalHdr.Reset();
			rebuiltNalHdr.SetNri(fuaHdr->GetNri());
			rebuiltNalHdr.SetType(fuaHdr->GetNalType());
			AddNalToAnxbBuf(fuaHdr->GetDataPtr(), nalLen - sizeof(FUAHeader), &rebuiltNalHdr, false);
		} else {
			AddNalToAnxbBuf(fuaHdr->GetDataPtr(), nalLen - sizeof(FUAHeader), NULL, true);
		}
	} else {
		ERROR("process packet - UNABLE TO GET PACKET TYPE");
	}

	if(rtpHdr->GetMarker()) {
		if (anxbFrameCb) {
			int timestamp = TIMESTAMP_RTP90KHZ_TO_MS(rtpHdr->GetTimestamp(), lastTimestampRtp, accumulator);
			anxbFrameCb->TransmitAnxbFrame(anxbPacketBuf, anxbPacketBufUsed, timestamp);
		}
		anxbPacketBufUsed = 0;
	}
}
