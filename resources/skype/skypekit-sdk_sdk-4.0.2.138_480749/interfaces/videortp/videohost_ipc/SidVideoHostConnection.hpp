/*
 * SidVideoRtpIfConnection.hpp
 *
 *  Created on: Apr 15, 2011
 *      Author: lauri
 */

#ifndef SIDVIDEOPHOSTCONNECTION_HPP_
#define SIDVIDEORTPIFCONNECTION_HPP_

#include "sidg_videortpif_server.hpp"
#include "SidVideoRTPInterface.hpp"
#include "sidg_videortpif_cb_client.hpp"

namespace Sid {
struct SidVideoHostConnection : public Sid::Thread {
	SidVideoHostConnection() : m_pRtpIf(NULL) {}
	~SidVideoHostConnection() {
		Disconnect();
		WaitDisconnect();
	}

	void StartIpcThread(const char *ipcprefix);
	void Disconnect();
	void WaitDisconnect();

	SkypeVideoRTPInterface* m_pRtpIf;
	Sid::Mutex m_Mutex;
	Sid::SkypeVideoRTPInterfaceCbClient m_RtpIfClient;
	Sid::SkypeVideoRTPInterfaceServer m_RtpIfServer;
	bool m_Started;
	char m_FromSkypekitKey[512];
	char m_ToSkypekitKey[512];
private:
	void Run();
};

}

#endif /* SIDVIDEORTPIFCONNECTION_HPP_ */
