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

#ifndef __SHM_WRITER_H__
#define __SHM_WRITER_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define SHM_TYPE_WRITER shm_writer_get_type()

#define SHM_WRITER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SHM_TYPE_WRITER, ShmWriter))

#define SHM_WRITER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SHM_TYPE_WRITER, ShmWriterClass))

#define SHM_IS_WRITER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SHM_TYPE_WRITER))

#define SHM_IS_WRITER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SHM_TYPE_WRITER))

#define SHM_WRITER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SHM_TYPE_WRITER, ShmWriterClass))

#define SHM_WRITER_FAILURE -1

typedef struct _ShmWriterPrivate ShmWriterPrivate;

typedef struct {
  GObject parent;
  ShmWriterPrivate *priv;
} ShmWriter;

typedef struct {
  GObjectClass parent_class;
} ShmWriterClass;

GType shm_writer_get_type (void);

ShmWriter *shm_writer_new (void);

ShmWriter *shm_writer_new_with_context (GMainContext *context);

const gchar *shm_writer_open (ShmWriter *self, const gchar *preferred_socket_path);

void shm_writer_close (ShmWriter *self);

int shm_writer_send_buffer (ShmWriter *self, const gchar *buffer, gsize len);

G_END_DECLS

#endif /* __SHM_WRITER_H__ */
