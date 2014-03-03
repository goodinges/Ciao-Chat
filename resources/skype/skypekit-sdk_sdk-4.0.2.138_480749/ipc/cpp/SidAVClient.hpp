#ifndef SIDAVCLIENT_HPP_
#define SIDAVCLIENT_HPP_
#include <stdio.h>

#include "SidPlatform.hpp"
#include "SidProtocolBinClient.hpp"
#include "SidAVTransportWrapper.hpp"

using namespace Sid::Protocol;

namespace Sid {

class AVClient {
public:

	AVClient(Field* descriptors) :
		m_protocol(&m_transport,descriptors)
	{}

	virtual ~AVClient() {}

	void Connect(const char *transport_key, int timeout_ms) {
		m_transport.Connect(transport_key, false, timeout_ms);
	}

	void Disconnect() {
		if (m_transport.IsConnected())
			m_transport.terminate();
	}
	
	bool IsConnected() { return m_transport.IsConnected(); }

protected:
	AVTransportWrapper m_transport;
	Sid::Protocol::BinClient m_protocol;
};

}

#endif
