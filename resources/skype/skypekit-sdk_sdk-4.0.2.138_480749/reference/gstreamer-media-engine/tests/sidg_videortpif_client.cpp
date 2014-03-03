/*
 * Copyright (c) 2010 Collabora Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "sidg_videortpif_client.hpp"

#include "sidg_msg_videortpif.hpp"

namespace Sid
{

bool
VideoRTPIfClient::GetVersion (Sid::String &version)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=1;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetVersionResponse::BEGIN, &version, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

int
VideoRTPIfClient::GetLastError ()
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
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetLastErrorResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return 0;

  return rv;
}

bool
VideoRTPIfClient::Init ()
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=3;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgInitResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::Uninit ()
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=4;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgUninitResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::GetCodecCount (int &codec_count)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=5;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetCodecCountResponse::BEGIN,
      &codec_count, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::GetCodec (int index,
    VideoCodec &codec)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=6;
  uint rid;
  Protocol::Command cmd;
  int fourcc;
  int recording_capable;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetCodecRequest::BEGIN, &index, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetCodecResponse::BEGIN, &fourcc, &recording_capable,
      &rv, NULL) != Protocol::OK)
    return false;

  codec.fourcc = fourcc;
  codec.recording_capable = recording_capable;

  return rv;
}

bool
VideoRTPIfClient::GetIntProperty (int prop_id,
    int index,
    int &value)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=7;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetIntPropertyRequest::BEGIN,
      &prop_id, &index, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetIntPropertyResponse::BEGIN, &value, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::SetIntProperty (int prop_id,
    int index,
    int value)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=8;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetIntPropertyRequest::BEGIN,
      &prop_id, &index, &value, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetIntPropertyResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::GetStringProperty (int prop_id,
    int index,
    Sid::String &value)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=9;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetStringPropertyRequest::BEGIN,
      &prop_id, &index, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetStringPropertyResponse::BEGIN, &value, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::SetStringProperty (int prop_id,
    int index,
    const Sid::String &value)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=10;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetStringPropertyRequest::BEGIN,
      &prop_id, &index, &value, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetStringPropertyResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::GetBinaryProperty (int prop_id,
    int index,
    Sid::Binary &value)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=11;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetBinaryPropertyRequest::BEGIN,
      &prop_id, &index, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgGetBinaryPropertyResponse::BEGIN, &value, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::SetBinaryProperty (int prop_id,
    int index,
    const Sid::Binary &value)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=12;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetBinaryPropertyRequest::BEGIN,
      &prop_id, &index, &value, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetBinaryPropertyResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::RequestKeyFrame ()
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=13;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgRequestKeyFrameResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::RequestRecoveryFrame (int reserved)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=14;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgRequestRecoveryFrameRequest::BEGIN,
      &reserved, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgRequestRecoveryFrameResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::SetBitrate (int bitrate)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=15;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetBitrateRequest::BEGIN, &bitrate, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetBitrateResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::SetFramerate (int framerate_fps)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=16;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetFramerateRequest::BEGIN,
      &framerate_fps, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetFramerateResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::SetMaxPacketSize (int max_packet_size)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=17;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetMaxPacketSizeRequest::BEGIN,
      &max_packet_size, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSetMaxPacketSizeResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::StartPlayback (int decoderfourcc,
    int width,
    int height)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=18;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgStartPlaybackRequest::BEGIN,
      &decoderfourcc, &width, &height, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgStartPlaybackResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

void
VideoRTPIfClient::SubmitRTPPacket (const Sid::Binary &data)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=19;
  uint rid;

  m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgSubmitRTPPacketRequest::BEGIN, &data, NULL);
}

bool
VideoRTPIfClient::StopPlayback ()
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=20;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgStopPlaybackResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::StartRecording (int fourcc,
    int initial_max_packet_size,
    int initial_bitrate_bytes_sec,
    int width,
    int height)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=21;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgStartRecordingRequest::BEGIN, &fourcc,
      &initial_max_packet_size, &initial_bitrate_bytes_sec,
      &width, &height, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgStartRecordingResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::StopRecording ()
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=22;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call (NULL, modid, actionid, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgStopRecordingResponse::BEGIN, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

bool
VideoRTPIfClient::CustomCommand (const Sid::String &cmd_str,
    Sid::String &result_str)
{
  Mutex::ScopedLock l(m_Mutex);
  uint modid=0;
  uint actionid=23;
  uint rid;
  Protocol::Command cmd;
  bool rv;

  if (m_protocol.wr_call_lst (NULL, modid, actionid,
      rid, &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgCustomCommandRequest::BEGIN, &cmd_str, NULL) != Protocol::OK)
    return false;

  if (m_protocol.rd_response_or_event(NULL, cmd) != Protocol::OK)
    return false;

  if (m_protocol.rd_value(NULL, rid) != Protocol::OK)
    return false;

  if (m_protocol.rd_parms_lst(NULL,
      &Field::M_SkypeVideoRTPInterface_fields[0],
      MsgCustomCommandResponse::BEGIN, &result_str, &rv, NULL) != Protocol::OK)
    return false;

  return rv;
}

} /* namespace Sid */
