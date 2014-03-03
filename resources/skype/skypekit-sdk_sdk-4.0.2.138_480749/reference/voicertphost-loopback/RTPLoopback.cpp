#include "SidMultiRTPInterface.hpp"
#include "SidDebugging.hpp"

class MyCodec: public SkypeMultiRTPInterface::Codec
{
  public:
  MyCodec (
    const char* name,
    int payloadType,
    int freq,
    int channels,
    const int* supportedPacketSizes,
    int defaultPacketSize,
    int minBitrate,
    int maxBitrate,
    int maxDesiredPacketLoss
    )
    {
      Name = name;
      PayloadType = payloadType;
      Freq = freq; 
      Channels = channels;
      for(int i = 0; i < 6; i++)
	SupportedPacketSizes.append(supportedPacketSizes[i]);
      DefaultPacketSize = defaultPacketSize;
      MinBitrate = minBitrate;
      MaxBitrate = maxBitrate;
      MaxDesiredPacketLoss = maxDesiredPacketLoss;
    }
};

// these are the packetsizes which are supported for sending
// in case of SILK codec 80 ms and 100 ms packetsizes must also be supported for receiving
const int supported_SILK_packet_sizes[6] = {20,40,60,0,0,0};
const int supported_NB_codec_packet_sizes[6] = {20,30,40,50,60,0};

const MyCodec  codecs[7] = { 
    MyCodec ("SILK_V3",    100, 24000, 1, supported_SILK_packet_sizes, 20,6000,40000, 100),
    MyCodec ("SILK_WB_V3", 104,16000,1, supported_SILK_packet_sizes, 20,6000,30000, 100),
    MyCodec ("SILK_MB_V3", 102,12000,1, supported_SILK_packet_sizes, 20,6000,25000, 100),
    MyCodec ("SILK_NB_V3", 103,8000,1, supported_SILK_packet_sizes, 20,6000,20000, 100),
    MyCodec ("G729",       18,8000,1, supported_NB_codec_packet_sizes, 20,8000,8000, 1),
    MyCodec ("PCMU",       0,8000,1, supported_NB_codec_packet_sizes, 20,64000,64000, 1),
    MyCodec ("PCMA",       8,8000,1, supported_NB_codec_packet_sizes, 20,64000,64000, 1)
};



struct RTPLoopback: public SkypeMultiRTPInterface {
  private:
	SkypeMultiRTPCallbackInterface* m_callback;
	int m_channel_created;
  public:
 	RTPLoopback(SkypeMultiRTPCallbackInterface* callback):m_callback(callback), m_channel_created(0)
	{
	}
	~RTPLoopback() {}
	

	int  GetVersion(Sid::String & version)
	{
	  static Sid::String m_version = "RTPLoopback 0.1";
	  version = m_version;
	  return 1;
	}
	
	int  GetLastError()
	{
	    return 0;
	}
	
	int  Init()
	{
	    return 1;
	}
	
	int  Uninit()
	{
	    return 1;
	}
	
	int  CreateChannel(uint call_member_id, uint conversation_id, int &channel)
	{
	    if(!m_channel_created)
	    {
	      SID_INFO("CreateChannel: new channel 0 created");
	      channel = 0;
	      return 1;
	    }
	    return 0;
	}
	
	int  DeleteChannel(int channel_id)
	{
	    if(!channel_id)
	    {
	      SID_INFO("DeleteChannel: channel %d deleted", channel_id);
	      m_channel_created = 0;
	      return 1;
	    }
	    return 0;
	}
	
	int  ChangeConversationId(int channel_id, uint conversation_id, uint call_member_id)
	{
	    SID_INFO("ChangeConversationId: new conversation_id %d, call_member_id %d", conversation_id, call_member_id);
	    return 1;
	}
	
	uint  GetCodec(uint id, Codec & c)
	{
	  if(id >= 7)
	    return 0;
	  c = codecs[id];
	  return 1;
	}
	
	int  GetCodecCount(uint &count)
	{
	    count = 7;
	    return 1;
	}
	
	int  SetSendCodec(int channel_id, const Sid::String& Name, int PacketSize, int BitRate, int PayloadType)
	{
	    SID_INFO("SetSendCodec: name %s(%d), PacketSize %d, Bitrate %d", Name.data(), PayloadType, PacketSize, BitRate);
	    return 1;
	}
	
	void InformSendPacketLoss(int channel_id, uint packetLossBeforeFEC, uint packetLossAfterFEC, uint jitterBeforeFEC, uint jitterAfterFEC)
	{
	    SID_INFO("InformSendPacketLoss: beforeFEC %d, afterFEC %d, jitterBeforeFEC %d, jitterAfterFEC %d", packetLossBeforeFEC, packetLossAfterFEC, jitterBeforeFEC, jitterAfterFEC);
	}

	
	int  StartPlayback(int channel_id)
	{
	  SID_INFO("StartPlayback: channel_id %d", channel_id);
	  return 1;
	}
	
	int  StopPlayback(int channel_id)
	{
	  SID_INFO("StopPlayback: channel_id %d", channel_id);
	  return 1;
	}
	
	int  StartRecording(int channel_id)
	{
	   SID_INFO("StartRecording: channel_id %d", channel_id);
	   return 1;
	}
	
	int  StopRecording(int channel_id)
	{
	  SID_INFO("StopRecording: channel_id %d", channel_id);
	  return 1;
	}
	
	void  SubmitRTPPacket(int channel_id, const Sid::Binary& data)
	{
	  if(m_callback)
 	    m_callback->SendRTPPacket(channel_id, data);
	}
	
	int  SendDTMF(int channel_id, int eventnr, int rtp_type)
	{
	  SID_INFO("SendDTMF: channel_id %d, event %d, rtp_type %d", channel_id, eventnr, rtp_type);
	  return 1;
	}
	
	int  SetOutputAmplification(int channel_id, int scale)
	{
	  SID_INFO("SetOutputAmplification: channel_id %d, scale %d", channel_id, scale);
	  return 1;
	}
	
	int  CustomCommand(const Sid::String& cmd, Sid::String & result_str)
	{
	    SID_INFO("CustomCommand: %s", cmd.data());
	    result_str = Sid::String("OK");
	    return 1;
	}
	
};
SkypeMultiRTPInterface * SkypeMultiRTPInterfaceGet(SkypeMultiRTPCallbackInterface* callback)
{
  return new RTPLoopback(callback);
}

void SkypeMultiRTPInterfaceRelease(SkypeMultiRTPInterface* rtpi)
{
  delete rtpi;
}
