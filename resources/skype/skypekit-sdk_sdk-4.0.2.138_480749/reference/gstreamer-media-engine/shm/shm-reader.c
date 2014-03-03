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

#include "shm-reader.h"

#include "shmpipe.h"

/**
 * SECTION:shm-reader
 * @title: ShmReader
 * @short_description: object for receiving data buffers over shared memory
 * @see_also: #ShmWriter
 *
 * The #ShmReader object is used to connect to and receive data buffers from a
 * shared memory server, such as #ShmWriter, over interprocess shared memory.
 */

/**
 * ShmReader:
 *
 * The Shm Reader connects to and receives data buffers from a shared
 * memory server, such as #ShmWriter, over interprocess shared memory.
 */

/**
 * ShmReaderClass:
 *
 * The class of a #ShmReader.
 */

G_DEFINE_TYPE (ShmReader, shm_reader, G_TYPE_OBJECT)

#define SHM_READER_GET_PRIVATE(o) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
    SHM_TYPE_READER, ShmReaderPrivate))

struct _ShmReaderPrivate
{
  gchar *socket_path;
  gboolean connected;

  ShmPipe *client;
  GSource *source;
  GMainContext *context;
};

enum
{
  PROP_0,
  PROP_CONTEXT,
  PROP_SOCKET_PATH,
  PROP_CONNECTED,
};

enum
{
  SIGNAL_GOT_DATA,
  N_SIGNALS
};

static guint signals[N_SIGNALS] = {0};

static void
shm_reader_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  ShmReaderPrivate *priv = SHM_READER (object)->priv;

  switch (property_id)
    {
      case PROP_CONTEXT:
        if (priv->context != NULL)
          g_main_context_ref (priv->context);
        g_value_set_pointer (value, priv->context);
        break;
      case PROP_SOCKET_PATH:
        g_value_set_string (value, priv->socket_path);
        break;
      case PROP_CONNECTED:
        g_value_set_boolean (value, priv->connected);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
shm_reader_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  ShmReaderPrivate *priv = SHM_READER (object)->priv;

  switch (property_id)
    {
      case PROP_CONTEXT:
        if (priv->context != NULL)
          g_main_context_unref (priv->context);
        priv->context = g_value_get_pointer (value);
        if (priv->context != NULL)
          g_main_context_ref (priv->context);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
shm_reader_dispose (GObject *object)
{
  ShmReaderPrivate *priv = SHM_READER (object)->priv;

  shm_reader_disconnect (SHM_READER (object));

  if (priv->context != NULL)
    {
      g_main_context_unref (priv->context);
      priv->context = NULL;
    }

  G_OBJECT_CLASS (shm_reader_parent_class)->dispose (object);
}

static void
shm_reader_finalize (GObject *object)
{
  G_OBJECT_CLASS (shm_reader_parent_class)->finalize (object);
}

static void
shm_reader_class_init (ShmReaderClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (ShmReaderPrivate));

  object_class->get_property = shm_reader_get_property;
  object_class->set_property = shm_reader_set_property;
  object_class->dispose = shm_reader_dispose;
  object_class->finalize = shm_reader_finalize;

  /**
   * ShmReader:context:
   *
   * The context within which operations of this reader should run.
   *
   * Pass NULL to indicate the default context.
   */
  pspec = g_param_spec_pointer ("context", "Context",
      "The context within which operation of this reader should run",
      G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_CONTEXT, pspec);

  /**
   * ShmReader:socket-path:
   *
   * The socket-path the #ShmReader is currently connected to.
   *
   * This property will be %NULL if the #ShmReader isn't connected.
   */
  pspec = g_param_spec_string ("socket-path", "Socket path",
      "The path to the writer socket",
      NULL,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_SOCKET_PATH, pspec);

  /**
   * ShmReader:connected:
   *
   * Whether or not the #ShmReader is connected
   *
   * Notifications of this property's value changing can be received through
   * the #GObject::notify signal.
   *
   * %TRUE if the #ShmReader is connected; %FALSE otherwise.
   */
  pspec = g_param_spec_boolean ("connected", "Connected",
      "True if the client is connected, false otherwise",
      FALSE,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_CONNECTED, pspec);

  /**
   * ShmReader::got-data
   * @reader: the #ShmReader that received the data buffer
   * @len: the length of the data @buffer received.
   * @buffer: the data buffer received.
   *
   * Emitted when a data buffer is received.
   *
   * The data buffer needs to be copied if it's to be used past
   * the return of this signal call.
   */
  signals[SIGNAL_GOT_DATA] = g_signal_new ("got-data",
      G_TYPE_FROM_CLASS (object_class), G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
      0, NULL, NULL, g_cclosure_marshal_VOID__UINT_POINTER, G_TYPE_NONE,
      2, G_TYPE_UINT, G_TYPE_POINTER, NULL);
}

static void
shm_reader_init (ShmReader *self)
{
  self->priv = SHM_READER_GET_PRIVATE (self);
}

/**
 * shm_reader_new
 *
 * Creates a new #ShmReader.
 *
 * Returns: the newly a created #ShmReader
 */
ShmReader *
shm_reader_new (void)
{
  return g_object_new (SHM_TYPE_READER, NULL);
}

/**
 * shm_reader_new_with_context
 *
 * Creates a new #ShmReader with a specific #GMainContext.
 *
 * Returns: the newly a created #ShmReader
 */
ShmReader *
shm_reader_new_with_context (GMainContext *context)
{
  return g_object_new (SHM_TYPE_READER,
      "context", context,
      NULL);
}

static void
shm_reader_add_fd_source (ShmReader *self,
    int fd,
    GIOFunc callback,
    gpointer user_data,
    GDestroyNotify notify)
{
  GIOChannel *channel;
  GSource *source;

  channel = g_io_channel_unix_new (fd);
  source = g_io_create_watch (channel,
      G_IO_IN | G_IO_ERR | G_IO_HUP | G_IO_NVAL);
  g_io_channel_unref (channel);

  g_source_set_callback (source, (GSourceFunc)callback, user_data, notify);
  g_source_attach (source, self->priv->context);
  self->priv->source = source;
}

static gboolean
shm_reader_fd_cb (GIOChannel *source,
    GIOCondition condition,
    gpointer data)
{
  ShmReaderPrivate *priv = SHM_READER (data)->priv;

  if (condition & G_IO_IN)
    {
      gchar *buf;
      gulong ret;

      ret = sp_client_recv (priv->client, &buf);

      if (ret == -1)
        {
          goto error;
        }
      else if (ret > 0)
        {
          g_signal_emit (data, signals[SIGNAL_GOT_DATA], 0, ret, buf);

          if (sp_client_recv_finish (priv->client, buf) == 0)
            goto error;
        }
      /* else: it was a system message and already handled */
    }
  else if (condition & (G_IO_ERR | G_IO_HUP | G_IO_NVAL))
    {
      goto error;
    }

  return TRUE;

error:
  shm_reader_disconnect (data);
  return FALSE;

}

/**
 * shm_reader_connect
 * @self: the #ShmReader to use to connect
 * @socket_path: the path to the socket to connect to.
 *
 * Connects an unconnected #ShmReader to the given @socket_path. After
 * a successful connection, #ShmReader::got-data will be triggered when
 * data buffers are received.
 *
 * Returns: %TRUE if succeeded; %FALSE otherwise
 */
gboolean
shm_reader_connect (ShmReader *self, const gchar *socket_path)
{
  ShmReaderPrivate *priv = self->priv;
  int fd;

  if (priv->connected)
    return FALSE;

  priv->client = sp_client_open (socket_path);

  if (priv->client == NULL)
    return FALSE;

  priv->socket_path = g_strdup (socket_path);
  fd = sp_get_fd (priv->client);

  shm_reader_add_fd_source (self, fd, shm_reader_fd_cb,
      self, NULL);

  priv->connected = TRUE;
  g_object_notify (G_OBJECT (self), "connected");

  return TRUE;
}

/**
 * shm_reader_disconnect
 * @self: the #ShmReader to disconnect.
 *
 * Disconnects a connected #ShmReader.
 */
void
shm_reader_disconnect (ShmReader *self)
{
  ShmReaderPrivate *priv = self->priv;

  if (!priv->connected)
    return;

  if (priv->client != NULL)
    {
      sp_close (priv->client);
      priv->client = NULL;
    }

  if (priv->source != NULL)
    {
       g_source_destroy (priv->source);
       g_source_unref (priv->source);
       priv->source = NULL;
    }

  g_free (priv->socket_path);
  priv->socket_path = NULL;

  priv->connected = FALSE;
  g_object_notify (G_OBJECT (self), "connected");
}
