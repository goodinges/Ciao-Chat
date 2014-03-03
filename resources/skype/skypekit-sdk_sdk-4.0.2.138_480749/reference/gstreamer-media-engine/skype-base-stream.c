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

#include "skype-base-stream.h"

#include <gst/farsight/fsu-conference.h>
#include <gst/farsight/fsu-stream.h>
#include <gst/filters/fsu-filter-manager.h>

#include <shm-writer.h>
#include <shm-reader.h>

#include "skype-utils.h"

/**
 * SECTION:skype-base-stream
 * @title: SkypeBaseStream
 * @short_description: base object for skype media streams
 * @see_also: #SkypeAudioStream #SkypeVideoStream
 *
 * The #SkypeBaseStream object is derived to create media streams for Skype
 * calls.
 */

/**
 * SkypeBaseStream:
 *
 * The SkypeBaseStream is a base class that takes care of most of the
 * required Farsight 2 setup and operation for media calls.
 */

/**
 * SkypeBaseStream:
 *
 * The class of a #SkypeBaseStream.
 */

G_DEFINE_ABSTRACT_TYPE (SkypeBaseStream, skype_base_stream, G_TYPE_OBJECT)

#define SKYPE_BASE_STREAM_GET_PRIVATE(o) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
    SKYPE_TYPE_BASE_STREAM, SkypeBaseStreamPrivate))

#define SKYPE_BASE_STREAM_DEFAULT_FS_SOCKET_PATH "/tmp/base-stream-fs-writer"
#define SKYPE_BASE_STREAM_DEFAULT_SHM_SOCKET_PATH "/tmp/base-stream-shm-writer"

struct _SkypeBaseStreamPrivate
{
  GMainContext *context;

  FsCodec *codec;
  guint bitrate;

  FsuSource *source;
  gchar *source_name;
  gchar *source_device;
  gchar *source_pipeline;

  FsuSink *sink;
  gchar *sink_name;
  gchar *sink_device;
  gchar *sink_pipeline;

  ShmWriter *shm_writer;
  ShmReader *shm_reader;

  FsuStream *stream;
  FsuConference *conf;
  GstElement *pipeline;

  GSource *watch_source;
};

enum
{
  PROP_0,
  PROP_CODEC,
  PROP_CONTEXT,
  PROP_BITRATE,
  PROP_MEDIA_TYPE,
  PROP_PIPELINE,
  PROP_SOURCE,
  PROP_SOURCE_NAME,
  PROP_SOURCE_DEVICE,
  PROP_SOURCE_PIPELINE,
  PROP_SINK,
  PROP_SINK_NAME,
  PROP_SINK_DEVICE,
  PROP_SINK_PIPELINE,
  PROP_INCOMING_FILTER,
  PROP_OUTGOING_FILTER,
};

enum
{
  SIGNAL_BUFFER_PREPARED,
  N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

static void
shm_reader_got_data_cb (ShmReader *reader,
    gsize len,
    const gchar *buffer,
    gpointer user_data)
{
  SkypeBaseStream *self = SKYPE_BASE_STREAM (user_data);

  g_signal_emit (self, signals[SIGNAL_BUFFER_PREPARED], 0, len, buffer);
}

static void
skype_base_stream_constructed (GObject *object)
{
  SkypeBaseStreamPrivate *priv = SKYPE_BASE_STREAM (object)->priv;

  priv->shm_writer = shm_writer_new_with_context (priv->context);
  priv->shm_reader = shm_reader_new_with_context (priv->context);

  g_signal_connect (priv->shm_reader, "got-data",
      G_CALLBACK (shm_reader_got_data_cb), object);
}

static void
skype_base_stream_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  SkypeBaseStreamPrivate *priv = SKYPE_BASE_STREAM (object)->priv;

  switch (property_id)
    {
      case PROP_CODEC:
        g_value_set_boxed (value, priv->codec);
        break;
      case PROP_CONTEXT:
        if (priv->context != NULL)
          g_main_context_ref (priv->context);
        g_value_set_pointer (value, priv->context);
        break;
      case PROP_BITRATE:
        g_value_set_uint (value, priv->bitrate);
        break;
      case PROP_PIPELINE:
        {
          GstPipeline *pipeline = NULL;

          if (priv->stream != NULL)
            {
              FsuConference *conf;

              g_object_get (priv->stream, "fsu-conference", &conf, NULL);

              if (conf == NULL)
                {
                  g_warning ("Error fetching FsuConference");
                }
              else
                {
                  g_object_get (conf, "pipeline", &pipeline, NULL);
                  g_object_unref (conf);
                }
            }

          g_value_take_object (value, pipeline);
        }
        break;
      case PROP_SOURCE:
        g_value_set_object (value, priv->source);
        break;
      case PROP_SOURCE_NAME:
        g_value_set_string (value, priv->source_name);
        break;
      case PROP_SOURCE_DEVICE:
        g_value_set_string (value, priv->source_device);
        break;
      case PROP_SOURCE_PIPELINE:
        g_value_set_string (value, priv->source_pipeline);
        break;
      case PROP_SINK:
        g_value_set_object (value, priv->sink);
        break;
      case PROP_SINK_NAME:
        g_value_set_string (value, priv->sink_name);
        break;
      case PROP_SINK_DEVICE:
        g_value_set_string (value, priv->sink_device);
        break;
      case PROP_SINK_PIPELINE:
        g_value_set_string (value, priv->sink_pipeline);
        break;
      case PROP_INCOMING_FILTER:
        if (priv->stream != NULL)
          {
            FsuFilterManager *manager = NULL;
            g_object_get (priv->stream, "filter-manager", &manager, NULL);

            if (manager != NULL)
              {
                g_value_take_object (value, manager);
                break;
              }
          }

        g_value_set_object (value, NULL);
        break;
      case PROP_OUTGOING_FILTER:
        if (priv->stream != NULL)
          {
            FsuSession *session = NULL;
            g_object_get (priv->stream, "fsu-session", &session, NULL);

            if (session != NULL)
              {
                FsuFilterManager *manager = NULL;
                g_object_get (session, "filter-manager", &manager, NULL);
                g_object_unref (session);

                if (manager != NULL)
                  {
                    g_value_take_object (value, manager);
                    break;
                  }
              }
          }

        g_value_set_object (value, NULL);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static gboolean
skype_base_stream_update_codecs (SkypeBaseStream *stream)
{
  SkypeBaseStreamPrivate *priv = stream->priv;

  if (priv->stream != NULL && priv->codec != NULL)
    {
      SkypeBaseStreamClass *klass = SKYPE_BASE_STREAM_GET_CLASS (stream);
      FsStream *fsstream;
      GList *list = NULL;
      GError *e = NULL;

      g_object_get (priv->stream, "fs-stream", &fsstream, NULL);

      if (fsstream == NULL)
        {
          g_warning ("Error fetching FsStream");
          return FALSE;
        }

      list = g_list_prepend (NULL, fs_codec_copy (priv->codec));

      if (klass->update_codecs != NULL)
        klass->update_codecs (stream, &list);

      if (!fs_stream_set_remote_codecs (fsstream, list, &e))
        {
          g_warning ("Error setting remote codecs: %s", e->message);
          return FALSE;
        }

      fs_codec_list_destroy (list);
      g_object_unref (fsstream);
    }

  return TRUE;
}

static void
skype_base_stream_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  SkypeBaseStreamPrivate *priv = SKYPE_BASE_STREAM (object)->priv;

  switch (property_id)
    {
      case PROP_CODEC:
        fs_codec_destroy (priv->codec);
        priv->codec = g_value_dup_boxed (value);
        skype_base_stream_update_codecs (SKYPE_BASE_STREAM (object));
        break;
      case PROP_CONTEXT:
        if (priv->context != NULL)
          g_main_context_unref (priv->context);
        priv->context = g_value_get_pointer (value);
        if (priv->context != NULL)
          g_main_context_ref (priv->context);
        break;
      case PROP_BITRATE:
        priv->bitrate = g_value_get_uint (value);
        break;
      case PROP_PIPELINE:
        g_assert (priv->pipeline == NULL);
        priv->pipeline = g_value_dup_object (value);
        break;
      case PROP_SOURCE:
        g_assert (priv->source == NULL);
        priv->source = g_value_dup_object (value);
        break;
      case PROP_SOURCE_NAME:
        g_free (priv->source_name);
        priv->source_name = g_value_dup_string (value);

        if (priv->stream != NULL)
          {
            FsuSession *session;
            FsuSource *source;

            g_object_get (priv->stream, "fsu-session", &session, NULL);

            if (session == NULL)
              {
                g_warning ("Error fetching FsuSession");
                break;
              }

            g_object_get (session, "source", &source, NULL);
            g_object_unref (session);

            if (source == NULL)
              {
                g_warning ("Error fetching source");
                break;
              }

            g_object_set (source, "source-name", priv->source_name, NULL);
            g_object_unref (source);
          }
        break;
      case PROP_SOURCE_DEVICE:
        g_free (priv->source_device);
        priv->source_device = g_value_dup_string (value);

        if (priv->stream != NULL)
          {
            FsuSession *session;
            FsuSource *source;

            g_object_get (priv->stream, "fsu-session", &session, NULL);

            if (session == NULL)
              {
                g_warning ("Error fetching FsuSession");
                break;
              }

            g_object_get (session, "source", &source, NULL);
            g_object_unref (session);

            if (source == NULL)
              {
                g_warning ("Error fetching source");
                break;
              }

            g_object_set (source, "source-device", priv->source_device, NULL);
            g_object_unref (source);
          }
        break;
      case PROP_SOURCE_PIPELINE:
        g_free (priv->source_pipeline);
        priv->source_pipeline = g_value_dup_string (value);

        if (priv->stream != NULL)
          {
            FsuSession *session;
            FsuSource *source;

            g_object_get (priv->stream, "fsu-session", &session, NULL);

            if (session == NULL)
              {
                g_warning ("Error fetching FsuSession");
                break;
              }

            g_object_get (session, "source", &source, NULL);
            g_object_unref (session);

            if (source == NULL)
              {
                g_warning ("Error fetching source");
                break;
              }

            g_object_set (source, "source-pipeline",
                priv->source_pipeline, NULL);
            g_object_unref (source);
          }
        break;
      case PROP_SINK:
        g_assert (priv->sink == NULL);
        priv->sink = g_value_dup_object (value);
        break;
      case PROP_SINK_NAME:
        g_free (priv->sink_name);
        priv->sink_name = g_value_dup_string (value);

        if (priv->stream != NULL)
          {
            FsuSink *sink;

            g_object_get (priv->stream, "sink", &sink, NULL);

            if (sink == NULL)
              {
                g_warning ("Error fetching sink");
                break;
              }

            g_object_set (sink, "sink-name", priv->sink_name, NULL);
            g_object_unref (sink);
          }
        break;
      case PROP_SINK_DEVICE:
        g_free (priv->sink_device);
        priv->sink_device = g_value_dup_string (value);

        if (priv->stream != NULL)
          {
            FsuSink *sink;

            g_object_get (priv->stream, "sink", &sink, NULL);

            if (sink == NULL)
              {
                g_warning ("Error fetching sink");
                break;
              }

            g_object_set (sink, "sink-device", priv->sink_device, NULL);
            g_object_unref (sink);
          }
        break;
      case PROP_SINK_PIPELINE:
        g_free (priv->sink_pipeline);
        priv->sink_pipeline = g_value_dup_string (value);

        if (priv->stream != NULL)
          {
            FsuSink *sink;

            g_object_get (priv->stream, "sink", &sink, NULL);

            if (sink == NULL)
              {
                g_warning ("Error fetching sink");
                break;
              }

            g_object_set (sink, "sink-pipeline", priv->sink_pipeline, NULL);
            g_object_unref (sink);
          }
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
skype_base_stream_dispose (GObject *object)
{
  SkypeBaseStreamPrivate *priv = SKYPE_BASE_STREAM (object)->priv;

  skype_base_stream_deinit_stream (SKYPE_BASE_STREAM (object));

  if (priv->context != NULL)
    {
      g_main_context_unref (priv->context);
      priv->context = NULL;
    }

  if (priv->shm_writer != NULL)
    {
      g_object_unref (priv->shm_writer);
      priv->shm_writer = NULL;
    }

  if (priv->shm_reader != NULL)
    {
      g_object_unref (priv->shm_reader);
      priv->shm_reader = NULL;
    }

  if (priv->pipeline != NULL)
    {
      gst_object_unref (priv->pipeline);
      priv->pipeline = NULL;
    }

  G_OBJECT_CLASS (skype_base_stream_parent_class)->dispose (object);
}

static void
skype_base_stream_finalize (GObject *object)
{
  SkypeBaseStreamPrivate *priv = SKYPE_BASE_STREAM (object)->priv;

  fs_codec_destroy (priv->codec);

  g_free (priv->source_name);
  g_free (priv->source_device);
  g_free (priv->source_pipeline);
  g_free (priv->sink_name);
  g_free (priv->sink_device);
  g_free (priv->sink_pipeline);

  G_OBJECT_CLASS (skype_base_stream_parent_class)->finalize (object);
}

static void
skype_base_stream_class_init (SkypeBaseStreamClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (SkypeBaseStreamPrivate));

  object_class->constructed = skype_base_stream_constructed;
  object_class->get_property = skype_base_stream_get_property;
  object_class->set_property = skype_base_stream_set_property;
  object_class->dispose = skype_base_stream_dispose;
  object_class->finalize = skype_base_stream_finalize;

  /**
   * SkypeBaseStream:codec:
   *
   * The codec that should be used for sending content.
   *
   * Unless a child class overrides this, the stream will not set data until
   * this property is set.
   */
  pspec = g_param_spec_boxed ("codec", "Codec",
      "The sending codec to be used for this stream",
      FS_TYPE_CODEC,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_CODEC, pspec);

  /**
   * SkypeBaseStream:context:
   *
   * The context within which operations of this stream should run.
   *
   * Pass NULL to indicate the default context.
   */
  pspec = g_param_spec_pointer ("context", "Context",
      "The context within which operation of this stream should run",
      G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_CONTEXT, pspec);

  /**
   * SkypeBaseStream:bitrate:
   *
   * The bitrate at which to encode outgoing data.
   *
   * Currently, this property uses the units of whichever element contains
   * the bitrate property. This is imperfect as the units can be different
   * per element.
   */
  pspec = g_param_spec_uint ("bitrate", "Bitrate",
      "The bitrate in which to have the stream send",
      0, G_MAXUINT, 0,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_BITRATE, pspec);

  /**
   * SkypeBaseStream:media-type:
   *
   * The media type of this stream.
   */
  pspec = g_param_spec_uint ("media-type", "Media type",
      "The media type of the stream",
      0, FS_MEDIA_TYPE_LAST, 0,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_MEDIA_TYPE, pspec);

  /**
   * SkypeBaseStream:pipeline:
   *
   * The GStreamer pipeline that contains this stream.
   *
   * This property is intended to be used for setting element and codec
   * preferences on the stream.
   */
  pspec = g_param_spec_object ("pipeline", "Pipeline",
      "The pipeline in which to have this stream take place",
      GST_TYPE_PIPELINE,
      G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_PIPELINE, pspec);

  /**
   * SkypeBaseStream:source:
   *
   * The source element to be used for this stream.
   */
  pspec = g_param_spec_object ("source", "Source",
      "The source element to internally be used by the stream",
      FSU_TYPE_SOURCE,
      G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_SOURCE, pspec);

  /**
   * SkypeBaseStream:source-name:
   *
   * The name of the element to be used for the source.
   */
  pspec = g_param_spec_string ("source-name", "Source Name",
      "The name of the element to internally be used by the stream's source",
      NULL,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_SOURCE_NAME, pspec);

  /**
   * SkypeBaseStream:source-device:
   *
   * The name of the source device to be used for the source.
   */
  pspec = g_param_spec_string ("source-device", "Source Device",
      "The name of the device to be used by the stream's source",
      NULL,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_SOURCE_DEVICE, pspec);

  /**
   * SkypeBaseStream:source-pipeline:
   *
   * The name of the source pipeline to be used for the source.
   */
  pspec = g_param_spec_string ("source-pipeline", "Source Pipeline",
      "The description of the pipeline to be used for the stream's source",
      NULL,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_SOURCE_PIPELINE, pspec);

  /**
   * SkypeBaseStream:sink:
   *
   * The sink element to be used for this stream.
   */
  pspec = g_param_spec_object ("sink", "Sink",
      "The sink element to internally be used by the stream",
      FSU_TYPE_SINK,
      G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_SINK, pspec);

  /**
   * SkypeBaseStream:sink-name:
   *
   * The name of the sink element to be used for the sink.
   */
  pspec = g_param_spec_string ("sink-name", "Sink Name",
      "The name of the element to internally be used by the stream's sink",
      NULL,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_SINK_NAME, pspec);

  /**
   * SkypeBaseStream:sink-device:
   *
   * The name of the sink device to be used for the sink.
   */
  pspec = g_param_spec_string ("sink-device", "Sink Device",
      "The name of the device to be used by the stream's sink",
      NULL,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_SINK_DEVICE, pspec);

  /**
   * SkypeBaseStream:sink-pipeline:
   *
   * The name of the sink pipeline to be used for the source.
   */
  pspec = g_param_spec_string ("sink-pipeline", "Sink Pipeline",
      "The description of the pipeline to be used for the stream's sink",
      NULL,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_SINK_PIPELINE, pspec);

  /**
   * SkypeBaseStream:incoming-filter:
   *
   * The filter manager used to modify the incoming stream.
   *
   * This property is intended to be used for getting a filter manager for
   * the purpose of adding effects and/or other filters to the incoming media.
   */
  pspec = g_param_spec_object ("incoming-filter", "Incoming filter",
      "The filter manager used to modifying the incoming stream",
      FSU_TYPE_FILTER_MANAGER,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_INCOMING_FILTER, pspec);

  /**
   * SkypeBaseStream:outgoing-filter:
   *
   * The filter manager used to modify the outgoing stream.
   *
   * This property is intended to be used for getting a filter manager for
   * the purpose of adding effects and/or other filters to the outgoing media.
   */
  pspec = g_param_spec_object ("outgoing-filter", "Outgoing filter",
      "The filter manager used to modify the outgoing stream",
      FSU_TYPE_FILTER_MANAGER,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_OUTGOING_FILTER, pspec);

  /**
   * SkypeBaseStream::buffer-prepared
   * @stream: the #SkypeBaseStream that prepared the data buffer
   * @len: the length of the data @buffer prepared.
   * @buffer: the data buffer prepared.
   *
   * Emitted when a data buffer has been prepared to send.
   *
   * The data buffer needs to be copied if it's to be used past
   * the return of this signal call.
   */
  signals[SIGNAL_BUFFER_PREPARED] = g_signal_new ("buffer-prepared",
      G_TYPE_FROM_CLASS (object_class), G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
      0, NULL, NULL, g_cclosure_marshal_VOID__UINT_POINTER, G_TYPE_NONE,
      2, G_TYPE_UINT, G_TYPE_POINTER, NULL);
}

static void
skype_base_stream_init (SkypeBaseStream *self)
{
  self->priv = SKYPE_BASE_STREAM_GET_PRIVATE (self);
}

gboolean
skype_base_stream_handle_message (SkypeBaseStream *self, GstMessage *message)
{
  SkypeBaseStreamClass *klass = SKYPE_BASE_STREAM_GET_CLASS (self);
  SkypeBaseStreamPrivate *priv = self->priv;

  /* Let subclasses handle any of their messages */
  if (klass->handle_message != NULL &&
      klass->handle_message (self, message))
    return TRUE;

  /* Let Fsu handle any of its messages */
  if (priv->conf != NULL && fsu_conference_handle_message (priv->conf, message))
    return TRUE;

  switch (GST_MESSAGE_TYPE (message))
    {
      case GST_MESSAGE_ELEMENT:
        {
          GstStructure *structure =
              (GstStructure *)gst_message_get_structure (message);

          if (gst_structure_has_name (structure,
              "farsight-new-local-candidate"))
            {
              FsStream *stream;
              FsStream *cmp_stream;
              FsCandidate *candidate;
              gchar *shm_path;

              if (priv->stream == NULL)
                break;

              g_object_get (priv->stream, "fs-stream", &cmp_stream, NULL);

              gst_structure_get (structure,
                  "stream", FS_TYPE_STREAM, &stream,
                  "candidate", FS_TYPE_CANDIDATE, &candidate,
                  NULL);

              /* Make sure this message is really for this stream */
              if (stream != cmp_stream)
                {
                  fs_candidate_destroy (candidate);
                  g_object_unref (stream);
                  g_object_unref (cmp_stream);
                  break;
                }

              g_object_unref (cmp_stream);

              g_object_get (priv->shm_reader, "socket-path", &shm_path, NULL);

              if (shm_path == NULL || g_strcmp0 (shm_path, candidate->ip) != 0)
                {
                  shm_reader_disconnect (priv->shm_reader);

                  if (!shm_reader_connect (priv->shm_reader, candidate->ip))
                    {
                      g_warning ("Error connecting to Fs2 shm socket");
                    }
                }

              fs_candidate_destroy (candidate);
              g_object_unref (stream);
              return TRUE;
            }

          break;
        }
      default:
        break;
    }

  return FALSE;
}

static FsuStream *
create_fsu_stream (SkypeBaseStream *self, GstElement *pipeline)
{
  SkypeBaseStreamClass *klass = SKYPE_BASE_STREAM_GET_CLASS (self);
  SkypeBaseStreamPrivate *priv = self->priv;
  GstElement *conf;
  FsuConference *conference;
  FsSession *sess;
  FsuSession *session;
  FsParticipant *participant;
  FsStream *fsstream;
  GError *e = NULL;
  FsuStream *stream;
  guint media_type;
  gboolean has_source = priv->source != NULL;
  gboolean has_sink = priv->sink != NULL;
  gchar *path;

  if (klass->pre_create_stream == NULL)
    {
      g_warning ("SkypeBaseStream derivative doesn't define pre_create_stream");
      goto error;
    }

  if (!klass->pre_create_stream (self,
      (gpointer *)&priv->source, (gpointer *)&priv->sink))
    {
      g_warning ("Error in pre_create_stream");
      goto error;
    }

  if (priv->source == NULL || priv->sink == NULL)
    {
      g_warning ("Source or sink is NULL");
      goto error;
    }

  if (!FSU_IS_SOURCE (priv->source) || !FSU_IS_SINK (priv->sink))
    {
      g_warning ("Source is not an FsuSource or sink is not an FsuSink");
      goto error;
    }

  if (!has_source)
    g_object_set (G_OBJECT (priv->source),
        "source-name", priv->source_name,
        "source-device", priv->source_device,
        "source-pipeline", priv->source_pipeline,
        NULL);

  if (!has_sink)
    g_object_set (G_OBJECT (priv->sink),
        "sink-name", priv->sink_name,
        "sink-device", priv->sink_device,
        "sink-pipeline", priv->sink_pipeline,
        NULL);

  conf = gst_element_factory_make ("fsrtpconference", NULL);

  if (conf == NULL)
    {
      g_warning ("Error making FsRtpConference");
      goto error;
    }

  conference = fsu_conference_new (FS_CONFERENCE (conf), pipeline, &e);

  if (conference == NULL)
    {
      g_warning ("Error making FsuConference: %s", e->message);
      g_error_free (e);
      goto error;
    }

  g_object_get (self, "media-type", &media_type, NULL);
  sess = fs_conference_new_session (FS_CONFERENCE (conf),
      media_type, NULL);

  if (sess == NULL)
    {
      g_warning ("Error making FsSession");
      goto error;
    }

  path = skype_get_codec_preferences_path ();

  if (path != NULL)
    {
      GList *codecs = fs_codec_list_from_keyfile (path, &e);

      if (codecs == NULL)
        {
          g_warning ("Couldn't find file %s: %s", path, e->message);
          g_error_free (e);
        }
      else
        {
          if (!fs_session_set_codec_preferences (sess, codecs, &e))
            {
              g_warning ("Error setting codec preferences: %s", e->message);
              g_error_free (e);
            }

          fs_codec_list_destroy (codecs);
          codecs = NULL;
        }
    }

  g_free (path);

  participant = fs_conference_new_participant (FS_CONFERENCE (conf),
      "skype", NULL);

  gst_object_unref (conf);

  if (participant == NULL)
    {
      g_warning ("Error making FsParticipant");
      goto error;
    }

  session = fsu_conference_handle_session (conference,
      sess, gst_object_ref (priv->source));

  priv->conf = conference;

  if (session == NULL)
    {
      g_warning ("Error making FsuSession");
      gst_object_unref (priv->source);
      goto error;
    }

  fsstream = fs_session_new_stream (sess, participant, FS_DIRECTION_NONE,
      "shm", 0, NULL, &e);

  g_object_unref (participant);
  g_object_unref (sess);

  if (fsstream == NULL)
    {
      g_warning ("error creating fsstream %p: %s", fsstream, e->message);
      g_error_free (e);
      goto error;
    }

  stream = fsu_session_handle_stream (session,
      fsstream, gst_object_ref (priv->sink));

  g_object_unref (session);
  g_object_unref (fsstream);

  if (stream == NULL)
    {
      g_warning ("Error creating FsuStream");
      gst_object_unref (priv->sink);
    }

  return stream;

error:
  g_error ("Error initializing SkypeBaseStream");

  if (priv->source != NULL)
    {
      gst_object_unref (priv->source);
      priv->source = NULL;
    }

  if (priv->sink != NULL)
    {
      gst_object_unref (priv->sink);
      priv->sink = NULL;
    }

  return NULL;
}

static gboolean
init_candidates (SkypeBaseStream *self, FsuStream *stream)
{
  SkypeBaseStreamPrivate *priv = self->priv;
  FsStream *fsstream;
  FsCandidate *cand;
  GList *list = NULL;
  GError *e;
  gchar *path = NULL;

  g_object_get (priv->shm_writer, "socket-path", &path, NULL);

  if (path == NULL)
    path = g_strdup (shm_writer_open (priv->shm_writer,
        SKYPE_BASE_STREAM_DEFAULT_SHM_SOCKET_PATH));

  if (path == NULL)
    {
      g_warning ("Error opening shm writer");
      return FALSE;
    }

  g_object_get (stream, "fs-stream", &fsstream, NULL);

  if (fsstream == NULL)
    {
      g_warning ("Error fetching FsStream");
      g_free (path);
      return FALSE;
    }

  cand = fs_candidate_new (NULL, FS_COMPONENT_RTP,
      FS_CANDIDATE_TYPE_HOST, FS_NETWORK_PROTOCOL_UDP,
      SKYPE_BASE_STREAM_DEFAULT_FS_SOCKET_PATH, 0);
  cand->username = path;
  list = g_list_append (list, cand);

  if (!fs_stream_set_remote_candidates (fsstream, list, &e))
    {
      g_warning ("Error setting remote candidates: %s", e->message);
      return FALSE;
    }

  fs_candidate_list_destroy (list);
  g_object_unref (fsstream);
  return TRUE;
}

static void
gst_message_handler_cb (GstBus *bus, GstMessage *message, gpointer user_data)
{
  SkypeBaseStream *stream = SKYPE_BASE_STREAM (user_data);

  skype_base_stream_handle_message (stream, message);
}

static gboolean
skype_base_stream_add_default_message_handler (SkypeBaseStream *self)
{
  GstElement *pipeline;
  GstBus *bus;
  GSource *source;
  GMainContext *context;

  g_object_get (self,
      "pipeline", &pipeline,
      "context", &context,
      NULL);

  if (pipeline == NULL)
    {
      g_warning ("Error fetching pipeline");
      goto error;
    }

  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  gst_object_unref (pipeline);

  if (bus == NULL)
    {
      g_warning ("Error fetching bus");
      goto error;
    }

  source = gst_bus_create_watch (bus);

  if (source == NULL)
    {
      g_warning ("Error creating gst watch source");
      goto error;
    }

  g_source_set_callback (source,
      (GSourceFunc)gst_bus_async_signal_func, NULL, NULL);
  if (g_source_attach (source, context) == 0)
    {
      g_warning ("Error attaching gst watch source to context");
      goto error;
    }

  self->priv->watch_source = source;

  if (context != NULL)
    g_main_context_unref (context);

  g_signal_connect (bus, "message",
      G_CALLBACK (gst_message_handler_cb), self);
  gst_object_unref (bus);

  return TRUE;

error:
  if (context != NULL)
    g_main_context_unref (context);

  if (bus != NULL)
    gst_object_unref (bus);

  if (source != NULL)
    g_source_unref (source);

  return FALSE;
}

/**
 * skype_base_stream_init_stream
 * @self: the #SkypeBaseStream instance of which to initialize the stream.
 *
 * Initializes the stream of the passed SkypeBaseStream.
 *
 * Returns: %TRUE if successful; %FALSE otherwise.
 */
gboolean
skype_base_stream_init_stream (SkypeBaseStream *self)
{
  SkypeBaseStreamClass *klass = SKYPE_BASE_STREAM_GET_CLASS (self);
  SkypeBaseStreamPrivate *priv = self->priv;

  priv->stream = create_fsu_stream (self, priv->pipeline);

  if (priv->stream == NULL)
    {
      g_warning ("Error creating stream");
      return FALSE;
    }

  if (priv->pipeline == NULL &&
      !skype_base_stream_add_default_message_handler (self))
    {
      g_warning ("Error adding default message handler");
      skype_base_stream_deinit_stream (self);
      return FALSE;
    }

  if (!skype_base_stream_update_codecs (self))
    {
      g_warning ("Error updating codecs");
      skype_base_stream_deinit_stream (self);
      return FALSE;
    }

  if (klass->post_create_stream != NULL &&
      !klass->post_create_stream (self, priv->stream))
    {
      g_warning ("Error in post_create_stream");
      skype_base_stream_deinit_stream (self);
      return FALSE;
    }

  if (!init_candidates (self, priv->stream))
    {
      g_warning ("Error in init_candidates");
      skype_base_stream_deinit_stream (self);
      return FALSE;
    }

  return TRUE;
}

/**
 * skype_base_stream_play
 * @self: the #SkypeBaseStream instance of which to change direction.
 * @play: %TRUE if the stream should be set to receiving; %FALSE otherwise.
 *
 * Sets the stream to start or stop receiving media.
 *
 * Returns: %TRUE if successful; %FALSE otherwise.
 */
gboolean
skype_base_stream_play (SkypeBaseStream *self, gboolean play)
{
  SkypeBaseStreamPrivate *priv = self->priv;
  FsStream *fsstream;
  FsStreamDirection direction;
  gboolean ret = TRUE;

  g_object_get (priv->stream, "fs-stream", &fsstream, NULL);

  if (fsstream == NULL)
    {
      g_warning ("Error fetching FsStream");
      return FALSE;
    }

  g_object_get (fsstream, "direction", &direction, NULL);

  if (play)
    {
      g_object_set (fsstream, "direction",
          direction | FS_DIRECTION_RECV, NULL);
      ret = fsu_stream_start_receiving (priv->stream);
    }
  else
    {
      g_object_set (fsstream, "direction",
          direction & ~FS_DIRECTION_RECV, NULL);
      fsu_stream_stop_receiving (priv->stream);
    }

  return ret;
}

/**
 * skype_base_stream_record
 * @self: the #SkypeBaseStream instance of which to change direction.
 * @play: %TRUE if the stream should be set to playing; %FALSE otherwise.
 *
 * Sets the stream to start or stop sending media.
 *
 * Returns: %TRUE if successful; %FALSE otherwise.
 */
gboolean
skype_base_stream_record (SkypeBaseStream *self, gboolean record)
{
  SkypeBaseStreamPrivate *priv = self->priv;
  FsStream *fsstream;
  FsStreamDirection direction;
  gboolean ret = TRUE;

  g_object_get (priv->stream, "fs-stream", &fsstream, NULL);

  if (fsstream == NULL)
    {
      g_warning ("Error fetching FsStream");
      return FALSE;
    }

  g_object_get (fsstream, "direction", &direction, NULL);

  if (record)
    {
      g_object_set (fsstream, "direction",
          direction | FS_DIRECTION_SEND, NULL);
      ret = fsu_stream_start_sending (priv->stream);
    }
  else
    {
      g_object_set (fsstream, "direction",
          direction & ~FS_DIRECTION_SEND, NULL);
      fsu_stream_stop_sending (priv->stream);
    }

  return ret;
}

/**
 * skype_base_stream_deinit_stream
 * @self: the #SkypeBaseStream instance of which to deinitialize the stream.
 *
 * Deinitializes the stream of the passed SkypeBaseStream. After this function
 * call the stream will be non-functional until skype_base_stream_init_stream
 * is called again.
 */
void
skype_base_stream_deinit_stream (SkypeBaseStream *self)
{
  SkypeBaseStreamClass *klass = SKYPE_BASE_STREAM_GET_CLASS (self);
  SkypeBaseStreamPrivate *priv = self->priv;

  if (priv->watch_source != NULL)
    {
      /* Cleanup the GstBus watch */
      g_source_destroy (priv->watch_source);
      g_source_unref (priv->watch_source);
      priv->watch_source = NULL;
    }

  if (priv->source != NULL)
    {
      gst_object_unref (priv->source);
      priv->source = NULL;
    }

  if (priv->sink != NULL)
    {
      gst_object_unref (priv->sink);
      priv->sink = NULL;
    }

  if (priv->conf != NULL)
    {
      g_object_unref (priv->conf);
      priv->conf = NULL;
    }

  if (priv->stream != NULL)
    {
      if (klass->deinit_stream != NULL &&
          !klass->deinit_stream (self, priv->stream))
        {
          g_warning ("Error in child's deinit_stream");
        }

      g_object_unref (priv->stream);
      priv->stream = NULL;
    }

  shm_reader_disconnect (priv->shm_reader);
  shm_writer_close (priv->shm_writer);
}

/**
 * skype_base_stream_push_buffer
 * @self: the #SkypeBaseStream instance of which to push a @buffer into.
 * @buffer: the data buffer to push.
 * @len: the length of the data @buffer to push
 *
 * Pushes a @buffer into the stream to be locally decoded and displayed
 * provided the stream is set to receiving.
 *
 * Returns: %TRUE if successful; %FALSE otherwise.
 */
gboolean
skype_base_stream_push_buffer (SkypeBaseStream *self,
    const gchar *buffer,
    gsize len)
{
  return (shm_writer_send_buffer (self->priv->shm_writer, buffer, len) >= 0);
}
