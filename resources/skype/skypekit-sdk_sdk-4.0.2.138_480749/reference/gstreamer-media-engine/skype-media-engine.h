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

#ifndef __SKYPE_MEDIA_ENGINE_H__
#define __SKYPE_MEDIA_ENGINE_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define SKYPE_TYPE_MEDIA_ENGINE skype_media_engine_get_type()

#define SKYPE_MEDIA_ENGINE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  SKYPE_TYPE_MEDIA_ENGINE, SkypeMediaEngine))

#define SKYPE_MEDIA_ENGINE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  SKYPE_TYPE_MEDIA_ENGINE, SkypeMediaEngineClass))

#define SKYPE_IS_MEDIA_ENGINE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  SKYPE_TYPE_MEDIA_ENGINE))

#define SKYPE_IS_MEDIA_ENGINE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  SKYPE_TYPE_MEDIA_ENGINE))

#define SKYPE_MEDIA_ENGINE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  SKYPE_TYPE_MEDIA_ENGINE, SkypeMediaEngineClass))

/**
 * SkypeMediaType:
 * @SKYPE_MEDIA_AUDIO: An audio media type
 * @SKYPE_MEDIA_VIDEO: A video media type
 * @SKYPE_MEDIA_BOTH: An audio and video media type
 *
 * Enum values to choose which media type the engine started with
 * skype_media_engine_start() or skype_media_engine_start_async() will provide.
 */
typedef enum
{
  SKYPE_MEDIA_AUDIO = 1,
  SKYPE_MEDIA_VIDEO = 1 << 1,
  SKYPE_MEDIA_BOTH = SKYPE_MEDIA_AUDIO | SKYPE_MEDIA_VIDEO,
} SkypeMediaType;

typedef struct _SkypeMediaEnginePrivate SkypeMediaEnginePrivate;

typedef struct {
  GObject parent;
  SkypeMediaEnginePrivate *priv;
} SkypeMediaEngine;

typedef struct {
  GObjectClass parent_class;
} SkypeMediaEngineClass;

/**
 * DeviceEnumerationType:
 * @DEVICE_ENUMERATION_INVALID: An invalid device
 * @DEVICE_ENUMERATION_AUDIO_SOURCE: An audio source
 * @DEVICE_ENUMERATION_AUDIO_SINK: An audio sink
 * @DEVICE_ENUMERATION_VIDEO_SOURCE: A video source
 * @DEVICE_ENUMERATION_VIDEO_SINK: A video sink
 *
 * Devices type of the enumerated element in the SkypeDeviceEnumeration
 * returned by skype_media_engine_enumerate_devices().
 */
typedef enum {
  DEVICE_ENUMERATION_INVALID,
  DEVICE_ENUMERATION_AUDIO_SOURCE,
  DEVICE_ENUMERATION_AUDIO_SINK,
  DEVICE_ENUMERATION_VIDEO_SOURCE,
  DEVICE_ENUMERATION_VIDEO_SINK,
} DeviceEnumerationType;

/**
 * SkypeDeviceEnumeration:
 * @type: the #DeviceEnumerationType of the device enumeration
 * @element: the name of the element this enumeration represents. This is the value to set to #SkypeMediaEngine:audio-source-name, #SkypeMediaEngine:audio-sink-name, #SkypeMediaEngine:video-source-name, and #SkypeMediaEngine:video-sink-name properties
 * @name: the friendly name of the element, suitable for displaying in a UI.
 * @description: a longer description of the element, suitable for
 * displaying an a UI.
 * @devices: a #GList of #gchar *s representing the devices available
 * through this element.
 *
 * A structure representing a group of devices available
 * through a given element.
 *
 * See Also: skype_media_engine_enumerate_devices()
 */
typedef struct {
  DeviceEnumerationType type;
  gchar *element;
  gchar *name;
  gchar *description;
  GList *devices;
} SkypeDeviceEnumeration;

GType skype_media_engine_get_type (void);

SkypeMediaEngine *skype_media_engine_new (void);

gboolean skype_media_engine_start (SkypeMediaEngine *engine,
    SkypeMediaType type, const gchar *ipc_path_prefix);

gboolean skype_media_engine_start_async (SkypeMediaEngine *engine,
    SkypeMediaType type, const gchar *ipc_path_prefix, GMainContext *context);

void skype_media_engine_stop (SkypeMediaEngine *engine,
    const gchar *ipc_path_prefix);

GList *skype_media_engine_enumerate_devices (gboolean full);

void skype_media_engine_free_devices (GList *devices);

G_END_DECLS

#endif /* __SKYPE_MEDIA_ENGINE_H__ */
