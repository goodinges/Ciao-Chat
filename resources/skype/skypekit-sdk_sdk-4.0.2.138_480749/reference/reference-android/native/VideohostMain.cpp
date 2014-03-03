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

#define LOG_TAG "VideohostMain"

#include "VideohostMain.h"
#include "VideoRTPStageFright.h"

#include "videohost_ipc/sidg_videortpif_server.hpp"
#include "videohost_ipc/sidg_videortpif_cb_client.hpp"

#include "utils/Log.h"
#include "utility.h"

#include <media/stagefright/CameraSource.h>

#include <media/IOMX.h>

#include <camera/ICamera.h>
#include <camera/CameraParameters.h>

#include "VideoPlayer.h"

#include "surface-hack.h"

#include "AudioTest.h"

const char * fromskypekitkey = "/tmp/" "vidrtp_from_skypekit_key";
const char * toskypekitkey = "/tmp/" "vidrtp_to_skypekit_key";

using android::sp;
using android::Surface;
using android::ISurface;

struct VideohostMain::pData {
	Sid::SkypeVideoRTPInterfaceServer *videoRtpIf_server;
	Sid::SkypeVideoRTPInterfaceCbClient *videoRtpIf_cb_client;
	SkypeVideoRTPInterface* videoRtpIf;
};

VideohostMain::VideohostMain() : videoSurface(NULL), previewSurface(NULL) {
	data = new pData;
	data->videoRtpIf_server = new Sid::SkypeVideoRTPInterfaceServer();
	data->videoRtpIf_cb_client = new Sid::SkypeVideoRTPInterfaceCbClient();

	data->videoRtpIf = new VideoRTPStageFright(data->videoRtpIf_cb_client);
	data->videoRtpIf_server->set_if(data->videoRtpIf );
}

VideohostMain::~VideohostMain() {
	delete data;
	delete data->videoRtpIf_server;
	delete data->videoRtpIf_cb_client;
}

bool VideohostMain::Init(runMode mode) {
	F_LOG;
	mMode = mode;

	Log("Setting video surfaces");
	sp<ISurface> mISurface;
	android::Test::getISurface(videoSurface, mISurface);
	((VideoRTPStageFright *)data->videoRtpIf)->setVideoSurface(mISurface);
	((VideoRTPStageFright *)data->videoRtpIf)->setPreviewSurface(previewSurface);
	Log("Video surfaces set");
	return true;
}


class threadTicker : public SEThread {
	SkypeVideoRTPInterface *mIface;
public:
	threadTicker(SkypeVideoRTPInterface *iface): mIface(iface) {}
	void Run() {
		String dummyResult;
		while( this->IsRunning() ) {
			usleep(33000 * 2);
			mIface->CustomCommand("VIDEO_TICK", dummyResult);
		}
	}
};

void VideohostMain::Run() {
	Log("Starting...");

	if (mMode == NORMAL) {
		Log("Connecting ..");
		data->videoRtpIf_server->Connect(fromskypekitkey, 0);
		data->videoRtpIf_cb_client->Connect(toskypekitkey, 500);

		Log("Connection created, entering loop...");

		Sid::Protocol::Status status;
		do {
			status = data->videoRtpIf_server->ProcessCommands();
		} while (status == Sid::Protocol::OK);

		Log("Shutting down...");

		delete data->videoRtpIf;
		data->videoRtpIf_server->Disconnect();
		data->videoRtpIf_cb_client->Disconnect();
	}
	if (mMode == TEST_FILEPLAYBACK) {
		String dummyResult;
		data->videoRtpIf->Init();
		data->videoRtpIf->CustomCommand("PLAY_FILE", dummyResult);
		usleep(1000000);
		data->videoRtpIf->StartPlayback(MAKEFOURCC('H','2','6','4'), 320, 240);
		threadTicker * t1 = new threadTicker(data->videoRtpIf);
		threadTicker * t2 = new threadTicker(data->videoRtpIf);
		t1->Start();
		usleep(15000 * 2);
		t2->Start();
		while(true) {
			usleep(66000 * 2);
		}
	}
	if (mMode == TEST_AUDIOPLAYBACK) {
		AudioTest *test = new AudioTest();
		test->setPlayFile(SKYPEKIT_ROOT_PATH "/16bit441khz.wav", 44100);
		test->playTest();
	}
	if (mMode == TEST_AUDIORECORD) {
		AudioTest *test = new AudioTest();
		test->setRecordFile(SKYPEKIT_ROOT_PATH "/capture.wav", 44100);
		test->recordTest();
	}

	Log("Closed");
}

bool VideohostMain::setVideoSurface(sp<Surface> surface) {
	F_LOG;
	videoSurface = surface;
	return true;
}

bool VideohostMain::setPreviewSurface(sp<Surface> surface) {
	F_LOG;
	previewSurface = surface;
	return true;
}
