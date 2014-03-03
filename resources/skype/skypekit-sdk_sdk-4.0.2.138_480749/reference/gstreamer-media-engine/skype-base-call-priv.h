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

#ifndef __SKYPE_BASE_CALL_PRIV_H__
#define __SKYPE_BASE_CALL_PRIV_H__

#include "skype-base-call.h"
#include "skype-audio-call.h"
#include "skype-video-call.h"

#include "skype-base-stream.h"
#include "skype-audio-stream.h"
#include "skype-video-stream.h"

G_BEGIN_DECLS

void skype_base_call_set_stream (SkypeBaseCall *self, SkypeBaseStream *stream);

SkypeAudioCall *skype_audio_call_new (const gchar *ipc_path_prefix,
    SkypeAudioStream *stream);

SkypeVideoCall *skype_video_call_new (const gchar *ipc_path_prefix,
    SkypeVideoStream *stream);

G_END_DECLS

#endif /* __SKYPE_BASE_CALL_PRIV_H__ */
