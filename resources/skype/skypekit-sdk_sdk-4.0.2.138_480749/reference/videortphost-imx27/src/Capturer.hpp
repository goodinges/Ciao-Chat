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

#ifndef CAPTURER_HPP_
#define CAPTURER_HPP_

#include "VideoInterfaces.hpp"
#include "TimingStat.hpp"
#include "lib/vpu/vpu_io.h"

#define CAP_OUT_QUEUE_LEN 2

/*! \class Capturer
 *  V4L2 based capturer implementation for Freescale i.MX27.
 */
class Capturer : public VideoCapturer {
public:
	Capturer(VideoParams* params, VideoRenderer* previewRenderer);
	~Capturer();
	void StartCapturing();
	void StopCapturing();
	void CaptureFrame(VideoData & frame);
	void ChangeFramerate(uint32_t framerate);
private:
	void OpenDevice();
	void ConfigureDev();
	void UnConfigureDev();
	void CloseDevice();

	TimingStat m_Timing;
	int m_Fd;
	int m_BufferCount;
	int m_LastTimestamp;
	vpu_mem_desc* m_CapBufs;
	vpu_mem_desc m_OutBuf;
	pMutex m_Mutex;
};

#endif /* CAPTURER_HPP_ */
