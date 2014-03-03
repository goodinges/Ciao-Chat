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

#ifndef STATICSYNCQUEUE_HPP_
#define STATICSYNCQUEUE_HPP_

#include <stdint.h>
#include <unistd.h>

#include "ptasking.hpp"

template <typename ElemType>
class StaticSyncQueue {
public:
	StaticSyncQueue(ElemType *elemArr, int len) :
		m_pElemArray(elemArr),
		m_QueueLen(len),
		m_WrIndex(0),
		m_RdIndex(0),
		m_ItemCnt(0),
		m_NumWaiters(0)
	{}

	~StaticSyncQueue() {
		WakeWait();
		m_QueueMutex.Acquire();
		while(m_NumWaiters) {
			WakeWait();
			m_QueueMutex.Release();
			usleep(1000);
			m_QueueMutex.Acquire();
		}
		m_QueueMutex.Release();
	}

	ElemType GetWritePtr() {
		pMutexLock l(m_QueueMutex);
		if (m_ItemCnt == m_QueueLen) {
			m_NumWaiters++;
			m_QueueEvent.Wait(m_QueueMutex);
			m_NumWaiters--;
			// WakeWait
			if (m_ItemCnt == m_QueueLen) {
				return NULL;
			}
		}
		return m_pElemArray[m_WrIndex];
	}

	void WriteDone() {
		pMutexLock l(m_QueueMutex);
		IncIndex(m_WrIndex);
		if (!m_ItemCnt)
			m_QueueEvent.Trigger(m_QueueMutex);
		m_ItemCnt++;
	}

	ElemType GetReadPtr() {
		pMutexLock l(m_QueueMutex);
		if (m_ItemCnt == 0) {
			m_NumWaiters++;
			m_QueueEvent.Wait(m_QueueMutex);
			m_NumWaiters--;
			// WakeWait
			if (m_ItemCnt == 0) {
				return NULL;
			}
		}
		return m_pElemArray[m_RdIndex];
	}

	void ReadDone() {
		pMutexLock l(m_QueueMutex);
		IncIndex(m_RdIndex);
		if (m_ItemCnt == m_QueueLen) {
			m_QueueEvent.Trigger(m_QueueMutex);
		}
		m_ItemCnt--;
	}

	void WakeWait() {
		m_QueueEvent.Trigger(m_QueueMutex);
	}

	uint32_t GetCnt() {
		pMutexLock l(m_QueueMutex);
		return m_ItemCnt;
	}
private:
	void IncIndex(int &index) {
		if (index < m_QueueLen - 1) {
			index++;
		} else {
			index = 0;
		}
	}
	const char *m_pName;
	ElemType *m_pElemArray;
	int m_QueueLen;
	int m_ItemSize;
	int m_WrIndex;
	int m_RdIndex;
	int m_ItemCnt;
	int m_NumWaiters;
	pMutex m_QueueMutex;
	pSyncEvent m_QueueEvent;
};
#endif /* STATICQUEUE_HPP_ */
