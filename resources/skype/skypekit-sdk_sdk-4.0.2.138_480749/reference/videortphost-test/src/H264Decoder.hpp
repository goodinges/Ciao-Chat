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
#ifndef H264DECODER_HPP_
#define H264DECODER_HPP_

#include "VideoInterfaces.hpp"

#include "ptasking.hpp"

struct AVCodecContext;
struct AVFrame;

/*! \class H264Decoder
 *  Implements H264 DecodeFrame method using libavcodec built-in h264 decoder
 */
class H264Decoder : public VideoDecoder 
{
public:
	H264Decoder(VideoParams *params, VideoRenderer *vRend);
	~H264Decoder();

	void DecodeFrame(VideoData & frame);

private:
	void CopyDataFromFrame();
	
	pMutex decodeMutex;
	AVCodecContext *m_pCodecCtx;
	AVFrame *m_pFrame;
	uint8_t *m_pFrameBuf;
	int m_pFrameBufLen;
	uint8_t *m_pYPlane;
	uint8_t *m_pUPlane;
	uint8_t *m_pVPlane;
	int m_LastTimestamp;
};
#endif 
