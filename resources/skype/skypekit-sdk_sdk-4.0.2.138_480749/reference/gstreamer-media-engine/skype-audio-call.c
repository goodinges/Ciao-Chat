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

#include "skype-audio-call.h"

#include <gst/filters/fsu-filter-manager.h>
#include <gst/filters/fsu-level-filter.h>
#include <gst/filters/fsu-volume-filter.h>

#include "skype-base-call-priv.h"

/**
 * SECTION:skype-audio-call
 * @title: SkypeAudioCall
 * @short_description: object for representing of audio calls
 * @see_also: #SkypeBaseCall, #SkypeVideoCall
 *
 * The #SkypeAudioCall object is used to represent audio calls.
 */

/**
 * SkypeAudioCall:
 *
 * The call object used to represent audio calls.
 */

/**
 * SkypeAudioCallClass:
 *
 * The class of a #SkypeAudioCall.
 */
G_DEFINE_TYPE (SkypeAudioCall, skype_audio_call, SKYPE_TYPE_BASE_CALL)

#define SKYPE_AUDIO_CALL_GET_PRIVATE(o) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
    SKYPE_TYPE_AUDIO_CALL, \
    SkypeAudioCallPrivate))

struct _SkypeAudioCallPrivate
{
  FsuFilter *level_in;
  FsuFilter *level_out;
  gulong level_in_id;
  gulong level_out_id;

  FsuFilter *volume_in;
  FsuFilter *volume_out;
};

enum
{
  PROP_0,
  PROP_VOLUME_IN,
  PROP_VOLUME_OUT,
};

enum
{
  SIGNAL_LEVEL_IN,
  SIGNAL_LEVEL_OUT,
  N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

static void
level_in_cb (FsuLevelFilter *filter, gdouble level, gpointer user_data)
{
  g_signal_emit (user_data, signals[SIGNAL_LEVEL_IN], 0, level);
}

static void
level_out_cb (FsuLevelFilter *filter, gdouble level, gpointer user_data)
{
  g_signal_emit (user_data, signals[SIGNAL_LEVEL_OUT], 0, level);
}

static void
skype_audio_call_construct_filters (SkypeAudioCall *self)
{
  SkypeAudioCallPrivate *priv = self->priv;
  FsuFilterManager *manager = NULL;
  FsuFilter *filter = NULL;

  /* Get outgoing filter */
  g_object_get (self, "outgoing-filter", &manager, NULL);

  if (manager == NULL)
    {
      g_debug ("Couldn't fetch outgoing-filter");
      goto error;
    }

  /* Add outgoing level filter */
  filter = FSU_FILTER (fsu_level_filter_new ());

  if (filter == NULL)
    {
      g_warning ("Error creating outgoing level filter");
      goto error;
    }

  if (fsu_filter_manager_append_filter (manager, filter) == NULL)
    {
      g_warning ("Error appending outgoing level filter");
      goto error;
    }

  priv->level_out = filter;
  priv->level_out_id = g_signal_connect (priv->level_out, "level",
      G_CALLBACK (level_out_cb), self);

  /* Add outgoing volume filter */
  filter = FSU_FILTER (fsu_volume_filter_new ());

  if (filter == NULL)
    {
      g_warning ("Error creating outgoing volume filter");
      goto error;
    }

  if (fsu_filter_manager_prepend_filter (manager, filter) == NULL)
    {
      g_warning ("Error appending outgoing volume filter");
      goto error;
    }

  priv->volume_out = filter;
  g_object_unref (manager);

  /* Get incoming filter */
  g_object_get (self, "incoming-filter", &manager, NULL);

  if (manager == NULL)
    {
      g_debug ("Couldn't fetch incoming-filter");
      goto error;
    }

  /* Add incoming level filter */
  filter = FSU_FILTER (fsu_level_filter_new ());

  if (filter == NULL)
    {
      g_warning ("Error creating incoming level filter");
      goto error;
    }

  if (fsu_filter_manager_append_filter (manager, filter) == NULL)
    {
      g_warning ("Error appending incoming level filter");
      goto error;
    }

  priv->level_in = filter;
  priv->level_in_id = g_signal_connect (priv->level_in, "level",
      G_CALLBACK (level_in_cb), self);

  /* Add incoming volume filter */
  filter = FSU_FILTER (fsu_volume_filter_new ());

  if (filter == NULL)
    {
      g_warning ("Error creating incoming volume filter");
      goto error;
    }

  if (fsu_filter_manager_prepend_filter (manager, filter) == NULL)
    {
      g_warning ("Error appending incoming volume filter");
      goto error;
    }

  priv->volume_in = filter;
  g_object_unref (manager);
  return;

error:
  if (manager != NULL)
    g_object_unref (manager);
  if (filter != NULL)
    g_object_unref (filter);
}

static void
skype_audio_call_dispose (GObject *object)
{
  SkypeAudioCallPrivate *priv = SKYPE_AUDIO_CALL (object)->priv;

  if (priv->level_in != NULL)
    {
      if (priv->level_in_id != 0)
        {
          g_signal_handler_disconnect (priv->level_in, priv->level_in_id);
          priv->level_in_id = 0;
        }

      g_object_unref (priv->level_in);
      priv->level_in = NULL;
    }

  if (priv->level_out != NULL)
    {
      if (priv->level_out_id != 0)
        {
          g_signal_handler_disconnect (priv->level_out, priv->level_out_id);
          priv->level_out_id = 0;
        }

      g_object_unref (priv->level_out);
      priv->level_out = NULL;
    }

  if (priv->volume_in != NULL)
    {
      g_object_unref (priv->volume_in);
      priv->volume_in = NULL;
    }

  if (priv->volume_out != NULL)
    {
      g_object_unref (priv->volume_out);
      priv->volume_out = NULL;
    }

  G_OBJECT_CLASS (skype_audio_call_parent_class)->dispose (object);
}

static void
skype_audio_call_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  SkypeAudioCallPrivate *priv = SKYPE_AUDIO_CALL (object)->priv;

  switch (property_id)
    {
      case PROP_VOLUME_IN:
        {
          gdouble volume;
          g_object_get (priv->volume_in, "volume", &volume, NULL);
          g_value_set_double (value, volume);
        }
        break;
      case PROP_VOLUME_OUT:
        {
          gdouble volume;
          g_object_get (priv->volume_out, "volume", &volume, NULL);
          g_value_set_double (value, volume);
        }
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
skype_audio_call_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  SkypeAudioCallPrivate *priv = SKYPE_AUDIO_CALL (object)->priv;

  switch (property_id)
    {
      case PROP_VOLUME_IN:
        g_object_set (priv->volume_in, "volume",
            g_value_get_double (value), NULL);
        break;
      case PROP_VOLUME_OUT:
        g_object_set (priv->volume_out, "volume",
            g_value_get_double (value), NULL);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
skype_audio_call_class_init (SkypeAudioCallClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (SkypeAudioCallPrivate));

  object_class->dispose = skype_audio_call_dispose;
  object_class->get_property = skype_audio_call_get_property;
  object_class->set_property = skype_audio_call_set_property;

  /**
   * SkypeAudioCall:volume-in:
   *
   * The volume level this call should use for incoming audio.
   *
   * 0.0 for mute, 1.0 for normal, 10.0 for 10 times amplification.
   */
  pspec = g_param_spec_double ("volume-in", "Volume in",
      "The volume level this stream should use for incoming audio",
      0.0, 10.0, 1.0,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_VOLUME_IN, pspec);

  /**
   * SkypeAudioCall:volume-out:
   *
   * The volume level this call should use for outgoing audio.
   *
   * 0.0 for mute, 1.0 for normal, 10.0 for 10 times amplification.
   */
  pspec = g_param_spec_double ("volume-out", "Volume out",
      "The volume level this stream should use for outgoing audio",
      0.0, 10.0, 1.0,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_VOLUME_OUT, pspec);

  /**
   * SkypeAudioCall::level-in
   * @call: the #SkypeAudioCall from which this level signal originated
   * @level: the current incoming volume level of this call in dB
   *
   * Emitted periodically to communicate the current incoming audio volume
   * level.
   */
  signals[SIGNAL_LEVEL_IN] = g_signal_new ("level-in",
      G_TYPE_FROM_CLASS (object_class),
      (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED),
      0, NULL, NULL, g_cclosure_marshal_VOID__DOUBLE, G_TYPE_NONE,
      1, G_TYPE_DOUBLE);

  /**
   * SkypeAudioCall::level-out
   * @call: the #SkypeAudioCall from which this level signal originated
   * @level: the current outgoing volume level of this call in dB
   *
   * Emitted periodically to communicate the current outgoing audio volume
   * level.
   */
  signals[SIGNAL_LEVEL_OUT] = g_signal_new ("level-out",
      G_TYPE_FROM_CLASS (object_class),
      (GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED),
      0, NULL, NULL, g_cclosure_marshal_VOID__DOUBLE, G_TYPE_NONE,
      1, G_TYPE_DOUBLE);
}

static void
skype_audio_call_init (SkypeAudioCall *self)
{
  self->priv = SKYPE_AUDIO_CALL_GET_PRIVATE (self);
}

/**
 * skype_audio_call_new
 *
 * Creates a new #SkypeAudioCall.
 *
 * Returns: the newly a created #SkypeAudioCall
 */
SkypeAudioCall *
skype_audio_call_new (const gchar *ipc_path_prefix,
    SkypeAudioStream *stream)
{
  SkypeAudioCall *call = g_object_new (SKYPE_TYPE_AUDIO_CALL,
      "ipc-path-prefix", ipc_path_prefix,
      NULL);

  skype_base_call_set_stream (SKYPE_BASE_CALL (call),
      SKYPE_BASE_STREAM (stream));

  skype_audio_call_construct_filters (call);

  return call;
}
