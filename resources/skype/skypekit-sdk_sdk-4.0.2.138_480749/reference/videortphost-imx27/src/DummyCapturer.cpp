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

#include "DummyCapturer.hpp"
#include "LoggingCtrl.hpp"
#include <stdlib.h>
#include <unistd.h>

DummyCapturer::DummyCapturer(VideoParams *params, VideoRenderer *previewRenderer) :
	VideoCapturer(params, previewRenderer),
	curr_Timestamp(0)
{
}

DummyCapturer::~DummyCapturer() {
}

void DummyCapturer::CaptureFrame(VideoData& frame) {
	uint32_t sleepTimeUs;
	m_TStat.Update();
	TIMING_DBG("V4l2Capturer: %s", m_TStat.GetStatStr());
	uint8_t *frameptr = (uint8_t *)frame.Data();
	curr_Timestamp += 1000 / m_VPar.framerate;
	frame.m_Timestamp = curr_Timestamp;

	for (int i = 0; i < m_VPar.width * m_VPar.height; i++) {
		*frameptr = 0x13 + (int)(5 * ((float)rand()/RAND_MAX));
		frameptr++;
	}

	for (int i = 0; i < m_VPar.width * m_VPar.height / 2; i++) {
		*frameptr = 0x80 + (int)(5 * ((float)rand()/RAND_MAX));
		frameptr++;
	}

	m_pPreviewRenderer->RenderFrame(frame);

	sleepTimeUs = (uint32_t)(1 / (float)m_VPar.framerate * 1000000);
	CAP_DBG("usleep %d us", sleepTimeUs);
	usleep(sleepTimeUs);
}
