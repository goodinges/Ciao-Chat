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

#ifndef __SKYPE_AUDIO_STREAM_H__
#define __SKYPE_AUDIO_STREAM_H__

#include "skype-base-stream.h"

#include <gst/gst.h>

G_BEGIN_DECLS

#define SKYPE_TYPE_AUDIO_STREAM skype_audio_stream_get_type()

#define SKYPE_AUDIO_STREAM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SKYPE_TYPE_AUDIO_STREAM, SkypeAudioStream))

#define SKYPE_AUDIO_STREAM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SKYPE_TYPE_AUDIO_STREAM, SkypeAudioStreamClass))

#define SKYPE_IS_AUDIO_STREAM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SKYPE_TYPE_AUDIO_STREAM))

#define SKYPE_IS_AUDIO_STREAM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SKYPE_TYPE_AUDIO_STREAM))

#define SKYPE_AUDIO_STREAM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SKYPE_TYPE_AUDIO_STREAM, SkypeAudioStreamClass))

typedef struct _SkypeAudioStreamPrivate SkypeAudioStreamPrivate;

typedef struct {
  SkypeBaseStream parent;
  SkypeAudioStreamPrivate *priv;
} SkypeAudioStream;

typedef struct {
  SkypeBaseStreamClass parent_class;
} SkypeAudioStreamClass;

GType skype_audio_stream_get_type (void);

SkypeAudioStream *skype_audio_stream_new (guint channel_id);

SkypeAudioStream *skype_audio_stream_new_with_context (guint channel_id,
    GMainContext *context);

SkypeAudioStream *skype_audio_stream_new_full (guint channel_id,
    GMainContext *context,
    GstElement *pipeline,
    GstElement *source,
    GstElement *sink);

gboolean skype_audio_stream_send_dtmf (SkypeAudioStream *self,
    int event,
    int type);

G_END_DECLS

#endif /* __SKYPE_AUDIO_STREAM_H__ */
