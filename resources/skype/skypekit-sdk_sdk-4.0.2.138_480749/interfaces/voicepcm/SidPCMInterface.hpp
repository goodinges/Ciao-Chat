/*
 * Copyright (C) 2007-2009 Skype Technologies S.A. Confidential and proprietary
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained
 * in, relating to, or arising from the internet telephony software of Skype
 * Limited (including its affiliates, "Skype"), including without limitation
 * this source code, Skype API and related material of such software
 * proprietary to Skype and/or its licensors ("IP Rights") are and shall
 * remain the exclusive property of Skype and/or its licensors. The recipient
 * hereby acknowledges and agrees that any unauthorized use of the IP Rights
 * is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and ownership
 * thereof.
 */

#ifndef __SkypePCMInterface_hpp__
#define __SkypePCMInterface_hpp__
#include "SidPlatform.hpp"

class SkypePCMInterface {
  public:
  virtual ~SkypePCMInterface() {}
  
  #define INPUT_DEVICE  0
  #define OUTPUT_DEVICE 1
  #define NOTIFICATION_DEVICE    2
  #define NR_OF_DEVICES 3
  
  #define DEVICEMASK_INPUT  		1
  #define DEVICEMASK_OUTPUT 		2
  #define DEVICEMASK_NOTIFICATION    	4
  #define DEVICEMASK_ALL    		7
  
  #define PCMIF_OK                       0
  #define PCMIF_ERROR                    1
  #define PCMIF_ERROR_PROP_NOT_SUPPORTED 2
  #define PCMIF_ERROR_NOT_INITIALIZED    3
  #define PCMIF_ERROR_UNKNOWN_DEVICE     4
  #define PCMIF_ERROR_ALREADY_RUNNING    5
  #define PCMIF_ERROR_SIZE               6
  
  
  
  virtual int  Init() = 0;
  virtual int  Start(int deviceType) = 0;
  virtual int  Stop(int deviceType) = 0;

  virtual int  GetDefaultDevice(int deviceType, Sid::String & guid, Sid::String & name, Sid::String & productID) = 0;
  virtual int  UseDefaultDevice(int deviceType) = 0;
  virtual int  GetCurrentDevice(int deviceType, Sid::String & guid, Sid::String & name, Sid::String & productID) = 0;
  virtual int  UseDevice(int deviceType, const Sid::String &guid) = 0;
  virtual int  GetDeviceCount(int deviceType, uint &count) = 0;
  virtual int  GetDevices(int deviceType, Sid::List_String & guid, Sid::List_String & name, Sid::List_String & productID) = 0;
  
  virtual int  GetVolumeParameters(int deviceType, uint &range_min, uint &range_max, uint &volume, int &boost) = 0;
  virtual int  SetVolume(int deviceType, uint volume) = 0;
  virtual int  SetInputBoost(int boost) = 0;
  virtual int  GetMute(int deviceType, int &muted) = 0;
  virtual int  SetMute(int deviceType, int mute) = 0;
  
  virtual int  GetSampleRateCount(int deviceType, uint &count) = 0;
  virtual int  GetSupportedSampleRates(int deviceType, Sid::List_uint & sampleRates) = 0;
  virtual int  GetCurrentSampleRate(int deviceType, uint &sampleRate) = 0;
  virtual int  SetSampleRate(int deviceType, uint sampleRate) = 0;
  
  virtual int  SetNumberOfChannels(int deviceType, int numberOfChannels) = 0;

  virtual int  CustomCommand(const Sid::String & command, Sid::String & response) = 0;
  
  };
  class SkypePCMCallbackInterface {
    public:
    virtual ~SkypePCMCallbackInterface() {}
    
    virtual void InputDeviceReady(int noOfSamples, int fs, int nChannels, const Sid::Binary& data) = 0;
    virtual void OutputDeviceReady(int noOfSamples, int fs, int nChannels, Sid::Binary& data) = 0;
    virtual void NotificationDeviceReady(int noOfSamples, int fs, int nChannels, Sid::Binary& data) = 0;
    virtual void InputAndOutputDeviceReady(int length_in_ms, int input_fs, int output_fs, int input_channels, int output_channels, const Sid::Binary& inputData, Sid::Binary& output_data) = 0;
    virtual void DeviceStopped(int deviceType) = 0;
    virtual void DeviceListChanged() = 0;
    virtual void CurrentDeviceChanged(int deviceType, const Sid::String &guid, const Sid::String &name, const Sid::String &productID) = 0;
    virtual void VolumeChanged(int deviceType, uint range_min, uint range_max, uint volume, int boost) = 0;
  };


extern "C" SkypePCMInterface* SkypePCMInterfaceGet(SkypePCMCallbackInterface*);
extern "C" void SkypePCMInterfaceRelease(SkypePCMInterface*);
#endif

