#include "SidAVServer.hpp"
#include "SidAVTransportWrapper.hpp"
#include <ctype.h>
#include "SidField.hpp"

#include <stdio.h>
#include <assert.h>

#include "SidDebugging.hpp"

namespace Sid {

int AVServer::StartServer(const char *transport_key, int timeout_ms) {
	return StartServer(transport_key, timeout_ms, NULL);
}


int AVServer::StartServer(const char *transport_key, int timeout_ms, DisconnectCb *callback) {
 	strncpy(m_transport_key, transport_key, sizeof(m_transport_key));
	m_transport_key[sizeof(m_transport_key) - 1] = 0;
	m_TimeoutMs = timeout_ms;
	m_pDisconnectCb = callback;

	if (timeout_ms > 0) {
		SID_DBG("AVServer::StartServer - Connect");
		if (!Connect(m_transport_key, timeout_ms)) {
			SID_ERROR("AVServer::StartServer - Connect failed");
			return 0;
		}
	}

	Start();
	for(int i = 0; i < 100 && !IsRunning(); i ++)
	  Sleep(10);
	return IsRunning();
}

void AVServer::StopServer() {
	Disconnect();
	if (!m_DisconnectPending)
		Stop();
}

int AVServer::Connect(const char *transport_key, int timeout_ms) {
	return m_transport.Connect(transport_key, true, timeout_ms);
}

void AVServer::Disconnect() {
	m_transport.terminate();
}

void AVServer::Run() {
	SID_DBG("AVServer::Connect");
	// connect needed here if starting server with infinite timeout
	// TODO: remove after audio ipc is updated to be similar with video
	if (!m_TimeoutMs)
		Connect(m_transport_key, 0);
	SID_DBG("AVServer::Run");
	m_DisconnectPending = false;
	Protocol::Status status;
	do {
		status = ProcessCommands();
	} while (status == Protocol::OK);

	SID_DBG("AVServer::Run disconnect");
	Disconnect();
	m_DisconnectPending = true;
	if (m_pDisconnectCb)
		m_pDisconnectCb->Disconnected();
	SID_DBG("AVServer::Run exit");
}

Protocol::Status AVServer::ProcessCommands() {
	Protocol::Status status = Protocol::OK;
	Protocol::Command cmd;
	uint rid;
	SID_VDBG("AVServerSession::ProcessCommands waiting for command");
	if ((status = m_protocol.rd_command(NULL, cmd)) == Protocol::OK) {
		SID_VDBG("AVServerSession::ProcessCommands processing command %c", cmd);
		switch (cmd) {
		case Protocol::CMD_XCALL: {
			uint target, method;
			SID_VDBG("AVServerSession::ProcessCommands reading call %c", cmd);
			if ((status = m_protocol.rd_call(NULL, rid, target, method)) == Protocol::OK) {
				SID_VDBG("AVServerSession::ProcessCommands dispatching call %d %d", rid, method);
				status = ProcessCall(rid, method);
				if (status == Protocol::ERR_CALL) {
					SID_ERROR("AVServerSession::ProcessCommands ProcessCall failed with ERR_CALL, respond with error");
					status = m_protocol.wr_response_error(NULL, rid);
					SID_ERROR("AVServerSession::ProcessCommands ProcessCall wr_response_error ret %d", status);
				} else if (status != Protocol::OK)
				{
					SID_ERROR("AVServerSession::ProcessCommands ProcessCall failed with status=%d", status);
				}
			}
			break;
		}
		default:
			m_protocol.resync_decode(NULL);
			SID_ERROR("AVServerSession::ProcessCommands Unknown command %d", cmd);
			// error uknown command
			status = Protocol::ERR_DECODE;
		}
	} else {
		m_protocol.resync_decode(NULL);
		SID_ERROR("AVServerSession::ProcessCommands command reading failed: %d", status);
	}

	return status;
}

}
