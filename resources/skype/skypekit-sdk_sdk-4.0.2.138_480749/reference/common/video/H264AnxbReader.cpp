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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
 #include <unistd.h>

#include "H264AnxbReader.hpp"
#include "H264AnxbToRtp.hpp"

#include "debugging.hpp"

H264AnxbReader::H264AnxbReader() :
	m_FileOffset(0),
	m_InitDone(false)
{
}

H264AnxbReader::~H264AnxbReader()
{
    if (m_InitDone) {
        Uninit();
    }
}

int H264AnxbReader::Init(const char * fileName)
{
    struct stat sb;

    DBG("H264AnxbReader, opening %s", fileName);

    m_Fd = open(fileName, O_RDONLY);
	if (m_Fd == -1)
		FATAL("File open failed: %s", fileName);

	DBG("fd: %d", m_Fd);

	if (fstat(m_Fd, &sb) == -1)
		FATAL("fstat failed");

	m_FileLen = sb.st_size;
	DBG("fileLen: %d", m_FileLen);

	m_pFileData = (uint8_t *)mmap(NULL, m_FileLen, PROT_READ, MAP_PRIVATE, m_Fd, 0);
	DBG("fileData: %p", m_pFileData);

	if (!m_pFileData)
		FATAL("mmap failed");

	m_InitDone = true;
    
    return 0;
}

int H264AnxbReader::Uninit()
{
	munmap(m_pFileData, m_FileLen);
	close(m_Fd);
	m_InitDone = false;
    
    return 0;
}

uint8_t *H264AnxbReader::GetFrame(uint32_t &len)
{
	bool gotFrame = false;
	NalHeader *nalHdr;
	uint32_t startOffset = m_FileOffset;

	if (!m_InitDone)
		FATAL("!m_InitDone");

	do {
		if (!GetAnxbUnitFromStream(m_pFileData + m_FileOffset, m_FileLen - m_FileOffset, m_AnxbUnit)) {
			if (m_FileOffset != 0) {
				INFO("H264AnxbReader: Restarting");
				Restart();
				startOffset = 0;
			}
			if (!GetAnxbUnitFromStream(m_pFileData + m_FileOffset, m_FileLen - m_FileOffset, m_AnxbUnit)) {
				FATAL("H264AnxbReader: Cant find Annexb units from file");
			}
		}
		nalHdr = (NalHeader *)m_AnxbUnit.GetNal();

		// right now we expect the file to have 1 NAL per frame, so allways read at least 1 NAL of SLICE type
		if (nalHdr->GetType() >= NALU_SLICE && nalHdr->GetType() <= NALU_SLICE_IDR) {
			gotFrame = true;
		}
		m_FileOffset += m_AnxbUnit.totalLen;
	} while (!gotFrame);

	len = m_FileOffset - startOffset;

	return m_pFileData + startOffset;
}

void H264AnxbReader::Restart() {
	m_FileOffset = 0;
}
