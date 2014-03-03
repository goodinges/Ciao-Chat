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

#include "H264AnxbFileReader.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "LoggingCtrl.hpp"
#include "PacketDump.hpp"

H264AnxbFileReader::H264AnxbFileReader(VideoParams *vParams, VideoCapturer *vCap, FrameCallback *cb) :
	VideoEncoder(vParams, vCap, cb),
	pThread(false),
	m_ForceKeyFrame(false)
{
	snprintf(m_FileName, sizeof(m_FileName), "%s/testvideo-%dx%d.264", m_VPar.devName, m_VPar.width, m_VPar.height);

	m_AnxbReader.Init(m_FileName);
}

H264AnxbFileReader::~H264AnxbFileReader() {
	m_AnxbReader.Uninit();
}

void H264AnxbFileReader::AddFillerUnit(uint8_t *buf, uint32_t len) {
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 1;
	((NalHeader *)(&buf[4]))->Reset();
	((NalHeader *)(&buf[4]))->SetNri(2);
	((NalHeader *)(&buf[4]))->SetType(NALU_FILLER);

	if (len > 5) {
		memset(buf + 5, 0xff, len - 1);
		buf[len - 1] = 0x80;
	}
}

static uint64_t getTimeUs() {
	struct timeval tv;
	uint64_t ret = 0;
	gettimeofday(&tv, NULL);
	ret = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
	return ret;
}

void H264AnxbFileReader::Run() {
	int32_t sleepUs = 0;
	uint8_t *anxbFrame = NULL;
	uint32_t anxbFrameLen = 0;

	while (!Shutdown) {
		int32_t desiredLen, fillerLen;
		int64_t cycleStartTime;
		int bitrateBytesSec = m_VPar.bitrate * 1024 / 8;

		if (m_ForceKeyFrame) {
			m_AnxbReader.Restart();
			m_ForceKeyFrame = false;
		}

		cycleStartTime = getTimeUs();

		anxbFrame = m_AnxbReader.GetFrame(anxbFrameLen);

		// add anxb unit to the frame buf
		memcpy(m_pFrameBuf, anxbFrame, anxbFrameLen);

		//add filler unit if needed to keep the desired bitrate
		desiredLen = bitrateBytesSec / m_VPar.framerate;
		fillerLen = ((int32_t)(desiredLen - anxbFrameLen) > 0? desiredLen - anxbFrameLen : 0);
		fillerLen += (int32_t)(100 * (rand() / (float)RAND_MAX));
		if (fillerLen) {
			AddFillerUnit(&m_pFrameBuf[anxbFrameLen], fillerLen);
		}

		VideoData frame;
		frame.SetBuf(m_pFrameBuf, anxbFrameLen + fillerLen);
		frame.SetTimestamp((getTimeUs() - m_StartTime) / 1000);
		m_pCb->TransmitFrame(frame);

		m_BStat.Update(anxbFrameLen + fillerLen);
		m_TStat.Update();
		ENC_DBG("bitrate: %s, framerate: %s", m_BStat.GetStatStr(), m_TStat.GetStatStr());
		sleepUs = (int32_t)(1 / (float)m_VPar.framerate * 1000000) - (getTimeUs() - cycleStartTime);
		if (sleepUs > 0)
			usleep(sleepUs);
	}
}

void H264AnxbFileReader::ChangeBitrate(uint32_t bitrate) {
	ENC_DBG("H264Reader::Changebitrate %d", bitrate);
	m_VPar.bitrate = bitrate;
	m_BStat.Reset();
}

void H264AnxbFileReader::ChangeFramerate(uint32_t framerate) {
	ENC_DBG("H264Reader::Changeframerate %d", framerate);
	m_VPar.framerate = framerate;
	m_BStat.Reset();
}

void H264AnxbFileReader::GenerateKeyFrame() {
	m_ForceKeyFrame = true;
	ENC_DBG("H264Reader::GenerateKeyFrame");
}

void H264AnxbFileReader::StartEncoding() {
	m_StartTime = getTimeUs();
	Start();
}

void H264AnxbFileReader::StopEncoding() {
	Stop();
}
