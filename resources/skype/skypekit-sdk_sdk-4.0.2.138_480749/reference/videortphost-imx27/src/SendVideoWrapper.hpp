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

#ifndef SENDVIDEOWRAPPER_HPP_
#define SENDVIDEOWRAPPER_HPP_

#include "iMX27RtpInterface.hpp"
#include "Configuration.hpp"
#include "H264AnxbToRtp.hpp"
#include "VideoInterfaces.hpp"
#include "PacketDump.hpp"
#include "H264AnxbFileReader.hpp"
#include "PcapFileReader.hpp"
#include "Capturer.hpp"
#include "DummyCapturer.hpp"
#include "Encoder.hpp"

class SendVideoWrapper;

class VideoEncCallback: public VideoEncoder::FrameCallback {
public:
	VideoEncCallback(SendVideoWrapper& send) :
		m_pSend(send),
		m_FrameCnt(0) {
	}
	void TransmitFrame(VideoData& frame);
	SendVideoWrapper& m_pSend;
	int m_FrameCnt;
};

class H264AnxbToRtpCb: public H264AnxbToRtp::RtpPacketCallback {
public:
	H264AnxbToRtpCb(SendVideoWrapper& send) :
		m_pSend(send) {
	}
	void TransmitRtpPacket(const uint8_t* data, uint32_t len);
	SendVideoWrapper& m_pSend;
};

/*! \class SendVideoWrapper
 *  Wrapper for capturer and encoder
 */
class SendVideoWrapper {
public:
	SendVideoWrapper(SendPacketCallback& sendCb, int initialMaxPacketSize, VideoParams& vidPar);
	~SendVideoWrapper();
	void RequestKeyFrame();
	void SetBitrate(uint32_t bitrate);
	void SetFramerate(uint32_t framerate);
	void SetMaxPacketSize(uint32_t maxPacketSize);

	void Run();
	SendPacketCallback& m_pSendCallback;
	VideoEncCallback m_VidEncCb;
	H264AnxbToRtpCb m_AnxbToRtpCb;
	H264AnxbToRtp m_AnxbToRtp;

	PacketDump* m_pSendRtpDump;
	PacketDump* m_pSendAnxbDump;
	VideoCapturer* m_pCapturer;
	VideoEncoder* m_pEncoder;
};
#endif /* SENDVIDEOWRAPPER_HPP_ */
