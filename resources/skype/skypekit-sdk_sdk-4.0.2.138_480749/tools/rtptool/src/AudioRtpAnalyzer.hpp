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

#ifndef AUDIORTPANALYZER_HPP_
#define AUDIORTPANALYZER_HPP_

#include <stdint.h>
#include "rfc3551.hpp"

class AudioRtpAnalyzer {
public:
	struct AudioFrameCallback {
	public:
		virtual void TransmitAudioFrame(uint8_t *data, uint32_t len) = 0;
	};
	AudioRtpAnalyzer(AudioFrameCallback *cb, int outHeader, int log);
	~AudioRtpAnalyzer();

	void RtpToAudioFrame(uint8_t *buffer,uint32_t buflen);

private:
	void AddDataToAudioBuf(uint8_t *buffer, uint32_t bufLen);

	AudioFrameCallback *audioFrameCb;
	int keepRtpHeader;
	unsigned int m_PayloadType;
	unsigned int m_Timestamp;
	int m_Log;
};

#endif /* AUDIORTPANALYZER_HPP_ */
