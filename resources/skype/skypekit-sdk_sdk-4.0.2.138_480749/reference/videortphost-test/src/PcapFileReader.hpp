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
 *
 *  Created on: Aug 5, 2009
 *      Author: lauri
 */

#ifndef PCAPFILEREADER_HPP_
#define PCAPFILEREADER_HPP_

#include "VideoInterfaces.hpp"
#include "PcapHandler.hpp"

class PcapFileReader : public VideoEncoder, public pThread
{
public:
	PcapFileReader(VideoParams *vParams, VideoCapturer *vCap, FrameCallback *cb_);
	~PcapFileReader();

	void ChangeBitrate(uint32_t bitrate);
	void ChangeFramerate(uint32_t framerate) {}
	void GenerateKeyFrame();
	void StartEncoding();
	void StopEncoding();

private:
	void Run(void);
	void AddFillerUnit(uint8_t *buf, uint32_t len);
	
	PcapReader m_PcapFile;
	uint8_t *m_pPacketBuf;
	uint32_t m_PacketBufLen;
};

#endif /* PCAPFILEREADER_HPP_ */
