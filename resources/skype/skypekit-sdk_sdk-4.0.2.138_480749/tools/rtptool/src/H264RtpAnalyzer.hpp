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

#ifndef H264RTPANALYZER_HPP_
#define H264RTPANALYZER_HPP_

#include <stdint.h>
#include "rfc3984.hpp"

const char nal_unit_type[32][63]= {
	"Unspecified",
	"Coded slice of a non-IDR picture",
	"Coded slice data partition A",
	"Coded slice data partition B",
	"Coded slice data partition C",
	"Coded slice of an IDR picture",
	"Supplemental enhancement information (SEI)",
	"Sequence parameter set",
	"Picture parameter set",
	"Access unit delimiter",
	"End of sequence",
	"End of stream",
	"Filler data",
	"Sequence parameter set extension",
	"Prefix NAL unit",
	"Subset sequence parameter set",
	"Reserved",
	"Reserved",
	"Reserved",
	"Coded slice of an auxiliary coded picture without partitioning",
	"Coded slice extension",
	"Reserved",
	"Reserved",
	"Reserved",
	"Single-time aggregation packet (STAP-A)",
	"Single-time aggregation packet (STAP-B)",
	"Multi-time aggregation packet (MTAP16)",
	"Multi-time aggregation packet (MTAP24)",
	"Fragmentation unit (FU-A)",
	"Fragmentation unit (FU-B)",
	"Unspecified",
	"Unspecified"};

class H264RtpAnalyzer {
public:
	struct AnxbFrameCallback {
	public:
		virtual void TransmitAnxbFrame(uint8_t *data, uint32_t len) = 0;
	};
	H264RtpAnalyzer(AnxbFrameCallback *cb, int inHeader, int outHeader, int videoHeader, int log);
	~H264RtpAnalyzer();

	void RtpToAnxbFrame(uint8_t *buffer,uint32_t buflen);

private:
	void AddNalToAnxbBuf(uint8_t *buffer, uint32_t bufLen, NalHeader *nalHdr, bool skipAnnexbHdr);

	AnxbFrameCallback *anxbFrameCb;
	int hasRtpHeader, keepRtpHeader, onlyVideoHeader;
	uint8_t *anxbPacketBuf;
	uint32_t anxbPacketBufLen;
	uint32_t anxbPacketBufUsed;
	int m_Log;
};

#endif /* H264RTPANALYZER_HPP_ */
