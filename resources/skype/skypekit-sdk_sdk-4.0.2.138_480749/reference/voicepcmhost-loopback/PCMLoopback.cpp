#include <stdio.h>
#include "SidPCMInterface.hpp"
#include "SidPlatform.hpp"
#ifndef _WIN32
#include <sys/times.h>
#include <unistd.h>
#endif

static const char* deviceNames[] = {"INPUT_DEVICE", "OUTPUT_DEVICE", "NOTIFICATION_DEVICE"};

#define INPUT_BUF_ARRAY_SIZE 	20
#define SAMPLE_RATE		44100
#define CALLBACK_INTERVAL	10
//#define USE_SINGLE_CALLBACK	1
int clocks_per_second = 0;
unsigned int get_milliseconds()
{
#ifdef _WIN32
	return GetTickCount();
#else
	if(clocks_per_second == 0)
	{
	  #ifdef _SC_CLK_TCK
	      clocks_per_second = sysconf(_SC_CLK_TCK);
	      if (clocks_per_second <= 0)
	  #endif
		  clocks_per_second = CLOCKS_PER_SEC;
	}
	struct tms t;
	int rv = ((unsigned long long)(unsigned int)times(&t)) * 1000 / clocks_per_second;
	
	return rv;
#endif
}

struct DevInfo
{
    Sid::String guid;
    Sid::String name;
    Sid::String productID;
};

class PCMLoopback : public SkypePCMInterface, public Sid::Thread{
      int current_device[3];
      DevInfo devices[3];
    public:


	PCMLoopback(SkypePCMCallbackInterface* transport):
	  input_started(0), output_started(0), notification_started(0),
	  input_muted(0), output_muted(0),
	  input_volume(100), output_volume(100),
	  input_sampleRate(SAMPLE_RATE), 
	  output_sampleRate(SAMPLE_RATE),notification_sampleRate(SAMPLE_RATE),
	  numOfOutputChannels(1),
	  input_p(0),
	  m_transport(transport)
	{
	  for(int i = 0 ; i < INPUT_BUF_ARRAY_SIZE; i++)
	  {
	    input_buf[i].resize(SAMPLE_RATE * CALLBACK_INTERVAL * 2 / 1000 );
	    memset((char* )input_buf[i].data(), 0, SAMPLE_RATE * CALLBACK_INTERVAL * 2 / 1000);
	  }

        devices[0].guid = "guid0";
        devices[1].guid = "9";
        devices[2].guid = "guid10";

        devices[0].name = "DefaultDevice";
        devices[1].name = "Dev9";
        devices[2].name = "Dev10";

        devices[0].productID = "productID 1";
        devices[1].productID = "productID 2";
        devices[2].productID = "productID 3";

        current_device[0] = 0;
        current_device[1] = 0;
        current_device[2] = 0;
    }
  
	~PCMLoopback() 
	{
	}
	
//	SkypePCMInterface::SendTransport* get_pcmtransport(){ return m_transport; }

	virtual int Init()
	{ 
	  printf("Init\n"); 
	  Sid::Thread::start("callbackthread");
	  return 0;
	}

	virtual int Start(int deviceType)
	{
        printf("Start: %s\n", deviceNames[deviceType]);
        if(deviceType == INPUT_DEVICE)	
	  {
	    input_started = 1;
	  }
	  else if(deviceType == OUTPUT_DEVICE)
	  {
	    output_started = 1;
	  }
	  else if(deviceType == NOTIFICATION_DEVICE)
	  {
	    notification_started = 1;
	  }
	  return PCMIF_OK;
	}

	virtual int Stop(int deviceType){
        printf("Stop: %s\n", deviceNames[deviceType]);
        if(deviceType == INPUT_DEVICE)	
	    input_started = 0;
	  if(deviceType == OUTPUT_DEVICE)
	    output_started = 0;
	  if(deviceType == NOTIFICATION_DEVICE)
	    notification_started = 0;
	  return PCMIF_OK;
	}

	virtual int GetDefaultDevice(int deviceType, Sid::String& guid, Sid::String& name, Sid::String& productID) 
	{
	  guid = devices[0].guid;
	  name = devices[0].name;
	  productID = devices[0].productID;
	  return 0;
	}
  
	virtual int UseDefaultDevice(int deviceType) 
	{
	  printf("UseDefaultDevice: %s\n", deviceNames[deviceType]);
      current_device[deviceType] = 0;
	  return PCMIF_OK;
	}

	virtual int GetCurrentDevice(int deviceType, Sid::String& guid, Sid::String& name, Sid::String& productID) 
	{
        guid = devices[current_device[deviceType]].guid;
        name = devices[current_device[deviceType]].name;
        productID = devices[current_device[deviceType]].productID;
        printf("GetCurrentDevice: %s, %s\n",deviceNames[deviceType], guid.data());
        return PCMIF_OK;
	}

	virtual int UseDevice(int deviceType, const Sid::String& guid) 
	{
	  printf("UseDevice: %s, %s\n", deviceNames[deviceType], guid.data());
      for (uint i = 0; i < sizeof(devices)/sizeof(devices[0]); ++i)
      {
          if (devices[i].guid == guid)
          {
            current_device[deviceType] = i;
            return PCMIF_OK;
          }
      }
	  return PCMIF_ERROR;
	}

	virtual int GetDeviceCount(int deviceType, uint& count) 
	{
	  count = sizeof(devices)/sizeof(devices[0]);
	  return PCMIF_OK;
	}
  
	virtual int GetDevices(int deviceType, Sid::List_String& guid, Sid::List_String& name, Sid::List_String& productID) 
	{
	  printf("GetDevices: %s\n", deviceNames[deviceType]);

      for(uint i = 0; i < sizeof(devices)/sizeof(devices[0]); ++i)
      {
          guid.append(devices[i].guid);
          name.append(devices[i].name);
          productID.append(devices[i].productID);
      }

	  return PCMIF_OK;
	}
	
	virtual int GetVolumeParameters(int deviceType, unsigned int &range_min, unsigned int &range_max, unsigned int &volume, int &boost)
	{
	  range_min = 0; 
	  range_max = 100;
	  printf("GetVolumeParameters: %s\n", deviceNames[deviceType]);
	  if(deviceType == INPUT_DEVICE)
	    volume = input_volume;
	  else if (deviceType == OUTPUT_DEVICE)
	    volume = output_volume;
	  
	  // input boost is not supported in this example
	  boost = -1;
	  return PCMIF_OK;
	}
  
	virtual int SetVolume(int deviceType, unsigned int volume)
	{
	  printf("SetVolume: %s, %d\n", deviceNames[deviceType], volume);
	  if(deviceType == INPUT_DEVICE)
	    input_volume = volume;
	  else if (deviceType == OUTPUT_DEVICE)
	    output_volume = volume;
	  return PCMIF_OK;
	}
  
	virtual int SetInputBoost(int boost)
	{
	  printf("SetInputBoost: %d\n", boost);
	  return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	}

	virtual int GetMute(int deviceType, int &muted)
	{
	  printf("GetMute: %s\n", deviceNames[deviceType]);
	  if(deviceType == INPUT_DEVICE)
	    muted = input_muted;
	  else if (deviceType == OUTPUT_DEVICE)
	    muted = output_muted;
	  return PCMIF_OK; 
	}

	virtual int SetMute(int deviceType, int mute)
	{
	  printf("SetMute: %s, %d\n", deviceNames[deviceType], mute);
	  if(deviceType == INPUT_DEVICE)
	    input_muted = mute;
	  else if (deviceType == OUTPUT_DEVICE)
	    output_muted = mute;
	  return PCMIF_OK;
	}
	
	virtual int GetSampleRateCount(int deviceType, uint& count)
	{
	  printf("GetSampleRateCount: %s\n", deviceNames[deviceType]);
	  count = 1;
	  return PCMIF_OK;
	}

	virtual int GetSupportedSampleRates(int deviceType, Sid::List_uint& sampleRates)
	{
	  printf("GetSupportedSampleRates: %s\n", deviceNames[deviceType]);
	  sampleRates.append(SAMPLE_RATE);
	  return PCMIF_OK;
	}

	virtual int GetCurrentSampleRate(int deviceType, unsigned int &sampleRate)
	{
	  if(deviceType == INPUT_DEVICE)
	    sampleRate = input_sampleRate;
	  else if (deviceType == OUTPUT_DEVICE)
	    sampleRate = output_sampleRate;
	  else if (deviceType == NOTIFICATION_DEVICE)
	    sampleRate = notification_sampleRate;
	  else
	    return PCMIF_ERROR_UNKNOWN_DEVICE;
	  printf("GetCurrentSampleRate: %s, %d\n", deviceNames[deviceType], sampleRate);
	  return PCMIF_OK;
	}

	virtual int SetSampleRate(int deviceType, unsigned int sampleRate)
	{
	  printf("SetSampleRate: %s, %d\n", deviceNames[deviceType], sampleRate);
	  if(sampleRate != SAMPLE_RATE)
	    return PCMIF_ERROR; 
	  if(deviceType == INPUT_DEVICE)
	    input_sampleRate = sampleRate;
	  else if (deviceType == OUTPUT_DEVICE)
	    output_sampleRate = sampleRate;
	  else if (deviceType == NOTIFICATION_DEVICE)
	    notification_sampleRate = sampleRate;
	  else
	    return PCMIF_ERROR_UNKNOWN_DEVICE;
	  return PCMIF_OK;
	}
	
	virtual int SetNumberOfChannels(int deviceType, int numberOfChannels)
	{
	  printf("SetNumberOfOutputChannels: %s, %d\n", deviceNames[deviceType], numberOfChannels);
	  if(deviceType != OUTPUT_DEVICE)
	    return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	  numOfOutputChannels = numberOfChannels;
	  return PCMIF_OK;
	}

	virtual int CustomCommand(const Sid::String& command, Sid::String& response)
	{
	  printf("CustomCommand %s\n", (char*)command.data());
		if(command == "PING")
			response = "PONG";
		else
			response = "NOT SUPPORTED";
		return PCMIF_OK;
	}

	void Run()
	{
	  int sleep_interval = CALLBACK_INTERVAL;
	  unsigned int stream_time;
	  stream_time = get_milliseconds();

	  while(1)
	  {
	    if(stream_time > get_milliseconds())
	    {
	      Sleep(sleep_interval);
	      continue;
	    }

	    stream_time += sleep_interval;

	    #ifdef USE_SINGLE_CALLBACK
	    if(input_started || output_started)
	    {
	      if(input_started)
		m_transport->InputAndOutputDeviceReady(sleep_interval, input_sampleRate, output_sampleRate, 1, numOfOutputChannels, input_buf[input_p % INPUT_BUF_ARRAY_SIZE], output_buf);
	      else
		m_transport->InputAndOutputDeviceReady(sleep_interval, input_sampleRate, output_sampleRate, 1, numOfOutputChannels, null_buf, output_buf);
	    }
	    #else
	    int input_samples = input_buf[input_p % INPUT_BUF_ARRAY_SIZE].size() / 2;
	    int output_samples = output_sampleRate * sleep_interval / 1000;

	    if(output_started)
	      m_transport->OutputDeviceReady(output_samples,output_sampleRate,numOfOutputChannels,output_buf);
	    if(input_started)
	      m_transport->InputDeviceReady(input_samples,input_sampleRate,1, input_buf[input_p % INPUT_BUF_ARRAY_SIZE]);
	    #endif
	    
	    if(output_started)
	    {
	      unsigned int samples_returned = output_buf.size() /( 2 * numOfOutputChannels );
	      

	      if(input_buf[(input_p % INPUT_BUF_ARRAY_SIZE)].size() != samples_returned * 2)
		input_buf[(input_p % INPUT_BUF_ARRAY_SIZE)].resize(samples_returned * 2);
    
	      if(input_muted)
	      {
		memset((char*)input_buf[(input_p % INPUT_BUF_ARRAY_SIZE)].data(), 0, input_buf[(input_p % INPUT_BUF_ARRAY_SIZE)].size());
	      }
	      else if(numOfOutputChannels > 1)
	      {
		short* ob = (short*) output_buf.data();
		short* ib = (short*) input_buf[(input_p % INPUT_BUF_ARRAY_SIZE)].data();
		for(unsigned int i = 0 ; i < samples_returned; i ++)
		{
		  ib[i] = ob[i * numOfOutputChannels];
		}
		    
	      }
	      else
	      {
		input_buf[(input_p % INPUT_BUF_ARRAY_SIZE)] = output_buf;
	      }
	    }
	    input_p ++;

	  }
	}

	private:
	  int input_started, output_started, notification_started;
	  int input_muted, output_muted;
	  int input_volume, output_volume;
	  int input_sampleRate, output_sampleRate, notification_sampleRate;
//	  int inputSamples, outputSamples;
	  int numOfOutputChannels;
	  Sid::Binary input_buf[INPUT_BUF_ARRAY_SIZE];
	  Sid::Binary output_buf;
	  Sid::Binary null_buf;
	  int input_p;
	  SkypePCMCallbackInterface* m_transport;
	};

SkypePCMInterface* SkypePCMInterfaceGet(SkypePCMCallbackInterface* transport)
{
  return new PCMLoopback(transport);
}
void SkypePCMInterfaceRelease(SkypePCMInterface* pcmif)
{
  delete pcmif;
}

