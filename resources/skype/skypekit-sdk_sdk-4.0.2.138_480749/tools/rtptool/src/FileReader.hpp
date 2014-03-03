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

#ifndef FILEREADER_HPP_
#define FILEREADER_HPP_

#include <stdio.h>
#include <stdint.h>
#include "Common.hpp"

class FileReader {
public:
	struct FrameCallback {
	public:
		virtual void TransmitFrame(uint8_t* data, int len) = 0;
	};
	FileReader(const char* devName, FileFormat format, FrameCallback* cb, int log);
	~FileReader();

	void StartReading();
	void StopReading();

private:
	void Run(void);
	int Read(void* ptr, size_t size, size_t count);
	int ReadPacket(uint32_t& actual_len);

	FILE* m_pFh;
	FileFormat m_Format;
	FrameCallback* m_pCb;
	uint8_t* m_pPacketBuf;
	uint32_t m_PacketBufLen;
	int m_SwapBytes;
	int m_Log;
	volatile bool Shutdown;
};

#endif /* FILEREADER_HPP_ */
