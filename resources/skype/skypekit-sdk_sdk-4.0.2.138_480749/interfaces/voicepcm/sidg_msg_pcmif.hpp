#ifndef __sidg_msg_pcmif_hpp_included
#define __sidg_msg_pcmif_hpp_included

#include "SidPlatform.hpp"
#include "SidPCMInterface.hpp"

namespace Sid {
namespace SkypePCMInterfaceMsgs {
  struct MsgInitResponse {
    enum { BEGIN = 0, END = 1 };
    int rv;
  };
  struct MsgStartRequest {
    enum { BEGIN = 1, END = 2 };
    int deviceType;
  };
  struct MsgStartResponse {
    enum { BEGIN = 2, END = 3 };
    int rv;
  };
  struct MsgStopRequest {
    enum { BEGIN = 3, END = 4 };
    int deviceType;
  };
  struct MsgStopResponse {
    enum { BEGIN = 4, END = 5 };
    int rv;
  };
  struct MsgGetDefaultDeviceRequest {
    enum { BEGIN = 5, END = 6 };
    int deviceType;
  };
  struct MsgGetDefaultDeviceResponse {
    enum { BEGIN = 6, END = 10 };
    int rv;
    String guid;
    String name;
    String productID;
  };
  struct MsgUseDefaultDeviceRequest {
    enum { BEGIN = 10, END = 11 };
    int deviceType;
  };
  struct MsgUseDefaultDeviceResponse {
    enum { BEGIN = 11, END = 12 };
    int rv;
  };
  struct MsgGetCurrentDeviceRequest {
    enum { BEGIN = 12, END = 13 };
    int deviceType;
  };
  struct MsgGetCurrentDeviceResponse {
    enum { BEGIN = 13, END = 17 };
    int rv;
    String guid;
    String name;
    String productID;
  };
  struct MsgUseDeviceRequest {
    enum { BEGIN = 17, END = 19 };
    int deviceType;
    String guid;
  };
  struct MsgUseDeviceResponse {
    enum { BEGIN = 19, END = 20 };
    int rv;
  };
  struct MsgGetDeviceCountRequest {
    enum { BEGIN = 20, END = 21 };
    int deviceType;
  };
  struct MsgGetDeviceCountResponse {
    enum { BEGIN = 21, END = 23 };
    int rv;
    uint count;
  };
  struct MsgGetDevicesRequest {
    enum { BEGIN = 23, END = 24 };
    int deviceType;
  };
  struct MsgGetDevicesResponse {
    enum { BEGIN = 24, END = 31 };
    int rv;
    List_String guid;
    List_String name;
    List_String productID;
  };
  struct MsgGetVolumeParametersRequest {
    enum { BEGIN = 31, END = 32 };
    int deviceType;
  };
  struct MsgGetVolumeParametersResponse {
    enum { BEGIN = 32, END = 37 };
    int rv;
    uint range_min;
    uint range_max;
    uint volume;
    int boost;
  };
  struct MsgSetVolumeRequest {
    enum { BEGIN = 37, END = 39 };
    int deviceType;
    uint volume;
  };
  struct MsgSetVolumeResponse {
    enum { BEGIN = 39, END = 40 };
    int rv;
  };
  struct MsgSetInputBoostRequest {
    enum { BEGIN = 40, END = 41 };
    int boost;
  };
  struct MsgSetInputBoostResponse {
    enum { BEGIN = 41, END = 42 };
    int rv;
  };
  struct MsgGetMuteRequest {
    enum { BEGIN = 42, END = 43 };
    int deviceType;
  };
  struct MsgGetMuteResponse {
    enum { BEGIN = 43, END = 45 };
    int rv;
    int muted;
  };
  struct MsgSetMuteRequest {
    enum { BEGIN = 45, END = 47 };
    int deviceType;
    int mute;
  };
  struct MsgSetMuteResponse {
    enum { BEGIN = 47, END = 48 };
    int rv;
  };
  struct MsgGetSampleRateCountRequest {
    enum { BEGIN = 48, END = 49 };
    int deviceType;
  };
  struct MsgGetSampleRateCountResponse {
    enum { BEGIN = 49, END = 51 };
    int rv;
    uint count;
  };
  struct MsgGetSupportedSampleRatesRequest {
    enum { BEGIN = 51, END = 52 };
    int deviceType;
  };
  struct MsgGetSupportedSampleRatesResponse {
    enum { BEGIN = 52, END = 55 };
    int rv;
    List_uint sampleRates;
  };
  struct MsgGetCurrentSampleRateRequest {
    enum { BEGIN = 55, END = 56 };
    int deviceType;
  };
  struct MsgGetCurrentSampleRateResponse {
    enum { BEGIN = 56, END = 58 };
    int rv;
    uint sampleRate;
  };
  struct MsgSetSampleRateRequest {
    enum { BEGIN = 58, END = 60 };
    int deviceType;
    uint sampleRate;
  };
  struct MsgSetSampleRateResponse {
    enum { BEGIN = 60, END = 61 };
    int rv;
  };
  struct MsgSetNumberOfChannelsRequest {
    enum { BEGIN = 61, END = 63 };
    int deviceType;
    int numberOfChannels;
  };
  struct MsgSetNumberOfChannelsResponse {
    enum { BEGIN = 63, END = 64 };
    int rv;
  };
  struct MsgCustomCommandRequest {
    enum { BEGIN = 64, END = 65 };
    String command;
  };
  struct MsgCustomCommandResponse {
    enum { BEGIN = 65, END = 67 };
    String response;
    int rv;
  };
  struct pcmifMsgs {
    MsgInitResponse m_MsgInitResponse;
    MsgStartRequest m_MsgStartRequest;
    MsgStartResponse m_MsgStartResponse;
    MsgStopRequest m_MsgStopRequest;
    MsgStopResponse m_MsgStopResponse;
    MsgGetDefaultDeviceRequest m_MsgGetDefaultDeviceRequest;
    MsgGetDefaultDeviceResponse m_MsgGetDefaultDeviceResponse;
    MsgUseDefaultDeviceRequest m_MsgUseDefaultDeviceRequest;
    MsgUseDefaultDeviceResponse m_MsgUseDefaultDeviceResponse;
    MsgGetCurrentDeviceRequest m_MsgGetCurrentDeviceRequest;
    MsgGetCurrentDeviceResponse m_MsgGetCurrentDeviceResponse;
    MsgUseDeviceRequest m_MsgUseDeviceRequest;
    MsgUseDeviceResponse m_MsgUseDeviceResponse;
    MsgGetDeviceCountRequest m_MsgGetDeviceCountRequest;
    MsgGetDeviceCountResponse m_MsgGetDeviceCountResponse;
    MsgGetDevicesRequest m_MsgGetDevicesRequest;
    MsgGetDevicesResponse m_MsgGetDevicesResponse;
    MsgGetVolumeParametersRequest m_MsgGetVolumeParametersRequest;
    MsgGetVolumeParametersResponse m_MsgGetVolumeParametersResponse;
    MsgSetVolumeRequest m_MsgSetVolumeRequest;
    MsgSetVolumeResponse m_MsgSetVolumeResponse;
    MsgSetInputBoostRequest m_MsgSetInputBoostRequest;
    MsgSetInputBoostResponse m_MsgSetInputBoostResponse;
    MsgGetMuteRequest m_MsgGetMuteRequest;
    MsgGetMuteResponse m_MsgGetMuteResponse;
    MsgSetMuteRequest m_MsgSetMuteRequest;
    MsgSetMuteResponse m_MsgSetMuteResponse;
    MsgGetSampleRateCountRequest m_MsgGetSampleRateCountRequest;
    MsgGetSampleRateCountResponse m_MsgGetSampleRateCountResponse;
    MsgGetSupportedSampleRatesRequest m_MsgGetSupportedSampleRatesRequest;
    MsgGetSupportedSampleRatesResponse m_MsgGetSupportedSampleRatesResponse;
    MsgGetCurrentSampleRateRequest m_MsgGetCurrentSampleRateRequest;
    MsgGetCurrentSampleRateResponse m_MsgGetCurrentSampleRateResponse;
    MsgSetSampleRateRequest m_MsgSetSampleRateRequest;
    MsgSetSampleRateResponse m_MsgSetSampleRateResponse;
    MsgSetNumberOfChannelsRequest m_MsgSetNumberOfChannelsRequest;
    MsgSetNumberOfChannelsResponse m_MsgSetNumberOfChannelsResponse;
    MsgCustomCommandRequest m_MsgCustomCommandRequest;
    MsgCustomCommandResponse m_MsgCustomCommandResponse;
  };
} // namespace SkypePCMInterfaceMsgs
} // namespace Sid
#endif
