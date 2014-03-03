/**
 * Copyright (C) 2011, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

#include "debugging.hpp"
#undef LOG // Collides with Android

#include "H264FileSource.hpp"
#include "rfc3984.hpp"

#define LOG_TAG "H264FileSource"
#include <utils/Log.h>
#include "utility.h"

#include <math.h>

#include <media/stagefright/MediaBufferGroup.h>
#include <media/stagefright/MediaDebug.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

namespace android {
//#define RESTART

	H264FileSource::H264FileSource(const char *fileName) :
		mStarted(false),
		mGroup(NULL),
		m_FileOffset(0),
		m_TimeScale(30000),
		m_FrameNum(0) {
		struct stat sb;

		m_Fd = open(fileName, O_RDONLY);
		if (m_Fd == -1) {
			FATAL("File open failed: %s", fileName);
		}

		if (fstat(m_Fd, &sb) == -1) {
			FATAL("fstat failed");
		}

		m_FileLen = sb.st_size;

		m_pFileData = (uint8_t *) mmap(NULL, m_FileLen, PROT_READ, MAP_PRIVATE, m_Fd, 0);

		if (!m_pFileData) {
			FATAL("mmap failed");
		}
	}

	H264FileSource::~H264FileSource() {
		if (mStarted) {
			stop();
		}

	}

	status_t H264FileSource::start(MetaData *params) {
		F_LOG;

		CHECK(!mStarted);

		mGroup = new MediaBufferGroup;
		mGroup->add_buffer(new MediaBuffer(kBufferSize));

		mStarted = true;

		return OK;
	}

	status_t H264FileSource::stop() {
		F_LOG;

		CHECK(mStarted);

		delete mGroup;
		mGroup = NULL;

		mStarted = false;

		munmap(m_pFileData, m_FileLen);

		return OK;
	}

	sp<MetaData> H264FileSource::getFormat() {
		F_LOG;

		sp<MetaData> meta = new MetaData;
		meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_AVC);
		meta->setInt32(kKeyChannelCount, 1);
		meta->setInt64(kKeyDuration, 100);
		meta->setInt32(kKeyWidth, 320);
		meta->setInt32(kKeyHeight, 240);

//		meta->setInt32(kKeySampleRate, mSampleRate);

		return meta;
	}

	bool H264FileSource::GetAnxbUnitFromStream(uint8_t *buf, uint32_t bufLen, AnxbUnit &unit) {
		uint32_t val = 0xffffffff;
		uint32_t offset = 0;

		unit.data = buf;

		while (offset < bufLen) {
			val <<= 8;
			val |= buf[offset];
			offset++;
			if ((val & 0x00ffffff) == 0x000001) {
				break;
			}
		}

		// If start code not found then report that no unit AnxbUnit found
		if (offset == bufLen) {
			return false;
		}

		unit.nalOffset = offset;

		val = 0xffffffff;
		while (offset < bufLen) {
			val <<= 8;
			val |= buf[offset];
			offset++;
			if ((val & 0x00ffffff) == 0x000001 || (val & 0x00ffffff) == 0x000000) {
				unit.totalLen = offset - 3;
				return true;
			}
		}

		// If end code was not found then the stream end will be the end of AnxbUnit
		unit.totalLen = offset;
		return true;
	}

	uint8_t *H264FileSource::GetFrame(uint32_t &len)
	{
		F_LOG;

		bool gotFrame = false;
		NalHeader *nalHdr;
		uint32_t startOffset = m_FileOffset;

		do {
			if (!GetAnxbUnitFromStream(m_pFileData + m_FileOffset, m_FileLen - m_FileOffset, m_AnxbUnit)) {
#ifdef RESTART
				if (m_FileOffset != 0) {
					LOGV("H264AnxbReader: Restarting");
					m_FileOffset = 0;
					startOffset = 0;
				}
				if (!GetAnxbUnitFromStream(m_pFileData + m_FileOffset, m_FileLen - m_FileOffset, m_AnxbUnit)) {
					FATAL("H264AnxbReader: Cant find Annexb units from file");
				}
#else
				len = 0;
				return NULL;
#endif
			}
			nalHdr = (NalHeader *)m_AnxbUnit.GetNal();

			// Right now we expect the file to have 1 NAL per frame, so allways read at least 1 NAL of SLICE type
			if (nalHdr->GetType() >= NALU_SLICE && nalHdr->GetType() <= NALU_SLICE_IDR) {
				gotFrame = true;
			}
			m_FileOffset += m_AnxbUnit.totalLen;
		} while (!gotFrame);

		len = m_FileOffset - startOffset;

		return m_pFileData + startOffset;
	}

	status_t H264FileSource::read(MediaBuffer **out, const ReadOptions *options) {
		F_LOG;
		Log("H264FileSource::read!!");
		*out = NULL;
		MediaBuffer *buffer;
		status_t err = mGroup->acquire_buffer(&buffer);

		if (err != OK) {
			return err;
		}

		uint32_t len;
		uint8_t *ptr = GetFrame(len);
		if (!ptr) {
			LOGE("read - end of file reached");
			return NOT_ENOUGH_DATA;
		}
		assert(len < buffer->size());
		LOGV("got frame, len %d", len);
		memcpy(buffer->data(), (void *)ptr, len);

		buffer->set_range(0, len);
		buffer->meta_data()->clear();
		buffer->meta_data()->setInt64(kKeyTime, m_FrameNum * 1000);
//	buffer->meta_data()->setInt32(kKeyTimeScale, m_TimeScale);

		*out = buffer;

		return OK;
	}

} // namespace android


