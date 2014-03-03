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

#ifndef V4L2CAPTURER_HPP_
#define V4L2CAPTURER_HPP_

#include "VideoInterfaces.hpp"
#include "TimingStat.hpp"
#include "V4l2Capture.hpp"

struct FrameBuf {
	void *start;
	size_t length;
};

/*! \class V4l2Capturer
 *  Implements Video for Linux ( V4l2 ) frame capture method.
 *  Works with webcams that have V4l2 driver available
 */
class V4l2Capturer : public VideoCapturer
{
public:
	V4l2Capturer(VideoParams *params, VideoRenderer *previewRenderer);
	~V4l2Capturer();
	void CaptureFrame(VideoData & frame);
	void ChangeFramerate(uint32_t framerate) {}

private:
	void OpenDevice();
	bool TryFormat(uint32_t fmt);
	bool SetFrameRate(int fps);
	void ConfigureDev();
	void StartCapturing();
	void StopCapturing();
	void UnConfigureDev();
	void CloseDevice();
	void ReadFrame(struct v4l2_buffer *buf);

	V4l2Capture m_VCap;
	TimingStat m_TStat;
    uint32_t m_CameraPixFmt;
    uint8_t *m_YUYVBuf;
    int m_LastTimestamp;
};


#endif /* V4L2CAPTURE_HPP_ */
