#include "SidAVTransportWrapper.hpp"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

//#define SID_LOG_MASK 0xF
#include "SidDebugging.hpp"

namespace Sid {

AVTransportWrapper::AVTransportWrapper() :
	m_Connected(false) {
	m_Transport = GetAVTransport();
}

AVTransportWrapper::~AVTransportWrapper() {
	delete m_Transport;
	SID_INFO("~AVTransportWrapper exit");
}

TransportInterface::Status AVTransportWrapper::bl_start_decoding(CommandInitiator* current_command_processor) {
	if (!m_Connected) {
		SID_ERROR("!m_Connected");
		return ERR_READ;
	}
	int ret = m_Transport->ReadStart();
	return (ret? OK: ERR_READ);

}

TransportInterface::Status AVTransportWrapper::end_decoding(CommandInitiator* current_command_processor) {
	if (!m_Connected) {
		SID_ERROR("!m_Connected");
		return ERR_READ;
	}
	int ret = m_Transport->ReadComplete();
	return (ret? OK: ERR_READ);
}

TransportInterface::Status AVTransportWrapper::bl_start_encoding(CommandInitiator* current_command_processor) {
	if (!m_Connected) {
		SID_ERROR("!m_Connected");
		return ERR_WRITE;
	}
	int ret = m_Transport->WriteStart();
	return (ret? OK: ERR_WRITE);
}

TransportInterface::Status AVTransportWrapper::end_encoding(CommandInitiator* current_command_processor, bool) {
	if (!m_Connected) {
		SID_ERROR("!m_Connected");
		return ERR_WRITE;
	}
	int ret = m_Transport->WriteComplete();
	return (ret? OK: ERR_WRITE);
}

TransportInterface::Status AVTransportWrapper::bl_read_bytes(CommandInitiator* curr, uint num_bytes, char* dest) {
	if (!m_Connected) {
		SID_ERROR("!m_Connected");
		return ERR_READ;
	}
	bool ret = true;

	// BinCommon uses bl_read_bytes with dest = NULL when skipping values
	if (dest == NULL) {
		char tmp_buf[1];
		for (uint i = 0; i < num_bytes; i++) {
			ret = m_Transport->Read(tmp_buf, 1);
			if (!ret)
				break;
		}
	} else {
		ret = m_Transport->Read(dest, num_bytes);
	}
	return (ret? OK: ERR_READ);
}

TransportInterface::Status AVTransportWrapper::bl_write_bytes(CommandInitiator* curr, uint num_bytes, const char* src) {
	if (!m_Connected) {
		SID_ERROR("!m_Connected");
		return ERR_WRITE;
	}
	int ret = m_Transport->Write(src, num_bytes);
	return (ret? OK: ERR_WRITE);

}

/* key - file name to be used as a local socket
 * server_mode - whetehr internal socket will used in server mode or client mode
 * block_timeout_ms - if 0 then blocks forever, otherwise will wait certain number of milliseconds before fails*/
int AVTransportWrapper::Connect(const char* key, bool server_mode, int block_timeout_ms) {
	if (m_Connected) {
		SID_ERROR("AVTransportWrapper (%s): already connected", key);
		return 0;
	}

	if (m_Transport->Connect(key, server_mode, block_timeout_ms? block_timeout_ms: -1)) {
		m_Connected = true;
		SID_INFO("AVTransportWrapper (%s): connected!", key);
		return 1;
	} else {
		return 0;
	}

}

void AVTransportWrapper::terminate() {
	m_Transport->Disconnect();
	m_Connected = false;
}

int AVTransportWrapper::IsConnected()	{
	return m_Connected;
}



} // namespace Sid
