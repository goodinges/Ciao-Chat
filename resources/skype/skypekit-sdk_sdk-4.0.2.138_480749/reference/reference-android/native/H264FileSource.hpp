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

#ifndef H264FileSource_HPP_
#define H264FileSource_HPP_

#include <media/stagefright/MediaSource.h>

#include <debugging.hpp>
#include <H264AnxbToRtp.hpp>

namespace android {

	struct MediaBufferGroup;

	struct H264FileSource : public MediaSource {
		H264FileSource(const char *filename);

		virtual status_t start(MetaData *params);
		virtual status_t stop();

		virtual sp<MetaData> getFormat();

		virtual status_t read(
			MediaBuffer **out, const ReadOptions *options = NULL);

	protected:
		virtual ~H264FileSource();

	private:
		bool GetAnxbUnitFromStream(uint8_t *buf, uint32_t bufLen, AnxbUnit &unit);
		uint8_t *GetFrame(uint32_t &len);

		enum { kBufferSize = 400000 };
		static const double kFrequency = 500.0;

		bool mStarted;

		MediaBufferGroup *mGroup;

		int m_Fd;
		uint8_t *m_pFileData;
		uint32_t m_FileLen;
		uint32_t m_FileOffset;
		AnxbUnit m_AnxbUnit;
		int32_t m_TimeScale;
		int32_t m_FrameNum;
	};

}

#endif /* H264FileSource_HPP_ */
