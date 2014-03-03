/**
 * Copyright (C) 2011, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

#include "PCMHostMain.h"

#include "utils/Log.h"
#include "utility.h"

#include "sidg_pcmif_server.hpp"
#include "sidg_pcmif_cb_client.hpp"

#include "AudioPCMAudioflinger.h"

const char * pcm_fromskypekitkey = "/tmp/" "pcm_from_skypekit_key";
const char * pcm_toskypekitkey = "/tmp/" "pcm_to_skypekit_key";

struct PCMHostMain::pData {
	Sid::SkypePCMInterfaceServer * pcmIf_server;
	Sid::SkypePCMCallbackInterfaceClient * pcmIf_cb_client;
	SkypePCMInterface* audioPcmIf;
};

PCMHostMain::PCMHostMain() {
	data = new pData;

	data->pcmIf_server = new Sid::SkypePCMInterfaceServer();
	data->pcmIf_cb_client = new Sid::SkypePCMCallbackInterfaceClient();

	data->audioPcmIf = new AudioPCMAudioflinger(data->pcmIf_cb_client);
	data->pcmIf_server->set_if(data->audioPcmIf);
}

PCMHostMain::~PCMHostMain() {
	delete data->pcmIf_server;
	delete data->pcmIf_cb_client;

	delete data;
	data = NULL;
}

bool PCMHostMain::Init() {
	F_LOG;
	return true;
}

void PCMHostMain::Run() {
	Log("Starting ..");

	data->pcmIf_server->Connect(pcm_fromskypekitkey, 0);
	data->pcmIf_cb_client->Connect(pcm_toskypekitkey, 500);

	Log("Connection created, entering loop..");

	Sid::Protocol::Status status;
	do {
		status = data->pcmIf_server->ProcessCommands();
	} while (status == Sid::Protocol::OK);

	delete data->audioPcmIf;
	data->pcmIf_server->Disconnect();
	data->pcmIf_cb_client->Disconnect();
}

