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

#include "SocketTransport.hpp"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

//#define SID_LOG_MASK 0xFF
#include "SidDebugging.hpp"

namespace Sid {

AVTransport * GetAVTransport() {
	return new SocketTransport();
}

SocketTransport::SocketTransport() :
	m_WrBufCnt(0),
	m_RdBufCnt(0),
	m_RdBufRdAddr(0) {

	m_Socket = GetSocket();
	assert(m_Socket);
}

SocketTransport::~SocketTransport() {
	delete m_Socket;
}

bool SocketTransport::Connect(const char *key, bool serverMode, int timeout_ms) {
	strncpy(m_Key, key, sizeof(m_Key));
	return m_Socket->Connect(m_Key, serverMode, timeout_ms);
}

void SocketTransport::Disconnect() {
	m_Socket->Disconnect();
}

/* Implements the logic needed to write the whole buffer. If socket is full then
 * polls until can write more*/
bool SocketTransport::WriteBuffer(const char *buf, int count) {
	SID_VDBG("WriteBuffer: %d", count);
	int remaining = count;
	int rv;
#define MAX_WRITE_LEN 65535

	do {
		int wr_len = remaining > MAX_WRITE_LEN? MAX_WRITE_LEN: remaining;
		rv = m_Socket->Write(buf, wr_len);
		if (rv > 0) {
			SID_VDBG("Wrote successfully %d", rv);
			remaining -= rv;
			buf += rv;
		// if write failed because socket internal buffers are full then poll until
		// we can write more
		} else if (errno == EAGAIN) {
			SID_VDBG("Write errno == EAGAIN, polling");
			rv = m_Socket->PollWrite();
			SID_VDBG("write poll ret: %d", rv);
			if (rv < 0) {
				SID_ERROR("write poll failed ret error: %d", errno);
				return false;
			}
		} else {
			SID_ERROR("write failed errno: %d", errno);
			return false;
		}
	} while (remaining > 0);

	return true;
}

inline int SocketTransport::ReadBuffer(char * buf, int min_count, int max_count) {
	SID_VDBG("ReadToBuf %d %d", min_count, max_count);
	int rv;
	int read_count = 0;
	while(read_count < min_count) {
		bool poll_ret = m_Socket->PollRead();
		SID_VDBG("Read poll ret: %d", poll_ret);
		if (!poll_ret) {
			SID_ERROR("Poll error: %d", errno);
			return -1;
		}
		rv = m_Socket->Read(&buf[read_count], max_count - read_count);
		if (rv > 0) {
			SID_VDBG("Read successfully: %d", rv);
			read_count += rv;
		} else if (errno == EAGAIN && read_count < min_count) {
			SID_VDBG("Read errno == EAGAIN, polling");
		} else {
			SID_ERROR("Read error: %d", errno);
			return -1;
		}
	}
	return read_count;
}

/* Reads at least count bytes to read buf, but may read up to IPCCOMM_RD_BUF_LEN if more is available */
inline bool SocketTransport::ReadToReadBuf(int count) {
	int ret;
	SID_VDBG("ReadToReadBuf %d", count);
	assert(m_RdBufCnt == 0 && count < IPCCOMM_RD_BUF_LEN);
	ret = ReadBuffer(m_RdBuf, count, IPCCOMM_RD_BUF_LEN);
	if (ret >= count) {
		m_RdBufCnt += ret;
		return true;
	} else {
		return false;
	}
}

/* read number of bytes from read buf */
inline int SocketTransport::ReadFromRdBuf(char * buf, int count) {
	int ret;
	SID_VDBG("ReadFromRdBuf: count %d, rd_buf_rd_addr: %d, rd_buf_count: %d", count, m_RdBufRdAddr, m_RdBufCnt);
	if (count < m_RdBufCnt) {
		memcpy(buf, &m_RdBuf[m_RdBufRdAddr], count);
		ret = count;
		m_RdBufRdAddr += count;
		m_RdBufCnt -= count;
	} else {
		memcpy(buf, &m_RdBuf[m_RdBufRdAddr], m_RdBufCnt);
		ret = m_RdBufCnt;
		m_RdBufRdAddr = 0;
		m_RdBufCnt = 0;
	}
	return ret;
}

bool SocketTransport::WriteStart() {
	SID_VDBG("SocketTransport::WriteStart");
	m_WrBufCnt = 0;
	return true;
}

bool SocketTransport::Write(const char *buf, int count) {
	SID_VDBG("SocketTransport::Write %d", count);

	if (m_WrBufCnt + count < IPCCOMM_WR_BUF_LEN) {
		memcpy(&m_WrBuf[m_WrBufCnt], buf, count);
		m_WrBufCnt+=count;
	// larger tahn 64K write - don't do any buffering
	} else {
		if (m_WrBufCnt) {
			int ret = WriteBuffer(m_WrBuf, m_WrBufCnt);
			m_WrBufCnt = 0;
			if (ret < 0)
				return false;
		}
		int ret = WriteBuffer(buf, count);
		if (ret < 0)
			return false;
	}
	return true;
}

bool SocketTransport::WriteComplete() {
	SID_VDBG("SocketTransport::WriteComplete %d", m_WrBufCnt);
	int ret = WriteBuffer(m_WrBuf, m_WrBufCnt);
	if (ret < 0)
		return false;
	else
		return true;
}

bool SocketTransport::Read(char * buf, int count) {
	SID_DBG("SocketTransport::Read: %d", count);
	if (count == 0)
		return true;

	int remaining_count = count;
	int read_count = 0;
	// check if we have data in read buffer
	if (m_RdBufCnt) {
		read_count = ReadFromRdBuf(buf, count);
		remaining_count-= read_count;
		SID_DBG("SocketTransport::m_RdBufCnt: %d, read_count: %d, remaining_count: %d, ", m_RdBufCnt, read_count, remaining_count);
		if (!remaining_count)
			return true;
	}

	if (remaining_count <= IPCCOMM_RD_BUF_LEN) {
		if (!ReadToReadBuf(remaining_count))
			return false;
		ReadFromRdBuf(&buf[read_count], remaining_count);
	
	// larger than 64 k read - use read directly to given buffer
	} else {
		int ret = ReadBuffer(&buf[read_count], remaining_count, remaining_count);
		if (ret < 0) {
			return false;
		}
		return true;
	}

	return true;
}

}

