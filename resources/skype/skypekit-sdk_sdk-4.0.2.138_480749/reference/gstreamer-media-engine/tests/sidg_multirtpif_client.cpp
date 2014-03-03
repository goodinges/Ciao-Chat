#include "sidg_multirtpif_client.hpp"

#include "sidg_msg_multirtpif.hpp"

namespace Sid
{

int
SkypeMultiRTPInterfaceClient::GetVersion (Sid::String &version)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=1;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgGetVersionResponse::BEGIN, &version, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::GetLastError ()
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=2;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[1],
      MsgGetLastErrorResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::Init ()
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=3;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgGetLastErrorResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::Uninit ()
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=4;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgGetLastErrorResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::CreateChannel (uint call_member_id,
    uint conversation_id,
    int &channel)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=10;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgCreateChannelRequest::BEGIN, &call_member_id,
      &conversation_id, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgCreateChannelResponse::BEGIN, &channel, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::DeleteChannel (int channel_id)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=11;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgDeleteChannelRequest::BEGIN, &channel_id, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgDeleteChannelResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::ChangeConversationId (int channel_id,
    uint conversation_id,
    uint call_member_id)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=12;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgChangeConversationIdRequest::BEGIN, &channel_id,
      &conversation_id, &call_member_id, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgChangeConversationIdResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

uint
SkypeMultiRTPInterfaceClient::GetCodec (uint id, Codec &codec)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=20;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgGetCodecRequest::BEGIN, &id, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgGetCodecResponse::BEGIN, &codec.Name, &codec.PayloadType,
      &codec.Freq, &codec.Channels, &codec.SupportedPacketSizes,
      &codec.DefaultPacketSize, &codec.MinBitrate, &codec.MaxBitrate,
      &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::GetCodecCount (uint &count)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=21;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgGetCodecCountResponse::BEGIN, &count, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::SetCodec (int channel_id,
    const Sid::String &name,
    int packet_size,
    int bit_rate)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=22;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgSetCodecRequest::BEGIN, &channel_id, &name,
      &packet_size, &bit_rate, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgSetCodecResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::StartPlayback (int channel_id)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=30;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgStartPlaybackRequest::BEGIN,
      &channel_id, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgStartPlaybackResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::StopPlayback (int channel_id)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=31;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgStopPlaybackRequest::BEGIN,
      &channel_id, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgStopPlaybackResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::StartRecording (int channel_id)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=32;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgStartRecordingRequest::BEGIN,
      &channel_id, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgStartRecordingResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::StopRecording (int channel_id)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=33;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgStopRecordingRequest::BEGIN,
      &channel_id, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgStopRecordingResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

void
SkypeMultiRTPInterfaceClient::SubmitRTPPacket (int channel_id,
    const Sid::Binary &data)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=40;
  uint rid;

  m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgSubmitRTPPacketRequest::BEGIN,
      &channel_id, &data, NULL);
}

int
SkypeMultiRTPInterfaceClient::SendDTMF (int channel_id,
    int eventnr,
    int rtp_type)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=41;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgSendDTMFRequest::BEGIN,
      &channel_id, &eventnr, &rtp_type, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgSendDTMFResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::SetOutputAmplification (int channel_id,
    int scale)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=50;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgSetOutputAmplificationRequest::BEGIN,
      &channel_id, &scale, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgSetOutputAmplificationResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

int
SkypeMultiRTPInterfaceClient::CustomCommand (const Sid::String &cmd_str,
    Sid::String &result_str)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=51;
  uint rid;
  Protocol::Command cmd;
  int rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgCustomCommandRequest::BEGIN, &cmd_str, NULL) != Protocol::OK)
    return 0;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return 0;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return 0;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeMultiRTPInterface_fields[0],
      MsgCustomCommandResponse::BEGIN, &result_str,
      &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

} /* namespace Sid */
