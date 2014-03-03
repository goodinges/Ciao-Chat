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
#ifndef H264RTPTOANXB_HPP_
#define H264RTPTOANXB_HPP_

#include <stdint.h>
#include "rfc3984.hpp"

class H264RtpToAnxb {
public:
	struct AnxbFrameCallback {
	public:
		virtual ~AnxbFrameCallback() {};
		virtual void TransmitAnxbFrame(const uint8_t *data, uint32_t len, int timestamp) = 0;
	};
	H264RtpToAnxb(AnxbFrameCallback *cb);
	~H264RtpToAnxb();

	void RtpToAnxbFrame(const uint8_t *buffer,uint32_t buflen);

private:
	void AddNalToAnxbBuf(const uint8_t *buffer, uint32_t bufLen, NalHeader *nalHdr, bool skipAnnexbHdr);
	
	AnxbFrameCallback *anxbFrameCb;
	uint8_t *anxbPacketBuf;
	uint32_t anxbPacketBufLen;
	uint32_t anxbPacketBufUsed;
	unsigned int lastTimestampRtp;
	long long accumulator;
};

#endif //H264RTPTOANXB_HPP_
