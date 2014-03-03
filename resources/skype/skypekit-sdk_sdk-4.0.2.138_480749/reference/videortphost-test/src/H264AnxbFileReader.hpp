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
#ifndef H264ANXBFILEREADER_HPP_
#define H264ANXBFILEREADER_HPP_

#include <stdint.h>

#include "ptasking.hpp"

#include "VideoInterfaces.hpp"
#include "H264AnxbReader.hpp"
#include "BitrateStat.hpp"
#include "TimingStat.hpp"

#define MAX_FRAME_LEN 0x500000

/*! \class H264Reader
 *  Implements reading encoded video stream from a raw .264 stream file. H264 reader
 *  also inserts filler data to the video stream to maintain correct bitrate
 */
class H264AnxbFileReader : public VideoEncoder, public pThread
{
public:
	H264AnxbFileReader(VideoParams *vParams, VideoCapturer *vCap, FrameCallback *cb_);
	~H264AnxbFileReader();

	void ChangeBitrate(uint32_t bitrate);
	void ChangeFramerate(uint32_t framerate);
	void GenerateKeyFrame();
	void StartEncoding();
	void StopEncoding();

private:
	void Run(void);
	void AddFillerUnit(uint8_t *buf, uint32_t len);
	
	H264AnxbReader m_AnxbReader;
	BitrateStat m_BStat;
	TimingStat m_TStat;
	uint8_t m_pFrameBuf[MAX_FRAME_LEN];
	char m_FileName[512];
	int m_ForceKeyFrame;
	uint64_t m_StartTime;
};

#endif /* H264READER_HPP_ */
