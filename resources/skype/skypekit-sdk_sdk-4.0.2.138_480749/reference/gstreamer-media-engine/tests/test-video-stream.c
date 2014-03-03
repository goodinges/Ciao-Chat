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

#include <gst/farsight/fs-codec.h>
#include <gst/gst.h>

#include "skype-video-stream.h"

static void
test_video_stream_init (void)
{
  SkypeVideoStream *stream = skype_video_stream_new ();
  g_assert (SKYPE_IS_VIDEO_STREAM (stream));
  g_object_unref (stream);
}

static void
test_video_stream_init_stream (void)
{
  SkypeVideoStream *stream;

  stream = skype_video_stream_new ();
  g_assert (SKYPE_IS_VIDEO_STREAM (stream));

  g_object_set (stream, "source-name", "videotestsrc", NULL);

  g_assert (skype_base_stream_init_stream (SKYPE_BASE_STREAM (stream)));

  skype_base_stream_deinit_stream (SKYPE_BASE_STREAM (stream));
  g_object_unref (stream);
}

static void
test_video_stream_buffer_prepared_cb (SkypeBaseStream *self,
    gsize len,
    const gchar *buffer,
    gpointer user_data)
{
  GSource *source;
  GMainContext *context;

  g_assert (buffer != NULL);
  g_assert_cmpint (len, >, 0);

  source = g_main_current_source ();
  g_assert (source != NULL);
  context = g_source_get_context (source);
  g_assert (context != NULL);

  source = g_idle_source_new ();
  g_assert (source != NULL);
  g_source_set_callback (source,
      (GSourceFunc)g_main_loop_quit, user_data, NULL);
  g_assert (g_source_attach (source, context) > 0);
  g_source_unref (source);
}

static void
test_video_stream_record (void)
{
  GMainLoop *loop;
  SkypeVideoStream *stream;
  FsCodec *codec;
  GMainContext *context;

  context = g_main_context_new ();
  g_assert (context);

  loop = g_main_loop_new (context, FALSE);
  g_assert (loop);

  stream = skype_video_stream_new_with_context (context);
  g_assert (SKYPE_IS_VIDEO_STREAM (stream));
  g_main_context_unref (context);

  g_object_set (stream, "source-name", "videotestsrc", NULL);

  g_signal_connect (stream, "buffer-prepared",
      G_CALLBACK (test_video_stream_buffer_prepared_cb), loop);
  g_assert (skype_base_stream_init_stream (SKYPE_BASE_STREAM (stream)));

  codec = fs_codec_new (96, "H264",
      FS_MEDIA_TYPE_VIDEO, 90000);
  g_assert (codec != NULL);
  g_object_set (stream, "codec", codec, NULL);
  fs_codec_destroy (codec);

  g_assert (skype_base_stream_record (SKYPE_BASE_STREAM (stream), TRUE));

  g_main_loop_run (loop);

  g_object_unref (stream);
  g_main_loop_unref (loop);
}

static void
test_video_stream_buffer_prepared_now_send_cb (SkypeBaseStream *self,
    gsize len,
    const gchar *buffer,
    gpointer user_data)
{
  g_assert (buffer != NULL);
  g_assert_cmpint (len, >, 0);
  g_assert (skype_base_stream_push_buffer (user_data, buffer, len));
}

static void
test_video_stream_play (void)
{
  GMainLoop *loop;
  /* Farsight 2 ignores looped back packets so we use two instances here */
  SkypeVideoStream *send_stream;
  SkypeVideoStream *recv_stream;
  FsCodec *codec;
  GMainContext *context;
  GSource *source;

  context = g_main_context_new ();
  g_assert (context != NULL);

  loop = g_main_loop_new (context, FALSE);
  g_assert (loop);

  codec = fs_codec_new (96, "H264", FS_MEDIA_TYPE_VIDEO, 90000);

  send_stream = skype_video_stream_new_with_context (context);
  g_assert (SKYPE_IS_VIDEO_STREAM (send_stream));

  g_object_set (send_stream,
      "source-name", "videotestsrc",
      "codec", fs_codec_copy (codec),
      NULL);

  recv_stream = skype_video_stream_new_with_context (context);
  g_assert (SKYPE_IS_VIDEO_STREAM (recv_stream));

  g_object_set (recv_stream,
      "sink-name", "fakesink",
      "codec", fs_codec_copy (codec),
      NULL);

  fs_codec_destroy (codec);

  g_assert (skype_base_stream_init_stream (SKYPE_BASE_STREAM (recv_stream)));

  g_signal_connect (send_stream, "buffer-prepared",
      G_CALLBACK (test_video_stream_buffer_prepared_now_send_cb), recv_stream);

  g_assert (skype_base_stream_init_stream (SKYPE_BASE_STREAM (send_stream)));

  g_assert (skype_base_stream_play (SKYPE_BASE_STREAM (recv_stream), TRUE));
  /* Start recording */
  g_assert (skype_base_stream_record (SKYPE_BASE_STREAM (send_stream), TRUE));

  /* TODO: Instead verify that the sink is receiving buffers possibly with
   * the last-buffer property once we can get to the sink */
  source = g_timeout_source_new_seconds (5);
  g_assert (source != NULL);
  g_source_set_callback (source,
      (GSourceFunc)g_main_loop_quit, loop, NULL);
  g_source_attach (source, context);
  g_source_unref (source);

  g_main_loop_run (loop);

  /* Stop recording */
  g_assert (skype_base_stream_record (SKYPE_BASE_STREAM (send_stream), FALSE));

  source = g_timeout_source_new_seconds (5);
  g_assert (source != NULL);
  g_source_set_callback (source,
      (GSourceFunc)g_main_loop_quit, loop, NULL);
  g_source_attach (source, context);
  g_source_unref (source);

  g_main_loop_run (loop);

  /* Restart recording */
  g_assert (skype_base_stream_record (SKYPE_BASE_STREAM (send_stream), TRUE));

  source = g_timeout_source_new_seconds (5);
  g_assert (source != NULL);
  g_source_set_callback (source,
      (GSourceFunc)g_main_loop_quit, loop, NULL);
  g_source_attach (source, context);
  g_source_unref (source);

  g_main_loop_run (loop);
  g_object_unref (send_stream);
  g_object_unref (recv_stream);
  g_main_context_unref (context);
  g_main_loop_unref (loop);
}

static void
test_video_stream_properties (void)
{
  GMainLoop *loop;
  /* Farsight 2 ignores looped back packets so we use two instances here */
  SkypeVideoStream *send_stream;
  SkypeVideoStream *recv_stream;
  FsCodec *codec;
  guint bitrate = 0;
  guint mtu = 0;
  guint media_type = 0;
  guint fps = 0;
  guint width = 0;
  guint height = 0;

  loop = g_main_loop_new (NULL, FALSE);
  g_assert (loop);

  codec = fs_codec_new (96, "H264", FS_MEDIA_TYPE_VIDEO, 90000);

  send_stream = skype_video_stream_new ();
  g_assert (SKYPE_IS_VIDEO_STREAM (send_stream));

  g_object_get (send_stream,
      "bitrate", &bitrate,
      "mtu", &mtu,
      "media-type", &media_type,
      NULL);
  g_assert_cmpint (bitrate, ==, 0);
  g_assert_cmpint (mtu, ==, 0);
  g_assert_cmpint (media_type, ==, FS_MEDIA_TYPE_VIDEO);

  g_object_set (send_stream,
      "source-name", "videotestsrc",
      "codec", fs_codec_copy (codec),
      "bitrate", 1920,
      "fps", 15,
      "mtu", 1000,
      NULL);

  g_assert (skype_video_stream_set_resolution (send_stream, 240, 180));

  recv_stream = skype_video_stream_new ();
  g_assert (SKYPE_IS_VIDEO_STREAM (recv_stream));

  g_object_set (recv_stream,
      "sink-name", "fakesink",
      "codec", fs_codec_copy (codec),
      NULL);

  fs_codec_destroy (codec);

  g_assert (skype_base_stream_init_stream (SKYPE_BASE_STREAM (recv_stream)));

  g_signal_connect (send_stream, "buffer-prepared",
      G_CALLBACK (test_video_stream_buffer_prepared_now_send_cb), recv_stream);

  g_assert (skype_base_stream_init_stream (SKYPE_BASE_STREAM (send_stream)));

  g_assert (skype_base_stream_play (SKYPE_BASE_STREAM (recv_stream), TRUE));
  g_assert (skype_base_stream_record (SKYPE_BASE_STREAM (send_stream), TRUE));

  /* TODO: Instead verify that the sink is receiving buffers possibly with
   * the last-buffer property once we can get to the sink */
  g_timeout_add_seconds (5, (GSourceFunc)g_main_loop_quit, loop);

  g_main_loop_run (loop);

  g_object_get (send_stream,
      "bitrate", &bitrate,
      "mtu", &mtu,
      "fps", &fps,
      "width", &width,
      "height", &height,
      NULL);
  g_assert_cmpint (bitrate, ==, 1920);
  g_assert_cmpint (mtu, ==, 1000);
  g_assert_cmpint (fps, ==, 15);
  g_assert_cmpint (width, ==, 240);
  g_assert_cmpint (height, ==, 180);

  g_assert (skype_video_stream_set_resolution (send_stream, 320, 240));

  g_object_get (send_stream,
      "width", &width,
      "height", &height,
      NULL);
  g_assert_cmpint (width, ==, 320);
  g_assert_cmpint (height, ==, 240);

  g_object_unref (send_stream);
  g_object_unref (recv_stream);
  g_main_loop_unref (loop);
}

static void
test_video_stream_requests (void)
{
  GMainLoop *loop;
  SkypeVideoStream *stream;
  FsCodec *codec;
  GMainContext *context;

  context = g_main_context_new ();
  g_assert (context);

  loop = g_main_loop_new (context, FALSE);
  g_assert (loop);

  stream = skype_video_stream_new_with_context (context);
  g_assert (SKYPE_IS_VIDEO_STREAM (stream));
  g_main_context_unref (context);

  g_object_set (stream, "source-name", "videotestsrc", NULL);

  g_signal_connect (stream, "buffer-prepared",
      G_CALLBACK (test_video_stream_buffer_prepared_cb), loop);
  g_assert (skype_base_stream_init_stream (SKYPE_BASE_STREAM (stream)));

  codec = fs_codec_new (96, "H264",
      FS_MEDIA_TYPE_VIDEO, 90000);
  g_assert (codec != NULL);
  g_object_set (stream, "codec", codec, NULL);
  fs_codec_destroy (codec);

  g_assert (skype_base_stream_record (SKYPE_BASE_STREAM (stream), TRUE));

  g_main_loop_run (loop);

  skype_video_stream_request_key_frame (stream);
  skype_video_stream_request_recovery_frame (stream);

  g_object_unref (stream);
  g_main_loop_unref (loop);
}

void
add_video_stream_tests (void)
{
  g_test_add_func ("/video-stream/init", test_video_stream_init);
  g_test_add_func ("/video-stream/init-stream", test_video_stream_init_stream);
  g_test_add_func ("/video-stream/record", test_video_stream_record);
  g_test_add_func ("/video-stream/play", test_video_stream_play);
  g_test_add_func ("/video-stream/properties", test_video_stream_properties);
  g_test_add_func ("/video-stream/requests", test_video_stream_requests);
}
