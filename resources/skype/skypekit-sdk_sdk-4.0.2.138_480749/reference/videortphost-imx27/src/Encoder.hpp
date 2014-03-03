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

#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include "VideoInterfaces.hpp"
#include "TimingStat.hpp"
#include "BitrateStat.hpp"
#include "ptasking.hpp"
#include "lib/vpu/vpu_io.h"
#include "lib/vpu/vpu_lib.h"

#define SEARCHRAM_ADDR 0xFFFF4C00
#define SIZE_BITSTREAM_BUFS 0x40000

/*! \class Encoder
 *  Video encoder implementation on Freescale i.MX27.
 */
class Encoder : public VideoEncoder, public pThread {
public:
	Encoder(VideoParams* params, VideoCapturer* vCap, FrameCallback* cb);
	~Encoder();

	void StartEncoding();
	void StopEncoding();
	void GenerateKeyFrame();
	void ChangeBitrate(uint32_t bitrate);
	void ChangeFramerate(uint32_t framerate);
private:
	void InitEncoder();
	void UninitEncoder();
	void Run();
	void EncodeFrame(VideoData& inFrame);
	void GenerateHeader(AvcHeaderType headerType);

	TimingStat m_Timing;
	BitrateStat m_Bitrate;
	vpu_mem_desc* m_FrameBufferPool;
	FrameBuffer* m_FrameBuffers;
	vpu_mem_desc m_OutBuf;
	FrameBuffer m_SourceFrame;
	EncParam m_Param;
	EncHandle m_Handle;
	int m_FrameBufferCount;
	bool m_InitDone;
	uint64_t m_StartTime;
};

#endif /* ENCODER_HPP_ */
