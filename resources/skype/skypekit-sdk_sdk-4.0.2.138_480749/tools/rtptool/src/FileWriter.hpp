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

#ifndef FILEWRITER_HPP_
#define FILEWRITER_HPP_

#include <stdio.h>
#include "Common.hpp"

class FileWriter {
public:
	FileWriter(const char* devName, FileFormat format, int log);
	~FileWriter();

	void WritePacket(const uint8_t* data, uint32_t packetLen);

private:
	int Write(const void* ptr, size_t size, size_t count);

	FILE* m_pFh;
	FileFormat m_Format;
	int m_Log;
	uint32_t m_PacketBufLen;
};

#endif /* FILEWRITER_HPP_ */
