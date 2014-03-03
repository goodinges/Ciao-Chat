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

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "VideoInterfaces.hpp"
#include "TimingStat.hpp"
#include "lib/vpu/vpu_io.h"

/*! \class Renderer
 *  V4L2 based video renderer implementation for Freescale i.MX27.
 */
class Renderer : public VideoRenderer {
public:
	Renderer(VideoParams* params);
	~Renderer();
	void RenderFrame(VideoData& frame);
	void* InitFrameBuffers(uint32_t* bufCount);
private:
	void OpenDevice();
	void ConfigureDev();
	void UnConfigureDev();
	void CloseDevice();
	void GetCropRatio(int& width, int& height, int maxwidth, int maxheight);

	TimingStat m_Timing;
	int m_Fd;
	int m_FrameBufferCount;
	vpu_mem_desc* m_DispBufs;
	bool m_Rotate;
	bool m_InitDone;
	pMutex m_Mutex;
};

#endif /* RENDERER_HPP_ */
