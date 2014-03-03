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

#ifndef VIDEOCONFERENCE_H_
#define VIDEOCONFERENCE_H_

#include <skype-thread.h>

#include <media/stagefright/MediaSource.h>
#include <media/stagefright/OMXClient.h>
#include <media/stagefright/MediaBuffer.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaExtractor.h>

#include <surfaceflinger/ISurface.h>

#include "PlayerEventQueue.h"

// interface for the render component
struct vcRenderer : public android::RefBase {
	vcRenderer() {}
	virtual void render(android::MediaBuffer *buffer) = 0;
	NOCOPY(vcRenderer);
};

/**
 * Video playback, fashioned after AwesomePlayer
 */
class VideoPlayer: public SEThread {
	android::sp<android::MediaSource> mVideoTrack;
	android::sp<android::MediaSource> mVideoSource;
	android::sp<vcRenderer> mVideoRenderer;

	int32_t mVideoWidth, mVideoHeight;

	android::sp<android::ISurface> mISurface;

	void initRenderer_l();

	mutable android::Mutex mLock;

	android::sp<android::IOMX> mIOMX;
	virtual void Run();
	bool mRunning;

public:
	VideoPlayer();
	virtual ~VideoPlayer();

	void setISurface(const android::sp<android::ISurface> &isurface);
	void setIOMX(android::sp<android::IOMX> IOMX);

	android::status_t initVideoDecoder();
	android::status_t stopVideoDecoder();
	void setVideoSource(android::sp<android::MediaSource> source);
};

#endif /* VIDEOCONFERENCE_H_ */
