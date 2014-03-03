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

#include "AudioPCMAudioflinger.h"
#include "utils/Log.h"
#include "utility.h"

#include <media/mediarecorder.h>

using Sid::String;

using namespace android;

enum {
	CHANNELS_MONO =1,
	CHANNELS_STEREO = 2,
	CHUNK_SIZE_MS = 10,
	SAMPLE_RATE = 44100
};

#define DEBUG_TICK_FREQ 1000

#define CHECK(x) if (x == NULL) { Log( #x " is uninitialized!"); return PCMIF_ERROR;}

int getChunkBytes(int sampleRate, int numChannels, int bytesPerSample) {
	return sampleRate * numChannels * bytesPerSample * CHUNK_SIZE_MS / 1000;
}

int getChunkSamples(int sampleRate) {
	return sampleRate * CHUNK_SIZE_MS / 1000;
}

AudioPCMAudioflinger::AudioPCMAudioflinger(SkypePCMCallbackInterface* transport) :
	input_started(false), output_started(false), notification_started(false),
	m_transport(transport)
{
	AudioRecord *recTest = new AudioRecord(AUDIO_SOURCE_DEFAULT);
	delete recTest;
}

AudioPCMAudioflinger::~AudioPCMAudioflinger() {
}

struct AudioPCMAudioflinger::RecordThread : public android::Thread {
	AudioRecord* audioRecord;
	android::DurationTimer timer;
	SkypePCMCallbackInterface *m_transport;
	RecordThread(SkypePCMCallbackInterface* transport) : m_transport(transport) {
		F_LOG;
	}
	~RecordThread() {
		F_LOG;
	}

	bool threadLoop() {
		F_LOG;
		int flags = AudioRecord::RECORD_AGC_ENABLE | AudioRecord::RECORD_NS_ENABLE;
		flags = 0;
		audioRecord = new AudioRecord(android::AUDIO_SOURCE_DEFAULT, SAMPLE_RATE, AudioSystem::PCM_16_BIT, AudioSystem::CHANNEL_IN_MONO, 0, flags);
		audioRecord->start();
		timer.start();

		const unsigned int timeSlice = 10 * 1000; // 10 MS, in usec
		unsigned long sleepTime = timeSlice;
		nsecs_t idealClock = systemTime();
		nsecs_t newClock;

		int mCounter = 0;
		Log("Entering record loop");
		while(!exitPending()) {
			mCounter++;

			uint32_t chunkSize = audioRecord->frameSize() * audioRecord->getSampleRate() / 100;
			Sid::Binary record_buffer;
			record_buffer.reserve(chunkSize);
			audioRecord->read(record_buffer.data(), chunkSize);
			m_transport->InputDeviceReady(audioRecord->getSampleRate() / 100, audioRecord->getSampleRate(), 1, record_buffer);

			usleep(sleepTime);
			idealClock += 10 * 1000;

			while ((newClock = systemTime()) < idealClock) {
				usleep(1);
			}
			while (newClock - idealClock > 10*1000) {
				idealClock+= 10 * 1000;
			}
			sleepTime = 10 * 1000 - (newClock - idealClock);

			if (((mCounter % DEBUG_TICK_FREQ) == 0)) {
				timer.stop();
				long long time = timer.durationUsecs();
				Log("rec duration: %u ms", (uint32_t) (time / 1000));
			}
		}
		return false;
	}
};

struct AudioPCMAudioflinger::PlaybackThread : public android::Thread {
	AudioTrack * audioTrack;
	android::DurationTimer timer;
	SkypePCMCallbackInterface *m_transport;

	PlaybackThread(SkypePCMCallbackInterface* transport) : m_transport(transport) {
		F_LOG;
	}
	~PlaybackThread() {
		F_LOG;
	}

	// Set volume for this track. Levels must be <= 1.0.
	void setVolume(float left, float right) {
		audioTrack->setVolume(left, right);
	}

	void getVolume(float* left, float* right) {
		audioTrack->getVolume(left, right);
	}

	bool threadLoop() {
		F_LOG;
		audioTrack = new AudioTrack(
			AudioSystem::VOICE_CALL, SAMPLE_RATE, AudioSystem::PCM_16_BIT, AudioSystem::CHANNEL_OUT_MONO,
			0 // Defaults to minimum framecount
			);
		audioTrack->start();
		timer.start();

		const unsigned int timeSlice = 10 * 1000; // 10 ms, in usec
		unsigned long sleepTime = timeSlice;
		nsecs_t idealClock = systemTime();
		nsecs_t newClock;

		int mCounter = 0;
		Log("Entering playback loop");
		while (!exitPending()) {
			mCounter++;

			uint32_t chunkSize = audioTrack->frameSize() * audioTrack->getSampleRate() / 100;

			Sid::Binary playback_buffer;
			m_transport->OutputDeviceReady(audioTrack->getSampleRate() / 100, audioTrack->getSampleRate(), 1, playback_buffer);

			if (playback_buffer.size() != chunkSize ) {
				Log("size(): %u, chunkSize: %u", playback_buffer.size(), chunkSize);
			}
			// Calculate bytes per 10 ms
			int written = audioTrack->write(playback_buffer.data(), playback_buffer.size());
			while(written < 1) {
				usleep(1);
				written = audioTrack->write(playback_buffer.data(), playback_buffer.size());
			}

			usleep(sleepTime);
			idealClock += timeSlice;

			while((newClock = systemTime()) < idealClock) {
				usleep(1); // Block until time catches up
			}
			while(newClock - idealClock > timeSlice) {
				idealClock+= timeSlice; // We had a hiccup
			}
			sleepTime = timeSlice - (newClock - idealClock);

			if (((mCounter % DEBUG_TICK_FREQ) == 0)) {
				timer.stop();
				long long time = timer.durationUsecs();
				Log("Play duration: %u ms", (uint32_t) (time / 1000));
			}
		}
		audioTrack->stop();
		delete audioTrack;
		return false;
	}
};

int AudioPCMAudioflinger::Init() {
	F_LOG;
	int streamType = AudioSystem::VOICE_CALL;
	int afSampleRate = 0, afFrameCount = 0;
	uint32_t afLatency = 0;

	if (AudioSystem::getOutputSamplingRate(&afSampleRate, streamType) != NO_ERROR) {
		Log("getOutputSamplingRate error");
		return PCMIF_ERROR;
	}
	if (AudioSystem::getOutputFrameCount(&afFrameCount, streamType) != NO_ERROR) {
		Log("getOutputFrameCount error");
		return PCMIF_ERROR;
	}
	if (AudioSystem::getOutputLatency(&afLatency, streamType) != NO_ERROR) {
		Log("getOutputLatency error");
		return PCMIF_ERROR;
	}
	Log("Output: sampling rate: %u, framecount: %u, latency: %u", afSampleRate, afFrameCount, afLatency);

	return PCMIF_OK;
}

int AudioPCMAudioflinger::Start(int deviceType) {
	F_LOG;
	if (deviceType == INPUT_DEVICE) {
		Log("Start INPUT_DEVICE");
		if (!input_started) {
			recordThread = new RecordThread(m_transport);
			recordThread->run("SkypeKitPCMRecord");
		}
		input_started = true;
	}
	else if (deviceType == OUTPUT_DEVICE) {
		Log("Start OUTPUT_DEVICE");
		if (!output_started) {
			playbackThread = new PlaybackThread(m_transport);
			playbackThread->run("SkypeKitPCMPlayback");
		}
		output_started = true;
	}
	else if (deviceType == NOTIFICATION_DEVICE) {
		Log("Start NOTIFICATION_DEVICE");
//		ringerDeviceThread = new PlaybackThread(m_transport);
//		ringerDeviceThread->run("SkypeKitPCMRinger");
		notification_started = true;
	}
	return PCMIF_OK;
}

int AudioPCMAudioflinger::Stop(int deviceType) {
	F_LOG;
	if (deviceType == INPUT_DEVICE) {
		Log("Stop INPUT_DEVICE");
		if (input_started) {
			recordThread->requestExit();
//			 delete recordThread;
//			 recordThread = NULL;
		}
		input_started = false;
	}
	else if (deviceType == OUTPUT_DEVICE) {
		Log("Stop OUTPUT_DEVICE");
		if (output_started) {
			playbackThread->requestExit();
//			delete playbackThread;
//			playbackThread = NULL;
		}
		output_started = false;
		Log("OUTPUT_DEVICE stopped");
	}
	else if (deviceType == NOTIFICATION_DEVICE) {
		Log("Stop NOTIFICATION_DEVICE");
//		ringerDeviceThread->requestExitAndWait();
		notification_started = false;
		Log("NOTIFICATION_DEVICE stopped");
	}
	return PCMIF_OK;
}

int AudioPCMAudioflinger::GetDefaultDevice(int deviceType, String & guid, Sid::String & name, String & productID) {
	F_LOG;
	guid = "0";
	name = "DefaultDevice";
	productID = "";
	return PCMIF_OK;
}

int AudioPCMAudioflinger::UseDefaultDevice(int deviceType) {
	F_LOG;
	return PCMIF_OK;
}

int AudioPCMAudioflinger::GetCurrentDevice(int deviceType, String & guid, String & name, String & productID) {
	F_LOG;
	guid = "0";
	name = "DefaultDevice";
	productID = "";
	return PCMIF_OK;
}

int AudioPCMAudioflinger::UseDevice(int deviceType, const String &guid) {
	F_LOG;
	return PCMIF_OK;
}

int AudioPCMAudioflinger::GetDeviceCount(int deviceType, uint &count) {
	F_LOG;
	count = 3; // Input, output, ringer
	return PCMIF_OK;
}

int AudioPCMAudioflinger::GetDevices(int deviceType, Sid::List_String & guid, Sid::List_String & name, Sid::List_String & productID) {
	F_LOG;

	guid.append("guid0");
	guid.append("9");
	guid.append("guid10");

	name.append("DefaultDevice");
	name.append("Dev9");
	name.append("Dev10");

	productID.append("productID 1");
	productID.append("productID 2");
	productID.append("productID 3");
	return PCMIF_OK;
}

#ifdef SKYPEKIT31
int AudioPCMAudioflinger::GetVolumeParameters(int deviceType, uint &range_min, uint &range_max, uint &volume, uint &boost) {
#else
int AudioPCMAudioflinger::GetVolumeParameters(int deviceType, uint &range_min, uint &range_max, uint &volume, int &boost) {
#endif
//	F_LOG;
	range_min = 0;
	range_max = 100;
	volume = 0;
	boost = 0;
	if (deviceType == INPUT_DEVICE) {
		return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	} else if (deviceType == OUTPUT_DEVICE) {
		float left, right;
		playbackThread->getVolume(&left, &right);
		volume = (left * 100.0f);
		return PCMIF_OK;
	} else if (deviceType == NOTIFICATION_DEVICE) {
		return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	} else {
		return PCMIF_ERROR_UNKNOWN_DEVICE;
	}
}

int AudioPCMAudioflinger::SetVolume(int deviceType, uint volume) {
	F_LOG;
	if (deviceType == INPUT_DEVICE) {
		return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	} else if (deviceType == OUTPUT_DEVICE) {
		float left = ((float) volume) / 100.0f;
		playbackThread->setVolume(left, left);
		return PCMIF_OK;
	} else if (deviceType == NOTIFICATION_DEVICE) {
		return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	} else {
		return PCMIF_ERROR_UNKNOWN_DEVICE;
	}
}

int AudioPCMAudioflinger::SetInputBoost(int boost) {
	F_LOG;

	return PCMIF_ERROR_PROP_NOT_SUPPORTED;
}

int AudioPCMAudioflinger::GetMute(int deviceType, int &muted) {
//	F_LOG;

	muted = 0;
	return PCMIF_OK;
}

int AudioPCMAudioflinger::SetMute(int deviceType, int mute) {
	F_LOG;

	return PCMIF_ERROR_PROP_NOT_SUPPORTED;
}

int AudioPCMAudioflinger::GetSampleRateCount(int deviceType, uint &count) {
	F_LOG;
	count = 1;
	return PCMIF_OK;
}

int AudioPCMAudioflinger::GetSupportedSampleRates(int deviceType, Sid::List_uint & sampleRates) {
	F_LOG;
	sampleRates.append(SAMPLE_RATE);
	return PCMIF_OK;
}

int AudioPCMAudioflinger::GetCurrentSampleRate(int deviceType, uint &sampleRate) {
	F_LOG;
	sampleRate = SAMPLE_RATE;
	return PCMIF_OK;
}

int AudioPCMAudioflinger::SetSampleRate(int deviceType, uint sampleRate) {
	F_LOG;
	if (sampleRate != SAMPLE_RATE) {
		return PCMIF_ERROR;
	}
	return PCMIF_OK;
}

int AudioPCMAudioflinger::SetNumberOfChannels(int deviceType, int numberOfChannels) {
	F_LOG;
	return PCMIF_OK;
}

int AudioPCMAudioflinger::CustomCommand(const String & command, String & response) {
	F_LOG;
	return PCMIF_OK;
}
