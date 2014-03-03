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

#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "Logging.hpp"
#include "FileWriter.hpp"

FileWriter::FileWriter(const char *devName, FileFormat format, int log) :
	m_pFh(0),
	m_Format(format),
	m_Log(log),
	m_PacketBufLen(0x100000) {
	m_pFh = fopen(devName, "wb");
	if (!m_pFh) {
		fatal("Cannot open '%s': %s\n", devName, strerror(errno));
	}

	if (FILE_FMT_PCAP == m_Format) {
		pcap_hdr_t pcapHdr = {0xa1b2c3d4, 2, 4, 0, 0, m_PacketBufLen, 1};
		Write(&pcapHdr, 1, sizeof(pcap_hdr_t));
	}
}

FileWriter::~FileWriter() {
	// Write dummy because it can not end with 0 bytes
	if (m_pFh && (FILE_FMT_BIT == m_Format)) {
		int16_t dummy = -1;
		Write(&dummy, sizeof(int16_t), 1);
	}

	m_PacketBufLen = 0;
	if (m_pFh) {
		fclose(m_pFh);
		m_pFh = NULL;
	}
}

int FileWriter::Write(const void* ptr, size_t size, size_t count) {
	if (fwrite(ptr, size, count, m_pFh) != count) {
		if (ferror (m_pFh)) {
			fatal("Failed to write data: %s\n", strerror(errno));
		}
		fatal("Failed to write data\n");
	}

	return 0;
}

void FileWriter::WritePacket(const uint8_t* data, uint32_t packetLen) {
	switch (m_Format) {
	case FILE_FMT_PCAP:
		struct timeval tv;
		pcaprec_hdr_t recHdr;

		if (gettimeofday(&tv, NULL) < 0) {
			fatal("gettimeofday failed: %s\n", strerror(errno));
		}

		recHdr.ts_sec = tv.tv_sec;
		recHdr.ts_usec = tv.tv_usec;
		if (packetLen > m_PacketBufLen) {
			recHdr.incl_len = m_PacketBufLen;
			error("Lost %d bytes due to %d bytes snaplen limitation\n", (packetLen - m_PacketBufLen), m_PacketBufLen);
			packetLen = m_PacketBufLen;
		} else {
			recHdr.incl_len = packetLen;
		}
		recHdr.orig_len = packetLen;

		Write(&recHdr, 1, sizeof(pcaprec_hdr_t));
		break;
	case FILE_FMT_DAT:
		uint16_t datHeader;
		datHeader = htons(packetLen);
		Write(&datHeader, sizeof(uint16_t), 1);
		break;
	case FILE_FMT_BIT:
		Write(&packetLen, sizeof(uint16_t), 1);
		break;
	case FILE_FMT_WIN:
		Write(&packetLen, sizeof(uint32_t), 1);
		break;
	case FILE_FMT_UDP:
		uint16_t udpHeader[4];
		udpHeader[0] = htons(0x1111);
		udpHeader[1] = htons(0x2222);
		udpHeader[2] = htons(packetLen + 8);
		udpHeader[3] = htons(0x3333);
		Write(udpHeader, sizeof(uint16_t), sizeof(udpHeader)/sizeof(uint16_t));
		break;
	case FILE_FMT_RAW:
		break;
	default:
		fatal("Unsupported output file type %d\n", m_Format);
	}

	if (packetLen) {
		Write(data, 1, packetLen);
	}
}
