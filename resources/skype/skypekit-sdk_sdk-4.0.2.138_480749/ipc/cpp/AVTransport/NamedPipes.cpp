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

//#define SID_LOG_MASK 0xff
#include "SidDebugging.hpp"
#include <windows.h>

namespace Sid {

class NamedPipes : public Socket {
	const char *m_Key;
	bool m_ServerMode;
	bool m_IsConnected;
	HANDLE m_hPipe;
	bool ClientConnect(int timeout_ms);
	bool ServerConnect(int timeout_ms);
public:
	NamedPipes();
	~NamedPipes();
	bool Connect(const char *key, bool serverMode, int timeout_ms);
	void Disconnect();
	bool IsConnected() const {return m_IsConnected;}

	int Write(const char *buf, int count);
	int Read(char *buf, int count);
	bool PollRead();
	bool PollWrite();
};

Socket * GetSocket() {
	return new NamedPipes();
}

NamedPipes::NamedPipes() :
	m_Key(NULL),
	m_ServerMode(false),
	m_IsConnected(false),
	m_hPipe(INVALID_HANDLE_VALUE) {
}

NamedPipes::~NamedPipes(void) {
	if (m_IsConnected)
		Disconnect();
}

bool NamedPipes::Connect(const char *key, bool serverMode, int timeout_ms) {
	m_Key = key;
	m_ServerMode = serverMode;
	SID_DBG("Connect: %s, server: %d, timeout: %d", m_Key, m_ServerMode, timeout_ms);

	if (m_IsConnected) {
		SID_ERROR("Connect: %s - already connected", m_Key);
		return false;
	}

	if (m_ServerMode) {
		m_IsConnected = ServerConnect(timeout_ms);
		SID_DBG("Connect: %s server connect %s", m_Key, m_IsConnected? "suceeded" : "failed");
	} else {
		int elapsed_ms = 0;
		do {
			m_IsConnected = ClientConnect(100);
			elapsed_ms += 100;
		} while (!m_IsConnected && (timeout_ms < 0 || elapsed_ms < timeout_ms));
		SID_DBG("Connect: %s client connect %s", m_Key, m_IsConnected? "suceeded" : "failed");
	}

	return m_IsConnected;
}

void NamedPipes::Disconnect() {
	m_IsConnected = false;
	SID_DBG("NamedPipes: closing socket %s", m_Key);

	int ret = CloseHandle(m_hPipe);
	SID_DBG("m_Socket close ret: %d", ret);
}

bool NamedPipes::ClientConnect(int timeout_ms) {
	m_hPipe = CreateFile( m_Key, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hPipe == INVALID_HANDLE_VALUE) {
		if(GetLastError() == ERROR_PIPE_BUSY)
		{
			SID_ERROR("%s: failed to connect, pipe busy", m_Key);
			WaitNamedPipe(m_Key,timeout_ms);
		}
		else
		{
			Sleep(timeout_ms);
			SID_ERROR("%s: failed to connect: %d", m_Key, GetLastError());
		}
		return false;
	}
	return true;
}

bool NamedPipes::ServerConnect(int timeout_ms) {
	do
	{
		m_hPipe = CreateNamedPipe(m_Key, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
			1, 1024, 1024, (timeout_ms < 0)? 10000:timeout_ms, NULL);
		if (m_hPipe == INVALID_HANDLE_VALUE) {
			SID_ERROR("%s: failed to create named pipe: %d", m_Key, GetLastError());
			return false;
		}
		if( ConnectNamedPipe(m_hPipe, NULL))
			return true;

		SID_ERROR("%s: failed to connect named pipe: %d", m_Key, GetLastError());
		CloseHandle(m_hPipe);
	}
	while(timeout_ms < 0);
	return false;
}

int NamedPipes::Write(const char *buf, int count) {
	DWORD b = 0;

	if (!count)
		return 0;
	SID_VDBG("Write count, %d", count);

	bool ret = WriteFile(m_hPipe, buf, count, &b, NULL);
	if (!ret) 
	{
		SID_DBG("Write error count %d, error: %d", count, GetLastError());
		return 0;
	}
	return b;
}

int NamedPipes::Read(char *buf, int count) {
	DWORD b = 0;

	if (!count)
		return 0;
	SID_VDBG("Read count, %d", count);

	bool ret = ReadFile(m_hPipe, buf, count, &b, NULL);
	if (!ret) 
	{
		SID_DBG("Read error count %d, error: %d", count, GetLastError());
		return 0;
	}
	return b;
}

bool NamedPipes::PollRead() {
	return 1;
}

bool NamedPipes::PollWrite() {
	return 1;
}

}
