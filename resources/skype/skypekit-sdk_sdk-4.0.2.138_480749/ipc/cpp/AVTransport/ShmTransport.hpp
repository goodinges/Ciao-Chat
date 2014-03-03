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
#ifndef SHMCOMM_HPP
#define SHMCOMM_HPP

#include <stdlib.h>
#include "AVTransport.hpp"

namespace Sid {

struct Shm;
struct ShmCircBuffer;
struct Semaphode;

struct ShmCbufMapping {
	volatile int wr_addr;
	volatile int rd_addr;
	char data[0];
};

class ShmCircBuffer {
	void IncAddr(volatile int *addr, int len);
	int m_Bufsize;
public:
	ShmCbufMapping *m_Map;
	void Init(ShmCbufMapping *shm, int size);
	void CopyTo(const char *src, int len);
	void CopyFrom(char *dst, int len);
	int Count();
	int Size() {
		return m_Bufsize;
	}
};

class Semaphore {
public:
	Semaphore() {}
	virtual ~Semaphore() {}

	virtual bool Init(const char *fileName, const char *fileSuffix,
			bool create, int initval) = 0;
	virtual void Uninit() = 0;

	virtual bool Wait() = 0;
	virtual bool Wait(int timeout_ms) = 0;
	virtual bool Signal() = 0;
};

class Shm {
public:
	Shm() {}
	virtual ~Shm() {}
	virtual bool Init(const char *fileName, int size, int serverMode) = 0;
	virtual unsigned char *GetMemPtr() = 0;
	virtual void Uninit() = 0;
};

extern Semaphore *GetSem();
extern Shm *GetShm();
class FileLock;

class ShmTransport : public AVTransport {
	bool ServerConnect(int timeout_ms);
	bool ClientConnect();
public:
	ShmTransport();
	~ShmTransport();

	bool Connect(const char *key, bool serverMode, int timeout_ms);
	void Disconnect();
	inline bool IsConnected(void) {
		return m_Connected;
	}

	bool WriteStart();
	bool Write(const char *buf, int count);
	bool WriteComplete();
	bool ReadStart();
	bool Read(char *buf, int count);
	bool ReadComplete();

private:
	ShmCircBuffer m_WrCbuf, m_RdCbuf;
	Semaphore *m_RdEmptySem, *m_WrEmptySem, *m_WrFullSem, *m_RdFullSem,
			*m_ConnSem;
	Shm *m_Shm;
	int m_WrCnt, m_RdCnt;
	char m_Key[255];
	bool m_ServerMode;
	bool m_Connected;
	FileLock *m_ServerFlock;
};

}
#endif
