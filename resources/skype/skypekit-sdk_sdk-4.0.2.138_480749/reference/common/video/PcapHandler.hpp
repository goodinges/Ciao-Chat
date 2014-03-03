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

#ifndef PCAPHANDLER_HPP_
#define PCAPHANDLER_HPP_

#include <stdio.h>
#include <stdint.h>

class PcapWriter  {
public:
	PcapWriter(const char *filename, int snapLen);
	~PcapWriter();
	void WritePacket(const void *data, int data_length);
	FILE *m_pFh;
	int m_SnapLen;
};

class PcapReader  {
public:
	PcapReader(const char *filename);
	~PcapReader();
	int GetSnapLen() {return m_SnapLen;}
	int ReadPacket(void *data, int buffer_length, int &actual_len, uint64_t &timestamp_us);
	FILE *m_pFh;
	int m_SnapLen;
	bool m_SwapBytes;
};


#endif /* PCAP_HPP_ */
