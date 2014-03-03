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

#include <errno.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileReader.hpp"
#include "Logging.hpp"

FileReader::FileReader(const char* devName, FileFormat format, FrameCallback* cb, int log) :
	m_pFh(NULL),
	m_Format(format),
	m_pCb(cb),
	m_pPacketBuf(NULL),
	m_PacketBufLen(0),
	m_SwapBytes(0),
	m_Log(log),
	Shutdown(0) {
	m_pFh = fopen(devName, "rb");
	if (!m_pFh) {
		fatal("Cannot open '%s': %s\n", devName, strerror(errno));
	}

	if (FILE_FMT_PCAP == m_Format) {
		pcap_hdr_t pcapHdr;

		if (Read(&pcapHdr, 1, sizeof(pcap_hdr_t))) {
			fatal("Can't read pcap header\n");;
		}

		if (0xa1b2c3d4 == pcapHdr.magic_number) {
			m_PacketBufLen = pcapHdr.snaplen;
			m_SwapBytes = 0;
		} else if (0xd4c3b2a1 == pcapHdr.magic_number) {
			m_PacketBufLen = SWAP32(pcapHdr.snaplen);
			m_SwapBytes = 1;
		} else {
			fatal("Invalid magic number '0x%x'\n", pcapHdr.magic_number);
		}
		m_pPacketBuf = (uint8_t *) malloc(m_PacketBufLen);
		if (!m_pPacketBuf) {
			fatal("Failed to allocate memory\n");
		}
	}
}

FileReader::~FileReader() {
	if (m_pPacketBuf) {
		free(m_pPacketBuf);
		m_pPacketBuf = NULL;
		m_PacketBufLen = 0;
	}
	if (m_pFh) {
		fclose(m_pFh);
		m_pFh = NULL;
	}
}

void FileReader::Run() {
	uint32_t packetLen = 0;

	while (!Shutdown) {
		if (ReadPacket(packetLen) != 0) {
			break;
		}

		m_pCb->TransmitFrame(m_pPacketBuf, packetLen);
	}
}

void FileReader::StartReading() {
	Run();
}

void FileReader::StopReading() {
	Shutdown = 1;
}

int FileReader::Read(void* ptr, size_t size, size_t count) {
	if (fread(ptr, size, count, m_pFh) != count) {
		if (feof(m_pFh)) {
			return -1;
		}
		if (ferror (m_pFh)) {
			fatal("Failed to read data: %s\n", strerror(errno));
		}
		fatal("Failed to read data\n");
	}

	return 0;
}

int FileReader::ReadPacket(uint32_t &packetLen) {
	switch (m_Format) {
	case FILE_FMT_PCAP:
		pcaprec_hdr_t recHdr;

		if (Read(&recHdr, 1, sizeof(pcaprec_hdr_t))) {
			return -1;
		}
		if (m_SwapBytes) {
			recHdr.ts_sec = SWAP32(recHdr.ts_sec);
			recHdr.ts_usec = SWAP32(recHdr.ts_usec);
			recHdr.incl_len = SWAP32(recHdr.incl_len);
			recHdr.orig_len = SWAP32(recHdr.orig_len);
		}
		packetLen = recHdr.incl_len;
		if (packetLen > m_PacketBufLen) {
			error("%d bytes snaplen limitation exceeded by %d bytes\n", m_PacketBufLen, (packetLen - m_PacketBufLen));
		}
		break;
	case FILE_FMT_DAT:
		if (Read(&packetLen, sizeof(uint16_t), 1)) {
			return -1;
		}
		packetLen = ntohs(packetLen);
		break;
	case FILE_FMT_WIN:
		if (Read(&packetLen, sizeof(uint32_t), 1)) {
			return -1;
		}
		break;
	case FILE_FMT_UDP:
		uint16_t udpHeader[4];
		if (Read(udpHeader, sizeof(uint16_t), sizeof(udpHeader)/sizeof(uint16_t))) {
			return -1;
		}
		packetLen = ntohs(udpHeader[2]) - 8;
		break;
	default:
		fatal("Unsupported input file type %d\n", m_Format);
	}

	if (packetLen > m_PacketBufLen) {
		m_PacketBufLen = packetLen;
		m_pPacketBuf = (uint8_t *) realloc(m_pPacketBuf, m_PacketBufLen);
		if (!m_pPacketBuf) {
			fatal("Failed to allocate memory\n");
		}
	}

	if (Read(m_pPacketBuf, 1, packetLen)) {
		return -1;
	}

	return 0;
}
