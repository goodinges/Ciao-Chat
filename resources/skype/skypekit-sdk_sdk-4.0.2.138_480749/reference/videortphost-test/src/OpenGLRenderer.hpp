/*
 * OpenGLRenderer.hpp
 *
 *  Created on: Mar 23, 2011
 *      Author: lauri
 */

#ifndef OPENGLRENDERER_HPP_
#define OPENGLRENDERER_HPP_

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

#include "VideoInterfaces.hpp"

#include <stdio.h>

#include "TimingStat.hpp"
#include "StaticSyncQueue.hpp"
#include "ptasking.hpp"

#define FRAME_QUEUE_LEN 8

/*! \class OpenGLRenderer
 *  Renders video with OpenGL methods
 */
class GlutContext {
	void Init() {}
	void UnInit() {}
}

class OpenGLRenderer : public VideoRenderer, public pThread
{
public:
	OpenGLRenderer(VideoParams *params, const char *title, int locX, int locY);
	~OpenGLRenderer();
	void RenderFrame(VideoData & frame);

private:
	void RenderFrameInt(VideoData &frame);
	void InitVideo(int width, int height);
	void UninitVideo();
	void Run();

	FILE *m_pFh;

	const char *m_pTitle;
	int m_LocX, m_LocY;
	bool m_InitDone;
	VideoData* m_pFrameQueueBufs[FRAME_QUEUE_LEN];
	StaticSyncQueue<VideoData*> *m_pFrameQueue;
	bool m_UsingXv;
	uint8_t *m_RgbBuf;
	TimingStat m_TStat;
};

#endif /* OPENGLRENDERER_HPP_ */
