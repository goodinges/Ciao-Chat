#ifndef __sidg_multirtpif_client_INCLUDED_HPP
#define __sidg_multirtpif_client_INCLUDED_HPP

#include "SidMultiRTPInterface.hpp"
#include "SidAVClient.hpp"

namespace Sid
{

class SkypeMultiRTPInterfaceClient :
    public SkypeMultiRTPInterface,
    public AVClient
{
public:
  Mutex m_Mutex;

  SkypeMultiRTPInterfaceClient () : AVClient () {}
  ~SkypeMultiRTPInterfaceClient () {}

  int GetVersion (Sid::String &version);
  int GetLastError ();
  int Init ();
  int Uninit ();
  int CreateChannel (uint call_member_id, uint conversation_id, int &channel);
  int DeleteChannel (int channel_id);
  int ChangeConversationId (int channel_id,
      uint conversation_id, uint call_member_id);
  uint GetCodec (uint id, Codec &codec);
  int GetCodecCount (uint &count);
  int SetCodec (int channel_id, const Sid::String &name,
      int packet_size, int bit_rate);
  int StartPlayback (int channel_id);
  int StopPlayback (int channel_id);
  int StartRecording (int channel_id);
  int StopRecording (int channel_id);
  void SubmitRTPPacket (int channel_id, const Sid::Binary &data);
  int SendDTMF (int channel_id, int eventnr, int rtp_type);
  int SetOutputAmplification (int channel_id, int scale);
  int CustomCommand (const Sid::String &cmd, Sid::String &result_str);
};

} /* namespace Sid */

#endif
