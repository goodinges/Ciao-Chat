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

#include "skype-base-call.h"
#include "skype-base-call-priv.h"

#include <gst/filters/fsu-filter-manager.h>

#include "skype-base-stream.h"

/**
 * SECTION:skype-base-call
 * @title: SkypeBaseCall
 * @short_description: base object for representing of media calls
 * @see_also: #SkypeAudioCall, #SkypeVideoCall
 *
 * The #SkypeBaseCall object is used as a base class to represent media calls.
 */

/**
 * SkypeBaseCall:
 *
 * The base call object used to represent media calls.
 */

/**
 * SkypeBaseCallClass:
 *
 * The class of a #SkypeBaseCall.
 */
G_DEFINE_TYPE (SkypeBaseCall, skype_base_call, G_TYPE_OBJECT)

#define SKYPE_BASE_CALL_GET_PRIVATE(o) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
    SKYPE_TYPE_BASE_CALL, \
    SkypeBaseCallPrivate))

struct _SkypeBaseCallPrivate
{
  gchar *ipc_path_prefix;
  SkypeBaseStream *stream;
};

enum
{
  PROP_0,
  PROP_IPC_PATH_PREFIX,
  PROP_INCOMING_FILTER,
  PROP_OUTGOING_FILTER,
};

enum
{
  SIGNAL_ENDED,
  N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

static void
skype_base_call_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  SkypeBaseCallPrivate *priv = SKYPE_BASE_CALL (object)->priv;

  switch (property_id)
    {
      case PROP_IPC_PATH_PREFIX:
        g_value_set_string (value, priv->ipc_path_prefix);
        break;
      case PROP_INCOMING_FILTER:
        if (priv->stream == NULL)
          {
            g_value_set_object (value, NULL);
          }
        else
          {
            FsuFilterManager *manager = NULL;
            g_object_get (priv->stream,
                "incoming-filter", &manager, NULL);
            g_value_take_object (value, manager);
          }
        break;
      case PROP_OUTGOING_FILTER:
        if (priv->stream == NULL)
          {
            g_value_set_object (value, NULL);
          }
        else
          {
            FsuFilterManager *manager = NULL;
            g_object_get (priv->stream,
                "outgoing-filter", &manager, NULL);
            g_value_take_object (value, manager);
          }
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
stream_weak_ref_cb (gpointer data, GObject *object)
{
  SkypeBaseCall *self = SKYPE_BASE_CALL (data);

  self->priv->stream = NULL;

  g_signal_emit (self, signals[SIGNAL_ENDED], 0);
}

void
skype_base_call_set_stream (SkypeBaseCall *self, SkypeBaseStream *stream)
{
  SkypeBaseCallPrivate *priv = self->priv;
  g_assert (priv->stream == NULL);
  priv->stream = stream;
  g_object_weak_ref (G_OBJECT (priv->stream), stream_weak_ref_cb, self);
}

static void
skype_base_call_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  SkypeBaseCallPrivate *priv = SKYPE_BASE_CALL (object)->priv;

  switch (property_id)
    {
      case PROP_IPC_PATH_PREFIX:
        priv->ipc_path_prefix = g_value_dup_string (value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
skype_base_call_dispose (GObject *object)
{
  SkypeBaseCallPrivate *priv = SKYPE_BASE_CALL (object)->priv;

  if (priv->stream != NULL)
    {
      g_object_weak_unref (G_OBJECT (priv->stream), stream_weak_ref_cb, object);
      priv->stream = NULL;
    }

  G_OBJECT_CLASS (skype_base_call_parent_class)->dispose (object);
}

static void
skype_base_call_finalize (GObject *object)
{
  SkypeBaseCallPrivate *priv = SKYPE_BASE_CALL (object)->priv;

  g_free (priv->ipc_path_prefix);

  G_OBJECT_CLASS (skype_base_call_parent_class)->finalize (object);
}

static void
skype_base_call_class_init (SkypeBaseCallClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (SkypeBaseCallPrivate));

  object_class->get_property = skype_base_call_get_property;
  object_class->set_property = skype_base_call_set_property;
  object_class->dispose = skype_base_call_dispose;
  object_class->finalize = skype_base_call_finalize;

  /**
   * SkypeBaseCall:ipc-path-prefix:
   *
   * The path prefix of the IPC socket used to communicate with Skypekit.
   *
   * NULL indicates the default prefix ('/tmp/').
   */
  pspec = g_param_spec_string ("ipc-path-prefix", "IPC path prefix",
      "The prefix to the path that contains the socket this base call uses",
      NULL,
      G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_IPC_PATH_PREFIX, pspec);

  /**
   * SkypeBaseCall:incoming-filter:
   *
   * The #FsuFilterManager that filters incoming media from the remote client.
   */
  pspec = g_param_spec_object ("incoming-filter", "Incoming filter",
      "The filter manager used to modifying the incoming stream",
      FSU_TYPE_FILTER_MANAGER,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_INCOMING_FILTER, pspec);

  /**
   * SkypeBaseCall:outgoing-filter:
   *
   * The #FsuFilterManager that filters outgoing media to the remote client.
   */
  pspec = g_param_spec_object ("outgoing-filter", "Outgoing filter",
      "The filter manager used to modify the outgoing stream",
      FSU_TYPE_FILTER_MANAGER,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_OUTGOING_FILTER, pspec);

  /**
   * SkypeBaseCall::ended
   * @call: the #SkypeBaseCall which has ended
   *
   * Emitted when this call has ended.
   */
  signals[SIGNAL_ENDED] = g_signal_new ("ended",
      G_TYPE_FROM_CLASS (object_class), G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
      0, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE,
      0, NULL);
}

static void
skype_base_call_init (SkypeBaseCall *self)
{
  self->priv = SKYPE_BASE_CALL_GET_PRIVATE (self);
}
