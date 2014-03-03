#include "SidVideoRtpIfConnection.hpp"
#include "AVIpcConfig.hpp"
#include "SidDebugging.hpp"

namespace Sid {

void SidVideoRtpIfConnection::AVServerDisconnect::Disconnected() {
	m_Connection.Disconnect();
	m_Connection.m_IpcDisconnect.Disconnected();
}

SkypeVideoRTPInterface* SidVideoRtpIfConnection::Interface() {
	return &m_RtpIfClient;
}

bool SidVideoRtpIfConnection::IsConnected() {
	Sid::Mutex::ScopedLock l(m_RtpIfClient.m_Mutex);
	return m_Initialized;
}

bool SidVideoRtpIfConnection::EnsureConnection() {
	Sid::Mutex::ScopedLock l(m_RtpIfClient.m_Mutex);

	if (!m_Initialized) {
		SID_DBG("SidVideoRTPInterfaceConnection::EnsureConnection - !initialized");
		m_RtpIfServer.set_if(&m_SkypeVideoRTPInterfaceCb);

		char fromskypekitkey[512];
		char toskypekitkey[512];

		snprintf(fromskypekitkey, sizeof(fromskypekitkey), "%svidrtp_from_skypekit_key", AVIpcConfig::g_AVIpcConfig.VideoIpcKeyPrefix());
		snprintf(toskypekitkey, sizeof(toskypekitkey), "%svidrtp_to_skypekit_key", AVIpcConfig::g_AVIpcConfig.VideoIpcKeyPrefix());

		m_RtpIfClient.Connect(fromskypekitkey, 500);
		if (!m_RtpIfClient.IsConnected()) {
			SID_ERROR("SidVideoRTPInterfaceConnection::EnsureConnection - client connection was not established\n");
			return false;
		}

		if (!m_RtpIfServer.StartServer(toskypekitkey, 500, &m_AvserverDisconnect)) {
			m_RtpIfClient.Disconnect();
			SID_ERROR("SidVideoRTPInterfaceConnection::EnsureConnection - server connection was not established\n");
			return false;
		}

		SID_DBG("SidVideoRTPInterfaceConnection::EnsureConnection - OK");
		m_Initialized = true;
	}
	return m_Initialized;
}


void SidVideoRtpIfConnection::Disconnect() {
	m_RtpIfClient.m_Mutex.lock();
	if (m_Initialized) {
		m_Initialized = false;

		m_RtpIfClient.Disconnect();
		m_RtpIfClient.m_Mutex.unlock();
		m_RtpIfServer.StopServer();
		return;
	}
	m_RtpIfClient.m_Mutex.unlock();
}

}
