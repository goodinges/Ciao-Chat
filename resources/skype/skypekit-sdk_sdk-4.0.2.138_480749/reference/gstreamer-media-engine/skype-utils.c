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

#include "skype-utils.h"

#include <gobject/gvaluecollector.h>
#include <gst/farsight/fs-base-conference.h>
#include <string.h>

typedef struct {
  GMutex *mutex;
  GstElement *element;
  GValue value;
  gboolean value_is_set;
} SkypeDynamicProperty;

struct _SkypeDynamicProperties
{
  FsElementAddedNotifier *notifier;
  GHashTable *props;
  gulong element_added_id;
};

SkypeDynamicProperty *
skype_dynamic_property_new ()
{
  SkypeDynamicProperty *prop = g_slice_new0 (SkypeDynamicProperty);
  prop->mutex = g_mutex_new ();
  return prop;
}

void
skype_dynamic_property_free (gpointer data)
{
  SkypeDynamicProperty *prop = data;
  GMutex *mutex = prop->mutex;
  g_mutex_lock (mutex);

  if (prop->element != NULL)
    gst_object_unref (prop->element);

  g_slice_free (SkypeDynamicProperty, prop);

  g_mutex_unlock (mutex);
  g_mutex_free (mutex);
}

static void
set_element_property (SkypeDynamicProperty *prop,
    const gchar *key)
{
  const gchar *element_name = gst_element_factory_get_longname(
      gst_element_get_factory(prop->element));

  if (!g_strcmp0 (key, "bitrate"))
    {
      guint bitrate = g_value_get_uint (&prop->value);

      if (!g_strcmp0 (element_name, "x264enc"))
        {
          /* x264enc has a bitrate property in kbit/s, we want the bitrate
           * property to be in byte/s */
          g_debug ("Modifying x264enc bitrate value from %u to %u",
              bitrate, bitrate / 128);
          bitrate /= 128;
        }

      if (bitrate <= 0)
        bitrate = 1;

      g_object_set (G_OBJECT (prop->element), key, bitrate, NULL);
    }
  else
    {
      g_object_set_property (G_OBJECT (prop->element), key, &prop->value);
    }
}

static void
element_added_cb (FsElementAddedNotifier *self,
    GstBin *bin,
    GstElement *element,
    gpointer user_data)
{
  SkypeDynamicProperties *props = user_data;
  GHashTableIter iter;
  gpointer key, value;

  if (element == NULL)
    return;

  g_hash_table_iter_init (&iter, props->props);
  while (g_hash_table_iter_next (&iter, &key, &value))
    {
      SkypeDynamicProperty *prop = value;

      if (g_object_class_find_property (
          G_OBJECT_GET_CLASS (element), key) == NULL)
        continue;

      g_mutex_lock (prop->mutex);

      if (prop->element != NULL)
        gst_object_unref (prop->element);

      prop->element = GST_ELEMENT (gst_object_ref (element));

      /* If skype_dynamic_properties_set was called for this property,
       * set the value on this new element */
      if (prop->value_is_set)
          set_element_property (prop, key);

      g_debug ("Found element with property: %s", (gchar *)key);

      g_mutex_unlock (prop->mutex);
    }
}

SkypeDynamicProperties *
skype_dynamic_properties_new (FsElementAddedNotifier *notifier, ...)
{
  SkypeDynamicProperties *props;
  va_list args;
  const gchar *val;

  props = (SkypeDynamicProperties *)g_slice_new0 (SkypeDynamicProperties);
  props->props = g_hash_table_new_full (g_str_hash, g_str_equal,
      g_free, skype_dynamic_property_free);
  props->notifier = g_object_ref (notifier);

  va_start (args, notifier);

  while ((val = va_arg (args, const gchar *)) != NULL)
    {
      SkypeDynamicProperty *prop = skype_dynamic_property_new ();

      g_debug ("Adding property: %s", val);
      g_value_init (&prop->value, va_arg (args, GType));

      g_hash_table_insert (props->props, g_strdup (val), prop);
    }

  va_end (args);

  props->element_added_id = g_signal_connect (props->notifier,
      "element-added", G_CALLBACK (element_added_cb), props);

  return props;
}

gboolean
skype_dynamic_properties_get (SkypeDynamicProperties *props,
    const gchar *property_name, gpointer value)
{
  SkypeDynamicProperty *prop = g_hash_table_lookup (props->props, property_name);
  const gchar *element_name;

  if (prop == NULL)
    return FALSE;

  g_mutex_lock (prop->mutex);

  if (prop->element == NULL)
    {
      g_mutex_unlock (prop->mutex);
      return FALSE;
    }

  g_object_get (prop->element, property_name, value, NULL);

  element_name = gst_element_factory_get_longname(
      gst_element_get_factory(prop->element));

  if (!g_strcmp0 (property_name, "bitrate") &&
      !g_strcmp0 (element_name, "x264enc"))
    {
      /* x264enc has a bitrate property in kbit/s, we want the bitrate
       * property to be in byte/s */
      guint *bitrate = (guint *)value;
      g_debug ("Modifying bitrate return value from %u to %u",
          *bitrate, *bitrate * 128);
      *bitrate *= 128;
    }

  g_mutex_unlock (prop->mutex);

  return TRUE;
}


gboolean
skype_dynamic_properties_set (SkypeDynamicProperties *props,
    const gchar *property_name, ...)
{
  SkypeDynamicProperty *prop = g_hash_table_lookup (props->props, property_name);
  va_list args;
  gchar *e;

  if (prop == NULL)
      return FALSE;

  g_mutex_lock (prop->mutex);

  /* Keep track of value set in case a new element is found */
  va_start (args, property_name);
  G_VALUE_COLLECT (&prop->value, args, 0, &e);
  va_end (args);

  if (e != NULL)
    {
      g_warning ("Error collecting dynamic property value: %s", e);
      g_mutex_unlock (prop->mutex);
      g_free (e);
      return FALSE;
    }

  prop->value_is_set = TRUE;

  if (prop->element == NULL)
    {
      g_mutex_unlock (prop->mutex);
      return FALSE;
    }

  set_element_property (prop, property_name);

  g_mutex_unlock (prop->mutex);

  return TRUE;
}

void
skype_dynamic_properties_free (SkypeDynamicProperties *props)
{
  g_signal_handler_disconnect (props->notifier, props->element_added_id);
  g_object_unref (props->notifier);
  g_hash_table_unref (props->props);
  g_slice_free (SkypeDynamicProperties, (gpointer)props);
}

GList *
skype_get_supported_codecs_list (FsMediaType media_type,
    const gchar *preferences_path)
{
  GError *e = NULL;
  GList *codecs = NULL;
  GstElement *pipeline = NULL;
  FsConference *conf = NULL;
  FsSession *session = NULL;

  pipeline = gst_pipeline_new ("codecspipeline");

  if (pipeline == NULL)
    {
      g_warning ("Error creating codec discovery pipeline");
      goto out;
    }

  conf = FS_CONFERENCE (gst_element_factory_make (
      "fsrtpconference", "codecsconf"));

  if (conf == NULL)
    {
      g_warning ("Error creating codec discovery conference");
      goto out;
    }

  if (gst_bin_add (GST_BIN (pipeline), GST_ELEMENT (conf)) == FALSE)
    {
      g_warning ("Error adding codec discovery conference to pipeline");
      goto out;
    }

  if (gst_element_set_state (pipeline, GST_STATE_PLAYING) ==
      GST_STATE_CHANGE_FAILURE)
    {
      g_warning ("Error setting codec discovery pipeline to playing");
      goto out;
    }

  session = fs_conference_new_session (conf, media_type, &e);

  if (session == NULL)
    {
      g_warning ("Error creating codec discovery session");
      goto out;
    }

  if (preferences_path != NULL)
    {
      codecs = fs_codec_list_from_keyfile (preferences_path, &e);

      if (codecs == NULL)
        {
          g_warning ("Couldn't find file %s: %s", preferences_path, e->message);
          g_error_free (e);
        }
      else
        {
          if (!fs_session_set_codec_preferences (session, codecs, &e))
            {
              g_warning ("Error setting codec preferences: %s", e->message);
              g_error_free (e);
            }

          fs_codec_list_destroy (codecs);
          codecs = NULL;
        }
    }

  g_object_get (session, "codecs", &codecs, NULL);

  if (codecs == NULL)
    {
      g_warning ("No codecs found in codec discovery");
    }

out:
  g_object_unref (session);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  if (conf != NULL && GST_ELEMENT_PARENT (conf) == NULL)
    gst_object_unref (conf);
  gst_object_unref (pipeline);

  return codecs;
}

static gchar *
skype_get_path (gchar *default_filename, const gchar *envvar)
{
  gchar *dir;
  gchar *path;

  dir = g_strdup (g_getenv (envvar));

  if (dir == NULL)
    dir = g_get_current_dir ();

  path = g_build_filename (dir, default_filename, NULL);
  g_free (dir);

  return path;
}

gchar *
skype_get_codec_preferences_path (void)
{
  return skype_get_path ("codec-preferences",
      "MEDIA_ENGINE_PREFERENCE_DIR");
}

gchar *
skype_get_element_preferences_path (void)
{
  return skype_get_path ("element-preferences",
      "MEDIA_ENGINE_PREFERENCE_DIR");
}
