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

#ifndef AUDIOTEST_H_
#define AUDIOTEST_H_

#include <media/AudioRecord.h>
#include <media/AudioTrack.h>
#include <media/stagefright/DataSource.h>
#include <utils/Timers.h>

class AudioTest {
	const char * m_playFile;
	const char * m_recordFile;
	uint16_t m_playbackRate;
	uint16_t m_recordRate;
	android::AudioTrack *audioTrack;

	bool mValidFormat;
	uint16_t mBitsPerSample;
	uint16_t mNumChannels;
	uint32_t mSampleRate;
	off_t mDataOffset;
	size_t mDataSize;
	off_t mCurrentPos;

	uint32_t mCounter;

	mutable android::Mutex mLock;
	mutable android::Condition mSignalFinished;
	android::DurationTimer timer;

	android::DataSource * mDataSource;
	int initWav();
	FILE *m_f;

	android::AudioRecord *audioRecord;

	inline static void c_playbackCallback(int event, void* user, void *info);
	void playbackCallback(int event, void *info);

public:
	AudioTest();
	virtual ~AudioTest();
	void setPlayFile(const char *file, uint16_t rate) {m_playFile = file; m_playbackRate = rate;}
	void setRecordFile(const char *file, uint16_t rate) {m_recordFile = file; m_recordRate = rate;}
	void playTest();
	void recordTest();
};

#endif /* AUDIOTEST_H_ */
