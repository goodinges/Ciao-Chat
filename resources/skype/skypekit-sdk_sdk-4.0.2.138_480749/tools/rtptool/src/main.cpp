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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include "Common.hpp"
#include "RTPTool.hpp"

RTPTool *m_pRTPTool;

void more(FILE *fp) {
	fprintf(fp,
"Try `%s -h' for more information.\n", RTPTOOL);
}

void about() {
	fprintf(stdout,
"Usage: %s [OPTION...] FILE\n"
"List contents of RTP headers contained in FILE. Optionally extract audio or\n"
"video stream contents, and convert between various container formats.\n"
"Copyright (C) 2010-2011 Skype Technologies S.A.\n"
"\n"
		"", RTPTOOL);
}

void usage() {
	fprintf(stdout,
"Options:\n"
"  -o <FILE>         write output to FILE\n"
"  -a                force 'audio' input media type\n"
"  -v                force 'video' input media type\n"
"  -r <FORMAT>       force input container format (see '-f' for FORMATs)\n"
"  -w <FORMAT>       force output container format (see '-f' for FORMATs)\n"
"  -s                silent (no logging)\n"
"  -h                display this information\n"
"  -f                further help on supported file formats\n"
"\n"
		"");
	fprintf(stdout,
"Examples:\n"
"  %s video.pcap                 # Print RTP headers of 'pcap' video stream\n"
"  %s audio.dat                  # Print RTP headers of 'dat' audio stream\n"
"  %s video.win -o video.h264 -s # Extract video stream to 'video.h264'\n"
"\n"
		"", RTPTOOL, RTPTOOL, RTPTOOL);
}

void formats() {
	fprintf(stdout,
"About file formats:\n"
"Input FILE is expected to contain an RTP stream in RFC 3550 format (with the\n"
"exception of win format files). Media type (audio or video) is automatically\n"
"detected from payload type, while container format is determined by the file\n"
"extension. Type and format can also be forced through options, both for input\n"
"and output files. Media types and container formats are described further below.\n"
"For details on RTP packetization according to RFC 3550 refer to:\n"
"  http://tools.ietf.org/rfc/rfc3550.txt\n"
"\n"
"Media types:\n"
"'audio'\n"
"Audio RTP stream in RFC 3551 format. When %s tries to determine the media\n"
"type of a file, this will automatically be set to 'audio' if the payload type is\n"
"anything other than 96 (see below for an explanation on this).\n"
"For details refer to:\n"
"  http://tools.ietf.org/rfc/rfc3551.txt\n"
"\n"
"'video'\n"
"Video RTP stream in RFC 3984 format. When %s tries to determine the media\n"
"type of a file, this will automatically be set to 'video' if the payload type is\n"
"equal to 96, as this is the default video payload type used by SkypeKit.\n"
"For details refer to:\n"
"  http://tools.ietf.org/rfc/rfc3984.txt\n"
"\n"
		"", RTPTOOL, RTPTOOL);
	fprintf(stdout,
"Container formats:\n"
"FORMAT=raw (file extensions automatically detected: raw, h264, 264)\n"
"  Raw audio or video stream without RTP headers (output format only).\n"
"\n"
"FORMAT=pcap (file extension automatically detected: pcap)\n"
"  For both audio and video files. Also known as the libpcap file format. Used by\n"
"  videortphost through options DumpSendRtpPackets/DumpReceiveRtpPackets and\n"
"  SendPacketsFromPcapFile to dump and send video streams to and from file,\n"
"  respectively. Used by tcpdump, WinDump, TShark, WireShark and similar\n"
"  networking tools. Also refer to the documentation for libpcap.\n"
"  As shown below, a pcap file has a global header followed by zero or more\n"
"  captured packets, which consist of a packet header and the packet data.\n"
"\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    | Global Header | Packet Header | Packet Data | Packet Header ...\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"\n"
"    Example pcap file.\n"
"\n"
		"");
	fprintf(stdout,
"  Byte ordering (endianness) is determined by by the magic number field in the\n"
"  global header. Only the magic number and max length fields are used from the\n"
"  global header.\n"
"\n"
"    0                   1                   2                   3\n"
"    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                          magic number                         |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |     major version number      |     minor version number      |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                    GMT to local correction                    |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                    accuracy of timestamps                     |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |           max length of captured packets, in octets           |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                        data link type                         |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"\n"
"    Structure of pcap format Global Header.\n"
"\n"
"  All fields are used in the packet header. When writing to a pcap file, the\n"
"  timestamp is set to the current time from gettimeofday(), to simulate actual\n"
"  timestamps. This also means pcap files generated separately, but from the same\n"
"  source data, will not be identical (in contrast to the other formats).\n"
"\n"
		"");
	fprintf(stdout,
"    0                   1                   2                   3\n"
"    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                      timestamp seconds                        |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                    timestamp microseconds                     |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |           number of octets of packet saved in file            |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                    actual length of packet                    |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                                                               |\n"
"    |                          Packet Data                          |\n"
"    |                                                               |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"\n"
"    Structure of pcap format Packet Header, followed by Packet Data.\n"
"\n"
		"");
	fprintf(stdout,
"FORMAT=dat (file extension automatically detected: dat)\n"
"  For both audio and video files. Length is in big-endian format (network byte\n"
"  order).\n"
"\n"
"    0                   1                   2                   3\n"
"    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |            Length             |                               |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +\n"
"    |                                                               |\n"
"    |                             Data                              |\n"
"    |                                                               |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"\n"
"    Structure of the dat format.\n"
"\n"
		"");
	fprintf(stdout,
"FORMAT=bit (file extension automatically detected: bit)\n"
"  SILK audio bitstream without RTP headers (output format only). Length is in\n"
"  little-endian format. For use with SILK SDK.\n"
"\n"
"    0                   1                   2                   3\n"
"    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |            Length             |                               |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +\n"
"    |                                                               |\n"
"    |                             Data                              |\n"
"    |                                                               |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"\n"
"    Structure of the bit format.\n"
"\n"
		"");
	fprintf(stdout,
"FORMAT=win (file extension automatically detected: win)\n"
"  For video files saved with the SaveEncoderOutput option on Skype for Windows.\n"
"  Length is in little-endian format. RFC 3550 RTP headers are not present.\n"
"  Format uses RFC 3984 STAP-A NAL headers, FU indicators, and FU headers only.\n"
"\n"
"    0                   1                   2                   3\n"
"    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                            Length                             |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                                                               |\n"
"    |                             Data                              |\n"
"    |                                                               |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"\n"
"    Structure of the win format.\n"
"\n"
		"");
	fprintf(stdout,
"FORMAT=udp (file extension automatically detected: udp)\n"
"  For both audio and video files. Length is in big-endian format (network byte\n"
"  order). Only the length field is used from the header. Also refer to RFC 768.\n"
"\n"
"     0                   1                   2                   3\n"
"     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |      Source port number       |    Destination port number    |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |            Length             |           Checksum            |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"    |                                                               |\n"
"    |                             Data                              |\n"
"    |                                                               |\n"
"    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
"\n"
"    Structure of the udp format.\n"
"\n"
		"");
}

void fail(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	more(stderr);
	exit(EXIT_FAILURE);
}

static const char opts[] = "o:avr:w:shf";

int main(int argc, char *argv[]) {
	char inName[256];
	char outName[256];
	int c = 0;
	int inMedia = 0, inSet = 0, inFormat = 0, outSet = 0, outFormat = 0, logging = 1;

	if (argc < 2) {
		fail("");
	}

	for (;;) {
		c = getopt (argc, argv, opts);

		if (-1 == c) {
			break;
		}

		switch (c) {
		case 'o':
			if (outSet) {
				fail("%s: output file already specified\n", RTPTOOL);
			}
			outSet = 1;
			strcpy(outName, optarg);
			break;

		case 'a':
			if (inMedia && (inMedia != MEDIA_FMT_AUDIO)) {
				fail("%s: input type already set to video\n", RTPTOOL);
			}
			inMedia = MEDIA_FMT_AUDIO;
			break;

		case 'v':
			if (inMedia && (inMedia != MEDIA_FMT_VIDEO)) {
				fail("%s: input type already set to audio\n", RTPTOOL);
			}
			inMedia = MEDIA_FMT_VIDEO;
			break;

		case 'r':
			if (inFormat) {
				fail("%s: input format already specified\n", RTPTOOL);
			}
			if (!strcmp(optarg, "pcap")) {
				inFormat = FILE_FMT_PCAP;
			} else if (!strcmp(optarg, "dat")) {
				inFormat = FILE_FMT_DAT;
			} else if (!strcmp(optarg, "win")) {
				inFormat = FILE_FMT_WIN;
			} else if (!strcmp(optarg, "udp")) {
				inFormat = FILE_FMT_UDP;
			} else {
				fail("%s: input format %s not supported (see '-f' for FORMATs)\n", RTPTOOL, optarg);
			}
			break;

		case 'w':
			if (outFormat) {
				fail("%s: output format already specified\n", RTPTOOL);
			}
			if (!strcmp(optarg, "raw") || !strcmp(optarg, "h264") || !strcmp(optarg, "264")) {
				outFormat = FILE_FMT_RAW;
			} else if (!strcmp(optarg, "pcap")) {
				outFormat = FILE_FMT_PCAP;
			} else if (!strcmp(optarg, "dat")) {
				outFormat = FILE_FMT_DAT;
			} else if (!strcmp(optarg, "bit")) {
				outFormat = FILE_FMT_BIT;
			} else if (!strcmp(optarg, "win")) {
				outFormat = FILE_FMT_WIN;
			} else if (!strcmp(optarg, "udp")) {
				outFormat = FILE_FMT_UDP;
			} else {
				fail("%s: output format %s not supported (see '-f' for FORMATs)\n", RTPTOOL, optarg);
			}
			break;

		case 's':
			logging = 0;
			break;

		case 'h':
			about();
			usage();
			exit(EXIT_SUCCESS);

		case 'f':
			about();
			formats();
			exit(EXIT_SUCCESS);

		case '?':
		default:
			more(stdout);
			exit(EXIT_FAILURE);
		}
	}

	for (; optind < argc; optind++) {
		// Input file name 'FILE'
		if (inSet) {
			fail("%s: input file '%s' already specified\n", RTPTOOL, inName);
		}
		if (!access(argv[optind], R_OK)) {
			inSet = 1;
			strcpy(inName, argv[optind]);
		} else {
			fail("Cannot open '%s': %s\n", RTPTOOL, argv[optind], strerror(errno));
		}
	}

	if (inSet && !inFormat) {
		// Automatically determine input file format
		char *suffix = strrchr(inName, '.');
		if (suffix != NULL) {
			suffix++;

			if (!strcmp(suffix, "pcap")) {
				inFormat = FILE_FMT_PCAP;
			} else if (!strcmp(suffix, "dat")) {
				inFormat = FILE_FMT_DAT;
			} else if (!strcmp(suffix, "win")) {
				inFormat = FILE_FMT_WIN;
			} else if (!strcmp(suffix, "udp")) {
				inFormat = FILE_FMT_UDP;
			}
		}
	}

	if (outSet && !outFormat) {
		// Automatically determine output file format
		char *suffix = strrchr(outName, '.');
		if (suffix != NULL) {
			suffix++;

			if (!strcmp(suffix, "raw") || !strcmp(suffix, "h264") || !strcmp(suffix, "264")) {
				outFormat = FILE_FMT_RAW;
			} else if (!strcmp(suffix, "pcap")) {
				outFormat = FILE_FMT_PCAP;
			} else if (!strcmp(suffix, "dat")) {
				outFormat = FILE_FMT_DAT;
			} else if (!strcmp(suffix, "bit")) {
				outFormat = FILE_FMT_BIT;
			} else if (!strcmp(suffix, "win")) {
				outFormat = FILE_FMT_WIN;
			} else if (!strcmp(suffix, "udp")) {
				outFormat = FILE_FMT_UDP;
			}
		}
	}

	if (!inSet) {
		// Input file name not specified
		fail("%s: input file name required\n", RTPTOOL);
	}
	if (!inFormat) {
		// Input file format unknown
		fail("%s: unable to determine input format (force with -r <FORMAT>)\n", RTPTOOL);
	}
	if (!outSet && outFormat) {
		// Output file format forced, but no file name provided
		fail("%s: output file requried if output format specified (-o <FILE>)\n", RTPTOOL);
	}
	if (outSet && !outFormat) {
		// Output file name specified, but format unknown
		fail("%s: unable to determine output format (force with -w <FORMAT>)\n", RTPTOOL);
	}
	if (outSet && (FILE_FMT_WIN == inFormat) && (outFormat != FILE_FMT_RAW) && (FILE_FMT_WIN != outFormat)) {
		// win input format has no RFC 3550 RTP headers
		fail("%s: only raw|win output formats allowed for win input format\n", RTPTOOL);
	}
	if ((MEDIA_FMT_AUDIO == inMedia) &&
	    ((FILE_FMT_WIN == inFormat) || (FILE_FMT_WIN == outFormat))) {
		// win format not used for audio streams
		fail("%s: win format not supported for audio streams\n", RTPTOOL);
	}

	m_pRTPTool = new RTPTool(inMedia, inFormat, inName, outFormat, outSet ? outName : NULL, logging);
	if (m_pRTPTool) {
		m_pRTPTool->AnalyzeRTP();
		CLEANUP(m_pRTPTool);
	}
}
