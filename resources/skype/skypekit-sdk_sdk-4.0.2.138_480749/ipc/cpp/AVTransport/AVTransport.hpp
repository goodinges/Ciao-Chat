/*
 * CommInterface.hpp
 *
 *  Created on: May 11, 2010
 *      Author: lauri
 */

#ifndef COMMINTERFACE_HPP_
#define COMMINTERFACE_HPP_

#if defined _WIN32
#define IPC_PREFIX_DEFAULT "\\\\.\\pipe\\"
#else
#define IPC_PREFIX_DEFAULT "/tmp/"
#endif

#ifndef TRANSPORT_TEST
#include "SidPlatform.hpp"
#endif

namespace Sid {

class AVTransport {
public:
	virtual ~AVTransport() {}

	virtual bool Connect(const char *key, bool serverMode, int timeout_ms) = 0;
	virtual void Disconnect() = 0;

	virtual bool WriteStart() = 0;
	virtual bool Write(const char *buf, int count) = 0;
	virtual bool WriteComplete() = 0;
	virtual bool ReadStart() = 0;
	virtual bool Read(char *buf, int count) = 0;
	virtual bool ReadComplete() = 0;
};

extern AVTransport * GetAVTransport();

}

#endif /* COMMINTERFACE_HPP_ */
