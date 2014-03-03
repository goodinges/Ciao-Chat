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
#ifndef H264ANXBTORTP_HPP_
#define H264ANXBTORTP_HPP_

#include <stdint.h>
#include "rfc3984.hpp"

#include "ptasking.hpp"

struct AnxbUnit {
	uint32_t nalOffset;
	uint32_t totalLen;
	const uint8_t *data;
	const uint8_t *GetNal() {
		return data + nalOffset;
	}
	uint32_t GetNalLen() {
		return totalLen - nalOffset;		
	}
};

#define DEFAULT_MAX_RTP_PACKET_LEN 400

class H264AnxbToRtp {
public:
	struct RtpPacketCallback {
	public:
		virtual ~RtpPacketCallback() {};
		virtual void TransmitRtpPacket(const uint8_t *data, uint32_t len) = 0;
	};	
	H264AnxbToRtp(RtpPacketCallback *cb);
	~H264AnxbToRtp();

	void SetMaxRtpPacketLen(uint32_t packetLen);
	void AnxbFrameToRtp(const uint8_t *buffer, uint32_t bufLen, int timestamp_ms, bool isFullFrame);

private:
	uint32_t GetRtpTimestamp();
	void ConstructRtpPacket(const uint8_t *payload, uint32_t payloadLen,
			FUAHeader *fuaHdr, bool addAsStapa, uint32_t rtpMarker);
	void NalToRtpPayload(const uint8_t *buffer, uint32_t bufLen, uint32_t rtpMarker);
	
	pMutex funcMutex;
	uint16_t seq;
	uint32_t ssrc;
	uint32_t maxRtpPayload;
	uint8_t *rtpPacketBuf;
	uint32_t pendingStapaLen;
	uint64_t startTimeUs;
	uint32_t currTimestamp;

	RtpPacketCallback *rtpPacketCb;
};

bool GetAnxbUnitFromStream(const uint8_t *buf, uint32_t bufLen, AnxbUnit &unit);

#endif //H264ANXBTORTP_HPP_
