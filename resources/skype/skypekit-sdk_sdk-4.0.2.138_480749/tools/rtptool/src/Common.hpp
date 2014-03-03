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

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <stdint.h>

#define RTPTOOL "rtptool"

#define CLEANUP(ptr) if (ptr) {delete ptr; ptr = NULL;}

#define SWAP32(A) \
	((((A) & 0xFF000000) >> 24) | \
	 (((A) & 0x00FF0000) >> 8)  | \
	 (((A) & 0x0000FF00) << 8)  | \
	 (((A) & 0x000000FF) << 24))

enum MediaFormat {
	MEDIA_FMT_NONE,
	MEDIA_FMT_AUDIO,
	MEDIA_FMT_VIDEO
};

enum FileFormat {
	FILE_FMT_NONE,
	FILE_FMT_RAW,
	FILE_FMT_PCAP,
	FILE_FMT_DAT,
	FILE_FMT_BIT,
	FILE_FMT_WIN,
	FILE_FMT_UDP
};

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

#endif /* COMMON_HPP_ */
