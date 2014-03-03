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

#include "skype-media-engine.h"
#include "skype-audio-call.h"
#include "skype-video-call.h"

#include <gst/gst.h>

typedef struct
{
  guint refs;
  gdouble level_in;
  gdouble level_out;
} LevelInfoData;

static void
level_info_data_free (gpointer data, GClosure *closure)
{
  LevelInfoData *info = data;

  if (--info->refs > 0)
    return;

  g_slice_free (LevelInfoData, info);
}

static void
level_in_cb (SkypeAudioCall *call, gdouble level, gpointer user_data)
{
  LevelInfoData *info = user_data;
  info->level_in = level;
  g_print ("level-in: %f level-out: %f\r", info->level_in, info->level_out);
}

static void
level_out_cb (SkypeAudioCall *call, gdouble level, gpointer user_data)
{
  LevelInfoData *info = user_data;
  info->level_out = level;
  g_print ("level-in: %f level-out: %f\r", info->level_in, info->level_out);
}

static void
new_audio_call_cb (SkypeMediaEngine *engine,
    SkypeAudioCall *call,
    gpointer user_data)
{
  LevelInfoData *info;

  info = g_slice_new0 (LevelInfoData);
  info->refs = 2;
  info->level_in = 0;
  info->level_out = 0;

  /* Ref this to keep it alive past the end of this callback */
  g_object_ref (call);

  /* Connect to signals etc to show level messages with '\r' */
  g_signal_connect_data (call, "level-in", G_CALLBACK (level_in_cb),
      info, level_info_data_free, 0);
  g_signal_connect_data (call, "level-out", G_CALLBACK (level_out_cb),
      info, level_info_data_free, 0);
  g_signal_connect (call, "ended", G_CALLBACK (g_object_unref), NULL);
}

static void
new_video_call_cb (SkypeMediaEngine *engine,
    SkypeVideoCall *call,
    gpointer user_data)
{
  if (!skype_video_call_add_preview_window (call, 0, NULL))
    g_warning ("Error adding preview window to video call");
}

int
main (int argc,
    char **argv)
{
  SkypeMediaEngine *engine;
  const gchar *path = NULL;

  g_thread_init (NULL);
  g_type_init ();
  gst_init (&argc, &argv);

  engine = skype_media_engine_new ();

  if (engine == NULL)
    {
      g_warning ("Error creating media engine.");
      return 1;
    }

  g_signal_connect (engine, "new-audio-call",
      G_CALLBACK (new_audio_call_cb), NULL);
  g_signal_connect (engine, "new-video-call",
      G_CALLBACK (new_video_call_cb), NULL);

  if (argc >= 2)
    path = argv[1];

  g_debug ("Using path: %s", path);

  if (!skype_media_engine_start (engine, SKYPE_MEDIA_BOTH,
      path))
    g_warning ("Error starting media engine");

  g_object_unref (engine);

  gst_deinit ();
  return 0;
}
