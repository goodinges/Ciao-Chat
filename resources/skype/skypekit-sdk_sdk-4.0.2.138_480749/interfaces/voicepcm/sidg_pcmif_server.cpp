#include "SidProtocolServerInterface.hpp"
#include "SidField.hpp"
#include "sidg_msg_pcmif.hpp"

#include "sidg_pcmif_server.hpp"
#include "SidPCMInterface.hpp"

namespace Sid {
  using namespace SkypePCMInterfaceMsgs;
    Protocol::Status SkypePCMInterfaceServer::ProcessCall(int rid, int method)
    {
    switch (method)
    {

      case 0:
      {
        MsgInitResponse* rsp = &msgs.m_MsgInitResponse;
        if (m_protocol.rd_parms(NULL) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->Init();
        if (m_protocol.wr_response(NULL, rid, rsp, MsgInitResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 1:
      {
        MsgStartRequest* req = &msgs.m_MsgStartRequest;
        MsgStartResponse* rsp = &msgs.m_MsgStartResponse;
        if (m_protocol.rd_parms(NULL, req, MsgStartRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->Start(req->deviceType);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStartResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 3:
      {
        MsgStopRequest* req = &msgs.m_MsgStopRequest;
        MsgStopResponse* rsp = &msgs.m_MsgStopResponse;
        if (m_protocol.rd_parms(NULL, req, MsgStopRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->Stop(req->deviceType);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgStopResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 20:
      {
        MsgGetDefaultDeviceRequest* req = &msgs.m_MsgGetDefaultDeviceRequest;
        MsgGetDefaultDeviceResponse* rsp = &msgs.m_MsgGetDefaultDeviceResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetDefaultDeviceRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->GetDefaultDevice(req->deviceType, rsp->guid, rsp->name, rsp->productID);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetDefaultDeviceResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 21:
      {
        MsgUseDefaultDeviceRequest* req = &msgs.m_MsgUseDefaultDeviceRequest;
        MsgUseDefaultDeviceResponse* rsp = &msgs.m_MsgUseDefaultDeviceResponse;
        if (m_protocol.rd_parms(NULL, req, MsgUseDefaultDeviceRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->UseDefaultDevice(req->deviceType);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgUseDefaultDeviceResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 22:
      {
        MsgGetCurrentDeviceRequest* req = &msgs.m_MsgGetCurrentDeviceRequest;
        MsgGetCurrentDeviceResponse* rsp = &msgs.m_MsgGetCurrentDeviceResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetCurrentDeviceRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->GetCurrentDevice(req->deviceType, rsp->guid, rsp->name, rsp->productID);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetCurrentDeviceResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 23:
      {
        MsgUseDeviceRequest* req = &msgs.m_MsgUseDeviceRequest;
        MsgUseDeviceResponse* rsp = &msgs.m_MsgUseDeviceResponse;
        if (m_protocol.rd_parms(NULL, req, MsgUseDeviceRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->UseDevice(req->deviceType, req->guid);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgUseDeviceResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 24:
      {
        MsgGetDeviceCountRequest* req = &msgs.m_MsgGetDeviceCountRequest;
        MsgGetDeviceCountResponse* rsp = &msgs.m_MsgGetDeviceCountResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetDeviceCountRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->GetDeviceCount(req->deviceType, rsp->count);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetDeviceCountResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 25:
      {
        MsgGetDevicesRequest* req = &msgs.m_MsgGetDevicesRequest;
        MsgGetDevicesResponse* rsp = &msgs.m_MsgGetDevicesResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetDevicesRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->GetDevices(req->deviceType, rsp->guid, rsp->name, rsp->productID);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetDevicesResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 30:
      {
        MsgGetVolumeParametersRequest* req = &msgs.m_MsgGetVolumeParametersRequest;
        MsgGetVolumeParametersResponse* rsp = &msgs.m_MsgGetVolumeParametersResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetVolumeParametersRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->GetVolumeParameters(req->deviceType, rsp->range_min, rsp->range_max, rsp->volume, rsp->boost);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetVolumeParametersResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 31:
      {
        MsgSetVolumeRequest* req = &msgs.m_MsgSetVolumeRequest;
        MsgSetVolumeResponse* rsp = &msgs.m_MsgSetVolumeResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetVolumeRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->SetVolume(req->deviceType, req->volume);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetVolumeResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 34:
      {
        MsgSetInputBoostRequest* req = &msgs.m_MsgSetInputBoostRequest;
        MsgSetInputBoostResponse* rsp = &msgs.m_MsgSetInputBoostResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetInputBoostRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->SetInputBoost(req->boost);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetInputBoostResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 35:
      {
        MsgGetMuteRequest* req = &msgs.m_MsgGetMuteRequest;
        MsgGetMuteResponse* rsp = &msgs.m_MsgGetMuteResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetMuteRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->GetMute(req->deviceType, rsp->muted);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetMuteResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 36:
      {
        MsgSetMuteRequest* req = &msgs.m_MsgSetMuteRequest;
        MsgSetMuteResponse* rsp = &msgs.m_MsgSetMuteResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetMuteRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->SetMute(req->deviceType, req->mute);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetMuteResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 40:
      {
        MsgGetSampleRateCountRequest* req = &msgs.m_MsgGetSampleRateCountRequest;
        MsgGetSampleRateCountResponse* rsp = &msgs.m_MsgGetSampleRateCountResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetSampleRateCountRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->GetSampleRateCount(req->deviceType, rsp->count);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetSampleRateCountResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 41:
      {
        MsgGetSupportedSampleRatesRequest* req = &msgs.m_MsgGetSupportedSampleRatesRequest;
        MsgGetSupportedSampleRatesResponse* rsp = &msgs.m_MsgGetSupportedSampleRatesResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetSupportedSampleRatesRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->GetSupportedSampleRates(req->deviceType, rsp->sampleRates);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetSupportedSampleRatesResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 42:
      {
        MsgGetCurrentSampleRateRequest* req = &msgs.m_MsgGetCurrentSampleRateRequest;
        MsgGetCurrentSampleRateResponse* rsp = &msgs.m_MsgGetCurrentSampleRateResponse;
        if (m_protocol.rd_parms(NULL, req, MsgGetCurrentSampleRateRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->GetCurrentSampleRate(req->deviceType, rsp->sampleRate);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgGetCurrentSampleRateResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 43:
      {
        MsgSetSampleRateRequest* req = &msgs.m_MsgSetSampleRateRequest;
        MsgSetSampleRateResponse* rsp = &msgs.m_MsgSetSampleRateResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetSampleRateRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->SetSampleRate(req->deviceType, req->sampleRate);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetSampleRateResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 50:
      {
        MsgSetNumberOfChannelsRequest* req = &msgs.m_MsgSetNumberOfChannelsRequest;
        MsgSetNumberOfChannelsResponse* rsp = &msgs.m_MsgSetNumberOfChannelsResponse;
        if (m_protocol.rd_parms(NULL, req, MsgSetNumberOfChannelsRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->SetNumberOfChannels(req->deviceType, req->numberOfChannels);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgSetNumberOfChannelsResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
      case 60:
      {
        MsgCustomCommandRequest* req = &msgs.m_MsgCustomCommandRequest;
        MsgCustomCommandResponse* rsp = &msgs.m_MsgCustomCommandResponse;
        if (m_protocol.rd_parms(NULL, req, MsgCustomCommandRequest::BEGIN) != Protocol::OK) return Protocol::ERR_DECODE;
          SkypePCMInterface* __if = static_cast<SkypePCMInterface*>(get_if());
          rsp->rv = __if->CustomCommand(req->command, rsp->response);
        if (m_protocol.wr_response(NULL, rid, rsp, MsgCustomCommandResponse::BEGIN) != Protocol::OK) return Protocol::ERR_ENCODE;
        return Protocol::OK;
      }
    default:
      return Protocol::ERR_DECODE;
  }
  }

} // namespace Sid
