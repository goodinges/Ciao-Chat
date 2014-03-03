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

#include <unistd.h>
#include <stdio.h>

#include "debugging.hpp"
#include "Configuration.hpp"

#ifdef SKYPEKIT
#include "SkypekitTestEngine.hpp"
#else
#include "TestRTPInterface.hpp"
#endif
#include "rfc3984.hpp"

#ifndef MINIMAL_MODE
#include "V4l2Capturer.hpp"
#include "H264Encoder.hpp"
#include "X11Renderer.hpp"
#include "H264Decoder.hpp"
#endif

class LoopbackTransport : public SkypeVideoRTPInterfaceCb
{
public:
	LoopbackTransport()
		:intf(NULL) {}
	void SetIf(SkypeVideoRTPInterface *intf_) {
		intf = intf_;
	}
	void SendRTPPacket(const Sid::Binary &data) {
		intf->RtpPacketReceived(data);
	}	
	void SendFrame(const Sid::Binary &data, int timespamp_ms) {
		intf->FrameReceived(data, timespamp_ms);
	}
	SkypeVideoRTPInterface *intf;
};

#include <iostream>
#include <fstream>
using namespace std;

int main() {
	FUNCLOG;

	LoopbackTransport tr;
	SkypeVideoRTPInterface *intf = SkypeVideoRTPInterfaceGet(&tr);
	tr.SetIf(intf);

	intf->StartPlayback(MAKEFOURCC('H', '2', '6', '4'), 640, 480);
	intf->StartRecording(MAKEFOURCC('H', '2', '6', '4'), 1368, 4000, 640, 480, 15);
	intf->SetBitrate(100000);
	sleep(15);
	intf->SetFramerate(30);
	sleep(15);

	intf->StopRecording();
	intf->StopPlayback();
	SkypeVideoRTPInterfaceRelease(intf);

}
