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

#include "AVTransport.hpp"

namespace Sid {

#define IPCCOMM_RD_BUF_LEN 65535
#define IPCCOMM_WR_BUF_LEN 65535


class Socket {
public:
	virtual bool Connect(const char *key, bool serverMode, int timeout_ms) = 0;
	virtual void Disconnect() = 0;
	virtual bool IsConnected() const = 0;

	virtual int Write(const char *buf, int count) = 0;
	virtual int Read(char *buf, int count) = 0;
	virtual bool PollRead() = 0;
	virtual bool PollWrite() = 0;
	virtual ~Socket() {}
};

extern Socket * GetSocket();

class SocketTransport : public AVTransport {
	char m_WrBuf[IPCCOMM_WR_BUF_LEN];
	int m_WrBufCnt;
	char m_RdBuf[IPCCOMM_RD_BUF_LEN];
	int m_RdBufCnt;
	int m_RdBufRdAddr;
	char m_Key[255];
	Socket *m_Socket;

	bool WriteBuffer(const char * buf, int count);

	inline int ReadBuffer(char * buf, int count, int max_count);
	inline bool ReadToReadBuf(int count);
	inline int ReadFromRdBuf(char * buf, int count);
	inline bool ReadToUserBuf(char * buf, int count);
public:
	SocketTransport();
	virtual ~SocketTransport();

	bool Connect(const char *key, bool serverMode, int timeout_ms);
	void Disconnect();

	bool WriteStart();
	bool Write(const char *buf, int count);
	bool WriteComplete();
	bool ReadStart() {return true;};
	bool Read(char *buf, int count);
	bool ReadComplete() {return true;};
};

}
