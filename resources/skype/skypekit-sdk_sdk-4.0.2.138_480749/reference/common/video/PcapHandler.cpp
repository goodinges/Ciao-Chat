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
 *
 *  Created on: Aug 5, 2009
 *      Author: lauri
 */

#include "PcapHandler.hpp"
#include "debugging.hpp"

#include <errno.h>
#include <sys/time.h>
#include <time.h>

typedef struct pcap_hdr_s {
	uint32_t magic_number;   /* magic number */
	uint16_t version_major;  /* major version number */
	uint16_t version_minor;  /* minor version number */
	int32_t  thiszone;       /* GMT to local correction */
	uint32_t sigfigs;        /* accuracy of timestamps */
	uint32_t snaplen;        /* max length of captured packets, in octets */
	uint32_t network;        /* data link type */
} __attribute__((packed)) pcap_hdr_t;

typedef struct pcaprec_hdr_s {
	uint32_t ts_sec;         /* timestamp seconds */
	uint32_t ts_usec;        /* timestamp microseconds */
	uint32_t incl_len;       /* number of octets of packet saved in file */
	uint32_t orig_len;       /* actual length of packet */
} __attribute__((packed)) pcaprec_hdr_t;

#define SWAP16(A) \
	((((A) & 0xFF00) >> 8) | \
	 (((A) & 0x00FF) << 8))

#define SWAP32(A) \
	((((A) & 0xFF000000) >> 24) | \
	 (((A) & 0x00FF0000) >> 8)  | \
	 (((A) & 0x0000FF00) << 8)  | \
	 (((A) & 0x000000FF) << 24))	

PcapWriter::PcapWriter(const char *filename, int snapLen) :
	m_pFh(NULL),
	m_SnapLen(snapLen) 
{
	pcap_hdr_t pcapHdr = {0xa1b2c3d4, 2, 4, 0, 0, m_SnapLen, 1};

	m_pFh = fopen(filename, "wb");	
	if (!m_pFh)
		FATAL("Opening file %s failed", filename);

	if (fwrite(&pcapHdr, 1, sizeof(pcapHdr), m_pFh) != sizeof(pcapHdr))
		FATAL("fwrite failed: errno %d", errno);
}

PcapWriter::~PcapWriter() {
	if (m_pFh)
		fclose(m_pFh);
}

void PcapWriter::WritePacket(const void *data, int data_length) {
	int ret;
	struct timeval tv;
	pcaprec_hdr_t recHdr;

	ret = gettimeofday(&tv, NULL); 
	if (ret < 0)
		FATAL("gettimeofday failed: errno %d", errno);

	recHdr.ts_sec = tv.tv_sec;
	recHdr.ts_usec = tv.tv_usec;
	recHdr.incl_len = (data_length <= m_SnapLen? data_length: m_SnapLen);
	recHdr.orig_len = data_length;

	ret = fwrite(&recHdr, 1, sizeof(recHdr), m_pFh);
	if (ret != sizeof(recHdr))
		FATAL("fwrite failed: errno %d", errno);

	ret = fwrite(data, 1, recHdr.incl_len, m_pFh);
	if (ret != (int)recHdr.incl_len)
		FATAL("fwrite failed: errno %d", errno);
}

PcapReader::PcapReader(const char *filename) :
	m_pFh(NULL),
	m_SwapBytes(false) 
{
	pcap_hdr_t pcapHdr;

	m_pFh = fopen(filename, "rb");	
	if (!m_pFh)
		FATAL("Opening file %s failed", filename);
	
	if (fread(&pcapHdr, 1, sizeof(pcapHdr), m_pFh) != sizeof(pcapHdr))
		FATAL("fread failed: errno %d", errno);
	
	if (pcapHdr.magic_number == 0xa1b2c3d4) {
		m_SnapLen = pcapHdr.snaplen;
		m_SwapBytes = false;
	} else if (pcapHdr.magic_number == 0xd4c3b2a1) {
		m_SnapLen = SWAP32(pcapHdr.snaplen);
		m_SwapBytes = true;
	} else {
		FATAL("Invalid magic in PCAP file: 0x%x", pcapHdr.magic_number);
	}
}

PcapReader::~PcapReader() {
	if (m_pFh)
		fclose(m_pFh);
}

int PcapReader::ReadPacket(void *data, int buffer_length, int &actual_len, uint64_t &timestamp_us) {
	pcaprec_hdr_t recHdr;

	if (buffer_length < m_SnapLen) {
		ERROR("Too small buffer");
		return -1;
	}

	if (fread(&recHdr, 1, sizeof(recHdr), m_pFh) != sizeof(recHdr))  {
		ERROR("fread failed: errno %d", errno);
		return -1;
	}

	if (m_SwapBytes) {
		recHdr.ts_sec = SWAP32(recHdr.ts_sec);
		recHdr.ts_usec = SWAP32(recHdr.ts_usec);
		recHdr.incl_len = SWAP32(recHdr.incl_len);
		recHdr.orig_len = SWAP32(recHdr.orig_len);
	}
	
	actual_len = recHdr.incl_len;
	timestamp_us = recHdr.ts_sec * 1000000 + recHdr.ts_usec;
	if (fread(data, 1, recHdr.incl_len, m_pFh) != recHdr.incl_len)  {
		ERROR("fread failed: errno %d", errno);
		return -1;
	}

	return 0;
}
