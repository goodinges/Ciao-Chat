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

#define LOG_TAG "VideoRTPStageFright"

#include "debugging.hpp"
#undef LOG // Collides with Android

#include <camera/CameraParameters.h>
#include <media/stagefright/MediaBufferGroup.h>
#include <media/stagefright/MediaDebug.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/CameraSource.h>
#include <sys/endian.h>

#include "VideoRTPStageFright.h"
#include "utils/Log.h"
#include "utility.h"
#include "H264RtpToAnxb.hpp"
#include "VideoPlayer.h"
#include "CameraContext.h"
#include "EncoderController.h"

using namespace android;

#include "NALUMediaSource.h"
#include "H264AnxbToRtp.hpp"
#include "H264FileSource.hpp"

class RecorderThread : public SEThread, H264AnxbToRtp::RtpPacketCallback {
	SkypeVideoRTPInterfaceCb *mCallback;
	EncoderController *mEncoder;
	VideoRTPStageFright *mVideoRTP;
	bool stopFlag;

#ifdef SKYPEKIT31
	void TransmitRtpPacket(uint8_t *data, uint32_t len) {
		mCallback->SendRTPPacket(Sid::Binary(data, len));
#else
	void TransmitRtpPacket(const uint8_t *data, uint32_t len) {
		mCallback->SendRTPPacket(Sid::Binary((uint8_t *) data, len));
#endif
		if (!mVideoRTP->pFs) {
			Log("File not open '%s'", VIDEO_SEND);
			return;
		}
		uint16_t datHeader;
		datHeader = htons(len); // Length is in big-endian format (network byte order)
		if (fwrite(&datHeader, sizeof(uint16_t), 1, mVideoRTP->pFs) != 1) {
			Log("Failed to write header to '%s'", VIDEO_SEND);
		}
		if (fwrite(data, 1, len, mVideoRTP->pFs) != len) {
			Log("Failed to write data to '%s'", VIDEO_SEND);
		}
	}
	H264AnxbToRtp mAnxbToRtp;
public:
	RecorderThread(SkypeVideoRTPInterfaceCb * _cb, EncoderController *_ec, VideoRTPStageFright *_vr) :
		mCallback(_cb), mEncoder(_ec), mVideoRTP(_vr),
		stopFlag(false),
		mAnxbToRtp(this)
	{}

	virtual void Stop() {
		F_LOG;
		stopFlag = true;
//		SEThread::Stop(); // Doesn't work in current SkypeKit
	}
	void SetMaxPacketSize(int bytes) {
		mAnxbToRtp.SetMaxRtpPacketLen(bytes);
	}

	void Run() {
		Log("RecorderThread start");
		mEncoder->mEncoder->start();

		MediaBuffer *buffer = NULL;
		while( this->IsRunning() && !stopFlag ) {
			status_t res = mEncoder->mEncoder->read(&buffer);
			if (res != OK) {
				// TODO Decide, for now keep running
				Log("Encoder error, %u", res);
				continue;
			}
			if (buffer == NULL) {
				continue;
			}
			if (buffer != NULL && buffer->range_length() ) {
#ifdef SKYPEKIT31
				mAnxbToRtp.AnxbFrameToRtp(( uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length());
#else
				mAnxbToRtp.AnxbFrameToRtp(( const uint8_t *)buffer->data() + buffer->range_offset(), buffer->range_length(), 0, true);
#endif
			}
			buffer->release();
			buffer = NULL;
		}

		mEncoder->mEncoder->stop();
		Log("RecorderThread quit");
	}
};

struct VideoRTPStageFright::pData : public NALUMediaSource::observer {
	VideoPlayer player;
	CameraContext cameraContext;
	EncoderController encoderController;
	RecorderThread * threadRecorder;
	sp<MediaSource> mediaSource;
	sp<Surface> previewSurface;
	sp<IOMX> mIOMX;
	OMXClient omxClient;
	String playbackFile;
	void onBufferAvailable() {
		F_LOG;
//		player.triggerVideoEvent();
	}
};

VideoRTPStageFright::VideoRTPStageFright(SkypeVideoRTPInterfaceCb *callback) :
	pFs(NULL),
	pFr(NULL),
	m_Callback(callback)
{
	F_LOG;
	pdata = new pData();

	pFs = fopen(VIDEO_SEND, "wb");
	if (!pFs) {
		Log("Cannot open '%s'", VIDEO_SEND);
	}
	pFr = fopen(VIDEO_RECEIVE, "wb");
	if (!pFr) {
		Log("Cannot open '%s'", VIDEO_RECEIVE);
	}
}

VideoRTPStageFright::~VideoRTPStageFright() {
	F_LOG;
	delete pdata;

	if (pFs) {
		fclose(pFs);
		pFs = NULL;
	}
	if (pFr) {
		fclose(pFr);
		pFr = NULL;
	}
}

#ifdef SKYPEKIT31
bool VideoRTPStageFright::GetVersion(String & version) {
	F_LOG;
	return true;
}
#endif

int VideoRTPStageFright::GetLastError() {
	F_LOG;
	return 0;
}

bool VideoRTPStageFright::Init() {
	F_LOG;
	CHECK_EQ(pdata->omxClient.connect(), OK);
	pdata->mIOMX = pdata->omxClient.interface();

	pdata->player.setIOMX(pdata->mIOMX);
	return true;
}

bool VideoRTPStageFright::Uninit() {
	F_LOG;
	pdata->omxClient.disconnect();
	return true;
}

bool VideoRTPStageFright::GetCodecCount( int &codec_count ) {
	F_LOG;
	codec_count = 1;
	return true;
}

bool VideoRTPStageFright::GetCodec(int index, VideoCodec &codec ) {
	F_LOG;
	if ( index != 0 ) {
		return false;
	}
	codec.fourcc = MAKEFOURCC('H', '2', '6', '4');
	codec.recording_capable = 1;
#ifndef SKYPEKIT31
	codec.payload_format = FORMAT_RTP;
#endif
	return true;
}

#ifndef SKYPEKIT31
bool VideoRTPStageFright::GetSenderResolutionCount(int& res_cap_count) {
	F_LOG;
	res_cap_count = 4;
	return true;
}

bool VideoRTPStageFright::GetSenderResolution(int index, FixedRes& resolution) {
	F_LOG;
	switch (index) {
	case 0:
		resolution.width = 160;
		resolution.height = 120;
		resolution.preference = 1;
		break;
	case 1:
		resolution.width = 320;
		resolution.height = 240;
		resolution.preference = 2;
		break;
	case 2:
		resolution.width = 640;
		resolution.height = 480;
		resolution.preference = 3;
		break;
	case 3:
		resolution.width = 1280;
		resolution.height = 720;
		resolution.preference = 4;
		break;
	default:
		return false;
	}
	return true;
}
#endif

bool VideoRTPStageFright::GetIntProperty(int prop_id, int index, int &value) {
	F_LOG;
	int ret = false;
	if (index != 0) {
		return false;
	}

	switch (prop_id) {
	case GET_NUMBER_OF_H264_SENDER_PARAMETERS:
		value = 1;
		ret = true;
		break;
	case GET_NUMBER_OF_H264_RECEIVER_PARAMETERS:
		value = 1;
		ret = true;
		break;
	default:
		break;
	}
	return ret;
}

#ifndef SKYPEKIT31
bool VideoRTPStageFright::GetReceiverResolutionCount(int& resolution_count) {
	F_LOG;
	resolution_count = 4;
	return true;
}

bool VideoRTPStageFright::GetReceiverResolution(int index, ResCap& resolution) {
	F_LOG;
	switch (index) {
	case 0:
		resolution.min_width = 160;
		resolution.min_height = 120;
		resolution.max_width = 160;
		resolution.max_height = 120;
		resolution.preference = 1;
		break;
	case 1:
		resolution.min_width = 320;
		resolution.min_height = 240;
		resolution.max_width = 320;
		resolution.max_height = 240;
		resolution.preference = 2;
		break;
	case 2:
		resolution.min_width = 640;
		resolution.min_height = 480;
		resolution.max_width = 640;
		resolution.max_height = 480;
		resolution.preference = 3;
		break;
	case 3:
		resolution.min_width = 1280;
		resolution.min_height = 720;
		resolution.max_width = 1280;
		resolution.max_height = 720;
		resolution.preference = 4;
		break;
	default:
		return false;
	}
	return true;
}
#endif

bool VideoRTPStageFright::SetIntProperty(int prop_id, int index, int value) {
	F_LOG;
	return false;
}

bool VideoRTPStageFright::GetStringProperty(int prop_id, int index, String &value) {
	F_LOG;
	return false;
}

bool VideoRTPStageFright::SetStringProperty(int prop_id, int index, const String &value) {
	F_LOG;
	return false;
}

bool VideoRTPStageFright::GetBinaryProperty(int prop_id, int index, Binary &value) {
	F_LOG;
	bool ret = false;
	switch (prop_id) {
	case GET_H264_SENDER_PARAMETERS:
	{
		value.reserve(sizeof(H264SenderParameters));
		H264SenderParameters *param = (H264SenderParameters *)value.data();
		if (index == 0) {
			memset(param, 0, sizeof(H264SenderParameters));
			param->profile_idc = 66;
#ifdef SKYPEKIT31
			param->profile_iop = 0xe0;
#else
			param->profile_iop = (H264ProfileIop)(PROFILE_IOP_CONSTRAINTS_SET0 | PROFILE_IOP_CONSTRAINTS_SET1);
#endif
			param->level_idc = 31;

#ifdef SKYPEKIT31
			param->resolution_caps[0].max_width = 160;
			param->resolution_caps[0].max_height = 120;
			param->resolution_caps[0].min_width = 160;
			param->resolution_caps[0].min_height = 120;
			param->resolution_caps[0].preference = 30;
			param->resolution_caps[1].max_width = 320;
			param->resolution_caps[1].max_height = 240;
			param->resolution_caps[1].min_width = 320;
			param->resolution_caps[1].min_height = 240;
			param->resolution_caps[1].preference = 40;
			param->resolution_caps[2].max_width = 640;
			param->resolution_caps[2].max_height = 480;
			param->resolution_caps[2].min_width = 640;
			param->resolution_caps[2].min_height = 480;
			param->resolution_caps[2].preference = 50;
			param->resolution_caps[3].max_width = 1280;
			param->resolution_caps[3].max_height = 720;
			param->resolution_caps[3].min_width = 1280;
			param->resolution_caps[3].min_height = 720;
			param->resolution_caps[3].preference = 60;
#endif
			ret = true;
		}
		break;
	}
	case GET_H264_RECEIVER_PARAMETERS:
	{
		value.reserve(sizeof(H264ReceiverParameters));
		H264ReceiverParameters *param = (H264ReceiverParameters *)value.data();
		if (index == 0) {
			memset(param, 0, sizeof(H264ReceiverParameters));
			param->profile_idc = 66;
#ifdef SKYPEKIT31
			param->profile_iop = 0xe0;
#else
			param->profile_iop = (H264ProfileIop)(PROFILE_IOP_CONSTRAINTS_SET0 | PROFILE_IOP_CONSTRAINTS_SET1);
#endif
			param->level_idc = 31;
			param->max_br = 1500;

#ifdef SKYPEKIT31
			param->resolution_caps[0].max_width = 160;
			param->resolution_caps[0].max_height = 120;
			param->resolution_caps[0].min_width = 160;
			param->resolution_caps[0].min_height = 120;
			param->resolution_caps[0].preference = 30;
			param->resolution_caps[1].max_width = 320;
			param->resolution_caps[1].max_height = 240;
			param->resolution_caps[1].min_width = 320;
			param->resolution_caps[1].min_height = 240;
			param->resolution_caps[1].preference = 40;
			param->resolution_caps[2].max_width = 640;
			param->resolution_caps[2].max_height = 480;
			param->resolution_caps[2].min_width = 640;
			param->resolution_caps[2].min_height = 480;
			param->resolution_caps[2].preference = 50;
			param->resolution_caps[3].max_width = 1280;
			param->resolution_caps[3].max_height = 720;
			param->resolution_caps[3].min_width = 1280;
			param->resolution_caps[3].min_height = 720;
			param->resolution_caps[3].preference = 60;
#endif
			ret = true;
		}
		break;
	}
	default:
		break;
	}
	return ret;
}

bool VideoRTPStageFright::SetBinaryProperty(int prop_id, int index, const Binary &value) {
	F_LOG;
	bool ret = false;
	switch (prop_id) {
	case SET_H264_SENDER_CONSTRAINTS:
	{
#ifdef SKYPEKIT31
		H264SenderConstraints *snd_constr = (H264SenderConstraints *)value.data();
#else
		H264SenderConstraints *snd_constr = (H264SenderConstraints *)((const char *)value);
#endif
		if (index != 0) {
			break;
		}
#ifdef SKYPEKIT31
		if (value.getSize() != sizeof(H264SenderConstraints)) {
#else
		if (value.size() != sizeof(H264SenderConstraints)) {
#endif
			printf("value.getSize() != sizeof(H264SenderConstraints)\n");
			break;
		}

		Log("snd_constr->profile_idc: %d", snd_constr->profile_idc);
		Log("snd_constr->profile_iop: %d", snd_constr->profile_iop);
		Log("snd_constr->level_idc: %d", snd_constr->level_idc);
		Log("snd_constr->max_mbps: %d", snd_constr->max_mbps);
		Log("snd_constr->max_fs: %d", snd_constr->max_fs);
		Log("snd_constr->max_cpb: %d", snd_constr->max_cpb);
		Log("snd_constr->max_dpb: %d", snd_constr->max_dpb);
		Log("snd_constr->max_br: %d", snd_constr->max_br);
		Log("snd_constr->max_smbps: %d", snd_constr->max_smbps);
		Log("snd_constr->redundant_pic_cap: %d", snd_constr->redundant_pic_cap);
#ifdef SKYPEKIT31
		for (int i = 0; i < MAX_RES_CAP_CNT; i++) {
			if (!snd_constr->resolution_caps[i].max_height) {
				break;
			}
			Log("snd_constr->resolution_caps[%d].max_height: %d", i, snd_constr->resolution_caps[i].max_height);
			Log("snd_constr->resolution_caps[%d].max_width: %d", i, snd_constr->resolution_caps[i].max_width);
			Log("snd_constr->resolution_caps[%d].min_height: %d", i, snd_constr->resolution_caps[i].min_height);
			Log("snd_constr->resolution_caps[%d].min_width: %d", i, snd_constr->resolution_caps[i].min_width);
			Log("snd_constr->resolution_caps[%d].preference: %d", i, snd_constr->resolution_caps[i].preference);
		}
#endif
		ret = true;
	}
	default:
		break;
	}

	if (!ret) {
		switch (prop_id) {
		case SET_H264_SENDER_CONSTRAINTS:
			printf("H264SenderConstraints struct broken!");
			break;
		default:
			printf("unknown property set");
			break;
		}
	}
	return ret;
}

bool VideoRTPStageFright::RequestKeyFrame() {
	F_LOG;
	pdata->encoderController.RequestKeyFrame();
	return true;
}

bool VideoRTPStageFright::RequestRecoveryFrame(int reserved) {
	F_LOG;
	return true;
}

bool VideoRTPStageFright::SetFramerate(int fps) {
	F_LOG;
	Log("SetFramerate: fps: %d", fps);
	pdata->cameraContext.setFramerate(fps);
	pdata->encoderController.SetFramerate(fps);
	return true;
}

bool VideoRTPStageFright::SetMaxPacketSize(int bytes) {
	F_LOG;
	if (!pdata->threadRecorder) {
		return false;
	}
	pdata->threadRecorder->SetMaxPacketSize(bytes);
	return true;
}

bool VideoRTPStageFright::SetBitrate(int bytesPerSecond) {
	F_LOG;
	Log("SetBitrate: bytesPerSecond: %d (%d kbps)", bytesPerSecond, 128 * bytesPerSecond);
	pdata->encoderController.SetBitrate(bytesPerSecond * 8);
	return true;
}

bool VideoRTPStageFright::StartPlayback(int decoderfourcc, int width, int height) {
	F_LOG;
	Log("StartPlayback: decoderfourcc: %d, width: %d, height: %d", decoderfourcc, width, height);
	if (decoderfourcc != MAKEFOURCC('H', '2', '6', '4')) {
		return false;
	}

	if (pdata->playbackFile.isEmpty()) {
		pdata->mediaSource = new NALUMediaSource(width, height, *pdata);
	} else {
		pdata->mediaSource = new H264FileSource(pdata->playbackFile);
	}

	Log("Mediasource created");
	pdata->player.setVideoSource(pdata->mediaSource);
	Log("Videosource set");
	pdata->player.initVideoDecoder();
	return true;
}

#ifdef SKYPEKIT31
void VideoRTPStageFright::SubmitRTPPacket(const Binary & inData) {
	F_LOG;
	Log("SubmitRTPPacket: size: %d", inData.getSize());
#else
void VideoRTPStageFright::RtpPacketReceived(const Sid::Binary & inData) {
	F_LOG;
	Log("RtpPacketReceived: size: %d", inData.size());
#endif
	((NALUMediaSource *)pdata->mediaSource.get())->SubmitPacket((uint8_t *)inData.data(), inData.getSize());
	if (!pFr) {
		Log("File not open '%s'", VIDEO_RECEIVE);
		return;
	}
	uint16_t datHeader;
	datHeader = htons(inData.getSize()); // Length is in big-endian format (network byte order)
	if (fwrite(&datHeader, sizeof(uint16_t), 1, pFr) != 1) {
		Log("Failed to write header to '%s'", VIDEO_RECEIVE);
	}
	if (fwrite((uint8_t *)inData.data(), 1, inData.getSize(), pFr) != inData.getSize()) {
		Log("Failed to write data to '%s'", VIDEO_RECEIVE);
	}
}

#ifndef SKYPEKIT31
void VideoRTPStageFright::FrameReceived(const Sid::Binary & data, int timestamp_ms) {
	F_LOG;
	// Frame format not implemented
	assert(false);
}
#endif

bool VideoRTPStageFright::StopPlayback() {
	F_LOG;
	pdata->player.stopVideoDecoder();
	pdata->mediaSource.clear();
	return true;
}

#ifdef SKYPEKIT31
bool VideoRTPStageFright::StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height) {
	int initial_framerate_fps = 15;
#else
bool VideoRTPStageFright::StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps) {
#endif
	F_LOG;
	Log("StartRecording: fourcc: %d, initial_max_packet_size: %d, initial_bitrate_bytes_sec: %d, width: %d, height: %d, initial_framerate_fps: %d", fourcc, initial_max_packet_size, initial_bitrate_bytes_sec, width, height, initial_framerate_fps);
	pdata->cameraContext.setupCamera(pdata->previewSurface, width, height, initial_framerate_fps);
//	pdata->cameraContext.startPreview();
	if (!pdata->encoderController.setupEncoder(pdata->mIOMX, pdata->cameraContext.mCameraSource, width, height)) {
		StopRecording();
		return false;
	}
	pdata->encoderController.ConfigureEncoder(pdata->mIOMX, initial_bitrate_bytes_sec * 8);
	pdata->threadRecorder = new RecorderThread(m_Callback, & pdata->encoderController, this);
	pdata->threadRecorder->Start();
	return true;
}

bool VideoRTPStageFright::StopRecording() {
	F_LOG;
	if (pdata->threadRecorder) {
		pdata->threadRecorder->Stop();
		while (pdata->threadRecorder->IsRunning()) {
			pdata->threadRecorder->Sleep(10);
		}
		delete pdata->threadRecorder;
		pdata->threadRecorder = NULL;
	}

	pdata->encoderController.stopEncoder();
	pdata->cameraContext.stopCamera();
	return true;
}

bool VideoRTPStageFright::CustomCommand(const String &cmd, String &result_str) {
	F_LOG;
	if (cmd == "PLAY_FILE") {
		pdata->playbackFile = SKYPEKIT_ROOT_PATH "/testvideo-320x240.264"; // Source: reference/videortphost-test/testvideos/testvideo-320x240.264
	}

	return true;
}

bool VideoRTPStageFright::setVideoSurface(const sp<ISurface> &videoSurface) {
	F_LOG;
	pdata->player.setISurface(videoSurface);
	return OK;
}

bool VideoRTPStageFright::setPreviewSurface(const sp<Surface> &previewSurface) {
	F_LOG;
	pdata->previewSurface = previewSurface;
	return OK;
}
