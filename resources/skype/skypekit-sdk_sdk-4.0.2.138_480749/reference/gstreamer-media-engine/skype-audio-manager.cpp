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


#include "skype-audio-manager.hpp"

#include <gst/gst.h>
#include <gst/farsight/fs-conference-iface.h>
#include <gst/farsight/fs-session.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "skype-utils.h"

SkypeAudioManager::SkypeAudioManager (
    SkypeMultiRTPCallbackInterface *callback,
    GMainContext *context = NULL) :
    m_next_channel_id (1), m_codecs (NULL),
    m_streams (NULL), m_pipeline (NULL), m_notifier (NULL),
    m_callback (callback), m_context (context),
    m_source_name (NULL), m_source_device (NULL), m_source_pipeline (NULL),
    m_sink_name (NULL), m_sink_device (NULL), m_sink_pipeline (NULL),
    m_new_stream_cb (NULL), m_new_stream_user_data (NULL)
{
  m_streams = g_hash_table_new_full (g_direct_hash, g_direct_equal,
      NULL, g_object_unref);
}

SkypeAudioManager::~SkypeAudioManager ()
{
  g_hash_table_destroy (m_streams);

  if (m_codecs != NULL)
    {
      fs_codec_list_destroy (m_codecs);
      m_codecs = NULL;
    }

  g_free (m_source_name);
  g_free (m_source_device);
  g_free (m_source_pipeline);
  g_free (m_sink_name);
  g_free (m_sink_device);
  g_free (m_sink_pipeline);
}

int
SkypeAudioManager::GetVersion (Sid::String &version)
{
  version = "Skype RTP Farstream Audio Host 0.1";
  return 1;
}

int
SkypeAudioManager::GetLastError ()
{
  return 0;
}

void
SkypeAudioManager::gst_message_handler_cb (GstBus *bus,
    GstMessage *message,
    gpointer user_data)
{
  SkypeAudioManager *manager = static_cast<SkypeAudioManager *> (user_data);
  GHashTableIter iter;
  gpointer stream;

  /* Iterate streams to have them handle the messages */
  g_hash_table_iter_init (&iter, manager->m_streams);
  while (g_hash_table_iter_next (&iter, NULL, &stream))
    {
      if (skype_base_stream_handle_message (
          SKYPE_BASE_STREAM (stream), message))
        return;
    }

  switch (GST_MESSAGE_TYPE (message))
    {
      case GST_MESSAGE_ERROR:
        {
          GError *e;
          gchar *debug;
          gst_message_parse_error (message, &e, &debug);
          g_warning ("GST_MESSAGE_ERROR: %s: %s", e->message, debug);
          g_free (debug);
          g_error_free (e);
          break;
        }
      case GST_MESSAGE_WARNING:
        {
          GError *e;
          gchar *debug;
          gst_message_parse_warning (message, &e, &debug);
          /* TODO: Set this back to g_warning and use
           * g_test_log_set_fatal_handler to catch any ignorable warnings */
          g_debug ("GST_MESSAGE_WARNING: %s: %s", e->message, debug);
          g_free (debug);
          g_error_free (e);
          break;
        }
      default:
        break;
    }
}

int
SkypeAudioManager::Init ()
{
  gchar *path;
  GstBus *bus;
  GSource *source;

  path = skype_get_codec_preferences_path ();
  m_codecs = skype_get_supported_codecs_list (FS_MEDIA_TYPE_AUDIO, path);
  g_free (path);

  if (m_codecs == NULL)
    return 0;

  m_pipeline = gst_pipeline_new (NULL);

  if (m_pipeline == NULL)
    {
      g_warning ("Error creating pipeline");
      goto error;
    }
  else
    {
      GError *e = NULL;

      m_notifier = fs_element_added_notifier_new ();
      path = skype_get_element_preferences_path ();

      if (!fs_element_added_notifier_set_properties_from_file (m_notifier,
          path, &e))
        {
          g_warning ("Unable to load element properties from file: %s",
              e->message);
          g_error_free (e);
          goto error;
        }
      else
        {
          fs_element_added_notifier_add (m_notifier, GST_BIN (m_pipeline));
        }

      g_free (path);
    }

  bus = gst_pipeline_get_bus (GST_PIPELINE (m_pipeline));

  if (bus == NULL)
    {
      g_warning ("Error fetching bus");
      gst_object_unref (m_pipeline);
      m_pipeline = NULL;
      return NULL;
    }

  source = gst_bus_create_watch (bus);
  g_source_set_callback (source,
      (GSourceFunc)gst_bus_async_signal_func, NULL, NULL);
  g_source_attach (source, m_context);
  g_source_unref (source);

  g_signal_connect (bus, "message",
      G_CALLBACK (SkypeAudioManager::gst_message_handler_cb), this);

  gst_object_unref (bus);

  m_source = gst_element_factory_make ("fsuaudiosrc", NULL);

  if (m_source == NULL)
    {
      g_warning ("Error creating audio source");
      goto error;
    }

  m_sink = gst_element_factory_make ("fsuaudiosink", NULL);

  if (m_sink == NULL)
    {
      g_warning ("Error creating audio sink");
      goto error;
    }

  g_object_set (m_source,
      "source-name", m_source_name,
      "source-device", m_source_device,
      "source-pipeline", m_source_pipeline,
      NULL);

  g_object_set (m_sink,
      "sink-name", m_sink_name,
      "sink-device", m_sink_device,
      "sink-pipeline", m_sink_pipeline,
      NULL);

  if (!gst_bin_add (GST_BIN (m_pipeline), m_source))
    {
      g_warning ("Error adding audio source to pipeline");
      goto error;
    }

  if (!gst_bin_add (GST_BIN (m_pipeline), m_sink))
    {
      g_warning ("Error adding audio sink to pipeline");
      goto error;
    }

  if (gst_element_set_state (m_pipeline, GST_STATE_PLAYING)
      == GST_STATE_CHANGE_FAILURE)
    {
      g_warning ("Error starting pipeline");
      goto error;
    }

  return 1;

error:
  if (m_source != NULL)
    {
      gst_object_unref (m_source);
      m_source = NULL;
    }

  if (m_sink != NULL)
    {
      gst_object_unref (m_sink);
      m_sink = NULL;
    }

  if (m_pipeline != NULL)
    {
      gst_element_set_state (m_pipeline, GST_STATE_NULL);
      gst_object_unref (m_pipeline);
      m_pipeline = NULL;
    }

  return 0;
}

int
SkypeAudioManager::Uninit ()
{
  g_hash_table_remove_all (m_streams);

  if (m_notifier != NULL)
    {
      g_object_unref (m_notifier);
      m_notifier = NULL;
    }

  if (m_source != NULL)
    {
      gst_object_unref (m_source);
      m_source = NULL;
    }

  if (m_sink != NULL)
    {
      gst_object_unref (m_sink);
      m_sink = NULL;
    }

  if (m_pipeline != NULL)
    {
      gst_element_set_state (m_pipeline, GST_STATE_NULL);
      gst_object_unref (m_pipeline);
      m_pipeline = NULL;
    }

  return 1;
}

static void
received_data_callback (SkypeBaseStream *stream,
    gsize len,
    const gchar *buf,
    gpointer data)
{
  SkypeMultiRTPCallbackInterface *__if;
  Sid::Binary binary;
  guint id;

  g_object_get (stream, "channel-id", &id, NULL);

  __if = static_cast<SkypeMultiRTPCallbackInterface *>(data);

  binary.set (buf, len);

  __if->SendRTPPacket (id, binary);
}

int
SkypeAudioManager::CreateChannel (uint call_member_id,
    uint conversation_id, int &channel)
{
  SkypeBaseStream *stream;

  g_debug ("CreateChannel: call_member_id: %u, conversation_id: %u",
      call_member_id, conversation_id);

  stream = (SkypeBaseStream *)skype_audio_stream_new_full (
      m_next_channel_id, m_context, m_pipeline, m_source, m_sink);

  g_object_set (stream,
      "conversation-id", conversation_id,
      "call-member-id", call_member_id,
      NULL);

  g_signal_connect (stream, "buffer-prepared",
      G_CALLBACK (received_data_callback), m_callback);

  if (!skype_base_stream_init_stream (stream))
    {
      g_warning ("CreateChannel: Error initializing audio stream");
      return 0;
    }

  g_hash_table_insert (m_streams, GINT_TO_POINTER (m_next_channel_id), stream);

  channel = m_next_channel_id;
  ++m_next_channel_id;

  if (m_new_stream_cb != NULL)
    (*m_new_stream_cb)(this, SKYPE_AUDIO_STREAM (stream),
        m_new_stream_user_data);

  g_debug ("CreateChannel: returning channel_id: %d", channel);

  return 1;
}

int
SkypeAudioManager::DeleteChannel (int channel_id)
{
  g_debug ("DeleteChannel: channel_id: %d", channel_id);

  if (!g_hash_table_remove (m_streams, GINT_TO_POINTER (channel_id)))
    {
      g_warning ("Error deleting audio stream: it doesn't exist.");
      return 0;
    }

  return 1;
}

int
SkypeAudioManager::ChangeConversationId (int channel_id,
    uint conversation_id, uint call_member_id)
{
  SkypeAudioStream *stream = SKYPE_AUDIO_STREAM (
      g_hash_table_lookup (m_streams, GINT_TO_POINTER (channel_id)));

  g_debug ("ChangeConversationId: channel_id: %d, "
      "conversation_id: %u, call_member_id %u",
      channel_id, conversation_id, call_member_id);

  if (stream == NULL)
    {
      g_warning ("ChangeConversationId: Audio channel %d not found", channel_id);
      return 0;
    }

  g_object_set (stream,
      "conversation-id", conversation_id,
      "call-member-id", call_member_id,
      NULL);

  return 1;
}

uint
SkypeAudioManager::GetCodec (uint id, Codec &codec)
{
  FsCodec *fscodec = NULL;

  g_debug ("GetCodec: id: %u", id);

  if (m_codecs == NULL)
    {
      g_warning ("SkypeAudioManager::GetCodec: Codecs not ready");
      return 0;
    }

  fscodec = (FsCodec *)g_list_nth_data (m_codecs, id);

  if (fscodec == NULL)
    {
      g_warning ("SkypeAudioManager::GetCodec: Codec not found: %d", id);
    }

  codec.Name = Sid::String (fscodec->encoding_name);
  codec.PayloadType = fscodec->id;
  codec.Freq = fscodec->clock_rate;
  codec.Channels = fscodec->channels;

  /* XXX: Default values picked at random. Need to be chosen more better. */
  codec.SupportedPacketSizes.append (20);
  codec.SupportedPacketSizes.append (30);
  codec.DefaultPacketSize = 20;
  codec.MinBitrate = 8000;
  codec.MaxBitrate = 8000;

  g_debug ("GetCodec: Returning codec: %s:%u", fscodec->encoding_name, fscodec->clock_rate);

  return 1;
}

int
SkypeAudioManager::GetCodecCount (uint &count)
{
  if (m_codecs == NULL)
    {
      g_warning ("SkypeAudioManager::GetCodecCount: Codecs not ready");
      return 0;
    }

  count = g_list_length (m_codecs);
  g_debug ("GetCodecCount: returning count: %u", count);

  return 1;
}

int
SkypeAudioManager::SetSendCodec (int channel_id, const Sid::String &name,
    int packet_size, int bit_rate, int PayloadType)
{
  SkypeAudioStream *stream = SKYPE_AUDIO_STREAM (
      g_hash_table_lookup (m_streams, GINT_TO_POINTER (channel_id)));
  GList *iter;
  FsCodec *codec;
  int bitrate = 0;
  int ptime;

  if (stream == NULL)
    {
      g_warning ("SetCodec: Audio stream %d not found", channel_id);
      return 0;
    }

  /* Verify that the codec really is changing in some way */
  g_object_get (stream,
      "codec", &codec,
      "ptime", &ptime,
      NULL);

  if (bit_rate != 0)
    g_object_get (stream, "bitrate", &bitrate, NULL);

  if (codec != NULL && bitrate == bit_rate && ptime == packet_size &&
      !g_strcmp0 (codec->encoding_name, name.data()))
    {
      /* No change to the codec. Skipping. */
      fs_codec_destroy (codec);
      return 1;
    }

  fs_codec_destroy (codec);

  g_debug ("SetCodec: channel_id: %d, name: %s, packet_size: %d, bitrate: %d",
      channel_id, name.data(), packet_size, bit_rate);

  for (iter = m_codecs; iter != NULL; iter = g_list_next (iter))
    {
      codec = (FsCodec *)iter->data;

      if (!g_strcmp0 (codec->encoding_name, name.data()))
        {
          g_object_set (stream,
              "codec", codec,
              "bitrate", bit_rate,
              "ptime", packet_size,
              NULL);

          return 1;
        }
    }

  return 0;
}

int
SkypeAudioManager::StartPlayback (int channel_id)
{
  SkypeBaseStream *stream = SKYPE_BASE_STREAM (
      g_hash_table_lookup (m_streams, GINT_TO_POINTER (channel_id)));

  g_debug ("StartPlayback: channel_id: %d", channel_id);

  if (stream == NULL)
    {
      g_warning ("StartPlayback: Audio stream %d not found", channel_id);
      return 0;
    }

  if (!skype_base_stream_play (stream, TRUE))
    {
      g_warning ("Error starting audio playback");
      return 0;
    }

  return 1;
}

int
SkypeAudioManager::StopPlayback (int channel_id)
{
  SkypeBaseStream *stream = SKYPE_BASE_STREAM (
      g_hash_table_lookup (m_streams, GINT_TO_POINTER (channel_id)));

  g_debug ("StopPlayback: channel_id: %d", channel_id);

  if (stream == NULL)
    {
      g_warning ("StopPlayback: Audio stream %d not found", channel_id);
      return 0;
    }

  if (!skype_base_stream_play (stream, FALSE))
    {
      g_warning ("Error stopping audio playback");
      return 0;
    }

  return 1;
}

int
SkypeAudioManager::StartRecording (int channel_id)
{
  SkypeBaseStream *stream = SKYPE_BASE_STREAM (
      g_hash_table_lookup (m_streams, GINT_TO_POINTER (channel_id)));

  g_debug ("StartRecording: channel_id: %d", channel_id);

  if (stream == NULL)
    {
      g_warning ("StartRecording: Audio stream %d not found", channel_id);
      return 0;
    }

  if (!skype_base_stream_record (stream, TRUE))
    {
      g_warning ("Error starting audio recording");
      return 0;
    }

  return 1;
}

int
SkypeAudioManager::StopRecording (int channel_id)
{
  SkypeBaseStream *stream = SKYPE_BASE_STREAM (
      g_hash_table_lookup (m_streams, GINT_TO_POINTER (channel_id)));

  g_debug ("StopRecording: channel_id: %d", channel_id);

  if (stream == NULL)
    {
      g_warning ("StopRecording: Audio stream %d not found", channel_id);
      return 0;
    }

  if (!skype_base_stream_record (stream, FALSE))
    {
      g_warning ("Error stopping audio recording");
      return 0;
    }

  return 1;
}

void
SkypeAudioManager::SubmitRTPPacket (int channel_id,
    const Sid::Binary &data)
{
  SkypeBaseStream *stream = SKYPE_BASE_STREAM (
      g_hash_table_lookup (m_streams, GINT_TO_POINTER (channel_id)));

  if (stream == NULL)
    {
      g_warning ("SubmitRTPPacket: Audio stream %d not found", channel_id);
      return;
    }

  if (!skype_base_stream_push_buffer (stream, data.data (), data.size ()))
    g_debug ("SubmitRTPPacket: failed to pushing buffer");
}

int
SkypeAudioManager::SendDTMF (int channel_id, int eventnr, int rtp_type)
{
  SkypeAudioStream *stream = SKYPE_AUDIO_STREAM (
      g_hash_table_lookup (m_streams, GINT_TO_POINTER (channel_id)));

  g_debug ("SendDTMF: channel_id: %d, eventnr: %d, rtp_type: %d",
      channel_id, eventnr, rtp_type);

  if (stream == NULL)
    {
      g_warning ("SendDTMF: Audio stream %d not found",
          channel_id);
      return 0;
    }

  if (!skype_audio_stream_send_dtmf (stream, eventnr, rtp_type))
    {
      g_warning ("SendDTMF: Error sending DTMF event");
      return 0;
    }

  return 1;
}

int
SkypeAudioManager::SetOutputAmplification (int channel_id, int scale)
{
  SkypeBaseStream *stream = SKYPE_BASE_STREAM (
      g_hash_table_lookup (m_streams, GINT_TO_POINTER (channel_id)));

  g_debug ("SetOutputAmplification: channel_id: %d, scale: %d",
      channel_id, scale);

  if (stream == NULL)
    {
      g_warning ("SetOutputAmplification: Audio stream %d not found",
          channel_id);
      return 0;
    }

  /* Skypekit passes the scale in a 100 being no amplification,
   * 0 being muted, and 300 being 3 times as loud */
  g_object_set (stream,
      "volume", (double)scale / 100.0,
      NULL);

  return 1;
}

int
SkypeAudioManager::CustomCommand (const Sid::String &cmd,
    Sid::String &result_str)
{
  result_str = "OK";
  return 1;
}

void
SkypeAudioManager::SetNewStreamCallback (
    SkypeAudioManagerNewStreamCallback callback,
    gpointer user_data)
{
  m_new_stream_cb = callback;
  m_new_stream_user_data = user_data;
}

void
SkypeAudioManager::SetSourceName (const gchar *source_name)
{
  g_free (m_source_name);
  m_source_name = g_strdup (source_name);
}

void
SkypeAudioManager::SetSourceDevice (const gchar *source_device)
{
  g_free (m_source_device);
  m_source_device = g_strdup (source_device);
}

void
SkypeAudioManager::SetSourcePipeline (const gchar *source_pipeline)
{
  g_free (m_source_pipeline);
  m_source_pipeline = g_strdup (source_pipeline);
}

void
SkypeAudioManager::SetSinkName (const gchar *sink_name)
{
  g_free (m_sink_name);
  m_sink_name = g_strdup (sink_name);
}

void
SkypeAudioManager::SetSinkDevice (const gchar *sink_device)
{
  g_free (m_sink_device);
  m_sink_device = g_strdup (sink_device);
}

void
SkypeAudioManager::SetSinkPipeline (const gchar *sink_pipeline)
{
  g_free (m_sink_pipeline);
  m_sink_pipeline = g_strdup (sink_pipeline);
}
