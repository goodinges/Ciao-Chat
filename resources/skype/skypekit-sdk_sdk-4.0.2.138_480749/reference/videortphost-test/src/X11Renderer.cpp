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

#include "X11Renderer.hpp"

#include <stdio.h>
#include <string.h>
#include <X11/Xutil.h>

#include "RtpTeLogging.hpp"
#include "ColorConvert.hpp"

pMutex X11mutex;

#define GUID_I420_PLANAR 0x30323449

static uint64_t getTimeUs() {
	struct timeval tv;
	uint64_t ret;
	gettimeofday(&tv, NULL);
	ret = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
	return ret;
}


X11Renderer::X11Renderer(VideoParams *params, const char *title, int locX, int locY) :
	VideoRenderer(params),
	pThread(false),
	m_pTitle(title),
	m_LocX(locX),
	m_LocY(locY),
	m_TStat()
{
	CreateWindow();
	InitVideo();

	m_RgbBuf = new uint8_t[m_VPar.height * m_VPar.width * 4];

	for (int i = 0; i < FRAME_QUEUE_LEN; i++)
		m_pFrameQueueBufs[i] = new VideoData(m_VPar.FrameSize());

	m_pFrameQueue = new StaticSyncQueue<VideoData*>(m_pFrameQueueBufs, FRAME_QUEUE_LEN);
	Start(); // start the thread
}

X11Renderer::~X11Renderer() {	
	m_pFrameQueue->WakeWait();
	Stop();
	delete m_pFrameQueue;

	for (int i = 0; i < FRAME_QUEUE_LEN; i++)
		delete m_pFrameQueueBufs[i];
	
	DeleteWindow();
	UninitVideo();

	delete[] m_RgbBuf;
}

void X11Renderer::CreateWindow() {
	pMutexLock l(X11mutex);
	uint32_t black, white;

	m_pDisplay = XOpenDisplay(NULL);
	if (!m_pDisplay)
		FATAL("XOpenDisplay failed");

	m_Screen = DefaultScreen(m_pDisplay);

	black = BlackPixel(m_pDisplay, m_Screen);
	white = WhitePixel(m_pDisplay, m_Screen);
		
	m_Window = XCreateSimpleWindow(m_pDisplay, DefaultRootWindow(m_pDisplay), 0, 0, m_VPar.width,
			m_VPar.height, 2, white, black);

	XSetStandardProperties(m_pDisplay, m_Window, m_pTitle, m_pTitle, None, NULL, 0, NULL);

	XSelectInput(m_pDisplay, m_Window, ExposureMask|ButtonPressMask|KeyPressMask);
 
	m_Gc = XCreateGC(m_pDisplay, m_Window, 0, 0);	
 
    XMapWindow(m_pDisplay, m_Window);
	XMoveWindow(m_pDisplay, m_Window, m_LocX, m_LocY);
    XFlush(m_pDisplay);	
}

void X11Renderer::DeleteWindow() {
	pMutexLock l(X11mutex);
	XFreeGC(m_pDisplay, m_Gc);
	XDestroyWindow(m_pDisplay, m_Window);
	XUnlockDisplay(m_pDisplay);
	XCloseDisplay(m_pDisplay);	
}

void X11Renderer::InitVideo() {
	pMutexLock l(X11mutex);

#ifndef FORCE_SOFT_RENDERING
	if (InitVideoXv()) {
		m_UsingXv = 1;
	} else {
#endif
		WARN("XV initialization failed, using soft rendering, which is much slower");
		m_UsingXv = 0;
		InitVideoX();
#ifndef FORCE_SOFT_RENDERING
	}
#endif
}

bool X11Renderer::InitVideoXv() {
	XvAdaptorInfo *ai;
	uint32_t adaptors;
	XvPortID xvP;
	int ret;

	ret = XvQueryAdaptors(m_pDisplay, DefaultRootWindow(m_pDisplay), &adaptors, &ai);
	if (ret != Success) {
		ERROR("XvQueryAdaptors failed");
		return false;
	}

	REND_DBG("adaptors: %d", adaptors);
    m_Port = 0;
	for (unsigned int i = 0; i < adaptors; i++) {
		REND_DBG("adaptor %d", adaptors);
		if ((ai[i].type & ( XvInputMask | XvImageMask)) == (XvInputMask | XvImageMask)) {
			for (xvP = ai[i].base_id; xvP<ai[i].base_id+ai[i].num_ports; xvP++ ) {
				REND_DBG("Port %d", xvP);
				if (XvGrabPort( m_pDisplay, xvP, CurrentTime ) == Success) {
					m_Port = xvP;
					REND_DBG("Got port %d\n", m_Port);
					break;
				}
			}
			if ( m_Port != 0 )
				break;
		}	
	}
	XvFreeAdaptorInfo(ai);

	if (!m_Port) {
		ERROR("Failed to grab port");
		return false;
	}

	m_pXvImage = XvCreateImage(m_pDisplay, m_Port, GUID_I420_PLANAR, NULL, m_VPar.width, m_VPar.height);

	if (!m_pXvImage) {
		ERROR("XvCreateImage failed");
		return false;
	}

	REND_DBG("Image data: 0x%x, size: %d", m_pXvImage->data, m_pXvImage->data_size);
	return true;
}

void X11Renderer::InitVideoX() {

	m_pXImage = XCreateImage(m_pDisplay, DefaultVisual(m_pDisplay, 0), 24, ZPixmap, 0, NULL, 
			m_VPar.width, m_VPar.height, 32, 0);
	if (!m_pXImage)
		FATAL("XCreateImage failed");

	REND_DBG("Image data: 0x%x, bytes_per_line: %d, bits_per_pixel: %d", m_pXImage->data, m_pXImage->bytes_per_line, m_pXImage->bits_per_pixel);
	REND_DBG("red_mask: 0x%x, blue_mask: 0x%x, green_mask: 0x%x", m_pXImage->red_mask, m_pXImage->blue_mask, m_pXImage->green_mask);
}

void X11Renderer::RenderFrameInt(VideoData *frame) {
	int ret;
	pMutexLock l(X11mutex);

	m_TStat.Update();
	TIMING_DBG("X11Renderer(%s): %s", m_pTitle, m_TStat.GetStatStr());

	if (m_UsingXv) {
		m_pXvImage->data = (char *)frame->Data();
		ret = XvPutImage(m_pDisplay, m_Port, m_Window, m_Gc, m_pXvImage,
					0, 0, m_VPar.width, m_VPar.height,
					0, 0, m_VPar.width, m_VPar.height);		
	} else {
		ColorConvert::ConvertI420toRGB(m_VPar.width, m_VPar.height, (uint8_t *)frame->Data(), m_RgbBuf);
	
		m_pXImage->data = (char *)m_RgbBuf;
		ret = XPutImage(m_pDisplay, m_Window, m_Gc, m_pXImage,
					0, 0, 0, 0,
					m_VPar.width, m_VPar.height);
	}

	if (ret != Success) {
		ERROR("X(v)PutImage failed");
	}
	XFlush(m_pDisplay);	
}

void X11Renderer::Run(void) {
	VideoData *frameBuf;
	int lastTimestampMs = 0;

	while (!Shutdown) {
		uint64_t renderStartUs = getTimeUs();
		frameBuf = m_pFrameQueue->GetReadPtr();
		if (!frameBuf) {
			break;
		}
		RenderFrameInt(frameBuf);

		m_pFrameQueue->ReadDone();
		if (lastTimestampMs) {
			// consider sleep between frames only if queue not getting full
			if (m_pFrameQueue->GetCnt() < FRAME_QUEUE_LEN / 2) {
				int elapsedUs = (int)(getTimeUs() - renderStartUs);
				int timestampDiffUs = (frameBuf->GetTimestamp() - lastTimestampMs) * 1000;
				int sleepTimeUs = timestampDiffUs - elapsedUs;
				if (sleepTimeUs > 0 && sleepTimeUs < 200000) {
					//REND_DBG("X11Renderer::Run usleep: %d, elapsed: %d, timestampdiff: %d", sleepTimeUs, elapsedUs, timestampDiffUs);
					usleep(sleepTimeUs);
				} else {
					REND_DBG("X11Renderer::Run DONT usleep: %d, elapsed: %d, timestampdiff: %d", timestampDiffUs, elapsedUs, timestampDiffUs);
				}
			} else {
				REND_DBG("X11Renderer::Run DONT usleep: queue %d", m_pFrameQueue->GetCnt());
			}
		}
		lastTimestampMs = frameBuf->GetTimestamp();
	}
	REND_DBG("Exit X11Renderer::Run (%s)", m_pTitle)
}

void X11Renderer::UninitVideo() {
	pMutexLock l(X11mutex);

	if (m_UsingXv) {
		XFree(m_pXvImage);		
	} else {
		XFree(m_pXImage);
	}
}

void X11Renderer::RenderFrame(VideoData & frame) {
	VideoData *frameBuf;
//	RenderFrameInt(yuvBuf);
	// Renderer can be too slow when software rendering is used. So 
	// we just drop some frames so the slow renrerer can't slow  everything down 
	if (m_pFrameQueue->GetCnt() == FRAME_QUEUE_LEN) {
		REND_DBG("%s: Dropping frame", m_pTitle);
	} else {
		frameBuf = m_pFrameQueue->GetWritePtr();
		memcpy((uint8_t *)frameBuf->Data(), frame.Data(), m_VPar.FrameSize());
		frameBuf->SetTimestamp(frame.GetTimestamp());
		m_pFrameQueue->WriteDone();
	}
}
