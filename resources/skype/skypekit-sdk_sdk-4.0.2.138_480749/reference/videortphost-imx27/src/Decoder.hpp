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

#ifndef DECODER_HPP_
#define DECODER_HPP_

#include "VideoInterfaces.hpp"
#include "TimingStat.hpp"
#include "BitrateStat.hpp"
#include "StaticSyncQueue.hpp"
#include "ptasking.hpp"
#include "lib/vpu/vpu_io.h"
#include "lib/vpu/vpu_lib.h"

#define DEC_IN_QUEUE_LEN 2
#define	SIZE_BITSTREAM_BUFS 0x40000
#define PS_SAVE_SIZE 0x028000
#define SLICE_SAVE_SIZE 0x02D800

/*! \class Decoder
 *  Video decoder implementation on Freescale i.MX27.
 */
class Decoder : public VideoDecoder, public pThread {
public:
	Decoder(VideoParams* params, VideoRenderer* vRend);
	~Decoder();
	void DecodeFrame(VideoData& frame);
private:
	void InitDecoder();
	void UninitDecoder();
	void Run();
	RetCode GetInitialInfo();

	TimingStat m_Timing;
	BitrateStat m_Bitrate;
	vpu_mem_desc m_BitStreamBuffer;
	vpu_mem_desc m_PSSaveBuffer;
	vpu_mem_desc m_SliceSaveBuffer;
	StaticSyncQueue<vpu_mem_desc *> *m_pInQueue;
	vpu_mem_desc** m_InQueueBuffers;
	FrameBuffer* m_FrameBuffers;
	DecParam m_Param;
	DecHandle m_Handle;
	int m_ClrDispFrame;
	int m_FrameBufferCount;
	int m_RotatorBufferOffset;
	int m_LastTimestamp;
	bool m_Rotate;
	bool m_InitDone;
};

#endif /* DECODER_HPP_ */
