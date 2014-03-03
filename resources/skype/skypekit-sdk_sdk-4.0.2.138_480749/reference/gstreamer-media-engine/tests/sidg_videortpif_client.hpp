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

#ifndef __sidg_videortpif_client_INCLUDED_HPP
#define __sidg_videortpif_client_INCLUDED_HPP

#include "SidVideoRTPInterface.hpp"
#include "SidAVClient.hpp"

namespace Sid
{

class VideoRTPIfClient :
    public SkypeVideoRTPInterface,
    public AVClient
{
public:
  Mutex m_Mutex;

  VideoRTPIfClient () : AVClient () {}
  ~VideoRTPIfClient () {}

  bool GetVersion (Sid::String &version);
  int GetLastError ();
  bool Init ();
  bool Uninit ();
  bool GetCodecCount (int &codec_count);
  bool GetCodec (int index, VideoCodec &codec);
  bool GetIntProperty (int prop_id, int index, int &value);
  bool SetIntProperty (int prop_id, int index, int value);
  bool GetStringProperty (int prop_id, int index, Sid::String &value);
  bool SetStringProperty (int prop_id, int index, const Sid::String &value);
  bool GetBinaryProperty (int prop_id, int index, Sid::Binary &value);
  bool SetBinaryProperty (int prop_id, int index, const Sid::Binary &value);
  bool RequestKeyFrame ();
  bool RequestRecoveryFrame (int reserved);
  bool SetBitrate (int bitrate);
  bool SetFramerate (int framerate_fps);
  bool SetMaxPacketSize (int max_packet_size);
  bool StartPlayback (int decoderfourcc, int width, int height);
  void SubmitRTPPacket (const Sid::Binary &data);
  bool StopPlayback ();
  bool StartRecording (int fourcc,
      int initial_max_packet_size,
      int initial_bitrate_bytes_sec,
      int width,
      int height);
  bool StopRecording ();
  bool CustomCommand (const Sid::String &cmd, Sid::String &result_str);
};

} /* namespace Sid */

#endif
