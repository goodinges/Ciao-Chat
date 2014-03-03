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

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>

//#define SID_LOG_MASK 0xff
#include "SidDebugging.hpp"

namespace Sid {

class UnixSocket : public Socket {
	const char *m_Key;
	bool m_ServerMode;
	bool m_IsConnected;
	int m_ListenSocket;
	int m_Socket;
	bool ClientConnect();
	bool ServerConnect(int timeout_ms);
	int MakeAddress(const char* name, struct sockaddr_un* pAddr);
public:
	UnixSocket();
	~UnixSocket();
	bool Connect(const char *key, bool serverMode, int timeout_ms);
	void Disconnect();
	bool IsConnected() const {return m_IsConnected;}

	int Write(const char *buf, int count);
	int Read(char *buf, int count);
	bool Poll(int socket, short int event_mask, int timeout_ms);
	bool PollRead();
	bool PollWrite();
};

Socket * GetSocket() {
	return new UnixSocket();
}

UnixSocket::UnixSocket() :
	m_Key(NULL),
	m_ServerMode(false),
	m_IsConnected(false),
	m_ListenSocket(-1),
	m_Socket(-1) {
}

UnixSocket::~UnixSocket(void) {
	if (m_IsConnected)
		Disconnect();
}

bool UnixSocket::Connect(const char *key, bool serverMode, int timeout_ms) {
	m_Key = key;
	m_ServerMode = serverMode;
	SID_INFO("Connect: %s, server: %d, timeout: %d", m_Key, m_ServerMode, timeout_ms);

	if (m_IsConnected) {
		SID_ERROR("Connect: %s - already connected", m_Key);
		return false;
	}

	if (m_ServerMode) {
		m_IsConnected = ServerConnect(timeout_ms);
		SID_INFO("Connect: %s server connect %s", m_Key, m_IsConnected? "suceeded" : "failed");
	} else {
		int elapsed_ms = 0;
		do {
			m_IsConnected = ClientConnect();
			if (!m_IsConnected) {
				usleep(100000);
				elapsed_ms += 100;
			}
		} while (!m_IsConnected && (timeout_ms < 0 || elapsed_ms < timeout_ms));
		SID_INFO("Connect: %s client connect %s", m_Key, m_IsConnected? "suceeded" : "failed");
	}

	return m_IsConnected;
}

void UnixSocket::Disconnect() {
	int ret;
	m_IsConnected = false;
	SID_INFO("UnixSocket: closing socket %s", m_Key);

	if (m_ListenSocket != -1) {
		ret = shutdown(m_ListenSocket, SHUT_RDWR);
		SID_INFO("m_ListenSocket shutdown ret: %d", ret);

		while ((ret = close(m_ListenSocket)) == -1 && errno == EINTR) /* temp failure, retrying */;
		SID_INFO("m_ListenSocket close ret: %d", ret);
		m_ListenSocket = -1;
	}

	ret = shutdown(m_Socket, SHUT_RDWR);
	SID_INFO("shutdown ret: %d", ret);

	while ((ret = close(m_Socket)) == -1 && errno == EINTR) /* temp failure, retrying */;
	SID_INFO("m_Socket close ret: %d", ret);
}

int UnixSocket::MakeAddress(const char* name, struct sockaddr_un* pAddr)
{
    int nameLen = strlen(name);
    if (nameLen >= (int) sizeof(pAddr->sun_path) - 1)  /* too long? */
        return -1;
    memset(pAddr, 0, sizeof(*pAddr));

    // create sockets in abstract namespace, because filesystem may be non-writable.
    #ifdef UNIX_SOCKET_NO_ABSTRACT_NAMESPACE
    strcpy(pAddr->sun_path, name);
    #else
    pAddr->sun_path[0] = '\0';  /* abstract namespace */
    strcpy(pAddr->sun_path+1, name);
    #endif

    pAddr->sun_family = AF_LOCAL;
    return 0;
}

bool UnixSocket::ClientConnect() {
	struct sockaddr_un dstaddr;
	if (MakeAddress(m_Key, &dstaddr) == -1) {
		SID_ERROR("Cannot make socket address %s: %s", m_Key, strerror(errno));
		return false;
	}

	if ((m_Socket = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1) {
		SID_ERROR("%s: failed to create socket: %s", m_Key, strerror(errno));
		return false;
	}

	SID_INFO("ClientConnect: connect");
	int resp;
	while ((resp = connect(m_Socket, (struct sockaddr *) &dstaddr, sizeof(dstaddr))) == -1 && errno == EINTR)
		/* temp failure, retrying */;
	if (resp == -1) {
		close(m_Socket);
		return false;
	}
	SID_INFO("ClientConnect: connect OK");

	long fl = fcntl(m_Socket, F_GETFL);
	fl |= O_NONBLOCK;
	fcntl(m_Socket, F_SETFL, fl);

	return true;
}

bool UnixSocket::ServerConnect(int timeout_ms) {
	struct sockaddr_un localaddr;
	bool result = false;

	if ((m_ListenSocket = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1) {
		SID_ERROR("%s: failed to create socket: %s", m_Key, strerror(errno));
		return false;
	} else {
		long fl = fcntl(m_ListenSocket, F_GETFL);
		fl |= O_NONBLOCK;
		fcntl(m_ListenSocket, F_SETFL, fl);

		do {
			unlink(m_Key);
			if (MakeAddress(m_Key, &localaddr) == -1) {
				SID_ERROR("Cannot make socket address %s: %s", m_Key, strerror(errno));
				break;
			}

			int resp = bind(m_ListenSocket, (struct sockaddr *) &localaddr, sizeof(localaddr));
			if (resp == -1) {
				SID_ERROR("%s: socket binding error: %s", m_Key, strerror(errno));
				break;
			}

			SID_INFO("ServerConnect: listen");
			if (listen(m_ListenSocket, 1)) {
				SID_ERROR("%s: socket listen error: %s", m_Key, strerror(errno));
				break;
			}

			SID_INFO("ServerConnect: Poll");
			if (!Poll(m_ListenSocket, POLLIN, timeout_ms)) {
				SID_ERROR("%s: poll m_ListenSocket failed: %s", m_Key, strerror(errno));
				break;
			}

			SID_INFO("ServerConnect: accept");
			struct sockaddr clntaddr;
			socklen_t len = sizeof(clntaddr);
			while ((m_Socket = accept(m_ListenSocket, (struct sockaddr *) &clntaddr, &len)) < 0 && errno != EINTR)
				/* temp failure, retrying */;
			if (m_Socket < 0) {
				SID_ERROR("%s: accept error: %d, %s", m_Key, errno, strerror(errno));
				break;
			}

			fl = fcntl(m_Socket, F_GETFL);
			fl |= O_NONBLOCK;
			fcntl(m_Socket, F_SETFL, fl);

			result = true;
		} while (0);

		if (shutdown(m_ListenSocket, SHUT_RDWR) < 0) {
			SID_ERROR("%s: m_ListenSocket shutdown error errno: %d, %s", m_Key, errno, strerror(errno));
		}

		if (close(m_ListenSocket) < 0) {
			SID_ERROR("%s: m_ListenSocket close error errno: %d, %s", m_Key, errno, strerror(errno));
		}
		m_ListenSocket = -1;
	}
	SID_INFO("ServerConnect: %s", result? "OK": "FAILED");

	return result;
}

int UnixSocket::Write(const char *buf, int count) {
	SID_VDBG("Write count, %d", count);
	int ret;
	while ((ret = write(m_Socket, buf, count)) < 0 && errno == EINTR);
	if (ret < 0)
		SID_VDBG("Write error: count, %d, ret %d, errno: %d", count, ret, errno);
	return ret;
}

int UnixSocket::Read(char *buf, int count) {
	if (!count)
		return 0;

	SID_VDBG("Read count, %d", count);
	int ret;
	while ((ret = read(m_Socket, buf, count)) < 0 && errno == EINTR);
	SID_VDBG("Read ret, %d", ret);

	if (ret < 0) {
		SID_VDBG("Read error count, %d, ret %d, errno: %d", count, ret, errno);
	}
	return ret;
}

static unsigned long long getTimeUs() {
	struct timeval tv;
	unsigned long long  ret;
	gettimeofday(&tv, NULL);
	ret = (unsigned long long)tv.tv_sec * 1000000 + tv.tv_usec;
	return ret;
}

bool UnixSocket::Poll(int socket, short int event_mask, int timeout_ms) {
	struct pollfd my_pollfd[1];

	do {
		memset(my_pollfd, 0, sizeof(my_pollfd));
		my_pollfd[0].fd = socket;
		my_pollfd[0].events = event_mask;

		while (poll(my_pollfd, 1, timeout_ms) < 0) {
			if (errno != EINTR) {
				SID_ERROR("Poll write failed on %s: errno: %d", m_Key, errno);
				return false;
			}
		}

		SID_DBG("my_pollfd[0].revents: 0x%x, mask: 0x%x", my_pollfd[0].revents, event_mask);

		if ((my_pollfd[0].revents & (POLLERR | POLLHUP | POLLNVAL))) {
			SID_ERROR("poll error: m_Key: %s, revents: 0x%x, errno: %d, time: %llu", m_Key, my_pollfd[0].revents, errno, getTimeUs());
			return false;
		}

		if (!my_pollfd[0].revents) {
			SID_ERROR("poll timeout, m_Key: %s", m_Key);
			return false;
		}
	} while (!(my_pollfd[0].revents & event_mask));

	return true;
}

bool UnixSocket::PollRead() {
	return Poll(m_Socket, POLLIN, -1);
}

bool UnixSocket::PollWrite() {
	return Poll(m_Socket, POLLOUT, -1);
}

}
