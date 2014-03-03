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

#include "PcapFileReader.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "LoggingCtrl.hpp"
#include "PacketDump.hpp"

PcapFileReader::PcapFileReader(VideoParams *vParams, VideoCapturer *vCap, FrameCallback *cb) :
	VideoEncoder(vParams, vCap, cb),
	pThread(false),
	m_PcapFile(m_VPar.devName),
	m_pPacketBuf(NULL)
{
	m_PacketBufLen = m_PcapFile.GetSnapLen();
	m_pPacketBuf = new uint8_t[m_PacketBufLen];
}

PcapFileReader::~PcapFileReader() {
	if (m_pPacketBuf)
		delete m_pPacketBuf;
}

void PcapFileReader::Run() {
	int packetLen;
	uint64_t timestamp, lastTimestamp = 0, sleepUs;

	while (!Shutdown) {
		if (m_PcapFile.ReadPacket((void *)m_pPacketBuf, m_PacketBufLen, packetLen, timestamp) != 0) {
			DBG("ReadPacket failed");
			break;
		}

		DBG("PCAP: len: %d, time: %lld, diff: %lld", packetLen, timestamp, timestamp - lastTimestamp);

		VideoData frame;
		frame.SetBuf(m_pPacketBuf, packetLen);
		frame.SetTimestamp(timestamp);
		m_pCb->TransmitFrame(frame);

		if (!lastTimestamp) {
			lastTimestamp = timestamp;
		} else {
			sleepUs = timestamp - lastTimestamp;
			usleep(sleepUs);
			lastTimestamp = timestamp;
		}
	}
}

void PcapFileReader::ChangeBitrate(uint32_t bitrate) {
}

void PcapFileReader::GenerateKeyFrame() {
}

void PcapFileReader::StartEncoding() {
	Start();
}

void PcapFileReader::StopEncoding() {
	Stop();
}
