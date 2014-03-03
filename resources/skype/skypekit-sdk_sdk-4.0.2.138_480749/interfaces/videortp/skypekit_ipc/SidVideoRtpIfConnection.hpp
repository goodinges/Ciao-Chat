#ifndef SKYPEVIDEORTPIFCONNECTION_HPP_
#define SKYPEVIDEORTPIFCONNECTION_HPP_

#include "SidVideoRTPInterface.hpp"
#include "sidg_videortpif_cb_server.hpp"
#include "sidg_videortpif_client.hpp"
namespace Sid {

class SidVideoRtpIfConnection {
public:
	struct DisconnectCallback {
		virtual void Disconnected() = 0;
	};
	SidVideoRtpIfConnection(SkypeVideoRTPInterfaceCb &callback, DisconnectCallback &disconnectCallback) :
		m_Initialized(false),
		m_SkypeVideoRTPInterfaceCb(callback),
		m_IpcDisconnect(disconnectCallback),
		m_AvserverDisconnect(*this) {}
	~SidVideoRtpIfConnection() {
		Disconnect();
	}
	SkypeVideoRTPInterface* Interface();
	bool IsConnected();
	bool EnsureConnection();
	void Disconnect();

private:
	struct AVServerDisconnect : public Sid::AVServer::DisconnectCb {
		SidVideoRtpIfConnection &m_Connection;
		AVServerDisconnect(SidVideoRtpIfConnection &connection) :
			m_Connection(connection){}
		void Disconnected();
	};

	bool m_Initialized;
	SkypeVideoRTPInterfaceCb &m_SkypeVideoRTPInterfaceCb;
	DisconnectCallback &m_IpcDisconnect;
	AVServerDisconnect m_AvserverDisconnect;
	SkypeVideoRTPInterfaceClient m_RtpIfClient;
	SkypeVideoRTPInterfaceCbServer m_RtpIfServer;
};

}
#endif /* SKYPEVIDEORTPIFCONNECTION_HPP_ */
