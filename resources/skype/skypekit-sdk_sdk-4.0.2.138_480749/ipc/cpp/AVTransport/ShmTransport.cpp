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

#include "ShmTransport.hpp"

//#define SID_LOG_MASK 0x1f
//#define SID_DBG_TIMESTAMPS 1
#include "SidDebugging.hpp"

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


#define SHM_CBUF_SIZE 60000
#define MAX_MSG_SIZE 2000

#define SID_CBDBG(fmt, args...) _SID_LOG(SID_DBG_TIMESTAMPS, 0x20, fmt, ##args)

namespace Sid {

void ShmCircBuffer::Init(ShmCbufMapping *shm, int size) {
	m_Map = shm;
	m_Map->wr_addr = 0;
	m_Map->rd_addr = 0;
	m_Bufsize = size - sizeof(ShmCbufMapping);
}

void ShmCircBuffer::CopyTo(const char *src, int len) {
	SID_CBDBG("buf: 0x%x: copy_to: w: %d, r: %d, c: %d, len: %d", (unsigned int)m_Map->data, m_Map->wr_addr, m_Map->rd_addr, Count(), len);
	assert(len + Count() < m_Bufsize);

	if (m_Bufsize - m_Map->wr_addr >= len) {
		memcpy(&m_Map->data[m_Map->wr_addr], src, len);
	} else {
		int wr_len1 = m_Bufsize - m_Map->wr_addr;
		memcpy(&m_Map->data[m_Map->wr_addr], src, wr_len1);
		int wr_len2 = len - wr_len1;
		memcpy(&m_Map->data[0], src + wr_len1, wr_len2);
	}
	IncAddr(&m_Map->wr_addr, len);
}

void ShmCircBuffer::CopyFrom(char *dst, int len) {
	SID_CBDBG("buf: 0x%x: copy_from: w: %d, r: %d, c: %d, len: %d", (unsigned int)m_Map->data, m_Map->wr_addr, m_Map->rd_addr, Count(), len);
	assert(Count() - len >= 0);
	assert(Count() < m_Bufsize);

	if (m_Bufsize - m_Map->rd_addr >= len) {
		memcpy(dst, &m_Map->data[m_Map->rd_addr], len);
	} else {
		int rd_len1 = m_Bufsize - m_Map->rd_addr;
		memcpy(dst, &m_Map->data[m_Map->rd_addr], rd_len1);
		int rd_len2 = len - rd_len1;
		memcpy(dst + rd_len1, &m_Map->data[0], rd_len2);
	}
	IncAddr(&m_Map->rd_addr, len);
}
int ShmCircBuffer::Count() {
	int wr_addr = m_Map->wr_addr;
	int rd_addr = m_Map->rd_addr;
	if (wr_addr >= rd_addr) {
		return wr_addr - rd_addr;
	} else {
		return m_Bufsize - rd_addr + wr_addr;
	}
}
void ShmCircBuffer::IncAddr(volatile int *addr, int len) {
	if (*addr + len >= m_Bufsize) {
		*addr = *addr + len - m_Bufsize;
	} else {
		*addr = *addr + len;
	}
}

AVTransport * GetAVTransport() {
	return new ShmTransport();
}


class FileLock {
public:
	FileLock() :
		m_Fd(-1),
		m_Locked(false)
		{}
	~FileLock() {
		Unlock();
	}
	bool Lock(const char * fname) {
		m_Fname = fname;
		SID_DBG("Lock: %s", m_Fname);

		m_Fd = open(fname, O_WRONLY | O_CREAT, 0600);
		if (m_Fd < 0) {
			SID_ERROR("Failed to open file: %d (%s)", errno, strerror(errno));
			return false;
		}

		struct flock flck;
		memset(&flck,0,sizeof(struct flock));
		flck.l_type=F_WRLCK;
		if (fcntl(m_Fd,F_SETLK,&flck) < 0) {
			SID_ERROR("Failed to lock file: %d (%s)", errno, strerror(errno));
			Unlock();
			return false;
		}
		m_Locked = true;
		return true;
	}
	void Unlock() {
		SID_DBG("Unlock: %s", m_Fname);
		if (m_Fd != -1) {
			if (m_Locked) {
				struct flock flck;
				memset(&flck,0,sizeof(struct flock));
				flck.l_type=F_UNLCK;
				fcntl(m_Fd,F_SETLK,&flck);
				m_Locked = false;
			}
			close(m_Fd);
			m_Fd = -1;
		}
	}
	static bool TestLocked(const char * fname, bool &locked) {
		SID_DBG("TestLocked: %s", fname);

		int fd = open(fname, O_WRONLY, 0600);
		if (fd < 0) {
			SID_ERROR("Failed to open file:  %s %d (%s)", fname, errno, strerror(errno));
			return false;
		}
		struct flock flck;
		memset(&flck,0,sizeof(struct flock));
		flck.l_type=F_WRLCK;
		fcntl(fd,F_GETLK,&flck);
		locked = (flck.l_type & F_UNLCK? false: true);
		close(fd);
		SID_DBG("TestLocked: %s ret: %d %d", fname, flck.l_type, locked);
		return true;
	}
	int m_Fd;
	bool m_Locked;
	const char * m_Fname;
};

ShmTransport::ShmTransport() :
	m_ServerMode(false) {
	m_Shm = GetShm();
	m_WrEmptySem = GetSem();
	m_RdEmptySem = GetSem();
	m_WrFullSem = GetSem();
	m_RdFullSem = GetSem();
	m_ConnSem = GetSem();
	m_ServerFlock = new FileLock();
}

ShmTransport::~ShmTransport(void) {
	Disconnect();
	delete m_Shm;
	delete m_WrEmptySem;
	delete m_RdEmptySem;
	delete m_WrFullSem;
	delete m_RdFullSem;
	delete m_ConnSem;
	delete m_ServerFlock;
}

bool ShmTransport::ServerConnect(int timeout_ms) {
	bool ret = m_Shm->Init(m_Key, SHM_CBUF_SIZE * 2, m_ServerMode);
	if (!ret)
		goto serv_conn_error;

	m_WrCbuf.Init((ShmCbufMapping *) m_Shm->GetMemPtr(), SHM_CBUF_SIZE);
	m_RdCbuf.Init((ShmCbufMapping *) (m_Shm->GetMemPtr() + SHM_CBUF_SIZE),
			SHM_CBUF_SIZE);

	if (!m_WrEmptySem->Init(m_Key, "wresem", true, 0))
		goto serv_conn_error;
	if (!m_RdEmptySem->Init(m_Key, "rdesem", true, 0))
		goto serv_conn_error;
	if (!m_WrFullSem->Init(m_Key, "wrfsem", true, SHM_CBUF_SIZE / MAX_MSG_SIZE))
		goto serv_conn_error;
	if (!m_RdFullSem->Init(m_Key, "rdfsem", true, SHM_CBUF_SIZE / MAX_MSG_SIZE))
		goto serv_conn_error;
	if (!m_ConnSem->Init(m_Key, "connsem", true, 0))
		goto serv_conn_error;

	// lock the file when everything is initialized - this tells client that it may connect
	if (!m_ServerFlock->Lock(m_Key))
		goto serv_conn_error;

	SID_DBG( "ShmComm::Connect(server) %s - waiting client...", m_Key);
	if (!m_ConnSem->Wait(timeout_ms))
		goto serv_conn_error;
	SID_DBG( "ShmComm::Connect(server) %s - client connected", m_Key);

	m_Connected = true;
	return true;

serv_conn_error:
	SID_ERROR("ShmComm::ServerConnect %s failed", m_Key);
	Disconnect();
	return false;
}

bool ShmTransport::ClientConnect() {
	bool ret;
	bool server_connected;

	// test if server has locked the key file. If not, can't connect yet
	ret = FileLock::TestLocked(m_Key, server_connected);
	if (!ret)
		goto client_conn_error;

	if (!server_connected) {
		SID_ERROR("Server not connected yet!");
		goto client_conn_error;
	}

	ret = m_Shm->Init(m_Key, SHM_CBUF_SIZE * 2, m_ServerMode);
	if (!ret)
		goto client_conn_error;

	m_RdCbuf.Init((ShmCbufMapping *) m_Shm->GetMemPtr(), SHM_CBUF_SIZE);
		m_WrCbuf.Init((ShmCbufMapping *) (m_Shm->GetMemPtr() + SHM_CBUF_SIZE),
				SHM_CBUF_SIZE);

	if (!m_RdEmptySem->Init(m_Key, "wresem", false, 0))
		goto client_conn_error;
	if (!m_WrEmptySem->Init(m_Key, "rdesem", false, 0))
		goto client_conn_error;
	if (!m_RdFullSem->Init(m_Key, "wrfsem", false, 0))
		goto client_conn_error;
	if (!m_WrFullSem->Init(m_Key, "rdfsem", false, 0))
		goto client_conn_error;
	if (!m_ConnSem->Init(m_Key, "connsem", false, 0))
		goto client_conn_error;

	SID_DBG( "ShmComm::Connect(client) %s - notify server", m_Key);
	if (!m_ConnSem->Signal())
		goto client_conn_error;

	m_Connected = true;
	return true;

client_conn_error:
	SID_ERROR("ShmComm::_Connect %s failed", m_Key);
	Disconnect();
	return false;
}

bool ShmTransport::Connect(const char *key, bool serverMode, int timeout_ms) {
	m_ServerMode = serverMode;
	strncpy(m_Key, key, sizeof(m_Key));
	if (serverMode) {
		return ServerConnect(timeout_ms);
	} else {
		bool ret;
		int elapsed_ms = 0;
		do {
			ret = ClientConnect();
			if (!ret) {
				usleep(100000);
				elapsed_ms += 100;
			}
		} while (!ret && (timeout_ms < 0 || elapsed_ms < timeout_ms));
		return ret;
	}
}

void ShmTransport::Disconnect() {
	m_Shm->Uninit();
	m_WrEmptySem->Uninit();
	m_RdEmptySem->Uninit();
	m_WrFullSem->Uninit();
	m_RdFullSem->Uninit();
	m_ConnSem->Uninit();
	if (m_ServerMode)
		m_ServerFlock->Unlock();
	m_Connected = false;
}

bool ShmTransport::WriteStart() {
	SID_VDBG("ShmComm::WriteStart currcnt: %d", m_WrCbuf.Count());
	m_WrFullSem->Wait();
	m_WrCnt = 0;
	return true;
}

bool ShmTransport::Write(const char *buf, int count) {
	SID_VDBG("ShmComm::Write %d, cnt: %d, msg_bytes: %d", count, m_WrCbuf.Count(), m_WrCnt);
	m_WrCnt += count;
	assert(m_WrCnt < MAX_MSG_SIZE);
	m_WrCbuf.CopyTo(buf, count);
	return true;
}

bool ShmTransport::WriteComplete() {
	SID_VDBG("ShmComm::WriteComplete, msg_bytes: %d, cnt: %d", m_WrCnt, m_WrCbuf.Count());
	if (!m_WrEmptySem->Signal())
		return false;
	return true;
}

bool ShmTransport::ReadStart() {
	SID_VDBG("ShmComm::ReadStart cnt: %d", m_RdCbuf.Count());
	m_RdCnt = 0;
	if (!m_RdEmptySem->Wait())
		return false;
	return true;
}

bool ShmTransport::Read(char *buf, int count) {
	SID_VDBG("ShmComm::Read %d, cnt: %d, msg_bytes: %d", count, m_RdCbuf.Count(), m_RdCnt);
	m_RdCnt += count;
	assert(m_RdCnt < MAX_MSG_SIZE);
	m_RdCbuf.CopyFrom(buf, count);
	return true;
}

bool ShmTransport::ReadComplete() {
	SID_VDBG("ShmComm::ReadComplete complete: msg_bytes: %d, cnt: %d", m_RdCnt, m_RdCbuf.Count());
	if (!m_RdFullSem->Signal())
		return false;
	return true;
}

}
