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

#include "H264Encoder.hpp"

#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

#include "RtpTeLogging.hpp"


static uint64_t getTimeUs() {
	struct timeval tv;
	uint64_t ret;
	gettimeofday(&tv, NULL);
	ret = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
	return ret;
}

H264Encoder::H264Encoder(VideoParams *params, VideoCapturer *vCap, FrameCallback *cb) :
	VideoEncoder(params, vCap, cb),
	pThread(false),
	m_ForceKeyFrame(0),
	m_BStat(3000000)
{
	m_pX264param = new x264_param_t;			//encoder params
	m_pInputPic = new x264_picture_t;		//decoded picture?

	x264_picture_alloc(m_pInputPic, X264_CSP_I420, m_VPar.width, m_VPar.height);

	m_pInputPic->i_type = X264_TYPE_AUTO;
	m_pInputPic->i_qpplus1 = 0;

	InitParameters(m_VPar);
	m_pX264Encoder = x264_encoder_open(m_pX264param);
}

H264Encoder::~H264Encoder()
{
	Stop();
	x264_picture_clean(m_pInputPic);
	x264_encoder_close(m_pX264Encoder);
	delete m_pX264param;
	delete m_pInputPic;
}

void H264Encoder::ChangeBitrate(uint32_t bitRate) {
	ENC_DBG("ChangeBitrate, %d", bitRate);
	m_VPar.bitrate = bitRate;
	RestartEncoder(m_VPar);
}

void H264Encoder::ChangeFramerate(uint32_t framerate) {
	ENC_DBG("ChangeFramerate, %d", framerate);
	m_VPar.framerate = framerate;
	RestartEncoder(m_VPar);
}

void H264Encoder::GenerateKeyFrame()
{
	ENC_DBG("Force I-frame");
	m_ForceKeyFrame = 1;
}

void H264Encoder::StartEncoding()
{
	m_StartTime = getTimeUs();
	Start();
}

void H264Encoder::StopEncoding()
{
	Stop();
}

void H264Encoder::InitParameters(VideoParams &params) {
	x264_param_default(m_pX264param);

	ENC_DBG("i_threads: %d, b_sliced_threads: %d, b_deterministic: %d, i_sync_lookahead: %d, i_lookahead: %d, i_bframe_adaptive: %d, i_frame_reference: %d",
			m_pX264param->i_threads, m_pX264param->b_sliced_threads, m_pX264param->b_deterministic, m_pX264param->i_sync_lookahead,
			m_pX264param->rc.i_lookahead, m_pX264param->i_bframe_adaptive, m_pX264param->i_frame_reference);

	m_pX264param->i_width = params.width;
	m_pX264param->i_height = params.height;
	m_pX264param->rc.i_rc_method  = X264_RC_ABR;
	m_pX264param->rc.i_bitrate = params.bitrate;
	m_pX264param->rc.i_vbv_max_bitrate = params.bitrate;
	m_pX264param->cpu = 0;
	m_pX264param->cpu |=X264_CPU_MMX;
	m_pX264param->cpu |=X264_CPU_MMXEXT;
	m_pX264param->cpu |=X264_CPU_SSE;
	m_pX264param->cpu |=X264_CPU_SSE2;
	m_pX264param->cpu |=X264_CPU_SSSE3;

	// minimize delay
	m_pX264param->rc.i_lookahead = 0;
	m_pX264param->i_threads = 1;

	// desktop supports only baseline profile:
	m_pX264param->analyse.b_transform_8x8 = 0;
	m_pX264param->b_cabac = 0;
	m_pX264param->i_cqm_preset = X264_CQM_FLAT;
	m_pX264param->i_bframe = 0;
	m_pX264param->analyse.i_weighted_pred = X264_WEIGHTP_NONE;
	m_pX264param->b_interlaced = 0;

	m_pX264param->i_fps_num = params.framerate;
	m_pX264param->i_fps_den = 1;

	m_pX264param->pf_log = NULL; //x264log;
	m_pX264param->p_log_private = NULL;
	m_pX264param->i_log_level = 0; //X264_LOG_DEBUG;
}

void H264Encoder::RestartEncoder(VideoParams &params) {
	if (Running) {
		ENC_DBG("ChangeBitRate, Running");
		Stop();

		x264_encoder_close(m_pX264Encoder);
		InitParameters(params);
		m_pX264Encoder = x264_encoder_open(m_pX264param);

		Start();
	} else {
		InitParameters(params);
		m_pX264Encoder = x264_encoder_open(m_pX264param);
	}
}

void H264Encoder::ReconfigureEncoder(VideoParams &params) {
	if (Running) {
		ENC_DBG("ChangeBitRate, Running");
		Stop();

		InitParameters(params);
		int ret = x264_encoder_reconfig(m_pX264Encoder, m_pX264param);
		ENC_DBG("x264_encoder_reconfig: ret %d\n", ret);

		Start();
	} else {
		InitParameters(params);
		m_pX264Encoder = x264_encoder_open(m_pX264param);
	}
}

void H264Encoder::Run(void)
{
	x264_picture_t pic_out;
	x264_nal_t *nal;
	int nalCnt;
	
	while (!Shutdown) {
		uint64_t cycleStartTime = getTimeUs();
		VideoData capturedFrame;
		capturedFrame.SetBuf(m_pInputPic->img.plane[0], 0);
		m_pVCap->CaptureFrame(capturedFrame);
		
		if (m_ForceKeyFrame) {
			m_pInputPic->i_type = X264_TYPE_IDR;			
		} else {
			m_pInputPic->i_type = X264_TYPE_AUTO;
		}
		int total_len = x264_encoder_encode(m_pX264Encoder, &nal, &nalCnt, m_pInputPic, &pic_out);
		if (total_len < 0) {
			ERROR("x264_encoder_encode failed");
		}
//		ENC_DBG("x264_encoder_encode: %lld\n", getTimeUs() - cycleStartTime);
//		ENC_DBG("got %d nals, total len: %d", nalCnt, total_len);

		if (pic_out.i_type == X264_TYPE_IDR || pic_out.i_type == X264_TYPE_I) {
			ENC_DBG("Got I-frame");
			m_ForceKeyFrame = false;
		}		
		

		if (total_len > 0) {
			VideoData frame;
			frame.SetBuf(nal->p_payload, total_len);
			frame.SetTimestamp(capturedFrame.GetTimestamp());
			m_pCb->TransmitFrame(frame);
			m_BStat.Update(total_len);
			if (m_BStat.IsValid())
				ENC_DBG("bitrate: %s", m_BStat.GetStatStr());
		}
		int32_t sleepUs = (int32_t)(1 / (float)m_VPar.framerate * 1000000) - (getTimeUs() - cycleStartTime);
		int32_t fpsAvg;
		if (!m_FpsAvg.GetAvg(fpsAvg))
			fpsAvg = m_VPar.framerate;
		ENC_DBG("sleep: %d, fpsavg: %d", sleepUs, fpsAvg);
		if (sleepUs > 0 && fpsAvg >= m_VPar.framerate) {
			usleep(sleepUs);
		}
		m_FpsAvg.Update(uint32_t((float)1/((getTimeUs() - cycleStartTime)*1000000)));
	}
	ENC_DBG("Exit H264Encoder::Run");
}

