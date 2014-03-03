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

#ifndef RTPTOOL_HPP_
#define RTPTOOL_HPP_

#include "FileReader.hpp"
#include "H264RtpAnalyzer.hpp"
#include "AudioRtpAnalyzer.hpp"
#include "FileWriter.hpp"

class RTPTool;

class ReaderCallback: public FileReader::FrameCallback {
public:
	ReaderCallback(RTPTool &checkrtp) :
		m_RTPTool(checkrtp) {
	}
	void TransmitFrame(uint8_t *data, int len);
	RTPTool &m_RTPTool;
};

class AudioRtpAnalyzerCb: public AudioRtpAnalyzer::AudioFrameCallback {
public:
	AudioRtpAnalyzerCb(RTPTool &checkrtp) :
		m_RTPTool(checkrtp) {
	}
	void TransmitAudioFrame(uint8_t *data, uint32_t len);
	RTPTool &m_RTPTool;
};

class H264RtpAnalyzerCb: public H264RtpAnalyzer::AnxbFrameCallback {
public:
	H264RtpAnalyzerCb(RTPTool &checkrtp) :
		m_RTPTool(checkrtp) {
	}
	void TransmitAnxbFrame(uint8_t *data, uint32_t len);
	RTPTool &m_RTPTool;
};

class RTPTool {
public:
	RTPTool(int inMedia, int inFormat, const char *inName, int outFormat, const char *outName, int log);
	~RTPTool();
	void AnalyzeRTP();

	FileReader *m_pReader;
	ReaderCallback m_ReaderCb;

	AudioRtpAnalyzerCb m_RtpToAudioCb;
	AudioRtpAnalyzer m_RtpToAudio;
	H264RtpAnalyzerCb m_RtpToAnxbCb;
	H264RtpAnalyzer m_RtpToAnxb;

	FileWriter *m_pWriter;
	int m_MediaType;
	int m_InFormat;
	int m_OutFormat;
	int m_Log;
};

#endif /* RTPTOOL_HPP_ */
