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

#ifndef AUDIOPCMAUDIOFLINGER_H_
#define AUDIOPCMAUDIOFLINGER_H_

#include "SidPCMInterface.hpp"
#include <media/AudioRecord.h>
#include <media/AudioTrack.h>

class AudioPCMAudioflinger : public SkypePCMInterface {
	bool input_started, output_started, notification_started;

	struct PlaybackThread;
	struct RecordThread;
	SkypePCMCallbackInterface* m_transport;

	PlaybackThread *playbackThread;
	PlaybackThread *ringerDeviceThread;
	RecordThread *recordThread;

public:
	AudioPCMAudioflinger(SkypePCMCallbackInterface* transport);
	virtual ~AudioPCMAudioflinger();

	int Init();
	int Start(int deviceType);
	int Stop(int deviceType);

	int GetDefaultDevice(int deviceType, Sid::String & guid, Sid::String & name, Sid::String & productID);
	int UseDefaultDevice(int deviceType);
	int GetCurrentDevice(int deviceType, Sid::String & guid, Sid::String & name, Sid::String & productID);
	int UseDevice(int deviceType, const Sid::String &guid);
	int GetDeviceCount(int deviceType, uint &count);
	int GetDevices(int deviceType, Sid::List_String & guid, Sid::List_String & name, Sid::List_String & productID);

#ifdef SKYPEKIT31
	int GetVolumeParameters(int deviceType, uint &range_min, uint &range_max, uint &volume, uint &boost);
#else
	int GetVolumeParameters(int deviceType, uint &range_min, uint &range_max, uint &volume, int &boost);
#endif
	int SetVolume(int deviceType, uint volume);
	int SetInputBoost(int boost);
	int GetMute(int deviceType, int &muted);
	int SetMute(int deviceType, int mute);

	int GetSampleRateCount(int deviceType, uint &count);
	int GetSupportedSampleRates(int deviceType, Sid::List_uint & sampleRates);
	int GetCurrentSampleRate(int deviceType, uint &sampleRate);
	int SetSampleRate(int deviceType, uint sampleRate);

	int SetNumberOfChannels(int deviceType, int numberOfChannels);

	int CustomCommand(const Sid::String & command, Sid::String & response);
};

#endif /* AUDIOPCMAUDIOFLINGER_H_ */
