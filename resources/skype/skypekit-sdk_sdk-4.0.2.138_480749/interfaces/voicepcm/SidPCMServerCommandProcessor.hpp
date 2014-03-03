#ifndef __PCMServerCommandProcessor_INCLUDED_HPP
#define __PCMServerCommandProcessor_INCLUDED_HPP

#include "SidAVIpcServerSession.hpp"
#include "SidPCMInterface.hpp"
#include "sidg_evt_pcmif.hpp"

namespace Sid {

  class PCMServerCommandProcessor : public AVIpcServerCommandProcessor {
  public:
    PCMServerCommandProcessor(Session* session) : AVIpcServerCommandProcessor(session) {}
    Protocol::Status ProcessCall(Protocol::ServerInterface*const protocol, uint rid, uint method);
  };

  class IpcPCMTransport : public SkypePCMInterface::SendTransport
  {
    public:
      IpcPCMTransport(AVIpcServerSession<PCMServerCommandProcessor>* session):m_session(session) {}
      ~IpcPCMTransport(){}

      void DeviceReady(int deviceType, int noOfSamples, int fs, int nChannels)
      {
	  ev_SendTransport_deviceReady(m_session->get_protocol(), deviceType, noOfSamples, fs, nChannels);
      }
      void MultipleDevicesReady(int deviceMask, int noOfSamples, int fs, int nChannels)
      {
	  ev_SendTransport_multipleDevicesReady(m_session->get_protocol(), deviceMask, noOfSamples, fs, nChannels);
      }

      void DeviceStopped(int deviceType)
      {
	ev_SendTransport_deviceStopped(m_session->get_protocol(), deviceType);
      }
      void DeviceListChanged()
      {
	ev_SendTransport_deviceListChanged(m_session->get_protocol());
      }
      void CurrentDeviceChanged(int deviceType, uint id, const Sid::String &guid, const Sid::String &name)
      {
	ev_SendTransport_currentDeviceChanged(m_session->get_protocol(), deviceType, id, guid, name);
      }
      void VolumeChanged(int deviceType, int range_min, int range_max, int volume, int boost)
      {
	ev_SendTransport_volumeChanged(m_session->get_protocol(), deviceType, range_min, range_max, volume, boost);
      }

    private:
      AVIpcServerSession<PCMServerCommandProcessor>* m_session;

  };

}

#endif

