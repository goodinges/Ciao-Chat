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

#ifndef __SKYPE_BASE_STREAM_H__
#define __SKYPE_BASE_STREAM_H__

#include <glib-object.h>
#include <gst/gst.h>

G_BEGIN_DECLS

#define SKYPE_TYPE_BASE_STREAM skype_base_stream_get_type()

#define SKYPE_BASE_STREAM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SKYPE_TYPE_BASE_STREAM, SkypeBaseStream))

#define SKYPE_BASE_STREAM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SKYPE_TYPE_BASE_STREAM, SkypeBaseStreamClass))

#define SKYPE_IS_BASE_STREAM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SKYPE_TYPE_BASE_STREAM))

#define SKYPE_IS_BASE_STREAM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SKYPE_TYPE_BASE_STREAM))

#define SKYPE_BASE_STREAM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SKYPE_TYPE_BASE_STREAM, SkypeBaseStreamClass))

typedef struct _SkypeBaseStreamPrivate SkypeBaseStreamPrivate;

typedef struct {
  GObject parent;
  SkypeBaseStreamPrivate *priv;
} SkypeBaseStream;

typedef struct {
  GObjectClass parent_class;

  gboolean (*pre_create_stream) (SkypeBaseStream *self,
      gpointer *source, gpointer *sink);
  gboolean (*post_create_stream) (SkypeBaseStream *self, gpointer stream);
  gboolean (*deinit_stream) (SkypeBaseStream *self, gpointer stream);
  void (*update_codecs) (SkypeBaseStream *self, GList **codecs);
  gboolean (*handle_message) (SkypeBaseStream *self, GstMessage *message);
} SkypeBaseStreamClass;

GType skype_base_stream_get_type (void);

gboolean skype_base_stream_init_stream (SkypeBaseStream *stream);

gboolean skype_base_stream_play (SkypeBaseStream *stream, gboolean play);

gboolean skype_base_stream_record (SkypeBaseStream *stream, gboolean record);

void skype_base_stream_deinit_stream (SkypeBaseStream *stream);

gboolean skype_base_stream_push_buffer (SkypeBaseStream *stream,
    const gchar *buffer, gsize len);

gboolean skype_base_stream_handle_message (SkypeBaseStream *self,
    GstMessage *message);

G_END_DECLS

#endif /* __SKYPE_BASE_STREAM_H__ */
