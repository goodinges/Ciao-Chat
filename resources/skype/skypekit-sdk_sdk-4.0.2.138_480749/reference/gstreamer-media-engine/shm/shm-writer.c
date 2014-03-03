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

#include "shm-writer.h"

#include <string.h>

#include "shmpipe.h"

/**
 * SECTION:shm-writer
 * @title: ShmWriter
 * @short_description: object for serving data buffers over shared memory
 * @see_also: #ShmReader
 *
 * The #ShmWriter object is used to send data buffers to one or more
 * connected clients over interprocess shared memory.
 */

/**
 * ShmWriter:
 *
 * The Shm Writer sends data buffers to one or more connected clients over
 * interprocess shared memory.
 */

/**
 * ShmWriterClass:
 *
 * The class of a #ShmWriter.
 */

G_DEFINE_TYPE (ShmWriter, shm_writer, G_TYPE_OBJECT)

#define SHM_WRITER_GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), SHM_TYPE_WRITER, ShmWriterPrivate))

#ifndef SHM_WRITER_DEFAULT_SIZE
#define SHM_WRITER_DEFAULT_SIZE 1024 * 256
#endif
#ifndef SHM_WRITER_DEFAULT_MODE
#define SHM_WRITER_DEFAULT_MODE S_IRWXU | S_IRWXG
#endif

struct _ShmWriterPrivate
{
  gchar *socket_path;
  gsize size;
  guint32 mode;
  gboolean open;

  ShmPipe *writer;
  GMainContext *context;

  GMutex *lock;
};

enum
{
  PROP_0,
  PROP_CONTEXT,
  PROP_SOCKET_PATH,
  PROP_SIZE,
  PROP_MODE,
  PROP_OPEN,
};

static void
shm_writer_get_property (GObject *object, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  ShmWriterPrivate *priv = SHM_WRITER (object)->priv;

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
      case PROP_SIZE:
        g_value_set_uint (value, priv->size);
        break;
      case PROP_MODE:
        g_value_set_uint (value, priv->mode);
        break;
      case PROP_OPEN:
        g_value_set_boolean (value, priv->open);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
shm_writer_set_property (GObject *object, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  ShmWriterPrivate *priv = SHM_WRITER (object)->priv;

  switch (property_id)
    {
      case PROP_CONTEXT:
        if (priv->context != NULL)
          g_main_context_unref (priv->context);
        priv->context = g_value_get_pointer (value);
        if (priv->context != NULL)
          g_main_context_ref (priv->context);
        break;
      case PROP_SIZE:
        {
          guint new_size = g_value_get_uint (value);

          if (priv->size == new_size)
            break;

          if (priv->open)
            {
              if (sp_writer_resize (priv->writer, new_size) ==
                  SHM_WRITER_FAILURE)
                /* Technically this should probably return an error,
                 * but being that this is a property setter.. */
                break;
            }

          priv->size = new_size;
          break;
        }
      case PROP_MODE:
        {
          guint new_mode = g_value_get_uint (value);

          if (priv->open)
            {
              sp_writer_setperms_shm (priv->writer, new_mode);
            }

          priv->mode = new_mode;
          break;
        }
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
shm_writer_dispose (GObject *object)
{
  ShmWriterPrivate *priv = SHM_WRITER (object)->priv;

  shm_writer_close (SHM_WRITER (object));

  if (priv->context != NULL)
    {
      g_main_context_unref (priv->context);
      priv->context = NULL;
    }

  G_OBJECT_CLASS (shm_writer_parent_class)->dispose (object);
}

static void
shm_writer_finalize (GObject *object)
{
  g_mutex_free (SHM_WRITER (object)->priv->lock);
  G_OBJECT_CLASS (shm_writer_parent_class)->finalize (object);
}

static void
shm_writer_class_init (ShmWriterClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (ShmWriterPrivate));

  object_class->get_property = shm_writer_get_property;
  object_class->set_property = shm_writer_set_property;
  object_class->dispose = shm_writer_dispose;
  object_class->finalize = shm_writer_finalize;

  /**
   * ShmWriter:context:
   *
   * The context within which operations of this writer should run.
   *
   * Pass NULL to indicate the default context.
   */
  pspec = g_param_spec_pointer ("context", "Context",
      "The context within which operation of this writer should run",
      G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_CONTEXT, pspec);

  /**
   * ShmWriter:socket-path:
   *
   * The current socket path of an open #ShmWriter.
   *
   * This property will %NULL when the #ShmWriter is not open.
   */
  pspec = g_param_spec_string ("socket-path", "Socket path",
      "The path to the writer socket",
      NULL,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_SOCKET_PATH, pspec);

  /**
   * ShmWriter:size:
   *
   * The size of the address space to allocate for this #ShmWriter.
   */
  pspec = g_param_spec_uint ("size", "Size",
      "The size of the address space to allocate for this shm writer",
      0, G_MAXSIZE, SHM_WRITER_DEFAULT_SIZE,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_SIZE, pspec);

  /**
   * ShmWriter:mode:
   *
   * The permission mode in which to open the #ShmWriter's socket.
   */
  pspec = g_param_spec_uint ("mode", "Mode",
      "The permissions to use for the writer socket",
      0, G_MAXSIZE, SHM_WRITER_DEFAULT_MODE,
      G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
  g_object_class_install_property (object_class, PROP_MODE, pspec);

  /**
   * ShmWriter:open:
   *
   * Whether or not the #ShmWriter is open.
   *
   * Notifications of this property's value changing can be received through
   * the #GObject::notify signal.
   *
   * %TRUE if the #ShmWriter is open; %FALSE otherwise.
   */
  pspec = g_param_spec_boolean ("open", "Open",
      "Whether or not the writer socket is open",
      FALSE,
      G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
  g_object_class_install_property (object_class, PROP_OPEN, pspec);
}

static void
shm_writer_init (ShmWriter *self)
{
  self->priv = SHM_WRITER_GET_PRIVATE (self);

  self->priv->size = SHM_WRITER_DEFAULT_SIZE;
  self->priv->mode = SHM_WRITER_DEFAULT_MODE;

  self->priv->lock = g_mutex_new ();
}

/**
 * shm_writer_new
 *
 * Creates a new #ShmWriter.
 *
 * Returns: a newly created #ShmWriter
 */
ShmWriter *
shm_writer_new (void)
{
  return g_object_new (SHM_TYPE_WRITER, NULL);
}

/**
 * shm_writer_new_with_context
 *
 * Creates a new #ShmWriter with a specific #GMainContext.
 *
 * Returns: a newly created #ShmWriter
 */
ShmWriter *
shm_writer_new_with_context (GMainContext *context)
{
  return g_object_new (SHM_TYPE_WRITER,
      "context", context,
      NULL);
}

static void
shm_writer_destroy_source (gpointer user_data, GObject *object)
{
  g_source_destroy (user_data);
}

static void
shm_writer_add_fd_source (ShmWriter *self,
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

  /* Weak ref so the source will be destroyed when the writer is destroyed */
  g_object_weak_ref (G_OBJECT (self), shm_writer_destroy_source, source);

  g_source_set_callback (source, (GSourceFunc)callback, user_data, notify);
  g_source_attach (source, self->priv->context);
  g_source_unref (source);
}

typedef struct
{
  ShmWriter *self;
  ShmClient *client;
} ShmWriterClientPair;

/*
 * This function is called when a client source is destroyed. If it's
 * destroyed by an IO error, then the client is freed in the source handler.
 * If it's destroyed by freeing the ShmWriter, then the client and the writer
 * are freed in the ShmWriter's dispose function. Therefore this function
 * only needs to free the structure holding them.
 */
static void
shm_writer_client_pair_free (ShmWriterClientPair *pair)
{
  g_slice_free (ShmWriterClientPair, pair);
}

static gboolean
shm_writer_client_fd_cb (GIOChannel *source,
    GIOCondition condition,
    gpointer data)
{
  ShmWriterClientPair *pair = data;
  ShmWriterPrivate *priv = pair->self->priv;

  if (condition == G_IO_IN)
    {
      g_mutex_lock (priv->lock);
      if (sp_writer_recv (pair->self->priv->writer, pair->client) != 0)
        goto error_locked;
      g_mutex_unlock (priv->lock);
    }
  else if (condition & (G_IO_ERR | G_IO_HUP | G_IO_NVAL))
    {
      goto error;
    }

  return TRUE;

error:
  g_mutex_lock (priv->lock);
error_locked:
  if (priv->writer != NULL)
    sp_writer_close_client (priv->writer, pair->client);
  g_mutex_unlock (priv->lock);

  /* No need to destroy the source when the writer is destroyed.
   * We're destroying it here */
  g_object_weak_unref (G_OBJECT (pair->self),
      shm_writer_destroy_source, g_main_current_source ());
  return FALSE;
}

static gboolean
shm_writer_fd_cb (GIOChannel *source,
    GIOCondition condition,
    gpointer data)
{
  ShmWriterPrivate *priv = SHM_WRITER (data)->priv;

  if (condition == G_IO_IN)
    {
      ShmWriterClientPair *pair;
      ShmClient *client;
      int fd;

      client = sp_writer_accept_client (priv->writer);

      if (client == NULL)
        goto out;

      /* Start listening on this socket.
       * This socket is used for the actual shm data transfer signaling
       * of an individual client. So when this socket is closed, the
       * client associated with this particular socket has disconnected */
      pair = g_slice_new0 (ShmWriterClientPair);
      pair->self = data;
      pair->client = client;

      fd = sp_writer_get_client_fd (client);

      shm_writer_add_fd_source (data, fd, shm_writer_client_fd_cb,
          pair, (GDestroyNotify)shm_writer_client_pair_free);
    }
  else if (condition & (G_IO_ERR | G_IO_HUP | G_IO_NVAL))
    {
      goto out;
    }

  return TRUE;

out:
  shm_writer_close (data);

  /* No need to destroy the source when the writer is destroyed.
   * We're destroying it here */
  g_object_weak_unref (G_OBJECT (data),
      shm_writer_destroy_source, g_main_current_source ());
  return FALSE;
}

/**
 * shm_writer_open
 * @self: the #ShmWriter instance to open
 * @preferred_socket_path: the preferred socket path to use for this #ShmWriter
 *
 * Opens an unopened #ShmWriter to start accepting connections from clients.
 * The @preferred_socket_path is a suggestion for the path of the socket the
 * #ShmWriter will use. If the path is available it will be used. Otherwise,
 * the ShmWriter will pick a name derived from it.
 *
 * Returns: the path actually chosen by the #ShmWriter.
 */
const gchar *
shm_writer_open (ShmWriter *self, const gchar *preferred_socket_path)
{
  ShmWriterPrivate *priv = self->priv;
  int fd;

  if (priv->open)
    return NULL;

  priv->writer = sp_writer_create (preferred_socket_path,
      priv->size, priv->mode);

  if (priv->writer == NULL)
    {
      g_warning ("Unable to open ShmWriter: %p", self);
      return NULL;
    }

  fd = sp_get_fd (priv->writer);

  /* Listen for events on the writer's fd.
   * This fd is used for accepting connections from ShmReaders
   * (or anything that knows the protocol used). Separate fds
   * are then opened for actual data signaling */
  shm_writer_add_fd_source (self, fd, shm_writer_fd_cb,
      self, NULL);

  priv->socket_path = g_strdup (sp_writer_get_path (priv->writer));

  priv->open = TRUE;
  g_object_notify (G_OBJECT (self), "open");
  return priv->socket_path;
}

/**
 * shm_writer_close
 * @self: the #ShmWriter to close
 *
 * Closes an open #ShmWriter and disconnects all connected clients.
 */
void
shm_writer_close (ShmWriter *self)
{
  ShmWriterPrivate *priv = self->priv;

  g_mutex_lock (priv->lock);

  if (!priv->open)
    goto out;

  g_free (priv->socket_path);
  priv->socket_path = NULL;

  if (priv->writer != NULL)
    {
      sp_close (priv->writer);
      priv->writer = NULL;
    }

  priv->open = FALSE;
  g_object_notify (G_OBJECT (self), "open");

out:
  g_mutex_unlock (priv->lock);
}

/**
 * shm_writer_send_buffer
 * @self: the #ShmWriter with which to send the data buffer
 * @buffer: the data buffer to send to all connected clients
 * @len: the length of the data @buffer
 *
 * Sends a buffer of data to all connected clients. The data is internally
 * copied into the appropriate shm area so the passed buffer doesn't need
 * to be saved.
 *
 * Returns: the number of clients the buffer was successfully sent to or %-1
 *          on error. This number only signifies that it was sent successfully,
 *          not that the clients processed it successfully.
 */
int
shm_writer_send_buffer (ShmWriter *self, const gchar *buffer, gsize len)
{
  ShmWriterPrivate *priv = self->priv;
  ShmBlock *block;
  gchar *buf;
  int ret;

  if (!priv->open)
    {
      g_debug ("ShmWriter not open");
      return SHM_WRITER_FAILURE;
    }

  /* Allocating, copying, and freeing all this may seem silly,
   * but the data needs to be in the correct range in memory
   * in order to share it with other processes */
  g_mutex_lock (priv->lock);
  block = sp_writer_alloc_block (priv->writer, len);

  if (block == NULL)
    {
      g_debug ("ShmWriter: unable to allocate block");
      g_mutex_unlock (priv->lock);
      return SHM_WRITER_FAILURE;
    }

  buf = sp_writer_block_get_buf (block);
  memcpy (buf, buffer, len);

  ret = sp_writer_send_buf (priv->writer, buf, len);

  sp_writer_free_block (block);
  g_mutex_unlock (priv->lock);
  return ret;
}
