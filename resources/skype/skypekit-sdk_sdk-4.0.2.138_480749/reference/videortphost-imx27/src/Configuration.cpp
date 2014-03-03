/*
 * Copyright (C) 2010 Skype Technologies S.A. Confidential and proprietary
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
#include "Configuration.hpp"
#include "debugging.hpp"
#include <string.h>
#include <errno.h>

#define CFG_MAX_STR 512

Configuration* Configuration::GetCfg() {
	static Configuration instance;
	return &instance;
}

Configuration::Configuration() :
	m_InitDone(false),
	m_CfgFile(NULL) {
}

int Configuration::Init(const char *filename) {
	if (m_InitDone) {
		WARN("Configuration::Init - already initialized");
		return -1;
	}
#ifdef MINIMAL_MODE
	int sendAnxb = 1;
	int sendFromCamera = 0;
	int render = 0;
#else
	int sendAnxb = 0;
	int sendFromCamera = 1;
	int render = 1;
#endif

	const char * defaultSendAnxbFileDir = "../reference/videortphost-test/testvideos";

	int index = 0;
	m_Entries[index++].initIntEntry("UseLoopback", 0,
			"act as a loopback engine");
	m_Entries[index++].initIntEntry("DumpReceiveRtpPackets", 0,
			"dump received RTP packets to received_rtp_packets.pcap");
	m_Entries[index++].initIntEntry("DumpReceiveAnxbPackets", 0,
			"dump received H264 Annex B packets to received_anxb_packets.264");
	m_Entries[index++].initIntEntry("DumpSendRtpPackets", 0,
			"dump sent RTP packets to sent_rtp_packets.pcap");
	m_Entries[index++].initIntEntry("DumpSendAnxbPackets", 0,
			"dump sent H264 Annex B packets to sent_anxb_packets.264");
	m_Entries[index++].initIntEntry("SendPacketsFromAnxbFile", sendAnxb,
			"send packets from H.264 Annex B file");
	m_Entries[index++].initStrEntry("SendAnxbFileDir",
			defaultSendAnxbFileDir,
			"Directory containing test videos");
	m_Entries[index++].initIntEntry("SendPacketsFromPcapFile", 0,
			"send packets from PCAP file");
	m_Entries[index++].initIntEntry("SendVideoFromCamera", sendFromCamera,
			"send video from camera");
	m_Entries[index++].initStrEntry("VideoCameraDevName", "/dev/video0",
			"video camera dev node");
	m_Entries[index++].initIntEntry("RenderVideo", render,
			"decode & render incoming video");
	m_Entries[index++].initIntEntry("UseDummyCapturer", 0,
			"use a captured video signal generator instead of camera");
	m_Entries[index++].initIntEntry("ForceSendBitrate", 0,
			"force certain send bitrate instead of following the bitrate set by SetBitrate command");
	m_Entries[index++].initIntEntry("SendVGAOnly", 0,
			"send only VGA resolution");
	m_Entries[index++].initIntEntry("LimitReceiveBitrate", 500,
			"limit receive bitrate to bitrate_kbps");
	m_Entries[index++].initIntEntry("LimitSendBitrate", 500,
			"limit send bitrate to bitrate_kbps (ignore higher values given by SetBitrate command)");
	m_Entries[index++].initIntEntry("ReceiveInitialFPS", 15,
			"initial framerate video parameter");
	m_Entries[index++].initIntEntry("EnableSendSetFramerate", 0,
			"enable dynamic sender framerate control");
	m_Entries[index++].initIntEntry("PayloadFormat", 0,
			"Use either rtp packets (value 0) or annex-B frames (value 1)");
	m_Entries[index++].initIntEntry("TogglePayloadFormat", 0,
			"Toggle between PAYLOAD_RTP and PAYLOAD_FRAMES after each new startrecording call. (for testing purposes)");
	m_Entries[index++].initIntEntry("", 0, "");

	assert(index < NR_ENTRIES);

	CreateConfigFile(filename);

	m_CfgFile = fopen(filename, "r");
	if (m_CfgFile == NULL) {
		WARN("Can't open file %s", filename);
		return -1;
	} else {
		INFO("Opened conf file %s", filename);
	}

	for (int i = 0; m_Entries[i].name[0]; i++) {
		if (m_Entries[i].type == ConfEntry::TYPE_INT) {
			ReadInt(m_Entries[i].name, m_Entries[i].intval);
		} else {
			ReadStr(m_Entries[i].name, m_Entries[i].strval);
		}
	}
	m_InitDone = true;
	return 0;
}

Configuration::~Configuration() {
	if (m_CfgFile) {
		fclose(m_CfgFile);
	}
}

int Configuration::GetIntVal(const char *key) {
	for (int i = 0; m_Entries[i].name[0]; i++) {
		if (m_Entries[i].type == ConfEntry::TYPE_INT) {
			if (strlen(m_Entries[i].name) == strlen(key) &&
				!strcmp(m_Entries[i].name, key))
				return m_Entries[i].intval;
		}
	}
	ERROR("Unknown int parameter asked: %s", key);
	return 0;
}

char *Configuration::GetStrVal(const char *key) {
	for (int i = 0; m_Entries[i].name[0]; i++) {
		if (m_Entries[i].type == ConfEntry::TYPE_STR) {
			if (strlen(m_Entries[i].name) == strlen(key) &&
				!strcmp(m_Entries[i].name, key))
				return m_Entries[i].strval;
		}
	}
	ERROR("Unknown string parameter asked: %s", key);
	return 0;
}

void Configuration::CreateConfigFile(const char *filename) {
	char tmpFilename[512];
	snprintf(tmpFilename, sizeof(tmpFilename), "%s.example", filename);
	INFO("Creating an example conf file %s (rename to %s manually)", tmpFilename, filename);
	FILE *wrfile = fopen(tmpFilename, "w");
	if (wrfile == NULL) {
		WARN("Unable to create: %s", tmpFilename);
		return;
	}
	for (int i = 0; m_Entries[i].name[0]; i++) {
		if (m_Entries[i].type == ConfEntry::TYPE_INT) {
			fprintf(wrfile, "#%s\n%s=%d\n", m_Entries[i].comment, m_Entries[i].name, m_Entries[i].intval);
		} else {
			fprintf(wrfile, "#%s\n%s=%s\n", m_Entries[i].comment, m_Entries[i].name, m_Entries[i].strval);
		}
	}
	fclose(wrfile);
	return;
}

int Configuration::ReadInt(const char *key, int &value) {
	if (FindKey(key, tmpLine)) {
		if (sscanf(&tmpLine[strlen(key) + 1], "%d", &value)) {
			return 1;
		}
	}
	return 0;
}

int Configuration::ReadStr(const char *key, char *value) {
	if (FindKey(key, tmpLine)) {
		char fmtstr[50];
		int maxlen = CFG_MAX_STR - strlen(key) + 1;
		sprintf(fmtstr, "%%%ds", maxlen);
		sscanf(&tmpLine[strlen(key) + 1], fmtstr, value);
		return 1;
	}
	return 0;
}

int Configuration::FindKey(const char *key, char *line) {
	fseek(m_CfgFile, 0, SEEK_SET);
	while(fgets(line, CFG_MAX_STR, m_CfgFile)) {
		if (strlen(line) > strlen(key) &&
			!strncmp(key, line, strlen(key)) &&
			line[strlen(key)] == '=')
		{
			return 1;
		}
	}
	return 0;
}

