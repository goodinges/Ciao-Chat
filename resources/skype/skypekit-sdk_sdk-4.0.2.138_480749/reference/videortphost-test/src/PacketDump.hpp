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

#ifndef PACKETDUMP_H_
#define PACKETDUMP_H_

#include "debugging.hpp"

#include "PcapHandler.hpp"
#include <errno.h>

enum DumpFormat {
	PDUMP_FMT_RAW,
	PDUMP_FMT_PCAP
};

/*! \class PacketDump
 *  Saves the video RTP stream in a disk file.
 */
class PacketDump {
public:
	PacketDump(const char *dumpFile, DumpFormat format) : 
		m_pFh(0),
		m_PcapWriter(NULL),
		m_Format(format)
	{
		if (m_Format == PDUMP_FMT_PCAP) {
			m_PcapWriter = new PcapWriter(dumpFile, 0x100000);
		} else {
			m_pFh = fopen(dumpFile, "wb");
			if (!m_pFh)
				FATAL("Opening file %s failed", dumpFile);
		}		
	}
	~PacketDump() {
		if (m_pFh)
			fclose(m_pFh);
		
		if (m_PcapWriter)
			delete m_PcapWriter;
	}
	void StorePacket(const void *data, int data_length) {
		if (m_Format == PDUMP_FMT_PCAP) {
			m_PcapWriter->WritePacket(data, data_length);
		} else {
			int ret = fwrite(data, 1, data_length, m_pFh);
			if (ret != data_length)
				FATAL("fwrite failed: errno %d", errno);
		}
	}
private:
	FILE * m_pFh;
	PcapWriter *m_PcapWriter;
	DumpFormat m_Format;
};

#endif /* RTPDUMP_H_ */
