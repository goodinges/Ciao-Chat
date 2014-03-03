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

/**
 * Just holds the variables for camera
 * and does basic start & stop
 */
// TODO Separate resolutions for preview and capture
// TODO Set preview res according to the passed surface
// TODO Set capture res according to the negotiated params
#define MAX_FRAME_RATE_VALUES (30 + 1)

struct CameraContext {
	android::sp<android::Camera> mCamera;
	android::sp<android::ICamera> mICamera;
	android::sp<android::CameraSource> mCameraSource;
	android::CameraParameters *mCameraParams;

	CameraContext();
	~CameraContext();
	bool setupCamera(android::sp<android::Surface> previewSurface, int width, int height, int initial_framerate_fps);
	bool stopCamera();
	void setFramerate(int fps);
	bool startPreview();
	bool mInitialized;
	int mSupportedFps[MAX_FRAME_RATE_VALUES];
};
