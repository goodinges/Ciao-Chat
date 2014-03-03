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

#include "skype-video-manager.hpp"

#include <gst/farsight/fs-codec.h>

#include "skype-utils.h"

static int
encoding_name_to_fourcc (const gchar *encoding_name)
{
  if (!g_ascii_strcasecmp (encoding_name, "H264"))
    {
      return MAKEFOURCC ('H', '2', '6', '4');
    }
  else if (!g_ascii_strcasecmp (encoding_name, "THEORA"))
    {
      return MAKEFOURCC ('V', 'P', '3', '1');
    }
  else
    {
      int len = strlen (encoding_name);

      return MAKEFOURCC (
          len >= 1 ? encoding_name[0] : '\0',
          len >= 2 ? encoding_name[1] : '\0',
          len >= 3 ? encoding_name[2] : '\0',
          len >= 4 ? encoding_name[3] : '\0');
    }
}

void
received_data_callback (SkypeBaseStream *stream,
    gsize len,
    const gchar *buf,
    gpointer data)
{
  SkypeVideoManager* __if;
  Sid::Binary binary;

  __if = static_cast<SkypeVideoManager *>(data);

  if (__if->m_videortpif_cb != NULL)
    {
      binary.set (buf, len);

      __if->m_videortpif_cb->SendRTPPacket (binary);
    }
  else
    {
      g_warning ("Got data but there's nowhere to send it");
    }
}

SkypeVideoManager::SkypeVideoManager (SkypeVideoRTPInterfaceCb *callback,
    GMainContext *context = NULL) :
    m_codecs (NULL), m_videortpif_cb (callback), m_context (context),
    m_source_name (NULL), m_source_device (NULL), m_source_pipeline (NULL),
    m_sink_name (NULL), m_sink_device (NULL), m_sink_pipeline (NULL),
    m_new_stream_cb (NULL), m_new_stream_user_data (NULL)
{
  if (m_context != NULL)
    g_main_context_ref (m_context);

  m_stream = skype_video_stream_new_with_context (m_context);

  g_signal_connect (m_stream, "buffer-prepared",
      G_CALLBACK (received_data_callback), this);
}

SkypeVideoManager::~SkypeVideoManager ()
{
  if (m_context != NULL)
    g_main_context_unref (m_context);
  g_object_unref (m_stream);
  m_stream = NULL;

  g_free (m_source_name);
  g_free (m_source_device);
  g_free (m_source_pipeline);
  g_free (m_sink_name);
  g_free (m_sink_device);
  g_free (m_sink_pipeline);
}

int
SkypeVideoManager::GetLastError ()
{
  return 0;
}

bool
SkypeVideoManager::Init ()
{
  GstElement *pipeline;
  GList *iter;
  gchar *path;
  gboolean ret = true;

  ret = skype_base_stream_init_stream (SKYPE_BASE_STREAM (m_stream));

  if (!ret)
    g_warning ("init_stream failed");

  g_object_set (m_stream,
      "source-name", m_source_name,
      "source-device", m_source_device,
      "source-pipeline", m_source_pipeline,
      "sink-name", m_sink_name,
      "sink-device", m_sink_device,
      "sink-pipeline", m_sink_pipeline,
      NULL);

  g_object_get (m_stream, "pipeline", &pipeline, NULL);

  if (pipeline == NULL)
    {
      g_warning ("Error fetching pipeline");
      ret = false;
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
          ret = false;
        }
      else
        {
          fs_element_added_notifier_add (m_notifier, GST_BIN (pipeline));
        }

      g_free (path);
      gst_object_unref (pipeline);
    }

  if (m_new_stream_cb != NULL)
    (*m_new_stream_cb)(this, m_stream, m_new_stream_user_data);

  path = skype_get_codec_preferences_path ();
  m_codecs = skype_get_supported_codecs_list (FS_MEDIA_TYPE_VIDEO, path);
  g_free (path);

  if (m_codecs == NULL)
    ret = false;

  /* Filter out codecs to only contain H264 */
  for (iter = m_codecs; iter != NULL; iter = g_list_next (iter))
    {
      FsCodec *codec = (FsCodec *)iter->data;

      if (!g_strcmp0 (codec->encoding_name, "H264"))
        {
          m_codecs = iter;
          iter = g_list_remove_link (iter, iter);
          fs_codec_list_destroy (iter);
          break;
        }
    }

  if (g_list_length (m_codecs) != 1)
    {
      g_warning ("H264 codec support not found");
      ret = false;
    }

  return ret;
}

bool
SkypeVideoManager::Uninit ()
{
  skype_base_stream_deinit_stream (SKYPE_BASE_STREAM (m_stream));

  if (m_notifier != NULL)
    {
      g_object_unref (m_notifier);
      m_notifier = NULL;
    }

  if (m_codecs != NULL)
    {
      fs_codec_list_destroy (m_codecs);
      m_codecs = NULL;
    }

  return true;
}

bool
SkypeVideoManager::GetCodecCount (int &codec_count)
{
  codec_count = g_list_length (m_codecs);
  g_debug ("SkypeVideoManager: GetCodecCount: %d", codec_count);
  return true;
}

bool
SkypeVideoManager::GetCodec (int id, VideoCodec &codec)
{
  FsCodec *fscodec;

  g_debug ("SkypeVideoManager: GetCodec: id: %d", id);

  if (m_codecs == NULL)
    {
      g_warning ("Codecs not ready");
      return false;
    }

  fscodec = (FsCodec *)g_list_nth_data (m_codecs, id);

  if (fscodec == NULL)
    {
      g_warning ("Codec not found");
      return false;
    }

  codec.fourcc = encoding_name_to_fourcc (fscodec->encoding_name);
  codec.recording_capable = 1;
  codec.payload_format = FORMAT_RTP;

  g_debug ("SkypeVideoManager: GetCodec: fourcc 0x%X, name %s",
      codec.fourcc, fscodec->encoding_name);

  return true;
}

bool
SkypeVideoManager::GetIntProperty (int prop_id, int index, int &value)
{
  g_debug ("SkypeVideoManager: GetIntProperty: id %d, index %d",
      prop_id, index);

  switch (prop_id)
    {
      case GET_NUMBER_OF_H264_SENDER_PARAMETERS:
        value = 1;
        break;
      case GET_NUMBER_OF_H264_RECEIVER_PARAMETERS:
        value = 1;
        break;
      default:
        return false;
    }

  return true;
}

bool
SkypeVideoManager::SetIntProperty (int prop_id, int index, int value)
{
  return false;
}

bool
SkypeVideoManager::GetStringProperty (int prop_id, int index, Sid::String &value)
{
  return false;
}

bool
SkypeVideoManager::SetStringProperty (int prop_id, int index, const Sid::String &value)
{
  return false;
}

bool
SkypeVideoManager::GetBinaryProperty (int prop_id, int index, Sid::Binary &value)
{
  g_debug ("SkypeVideoManager: GetBinaryProperty: id: %d, index: %d",
      prop_id, index);

  switch (prop_id)
    {
      case GET_H264_SENDER_PARAMETERS:
        {
          value.reserve (sizeof (H264SenderParameters));
          H264SenderParameters *param = (H264SenderParameters *)value.data ();

          param->profile_idc = 66;
          param->profile_iop = (H264ProfileIop)(PROFILE_IOP_CONSTRAINTS_SET0 |
					PROFILE_IOP_CONSTRAINTS_SET1 |
					PROFILE_IOP_CONSTRAINTS_SET2);
          param->level_idc = 30;
          break;
        }

      case GET_H264_RECEIVER_PARAMETERS:
        {
          value.reserve (sizeof (H264ReceiverParameters));
          H264ReceiverParameters *param =
              (H264ReceiverParameters *)value.data ();

          param->profile_idc = 66;
          param->profile_iop = (H264ProfileIop)(PROFILE_IOP_CONSTRAINTS_SET0 |
					PROFILE_IOP_CONSTRAINTS_SET1 |
					PROFILE_IOP_CONSTRAINTS_SET2);
          param->level_idc = 30;
          param->max_br = 400;
          break;
        }
      default:
        return false;
    }

  return true;
}

bool  
SkypeVideoManager::GetSenderResolutionCount(int &resolution_count)
{
    resolution_count = 4;

    return true;
}

bool  
SkypeVideoManager::GetSenderResolution(int index, FixedRes & resolution)
{
    switch (index)
    {
        case 0:
            resolution.width = 160;
            resolution.height = 120;
            resolution.preference = 30;
            break;
        case 1:
            resolution.width = 320;
            resolution.height = 240;
            resolution.preference = 40;
            break;
        case 2:
            resolution.width = 640;
            resolution.height = 480;
            resolution.preference = 50;
            break;
        case 3:
            resolution.width = 1280;
            resolution.height = 720;
            resolution.preference = 60;
            break;
        default:
            return false;
    }

    return true;
}

bool  
SkypeVideoManager::GetReceiverResolutionCount(int &resolution_count)
{
    resolution_count = 4;

    return true;
}

bool  
SkypeVideoManager::GetReceiverResolution(int index, ResCap & resolution)
{
    switch (index)
    {
        case 0:
            resolution.max_width = 160;
            resolution.max_height = 120;
            resolution.min_width = 160;
            resolution.min_height = 120;
            resolution.preference = 30;
            break;
        case 1:
            resolution.max_width = 320;
            resolution.max_height = 240;
            resolution.min_width = 320;
            resolution.min_height = 240;
            resolution.preference = 40;
            break;
        case 2:
            resolution.max_width = 640;
            resolution.max_height = 480;
            resolution.min_width = 640;
            resolution.min_height = 480;
            resolution.preference = 50;
            break;
        case 3:
            resolution.max_width = 1280;
            resolution.max_height = 720;
            resolution.min_width = 1280;
            resolution.min_height = 720;
            resolution.preference = 60;
            break;
        default:
            return false;
    }

    return true;
}

bool
SkypeVideoManager::SetBinaryProperty (int prop_id, int index, const Sid::Binary &value)
{
  /* TODO: Actually handle this */
  return true;
}

bool
SkypeVideoManager::RequestKeyFrame ()
{
  skype_video_stream_request_key_frame (m_stream);
  return true;
}

bool
SkypeVideoManager::RequestRecoveryFrame (int reserved)
{
  return false;
}

/* bitrate is in bytes/sec */
bool
SkypeVideoManager::SetBitrate (int bitrate)
{
  g_debug("SkypeVideoManager: SetBitrate: %d", bitrate);
  g_object_set (m_stream, "bitrate", bitrate, NULL);
  return true;
}

bool
SkypeVideoManager::SetFramerate (int framerate_fps)
{
  g_debug ("SkypeVideoManager: SetFramerate %d", framerate_fps);
  g_object_set (m_stream, "fps", framerate_fps, NULL);
  return true;
}

bool
SkypeVideoManager::SetMaxPacketSize (int max_packet_size)
{
  g_debug ("SkypeVideoManager: SetMaxPacketSize: %d", max_packet_size);
  g_object_set (m_stream, "mtu", max_packet_size, NULL);
  return true;
}

bool
SkypeVideoManager::StartPlayback (int decoderfourcc, int width, int height)
{
  GList *iter;
  FsCodec *codec;

  g_debug ("SkypeVideoManager: StartPlayback: fourcc: 0x%X, resolution %dx%d",
      decoderfourcc, width, height);

  for (iter = m_codecs; iter != NULL; iter = g_list_next (iter))
    {
      codec = (FsCodec *)iter->data;

      if (encoding_name_to_fourcc (codec->encoding_name) == decoderfourcc)
        break;
    }

  if (iter == NULL)
    return false;

  g_debug ("SkypeVideoManager: StartPlayback: Chose %s", codec->encoding_name);

  codec->id = 96;

  g_object_set (m_stream,
      "codec", codec,
      NULL);

  return skype_base_stream_play (SKYPE_BASE_STREAM (m_stream), TRUE);
}

void
SkypeVideoManager::RtpPacketReceived (const Sid::Binary &data)
{
  skype_base_stream_push_buffer (SKYPE_BASE_STREAM (m_stream),
      data.data (), data.size ());
}

void
SkypeVideoManager::FrameReceived (const Sid::Binary &data, int timestamp_ms)
{
}

bool
SkypeVideoManager::StopPlayback ()
{
  g_debug ("SkypeVideoManager: StopPlayback");
  return skype_base_stream_play (SKYPE_BASE_STREAM (m_stream), FALSE);
}

bool
SkypeVideoManager::StartRecording (int fourcc, int initial_max_packet_size,
    int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps)
{
  GList *iter;
  FsCodec *codec;

  g_debug ("SkypeVideoManager: StartRecording: fourcc: 0x%X, packet size: %d, "
      "bitrate: %d, %dx%d", fourcc, initial_max_packet_size,
      initial_bitrate_bytes_sec, width, height);

  for (iter = m_codecs; iter != NULL; iter = g_list_next (iter))
    {
      codec = (FsCodec *)iter->data;

      if (encoding_name_to_fourcc (codec->encoding_name) == fourcc)
        break;
    }

  if (iter == NULL)
    return false;

  g_debug ("SkypeVideoManager: StartRecording: Chose %s", codec->encoding_name);

  codec->id = 96;

  g_object_set (m_stream,
      "codec", codec,
      "mtu", initial_max_packet_size,
      "bitrate", initial_bitrate_bytes_sec,
      NULL);

  g_object_set (m_stream, "fps", initial_framerate_fps, NULL);

  if (!skype_video_stream_set_resolution (m_stream, width, height))
    g_warning ("Error setting resolution");

  if (!skype_base_stream_record (SKYPE_BASE_STREAM (m_stream), TRUE))
    g_warning ("Error starting to record stream");

  return true;
}

bool
SkypeVideoManager::StopRecording ()
{
  g_debug ("SkypeVideoManager: StopRecording");
  return skype_base_stream_record (SKYPE_BASE_STREAM (m_stream), FALSE);
}

bool
SkypeVideoManager::CustomCommand (const Sid::String &cmd, Sid::String &result_str)
{
  result_str = "OK";
  return true;
}

SkypeVideoRTPInterface *
SkypeVideoRTPInterfaceGet (SkypeVideoRTPInterfaceCb *callback)
{
  return NULL;
}

void
SkypeVideoRTPInterfaceRelease (SkypeVideoRTPInterface *rtpvideo)
{
}

void
SkypeVideoManager::SetNewStreamCallback (
    SkypeVideoManagerNewStreamCallback callback,
    gpointer user_data)
{
  m_new_stream_cb = callback;
  m_new_stream_user_data = user_data;
}

void
SkypeVideoManager::SetSourceName (const gchar *source_name)
{
  g_free (m_source_name);
  m_source_name = g_strdup (source_name);
}

void
SkypeVideoManager::SetSourceDevice (const gchar *source_device)
{
  g_free (m_source_device);
  m_source_device = g_strdup (source_device);
}

void
SkypeVideoManager::SetSourcePipeline (const gchar *source_pipeline)
{
  g_free (m_source_pipeline);
  m_source_pipeline = g_strdup (source_pipeline);
}

void
SkypeVideoManager::SetSinkName (const gchar *sink_name)
{
  g_free (m_sink_name);
  m_sink_name = g_strdup (sink_name);
}

void
SkypeVideoManager::SetSinkDevice (const gchar *sink_device)
{
  g_free (m_sink_device);
  m_sink_device = g_strdup (sink_device);
}

void
SkypeVideoManager::SetSinkPipeline (const gchar *sink_pipeline)
{
  g_free (m_sink_pipeline);
  m_sink_pipeline = g_strdup (sink_pipeline);
}
