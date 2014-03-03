#ifndef sidg_pcmif_cb_client_HPPINCLUDED
#define sidg_pcmif_cb_client_HPPINCLUDED

#include "SidPlatform.hpp"
#include "SidAVClient.hpp"
#include "SidAVServer.hpp"
#include "SidPCMInterface.hpp"
#include "sidg_msg_pcmif_cb.hpp"

namespace Sid {
  class SkypePCMCallbackInterfaceClient: public SkypePCMCallbackInterface, public AVClient
  {

  public:

    SkypePCMCallbackInterfaceClient() : AVClient(Field::M_SkypePCMCallbackInterface_fields)
    {
    }

    void InputDeviceReady(int noOfSamples, int fs, int nChannels, const Binary& data);
    void OutputDeviceReady(int noOfSamples, int fs, int nChannels, Binary& data);
    void NotificationDeviceReady(int noOfSamples, int fs, int nChannels, Binary& data);
    void InputAndOutputDeviceReady(int length_in_ms, int input_fs, int output_fs, int input_channels, int output_channels, const Binary& input_data, Binary& output_data);
    void DeviceStopped(int deviceType);
    void DeviceListChanged();
    void CurrentDeviceChanged(int deviceType, const String& guid, const String& name, const String& productID);
    void VolumeChanged(int deviceType, uint range_min, uint range_max, uint volume, int boost);
    Mutex m_Mutex;
  };
}
#endif
