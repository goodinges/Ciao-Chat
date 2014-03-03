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

#include "skype-media-engine-audio-server.hpp"
#include "skype-media-engine-video-server.hpp"
#include "sidg_multirtpif_cb_client.hpp"
#include "sidg_videortpif_cb_client.hpp"
#include <gst/farsight/fsu-probe.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "skype-audio-call.h"
#include "skype-audio-manager.hpp"
#include "skype-base-call-priv.h"
#include "skype-video-call.h"
#include "skype-video-manager.hpp"

#define SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX "/tmp"
#define SKYPEKIT_MULTIRTP_FROM_SKYPEKIT_FILE "multirtp_from_skypekit_key"
#define SKYPEKIT_MULTIRTP_TO_SKYPEKIT_FILE "multirtp_to_skypekit_key"
#define SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE "vidrtp_from_skypekit_key"
#define SKYPEKIT_VIDEORTP_TO_SKYPEKIT_FILE "vidrtp_to_skypekit_key"

/**
 * SECTION:skype-media-engine
 * @title: SkypeMediaEngine
 * @short_description: object for handling the overall operations of media calls
 * @see_also: #SkypeAudioCall, #SkypeVideoCall
 *
 * <para>
 * The media engine starts, stops, and allows configuration of the audio and
 * video engines. These engines are then connected to through Skype IPC
 * to provide audio and video call support. The media engine has both blocking
 * and non-blocking modes to allow for several different use cases. It also
 * provides signals for new audio and video calls which can then be used to
 * configure individual calls. Devices for audio/video input and output can
 * be enumerated to allow selection through the source and sink properties.
 * </para>
 */

/**
 * SkypeMediaEngine:
 *
 * The SkypeMediaEngine object.
 */

/**
 * SkypeMediaEngineClass:
 *
 * The class of a #SkypeMediaEngine.
 */
G_DEFINE_TYPE (SkypeMediaEngine, skype_media_engine, G_TYPE_OBJECT)

#define SKYPE_MEDIA_ENGINE_GET_PRIVATE(o) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
    SKYPE_TYPE_MEDIA_ENGINE, \
    SkypeMediaEnginePrivate))

typedef struct
{
  SkypeMediaEngine *engine;
  gchar *ipc_path_prefix;
  GMainContext *context;
  gboolean stop;
  GThread *thread;
  Sid::AVServer *av_server;
  SkypeMediaType media_type;
} SkypeMediaEngineThread;

typedef struct
{
  GMainLoop *loop;
  SkypeMediaEngineThread *audio_thread;
  SkypeMediaEngineThread *video_thread;
} SkypeMediaEngineThreadGroup;

struct _SkypeMediaEnginePrivate
{
  gchar *default_audio_source_name;
  gchar *default_audio_source_device;
  gchar *default_audio_source_pipeline;
  gchar *default_audio_sink_name;
  gchar *default_audio_sink_device;
  gchar *default_audio_sink_pipeline;

  gchar *default_video_source_name;
  gchar *default_video_source_device;
  gchar *default_video_source_pipeline;
  gchar *default_video_sink_name;
  gchar *default_video_sink_device;
  gchar *default_video_sink_pipeline;

  GHashTable *groups;
  GMutex *lock;
};

enum
{
  PROP_0,
  PROP_DEFAULT_AUDIO_SOURCE_NAME,
  PROP_DEFAULT_AUDIO_SOURCE_DEVICE,
  PROP_DEFAULT_AUDIO_SOURCE_PIPELINE,
  PROP_DEFAULT_AUDIO_SINK_NAME,
  PROP_DEFAULT_AUDIO_SINK_DEVICE,
  PROP_DEFAULT_AUDIO_SINK_PIPELINE,
  PROP_DEFAULT_VIDEO_SOURCE_NAME,
  PROP_DEFAULT_VIDEO_SOURCE_DEVICE,
  PROP_DEFAULT_VIDEO_SOURCE_PIPELINE,
  PROP_DEFAULT_VIDEO_SINK_NAME,
  PROP_DEFAULT_VIDEO_SINK_DEVICE,
  PROP_DEFAULT_VIDEO_SINK_PIPELINE,
};

enum
{
  SIGNAL_NEW_AUDIO_CALL,
  SIGNAL_NEW_VIDEO_CALL,
  N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

static void
skype_media_engine_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  SkypeMediaEnginePrivate *priv = SKYPE_MEDIA_ENGINE (object)->priv;

  switch (property_id)
    {
      case PROP_DEFAULT_AUDIO_SOURCE_NAME:
        g_value_set_string (value, priv->default_audio_source_name);
        break;
      case PROP_DEFAULT_AUDIO_SOURCE_DEVICE:
        g_value_set_string (value, priv->default_audio_source_device);
        break;
      case PROP_DEFAULT_AUDIO_SOURCE_PIPELINE:
        g_value_set_string (value, priv->default_audio_source_pipeline);
        break;
      case PROP_DEFAULT_AUDIO_SINK_NAME:
        g_value_set_string (value, priv->default_audio_sink_name);
        break;
      case PROP_DEFAULT_AUDIO_SINK_DEVICE:
        g_value_set_string (value, priv->default_audio_sink_device);
        break;
      case PROP_DEFAULT_AUDIO_SINK_PIPELINE:
        g_value_set_string (value, priv->default_audio_sink_pipeline);
        break;
      case PROP_DEFAULT_VIDEO_SOURCE_NAME:
        g_value_set_string (value, priv->default_video_source_name);
        break;
      case PROP_DEFAULT_VIDEO_SOURCE_DEVICE:
        g_value_set_string (value, priv->default_video_source_device);
        break;
      case PROP_DEFAULT_VIDEO_SOURCE_PIPELINE:
        g_value_set_string (value, priv->default_video_source_pipeline);
        break;
      case PROP_DEFAULT_VIDEO_SINK_NAME:
        g_value_set_string (value, priv->default_video_sink_name);
        break;
      case PROP_DEFAULT_VIDEO_SINK_DEVICE:
        g_value_set_string (value, priv->default_video_sink_device);
        break;
      case PROP_DEFAULT_VIDEO_SINK_PIPELINE:
        g_value_set_string (value, priv->default_video_sink_pipeline);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
proxy_default_element (SkypeMediaEngine *self,
    guint property_id,
    const gchar *value)
{
  GHashTableIter iter;
  SkypeMediaEngineThreadGroup *group;
  /* Member function pointers for calling on each manager */
  void (SkypeAudioManager::*audio_ptr)(const gchar *) = NULL;
  void (SkypeVideoManager::*video_ptr)(const gchar *) = NULL;

  /* Pick the corresponding function to call */
  switch (property_id)
    {
      case PROP_DEFAULT_AUDIO_SOURCE_NAME:
        audio_ptr = &SkypeAudioManager::SetSourceName;
        break;
      case PROP_DEFAULT_AUDIO_SOURCE_DEVICE:
        audio_ptr = &SkypeAudioManager::SetSourceDevice;
        break;
      case PROP_DEFAULT_AUDIO_SOURCE_PIPELINE:
        audio_ptr = &SkypeAudioManager::SetSourcePipeline;
        break;
      case PROP_DEFAULT_AUDIO_SINK_NAME:
        audio_ptr = &SkypeAudioManager::SetSinkName;
        break;
      case PROP_DEFAULT_AUDIO_SINK_DEVICE:
        audio_ptr = &SkypeAudioManager::SetSinkDevice;
        break;
      case PROP_DEFAULT_AUDIO_SINK_PIPELINE:
        audio_ptr = &SkypeAudioManager::SetSinkPipeline;
        break;
      case PROP_DEFAULT_VIDEO_SOURCE_NAME:
        video_ptr = &SkypeVideoManager::SetSourceName;
        break;
      case PROP_DEFAULT_VIDEO_SOURCE_DEVICE:
        video_ptr = &SkypeVideoManager::SetSourceDevice;
        break;
      case PROP_DEFAULT_VIDEO_SOURCE_PIPELINE:
        video_ptr = &SkypeVideoManager::SetSourcePipeline;
        break;
      case PROP_DEFAULT_VIDEO_SINK_NAME:
        video_ptr = &SkypeVideoManager::SetSinkName;
        break;
      case PROP_DEFAULT_VIDEO_SINK_DEVICE:
        video_ptr = &SkypeVideoManager::SetSinkDevice;
        break;
      case PROP_DEFAULT_VIDEO_SINK_PIPELINE:
        video_ptr = &SkypeVideoManager::SetSinkPipeline;
        break;
      default:
        g_assert_not_reached ();
        break;
    }

  g_hash_table_iter_init (&iter, self->priv->groups);
  while (g_hash_table_iter_next (&iter, NULL, (gpointer*)&group))
    {
      /* Call the function chosen above for the audio or video manager
       * in each thread group. */
      if (audio_ptr != NULL && group->audio_thread != NULL &&
          group->audio_thread->av_server != NULL)
        ((SkypeAudioManager *)(group->audio_thread->av_server)->*audio_ptr) (
            value);
      else if (video_ptr != NULL && group->video_thread != NULL &&
          group->video_thread->av_server != NULL)
        ((SkypeVideoManager *)(group->video_thread->av_server)->*video_ptr) (
            value);
    }
}

static void
skype_media_engine_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  SkypeMediaEnginePrivate *priv = SKYPE_MEDIA_ENGINE (object)->priv;

  switch (property_id)
    {
      case PROP_DEFAULT_AUDIO_SOURCE_NAME:
        priv->default_audio_source_name = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_AUDIO_SOURCE_DEVICE:
        priv->default_audio_source_device = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_AUDIO_SOURCE_PIPELINE:
        priv->default_audio_source_pipeline = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_AUDIO_SINK_NAME:
        priv->default_audio_sink_name = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_AUDIO_SINK_DEVICE:
        priv->default_audio_sink_device = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_AUDIO_SINK_PIPELINE:
        priv->default_audio_sink_pipeline = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_VIDEO_SOURCE_NAME:
        priv->default_video_source_name = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_VIDEO_SOURCE_DEVICE:
        priv->default_video_source_device = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_VIDEO_SOURCE_PIPELINE:
        priv->default_video_source_pipeline = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_VIDEO_SINK_NAME:
        priv->default_video_sink_name = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_VIDEO_SINK_DEVICE:
        priv->default_video_sink_device = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      case PROP_DEFAULT_VIDEO_SINK_PIPELINE:
        priv->default_video_sink_pipeline = g_value_dup_string (value);
        proxy_default_element (SKYPE_MEDIA_ENGINE (object),
            property_id, g_value_get_string (value));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
skype_media_engine_dispose (GObject *object)
{
  SkypeMediaEnginePrivate *priv = SKYPE_MEDIA_ENGINE (object)->priv;

  g_mutex_lock (priv->lock);
  g_hash_table_unref (priv->groups);
  g_mutex_unlock (priv->lock);

  G_OBJECT_CLASS (skype_media_engine_parent_class)->dispose (object);
}

static void
skype_media_engine_finalize (GObject *object)
{
  SkypeMediaEnginePrivate *priv = SKYPE_MEDIA_ENGINE (object)->priv;

  g_free (priv->default_audio_source_name);
  g_free (priv->default_audio_source_device);
  g_free (priv->default_audio_source_pipeline);
  g_free (priv->default_audio_sink_name);
  g_free (priv->default_audio_sink_device);
  g_free (priv->default_audio_sink_pipeline);

  g_free (priv->default_video_source_name);
  g_free (priv->default_video_source_device);
  g_free (priv->default_video_source_pipeline);
  g_free (priv->default_video_sink_name);
  g_free (priv->default_video_sink_device);
  g_free (priv->default_video_sink_pipeline);

  g_mutex_free (priv->lock);

  G_OBJECT_CLASS (skype_media_engine_parent_class)->finalize (object);
}

static void skype_media_engine_thread_group_free (
    SkypeMediaEngineThreadGroup *group);

static void
skype_media_engine_class_init (SkypeMediaEngineClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (SkypeMediaEnginePrivate));

  object_class->get_property = skype_media_engine_get_property;
  object_class->set_property = skype_media_engine_set_property;
  object_class->dispose = skype_media_engine_dispose;
  object_class->finalize = skype_media_engine_finalize;

  /**
   * SkypeMediaEngine:audio-source-name:
   *
   * The name of the audio source element to use for new audio sources.
   *
   * Set to NULL to indicate the automatic detection of the audio source name.
   */
  pspec = g_param_spec_string ("audio-source-name",
      "Default Audio Source Name",
      "The default source name to be used for new audio sources in the engine",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_AUDIO_SOURCE_NAME, pspec);

  /**
   * SkypeMediaEngine:audio-source-device:
   *
   * The name of the audio source device to use for new audio sources.
   *
   * Set to NULL to indicate the automatic detection of the audio device name.
   *
   * This property will be ignored if SkypeMediaEngine:audio-source-name is set to NULL.
   */
  pspec = g_param_spec_string ("audio-source-device",
      "Default Audio Source Device",
      "The default source device to be used for new audio sources",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_AUDIO_SOURCE_DEVICE, pspec);

  /**
   * SkypeMediaEngine:audio-source-pipeline:
   *
   * A string description representing the audio source pipeline.
   *
   * This property uses the same form as gst_parse_launch. It overrides the
   * audio-source-name and audio-source-device properties.
   * Set to NULL to disable using a custom pipeline.
   */
  pspec = g_param_spec_string ("audio-source-pipeline",
      "Default Audio Source Pipeline",
      "The default source pipeline to be used for new audio sources",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_AUDIO_SOURCE_PIPELINE, pspec);

  /**
   * SkypeMediaEngine:audio-sink-name:
   *
   * The name of the audio sink element to use for new audio sinks.
   *
   * Set to NULL to indicate the automatic detection of the audio sink name.
   */
  pspec = g_param_spec_string ("audio-sink-name",
      "Default Audio Sink Name",
      "The default sink name to be used for new audio sinks",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_AUDIO_SINK_NAME, pspec);

  /**
   * SkypeMediaEngine:audio-sink-device:
   *
   * The name of the audio sink device to use for new audio sinks.
   *
   * Set to NULL to indicate the automatic detection of the audio sink device.
   *
   * This property will be ignored if SkypeMediaEngine:audio-sink-name is set to NULL.
   */
  pspec = g_param_spec_string ("audio-sink-device",
      "Default Audio Sink Device",
      "The default sink device to be used for new audio sinks",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_AUDIO_SINK_DEVICE, pspec);

  /**
   * SkypeMediaEngine:audio-sink-pipeline:
   *
   * The description of the audio sink pipeline to use for new audio sinks.
   *
   * This property uses the same form as gst_parse_launch. It overrides the
   * audio-sink-name and audio-sink-device properties.
   * Set to NULL to disable using a custom pipeline.
   */
  pspec = g_param_spec_string ("audio-sink-pipeline",
      "Default Audio Sink Pipeline",
      "The default sink pipeline to be used for new audio sinks",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_AUDIO_SINK_PIPELINE, pspec);


  /**
   * SkypeMediaEngine:video-source-name:
   *
   * The name of the video source element to use for new video sources.
   *
   * Set to NULL to indicate the automatic detection of the video source name.
   */
  pspec = g_param_spec_string ("video-source-name",
      "Default Video Source Name",
      "The default source name to be used for new video sources in the engine",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_VIDEO_SOURCE_NAME, pspec);

  /**
   * SkypeMediaEngine:video-source-device:
   *
   * The name of the video source device to use for new video sources.
   *
   * Set to NULL to indicate the automatic detection of the video device name.
   *
   * This property will be ignored if SkypeMediaEngine:video-source-name is set to NULL.
   */
  pspec = g_param_spec_string ("video-source-device",
      "Default Video Source Device",
      "The default source device to be used for new video sources",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_VIDEO_SOURCE_DEVICE, pspec);

  /**
   * SkypeMediaEngine:video-source-pipeline:
   *
   * A string description representing the video source pipeline.
   *
   * This property uses the same form as gst_parse_launch. It overrides the
   * video-source-name and video-source-device properties.
   * Set to NULL to disable using a custom pipeline.
   */
  pspec = g_param_spec_string ("video-source-pipeline",
      "Default Video Source Pipeline",
      "The default source pipeline to be used for new video sources",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_VIDEO_SOURCE_PIPELINE, pspec);

  /**
   * SkypeMediaEngine:video-sink-name:
   *
   * The name of the video sink element to use for new audio sinks.
   *
   * Set to NULL to indicate the automatic detection of the audio sink name.
   */
  pspec = g_param_spec_string ("video-sink-name",
      "Default Video Sink Name",
      "The default sink name to be used for new video sinks",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_VIDEO_SINK_NAME, pspec);

  /**
   * SkypeMediaEngine:video-sink-device:
   *
   * The name of the video sink device to use for new video sinks.
   *
   * Set to NULL to indicate the automatic detection of the video sink device.
   *
   * This property will be ignored if SkypeMediaEngine:video-sink-name is set to NULL.
   */
  pspec = g_param_spec_string ("video-sink-device",
      "Default Video Sink Device",
      "The default sink device to be used for new video sinks",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_VIDEO_SINK_DEVICE, pspec);

  /**
   * SkypeMediaEngine:video-sink-pipeline:
   *
   * The description of the video sink pipeline to use for new video sinks.
   *
   * This property uses the same form as gst_parse_launch. It overrides the
   * video-sink-name and video-sink-device properties.
   * Set to NULL to disable using a custom pipeline.
   */
  pspec = g_param_spec_string ("video-sink-pipeline",
      "Default Video Sink Pipeline",
      "The default sink pipeline to be used for new video sinks",
      NULL,
      (GParamFlags)(G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));
  g_object_class_install_property (object_class,
      PROP_DEFAULT_VIDEO_SINK_PIPELINE, pspec);

  /**
   * SkypeMediaEngine::new-audio-call
   * @engine: the #SkypeMediaEngine which contains the new #SkypeAudioCall
   * @call: the new #SkypeAudioCall this signal represents
   *
   * Emitted when a new audio call is created.
   *
   * The call object needs to be reffed if it's to be used past
   * the return of this signal call.
   */
  signals[SIGNAL_NEW_AUDIO_CALL] = g_signal_new ("new-audio-call",
      G_TYPE_FROM_CLASS (object_class),
      (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED),
      0, NULL, NULL, g_cclosure_marshal_VOID__OBJECT, G_TYPE_NONE,
      1, SKYPE_TYPE_AUDIO_CALL);

  /**
   * SkypeMediaEngine::new-video-call
   * @engine: the #SkypeMediaEngine which contains the new #SkypeVideoCall
   * @call: the new #SkypeVideoCall this signal represents
   *
   * Emitted when a new video call is created.
   *
   * The call object needs to be reffed if it's to be used past
   * the return of this signal call.
   */
  signals[SIGNAL_NEW_VIDEO_CALL] = g_signal_new ("new-video-call",
      G_TYPE_FROM_CLASS (object_class),
      (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED),
      0, NULL, NULL, g_cclosure_marshal_VOID__OBJECT, G_TYPE_NONE,
      1, SKYPE_TYPE_VIDEO_CALL);
}

static void
skype_media_engine_init (SkypeMediaEngine *self)
{
  self->priv = SKYPE_MEDIA_ENGINE_GET_PRIVATE (self);

  self->priv->lock = g_mutex_new ();

  self->priv->groups = g_hash_table_new_full (g_str_hash, g_str_equal,
      g_free, (GDestroyNotify)skype_media_engine_thread_group_free);
}

/**
 * skype_media_engine_new
 *
 * Creates a new #SkypeMediaEngine.
 *
 * Returns: the newly a created #SkypeMediaEngine
 */
SkypeMediaEngine *
skype_media_engine_new (void)
{
  return SKYPE_MEDIA_ENGINE (g_object_new (SKYPE_TYPE_MEDIA_ENGINE, NULL));
}

static SkypeMediaEngineThread *
skype_media_engine_thread_new (SkypeMediaEngine *engine,
    const gchar *ipc_path_prefix,
    SkypeMediaType media_type,
    GMainContext *context)
{
  SkypeMediaEngineThread *thread = g_slice_new0 (SkypeMediaEngineThread);
  thread->engine = (SkypeMediaEngine *)g_object_ref (engine);
  thread->ipc_path_prefix = g_strdup (ipc_path_prefix);
  thread->media_type = media_type;
  if (context != NULL)
    thread->context = g_main_context_ref (context);
  return thread;
}

static void
skype_media_engine_thread_free (SkypeMediaEngineThread *thread)
{
  if (thread->stop != TRUE)
    thread->stop = TRUE;

  if (thread->thread != NULL)
    {
      struct sockaddr_un un;
      gchar *path;

      if (thread->av_server != NULL)
        thread->av_server->Disconnect ();

      if (thread->media_type == SKYPE_MEDIA_AUDIO)
        path = g_build_filename (thread->ipc_path_prefix,
            SKYPEKIT_MULTIRTP_FROM_SKYPEKIT_FILE, NULL);
      else
        path = g_build_filename (thread->ipc_path_prefix,
            SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE, NULL);

      if (strlen (path) + 1 > sizeof (un.sun_path))
        {
          /* No buffer overflows here thanks */
          g_warning ("SkypeMediaEngine: ipc path to long to create socket");
        }
      else
        {
          int sock;

          un.sun_family = AF_UNIX;
          strcpy (un.sun_path, path);

          /* Connect to socket to knock it out of listening for a connection */
          sock =  socket (AF_UNIX, SOCK_STREAM, 0);
          connect (sock, (sockaddr *)&un, sizeof (struct sockaddr_un));
          close (sock);
        }

      g_free (path);

      g_thread_join (thread->thread);
    }

  g_object_unref (thread->engine);
  g_free (thread->ipc_path_prefix);

  if (thread->context != NULL)
    g_main_context_unref (thread->context);

  g_slice_free (SkypeMediaEngineThread, thread);
}

static void
skype_media_engine_thread_group_free (SkypeMediaEngineThreadGroup *group)
{
  if (group->audio_thread != NULL)
    skype_media_engine_thread_free (group->audio_thread);

  if (group->video_thread != NULL)
    skype_media_engine_thread_free (group->video_thread);

  if (group->loop != NULL)
    g_main_loop_quit (group->loop);
}

static void
new_audio_stream_callback (SkypeAudioManager *manager,
    SkypeAudioStream *stream,
    gpointer user_data)
{
  SkypeMediaEngineThread *thread = (SkypeMediaEngineThread *)user_data;
  SkypeAudioCall *call;

  call = skype_audio_call_new (thread->ipc_path_prefix, stream);
  g_signal_emit (thread->engine, signals[SIGNAL_NEW_AUDIO_CALL], 0, call);
  g_object_unref (call);
}

static void
new_video_stream_callback (SkypeVideoManager *manager,
    SkypeVideoStream *stream,
    gpointer user_data)
{
  SkypeMediaEngineThread *thread = (SkypeMediaEngineThread *)user_data;
  SkypeVideoCall *call;

  call = skype_video_call_new (thread->ipc_path_prefix, stream);
  g_signal_emit (thread->engine, signals[SIGNAL_NEW_VIDEO_CALL], 0, call);
  g_object_unref (call);
}

typedef struct
{
  SkypeMediaEngine *engine;
  gchar *ipc_path_prefix;
  SkypeMediaType media_type;
} SkypeMediaEngineFreeThreadData;

static void
skype_media_engine_free_thread_data_free (gpointer user_data)
{
  SkypeMediaEngineFreeThreadData *data =
      (SkypeMediaEngineFreeThreadData *)user_data;

  g_free (data->ipc_path_prefix);
  g_object_unref (data->engine);
}

static gboolean
thread_ended (gpointer user_data)
{
  SkypeMediaEngineFreeThreadData *data =
      (SkypeMediaEngineFreeThreadData *)user_data;
  SkypeMediaEnginePrivate *priv = data->engine->priv;
  SkypeMediaEngineThreadGroup *group;

  g_mutex_lock (priv->lock);

  group = (SkypeMediaEngineThreadGroup *)g_hash_table_lookup (priv->groups,
      data->ipc_path_prefix);

  if (group == NULL)
    {
      g_debug ("Tried to free thread, but it wasn't found");
    }
  else if (group->audio_thread == NULL || group->video_thread == NULL)
    {
      g_debug ("Freeing thread group");
      g_hash_table_remove (priv->groups, data->ipc_path_prefix);
    }
  else if (data->media_type == SKYPE_MEDIA_AUDIO && group->audio_thread != NULL)
    {
      g_debug ("Freeing audio thread");
      skype_media_engine_thread_free (group->audio_thread);
      group->audio_thread = NULL;
    }
  else if (data->media_type == SKYPE_MEDIA_VIDEO && group->video_thread != NULL)
    {
      g_debug ("Freeing video thread");
      skype_media_engine_thread_free (group->video_thread);
      group->video_thread = NULL;
    }
  else
    {
      g_assert_not_reached ();
    }

  g_mutex_unlock (priv->lock);

  return FALSE;
}

static gpointer
handle_thread_cb (gpointer user_data)
{
  SkypeMediaEngineThread *thread = (SkypeMediaEngineThread *)user_data;
  Sid::Protocol::Status status = Sid::Protocol::OK;
  Sid::AVClient *av_client;
  SkypeAudioManager *audio_manager = NULL;
  SkypeVideoManager *video_manager = NULL;
  const gchar *server_file;
  const gchar *client_file;
  gchar *path;
  SkypeMediaEngineFreeThreadData *data;
  GSource *source;

  if (thread->media_type == SKYPE_MEDIA_AUDIO)
    {
      Sid::SkypeMultiRTPCallbackInterfaceClient *client;
      SkypeMediaEnginePrivate *priv = thread->engine->priv;
      SkypeAudioManager *manager;

      thread->av_server = skype_media_engine_audio_server_new ();
      client = new Sid::SkypeMultiRTPCallbackInterfaceClient;

      manager = new SkypeAudioManager (client, thread->context);
      manager->SetNewStreamCallback (new_audio_stream_callback, user_data);
      manager->SetSourceName (priv->default_audio_source_name);
      manager->SetSourceDevice (priv->default_audio_source_device);
      manager->SetSourcePipeline (priv->default_audio_source_pipeline);
      manager->SetSinkName (priv->default_audio_sink_name);
      manager->SetSinkDevice (priv->default_audio_sink_device);
      manager->SetSinkPipeline (priv->default_audio_sink_pipeline);

      av_client = client;
      thread->av_server->set_if (manager);
      audio_manager = manager;

      server_file = SKYPEKIT_MULTIRTP_FROM_SKYPEKIT_FILE;
      client_file = SKYPEKIT_MULTIRTP_TO_SKYPEKIT_FILE;
    }
  else if (thread->media_type == SKYPE_MEDIA_VIDEO)
    {
      Sid::SkypeVideoRTPInterfaceCbClient *client;
      SkypeMediaEnginePrivate *priv = thread->engine->priv;
      SkypeVideoManager *manager;

      thread->av_server = skype_media_engine_video_server_new ();
      client = new Sid::SkypeVideoRTPInterfaceCbClient;

      manager = new SkypeVideoManager (client, thread->context);
      manager->SetNewStreamCallback (new_video_stream_callback, user_data);
      manager->SetSourceName (priv->default_video_source_name);
      manager->SetSourceDevice (priv->default_video_source_device);
      manager->SetSourcePipeline (priv->default_video_source_pipeline);
      manager->SetSinkName (priv->default_video_sink_name);
      manager->SetSinkDevice (priv->default_video_sink_device);
      manager->SetSinkPipeline (priv->default_video_sink_pipeline);

      av_client = client;
      thread->av_server->set_if (manager);
      video_manager = manager;

      server_file = SKYPEKIT_VIDEORTP_FROM_SKYPEKIT_FILE;
      client_file = SKYPEKIT_VIDEORTP_TO_SKYPEKIT_FILE;
    }
  else
    {
      g_assert_not_reached ();
    }

  path = g_build_filename (thread->ipc_path_prefix, server_file, NULL);

  while (!thread->stop)
    {
      if (thread->av_server->Connect (path, 0) == 1)
        break;
    }

  g_free (path);

  if (thread->stop)
    goto escape;

  path = g_build_filename (thread->ipc_path_prefix, client_file, NULL);

  av_client->Connect (path, 500);

  g_free (path);

  while (!thread->stop && status == Sid::Protocol::OK)
    {
      status = thread->av_server->ProcessCommands();
    }

escape:
  thread->av_server->Disconnect ();
  av_client->Disconnect ();

  delete thread->av_server;
  delete av_client;

  if (audio_manager != NULL)
    delete audio_manager;

  if (video_manager != NULL)
    delete video_manager;

  g_debug ("SkypeMediaEngine: Stopping thread");

  data = (SkypeMediaEngineFreeThreadData *)
      g_slice_new0 (SkypeMediaEngineFreeThreadData);
  data->engine = (SkypeMediaEngine *)g_object_ref (thread->engine);
  data->ipc_path_prefix = g_strdup (thread->ipc_path_prefix);
  data->media_type = thread->media_type;

  source = g_idle_source_new ();
  g_source_set_callback (source, thread_ended, data,
      skype_media_engine_free_thread_data_free);
  g_source_attach (source, thread->context);

  return NULL;
}

static SkypeMediaEngineThreadGroup *
skype_media_engine_start_async_internal (SkypeMediaEngine *engine,
    SkypeMediaType type,
    const gchar *ipc_path_prefix,
    GMainContext *context)
{
  SkypeMediaEnginePrivate *priv = engine->priv;
  SkypeMediaEngineThreadGroup *group;

  if ((type & SKYPE_MEDIA_BOTH) == 0)
    {
      g_warning ("Tried to start the media engine with no media types");
      return NULL;
    }

  if (ipc_path_prefix == NULL)
    ipc_path_prefix = SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX;

  group = g_slice_new0 (SkypeMediaEngineThreadGroup);

  /* Create SkypeMediaType's with GMainContext and ipc_path_prefix */
  if (type & SKYPE_MEDIA_AUDIO)
    {
      SkypeMediaEngineThread *thread;
      GError *e = NULL;

      thread = skype_media_engine_thread_new (engine, ipc_path_prefix,
          SKYPE_MEDIA_AUDIO, context);
      g_debug ("SkypeMediaEngine: Starting thread.");

      thread->thread = g_thread_create (handle_thread_cb,
          thread, TRUE, &e);

      if (thread->thread == NULL)
        {
          g_warning ("Error starting the SkypeMediaEngine: %s", e->message);
          g_error_free (e);
          skype_media_engine_thread_free (thread);
          goto error;
        }

      group->audio_thread = thread;
    }

  if (type & SKYPE_MEDIA_VIDEO)
    {
      SkypeMediaEngineThread *thread;
      GError *e = NULL;

      thread = skype_media_engine_thread_new (engine, ipc_path_prefix,
          SKYPE_MEDIA_VIDEO, context);
      g_debug ("SkypeMediaEngine: Starting thread.");

      thread->thread = g_thread_create (handle_thread_cb,
          thread, TRUE, &e);

      if (thread->thread == NULL)
        {
          g_warning ("Error starting the SkypeMediaEngine: %s", e->message);
          g_error_free (e);
          skype_media_engine_thread_free (thread);
          goto error;
        }

      group->video_thread = thread;
    }

  g_mutex_lock (priv->lock);
  g_hash_table_insert (priv->groups, g_strdup (ipc_path_prefix), group);
  g_mutex_unlock (priv->lock);

  return group;

error:
  if (group != NULL)
    {
      if (group->audio_thread != NULL)
        {
          skype_media_engine_thread_free (group->audio_thread);
        }

      if (group->video_thread != NULL)
        {
          skype_media_engine_thread_free (group->video_thread);
        }

      g_slice_free (SkypeMediaEngineThreadGroup, group);
    }

  return NULL;
}

/**
 * skype_media_engine_start
 * @self: the #SkypeMediaEngine to use to start media
 * @type: the #SkypeMediaType<!-- -->s to start
 * @ipc_path_prefix: the path prefix of the IPC socket used to communicate with
 * Skypekit or %NULL. This is the same value passed when starting Skypekit.
 * Defaults to '/tmp' if set to %NULL.
 * 
 * Starts the #SkypeMediaEngine with the passed media types. This function
 * blocks until all started media types have stopped. The @ipc_path_prefix is
 * the same value passed when starting Skypekit. It defaults to '/tmp' if set
 * to %NULL.
 *
 * See Also: skype_media_engine_start_async(), skype_media_engine_stop()
 *
 * Returns: the %TRUE on success; %FALSE otherwise
 */
gboolean
skype_media_engine_start (SkypeMediaEngine *engine,
    SkypeMediaType type,
    const gchar *ipc_path_prefix)
{
  GMainContext *context;
  GMainLoop *loop;
  SkypeMediaEngineThreadGroup *group;

  context = g_main_context_new ();

  if (context == NULL)
    {
      g_warning ("Unable to create a new context");
      return FALSE;
    }

  loop = g_main_loop_new (context, FALSE);

  if (loop == NULL)
    {
      g_warning ("Unable to create a new main loop");
      g_main_context_unref (context);
      return FALSE;
    }

  group = skype_media_engine_start_async_internal (engine, type,
      ipc_path_prefix, context);

  if (group == NULL)
    {
      g_main_loop_unref (loop);
      g_main_context_unref (context);
      return FALSE;
    }

  group->loop = loop;
  g_main_loop_run (loop);
  g_main_loop_unref (loop);
  return TRUE;
}

/**
 * skype_media_engine_start_async
 * @self: the #SkypeMediaEngine to use to start media.
 * @type: the #SkypeMediaType<!-- -->s to start
 * @ipc_path_prefix: the path prefix of the IPC socket used to communicate with
 * Skypekit or %NULL. This is the same value passed when starting Skypekit.
 * Defaults to '/tmp' if set to %NULL.
 * @context: the #GMainContext on which to run internal processes or %NULL.
 * 
 * Starts the #SkypeMediaEngine with the passed media types. This is the
 * asynchronous version. It requires a valid #GMainContext or %NULL for the
 * default #GMainContext. The #GMainContext must be in a running #GMainLoop
 * for the #SkypeMediaEngine to function. Therefore, the #GMainLoop must be
 * started soon after this function is called. The @ipc_path_prefix is the
 * same value passed when starting Skypekit. It defaults to '/tmp' if set
 * to %NULL. For the blocking version see skype_media_engine_start().
 *
 * See Also: skype_media_engine_start(), skype_media_engine_stop()
 *
 * Returns: the %TRUE on success; %FALSE otherwise
 */
gboolean
skype_media_engine_start_async (SkypeMediaEngine *engine,
    SkypeMediaType type,
    const gchar *ipc_path_prefix,
    GMainContext *context)
{
  return (skype_media_engine_start_async_internal (engine,
      type, ipc_path_prefix, context) != NULL);
}

/**
 * skype_media_engine_stop
 * @self: the #SkypeMediaEngine which contains the media you want to stop.
 * @ipc_path_prefix: the path prefix of the IPC socket used to communicate with
 * Skypekit or %NULL.
 * 
 * Stops the #SkypeMediaEngine matching the passed @ipc_path_prefix. This function
 * blocks until all started media types have stopped. The @ipc_path_prefix
 * is the same value passed to skype_media_engine_start() or
 * skype_media_engine_start_async(). It defaults to '/tmp' if set to %NULL.
 *
 * See Also: skype_media_engine_start(), skype_media_engine_start_async()
 *
 * Returns: the %TRUE on success; %FALSE otherwise
 */
void
skype_media_engine_stop (SkypeMediaEngine *engine,
    const gchar *ipc_path_prefix)
{
  SkypeMediaEnginePrivate *priv = engine->priv;

  if (ipc_path_prefix == NULL)
    ipc_path_prefix = SKYPE_MEDIA_ENGINE_DEFAULT_IPC_PATH_PREFIX;

  g_mutex_lock (priv->lock);

  if (!g_hash_table_remove (priv->groups, ipc_path_prefix))
    g_debug ("Couldn't find thread group to stop: %s", ipc_path_prefix);

  g_mutex_unlock (priv->lock);
}

/**
 * skype_media_engine_enumerate_devices
 * @full: Whether or not to return the full list of sources and sinks.
 * 
 * Probes the system and retrieves a list of available devices for audio and
 * video sources and sinks. If @full is %FALSE it returns an abridged list of
 * the recommended sources and sinks; %TRUE returns all the sources and sinks
 * installed on the machine.
 *
 * See Also: skype_media_engine_free_devices()
 *
 * Returns: a #GList * of #SkypeDeviceEnumeration<!-- -->s of the
 * requested devices. Free with skype_media_engine_free_devices().
 */
GList *
skype_media_engine_enumerate_devices (gboolean full)
{
  GList *fsu_devices;
  GList *devices = NULL;
  GList *iter;

  fsu_devices = fsu_probe_devices (full);

  for (iter = fsu_devices; iter != NULL; iter = g_list_next (iter))
    {
      FsuProbeDeviceElement *fsu_element = (FsuProbeDeviceElement *)iter->data;
      SkypeDeviceEnumeration *element;
      GList *iter2;

      element = g_slice_new0 (SkypeDeviceEnumeration);
      element->type = (DeviceEnumerationType)(fsu_element->type + 1);
      element->element = g_strdup (fsu_element->element);
      element->name = g_strdup (fsu_element->name);
      element->description = g_strdup (fsu_element->description);

      for (iter2 = element->devices; iter2 != NULL; iter2 = g_list_next (iter2))
        {
          element->devices = g_list_prepend (element->devices,
              g_strdup ((gchar *)iter2->data));
        }

      element->devices = g_list_reverse (element->devices);

      devices = g_list_prepend (devices, element);
    }

  devices = g_list_reverse (devices);

  fsu_probe_free (fsu_devices);

  return devices;
}

/**
 * skype_media_engine_free_devices
 * @devices: A #GList of #SkypeDeviceEnumeration<!-- -->s to free.
 * 
 * Frees a list of #SkypeDeviceEnumeration<!-- -->s returned by
 * skype_media_engine_enumerate_devices().
 *
 * See Also: skype_media_engine_enumerate_devices()
 */
void
skype_media_engine_free_devices (GList *devices)
{
  GList *iter;

  for (iter = devices; iter != NULL; iter = g_list_next (iter))
    {
      SkypeDeviceEnumeration *element = (SkypeDeviceEnumeration *)iter->data;

      g_free (element->element);
      g_free (element->name);
      g_free (element->description);

      g_list_foreach (element->devices, (GFunc)g_free, NULL);
      g_list_free (element->devices);
      g_slice_free (SkypeDeviceEnumeration, element);
    }

  g_list_free (devices);
}

