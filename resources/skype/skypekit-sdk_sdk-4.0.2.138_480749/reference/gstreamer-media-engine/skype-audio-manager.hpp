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

#ifndef __SKYPE_AUDIO_MANAGER_HPP__
#define __SKYPE_AUDIO_MANAGER_HPP__

#include "SidMultiRTPInterface.hpp"
#include <glib.h>
#include <gst/farsight/fs-element-added-notifier.h>
#include <gst/gst.h>

#include "skype-audio-stream.h"

class SkypeAudioManager;

typedef void (*SkypeAudioManagerNewStreamCallback)
    (SkypeAudioManager *, SkypeAudioStream *, gpointer);

class SkypeAudioManager : public SkypeMultiRTPInterface
{
public:
  SkypeAudioManager (SkypeMultiRTPCallbackInterface *callback,
      GMainContext *context);
  ~SkypeAudioManager ();

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
  int SetSendCodec (int channel_id, const Sid::String &name,
      int packet_size, int bit_rate, int PayloadType);
  int StartPlayback (int channel_id);
  int StopPlayback (int channel_id);
  int StartRecording (int channel_id);
  int StopRecording (int channel_id);
  void SubmitRTPPacket (int channel_id, const Sid::Binary &data);
  int SendDTMF (int channel_id, int eventnr, int rtp_type);
  int SetOutputAmplification (int channel_id, int scale);
  int CustomCommand (const Sid::String &cmd, Sid::String &result_str);

  void SetNewStreamCallback (SkypeAudioManagerNewStreamCallback callback,
      gpointer user_data);

  void SetSourceName (const gchar *source_name);
  void SetSourceDevice (const gchar *source_device);
  void SetSourcePipeline (const gchar *source_pipeline);
  void SetSinkName (const gchar *sink_name);
  void SetSinkDevice (const gchar *sink_device);
  void SetSinkPipeline (const gchar *sink_pipeline);

private:
  guint m_next_channel_id;

  GList *m_codecs;
  GHashTable *m_streams;

  GstElement *m_pipeline;
  GstElement *m_source;
  GstElement *m_sink;
  FsElementAddedNotifier *m_notifier;

  SkypeMultiRTPCallbackInterface *m_callback;
  GMainContext *m_context;

  gchar *m_source_name;
  gchar *m_source_device;
  gchar *m_source_pipeline;
  gchar *m_sink_name;
  gchar *m_sink_device;
  gchar *m_sink_pipeline;

  SkypeAudioManagerNewStreamCallback m_new_stream_cb;
  gpointer m_new_stream_user_data;

  static void gst_message_handler_cb (GstBus *bus,
      GstMessage *message, gpointer user_data);
};

#endif /* __SKYPE_AUDIO_MANAGER_HPP__ */
