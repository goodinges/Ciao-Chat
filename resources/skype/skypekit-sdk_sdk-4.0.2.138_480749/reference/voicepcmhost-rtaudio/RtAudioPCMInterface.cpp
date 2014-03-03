#include <stdio.h>
#include "RtAudio.h"
#include "SidPlatform.hpp"
#include "SidPCMInterface.hpp"

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

#define RATE 16000
#define IN_CHANNELS 1
#define OUT_CHANNELS 1
#define NR_OR_SAMPLES 80

//======================================================================================================================
// debug helpers
//======================================================================================================================

#define TRACE(x) printf("%s\n", x)

#ifdef _WIN32
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif
const char *devTypeNames[NR_OF_DEVICES] = {"capture", "playback", "PCM"};

void print_buffer(Sid::Binary buffer)
{
/*    unsigned char* ptr = (unsigned char*)buffer.data();
    for (int i = 0; i < buffer.size(); i++)
    {
        printf(" %02x", *ptr++);
        if ((i+1) % 16 == 0)
            printf("\n");
    }*/
}

//======================================================================================================================
// interface
//======================================================================================================================

class RtAudioPCMInterface : public SkypePCMInterface//, public Sid::Thread
{
public:
    RtAudioPCMInterface(SkypePCMCallbackInterface* transport);
    ~RtAudioPCMInterface();

    virtual int Init();
    virtual int Start(int deviceType);
    virtual int Stop(int deviceType);
    virtual int GetDefaultDevice(int deviceType, Sid::String& guid, Sid::String& name, Sid::String& productID);
    virtual int UseDefaultDevice(int deviceType);
    virtual int GetCurrentDevice(int deviceType, Sid::String& guid, Sid::String& name, Sid::String& productID);
    virtual int UseDevice(int deviceType, const Sid::String& guid);
    virtual int GetDeviceCount(int deviceType, uint& count);
    virtual int GetDevices(int deviceType, Sid::List_String& guid, Sid::List_String& name, Sid::List_String& productID);
    virtual int GetVolumeParameters(int deviceType, uint &range_min, uint &range_max, uint &volume, int &boost);
    virtual int SetVolume(int deviceType, unsigned int volume);
    virtual int SetInputBoost(int boost);
    virtual int GetMute(int deviceType, int &muted);
    virtual int SetMute(int deviceType, int mute);
    virtual int GetSampleRateCount(int deviceType, uint& count);
    virtual int GetSupportedSampleRates(int deviceType, Sid::List_uint& sampleRates);
    virtual int GetCurrentSampleRate(int deviceType, unsigned int &sampleRate);
    virtual int SetSampleRate(int deviceType, unsigned int sampleRate);
    virtual int SetNumberOfChannels(int deviceType, int numberOfChannels);
    virtual int CustomCommand(const Sid::String& cmd, Sid::String& response) {return PCMIF_ERROR;}

//    virtual void Run();

private:
    int outputCallback(void* outputBuffer, unsigned int nBufferFrames);
    int inputCallback(void* inputBuffer, unsigned int nBufferFrames);

    static int output_callback(void* outputBuffer, UNUSED void* inputBuffer, unsigned int nBufferFrames, UNUSED double streamTime, RtAudioStreamStatus UNUSED status, void* data)
    {
        return static_cast<RtAudioPCMInterface*>(data)->outputCallback(outputBuffer, nBufferFrames);
    }

    static int input_callback(void* UNUSED outputBuffer, void* inputBuffer, unsigned int nBufferFrames, double UNUSED streamTime, RtAudioStreamStatus UNUSED status, void* data)
    {
        return static_cast<RtAudioPCMInterface*>(data)->inputCallback(inputBuffer, nBufferFrames);
    }

    int startOutputDevice(RtAudio *device);
    int startInputDevice(RtAudio *device);
    int stopDevice(RtAudio *device);

private:
    SkypePCMCallbackInterface* m_transport;
    RtAudio* input_device;
    RtAudio* output_device;
    RtAudio* notification_device;
    int input_started, output_started, notification_started;
    int input_muted, output_muted;
    int input_volume, output_volume;
//    volatile uint input_buffer_size, output_buffer_size;
    Sid::Binary playback_buffer;
    Sid::Binary playback_buffer_transport;
    Sid::Binary capture_buffer;
};

//======================================================================================================================
// implementation
//======================================================================================================================

typedef signed short SAMPLE_TYPE;
#define FORMAT RTAUDIO_SINT16

RtAudioPCMInterface::RtAudioPCMInterface(SkypePCMCallbackInterface* transport)
    : m_transport(transport)
    , input_device(0)
    , output_device(0)
    , notification_device(0)
    , input_started(0)
    , output_started(0)
    , notification_started(0)
    , input_muted(0)
    , output_muted(0)
    , input_volume(100)
    , output_volume(100)
//    , input_buffer_size(0)
//    , output_buffer_size(0)
{
    input_device  = new RtAudio;
    output_device = new RtAudio;
    notification_device    = new RtAudio;
}

RtAudioPCMInterface::~RtAudioPCMInterface()
{
    delete input_device;  input_device  = 0;
    delete output_device; output_device = 0;
    delete notification_device;    notification_device    = 0;
}

int RtAudioPCMInterface::Init()
{
    TRACE(__PRETTY_FUNCTION__);
//    Sid::Thread::Start(/*"RtAudioPCMInterface thread"*/);
    return 0;
}

int RtAudioPCMInterface::startOutputDevice(RtAudio *device)
{
    uint buffer_frames = 512;
    uint fs = RATE;
    RtAudio::StreamParameters params;
    RtAudio::StreamOptions options;
    params.deviceId = 0;//device;
    params.nChannels = OUT_CHANNELS;//channels;
    params.firstChannel = 0;
    options.flags = RTAUDIO_NONINTERLEAVED;

    try {
        device->openStream(&params, NULL, FORMAT, fs, &buffer_frames, &output_callback, (void *)this, &options);
        device->startStream();
    }
    catch (RtError& e) {
        std::cout << '\n' << e.getMessage() << '\n' << std::endl;
        return 0;
    }
    return 1;
}

int RtAudioPCMInterface::startInputDevice(RtAudio *device)
{
    uint buffer_frames = 512;
    uint fs = RATE;
    RtAudio::StreamParameters params;
    params.deviceId = 0;//device;
    params.nChannels = IN_CHANNELS;//channels;
    params.firstChannel = 0;

    try {
        device->openStream(NULL, &params, FORMAT, fs, &buffer_frames, &input_callback, (void *)this);
        device->startStream();
    }
    catch (RtError& e) {
        std::cout << '\n' << e.getMessage() << '\n' << std::endl;
        return 0;
    }
    return 1;
}

int RtAudioPCMInterface::stopDevice(RtAudio *device)
{
    device->closeStream();
    return 1;
}

int RtAudioPCMInterface::Start(int deviceType)
{
    if(deviceType == INPUT_DEVICE)
    {
        printf("Start capture\n");
        if (startInputDevice(input_device))
            input_started = 1;
    }
    else if(deviceType == OUTPUT_DEVICE)
    {
        printf("Start playback\n");
        if (startOutputDevice(output_device))
            output_started = 1;
    }
    else if(deviceType == NOTIFICATION_DEVICE)
    {
        printf("Start Notification: not supported for now\n");
//        if (startOutputDevice(notification_device))
            notification_started = 1;
    }
    return PCMIF_OK;
}

int RtAudioPCMInterface::Stop(int deviceType)
{
    printf("Stop: %s\n", devTypeNames[deviceType]);
    if(deviceType == INPUT_DEVICE)
    {
        stopDevice(input_device);
        input_started = 0;
    }
    if(deviceType == OUTPUT_DEVICE)
    {
        stopDevice(output_device);
        output_started = 0;
    }
    if(deviceType == NOTIFICATION_DEVICE)
    {
//        stopDevice(notification_device);
	notification_started = 0;
    }
    return PCMIF_OK;
}

//=== device control ===================================================================================================

int RtAudioPCMInterface::GetDefaultDevice(int deviceType, Sid::String& guid, Sid::String& name, Sid::String& productID)
{
    TRACE(__PRETTY_FUNCTION__);
    RtAudio::DeviceInfo info;
    int id;
    try {
        if (deviceType == INPUT_DEVICE)
        {
            id = input_device->getDefaultInputDevice();
            info = input_device->getDeviceInfo(id);
        }
        else if (deviceType == OUTPUT_DEVICE)
        {
            id = output_device->getDefaultOutputDevice();
            info = output_device->getDeviceInfo(id);
        }
        else if (deviceType == NOTIFICATION_DEVICE)
        {
            id = notification_device->getDefaultOutputDevice();
            info = notification_device->getDeviceInfo(id);
        }
        else
            return PCMIF_ERROR;
    }
    catch (RtError& e) {
        std::cout << '\n' << e.getMessage() << '\n' << std::endl;
        return PCMIF_ERROR;
    }
    char buf[25];
    sprintf(buf,"%d",id);
    guid = Sid::String(buf);
    name = Sid::String(info.name.c_str());
    productID = Sid::String("");
    
    return PCMIF_OK;
}

int RtAudioPCMInterface::UseDefaultDevice(int deviceType)
{
    printf("UseDefaultDevice %d\n", deviceType);
    TRACE(__PRETTY_FUNCTION__);
    return PCMIF_OK;
}

int RtAudioPCMInterface::GetCurrentDevice(int deviceType, Sid::String& guid, Sid::String& name, Sid::String& productID)
{
    TRACE(__PRETTY_FUNCTION__);
    printf("GetCurrentDevice %d\n", deviceType);
    guid = Sid::String("0");
    name = Sid::String("DefaultDevice");
    productID = Sid::String("");
    return PCMIF_OK;
}

int RtAudioPCMInterface::UseDevice(int deviceType, const Sid::String& guid)
{
    TRACE(__PRETTY_FUNCTION__);
    printf("UseDevice %d\n", deviceType);

    return PCMIF_OK;
}

int RtAudioPCMInterface::GetDeviceCount(int deviceType, uint& count)
{
    TRACE(__PRETTY_FUNCTION__);
    printf("GetDeviceCount %d\n", deviceType);

    if (deviceType == INPUT_DEVICE)
        count = input_device->getDeviceCount();
    else if (deviceType == OUTPUT_DEVICE)
        count = output_device->getDeviceCount();
    else if (deviceType == NOTIFICATION_DEVICE)
        count = notification_device->getDeviceCount();
    else
        return PCMIF_ERROR;
    return PCMIF_OK;
}

int RtAudioPCMInterface::GetDevices(int deviceType, Sid::List_String& guid, Sid::List_String& name, Sid::List_String& productID)
{
    TRACE(__PRETTY_FUNCTION__);
    printf("GetDevices %d\n", deviceType);

    RtAudio* device;
    if (deviceType == INPUT_DEVICE)
        device = input_device;
    else if (deviceType == OUTPUT_DEVICE)
        device = output_device;
    else if (deviceType == NOTIFICATION_DEVICE)
        device = notification_device;
    else
        return PCMIF_ERROR;

    for (uint o = 0; o < device->getDeviceCount(); o++)
    {
        RtAudio::DeviceInfo info;
        info = device->getDeviceInfo(o);
	char buf[25];
	sprintf(buf,"%d",o);
        guid.append(buf);
        name.append(info.name.c_str());
	productID.append("");
    }

    return PCMIF_OK;
}

//=== volume control ===================================================================================================

/* Volume control not supported by RtAudio */
int RtAudioPCMInterface::GetVolumeParameters(int deviceType, unsigned int &range_min, unsigned int &range_max, unsigned int &volume, int &boost)
{
    range_min = 0;
    range_max = 100;
    if(deviceType == INPUT_DEVICE)
        volume = input_volume;
    else if (deviceType == OUTPUT_DEVICE)
        volume = output_volume;
    printf("GetVolume %s %d\n", devTypeNames[deviceType], volume);

    // boost not supported
    boost = -1;
    return PCMIF_OK;
}

/* Volume control not supported by RtAudio */
int RtAudioPCMInterface::SetVolume(int deviceType, unsigned int volume)
{
    printf("SetVolume %s %d\n", devTypeNames[deviceType], volume);
    if(deviceType == INPUT_DEVICE)
        input_volume = volume;
    else if (deviceType == OUTPUT_DEVICE)
        output_volume = volume;

    return PCMIF_OK;
}

int RtAudioPCMInterface::SetInputBoost(int boost)
{
    printf("SetInputBoost %d\n", boost);
    return PCMIF_ERROR_PROP_NOT_SUPPORTED;
}

int RtAudioPCMInterface::GetMute(int deviceType, int &muted)
{
    if(deviceType == INPUT_DEVICE)
        muted = input_muted;
    else
        muted = output_muted;
    printf("GetMute %s %d\n", devTypeNames[deviceType], muted);
    return PCMIF_OK;
}

int RtAudioPCMInterface::SetMute(int deviceType, int mute)
{
    printf("SetMute %s %d\n", devTypeNames[deviceType], mute);
    if(deviceType == INPUT_DEVICE)
        input_muted = mute;
    else
        output_muted = mute;
    return PCMIF_OK;
}

//=== sample rate control ==============================================================================================

int RtAudioPCMInterface::GetSampleRateCount(int deviceType, uint& count)
{
    count = 1;
    printf("GetSampleRateCount %s %d\n", devTypeNames[deviceType], count);
    return PCMIF_OK;
}

int RtAudioPCMInterface::GetSupportedSampleRates(int deviceType, Sid::List_uint& sampleRates)
{
    printf("GetSupportedSampleRates %s\n", devTypeNames[deviceType]);
    sampleRates.append(RATE);
    return PCMIF_OK;
}

int RtAudioPCMInterface::GetCurrentSampleRate(int deviceType, unsigned int &sampleRate)
{
    sampleRate = RATE;
    printf("GetCurrentSampleRate %s %d\n", devTypeNames[deviceType], sampleRate);
    return PCMIF_OK;
}

int RtAudioPCMInterface::SetSampleRate(int deviceType, unsigned int sampleRate)
{
    printf("SetSampleRate %s %d\n", devTypeNames[deviceType], sampleRate);
    if(sampleRate != RATE)
        return PCMIF_ERROR;
    return PCMIF_OK;
}

int RtAudioPCMInterface::SetNumberOfChannels(int deviceType, int numberOfChannels)
{
    printf("SetNumberOfOutputChannels\n");
    return PCMIF_ERROR_PROP_NOT_SUPPORTED;
}

//=== audio i/o ========================================================================================================


inline int frame_size(uint frames, uint channels)
{
    return frames * channels * sizeof(SAMPLE_TYPE);
}

int RtAudioPCMInterface::outputCallback(void* outputBuffer, unsigned int nBufferFrames)
{
    if(output_started)
    {
      printf("%s: %p %d\n", __PRETTY_FUNCTION__, outputBuffer, nBufferFrames);

      // calculate number of bytes needed
      uint requested_bytes = frame_size(nBufferFrames, OUT_CHANNELS);

      while(playback_buffer.size() < requested_bytes)
      {
	int needed_bytes = requested_bytes - playback_buffer.size();
	int bytes_in_playback_buffer = playback_buffer.size();

	// only 10 ms frames are acceptable
	int bytes_to_be_requested = needed_bytes - (needed_bytes % (frame_size(1, OUT_CHANNELS) * RATE / 100))
	  + (frame_size(1, OUT_CHANNELS) * RATE / 100);

	m_transport->OutputDeviceReady(bytes_to_be_requested/frame_size(1, OUT_CHANNELS), RATE, OUT_CHANNELS, playback_buffer_transport);

	// it is possible that less 10 ms frames were returned than requested, looping in that case
	playback_buffer.resize(bytes_in_playback_buffer + playback_buffer_transport.size());
	memcpy((char *)playback_buffer.data() + bytes_in_playback_buffer, playback_buffer_transport.data(), playback_buffer_transport.size());


	// need to prevent endless looping, 0 bytes returned is erroneous situation, play silence
	if(!playback_buffer_transport.size())
	{
	    memset(outputBuffer, 0, frame_size(nBufferFrames, OUT_CHANNELS));
	    return 0;
	}
      }
      memcpy(outputBuffer, playback_buffer.data(), frame_size(nBufferFrames, OUT_CHANNELS));
      
      memcpy((char*)playback_buffer.data(), playback_buffer.data() + frame_size(nBufferFrames, OUT_CHANNELS), playback_buffer.size() - frame_size(nBufferFrames, OUT_CHANNELS));
      playback_buffer.resize(playback_buffer.size() - frame_size(nBufferFrames, OUT_CHANNELS));
    }
    else
    {
      printf("%s: output not started %p %d\n", __PRETTY_FUNCTION__, outputBuffer, nBufferFrames);
    }
    return 0;
}

int RtAudioPCMInterface::inputCallback(void* inputBuffer, unsigned int nBufferFrames)
{

    if(input_started)
    {
      printf("%s: %p %d\n", __PRETTY_FUNCTION__, inputBuffer, nBufferFrames);
      int bytes_received = frame_size(nBufferFrames, IN_CHANNELS);
      int bytes_from_last_callback = capture_buffer.size();
      int bytes_available = bytes_received + bytes_from_last_callback;
      // only 10 ms frames are processed
      int bytes_to_be_sent = bytes_available - (bytes_available % (frame_size(1, IN_CHANNELS) * RATE / 100));

      capture_buffer.resize(bytes_to_be_sent);
    
      memcpy((char*)capture_buffer.data() + bytes_from_last_callback, inputBuffer, bytes_to_be_sent - bytes_from_last_callback);

      m_transport->InputDeviceReady(bytes_to_be_sent/frame_size(1, IN_CHANNELS), RATE, IN_CHANNELS, capture_buffer);

      capture_buffer.resize(bytes_available - bytes_to_be_sent);
      memcpy((char*)capture_buffer.data(), (char*)inputBuffer + bytes_to_be_sent - bytes_from_last_callback, bytes_available - bytes_to_be_sent);
    }
    else
    {
      printf("%s: input not started %p %d\n", __PRETTY_FUNCTION__, inputBuffer, nBufferFrames);
    }
    return 0;
}
/*
void RtAudioPCMInterface::Run()
{
    TRACE(__PRETTY_FUNCTION__);
    while(1)
    {
        Sleep(10);//FIXME: don't uselessly chew cpu here
        if(input_started)
            m_transport->DeviceReady(INPUT_DEVICE, NR_OR_SAMPLES, RATE, IN_CHANNELS);
        if(output_started)
            m_transport->DeviceReady(OUTPUT_DEVICE, NR_OR_SAMPLES, RATE, OUT_CHANNELS);
    }
}
*/
//======================================================================================================================
// instantiation
//======================================================================================================================

SkypePCMInterface* SkypePCMInterfaceGet(SkypePCMCallbackInterface* transport)
{
    return new RtAudioPCMInterface(transport);
}

void SkypePCMInterfaceRelease(SkypePCMInterface* pcmif)
{
    delete pcmif;
}
