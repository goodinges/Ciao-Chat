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

#include <camera/ICamera.h>
#include <camera/CameraParameters.h>
#include <media/stagefright/CameraSource.h>
#include <surfaceflinger/Surface.h>

#include "utils/Log.h"
#include "utility.h"
#include "CameraContext.h"
#include "surface-hack.h"

using namespace android;

CameraContext::CameraContext() :
	mCamera(NULL),
	mICamera(NULL),
	mCameraParams(NULL),
	mInitialized(false)
{
}

CameraContext::~CameraContext() {
}

bool CameraContext::startPreview() {
	F_LOG;
	mCamera->startPreview();
	return true;
}

bool CameraContext::setupCamera(sp<Surface> previewSurface, int width, int height, int initial_framerate_fps) {
	F_LOG;
#if defined(FROYO)
	mCamera = android::Camera::connect();
#elif defined(GINGERBREAD)
	mCamera = android::Camera::connect(0);
#endif
	android::String8 s = mCamera->getParameters();
	mCameraParams = new android::CameraParameters(s);
	Log("Getting camera parameters");
	char buf[50];
	sprintf(buf, "%ux%u", width, height);
	mCameraParams->set("video-size", buf);
	mCameraParams->setPreviewSize(width, height);
	Log("Setting camera params (%s)", buf);
	mCamera->setParameters(mCameraParams->flatten());

#if defined(FROYO)
	mICamera = mCamera->remote();
	mCameraSource = android::CameraSource::CreateFromICamera(mICamera);

	sp<ISurface> mISurface;
	android::Test::getISurface(previewSurface, mISurface);
	mCameraSource->setPreviewSurface(mISurface);
#elif defined(GINGERBREAD)
	mCameraSource = android::CameraSource::CreateFromCamera(mCamera);
#endif

	Log("Setting preview");
	mCamera->setPreviewDisplay(previewSurface);

	// Get supported preview frame rates from camera driver
	memset(mSupportedFps, 0, sizeof(mSupportedFps));
	const char *fpsValues = mCameraParams->get(CameraParameters::KEY_SUPPORTED_PREVIEW_FRAME_RATES);
	Log("Supported camera preview framerates: %s", fpsValues);
	char *tokenString = new char[strlen(fpsValues)];
	strcpy(tokenString, fpsValues);
	char *fpsToken;
	fpsToken = strtok(tokenString, ",");
	while (fpsToken != NULL) {
		if (atoi(fpsToken)< MAX_FRAME_RATE_VALUES) {
			mSupportedFps[atoi(fpsToken)] = 1;
		}
		fpsToken = strtok(NULL, ",");
	}

	mInitialized = true;
	setFramerate(initial_framerate_fps);

	return true;
}

bool CameraContext::stopCamera() {
	F_LOG;
	if (mCamera == NULL) {
		return false;
	}
	mCamera->stopPreview();
	if ( mCamera.get() != NULL ) {
		if (mCamera->previewEnabled()) {
			mCamera->stopPreview();
		}
	}
	mCamera.clear();
	mCamera = NULL;
	return true;
}

void CameraContext::setFramerate(int fps) {
	F_LOG;
	if (!mInitialized) {
		return;
	}

	while ((fps > 0) && !mSupportedFps[fps]) {
		fps--;
	}
	if (!fps) {
		while ((fps < MAX_FRAME_RATE_VALUES) && !mSupportedFps[fps]) {
			fps++;
		}
	}

	if (mSupportedFps[fps]) {
		Log("Actual set preview framerate: %d", fps);
		mCameraParams->setPreviewFrameRate(fps);
	} else {
		Log("Preview framerate not set");
	}
}
