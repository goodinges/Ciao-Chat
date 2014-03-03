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
 *  Created on: Mar 27, 2009
 *      Author: lauri
 */

#ifndef BITRATESTAT_HPP_
#define BITRATESTAT_HPP_
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#define MAX_STAT_STR 255
#define FRAME_WINDOW_SIZE 10

class BitrateStat {
public:
	BitrateStat(int calc_interval_us = -1) : m_CalcInterval(calc_interval_us) {
		Reset();
	}
	~BitrateStat() {}
	void Update(int packetSize) {
		m_ByteCount += packetSize;

		if (m_IntervalStart) {
			m_IntervalStart = false;
			m_StartTime = getTimeUs();
		}
	}
	bool IsValid() {
		if (m_CalcInterval > 0 && getTimeUs() - m_StartTime < (uint64_t)m_CalcInterval)
			return false;

		return true;
	}
	char *GetStatStr() {
		uint32_t bitrateAvg;
		uint32_t frmWinSize = 0;
		frmWinSize = 0;

		bitrateAvg = kbitSec(m_ByteCount, getTimeUs() - m_StartTime);

		sprintf(m_pStatStr, "Bitrate: average: %d kbits/sec", bitrateAvg);

		if (m_CalcInterval > 0 && getTimeUs() - m_StartTime > (uint64_t)m_CalcInterval)
			Reset();

		return m_pStatStr;
	}
	void Reset() {
		m_IntervalStart = true;
		m_ByteCount = 0;
	}
private:
	uint64_t getTimeUs() {
		struct timeval tv;
		uint64_t ret;
		gettimeofday(&tv, NULL);
		ret = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
		return ret;
	}

	uint32_t kbitSec(uint32_t size, uint64_t time) {
		return (uint32_t)(size * 8 / (time / (float)1000000) / 1024);
	}

	char m_pStatStr[MAX_STAT_STR];
	int m_CalcInterval;
	bool m_IntervalStart;
	uint64_t m_StartTime;
	uint64_t m_ByteCount;
};

#endif /* BITRATESTAT_HPP_ */
