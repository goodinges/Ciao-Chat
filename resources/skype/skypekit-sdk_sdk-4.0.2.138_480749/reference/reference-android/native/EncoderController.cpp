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

#define LOG_NDEBUG 0
#define LOG_TAG "EncoderController"

#include <media/stagefright/OMXCodec.h>
#include <media/stagefright/MediaDebug.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MetaData.h>

#include "utils/Log.h"
#include "utility.h"
#include "EncoderController.h"

#include "OMX_Image.h"
#if defined(NV_OMX)
// NVIDIA Tegra: OpenMAX extensions
#include "NVOMX_CameraExtensions.h"
#endif

using namespace android;

enum {
	PORT_OUTPUT = 1 // Safe bet?
};

template <typename T>
void INIT_PARAM(T &p, OMX_U32 portIndex) {
	memset(&p, 0, sizeof(T));
	p.nSize = sizeof(T);
	p.nVersion.s.nVersionMajor = 1;
	p.nPortIndex = portIndex;
}

EncoderController::EncoderController() :
	mEncoder(NULL),
	mIOMX(NULL),
	mNode(0) {
}

bool EncoderController::setupEncoder(android::sp<android::IOMX> & IOMX, android::sp<android::CameraSource> cameraSource, int width, int height) {
	F_LOG;
	sp<android::MetaData> encoderMetaData = new android::MetaData;
	encoderMetaData->setCString(kKeyMIMEType, android::MEDIA_MIMETYPE_VIDEO_AVC);
	encoderMetaData->setInt32(kKeyWidth, width);
	encoderMetaData->setInt32(kKeyHeight, height);
    encoderMetaData->setInt32(kKeyBitRate, 8000000); // bps
    encoderMetaData->setInt32(kKeySampleRate, 15); // video frame rate
	encoderMetaData->setInt32(kKeyIFramesInterval, 1);

	// Get stride, slice height and color format from camera
    CHECK(cameraSource != 0);
    sp<MetaData> camMeta = cameraSource->getFormat();
    CHECK(camMeta != 0);
    int32_t stride, sliceHeight, colorFormat;
    CHECK(camMeta->findInt32(kKeyStride, &stride));
    CHECK(camMeta->findInt32(kKeySliceHeight, &sliceHeight));
    CHECK(camMeta->findInt32(kKeyColorFormat, &colorFormat));
    encoderMetaData->setInt32(kKeyStride, stride);
    encoderMetaData->setInt32(kKeySliceHeight, sliceHeight);
    encoderMetaData->setInt32(kKeyColorFormat, colorFormat);

	Log("Creating encoder (%ux%u)", width, height);
	mEncoder = android::OMXCodec::Create(IOMX, encoderMetaData, true, cameraSource);
#ifdef MODIFIED_STAGEFRIGHT
	// NVIDIA Tegra: Temporary workaround
	mNode = mEncoder->getNodeId();
#else
	if (!(mEncoder->getFormat()->findInt32(kKeyNodeId, (int32_t*) &mNode))) {
		Log("Failed to retrieve Encoder Node ID");
		return false;
	}
	Log("Retrieved Encoder Node ID: %d", (status_t) mNode);
#endif
	return true;
}

bool EncoderController::stopEncoder() {
	F_LOG;
	if (mEncoder == NULL) {
		return false;
	}
	mEncoder.clear();
	mEncoder = NULL;
	return true;
}

void EncoderController::ConfigureEncoder(android::sp<android::IOMX> & IOMX, int initialBitrate) {
	mIOMX = IOMX;

	OMX_VIDEO_PARAM_PORTFORMATTYPE format;
	INIT_PARAM(format, PORT_OUTPUT);

	// Quick sanity check
	status_t err = mIOMX->getParameter(mNode, OMX_IndexParamVideoPortFormat, &format, sizeof(format));
	CHECK_EQ(err, OK);

	Log("portIndex: %ld, index: %ld, eCompressionFormat=%d eColorFormat=%d",
			format.nPortIndex, format.nIndex, format.eCompressionFormat, format.eColorFormat);

	OMX_VIDEO_PARAM_AVCTYPE h264type; // AVC params
	INIT_PARAM(h264type, PORT_OUTPUT);
	err = mIOMX->getParameter(mNode, OMX_IndexParamVideoAvc, &h264type, sizeof(h264type));
	CHECK_EQ(err, OK);
	Log("AVParam:\n\t"
	    "eProfile=%u\n\t"
	    "eLevel=%u\n\t"
	    "bEnableFMO=%u\n\t"
	    ,h264type.eProfile
	    ,h264type.eLevel
	    ,h264type.bEnableFMO);
	h264type.eProfile = OMX_VIDEO_AVCProfileBaseline;
	err = mIOMX->setParameter(mNode, OMX_IndexParamVideoAvc, &h264type, sizeof(h264type));
	Log("SetParams result=%u", err);

#if defined(NV_OMX)
	// NVIDIA Tegra: OpenMAX extension to fine tune video encoder configuration
	OMX_INDEXTYPE eIndexExt;
	err = mIOMX->getExtensionIndex(mNode, NVX_INDEX_PARAM_VIDEO_ENCODE_PROPERTY, &eIndexExt);
	CHECK_EQ(err, OK);
	NVX_PARAM_VIDENCPROPERTY encProp;
	INIT_PARAM(encProp, PORT_OUTPUT);
	encProp.eApplicationType = NVX_VIDEO_Application_VideoTelephony; // Timestamps set for telephony
	encProp.eErrorResiliencyLevel = NVX_VIDEO_ErrorResiliency_None; // Error Resiliency Level: None
	encProp.bSvcEncodeEnable = OMX_FALSE; // Disable H.264 Scalable Video Codec mode
	err = mIOMX->setParameter(mNode, eIndexExt, &encProp, sizeof(encProp));
	if (OK != err) {
		Log("VIDENCPROPERTY ERROR: %d ( %x )", err, err);
	}
#endif

	OMX_IMAGE_PARAM_QFACTORTYPE qFactor; // Q Factor for JPEG compression
	INIT_PARAM(qFactor, PORT_OUTPUT);
	// Quality level high
	qFactor.nQFactor = 80; // JPEG Q factor value in the range of 1-100.
	err = mIOMX->setParameter(mNode, OMX_IndexParamQFactor, &qFactor, sizeof(qFactor));
	if (OK != err) {
		Log("VIDENC QFACTOR ERROR: %d ( %x )", err, err);
	}

	OMX_VIDEO_PARAM_BITRATETYPE bitrateParm;
	INIT_PARAM(bitrateParm, PORT_OUTPUT);
	bitrateParm.eControlRate = OMX_Video_ControlRateConstant;
	bitrateParm.nTargetBitrate = initialBitrate; // Target average bitrate to be generated in bps
	err = mIOMX->setParameter(mNode, OMX_IndexParamVideoBitrate, &bitrateParm, sizeof(bitrateParm));
	if (OK != err) {
		Log("OMX_VIDEO_PARAM_BITRATETYPE ERROR: %d ( %x )", err, err);
	}
}

bool EncoderController::RequestKeyFrame() {
	if (mIOMX == NULL) {
		return false;
	}

	OMX_CONFIG_INTRAREFRESHVOPTYPE refresh;
	INIT_PARAM(refresh, PORT_OUTPUT);
	refresh.IntraRefreshVOP = OMX_TRUE;
	status_t err = mIOMX->setConfig(mNode, OMX_IndexConfigVideoIntraVOPRefresh, &refresh, sizeof(refresh));
	if (OK != err) {
		Log("RequestKeyFrame ERROR: %d ( %x )", err, err);
	}
	return true;
}

bool EncoderController::SetFramerate(int fps) {
	if (mIOMX == NULL) {
		return false;
	}
	status_t err;

	OMX_CONFIG_FRAMERATETYPE framerateCfg; // Defines Encoder Frame Rate setting
	INIT_PARAM(framerateCfg, PORT_OUTPUT);
	framerateCfg.xEncodeFramerate = (OMX_U32)(fps * (1 << 16)); // Encoding framerate represented in Q16 format
	err = mIOMX->setConfig(mNode, OMX_IndexConfigVideoFramerate, &framerateCfg, sizeof(framerateCfg));
	if (OK != err) {
		Log("SetFramerate ERROR: %d (%x) rate: %d (%lu)", err, err, fps, framerateCfg.xEncodeFramerate);
		return false;
	}
	else {
		Log("SetFramerate: %d (%lu)", fps, framerateCfg.xEncodeFramerate);
	}
	return true;
}

bool EncoderController::SetBitrate(int bitrate) {
	if (mIOMX == NULL) {
		return false;
	}
	status_t err;

	OMX_VIDEO_CONFIG_BITRATETYPE bitrateCfg; // Structure for dynamically configuring bitrate mode of a codec.
	INIT_PARAM(bitrateCfg, PORT_OUTPUT);
	bitrateCfg.nEncodeBitrate = bitrate; // Target average bitrate to be generated in bps
	err = mIOMX->setConfig(mNode, OMX_IndexConfigVideoBitrate, &bitrateCfg, sizeof(bitrateCfg));
	if (OK != err) {
		Log("SetBitrate ERROR: %d ( %x ) rate: %d", err, err, bitrate);
		return false;
	}
	else {
		Log("SetBitrate: %d", bitrate);
	}
	return true;
}
