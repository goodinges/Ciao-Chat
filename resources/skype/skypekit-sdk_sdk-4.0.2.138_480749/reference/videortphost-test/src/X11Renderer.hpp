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

#ifndef X11RENDERER_HPP_
#define X11RENDERER_HPP_

#include "VideoInterfaces.hpp"

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xvlib.h>

#include "TimingStat.hpp"
#include "StaticSyncQueue.hpp"
#include "ptasking.hpp"

#define FRAME_QUEUE_LEN 8

/*! \class X11Renderer
 *  Implements render frame method on top of X11 window
 *  Can use Xv extensions, if available, to speed up rendering
 */
class X11Renderer : public VideoRenderer, public pThread
{
public:
	X11Renderer(VideoParams *params, const char *title, int locX, int locY);
	~X11Renderer();
	void RenderFrame(VideoData & frame);

private:
	void RenderFrameInt(VideoData *frame);
	void CreateWindow();
	void DeleteWindow();
	bool InitVideoXv();
	void InitVideoX();
	void InitVideo();
	void UninitVideo();
	void Run();

	FILE *m_pFh;

	const char *m_pTitle;
	int m_LocX, m_LocY;
	Display *m_pDisplay;
	int m_Screen;
	Window m_Window;
	GC m_Gc;
	XvPortID m_Port;
	VideoData* m_pFrameQueueBufs[FRAME_QUEUE_LEN];
	StaticSyncQueue<VideoData*> *m_pFrameQueue;
	XvImage *m_pXvImage;
	bool m_UsingXv;
	XImage *m_pXImage;
	uint8_t *m_RgbBuf;
	TimingStat m_TStat;
};

#endif /* X11RENDERER_HPP_ */
