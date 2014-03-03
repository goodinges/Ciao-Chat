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

#ifndef H264ANXBREADER_HPP_
#define H264ANXBREADER_HPP_

#include <stdio.h>
#include <stdint.h>

#include "H264AnxbToRtp.hpp"

class H264AnxbReader {
public:
	H264AnxbReader();
	~H264AnxbReader();
	
	int Init(const char *fileName);
	int Uninit();
	void Restart();
	uint8_t *GetFrame(uint32_t &len);

private:
	int m_Fd;
	uint8_t *m_pFileData;
	uint32_t m_FileLen;
	uint32_t m_FileOffset;
	AnxbUnit m_AnxbUnit;
	bool m_InitDone;
};


#endif /* H264ANXBREADER_HPP_ */
