#ifndef __sidg_msgs_pcmif_cb_hpp_included__
#define __sidg_msgs_pcmif_cb_hpp_included__
#include "SidPCMInterface.hpp"

namespace Sid {
namespace SkypePCMCallbackInterfaceMsgs {
  struct MsgInputDeviceReadyRequest {
    enum { BEGIN = 0, END = 4 };
    int noOfSamples;
    int fs;
    int nChannels;
    Binary data;
  };
  struct MsgOutputDeviceReadyRequest {
    enum { BEGIN = 4, END = 7 };
    int noOfSamples;
    int fs;
    int nChannels;
  };
  struct MsgOutputDeviceReadyResponse {
    enum { BEGIN = 7, END = 8 };
    Binary data;
  };
  struct MsgNotificationDeviceReadyRequest {
    enum { BEGIN = 8, END = 11 };
    int noOfSamples;
    int fs;
    int nChannels;
  };
  struct MsgNotificationDeviceReadyResponse {
    enum { BEGIN = 11, END = 12 };
    Binary data;
  };
  struct MsgInputAndOutputDeviceReadyRequest {
    enum { BEGIN = 12, END = 18 };
    int length_in_ms;
    int input_fs;
    int output_fs;
    int input_channels;
    int output_channels;
    Binary input_data;
  };
  struct MsgInputAndOutputDeviceReadyResponse {
    enum { BEGIN = 18, END = 19 };
    Binary output_data;
  };
  struct MsgDeviceStoppedRequest {
    enum { BEGIN = 19, END = 20 };
    int deviceType;
  };
  struct MsgCurrentDeviceChangedRequest {
    enum { BEGIN = 20, END = 24 };
    int deviceType;
    String guid;
    String name;
    String productID;
  };
  struct MsgVolumeChangedRequest {
    enum { BEGIN = 24, END = 29 };
    int deviceType;
    uint range_min;
    uint range_max;
    uint volume;
    int boost;
  };
  struct pcmif_cbMsgs {
    MsgInputDeviceReadyRequest m_MsgInputDeviceReadyRequest;
    MsgOutputDeviceReadyRequest m_MsgOutputDeviceReadyRequest;
    MsgOutputDeviceReadyResponse m_MsgOutputDeviceReadyResponse;
    MsgNotificationDeviceReadyRequest m_MsgNotificationDeviceReadyRequest;
    MsgNotificationDeviceReadyResponse m_MsgNotificationDeviceReadyResponse;
    MsgInputAndOutputDeviceReadyRequest m_MsgInputAndOutputDeviceReadyRequest;
    MsgInputAndOutputDeviceReadyResponse m_MsgInputAndOutputDeviceReadyResponse;
    MsgDeviceStoppedRequest m_MsgDeviceStoppedRequest;
    MsgCurrentDeviceChangedRequest m_MsgCurrentDeviceChangedRequest;
    MsgVolumeChangedRequest m_MsgVolumeChangedRequest;
  };
} // namespace SkypePCMCallbackInterfaceMsgs
} // namespace Sid
#endif
