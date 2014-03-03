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

#include "skype-video-stream.h"

#include <gst/farsight/fs-element-added-notifier.h>
#include <gst/farsight/fsu-conference.h>
#include <gst/farsight/fsu-stream.h>
#include <gst/filters/fsu-filter-manager.h>
#include <gst/filters/fsu-maxframerate-filter.h>
#include <gst/filters/fsu-resolution-filter.h>

#include "skype-utils.h"

/**
 * SECTION:skype-video-stream
 * @title: SkypeVideoStream
 * @short_description: object for skype video streams
 * @see_also: #SkypeBaseStream #SkypeAudioStream
 *
 * The #SkypeVideoStream object used to create video streams for Skype video
 * calls.
 */

/**
 * SkypeVideoStream:
 *
 * The SkypeVideoStream is a class that takes care of most of the
 * required Farsight 2 setup and operation for video calls.
 */

/**
 * SkypeBaseStream:
 *
 * The class of a #SkypeBaseStream.
 */

G_DEFINE_TYPE (SkypeVideoStream, skype_video_stream, SKYPE_TYPE_BASE_STREAM)

#define SKYPE_VIDEO_STREAM_GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
  SKYPE_TYPE_VIDEO_STREAM, SkypeVideoStreamPrivate))

#define DEFAULT_FPS 30
#define DEFAULT_WIDTH 320
#define DEFAULT_HEIGHT 240

struct _SkypeVideoStreamPrivate
{
  guint fps;
  guint width;
  guint height;

  FsuStream *stream;

  FsElementAddedNotifier *notifier;
  SkypeDynamicProperties *properties;

  FsuFilterId *framerate_id;
  FsuFilterId *resolution_id;
};

enum
{
  PROP_0,
  PROP_BITRATE,
  PROP_FPS,
  PROP_MEDIA_TYPE,
  PROP_MTU,
  PROP_WIDTH,
  PROP_HEIGHT,
};

static void
skype_video_stream_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  SkypeVideoStreamPrivate *priv = SKYPE_VIDEO_STREAM (object)->priv;

  switch (property_id)
    {
      case PROP_BITRATE:
        {
          guint bitrate = 0;

          if (!skype_dynamic_properties_get (priv->properties,
              "bitrate", &bitrate))
            g_debug ("SkypeVideoStream: Bitrate property not yet available");

          g_value_set_uint (value, bitrate);
        }
        break;
      case PROP_FPS:
        g_value_set_uint (value, priv->fps);
        break;
      case PROP_MEDIA_TYPE:
        g_value_set_uint (value, FS_MEDIA_TYPE_VIDEO);
        break;
      case PROP_MTU:
        {
          guint mtu = 0;

          if (!skype_dynamic_properties_get (priv->properties, "mtu", &mtu))
            g_debug ("SkypeVideoStream: MTU property not yet available");

          g_value_set_uint (value, mtu);
        }
        break;
      case PROP_WIDTH:
        g_value_set_uint (value, priv->width);
        break;
      case PROP_HEIGHT:
        g_value_set_uint (value, priv->height);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
skype_video_stream_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  SkypeVideoStreamPrivate *priv = SKYPE_VIDEO_STREAM (object)->priv;

  switch (property_id)
    {
      case PROP_BITRATE:
        skype_dynamic_properties_set (priv->properties, "bitrate",
            g_value_get_uint (value));
        break;
      case PROP_FPS:
        {
          guint fps = g_value_get_uint (value);

          /* No change. Nothing to do */
          if (fps == priv->fps)
            break;

          if (priv->framerate_id != NULL)
            {
              FsuSession *fsusession;
              FsuFilterManager *manager;
              FsuFilter *filter;

              g_object_get (G_OBJECT (priv->stream),
                  "fsu-session", &fsusession, NULL);

              if (fsusession == NULL)
                {
                  g_warning ("Error fetching FsuSession");
                  break;
                }

              g_object_get (G_OBJECT (fsusession),
                  "filter-manager", &manager, NULL);

              g_object_unref (fsusession);

              if (manager == NULL)
                {
                  g_warning ("Error fetching FsuFilterManager");
                  break;
                }

              filter = fsu_filter_manager_get_filter_by_id (manager,
                  priv->framerate_id);

              g_object_set (filter, "fps", fps, NULL);

              g_object_unref (manager);
              g_object_unref (filter);
            }

          priv->fps = fps;
        }
        break;
      case PROP_MTU:
        skype_dynamic_properties_set (priv->properties, "mtu",
            g_value_get_uint (value));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
skype_video_stream_dispose (GObject *object)
{
  G_OBJECT_CLASS (skype_video_stream_parent_class)->dispose (object);
}

static void
skype_video_stream_finalize (GObject *object)
{
  SkypeVideoStreamPrivate *priv = SKYPE_VIDEO_STREAM (object)->priv;

  g_object_unref (priv->notifier);
  skype_dynamic_properties_free (priv->properties);

  G_OBJECT_CLASS (skype_video_stream_parent_class)->finalize (object);
}

static gboolean
skype_video_stream_pre_create_stream (SkypeBaseStream *self,
    gpointer *source,
    gpointer *sink)
{
  if (source != NULL && *source == NULL)
    {
      *source = gst_element_factory_make ("fsuvideosrc", NULL);

      if (*source == NULL)
        {
          g_warning ("Unable to create fsuvideosrc. Does the element exist?");
          return FALSE;
        }
    }

  if (sink != NULL && *sink == NULL)
    {
      *sink = gst_element_factory_make ("fsuvideosink", NULL);

      if (*sink == NULL)
        {
          g_warning ("Unable to create fsuvideosink. Does the element exist?");
          return FALSE;
        }
    }

  return TRUE;
}

static gboolean
skype_video_stream_post_create_stream (SkypeBaseStream *self, gpointer stream)
{
  SkypeVideoStreamPrivate *priv = SKYPE_VIDEO_STREAM (self)->priv;
  FsuConference *fsuconf;
  FsConference *fsconf;
  FsuSession *fsusession;
  FsuFilterManager *manager;
  FsuFilter *filter;

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

  /* Setup filters */
  g_object_get (G_OBJECT (stream), "fsu-session", &fsusession, NULL);

  if (fsusession == NULL)
    {
      g_warning ("Error fetching FsuSession");
      return FALSE;
    }

  g_object_get (G_OBJECT (fsusession), "filter-manager", &manager, NULL);

  g_object_unref (fsusession);

  if (manager == NULL)
    {
      g_warning ("Error fetching FsuFilterManager");
      return FALSE;
    }

  /* Setup framerate filter */
  filter = FSU_FILTER (fsu_maxframerate_filter_new (priv->fps));

  priv->framerate_id = fsu_filter_manager_prepend_filter (manager, filter);

  g_object_unref (filter);

  if (priv->framerate_id == NULL)
    {
      g_warning ("Error adding framerate filter to filter manager.");
      return FALSE;
    }

  /* Setup resolution filter */
  filter = FSU_FILTER (fsu_resolution_filter_new (priv->width, priv->height));

  priv->resolution_id = fsu_filter_manager_prepend_filter (manager, filter);

  g_object_unref (manager);
  g_object_unref (filter);

  if (priv->resolution_id == NULL)
    {
      g_warning ("Error adding resolution filter to filter manager.");
      return FALSE;
    }

  priv->stream = g_object_ref (stream);
  return TRUE;
}

static gboolean
skype_video_stream_deinit_stream (SkypeBaseStream *self, gpointer stream)
{
  SkypeVideoStreamPrivate *priv = SKYPE_VIDEO_STREAM (self)->priv;
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

  if (priv->resolution_id != NULL || priv->framerate_id != NULL)
    {
      FsuSession *fsusession;
      FsuFilterManager *manager;

      g_object_get (stream, "fsu-session", &fsusession, NULL);

      if (fsusession == NULL)
        {
          g_warning ("Error fetching FsuSession");
          ret = FALSE;
        }
      else
        {
          g_object_get (G_OBJECT (fsusession),
              "filter-manager", &manager, NULL);

          g_object_unref (fsusession);

          if (manager == NULL)
            {
              g_warning ("Error fetching FsuFilterManager");
              ret = FALSE;
            }
          else
            {
              if (priv->resolution_id != NULL)
                {
                  if (!fsu_filter_manager_remove_filter (manager,
                      priv->resolution_id))
                    {
                      g_warning ("Error removing resolution filter");
                      ret = FALSE;
                    }

                  priv->resolution_id = NULL;
               }

              if (priv->framerate_id != NULL)
                {
                  if (!fsu_filter_manager_remove_filter (manager,
                      priv->framerate_id))
                    {
                      g_warning ("Error removing framerate filter");
                      ret = FALSE;
                    }

                  priv->framerate_id = NULL;
               }

              g_object_unref (manager);
            }
        }
    }

  g_object_unref (priv->stream);
  priv->stream = NULL;
  return ret;
}

static void
skype_video_stream_class_init (SkypeVideoStreamClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  SkypeBaseStreamClass *base_class = SKYPE_BASE_STREAM_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (SkypeVideoStreamPrivate));

  object_class->get_property = skype_video_stream_get_property;
  object_class->set_property = skype_video_stream_set_property;
  object_class->dispose = skype_video_stream_dispose;
  object_class->finalize = skype_video_stream_finalize;

  base_class->pre_create_stream = skype_video_stream_pre_create_stream;
  base_class->post_create_stream = skype_video_stream_post_create_stream;
  base_class->deinit_stream = skype_video_stream_deinit_stream;

  g_object_class_override_property (object_class,
      PROP_MEDIA_TYPE, "media-type");
  g_object_class_override_property (object_class,
      PROP_BITRATE, "bitrate");

  /**
   * SkypeVideoStream:fps:
   *
   * The framerate at which to encode outgoing data.
   */
  pspec = g_param_spec_uint ("fps", "Frames Per Second",
      "The fps for the stream to use",
      0, G_MAXUINT, DEFAULT_FPS,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_FPS, pspec);

  /**
   * SkypeVideoStream:mtu:
   *
   * The max transmission unit aka max packet size at which to
   * packetize outgoing data.
   */
  pspec = g_param_spec_uint ("mtu", "Maximum Transmission Unit",
      "The MTU for the stream to use",
      0, G_MAXUINT, 0,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_MTU, pspec);

  /**
   * SkypeVideoStream:width:
   *
   * The width of the resolution at which to encode outgoing data.
   */
  pspec = g_param_spec_uint ("width", "Width",
      "The width of the video to use on this stream",
      0, G_MAXUINT, DEFAULT_WIDTH,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_WIDTH, pspec);

  /**
   * SkypeVideoStream:height:
   *
   * The height of the resolution at which to encode outgoing data.
   */
  pspec = g_param_spec_uint ("height", "Height",
      "The height of the video to use on this stream",
      0, G_MAXUINT, DEFAULT_HEIGHT,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_HEIGHT, pspec);
}

static void
skype_video_stream_init (SkypeVideoStream *self)
{
  self->priv = SKYPE_VIDEO_STREAM_GET_PRIVATE (self);

  self->priv->fps = DEFAULT_FPS;
  self->priv->width = DEFAULT_WIDTH;
  self->priv->height = DEFAULT_HEIGHT;

  self->priv->notifier = fs_element_added_notifier_new ();
  self->priv->properties = skype_dynamic_properties_new (self->priv->notifier,
      "bitrate", G_TYPE_UINT,
      "mtu", G_TYPE_UINT,
      NULL);
}

/**
 * skype_video_stream_new
 *
 * Creates a new #SkypeVideoStream.
 *
 * Returns: a newly created #SkypeVideoStream
 */
SkypeVideoStream *
skype_video_stream_new (void)
{
  return g_object_new (SKYPE_TYPE_VIDEO_STREAM, NULL);
}

/**
 * skype_video_stream_new_with_context
 *
 * Creates a new #SkypeVideoStream with a specific #GMainContext.
 *
 * Returns: a newly created #SkypeVideoStream
 */
SkypeVideoStream *
skype_video_stream_new_with_context (GMainContext *context)
{
  return g_object_new (SKYPE_TYPE_VIDEO_STREAM,
      "context", context,
      NULL);
}

/**
 * skype_video_stream_request_key_frame
 * @self: the #SkypeBaseStream instance on which to request a key frame.
 *
 * Requests a key frame to be generated to be sent to the remote party.
 */
void
skype_video_stream_request_key_frame (SkypeVideoStream *self)
{
  SkypeVideoStreamPrivate *priv = self->priv;
  GstElement *sink;

  g_object_get (priv->stream, "sink", &sink, NULL);

  if (sink == NULL)
    {
      g_warning ("Error fetching sink");
      return;
    }

  gst_element_send_event (sink,
      gst_event_new_custom (GST_EVENT_CUSTOM_UPSTREAM,
          gst_structure_new ("GstForceKeyUnit", NULL)));

  gst_object_unref (sink);
}

/**
 * skype_base_stream_push_buffer
 * @self: the #SkypeBaseStream instance on which to operate.
 *
 * This function is reserved for potential use later.
 */
void
skype_video_stream_request_recovery_frame (SkypeVideoStream *self)
{
  /* Unused for now */
}

/**
 * skype_base_stream_set_resolution
 * @self: the #SkypeBaseStream instance on which to set the resolution.
 * @width: the width of the resolution to set.
 * @height: the height of the resolution to set.
 *
 * Sets the passed resolution on @self.
 *
 * Returns: %TRUE if successful; %FALSE otherwise.
 */
gboolean
skype_video_stream_set_resolution (SkypeVideoStream *self,
    guint width,
    guint height)
{
  SkypeVideoStreamPrivate *priv = self->priv;

  /* No change. Nothing to do */
  if (width == priv->width && height == priv->height)
    return TRUE;

  if (priv->resolution_id != NULL)
    {
      FsuSession *fsusession;
      FsuFilterManager *manager;
      FsuFilter *filter;

      g_object_get (G_OBJECT (priv->stream), "fsu-session", &fsusession, NULL);

      if (fsusession == NULL)
        {
          g_warning ("Error fetching FsuSession");
          return FALSE;
        }

      g_object_get (G_OBJECT (fsusession), "filter-manager", &manager, NULL);

      g_object_unref (fsusession);

      if (manager == NULL)
        {
          g_warning ("Error fetching FsuFilterManager");
          return FALSE;
        }

      filter = FSU_FILTER (fsu_resolution_filter_new (width, height));

      priv->resolution_id = fsu_filter_manager_replace_filter (manager,
          filter, priv->resolution_id);

      g_object_unref (manager);
      g_object_unref (filter);
    }

  priv->width = width;
  priv->height = height;
  return TRUE;
}
