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

#ifndef __SHM_READER_H__
#define __SHM_READER_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define SHM_TYPE_READER shm_reader_get_type()

#define SHM_READER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SHM_TYPE_READER, ShmReader))

#define SHM_READER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SHM_TYPE_READER, ShmReaderClass))

#define SHM_IS_READER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SHM_TYPE_READER))

#define SHM_IS_READER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SHM_TYPE_READER))

#define SHM_READER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SHM_TYPE_READER, ShmReaderClass))

typedef struct _ShmReaderPrivate ShmReaderPrivate;

typedef struct {
  GObject parent;
  ShmReaderPrivate *priv;
} ShmReader;

typedef struct {
  GObjectClass parent_class;
} ShmReaderClass;

GType shm_reader_get_type (void);

ShmReader *shm_reader_new (void);

ShmReader *shm_reader_new_with_context (GMainContext *context);

gboolean shm_reader_connect (ShmReader *self, const gchar *socket_path);

void shm_reader_disconnect (ShmReader *self);

G_END_DECLS

#endif /* __SHM_READER_H__ */
