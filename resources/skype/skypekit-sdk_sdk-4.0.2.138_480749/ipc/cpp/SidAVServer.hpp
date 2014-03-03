#ifndef SIDAVSERVER_HPP_
#define SIDAVSERVER_HPP_

#include "SidPlatform.hpp"
#include "SidProtocolBinServer.hpp"
#include "SidAVTransportWrapper.hpp"

namespace Sid {

class AVServer: public Thread {
public:
	struct DisconnectCb {
		virtual ~DisconnectCb() {};
		virtual void Disconnected() = 0;
	};

	AVServer(Field* descriptors) : m_pDisconnectCb(NULL), m_protocol(0,&m_transport,descriptors) {
	}

	virtual ~AVServer() {};

	/* Use startserver/stopserver, if you need a thread. */
	int StartServer(const char *transport_key, int timeout_ms);
	int StartServer(const char *transport_key, int timeout_ms, DisconnectCb *callback);
	void StopServer();
	/* If you create a thread yourself then use connect/disconnect */
	int Connect(const char *transport_key, int timeout_ms);
	void Disconnect();

	bool IsConnected() { return m_transport.IsConnected(); }
    void set_if(void *interface) { m_pIf = interface; }
    void* get_if() const { return m_pIf; }
	Protocol::Status ProcessCommands();
	virtual Protocol::Status ProcessCall(int rid, int method) = 0;
protected:
	void Run();

	AVTransportWrapper m_transport;
	DisconnectCb *m_pDisconnectCb;
	Protocol::BinServer m_protocol;
	void* m_pIf;
	void* m_pIfTransport;
	bool m_DisconnectPending;
	char m_transport_key[512];
	int m_TimeoutMs;
};

}

#endif /* SIDAVSERVERSESSION_HPP_ */
