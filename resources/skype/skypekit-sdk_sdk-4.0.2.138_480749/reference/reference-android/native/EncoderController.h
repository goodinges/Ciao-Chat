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
 * Class to wrap encoder controls not available in stock SF
 * OMX direct handle calls are in here
 */

#include <media/stagefright/CameraSource.h>

#if !defined(MODIFIED_METADATA)
namespace android {
enum {
	kKeyNodeId	= 'node',
};
} // namespace android
#endif

class EncoderController {
public:
	android::sp<android::MediaSource> mEncoder;
	android::sp<android::IOMX> mIOMX;
	android::IOMX::node_id mNode;

	EncoderController();
	bool setupEncoder(android::sp<android::IOMX> & IOMX, android::sp<android::CameraSource> cameraSource, int width, int height);
	bool stopEncoder();
	void ConfigureEncoder(android::sp<android::IOMX> & IOMX, int initialBitrate);
	bool RequestKeyFrame();
	bool SetFramerate(int fps);
	bool SetBitrate(int bitrate);
};
