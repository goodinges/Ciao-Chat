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

#include <gst/gst.h>
#include <gst/farsight/fs-codec.h>
#include <gst/farsight/fs-element-added-notifier.h>

G_BEGIN_DECLS

typedef struct _SkypeDynamicProperties SkypeDynamicProperties;

SkypeDynamicProperties *skype_dynamic_properties_new (
    FsElementAddedNotifier *notifier, ...);

gboolean skype_dynamic_properties_get (SkypeDynamicProperties *props,
    const gchar *property_name, gpointer value);

gboolean skype_dynamic_properties_set (SkypeDynamicProperties *props,
    const gchar *property_name, ...);

void skype_dynamic_properties_free (SkypeDynamicProperties *props);

GList *skype_get_supported_codecs_list (FsMediaType media_type,
    const gchar *preferences_path);

gchar *skype_get_codec_preferences_path (void);

gchar *skype_get_element_preferences_path (void);

G_END_DECLS
