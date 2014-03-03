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

#include "skype-audio-stream.h"

#include <gst/farsight/fs-codec.h>
#include <gst/farsight/fs-element-added-notifier.h>
#include <gst/farsight/fsu-conference.h>
#include <gst/farsight/fsu-stream.h>
#include <gst/filters/fsu-filter-manager.h>
#include <gst/filters/fsu-volume-filter.h>

#include "skype-utils.h"

/**
 * SECTION:skype-audio-stream
 * @title: SkypeAudioStream
 * @short_description: object for skype audio streams
 * @see_also: #SkypeBaseStream #SkypeVideoStream
 *
 * The #SkypeAudioStream object used to create audio streams for Skype audio
 * calls.
 */

/**
 * SkypeAudioStream:
 *
 * The SkypeAudioStream is a class that takes care of most of the
 * required Farsight 2 setup and operation for audio calls.
 */

/**
 * SkypeBaseStream:
 *
 * The class of a #SkypeBaseStream.
 */

G_DEFINE_TYPE (SkypeAudioStream, skype_audio_stream, SKYPE_TYPE_BASE_STREAM)

#define SKYPE_AUDIO_STREAM_GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
  SKYPE_TYPE_AUDIO_STREAM, SkypeAudioStreamPrivate))

#define DEFAULT_VOLUME 1.0
#define DEFAULT_PTIME 20
#define SKYPE_DTMF_EVENT_TYPE_IN_BAND 0

typedef struct
{
  int event_nr;
  int rtp_type;
} DTMFQueueEvent;

struct _SkypeAudioStreamPrivate
{
  guint channel_id;
  guint conversation_id;
  guint call_member_id;

  guint ptime;
  gdouble volume;

  FsuStream *stream;

  FsElementAddedNotifier *notifier;
  SkypeDynamicProperties *properties;

  FsuFilterId *volume_id;

  GQueue *event_queue;
  guint last_telephony_pt;
  guint pending_telephony_pt;

  GstElement *dtmfsrc;
};

enum
{
  PROP_0,
  PROP_BITRATE,
  PROP_CHANNEL_ID,
  PROP_CONVERSATION_ID,
  PROP_CALL_MEMBER_ID,
  PROP_MEDIA_TYPE,
  PROP_PTIME,
  PROP_VOLUME,
};

static void
skype_audio_stream_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  SkypeAudioStreamPrivate *priv = SKYPE_AUDIO_STREAM (object)->priv;

  switch (property_id)
    {
      case PROP_BITRATE:
        {
          guint bitrate = 0;

          if (!skype_dynamic_properties_get (priv->properties,
              "bitrate", &bitrate))
            g_debug ("SkypeAudioStream: Bitrate property not yet available");

          g_value_set_uint (value, bitrate);
        }
        break;
      case PROP_CHANNEL_ID:
        g_value_set_uint (value, priv->channel_id);
        break;
      case PROP_CONVERSATION_ID:
        g_value_set_uint (value, priv->conversation_id);
        break;
      case PROP_CALL_MEMBER_ID:
        g_value_set_uint (value, priv->call_member_id);
        break;
      case PROP_MEDIA_TYPE:
        g_value_set_uint (value, FS_MEDIA_TYPE_AUDIO);
        break;
      case PROP_PTIME:
        g_value_set_uint (value, priv->ptime);
        break;
      case PROP_VOLUME:
        g_value_set_double (value, priv->volume);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
skype_audio_stream_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  SkypeAudioStreamPrivate *priv = SKYPE_AUDIO_STREAM (object)->priv;

  switch (property_id)
    {
      case PROP_BITRATE:
        skype_dynamic_properties_set (priv->properties, "bitrate",
            g_value_get_uint (value));
        break;
      case PROP_CHANNEL_ID:
        priv->channel_id = g_value_get_uint (value);
        break;
      case PROP_CONVERSATION_ID:
        priv->conversation_id = g_value_get_uint (value);
        break;
      case PROP_CALL_MEMBER_ID:
        priv->call_member_id = g_value_get_uint (value);
        break;
      case PROP_PTIME:
        {
          FsCodec *codec = NULL;

          priv->ptime = g_value_get_uint (value);

          /* Trigger a codec update */
          g_object_get (object, "codec", &codec, NULL);
          g_object_set (object, "codec", codec, NULL);
          fs_codec_destroy (codec);
        }
        break;
      case PROP_VOLUME:
        {
          priv->volume = g_value_get_double (value);

          if (priv->volume_id != NULL)
            {
              FsuFilterManager *manager;

              g_object_get (G_OBJECT (priv->stream),
                  "filter-manager", &manager, NULL);

              if (manager == NULL)
                {
                  g_warning ("Error fetching FsuFilterManager");
                }
              else
                {
                  FsuFilter *filter;

                  filter = fsu_filter_manager_get_filter_by_id (manager,
                      priv->volume_id);

                  if (filter == NULL)
                    {
                      g_warning ("Error retrieving volume filter");
                    }
                  else
                    {
                      g_object_set (filter, "volume", priv->volume, NULL);
                      g_object_unref (filter);
                    }

                  g_object_unref (manager);
                }
            }
        }
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
skype_audio_stream_dispose (GObject *object)
{
  G_OBJECT_CLASS (skype_audio_stream_parent_class)->dispose (object);
}

static void
skype_audio_stream_finalize (GObject *object)
{
  SkypeAudioStreamPrivate *priv = SKYPE_AUDIO_STREAM (object)->priv;

  g_object_unref (priv->notifier);
  skype_dynamic_properties_free (priv->properties);

  G_OBJECT_CLASS (skype_audio_stream_parent_class)->finalize (object);
}

static gboolean
skype_audio_stream_pre_create_stream (SkypeBaseStream *self,
    gpointer *source,
    gpointer *sink)
{
  SkypeAudioStreamPrivate *priv = SKYPE_AUDIO_STREAM (self)->priv;

  /* Setup bus handler for DTMF */
  priv->event_queue = g_queue_new ();
  priv->last_telephony_pt = FS_CODEC_ID_DISABLE;
  priv->pending_telephony_pt = 127;

  /* Setup src and sink */
  if (source != NULL && *source == NULL)
    {
      *source = gst_element_factory_make ("fsuaudiosrc", NULL);

      if (*source == NULL)
        {
          g_warning ("Unable to create fsuaudiosrc. Does the element exist?");
          return FALSE;
        }
    }

  if (sink != NULL && *sink == NULL)
    {
      *sink = gst_element_factory_make ("fsuaudiosink", NULL);

      if (*sink == NULL)
        {
          g_warning ("Unable to create fsuaudiosink. Does the element exist?");
          return FALSE;
        }
    }

  if (sink != NULL && *sink != NULL)
    {
      /* Set sink to async so it doesn't hang due to having two sources. */
      g_object_set (*sink, "async", FALSE, NULL);
    }

  return TRUE;
}

gboolean
skype_audio_stream_handle_message (SkypeBaseStream *self, GstMessage *message)
{
  SkypeAudioStreamPrivate *priv = SKYPE_AUDIO_STREAM (self)->priv;
  GstStructure *structure;
  FsSession *session = NULL;
  GList *secondary_codecs = NULL;
  GList *iter;

  if (GST_MESSAGE_TYPE (message) != GST_MESSAGE_ELEMENT)
    return FALSE;

  structure = (GstStructure *)gst_message_get_structure (message);

  if (!gst_structure_has_name (structure, "farsight-send-codec-changed"))
    return FALSE;

  gst_structure_get (structure,
      "session", FS_TYPE_SESSION, &session,
      "secondary-codecs", FS_TYPE_CODEC_LIST, &secondary_codecs,
      NULL);

  /* Update the current telephony PT */
  for (iter = secondary_codecs; iter != NULL; iter = g_list_next (iter))
    {
      FsCodec *codec = (FsCodec *)iter->data;

      if (codec != NULL && !g_strcmp0 (codec->encoding_name, "telephone-event"))
        {
          priv->last_telephony_pt = codec->id;
          if (priv->pending_telephony_pt != FS_CODEC_ID_DISABLE &&
              priv->pending_telephony_pt == priv->last_telephony_pt)
            priv->pending_telephony_pt = FS_CODEC_ID_DISABLE;
          break;
        }
    }

  while (!g_queue_is_empty (priv->event_queue))
    {
      DTMFQueueEvent *event = (DTMFQueueEvent *)
          g_queue_pop_head (priv->event_queue);
      FsDTMFMethod method = FS_DTMF_METHOD_AUTO;

      if (event->rtp_type == SKYPE_DTMF_EVENT_TYPE_IN_BAND)
        {
          method = FS_DTMF_METHOD_IN_BAND;
        }
      else if (event->rtp_type == priv->last_telephony_pt)
        {
          method = FS_DTMF_METHOD_RTP_RFC4733;
        }
      else
        {
          /* New PT needed. Update codecs */
          FsCodec *codec;

          g_queue_push_head (priv->event_queue, event);

          priv->pending_telephony_pt = event->rtp_type;

          /* Trigger a codec update */
          g_object_get (self, "codec", &codec, NULL);
          g_object_set (self, "codec", codec, NULL);
          fs_codec_destroy (codec);

          break;
        }

      if (!fs_session_start_telephony_event (session,
          event->event_nr, 0, method))
        {
          g_warning ("Telephony event type not supported");
          g_queue_push_head (priv->event_queue, event);
        }
      else
        {
          fs_session_stop_telephony_event (session, method);
        }

      g_slice_free (DTMFQueueEvent, event);
    }

  g_object_unref (session);
  fs_codec_list_destroy (secondary_codecs);

  return TRUE;
}

static gboolean
skype_audio_stream_post_create_stream (SkypeBaseStream *self, gpointer stream)
{
  SkypeAudioStreamPrivate *priv = SKYPE_AUDIO_STREAM (self)->priv;
  FsuConference *fsuconf;
  FsConference *fsconf;
  FsuFilterManager *manager;
  FsuFilter *filter;
  FsuSink *sink;
  GstElement *parent;
  GstElement *src;

  /* Setup dynamic properties */
  g_object_get (G_OBJECT (stream), "fsu-conference", &fsuconf, NULL);

  if (fsuconf == NULL)
    {
      g_warning ("Error fetching FsuConference");
      return FALSE;
    }

  g_object_get (G_OBJECT (fsuconf), "fs-conference", &fsconf, NULL);
  g_object_unref (fsuconf);

  if (fsconf == NULL)
    {
      g_warning ("Error fetching FsConference");
      return FALSE;
    }

  fs_element_added_notifier_add (priv->notifier, GST_BIN (fsconf));

  g_object_unref (fsconf);

  /* Setup volume filter */
  g_object_get (G_OBJECT (stream), "filter-manager", &manager, NULL);

  if (manager == NULL)
    {
      g_warning ("Error fetching FsuFilterManager");
      return FALSE;
    }

  filter = FSU_FILTER (fsu_volume_filter_new ());

  g_object_set (filter, "volume", priv->volume, NULL);

  priv->volume_id = fsu_filter_manager_prepend_filter (manager, filter);

  g_object_unref (manager);
  g_object_unref (filter);

  if (priv->volume_id == NULL)
    {
      g_warning ("Error adding resolution filter to filter manager.");
      return FALSE;
    }

  /* Setup DTMF source */
  g_object_get (stream, "sink", &sink, NULL);

  if (sink == NULL)
    {
      g_warning ("Error fetching sink");
      return FALSE;
    }

  src = gst_element_factory_make ("dtmfsrc", NULL);

  if (src == NULL)
    {
      g_warning ("Error creating dtmfsrc");
      gst_object_unref (sink);
      return FALSE;
    }

  parent = GST_ELEMENT (gst_element_get_parent (sink));

  if (parent == NULL)
    {
      g_warning ("Error fetching FsuSink parent");
      gst_object_unref (src);
      gst_object_unref (sink);
      return FALSE;
    }

  if (!gst_bin_add (GST_BIN (parent), gst_object_ref (src)))
    {
      g_warning ("Error adding dtmfsrc to FsuSink parent");
      /* Unref src twice if it fails as it's reffed in the call */
      gst_object_unref (src);
      gst_object_unref (src);
      gst_object_unref (sink);
      gst_object_unref (parent);
      return FALSE;
    }

  gst_object_unref (parent);

  if (!gst_element_link (src, GST_ELEMENT (sink)))
    {
      g_warning ("Error adding dtmfsrc to FsuSink parent");
      gst_object_unref (src);
      gst_object_unref (sink);
      return FALSE;
    }

  gst_object_unref (sink);

  if (!gst_element_sync_state_with_parent (src))
    {
      g_warning ("Error syncing dtmfsrc state with parent");
      gst_object_unref (src);
      return FALSE;
    }

  priv->dtmfsrc = src;

  priv->stream = g_object_ref (stream);
  return TRUE;
}

static gboolean
skype_audio_stream_deinit_stream (SkypeBaseStream *self, gpointer stream)
{
  SkypeAudioStreamPrivate *priv = SKYPE_AUDIO_STREAM (self)->priv;
  FsuConference *fsuconf;
  gboolean ret = TRUE;

  g_object_get (G_OBJECT (stream), "fsu-conference", &fsuconf, NULL);

  if (fsuconf != NULL)
    {
      FsConference *fsconf;

      g_object_get (G_OBJECT (fsuconf), "fs-conference", &fsconf, NULL);

      g_object_unref (fsuconf);

      if (fsconf == NULL)
        {
          g_warning ("Error fetching FsConference");
          ret = FALSE;
        }
      else
        {
          fs_element_added_notifier_remove (priv->notifier, GST_BIN (fsconf));

          g_object_unref (fsconf);
        }
    }
  else
    {
      g_warning ("Error fetching FsuConference");
      ret = FALSE;
    }

  if (priv->volume_id != NULL)
    {
      FsuFilterManager *manager;

      g_object_get (G_OBJECT (stream),
          "filter-manager", &manager, NULL);

      if (manager == NULL)
        {
          g_warning ("Error fetching FsuFilterManager");
          ret = FALSE;
        }
      else
        {
          if (!fsu_filter_manager_remove_filter (manager,
              priv->volume_id))
            {
              g_warning ("Error removing volume filter");
              ret = FALSE;
            }

          g_object_unref (manager);
          priv->volume_id = NULL;
        }
    }

  if (priv->event_queue != NULL)
    {
      while (!g_queue_is_empty (priv->event_queue))
        {
          g_slice_free (DTMFQueueEvent, g_queue_pop_head (priv->event_queue));
        }

      g_queue_free (priv->event_queue);
      priv->event_queue = NULL;
    }

  if (priv->dtmfsrc != NULL)
    {
      gst_object_unref (priv->dtmfsrc);
      priv->dtmfsrc = NULL;
    }

  g_object_unref (priv->stream);
  priv->stream = NULL;
  return ret;
}

static void
skype_audio_stream_update_codecs (SkypeBaseStream *stream,
    GList **codecs)
{
  SkypeAudioStreamPrivate *priv = SKYPE_AUDIO_STREAM (stream)->priv;
  FsCodec *codec;
  gchar *value;
  guint telephony_pt = priv->last_telephony_pt;

  if (codecs == NULL || *codecs == NULL || (*codecs)->data == NULL)
    {
      g_warning ("update_codecs should not be passed an empty codec list");
      return;
    }

  codec = (FsCodec *)((*codecs)->data);

  /* Add ptime value */
  value = g_strdup_printf ("%d", priv->ptime);
  fs_codec_add_optional_parameter (codec, "ptime", value);
  g_free (value);

  /* Add telephony codec if needed */
  if (priv->pending_telephony_pt != FS_CODEC_ID_DISABLE)
    telephony_pt = priv->pending_telephony_pt;

  if (telephony_pt != FS_CODEC_ID_DISABLE)
    {
      codec = fs_codec_new (telephony_pt, "telephone-event",
          FS_MEDIA_TYPE_AUDIO, 8000);
      fs_codec_add_optional_parameter (codec, "events", "0-11");
      *codecs = g_list_append (*codecs, codec);
    }
}

static void
skype_audio_stream_class_init (SkypeAudioStreamClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  SkypeBaseStreamClass *base_class = SKYPE_BASE_STREAM_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (SkypeAudioStreamPrivate));

  object_class->get_property = skype_audio_stream_get_property;
  object_class->set_property = skype_audio_stream_set_property;
  object_class->dispose = skype_audio_stream_dispose;
  object_class->finalize = skype_audio_stream_finalize;

  base_class->pre_create_stream = skype_audio_stream_pre_create_stream;
  base_class->post_create_stream = skype_audio_stream_post_create_stream;
  base_class->deinit_stream = skype_audio_stream_deinit_stream;
  base_class->update_codecs = skype_audio_stream_update_codecs;
  base_class->handle_message = skype_audio_stream_handle_message;

  g_object_class_override_property (object_class,
      PROP_MEDIA_TYPE, "media-type");
  g_object_class_override_property (object_class,
      PROP_BITRATE, "bitrate");

  pspec = g_param_spec_uint ("channel-id", "Channel ID",
      "The channel id assigned to this stream",
      0, G_MAXUINT, 0,
      G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_CHANNEL_ID, pspec);

  pspec = g_param_spec_uint ("conversation-id", "Conversation ID",
      "The conversation id assigned to this stream",
      0, G_MAXUINT, 0,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_CONVERSATION_ID, pspec);

  pspec = g_param_spec_uint ("call-member-id", "Call Member ID",
      "The call member id assigned to this stream",
      0, G_MAXUINT, 0,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_CALL_MEMBER_ID, pspec);

  pspec = g_param_spec_uint ("ptime", "Packet Time",
      "The packet time this stream should use",
      0, G_MAXUINT, 0,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_PTIME, pspec);

  pspec = g_param_spec_double ("volume", "Volume",
      "The volume level this stream should use",
      0.0, 10.0, 1.0,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_VOLUME, pspec);
}

static void
skype_audio_stream_init (SkypeAudioStream *self)
{
  self->priv = SKYPE_AUDIO_STREAM_GET_PRIVATE (self);

  self->priv->ptime = DEFAULT_PTIME;
  self->priv->volume = DEFAULT_VOLUME;

  self->priv->notifier = fs_element_added_notifier_new ();
  self->priv->properties = skype_dynamic_properties_new (self->priv->notifier,
      "bitrate", G_TYPE_UINT,
      NULL);
}

/**
 * skype_audio_stream_new
 *
 * Creates a new #SkypeAudioStream.
 *
 * Returns: a newly created #SkypeAudioStream
 */
SkypeAudioStream *
skype_audio_stream_new (guint channel_id)
{
  return g_object_new (SKYPE_TYPE_AUDIO_STREAM,
      "channel-id", channel_id,
      NULL);
}

/**
 * skype_audio_stream_new_with_context
 *
 * Creates a new #SkypeAudioStream with a specific #GMainContext.
 *
 * Returns: a newly created #SkypeAudioStream
 */
SkypeAudioStream *
skype_audio_stream_new_with_context (guint channel_id, GMainContext *context)
{
  return g_object_new (SKYPE_TYPE_AUDIO_STREAM,
      "channel-id", channel_id,
      "context", context,
      NULL);
}

SkypeAudioStream *
skype_audio_stream_new_full (guint channel_id,
    GMainContext *context,
    GstElement *pipeline,
    GstElement *source,
    GstElement *sink)
{
  return g_object_new (SKYPE_TYPE_AUDIO_STREAM,
      "channel-id", channel_id,
      "context", context,
      "pipeline", pipeline,
      "source", source,
      "sink", sink,
      NULL);
}

gboolean
skype_audio_stream_send_dtmf (SkypeAudioStream *self,
    int event,
    int type)
{
  SkypeAudioStreamPrivate *priv = self->priv;
  FsuSession *session;
  FsSession *fssession;
  FsDTMFMethod method;
  gboolean ret = TRUE;
  GstStructure *structure;
  GstEvent *gstevent;

  g_debug ("Sending telephony event: %d, type: %d", event, type);

  structure = gst_structure_new ("dtmf-event",
      "type", G_TYPE_INT, 1,
      "number", G_TYPE_INT, event,
      "volume", G_TYPE_INT, 8,
      "start", G_TYPE_BOOLEAN, TRUE,
      NULL);
  gstevent = gst_event_new_custom (GST_EVENT_CUSTOM_UPSTREAM, structure);

  if (!gst_element_send_event (priv->dtmfsrc, gstevent))
    g_warning ("DTMF start event wasn't applied");

  structure = gst_structure_new ("dtmf-event",
      "type", G_TYPE_INT, 1,
      "number", G_TYPE_INT, event,
      "volume", G_TYPE_INT, 8,
      "start", G_TYPE_BOOLEAN, FALSE,
      NULL);
  gstevent = gst_event_new_custom (GST_EVENT_CUSTOM_UPSTREAM, structure);

  if (!gst_element_send_event (priv->dtmfsrc, gstevent))
    g_warning ("DTMF start event wasn't applied");

  if (g_queue_is_empty (priv->event_queue))
    {
      /* The type argument doubles as both the type of event
       * (RTP or in-band) as well as the PT to use if it's RTP */
      if (type == SKYPE_DTMF_EVENT_TYPE_IN_BAND)
        {
          method = FS_DTMF_METHOD_IN_BAND;
        }
      else if (type == priv->last_telephony_pt)
        {
          /* Play now only if the current PT matches the desired one */
          method = FS_DTMF_METHOD_RTP_RFC4733;
        }
      else
        {
          FsCodec *codec;

          /* Can't play the DTMF event now, queue the event and update codecs */
          DTMFQueueEvent *dtmf_event = g_slice_new0 (DTMFQueueEvent);
          dtmf_event->event_nr = event;
          dtmf_event->rtp_type = type;
          g_queue_push_tail (priv->event_queue, dtmf_event);

          priv->pending_telephony_pt = type;

          /* Trigger a codec update */
          g_object_get (self, "codec", &codec, NULL);
          g_object_set (self, "codec", codec, NULL);
          fs_codec_destroy (codec);
          return ret;
        }
    }
  else
    {
      /* Other events are pending, queue this one */
      DTMFQueueEvent *dtmf_event = g_slice_new0 (DTMFQueueEvent);
      dtmf_event->event_nr = event;
      dtmf_event->rtp_type = type;
      g_queue_push_tail (priv->event_queue, dtmf_event);
      return ret;
    }

  g_object_get (priv->stream, "fsu-session", &session, NULL);

  if (session == NULL)
    {
      g_warning ("SendDTMF: session == NULL");
      return FALSE;
    }

  g_object_get (session, "fs-session", &fssession, NULL);

  g_object_unref (session);

  if (fssession == NULL)
    {
      g_warning ("SendDTMF: fssession == NULL");
      return FALSE;
    }

  /* Play the event */
  if (!fs_session_start_telephony_event (fssession, event, 0, method))
    {
      g_warning ("Telephony event type not supported");
      ret = FALSE;
    }
  else
    {
      fs_session_stop_telephony_event (fssession, method);
    }

  g_object_unref (fssession);
  return ret;
}
