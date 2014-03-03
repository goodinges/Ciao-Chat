#ifndef sidg_videortpif_client_HPPINCLUDED
#define sidg_videortpif_client_HPPINCLUDED

#include "SidPlatform.hpp"
#include "SidAVClient.hpp"
#include "SidAVServer.hpp"
#include "SidVideoRTPInterface.hpp"
#include "sidg_msg_videortpif.hpp"

namespace Sid {
  class SkypeVideoRTPInterfaceClient: public SkypeVideoRTPInterface, public AVClient
  {

  public:

    SkypeVideoRTPInterfaceClient() : AVClient(Field::M_SkypeVideoRTPInterface_fields)
    {
    }

    bool GetInterfaceVersion(int& version);
    int GetLastError();
    bool Init();
    bool Uninit();
    bool GetCodecCount(int& codec_count);
    bool GetCodec(int index, SkypeVideoRTPInterface::VideoCodec& codec);
    bool GetSenderResolutionCount(int& resolution_count);
    bool GetSenderResolution(int index, SkypeVideoRTPInterface::FixedRes& resolution);
    bool GetReceiverResolutionCount(int& resolution_count);
    bool GetReceiverResolution(int index, SkypeVideoRTPInterface::ResCap& resolution);
    bool GetIntProperty(int prop_id, int index, int& value);
    bool SetIntProperty(int prop_id, int index, int value);
    bool GetStringProperty(int prop_id, int index, String& value);
    bool SetStringProperty(int prop_id, int index, const String& value);
    bool GetBinaryProperty(int prop_id, int index, Binary& value);
    bool SetBinaryProperty(int prop_id, int index, const Binary& value);
    bool RequestKeyFrame();
    bool RequestRecoveryFrame(int reserved);
    bool SetBitrate(int bitrate_bytes_sec);
    bool SetFramerate(int framerate_fps);
    bool SetMaxPacketSize(int max_packet_size);
    bool StartPlayback(int decoderfourcc, int width, int height);
    void RtpPacketReceived(const Binary& data);
    void FrameReceived(const Binary& data, int timestamp_ms);
    bool StopPlayback();
    bool StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps);
    bool StopRecording();
    bool CustomCommand(const String& command, String& result_str);
    bool VideoCallStart();
    bool VideoCallEnd();
    Mutex m_Mutex;
  };
}
#endif
