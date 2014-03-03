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

#include "skype-media-engine.h"

#include "skype-audio-call.h"
#include "skype-video-call.h"
#include <gst/filters/fsu-filter-manager.h>

#include "sidg_videortpif_client.hpp"
#include "sidg_multirtpif_client.hpp"

#define SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/tmp"
#define SKYPEKIT_MULTIRTP_FROM_SKYPEKIT_FILE "multirtp_from_skypekit_key"
#define SKYPEKIT_MULTIRTP_TO_SKYPEKIT_FILE "multirtp_to_skypekit_key"
#define SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE "vidrtp_from_skypekit_key"
#define SKYPEKIT_VIDEORTP_TO_SKYPEKIT_FILE "vidrtp_to_skypekit_key"

typedef struct
{
  SkypeMediaEngine *engine;
  GMainLoop *loop;
} EngineLoopPair;

static gboolean
quit_loop (gpointer data)
{
  EngineLoopPair *pair = (EngineLoopPair *)data;
  skype_media_engine_stop (pair->engine, "/tmp");
  g_main_loop_quit (pair->loop);
  return FALSE;
}

static void
test_media_engine_init (void)
{
  SkypeMediaEngine *engine;

  engine = skype_media_engine_new ();
  g_assert (engine);

  g_object_unref (engine);
}

static void
test_media_engine_no_connection (void)
{
  EngineLoopPair pair;

  pair.engine = skype_media_engine_new ();
  g_assert (pair.engine);

  g_assert (skype_media_engine_start_async (pair.engine,
      SKYPE_MEDIA_VIDEO, "/tmp", NULL));

  pair.loop = g_main_loop_new (NULL, FALSE);

  g_timeout_add_seconds (3, quit_loop, &pair);

  g_main_loop_run (pair.loop);

  g_main_loop_unref (pair.loop);
  g_object_unref (pair.engine);
}

static void
test_media_engine_connection_but_no_data (void)
{
  EngineLoopPair pair;
  Sid::VideoRTPIfClient client;

  pair.engine = skype_media_engine_new ();
  g_assert (pair.engine);

  g_assert (skype_media_engine_start_async (pair.engine,
      SKYPE_MEDIA_VIDEO, "/tmp", NULL));

  client.Connect (SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/"
      SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE, 500);

  /* Complains here as the media engine is trying to connect, but
   * we aren't providing the server for it to connect to */

  pair.loop = g_main_loop_new (NULL, FALSE);

  g_timeout_add_seconds (3, quit_loop, &pair);

  g_main_loop_run (pair.loop);

  g_main_loop_unref (pair.loop);
  g_object_unref (pair.engine);
}

static void
test_media_engine_connection_init (void)
{
  EngineLoopPair pair;
  Sid::VideoRTPIfClient client;
  GMainContext *context;
  GSource *source;

  pair.engine = skype_media_engine_new ();
  g_assert (pair.engine);

  context = g_main_context_new ();
  g_assert (context != NULL);

  g_assert (skype_media_engine_start_async (pair.engine,
      SKYPE_MEDIA_VIDEO, "/tmp", context));

  client.Connect (SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/"
      SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE, 500);

  /* Complains here as the media engine is trying to connect, but
   * we aren't providing the server for it to connect to */

  client.Init ();

  pair.loop = g_main_loop_new (context, FALSE);

  source = g_timeout_source_new_seconds (3);
  g_assert (source != NULL);
  g_source_set_callback (source, quit_loop, &pair, NULL);
  g_source_attach (source, context);
  g_source_unref (source);

  g_main_context_unref (context);

  g_main_loop_run (pair.loop);

  g_main_loop_unref (pair.loop);
  g_object_unref (pair.engine);
}

static void
new_video_call_cb (SkypeMediaEngine *engine,
    SkypeVideoCall *call,
    gpointer user_data)
{
  EngineLoopPair *pair = (EngineLoopPair *)user_data;
  GMainContext *context;
  GSource *source;
  gchar *ipc_path_prefix;
  FsuFilterManager *incoming_filter;
  FsuFilterManager *outgoing_filter;
  GList *filters;
  guint num_filters;

  g_debug ("New video call: %p", call);

  g_assert (SKYPE_IS_MEDIA_ENGINE (engine));
  g_assert (engine == pair->engine);
  g_assert (SKYPE_IS_VIDEO_CALL (call));

  g_object_get (call,
      "ipc_path_prefix", &ipc_path_prefix,
      "incoming-filter", &incoming_filter,
      "outgoing-filter", &outgoing_filter,
      NULL);

  /* Validate properties */
  g_assert (!strcmp (ipc_path_prefix, "/tmp"));
  g_assert (FSU_IS_FILTER_MANAGER (incoming_filter));
  g_assert (FSU_IS_FILTER_MANAGER (outgoing_filter));
  g_assert (incoming_filter != outgoing_filter);

  g_free (ipc_path_prefix);
  g_object_unref (incoming_filter);

  /* Validate adding a preview window actually adds a filter */
  filters = fsu_filter_manager_list_filters (outgoing_filter);
  num_filters = g_list_length (filters);
  g_list_free (filters);

  g_assert (skype_video_call_add_preview_window (call, 0, NULL));

  filters = fsu_filter_manager_list_filters (outgoing_filter);
  g_assert (num_filters + 1 == g_list_length (filters));
  g_list_free (filters);

  g_object_unref (outgoing_filter);

  context = g_main_loop_get_context (pair->loop);
  g_assert (context != NULL);

  /* Call quit_loop in the correct context and thread
   * Perhaps the signal should be fired in the correct context
   * and thread for us? */
  source = g_idle_source_new ();
  g_assert (source != NULL);
  g_source_set_callback (source,
      (GSourceFunc)quit_loop, user_data, NULL);
  g_assert (g_source_attach (source, context) > 0);
  g_source_unref (source);
}

static void
test_media_engine_new_video_call (void)
{
  EngineLoopPair pair;
  Sid::VideoRTPIfClient client;
  GMainContext *context;

  pair.engine = skype_media_engine_new ();
  g_assert (pair.engine);

  context = g_main_context_new ();
  g_assert (context != NULL);

  pair.loop = g_main_loop_new (context, FALSE);
  g_assert (pair.loop != NULL);

  g_assert (skype_media_engine_start_async (pair.engine,
      SKYPE_MEDIA_VIDEO, "/tmp", context));
  g_main_context_unref (context);

  g_signal_connect (pair.engine, "new-video-call",
      G_CALLBACK (new_video_call_cb), &pair);

  client.Connect (SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/"
      SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE, 500);

  /* Complains here as the media engine is trying to connect, but
   * we aren't providing the server for it to connect to */

  client.Init ();

  g_main_loop_run (pair.loop);

  g_main_loop_unref (pair.loop);
  g_object_unref (pair.engine);
}

static void
call_ended_cb (SkypeVideoCall *call, gpointer user_data)
{
  /* TODO: Verify this is actually called */
  g_debug ("Call ended");
  g_object_unref (call);
}

static void
new_video_call_ended_cb (SkypeMediaEngine *engine,
    SkypeVideoCall *call,
    gpointer user_data)
{
  EngineLoopPair *pair = (EngineLoopPair *)user_data;
  GMainContext *context;
  GSource *source;
  gchar *ipc_path_prefix;
  FsuFilterManager *incoming_filter;
  FsuFilterManager *outgoing_filter;
  GList *filters;
  guint num_filters;

  g_debug ("New video call: %p", call);

  g_assert (SKYPE_IS_MEDIA_ENGINE (engine));
  g_assert (engine == pair->engine);
  g_assert (SKYPE_IS_VIDEO_CALL (call));

  g_object_get (call,
      "ipc_path_prefix", &ipc_path_prefix,
      "incoming-filter", &incoming_filter,
      "outgoing-filter", &outgoing_filter,
      NULL);

  /* Validate properties */
  g_assert (!strcmp (ipc_path_prefix, "/tmp"));
  g_assert (FSU_IS_FILTER_MANAGER (incoming_filter));
  g_assert (FSU_IS_FILTER_MANAGER (outgoing_filter));
  g_assert (incoming_filter != outgoing_filter);

  g_free (ipc_path_prefix);
  g_object_unref (incoming_filter);

  /* Validate adding a preview window actually adds a filter */
  filters = fsu_filter_manager_list_filters (outgoing_filter);
  num_filters = g_list_length (filters);
  g_list_free (filters);

  g_assert (skype_video_call_add_preview_window (call, 0, NULL));

  filters = fsu_filter_manager_list_filters (outgoing_filter);
  g_assert (num_filters + 1 == g_list_length (filters));
  g_list_free (filters);

  g_object_unref (outgoing_filter);

  context = g_main_loop_get_context (pair->loop);
  g_assert (context != NULL);

  /* Call quit_loop in the correct context and thread
   * Perhaps the signal should be fired in the correct context
   * and thread for us? */
  source = g_idle_source_new ();
  g_assert (source != NULL);
  g_source_set_callback (source,
      (GSourceFunc)quit_loop, user_data, NULL);
  g_assert (g_source_attach (source, context) > 0);
  g_source_unref (source);

  g_object_ref (call);
  g_signal_connect (call, "ended", G_CALLBACK (call_ended_cb), user_data);
}

static void
test_media_engine_new_video_call_ended (void)
{
  EngineLoopPair pair;
  Sid::VideoRTPIfClient client;
  GMainContext *context;

  pair.engine = skype_media_engine_new ();
  g_assert (pair.engine);

  context = g_main_context_new ();
  g_assert (context != NULL);

  pair.loop = g_main_loop_new (context, FALSE);
  g_assert (pair.loop != NULL);

  g_assert (skype_media_engine_start_async (pair.engine,
      SKYPE_MEDIA_VIDEO, "/tmp", context));
  g_main_context_unref (context);

  g_signal_connect (pair.engine, "new-video-call",
      G_CALLBACK (new_video_call_ended_cb), &pair);

  client.Connect (SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/"
      SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE, 500);

  /* Complains here as the media engine is trying to connect, but
   * we aren't providing the server for it to connect to */

  client.Init ();

  g_main_loop_run (pair.loop);

  g_main_loop_unref (pair.loop);
  g_object_unref (pair.engine);
}

static void
new_video_call_add_preview_cb (SkypeMediaEngine *engine,
    SkypeVideoCall *call,
    gpointer user_data)
{
  EngineLoopPair *pair = (EngineLoopPair *)user_data;
  GMainContext *context;
  GSource *source;
  gchar *ipc_path_prefix;
  FsuFilterManager *incoming_filter;
  FsuFilterManager *outgoing_filter;
  GList *filters;
  guint num_filters;

  g_debug ("New video call: %p", call);

  g_assert (SKYPE_IS_MEDIA_ENGINE (engine));
  g_assert (engine == pair->engine);
  g_assert (SKYPE_IS_VIDEO_CALL (call));

  g_object_get (call,
      "ipc_path_prefix", &ipc_path_prefix,
      "incoming-filter", &incoming_filter,
      "outgoing-filter", &outgoing_filter,
      NULL);

  /* Validate properties */
  g_assert (!strcmp (ipc_path_prefix, "/tmp"));
  g_assert (FSU_IS_FILTER_MANAGER (incoming_filter));
  g_assert (FSU_IS_FILTER_MANAGER (outgoing_filter));
  g_assert (incoming_filter != outgoing_filter);

  g_free (ipc_path_prefix);
  g_object_unref (incoming_filter);

  /* Validate adding a preview window actually adds a filter */
  filters = fsu_filter_manager_list_filters (outgoing_filter);
  num_filters = g_list_length (filters);
  g_list_free (filters);

  g_assert (skype_video_call_add_preview_window (call, 0, NULL));

  filters = fsu_filter_manager_list_filters (outgoing_filter);
  g_assert (num_filters + 1 == g_list_length (filters));
  g_list_free (filters);

  g_object_unref (outgoing_filter);

  context = g_main_loop_get_context (pair->loop);
  g_assert (context != NULL);

  /* Call quit_loop in the correct context and thread
   * Perhaps the signal should be fired in the correct context
   * and thread for us? */
  source = g_idle_source_new ();
  g_assert (source != NULL);
  g_source_set_callback (source,
      (GSourceFunc)quit_loop, user_data, NULL);
  g_assert (g_source_attach (source, context) > 0);
  g_source_unref (source);

  g_assert (skype_video_call_add_preview_window (call, 0, NULL));
}

static void
test_media_engine_new_video_call_add_preview (void)
{
  EngineLoopPair pair;
  Sid::VideoRTPIfClient client;
  GMainContext *context;

  pair.engine = skype_media_engine_new ();
  g_assert (pair.engine);

  context = g_main_context_new ();
  g_assert (context != NULL);

  pair.loop = g_main_loop_new (context, FALSE);
  g_assert (pair.loop != NULL);

  g_assert (skype_media_engine_start_async (pair.engine,
      SKYPE_MEDIA_VIDEO, "/tmp", context));
  g_main_context_unref (context);

  g_signal_connect (pair.engine, "new-video-call",
      G_CALLBACK (new_video_call_add_preview_cb), &pair);

  client.Connect (SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/"
      SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE, 500);

  /* Complains here as the media engine is trying to connect, but
   * we aren't providing the server for it to connect to */

  client.Init ();

  g_main_loop_run (pair.loop);

  g_main_loop_unref (pair.loop);
  g_object_unref (pair.engine);
}

static void
new_audio_call_cb (SkypeMediaEngine *engine,
    SkypeAudioCall *call,
    gpointer user_data)
{
  EngineLoopPair *pair = (EngineLoopPair *)user_data;
  GMainContext *context;
  GSource *source;
  gchar *ipc_path_prefix;
  FsuFilterManager *incoming_filter;
  FsuFilterManager *outgoing_filter;
  gdouble volume_in = 0;
  gdouble volume_out = 0;

  g_debug ("New audio call: %p", call);

  g_assert (SKYPE_IS_MEDIA_ENGINE (engine));
  g_assert (engine == pair->engine);
  g_assert (SKYPE_IS_AUDIO_CALL (call));

  g_object_get (call,
      "ipc_path_prefix", &ipc_path_prefix,
      "incoming-filter", &incoming_filter,
      "outgoing-filter", &outgoing_filter,
      "volume-in", &volume_in,
      "volume-out", &volume_out,
      NULL);

  /* Validate properties */
  g_assert (!strcmp (ipc_path_prefix, "/tmp"));
  g_assert (FSU_IS_FILTER_MANAGER (incoming_filter));
  g_assert (FSU_IS_FILTER_MANAGER (outgoing_filter));
  g_assert (incoming_filter != outgoing_filter);
  g_assert_cmpfloat (volume_in, ==, 1.0);
  g_assert_cmpfloat (volume_out, ==, 1.0);

  g_free (ipc_path_prefix);
  g_object_unref (incoming_filter);
  g_object_unref (outgoing_filter);

  g_object_set (call,
      "volume-in", 2.0,
      "volume-out", 5.0,
      NULL);
  g_object_get (call,
      "volume-in", &volume_in,
      "volume-out", &volume_out,
      NULL);
  g_assert_cmpfloat (volume_in, ==, 2.0);
  g_assert_cmpfloat (volume_out, ==, 5.0);

  context = g_main_loop_get_context (pair->loop);
  g_assert (context != NULL);

  /* Call quit_loop in the correct context and thread
   * Perhaps the signal should be fired in the correct context
   * and thread for us? */
  source = g_idle_source_new ();
  g_assert (source != NULL);
  g_source_set_callback (source,
      (GSourceFunc)quit_loop, user_data, NULL);
  g_assert (g_source_attach (source, context) > 0);
  g_source_unref (source);
}

static void
test_media_engine_new_audio_call (void)
{
  EngineLoopPair pair;
  Sid::SkypeMultiRTPInterfaceClient client;
  GMainContext *context;
  int channel = 0;

  pair.engine = skype_media_engine_new ();
  g_assert (pair.engine);

  context = g_main_context_new ();
  g_assert (context != NULL);

  pair.loop = g_main_loop_new (context, FALSE);
  g_assert (pair.loop != NULL);

  g_assert (skype_media_engine_start_async (pair.engine,
      SKYPE_MEDIA_AUDIO, "/tmp", context));
  g_main_context_unref (context);

  g_signal_connect (pair.engine, "new-audio-call",
      G_CALLBACK (new_audio_call_cb), &pair);

  client.Connect (SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/"
      SKYPEKIT_MULTIRTP_FROM_SKYPEKIT_FILE, 500);

  /* Complains here as the media engine is trying to connect, but
   * we aren't providing the server for it to connect to */

  client.Init ();

  g_assert_cmpint (client.CreateChannel (1, 1, channel), ==, 1);

  g_main_loop_run (pair.loop);

  g_main_loop_unref (pair.loop);
  g_object_unref (pair.engine);
}

static void
audio_call_ended_cb (SkypeAudioCall *call, gpointer user_data)
{
  /* TODO: Verify this is actually called */
  g_debug ("Call ended");
  g_object_unref (call);
}

static void
new_audio_call_ended_cb (SkypeMediaEngine *engine,
    SkypeAudioCall *call,
    gpointer user_data)
{
  EngineLoopPair *pair = (EngineLoopPair *)user_data;
  GMainContext *context;
  GSource *source;
  gchar *ipc_path_prefix;
  FsuFilterManager *incoming_filter;
  FsuFilterManager *outgoing_filter;

  g_debug ("New audio call: %p", call);

  g_assert (SKYPE_IS_MEDIA_ENGINE (engine));
  g_assert (engine == pair->engine);
  g_assert (SKYPE_IS_AUDIO_CALL (call));

  g_object_get (call,
      "ipc_path_prefix", &ipc_path_prefix,
      "incoming-filter", &incoming_filter,
      "outgoing-filter", &outgoing_filter,
      NULL);

  /* Validate properties */
  g_assert (!strcmp (ipc_path_prefix, "/tmp"));
  g_assert (FSU_IS_FILTER_MANAGER (incoming_filter));
  g_assert (FSU_IS_FILTER_MANAGER (outgoing_filter));
  g_assert (incoming_filter != outgoing_filter);

  g_free (ipc_path_prefix);
  g_object_unref (incoming_filter);

  context = g_main_loop_get_context (pair->loop);
  g_assert (context != NULL);

  /* Call quit_loop in the correct context and thread
   * Perhaps the signal should be fired in the correct context
   * and thread for us? */
  source = g_idle_source_new ();
  g_assert (source != NULL);
  g_source_set_callback (source,
      (GSourceFunc)quit_loop, user_data, NULL);
  g_assert (g_source_attach (source, context) > 0);
  g_source_unref (source);

  g_object_ref (call);
  g_signal_connect (call, "ended", G_CALLBACK (audio_call_ended_cb), user_data);
}

static void
test_media_engine_new_audio_call_ended (void)
{
  EngineLoopPair pair;
  Sid::SkypeMultiRTPInterfaceClient client;
  GMainContext *context;
  int channel = 0;

  pair.engine = skype_media_engine_new ();
  g_assert (pair.engine);

  context = g_main_context_new ();
  g_assert (context != NULL);

  pair.loop = g_main_loop_new (context, FALSE);
  g_assert (pair.loop != NULL);

  g_assert (skype_media_engine_start_async (pair.engine,
      SKYPE_MEDIA_AUDIO, "/tmp", context));
  g_main_context_unref (context);

  g_signal_connect (pair.engine, "new-audio-call",
      G_CALLBACK (new_audio_call_ended_cb), &pair);

  client.Connect (SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/"
      SKYPEKIT_MULTIRTP_FROM_SKYPEKIT_FILE, 500);

  /* Complains here as the media engine is trying to connect, but
   * we aren't providing the server for it to connect to */

  client.Init ();

  g_assert_cmpint (client.CreateChannel (1, 1, channel), ==, 1);

  g_main_loop_run (pair.loop);

  g_main_loop_unref (pair.loop);
  g_object_unref (pair.engine);
}

gpointer
start_thread_cb (gpointer data)
{
  SkypeMediaEngine *engine;

  engine = skype_media_engine_new ();

  g_assert (skype_media_engine_start (engine, SKYPE_MEDIA_BOTH,
      SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX));

  g_object_unref (engine);

  return NULL;
}

static void
test_media_engine_start (void)
{
  GThread *thread;
  Sid::SkypeMultiRTPInterfaceClient audio_client;
  Sid::VideoRTPIfClient video_client;
  GError *e;

  thread = g_thread_create (start_thread_cb, NULL, TRUE, &e);
  g_assert (thread != NULL);

  audio_client.Connect (SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/"
      SKYPEKIT_MULTIRTP_FROM_SKYPEKIT_FILE, 1000);
  video_client.Connect (SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/"
      SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE, 1000);

  audio_client.Disconnect();
  video_client.Disconnect();

  g_thread_join (thread);
}

static void
test_media_engine_device_enumeration (void)
{
  GList *devices;
  GList *iter;

  devices = skype_media_engine_enumerate_devices (FALSE);

  for (iter = devices; iter != NULL; iter = g_list_next (iter))
    {
      SkypeDeviceEnumeration *element = (SkypeDeviceEnumeration *)iter->data;
      g_assert_cmpint (element->type, >, DEVICE_ENUMERATION_INVALID);
      g_assert_cmpint (element->type, <=, DEVICE_ENUMERATION_VIDEO_SINK);
      g_assert (element->element != NULL);
    }

  skype_media_engine_free_devices (devices);
}

G_BEGIN_DECLS

void
add_media_engine_tests (void)
{
  g_test_add_func ("/media-engine/init",
      test_media_engine_init);
  g_test_add_func ("/media-engine/no-connection",
      test_media_engine_no_connection);
  g_test_add_func ("/media-engine/connection-but-no-data",
      test_media_engine_connection_but_no_data);
  g_test_add_func ("/media-engine/connection-init",
      test_media_engine_connection_init);
  g_test_add_func ("/media-engine/new-video-call",
      test_media_engine_new_video_call);
  g_test_add_func ("/media-engine/new-video-call-ended",
      test_media_engine_new_video_call_ended);
  g_test_add_func ("/media-engine/new-video-call-add-preview",
      test_media_engine_new_video_call_add_preview);

  g_test_add_func ("/media-engine/new-audio-call",
      test_media_engine_new_audio_call);
  g_test_add_func ("/media-engine/new-audio-call-ended",
      test_media_engine_new_audio_call_ended);

  g_test_add_func ("/media-engine/start",
      test_media_engine_start);
  g_test_add_func ("/media-engine/device-enumeration",
      test_media_engine_device_enumeration);
}

G_END_DECLS
