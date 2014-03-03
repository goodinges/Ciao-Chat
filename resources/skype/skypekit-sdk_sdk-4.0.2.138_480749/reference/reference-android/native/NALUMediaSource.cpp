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

#include <media/stagefright/MediaDefs.h> // Pull in mime type
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <media/stagefright/MediaDebug.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MetaData.h>

#include "H264RtpToAnxb.hpp"

#include "utils/Log.h"
#include "utility.h"

#include <media/IOMX.h>

using namespace android;

#include "NALUMediaSource.h"

enum {
	INIT_NAL_SIZE = 65536
};

NALUMediaSource::NALUMediaSource(int w,int h, observer &obs)	:
	mObserver(obs),
	dumpFile(SKYPEKIT_ROOT_PATH "/recv_packets.264"),
	dumpRenderFile(SKYPEKIT_ROOT_PATH "/render_packets.264"),
	mRtpToAnxb(&mAnxBCallback),
	mAnxBCallback(*this),
	mStarted(false),
	mBufferGroup(NULL),
	mSrcBuffer(NULL), mSrcBufferLen(0), NALLen(0)
{
//	mFh = fopen(dumpFile, "wb");
//	mFRendered = fopen(dumpRenderFile, "wb");
	mMeta = new MetaData();
	mMeta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_VIDEO_AVC);
	mMeta->setInt32(kKeyWidth, w);
	mMeta->setInt32(kKeyHeight, h);
}

NALUMediaSource::~NALUMediaSource() {
}

void NALUMediaSource::addNAL(uint8_t *data, uint32_t len) {
	F_LOG;
	Mutex::Autolock autoLock(mLock);
	while(NALLen) {
		Log("addNAL:waiting for buffer to be emptied");
		mSignalBuffer.wait(mLock);
	}

	if (len > mSrcBufferLen) {
		Log("addNAL:resizing buffer (%u->%u)", mSrcBufferLen, len);
		mSrcBuffer = (uint8_t *) realloc(mSrcBuffer, len);
		CHECK(!(mSrcBuffer == NULL));
		mSrcBufferLen = len;
	}

	memcpy(mSrcBuffer, data, len);
	NALLen = len;
	// Free waiting readers
	mSignalBuffer.signal();
}

void NALUMediaSource::SubmitPacket(const uint8_t *data, uint32_t len) {
	F_LOG;
#ifdef SKYPEKIT31
	mRtpToAnxb.RtpToAnxbFrame((uint8_t *)data, len);
#else
	mRtpToAnxb.RtpToAnxbFrame((const uint8_t *)data, len);
#endif
}

#ifdef SKYPEKIT31
void NALUMediaSource::AnxBCallback::TransmitAnxbFrame(uint8_t *data, uint32_t len) {
	F_LOG;
	ref.addNAL(data, len);
}
#else
void NALUMediaSource::AnxBCallback::TransmitAnxbFrame(const uint8_t *data, uint32_t len, int timestamp) {
	F_LOG;
	ref.addNAL((uint8_t *)data, len);
}
#endif

status_t NALUMediaSource::start(MetaData * params ) {
	F_LOG;
	CHECK(!mStarted);
	int32_t val;
	if (params && params->findInt32(kKeyWantsNALFragments, &val) && val != 0) {
		Log("mWantsNALFragments = true");
	} else {
		Log("mWantsNALFragments = false");
	}
	mSrcBufferLen = INIT_NAL_SIZE;
	mSrcBuffer = (uint8_t *) malloc(mSrcBufferLen);

	mBufferGroup = new MediaBufferGroup();
	mBufferGroup->add_buffer(new MediaBuffer(mSrcBufferLen));

	mStarted = true;
	return OK;
}

status_t NALUMediaSource::stop() {
	F_LOG;
	CHECK(mStarted);

	if (mSrcBuffer) {
		free(mSrcBuffer);
		mSrcBuffer = NULL;
		mSrcBufferLen = 0;
	}

	delete mBufferGroup;
	mBufferGroup = NULL;

	mStarted = false;

	// Free waiting readers
	mSignalBuffer.signal();

	return OK;
}

sp<MetaData> NALUMediaSource::getFormat() {
	F_LOG;
	return mMeta;
}

status_t NALUMediaSource::read(MediaBuffer **out, const ReadOptions * options) {
	F_LOG;
	*out = NULL;

	if (!mStarted) {
		return NOT_ENOUGH_DATA;
	}

	Mutex::Autolock autoLock(mLock);
	while(NALLen == 0) {
		Log("read:waiting for source data");
		mSignalBuffer.wait(mLock);

		// If we were freed because we stopped, then exit
		if (!mStarted) {
			return NOT_ENOUGH_DATA;
		}
	}

	MediaBuffer *buffer;
//	Log("NALUMediaSource::waiting for buffer");
	status_t err = mBufferGroup->acquire_buffer(&buffer);
	if (err != OK) {
		Log("read:err=%u", err);
		return err;
	}
	CHECK(!(buffer == NULL));
//	Log("NALUMediaSource::got a buffer");

	if (NALLen > buffer->size()) {
		Log("read:resizing buffer (%u->%u)", buffer->size(), NALLen);
		while (buffer->refcount()) {
			buffer->release();
		}
		delete mBufferGroup;
		mBufferGroup = new MediaBufferGroup();
		mBufferGroup->add_buffer(new MediaBuffer(NALLen));

		err = mBufferGroup->acquire_buffer(&buffer);
		if (err != OK) {
			Log("read:err=%u", err);
			return err;
		}
		CHECK(!(buffer == NULL));
	}

	memcpy(buffer->data(), mSrcBuffer, NALLen);
	buffer->set_range(0, NALLen);
	NALLen = 0;
	buffer->meta_data()->clear();
	buffer->meta_data()->setInt64(kKeyTime, 0);
	*out = buffer;
	// Free waiting readers
	mSignalBuffer.signal();

	return OK;
}
