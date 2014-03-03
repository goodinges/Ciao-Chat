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

#ifndef DUMMYCAPTURER_HPP_
#define DUMMYCAPTURER_HPP_

#include <stdint.h>
#include "VideoInterfaces.hpp"
#include "TimingStat.hpp"

/*! \class DymmyCapture
 *  Generates YUV420 frames with random content
 */
class DummyCapturer : public VideoCapturer {
public:
	DummyCapturer(VideoParams *params, VideoRenderer *previewRenderer);
	~DummyCapturer();
	void StartCapturing() {}
	void StopCapturing() {}
	void CaptureFrame(VideoData & frame);
	void ChangeFramerate(uint32_t framerate) {}

private:

	TimingStat m_TStat;
	int curr_Timestamp;
};

#endif /* DUMMYCAPTURER_HPP_ */
