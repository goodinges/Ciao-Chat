
#include "SidVideoHostConnection.hpp"

#include "SidProtocolEnums.hpp"
#include "SidDebugging.hpp"

namespace Sid {
void SidVideoHostConnection::StartIpcThread(const char *ipcprefix) {
	Sid::Mutex::ScopedLock l(m_Mutex);
	SID_INFO("StartVideohostThread");

	m_pRtpIf = SkypeVideoRTPInterfaceGet(&m_RtpIfClient);
	m_RtpIfServer.set_if(m_pRtpIf);

	snprintf(m_FromSkypekitKey, sizeof(m_FromSkypekitKey), "%svidrtp_from_skypekit_key", ipcprefix);
	snprintf(m_ToSkypekitKey, sizeof(m_ToSkypekitKey), "%svidrtp_to_skypekit_key", ipcprefix);
	Start();
	m_Started = true;
}

void SidVideoHostConnection::Disconnect() {
	m_RtpIfServer.Disconnect();
	m_RtpIfClient.Disconnect();
}

void SidVideoHostConnection::Run() {
	SID_INFO("StartVideohostThread enter");
	if (m_RtpIfServer.Connect(m_FromSkypekitKey, 0)) {
		m_RtpIfClient.Connect(m_ToSkypekitKey, 500);
		if (!m_RtpIfClient.IsConnected()) {
			m_RtpIfServer.Disconnect();
			SID_ERROR("StartVideohostThread m_RtpIfClient connection failed");
			return;
		}
	} else {
		SID_ERROR("StartVideohostThread m_RtpIfServer connection failed");
		return;
	}
	SID_INFO("StartVideohostThread connected");
	Sid::Protocol::Status status;
	do {
		status = m_RtpIfServer.ProcessCommands();
	} while (status == Sid::Protocol::OK);
	Disconnect();
	SID_INFO("StartVideohostThread exit");
}

void SidVideoHostConnection::WaitDisconnect() {
	Sid::Mutex::ScopedLock l(m_Mutex);
	SID_INFO("WaitDisconnect enter");

	if (m_Started) {
		Stop();
		SkypeVideoRTPInterfaceRelease(m_pRtpIf);
		m_pRtpIf = NULL;
		m_RtpIfServer.Disconnect();
		m_RtpIfClient.Disconnect();
		m_Started = false;
	} else {
		SID_ERROR("WaitDisconnect - thread not started");
		return;
	}
}

}
