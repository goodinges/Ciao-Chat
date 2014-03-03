#include <stdio.h>
#include <ctype.h>
#include <audiofile.h>
#include "SidPCMInterface.hpp"
#include "SidPlatform.hpp"
#include "SidDebugging.hpp"
#ifndef _WIN32
#include <sys/times.h>
#include <unistd.h>
#endif

#define SAMPLE_RATE		44100
#define CALLBACK_INTERVAL	20

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


class PCMFile : public SkypePCMInterface, public Sid::Thread
{
	  Sid::String current_device_guid;
	  Sid::String current_device_name;
	  Sid::String current_device_productID;
    public:


	PCMFile(SkypePCMCallbackInterface* transport):
	  input_started(0), output_started(0), notification_started(0),
	  input_muted(0), output_muted(0),
	  input_volume(100), output_volume(100),
	  input_sampleRate(SAMPLE_RATE), 
	  output_sampleRate(SAMPLE_RATE),notification_sampleRate(SAMPLE_RATE),
	  input_channels(2), output_channels(2),
	  m_transport(transport),
	  input_file_handle(0), output_file_handle(0),
	  input_file_loop(0)
	{
    	  current_device_guid = Sid::String("0");
	  current_device_name = Sid::String("DefaultDevice");
	  current_device_productID = Sid::String("");
	}
  
	~PCMFile() 
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
	  if(deviceType == INPUT_DEVICE)	
	  {
	    if(input_file_handle)
	    {
	      afCloseFile(input_file_handle);
	      input_file_handle = 0;
	    }
	    if(!input_filename.size())
	    {
		SID_INFO("No input file name");
	    }
	    if(input_filename.size())
	    {
	      SID_INFO("Opening input file %s", input_filename.data());
	      input_file_handle = afOpenFile ( input_filename, "r", NULL);
	      if(!input_file_handle)
	      {
		SID_ERROR("Failed to open input file");
		return PCMIF_ERROR;
	      }
	      int fileFormatVersion;
	      int fileFormat = afGetFileFormat(input_file_handle,&fileFormatVersion);
	      if(fileFormat != AF_FILE_WAVE)
	      {
		SID_ERROR("Input file format not supported %d", fileFormat);
		afCloseFile(input_file_handle);
		return PCMIF_ERROR;
	      }
	      SID_INFO("Input file format version %d", fileFormatVersion);
	      
	      SID_INFO("Input file parameters:");
	      int byteOrder = afGetByteOrder(input_file_handle, AF_DEFAULT_TRACK);
	      input_channels = afGetChannels(input_file_handle, AF_DEFAULT_TRACK);
	      int sample_format;
	      int sample_width;
	      afGetSampleFormat (input_file_handle, AF_DEFAULT_TRACK, &sample_format, &sample_width);
 	      input_sampleRate = (int) afGetRate (input_file_handle, AF_DEFAULT_TRACK);
	      int compression =  afGetCompression (input_file_handle, AF_DEFAULT_TRACK);
	      AFframecount frame_count = afGetFrameCount(input_file_handle, AF_DEFAULT_TRACK);
	      
	      SID_INFO("\tbyteOrder: %s", (byteOrder == AF_BYTEORDER_LITTLEENDIAN)?"AF_BYTEORDER_LITTLEENDIAN":"AF_BYTEORDER_BIGENDIAN");
	      SID_INFO("\tnumber of channels: %d", input_channels);
	      if(sample_format == AF_SAMPFMT_TWOSCOMP)
	      {
		SID_INFO("\tsample_format: signed");
	      }
	      else
	      {
		SID_INFO("\tsample_format: %d", sample_format);
	      }
	      SID_INFO("\tsample_width: %d", sample_width);
	      SID_INFO("\tsample_rate: %d", input_sampleRate);
	      SID_INFO("\tcompression: %d", compression);
	      SID_INFO("\tframes: %d", (int)frame_count);
	      if(byteOrder != AF_BYTEORDER_LITTLEENDIAN ||
		sample_format != AF_SAMPFMT_TWOSCOMP || 
		sample_width != 16 ||
		compression != 0)
	      {
		SID_ERROR("Input file not littleendian S16 PCM");
		afCloseFile(input_file_handle);
		return PCMIF_ERROR;
	      }
	      if((input_channels != 1 && input_channels != 2) || 
		(input_sampleRate != 8000 &&
		input_sampleRate != 16000 &&
		input_sampleRate != 24000 &&
		input_sampleRate != 44100 &&
		input_sampleRate != 48000))
	      {
		SID_ERROR("Input file: only mono/stereo and 8000/16000/24000/44100/48000 Hz supported");
		afCloseFile(input_file_handle);
		return PCMIF_ERROR;
	      }
	    }
	    input_started = 1;
	  }
	  else if(deviceType == OUTPUT_DEVICE)
	  {
	    if(!output_filename.size())
	    {
		SID_INFO("No output file name");
	    }
	    if(output_filename.size())
	    {
	      SID_INFO("Opening output file: %s\n", output_filename.data());

	      AFfilesetup setup = afNewFileSetup ();

	      afInitFileFormat(setup, AF_FILE_WAVE);
	      afInitByteOrder(setup, AF_DEFAULT_TRACK, AF_BYTEORDER_LITTLEENDIAN);
	      afInitChannels(setup, AF_DEFAULT_TRACK, 2);
	      afInitSampleFormat(setup, AF_DEFAULT_TRACK, AF_SAMPFMT_TWOSCOMP, 16);
	      afInitRate(setup, AF_DEFAULT_TRACK, 44100.0);
	      afInitCompression(setup, AF_DEFAULT_TRACK, AF_COMPRESSION_NONE);

	      output_file_handle = afOpenFile (output_filename,"w", setup);
	      afFreeFileSetup (setup);
	      if(!output_file_handle)
	      {
		SID_ERROR("Failed to open output file");
		return PCMIF_ERROR;
	      }
	    }
	    output_started = 1;
	    Sleep(100);
	  }
	  else if(deviceType == NOTIFICATION_DEVICE)
	  {
	    SID_INFO("Start NOTIFICATION_DEVICE\n");
	    notification_started = 1;
	  }
	  return PCMIF_OK;
	}

	virtual int Stop(int deviceType){
	  SID_INFO("Stop: device %d\n", deviceType);
	  if(deviceType == INPUT_DEVICE)	
	  {
	    if(input_file_handle)
	      afCloseFile(input_file_handle);
	    input_file_handle = 0;
	    input_started = 0;
	  }
	  if(deviceType == OUTPUT_DEVICE)
	    output_started = 0;
	  if(deviceType == NOTIFICATION_DEVICE)
	    notification_started = 0;
	  return PCMIF_OK;
	}

	virtual int GetDefaultDevice(int deviceType, Sid::String& guid, Sid::String& name, Sid::String& productID) 
	{
	  guid = Sid::String("0");
	  name = Sid::String("DefaultDevice");
	  productID = Sid::String("");
	  return 0;
	}
  
	virtual int UseDefaultDevice(int deviceType) 
	{
	  SID_INFO("UseDefaultDevice: %d\n",deviceType);
	  return PCMIF_OK;
	}

	virtual int GetCurrentDevice(int deviceType, Sid::String& guid, Sid::String& name, Sid::String& productID) 
	{
	  SID_INFO("GetCurrentDevice: %d\n",deviceType);
	  guid = current_device_guid;
	  name = current_device_name;
	  productID = current_device_productID;
	  return PCMIF_OK;
	}
  
	virtual int UseDevice(int deviceType, const Sid::String& guid) 
	{
	  SID_INFO("UseDevice: %d, %s\n",deviceType, (const char*)guid);
	  return PCMIF_OK;
	}
  
	virtual int GetDeviceCount(int deviceType, uint& count) 
	{
	  count = 1;
	  return PCMIF_OK;
	}
  
	virtual int GetDevices(int deviceType, Sid::List_String& guid, Sid::List_String& name, Sid::List_String& productID) 
	{
	  SID_INFO("GetDevices\n");
	  
	  guid.append("guid0");
	  name.append("DefaultDevice");
	  productID.append("productID 1");

	  return PCMIF_OK;
	}
	
	virtual int GetVolumeParameters(int deviceType, unsigned int &range_min, unsigned int &range_max, unsigned int &volume, int &boost)
	{
	  range_min = 0; 
	  range_max = 100;
	  SID_INFO("GetVolumeParameters %d\n", deviceType);
	  if(deviceType == INPUT_DEVICE)
	    volume = input_volume;
	  else if (deviceType == OUTPUT_DEVICE)
	    volume = output_volume;
	  boost = 0;
	  return PCMIF_OK;
	}
  
	virtual int SetVolume(int deviceType, unsigned int volume)
	{
	  SID_DBG("SetVolume %d %d\n", deviceType, volume);
	  return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	}
  
	virtual int SetInputBoost(int boost)
	{
	  SID_INFO("SetInputBoost %d\n", boost);
	  return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	}

	virtual int GetMute(int deviceType, int &muted)
	{
	  SID_DBG("GetMute %d\n", deviceType);
	  if(deviceType == INPUT_DEVICE)
	    muted = input_muted;
	  else if (deviceType == OUTPUT_DEVICE)
	    muted = output_muted;
	  return PCMIF_OK; 
	}

	virtual int SetMute(int deviceType, int mute)
	{
	  SID_INFO("SetMute %d %d\n", deviceType, mute);
	  if(deviceType == INPUT_DEVICE)
	    input_muted = mute;
	  else if (deviceType == OUTPUT_DEVICE)
	    output_muted = mute;
	  return PCMIF_OK;
	}
	
	virtual int GetSampleRateCount(int deviceType, uint& count)
	{
	  SID_INFO("GetSampleRateCount %d\n", deviceType); 
	  count = 1;
	  return PCMIF_OK;
	}

	virtual int GetSupportedSampleRates(int deviceType, Sid::List_uint& sampleRates)
	{
	  SID_INFO("GetSupportedSampleRates: %d\n", deviceType);
	  if(deviceType == INPUT_DEVICE)
	    sampleRates.append(input_sampleRate);
	  else
	    sampleRates.append(output_sampleRate);
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
	  SID_INFO("GetCurrentSampleRate: %d %d\n", deviceType, sampleRate);
	  return PCMIF_OK;
	}

	virtual int SetSampleRate(int deviceType, unsigned int sampleRate)
	{
	  SID_INFO("SetSampleRate: %d %d\n", deviceType, sampleRate);
	  return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	}
	
	virtual int SetNumberOfChannels(int deviceType, int numberOfChannels)
	{
	  SID_INFO("SetNumberOfOutputChannels %d\n", numberOfChannels);
	  return PCMIF_ERROR_PROP_NOT_SUPPORTED;
	}

	virtual int CustomCommand(const Sid::String& command, Sid::String& response)
	{
	  SID_INFO("CustomCommand %s\n", (char*)command.data());
	  if(command.startWith("IN:"))
	  {
	    input_filename=command.trim("IN:");
	    if(input_filename.length())
	    {
	      int i = 0;
	      while(isspace(input_filename[i])) i++;
	      input_filename=input_filename.right(input_filename.length() - i);
	    }
	    if(!input_filename.length())
	    {
	      response = "ERROR: no filename";
	      return PCMIF_OK;
	    }
	    response = "OK";
	  }
	  else if(command.startWith("OUT:"))
	  {
	    output_filename=command.trim("OUT:");
	    if(output_filename.length())
	    {
	      int i = 0;
	      while(isspace(output_filename[i])) i++;
	      output_filename=output_filename.right(output_filename.length() - i);
	    }
	    if(!output_filename.length())
	    {
	      response = "ERROR: no filename";
	      return PCMIF_OK;
	    }
	    response = "OK";
	      // TODO: reset output file
	  }
	  else if(command.startWith("LOOP:"))
	  {
	    Sid::String data = command.trim("LOOP:");
	    if(data.length())
	      input_file_loop = atoi(data.data());
	    
	      // TODO: set value of looping
	  }
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
	    
	    
	    int samples = output_sampleRate * sleep_interval / 1000;
	    
	    if(output_started)
	    {
	      m_transport->OutputDeviceReady(samples,output_sampleRate,output_channels,output_buf);
	      if(output_file_handle)
	      {
		int samples_returned = output_buf.size() / (sizeof(short) * output_channels);
		if(samples_returned != samples)
		  SID_ERROR("Wrong number of samples returned, requested %d, received %d",samples, samples_returned);
		
		int samples_written = afWriteFrames(output_file_handle, AF_DEFAULT_TRACK, output_buf.data(), samples_returned);
		if(samples_returned != samples_written)
		{
		  SID_ERROR("Wrong number of samples written, requested %d, written %d", samples_returned, samples_written);
		  if(samples_written == 0)
		  {
		    afCloseFile(output_file_handle);
		    output_file_handle = 0;
		    output_started = 0;
		    m_transport->DeviceStopped(OUTPUT_DEVICE);
		  }
		}
	      }
	    }
	    
	    if(input_started)
	    {
	      int samples = input_sampleRate * sleep_interval / 1000;
	    

	      if(input_file_handle)
	      {
		input_buf.resize(samples * sizeof(short) * input_channels);
		int samples_read = afReadFrames(input_file_handle, AF_DEFAULT_TRACK, input_buf.data(), samples);
		if(samples_read != samples)
		{
		  SID_INFO("Input file EOF reached");
		  afCloseFile(input_file_handle);
		  input_file_handle = 0;
		  if(input_file_loop)
		  {
		    // afSeekFrame does not work since ubuntu 9.04
		    //afSeekFrame(input_file_handle, AF_DEFAULT_TRACK, 0);
		    Start(INPUT_DEVICE);
		  }
		}
	      }
	      if(input_muted || !input_file_handle)
	      {
		input_buf.resize(samples * sizeof(short) * input_channels);
		memset((char*)input_buf.data(), 0, sizeof(short)*samples*input_channels);
	      }
	      

	      m_transport->InputDeviceReady(samples,input_sampleRate,input_channels, input_buf);
	    }
	  }
	}

	public:
	  int input_started, output_started, notification_started;
	  int input_muted, output_muted;
	  int input_volume, output_volume;
	  int input_sampleRate, output_sampleRate, notification_sampleRate;
	  int input_channels, output_channels;
	  Sid::Binary input_buf;
	  Sid::Binary output_buf;
	  SkypePCMCallbackInterface* m_transport;
	  Sid::String input_filename, output_filename;
	  AFfilehandle input_file_handle, output_file_handle;
	  int input_file_loop;
	};

SkypePCMInterface* SkypePCMInterfaceGet(SkypePCMCallbackInterface* transport)
{
  return new PCMFile(transport);
}
void SkypePCMInterfaceRelease(SkypePCMInterface* pcmif)
{
  if((static_cast<PCMFile*>(pcmif))->output_file_handle)
  {
    afCloseFile((static_cast<PCMFile*>(pcmif))->output_file_handle);
  }
  delete pcmif;
}

