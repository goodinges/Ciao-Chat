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

#ifndef RECEIVEVIDEOWRAPPER_HPP_
#define RECEIVEVIDEOWRAPPER_HPP_

#include "Configuration.hpp"
#include "VideoInterfaces.hpp"
#include "H264RtpToAnxb.hpp"
#include "PacketDump.hpp"
#include "Renderer.hpp"
#include "Decoder.hpp"

class ReceiveVideoWrapper;

/*! \class H264RtpToAnxbCb
 *  Implements TransmitAnxbFrame method (RTP packet as H264 frame).
 */
class H264RtpToAnxbCb: public H264RtpToAnxb::AnxbFrameCallback {
public:
	H264RtpToAnxbCb(ReceiveVideoWrapper& receive) :
		m_pReceive(receive),
		m_FrameCnt(0) {
	}
	void TransmitAnxbFrame(const uint8_t *data, uint32_t len, int timestamp_ms);
	ReceiveVideoWrapper& m_pReceive;
	uint32_t m_FrameCnt;
};

/*! \class ReceiveVideoWrapper
 *  Wrapper for decoder and renderer
 */
class ReceiveVideoWrapper {
	friend class H264RtpToAnxbCb;
public:
	ReceiveVideoWrapper(VideoParams& vidPar);
	~ReceiveVideoWrapper();
	void SubmitPacket(const uint8_t *packet, uint32_t packetLen);
	void SubmitFrame(VideoData& frame);
private:
	H264RtpToAnxbCb m_RtpToAnxbCb;
	H264RtpToAnxb m_RtpToAnxb;

	PacketDump* m_pReceiveRtpDump;
	PacketDump* m_pReceiveAnxbDump;

	VideoRenderer* m_pRenderer;
	VideoDecoder* m_pDecoder;
};

#endif /* RECEIVEVIDEOWRAPPER_HPP_ */
