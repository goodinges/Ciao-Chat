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

#include "V4l2Capturer.hpp"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/videodev2.h>

#include "RtpTeLogging.hpp"
#include "ColorConvert.hpp"

V4l2Capturer::V4l2Capturer(VideoParams *params, VideoRenderer *previewRenderer) :
	VideoCapturer(params, previewRenderer),
	m_VCap(m_VPar.devName)
{
	
	if (!m_VCap.ConfigureDev(m_VPar.width, m_VPar.height, V4L2_PIX_FMT_YUV420, m_VPar.framerate)) {
		m_VCap.UnConfigureDev();
		if (!m_VCap.ConfigureDev(m_VPar.width, m_VPar.height, V4L2_PIX_FMT_YUYV, m_VPar.framerate)) {
			FATAL("Camera configuration failed");
		} else {
			m_CameraPixFmt = V4L2_PIX_FMT_YUYV;
			CAP_DBG("Pix fmt V4L2_PIX_FMT_YUYV");
		}
	} else {
		m_CameraPixFmt = V4L2_PIX_FMT_YUV420;
		CAP_DBG("Pix fmt V4L2_PIX_FMT_YUV420");
	}
	m_YUYVBuf = new uint8_t[m_VPar.width * m_VPar.height * 2];
}

V4l2Capturer::~V4l2Capturer()
{
	delete m_YUYVBuf;
}

void V4l2Capturer::CaptureFrame(VideoData & frame)
{	
	m_TStat.Update();
	TIMING_DBG("V4l2Capturer: %s", m_TStat.GetStatStr());
	
	if (m_CameraPixFmt == V4L2_PIX_FMT_YUYV) {
		int timestamp;
		m_VCap.CaptureFrame(m_YUYVBuf, m_VPar.width * m_VPar.height * 2, timestamp);
		frame.SetTimestamp(timestamp);
		ColorConvert::ConvertYUY2toI420(m_VPar.width, m_VPar.height, 
				m_YUYVBuf, (uint8_t *)frame.Data());
	} else {
		int timestamp;
		m_VCap.CaptureFrame((uint8_t *)frame.Data(), m_VPar.FrameSize(), timestamp);
		frame.SetTimestamp(timestamp);
	}
	CAP_DBG("frame: timestamp %d diff: %d\n", frame.GetTimestamp(), frame.GetTimestamp() - m_LastTimestamp);
	m_LastTimestamp = frame.GetTimestamp();

	m_pPreviewRenderer->RenderFrame(frame);
}
