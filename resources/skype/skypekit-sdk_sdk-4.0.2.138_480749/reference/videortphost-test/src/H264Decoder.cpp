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
#define __STDC_CONSTANT_MACROS
#include "H264Decoder.hpp"


extern "C" {

#include <libavcodec/avcodec.h>
}
#include <string.h>

#include "RtpTeLogging.hpp"

H264Decoder::H264Decoder(VideoParams *params, VideoRenderer *vRend) :
	VideoDecoder(params, vRend),
	m_pFrameBuf(0),
	m_pFrameBufLen(0)
{
	pMutexLock l(decodeMutex);
	AVCodec *pCodec;

	avcodec_init();
	avcodec_register_all();

	pCodec = avcodec_find_decoder(CODEC_ID_H264);
	if(pCodec == NULL)
	    FATAL("avcodec_find_decoder failed");
/*
	// Inform the codec that we can handle truncated bitstreams -- i.e.,
	// bitstreams where frame boundaries can fall in the middle of packets
	if(pCodec->capabilities & CODEC_CAP_TRUNCATED)
	    pCodecCtx->flags|=CODEC_FLAG_TRUNCATED;
*/
	// Open codec
	m_pCodecCtx = avcodec_alloc_context();
	if (m_pCodecCtx == NULL)
	    FATAL("avcodec_alloc_context failed");

	if(avcodec_open(m_pCodecCtx, pCodec)<0)
	    FATAL("avcodec_open failed");

	m_pFrame = avcodec_alloc_frame();
	if (m_pFrame == NULL)
	    FATAL("avcodec_alloc_frame failed");
}

H264Decoder::~H264Decoder() {
	av_free(m_pFrame);
	avcodec_close(m_pCodecCtx);
	delete[] m_pFrameBuf;
}

void H264Decoder::CopyDataFromFrame() {
	int desiredBufLen = m_pCodecCtx->width * m_pCodecCtx->height * 2;

	if (m_pFrameBufLen < desiredBufLen) {
		DEC_DBG("CopyDataFromFrame: reallocate buffer: old buf: %d, new buf: %d", m_pFrameBufLen, desiredBufLen);
		if (m_pFrameBuf)
			delete m_pFrameBuf;
		m_pFrameBuf = new uint8_t[desiredBufLen];
		m_pFrameBufLen = desiredBufLen;
	}

	int plane_y_idx = 0;
	int plane_u_idx = 1;
	int plane_v_idx = 2;

	uint8_t *dest = m_pFrameBuf;
	for (int z = 0; m_pFrame->data[plane_y_idx] && z < m_pCodecCtx->height; z++)
	{
		void * dataToWrite = (void*)&(m_pFrame->data[plane_y_idx][m_pFrame->linesize[plane_y_idx]*z]);
		memcpy(dest, dataToWrite, m_pCodecCtx->width);
		dest += m_pCodecCtx->width;
	}
	for (int z = 0; m_pFrame->data[plane_u_idx] && z<m_pCodecCtx->height/2; z++)
	{
		void * dataToWrite = (void*)&(m_pFrame->data[plane_u_idx][m_pFrame->linesize[plane_u_idx]*z]);
		memcpy(dest, dataToWrite, m_pCodecCtx->width / 2);
		dest += m_pCodecCtx->width / 2;
	}
	for (int z = 0; m_pFrame->data[plane_v_idx] && z < m_pCodecCtx->height/2; z++)
	{
		void * dataToWrite = (void*)&(m_pFrame->data[plane_v_idx][m_pFrame->linesize[plane_v_idx]*z]);
		memcpy(dest, dataToWrite, m_pCodecCtx->width / 2);
		dest += m_pCodecCtx->width / 2;
	}	
}

void H264Decoder::DecodeFrame(VideoData & frame) {
	int decodedLen;
	int gotPicture;
	pMutexLock l(decodeMutex);

	DEC_DBG("width: %d height: %d, timestamp %d, diff: %d", m_pCodecCtx->width, m_pCodecCtx->height, frame.GetTimestamp(), frame.GetTimestamp() - m_LastTimestamp);
	m_LastTimestamp = frame.GetTimestamp();

#if (LIBAVCODEC_VERSION_MAJOR == 52 && LIBAVCODEC_VERSION_MINOR > 20) || LIBAVCODEC_VERSION_MAJOR > 52
	AVPacket avpkt;
	avpkt.pts = AV_NOPTS_VALUE;
	avpkt.dts = AV_NOPTS_VALUE;
	avpkt.data = (uint8_t *)frame.Data();
	avpkt.size = frame.Size();
	avpkt.duration = 0;
	decodedLen = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &gotPicture, &avpkt);
#else
	decodedLen = avcodec_decode_video(m_pCodecCtx, m_pFrame, &gotPicture, frame.Data(), frame.Size());
#endif
	
	if (decodedLen < 0) {
		ERROR("avcodec_decode_video failed: res: %d, gotPicture: %d", decodedLen, gotPicture);
		return;
	}
	
	if (decodedLen != frame.Size())
		WARN("decoded less data: given %d, decoded %d", frame.Size(), decodedLen);
	
	CopyDataFromFrame();
	VideoData dec_frame;
	dec_frame.SetBuf(m_pFrameBuf, decodedLen);
	dec_frame.SetTimestamp(frame.GetTimestamp());
	m_pVRend->RenderFrame(dec_frame);
}
