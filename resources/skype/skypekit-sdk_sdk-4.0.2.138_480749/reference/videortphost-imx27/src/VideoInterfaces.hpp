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

#ifndef VIDEOREADER_HPP_
#define VIDEOREADER_HPP_

#include <stdint.h>
#include <linux/videodev2.h>

#include "H264AnxbToRtp.hpp"
#include "H264RtpToAnxb.hpp"

/*! \struct VideoParams
 *  Video parameters structure.
 */
struct VideoParams {
	char devName[512];
	int bitrate;
	int framerate;
	int width;
	int height;
	uint32_t pixFmt;
	pMutex* mutex;
	int FrameSize() {
		switch (pixFmt) {
		case V4L2_PIX_FMT_UYVY: // YUV 4:2:2 (FOURCC=UYVY)
		case V4L2_PIX_FMT_YUYV: // YUV 4:2:2 (FOURCC=YUYV)
			return width * height * 2;
		case V4L2_PIX_FMT_YUV420: // YUV 4:2:0 (FOURCC=YU12/I420)
			return width * height + width * height / 2;
		default:
			FATAL("RawFmt: Invalid PixFmt");
		}
		return -1;
	}
};

struct VideoData {
	int m_Height;
	int m_Width;
	int m_Compression;
	int m_Timestamp;
	const uint8_t* m_Data;
	int m_Len;
	bool m_Allocated;
public:
	VideoData() :
		m_Height(0),
		m_Width(0),
		m_Compression(0),
		m_Timestamp(0),
		m_Data(NULL),
		m_Len(0),
		m_Allocated(false) {}

	VideoData(int len) :
		m_Height(0),
		m_Width(0),
		m_Compression(0),
		m_Timestamp(0),
		m_Len(len),
		m_Allocated(true) {
		m_Data = new uint8_t[len];
	}
	~VideoData() {
		if (m_Allocated)
			delete m_Data;
	}

	void SetBuf(const uint8_t* data, int len) {m_Data = data; m_Len = len;}
	void SetHeight(int height) {m_Height = height;}
	void SetWidth(int width) {m_Width = width;}
	void SetCompression(unsigned int compression) {m_Compression = compression;}
	void SetTimestamp(unsigned int timestamp) {m_Timestamp = timestamp;}

	int GetHeight() {return m_Height;}
	int GetWidth() {return m_Width;}
	int GetCompression() {return m_Compression;}
	int GetTimestamp() {return m_Timestamp;}

	const uint8_t* Data() {return m_Data;}
	int Size() {return m_Len;}
};

class VideoRenderer;

/*! \class VideoCapturer
 *  VideoCapturer base class. See Capturer for reference implementation.
 */
class VideoCapturer {
public:
	VideoCapturer(VideoParams* params, VideoRenderer* previewRenderer) :
		m_VPar(*params), m_pPreviewRenderer(previewRenderer) {};
	virtual ~VideoCapturer() {};
	virtual void StartCapturing() = 0;
	virtual void StopCapturing() = 0;
	virtual void CaptureFrame(VideoData& frame) = 0;
	virtual void ChangeFramerate(uint32_t framerate) = 0;

	VideoParams m_VPar;
	VideoRenderer* m_pPreviewRenderer;
};

/*! \class VideoEncoder
 *  VideoEncoder base class. See Encoder for reference implementation.
 */
class VideoEncoder {
public:
	struct FrameCallback {
	public:
		virtual void TransmitFrame(VideoData& frame) = 0;
	};
	VideoEncoder(VideoParams* params, VideoCapturer* vCap, FrameCallback* cb) :
		m_VPar(*params),
		m_pVCap(vCap),
		m_pCb(cb) {};
	virtual ~VideoEncoder() {};

	virtual void ChangeBitrate(uint32_t bitrate) = 0;
	virtual void ChangeFramerate(uint32_t framerate) = 0;
	virtual void GenerateKeyFrame() = 0;
	virtual void StartEncoding() = 0;
	virtual void StopEncoding() = 0;

	VideoParams m_VPar;
	VideoCapturer* m_pVCap;
	FrameCallback* m_pCb;
};

/*! \class VideoRenderer
 *  VideoRenderer base class. See Renderer for reference implementation.
 */
class VideoRenderer {
public:
	VideoRenderer(VideoParams* params) :
		m_VPar(*params) {};
	virtual ~VideoRenderer() {};
	virtual void RenderFrame(VideoData& frame) = 0;
	virtual void* InitFrameBuffers(uint32_t* bufCount) = 0;

	VideoParams m_VPar;
};

/*! \class VideoDecoder
 *  VideoDecoder base class. See Decoder for reference implementation.
 */
class VideoDecoder {
public:
	VideoDecoder(VideoParams* params, VideoRenderer* vRend) :
		m_VPar(*params),
		m_pVRend(vRend) {};
	virtual ~VideoDecoder() {};

	virtual void DecodeFrame(VideoData& frame) = 0;

	VideoParams m_VPar;
	VideoRenderer* m_pVRend;
};

#endif /* VIDEOREADER_HPP_ */
