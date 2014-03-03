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

#define LOG_TAG "VideoPlayer"

#include <media/stagefright/OMXCodec.h>
#include <media/stagefright/MediaDebug.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MetaData.h>

#include "utils/Log.h"
#include "utility.h"
#include "VideoPlayer.h"

#include "TimedEventQueue.h"
using namespace android;

struct VideoPlayerEvent : public TimedEventQueue::Event {
	VideoPlayerEvent(VideoPlayer *player, void (VideoPlayer::*method)()) :
		mPlayer(player),mMethod(method)
	{}
protected:
	virtual ~VideoPlayerEvent(){}
	virtual void fire(TimedEventQueue *queue, int64_t /* now_us */) {
		(mPlayer->*mMethod)();
	}
private:
	VideoPlayer *mPlayer;
	void (VideoPlayer::*mMethod)();

	NOCOPY(VideoPlayerEvent);
};

//-------------------------------------------------
struct vcRemoteRenderer : public vcRenderer {
	vcRemoteRenderer(const sp<IOMXRenderer> &target)
		: mTarget(target) {
	}

	virtual void render(MediaBuffer *buffer) {
		void *id;
		if (buffer->meta_data()->findPointer(kKeyBufferID, &id)) {
			mTarget->render((IOMX::buffer_id)id);
		}
	}

private:
	sp<IOMXRenderer> mTarget;
	NOCOPY(vcRemoteRenderer);
};

//-------------------------------------------------
VideoPlayer::VideoPlayer() :
	mVideoTrack(NULL),
	mVideoSource(NULL),
	mVideoRenderer(NULL),
	mVideoWidth(-1), mVideoHeight(-1),
	mISurface(NULL),
	mIOMX(NULL)
{
	F_LOG;
}

VideoPlayer::~VideoPlayer() {
	F_LOG;
}

void VideoPlayer::setISurface(const sp<ISurface> &isurface) {
	Mutex::Autolock autoLock(mLock);

	mISurface = isurface;
}

void VideoPlayer::setIOMX(sp<IOMX> IOMX) {
	mIOMX = IOMX;
}

void VideoPlayer::initRenderer_l() {
	F_LOG;
	if (mISurface == NULL) {
		Log("render surface missing");
		return;
	}

	sp<MetaData> meta = mVideoSource->getFormat();

	int32_t format;
	const char *component;
	int32_t decodedWidth, decodedHeight;
	CHECK(meta->findInt32(kKeyColorFormat, &format));
	CHECK(meta->findCString(kKeyDecoderComponent, &component));
	CHECK(meta->findInt32(kKeyWidth, &decodedWidth));
	CHECK(meta->findInt32(kKeyHeight, &decodedHeight));
	Log("decoder '%s' w:%u h:%u color:%u", component, decodedWidth, decodedHeight, format);
	mVideoRenderer.clear();

#if defined(FROYO)
	mVideoRenderer = new vcRemoteRenderer(
		mIOMX->createRenderer(
			mISurface, component,
			(OMX_COLOR_FORMATTYPE)format,
			decodedWidth, decodedHeight,
			mVideoWidth, mVideoHeight));
#elif defined(GINGERBREAD)
	mVideoRenderer = new vcRemoteRenderer(
		mIOMX->createRenderer(
			mISurface, component,
			(OMX_COLOR_FORMATTYPE)format,
			decodedWidth, decodedHeight,
			mVideoWidth, mVideoHeight,
			0));
#endif
}

status_t VideoPlayer::initVideoDecoder() {
	F_LOG;

	CHECK(!(mVideoTrack == NULL));
	sp<MetaData> trackMeta = mVideoTrack->getFormat();
	CHECK(!(trackMeta == NULL));
	const char *mime;
	bool success = trackMeta->findCString(kKeyMIMEType, &mime);
	CHECK_EQ(success, true);
	Log("%s:OMXCodec creating..(%s)", __func__, mime);

	mVideoSource = OMXCodec::Create(
		mIOMX, mVideoTrack->getFormat(),
		false, // createEncoder
		mVideoTrack,
		NULL, 0
//		,"AVCDecoder"
		);
	Log("OMXCodec created");

	if (mVideoSource != NULL) {
		Log("VideoSource is OK");
		CHECK(mVideoTrack->getFormat()->findInt32(kKeyWidth, &mVideoWidth));
		CHECK(mVideoTrack->getFormat()->findInt32(kKeyHeight, &mVideoHeight));
		const char *component;
		CHECK(mVideoSource->getFormat()->findCString(kKeyDecoderComponent, &component) );
		Log("decoder comp: %s", component);
		status_t err = mVideoSource->start();
		if (err != OK) {
			Log("videoSource not started!");
			mVideoSource.clear();
			return err;
		}
	} else {
		Log("videosource not created");
	}

	mRunning = true;
	Start();

	return mVideoSource != NULL ? OK : UNKNOWN_ERROR;
}

android::status_t VideoPlayer::stopVideoDecoder() {
	mRunning = false;

	Log("VideoPlayer::stopVideoDecoder() mVideoSource->stop()");
	mVideoSource->stop();

	// Lmb WAR - seems like we need to wait for the thread to exit before we
	// go about blowing away the object!?!
	while (IsRunning())
		Sleep(10);

	Log("VideoPlayer::stopVideoDecoder() mVideoSource.clear()");
	mVideoSource.clear();
	Log("VideoPlayer::stopVideoDecoder() done");
	return OK;
}


void VideoPlayer::Run() {
	F_LOG;
	Log("VideoPlayer::Run()");

	while (mRunning) {
		android::MediaBuffer *videoBuffer;

		MediaSource::ReadOptions options;
//			Log("TRACE: reading from videosource");
		status_t err = mVideoSource->read(&videoBuffer, NULL /* &options */);
//			Log("TRACE: returned from read call");
		if (err != OK) {
			Log("media read failed, err=%d", err);
			CHECK_EQ(videoBuffer, NULL);

			if (err == INFO_FORMAT_CHANGED) {
				Log("VideoSource signalled format change.");
				continue;
			}
			return;
		}

		if (! videoBuffer) {
			Log("media read returned invalid buffer");
			return;
		}
		if (videoBuffer->range_length() == 0) {
			Log("media read returned null buffer");
			videoBuffer->release();
			videoBuffer = NULL;
			continue;
		}

		int64_t timeUs;
		CHECK(videoBuffer->meta_data()->findInt64(kKeyTime, &timeUs));

		if (mVideoRenderer == NULL) {
			initRenderer_l();
		}

		if (mVideoRenderer != NULL) {
			Log("Calling render");
			mVideoRenderer->render(videoBuffer);
			videoBuffer->release();
		}
		else {
			Log("VideoPlayer::Run NULL renderer!");
		}

		videoBuffer = NULL;
	}
}

void VideoPlayer::setVideoSource(sp<MediaSource> source) {
	F_LOG;
	assert(source != NULL);
	mVideoTrack = source;
}

