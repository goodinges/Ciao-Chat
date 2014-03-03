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

#ifndef VIDEOHOSTMAIN_H_
#define VIDEOHOSTMAIN_H_

#include "SidVideoRTPInterface.hpp"

#include "skype-thread.h"

#include <surfaceflinger/Surface.h>

class CameraContext;
class ConferenceContext;

/**
 * Sets up the main IPC host
 */
class VideohostMain : public SEThread {
	struct pData;
	pData * data;

	android::sp<android::Surface> videoSurface;
	android::sp<android::Surface> previewSurface;
	CameraContext *cameraContext;
	ConferenceContext *conferenceContext;
public:
	enum runMode {
		NORMAL = 0,
		TEST_FILEPLAYBACK =1,
		TEST_AUDIOPLAYBACK = 2,
		TEST_AUDIORECORD = 3
	};
	VideohostMain();
	virtual ~VideohostMain();
	bool Init(runMode mode = NORMAL);
	virtual void Run();

	bool setVideoSurface(android::sp<android::Surface> surface);
	bool setPreviewSurface(android::sp<android::Surface> surface);
private:
	runMode mMode;
};

#endif /* VIDEOHOSTMAIN_H_ */
