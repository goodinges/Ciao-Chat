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

#ifndef TIMINGSTAT_HPP_
#define TIMINGSTAT_HPP_

#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#define MAX_STAT_STR 255 

class TimingStat {
public:
	TimingStat(int calc_interval_us = -1) : 
		m_CalcInterval(calc_interval_us) {
		Reset();
	} 
	~TimingStat() {}
	void Update() {
		uint64_t currTime;
		
		currTime = getTimeUs();

		if (m_IntervalStart) {
			m_IntervalStart = false;
			m_StartTime = currTime;
		}

		if (m_FrmNum) {
			m_LastFrmTime = (int)(currTime - m_LastFrmTimestamp);
			m_FrmTimeSum += m_LastFrmTime;
		}
		m_LastFrmTimestamp = currTime;
		m_FrmNum++;
	}
	bool IsValid() {
		if (m_CalcInterval > 0 && getTimeUs() - m_StartTime < m_CalcInterval) 
			return false;

		return true;
	}
	char *GetStatStr() {
		float fps, fpsAvg = 0;
		char *retPtr;

		if (m_FrmNum > 1) {			
			fps = 1000000 / (float)m_LastFrmTime;
			fpsAvg = 1000000 / (float)(m_FrmTimeSum / m_FrmNum);
			sprintf(m_pStatStr, "Frame: %d time: %lld us, fps %f, fpsAvg: %f", m_FrmNum, m_LastFrmTime, fps, fpsAvg);
			retPtr = m_pStatStr;
		} else {
			retPtr = NULL;
		}

		if (m_CalcInterval > 0 && getTimeUs() - m_StartTime > m_CalcInterval) 
			Reset();

		return retPtr;
	}
	void Reset() {
		m_FrmNum = 0;
		m_LastFrmTime = 0;
		m_LastFrmTimestamp = 0;
		m_FrmTimeSum = 0;
		m_IntervalStart = true;
	}
	uint64_t getTimeUs() {
		struct timeval tv;
		uint64_t ret;
		gettimeofday(&tv, NULL);
		ret = (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
		return ret; 
	}	

	char m_pStatStr[MAX_STAT_STR];
	uint32_t m_FrmNum;
	uint64_t m_LastFrmTimestamp;
	uint64_t m_LastFrmTime;
	uint64_t m_FrmTimeSum;
	uint32_t m_CalcInterval;
	uint64_t m_StartTime;
	bool m_IntervalStart;
};

#endif /* TIMINGSTAT_HPP_ */
