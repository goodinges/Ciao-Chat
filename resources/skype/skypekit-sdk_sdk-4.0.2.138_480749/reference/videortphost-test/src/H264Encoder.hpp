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

#ifndef H264ENCODER_HPP_
#define H264ENCODER_HPP_

#include "VideoInterfaces.hpp"

#include <stdint.h>
extern "C" {
#include <x264.h>
}

#include "ptasking.hpp"
#include "BitrateStat.hpp"
#include "SlidingAvg.hpp"

/*! \class H264Encoder
 *  A video frame encoder implementation using x264 codec
 *  Gets unencoded video stream in constructor. Implements start encoding, stop encoding, generate key frame and change bitrate methods.
 */
class H264Encoder : public VideoEncoder, public pThread
{
public:
	H264Encoder(VideoParams *params, VideoCapturer *vCap, FrameCallback *cb);
	~H264Encoder();

	void ChangeBitrate(uint32_t bitrate);
	void ChangeFramerate(uint32_t framerate);
	void GenerateKeyFrame();
	void StartEncoding();
	void StopEncoding();

private:
	void InitParameters(VideoParams &params);
	void RestartEncoder(VideoParams &params);
	void ReconfigureEncoder(VideoParams &params);
	void Run();

	int m_ForceKeyFrame;
	void *m_LibHandle;
	int m_PlayTime;
	x264_t *m_pX264Encoder;			//encoder
	x264_param_t *m_pX264param;		//encoder params
	x264_picture_t *m_pInputPic;	//decoded picture
	BitrateStat m_BStat;
	SlidingAvg<int32_t, 30> m_FpsAvg;
	uint64_t m_StartTime;
};

#endif /* H264ENCODER_HPP_ */
