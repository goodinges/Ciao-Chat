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

#include "skype-video-call.h"

#include <gst/filters/fsu-preview-filter.h>

#include "skype-base-call-priv.h"

/**
 * SECTION:skype-video-call
 * @title: SkypeVideoCall
 * @short_description: object for representing of video calls
 * @see_also: #SkypeBaseCall, #SkypeAudioCall
 *
 * The #SkypeVideoCall object is used to represent video calls.
 */

/**
 * SkypeVideoCall:
 *
 * The call object used to represent video calls.
 */

/**
 * SkypeVideoCallClass:
 *
 * The class of a #SkypeVideoCall.
 */
G_DEFINE_TYPE (SkypeVideoCall, skype_video_call, SKYPE_TYPE_BASE_CALL)

static void
skype_video_call_class_init (SkypeVideoCallClass *klass)
{
}

static void
skype_video_call_init (SkypeVideoCall *self)
{
}

/**
 * skype_video_call_new
 *
 * Creates a new #SkypeVideoCall
 *
 * Returns: the newly a created #SkypeVideoCall
 */
SkypeVideoCall *
skype_video_call_new (const gchar *ipc_path_prefix,
    SkypeVideoStream *stream)
{
  SkypeVideoCall *call = g_object_new (SKYPE_TYPE_VIDEO_CALL,
      "ipc-path-prefix", ipc_path_prefix,
      NULL);

  skype_base_call_set_stream (SKYPE_BASE_CALL (call),
      SKYPE_BASE_STREAM (stream));

  return call;
}

/**
 * skype_video_call_add_preview_window
 * @self: the #SkypeVideoCall for which to add a preview window
 * @xid: the window xid or HWND to which the preview window shall be bound or 0
 * @preview_manager: address to store the pointer to the filter manager of the preview window or NULL
 * 
 * Adds a preview window bound to the passed xid. 0 may be passed which causes
 * the sink to create its own window.
 *
 * Returns: the %TRUE on success; %FALSE otherwise
 */
gboolean
skype_video_call_add_preview_window (SkypeVideoCall *call, guint xid,
    FsuFilterManager **preview_manager)
{
  FsuFilterManager *manager = NULL;
  FsuFilter *filter = NULL;

  g_object_get (call, "outgoing-filter", &manager, NULL);

  if (manager == NULL)
    {
      g_debug ("Couldn't fetch outgoing-filter");
      goto error;
    }

  filter = FSU_FILTER (fsu_preview_filter_new (xid));

  if (filter == NULL)
    {
      g_warning ("Error creating preview filter");
      goto error;
    }

  if (fsu_filter_manager_append_filter (manager, filter) == NULL)
    {
      g_warning ("Error appending preview filter");
      goto error;
    }

  if (preview_manager != NULL)
      g_object_get (filter, "filter-manager", preview_manager, NULL);

  g_object_unref (filter);
  g_object_unref (manager);

  return TRUE;

error:
  if (manager != NULL)
    g_object_unref (manager);
  if (filter != NULL)
    g_object_unref (filter);
  return FALSE;
}
