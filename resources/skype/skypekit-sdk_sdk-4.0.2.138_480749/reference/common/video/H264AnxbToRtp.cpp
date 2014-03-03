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

#include "H264AnxbToRtp.hpp"

#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "rtph.hpp"

#undef LOG_PREFIX
#define LOG_PREFIX "Anxb2Rtp:"
#include "debugging.hpp"

#define TIMESTAMP_MS_TO_RTP90KHZ(timestamp_ms) timestamp_ms * 90

H264AnxbToRtp::H264AnxbToRtp(RtpPacketCallback *cb) :
	seq(0),
	ssrc(0),
	maxRtpPayload(DEFAULT_MAX_RTP_PACKET_LEN - sizeof(rtph)),
	pendingStapaLen(0),
	startTimeUs(0),
	rtpPacketCb(cb)
	{
	
	ssrc = (uint32_t)lrand48();
	
	rtpPacketBuf = new uint8_t[DEFAULT_MAX_RTP_PACKET_LEN];
}

H264AnxbToRtp::~H264AnxbToRtp() {
	delete[] rtpPacketBuf;
}

void H264AnxbToRtp::SetMaxRtpPacketLen(uint32_t packetLen) {
	pMutexLock l(funcMutex);

	delete[] rtpPacketBuf;	
	maxRtpPayload = packetLen - sizeof(rtph);
	rtpPacketBuf = new uint8_t[packetLen];
}

bool GetAnxbUnitFromStream(const uint8_t *buf, uint32_t bufLen, AnxbUnit &unit) {
	uint32_t val = 0xffffffff;
	uint32_t offset = 0;

	unit.data = buf;

	while (offset < bufLen) {
		val <<= 8;
		val |= buf[offset];
		offset++;
		if ((val & 0x00ffffff) == 0x000001) {
			break;
		}
	}

	/* if start code not found then report that no unit AnxbUnit found */
	if (offset == bufLen)
		return false;

	unit.nalOffset = offset;

	val = 0xffffffff;
	while (offset < bufLen) {
		val <<= 8;
		val |= buf[offset];
		offset++;
		if ((val & 0x00ffffff) == 0x000001 || (val & 0x00ffffff) == 0x000000) {
			unit.totalLen = offset - 3;
			return true;
		}
	}

	/* If end code was not found then the stream end will be the end of AnxbUnit */
	unit.totalLen = offset;
	return true;
}

void H264AnxbToRtp::ConstructRtpPacket(const uint8_t *payload, uint32_t payloadLen, 
		FUAHeader *fuaHdr, bool addAsStapa, uint32_t rtpMarker) {
	rtph *rtpHdr = (rtph *) rtpPacketBuf;
	bool sendBuffer;
	uint32_t rtpPacketLen;

	/* send collected stapa packets when:
	 * 1) current packet is not stapa packet
	 * 2) current stapa packet is too big for this RTP packet
	*/
	if (pendingStapaLen && 
			(!addAsStapa || 
			pendingStapaLen + payloadLen + 2 > maxRtpPayload)) {
		
		if (rtpPacketCb)
			rtpPacketCb->TransmitRtpPacket(rtpPacketBuf, sizeof(rtph) + pendingStapaLen);

		pendingStapaLen = 0;
	} 	
	
	if (!pendingStapaLen) {
	
		rtpHdr->Reset();
		rtpHdr->SetMarker(rtpMarker);
		rtpHdr->SetPayloadType(0x60); //payload type
		rtpHdr->SetSequence(seq++); //sequence number
		rtpHdr->SetTimestamp(currTimestamp); // timestamp
		rtpHdr->SetSSRC(ssrc); //synchronization source SSRC
	}
	
	if (fuaHdr) {
		memcpy(rtpHdr->GetDataPtr(), (void *) fuaHdr,  sizeof(FUAHeader));
		memcpy(rtpHdr->GetDataPtr() + sizeof(FUAHeader), (void *) payload, payloadLen);
		sendBuffer = true;
		rtpPacketLen = sizeof(rtph) + sizeof(FUAHeader) + payloadLen;
	} else if (addAsStapa) {
		NalHeader nalHdr;

		// if first stap-a unit then add stapA header  
		if (!pendingStapaLen) {
			nalHdr.Reset();
			nalHdr.SetType(NALU_STAP_A);
			nalHdr.SetNri(((NalHeader *)payload)->GetNri());
			memcpy(rtpHdr->GetDataPtr(), (void *)&nalHdr, sizeof(NalHeader));
			pendingStapaLen += sizeof(NalHeader);
		}

		// nalu size
		uint16_t naluSize = htons(payloadLen); 
		memcpy(rtpHdr->GetDataPtr() + pendingStapaLen, (void *) &naluSize, sizeof(uint16_t));
		pendingStapaLen += sizeof(uint16_t);

		// nalu
		memcpy(rtpHdr->GetDataPtr() + pendingStapaLen, (void *) payload, payloadLen);
		pendingStapaLen += payloadLen;

		// if stapa packet has RTP marker set then send it instantly
		if (rtpMarker) {
			sendBuffer = true;
			rtpHdr->SetMarker(rtpMarker);
		} else {
			sendBuffer = false;
		}
		rtpPacketLen = sizeof(rtph) + pendingStapaLen;
	} else {
		memcpy(rtpHdr->GetDataPtr(), (void *) payload, payloadLen);	
		sendBuffer = true;
		rtpPacketLen = sizeof(rtph) + payloadLen;
	}

	if (sendBuffer) {
		if (rtpPacketCb)
			rtpPacketCb->TransmitRtpPacket(rtpPacketBuf, rtpPacketLen);
		pendingStapaLen = 0;
	}	
}

void H264AnxbToRtp::NalToRtpPayload(const uint8_t *buffer, uint32_t bufLen,
		uint32_t rtpMarker) {

	if (bufLen > maxRtpPayload) { // if NAL is too big then use fragmentation unit 
		FUAHeader fuaHdr;
		NalHeader *nalHdr;
		const uint8_t *nalBuf = buffer;
		uint32_t nalLen = bufLen;

		nalHdr = (NalHeader *) buffer;
		
		fuaHdr.Reset();
		fuaHdr.SetNri(nalHdr->GetNri());
		fuaHdr.SetS(1);
		fuaHdr.SetE(0);
		fuaHdr.SetNalType(nalHdr->GetType());

		nalBuf += sizeof(NalHeader); // remove the original Nal header
		nalLen -= sizeof(NalHeader);

		while (nalLen > 0) {
			uint32_t fuaDataLen;
			bool last = false;

			if ((nalLen + sizeof(FUAHeader)) <= maxRtpPayload) {
				fuaHdr.SetE(1);
				fuaDataLen = nalLen;
				last = true;
			} else {
				fuaDataLen = maxRtpPayload - sizeof(FUAHeader);
			}

			// send the FU-A packet
			ConstructRtpPacket(nalBuf, fuaDataLen, &fuaHdr, false, last ? rtpMarker : 0);

			fuaHdr.SetS(0);
			nalBuf += fuaDataLen;
			nalLen -= fuaDataLen;
		}
	} else if (bufLen < maxRtpPayload / 2 && bufLen <= MAX_STAPA_PACKET_PAYLOAD) { // if NAL is small then add it as STAP-A unit
		ConstructRtpPacket(buffer, bufLen, NULL, true, rtpMarker);
	} else {
		ConstructRtpPacket(buffer, bufLen, NULL, false, rtpMarker);
	}
}

void H264AnxbToRtp::AnxbFrameToRtp(const uint8_t *buffer, uint32_t bufLen, int timestamp_ms, bool isFullFrame) {
	pMutexLock l(funcMutex);
	AnxbUnit anxbUnit;
	uint32_t buf_offset = 0;
	NalHeader *nalHdr;
	int marker = 0;
	currTimestamp = TIMESTAMP_MS_TO_RTP90KHZ(timestamp_ms);

	while (GetAnxbUnitFromStream(buffer + buf_offset, bufLen - buf_offset,
			anxbUnit)) {
		nalHdr = (NalHeader *) anxbUnit.GetNal();

		buf_offset += anxbUnit.totalLen;
		if (buf_offset == bufLen && isFullFrame) {
			marker = 1;
			VDBG("Set marker!");
		}
		NalToRtpPayload(anxbUnit.GetNal(), anxbUnit.GetNalLen(), marker);
	}
}
