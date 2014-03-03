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

#ifndef VIDEORTPSTAGEFRIGHT_H_
#define VIDEORTPSTAGEFRIGHT_H_

#include <SidVideoRTPInterface.hpp>

#include <surfaceflinger/Surface.h>

#define VIDEO_SEND (SKYPEKIT_ROOT_PATH "/send.dat")
#define VIDEO_RECEIVE (SKYPEKIT_ROOT_PATH "/receive.dat")

using Sid::String;
using Sid::Binary;

struct VideoRTPStageFright : public SkypeVideoRTPInterface {
	struct pData;

	VideoRTPStageFright(SkypeVideoRTPInterfaceCb *callback);
	virtual ~VideoRTPStageFright();

#ifdef SKYPEKIT31
	bool GetVersion(String & version);
#endif
	int GetLastError();

	bool Init();
	bool Uninit();

	bool GetCodecCount( int &codec_count );
	bool GetCodec(int index, VideoCodec &codec );
#ifndef SKYPEKIT31
	bool GetSenderResolutionCount(int &res_cap_count);
	bool GetSenderResolution(int index, FixedRes & resolution);
	bool GetReceiverResolutionCount(int &resolution_count);
	bool GetReceiverResolution(int index, ResCap & resolution);
#endif

	bool GetIntProperty(int prop_id, int index, int &value);
	bool SetIntProperty(int prop_id, int index, int value);
	bool GetStringProperty(int prop_id, int index, String &value);
	bool SetStringProperty(int prop_id, int index, const String &value);
	bool GetBinaryProperty(int prop_id, int index, Binary &value);
	bool SetBinaryProperty(int prop_id, int index, const Binary &value);

	bool RequestKeyFrame();
	bool RequestRecoveryFrame(int reserved);

	bool SetFramerate(int fps);
	bool SetMaxPacketSize(int bytes);
	bool SetBitrate(int bitrate);

	bool StartPlayback(int decoderfourcc, int width, int height);
	bool StopPlayback();

#ifdef SKYPEKIT31
	bool StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height);
#else
	bool StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps);
#endif
	bool StopRecording();

#ifdef SKYPEKIT31
	void SubmitRTPPacket(const Binary &data);
#else
	void RtpPacketReceived(const Sid::Binary &data);
	void FrameReceived(const Sid::Binary &data, int timestamp_ms);
#endif

	bool CustomCommand(const String &cmd, String &result_str);
#ifndef SKYPEKIT31
	bool VideoCallStart() { return true; };
	bool VideoCallEnd() { return true; };
#endif

	//
	bool setVideoSurface(const android::sp<android::ISurface> &videoSurface);
	bool setPreviewSurface(const android::sp<android::Surface> &previewSurface);
	FILE *pFs;

private:
	FILE *pFr;
	SkypeVideoRTPInterfaceCb *m_Callback;

	pData *pdata;
};

#endif /* VIDEORTPSTAGEFRIGHT_H_ */
