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

#ifndef __SKYPE_VIDEO_CALL_H__
#define __SKYPE_VIDEO_CALL_H__

#include "skype-base-call.h"

#include <glib-object.h>

#include <gst/filters/fsu-filter-manager.h>

G_BEGIN_DECLS

#define SKYPE_TYPE_VIDEO_CALL skype_video_call_get_type()

#define SKYPE_VIDEO_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SKYPE_TYPE_VIDEO_CALL, SkypeVideoCall))

#define SKYPE_VIDEO_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SKYPE_TYPE_VIDEO_CALL, SkypeVideoCallClass))

#define SKYPE_IS_VIDEO_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SKYPE_TYPE_VIDEO_CALL))

#define SKYPE_IS_VIDEO_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SKYPE_TYPE_VIDEO_CALL))

#define SKYPE_VIDEO_CALL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SKYPE_TYPE_VIDEO_CALL, SkypeVideoCallClass))

typedef struct {
  SkypeBaseCall parent;
} SkypeVideoCall;

typedef struct {
  SkypeBaseCallClass parent_class;
} SkypeVideoCallClass;

GType skype_video_call_get_type (void);

gboolean skype_video_call_add_preview_window (SkypeVideoCall *call, guint xid,
    FsuFilterManager **preview_manager);

G_END_DECLS

#endif /* __SKYPE_VIDEO_CALL_H__ */
