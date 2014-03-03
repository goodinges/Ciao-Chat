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

#ifndef __SKYPE_VIDEO_MANAGER_HPP__
#define __SKYPE_VIDEO_MANAGER_HPP__

#include "SidVideoRTPInterface.hpp"

#include <gst/farsight/fs-element-added-notifier.h>

#include "skype-video-stream.h"

extern void received_data_callback (SkypeBaseStream *stream,
    gsize len, const char *buf, gpointer data);

class SkypeVideoManager;

typedef void (*SkypeVideoManagerNewStreamCallback)
    (SkypeVideoManager *, SkypeVideoStream *, gpointer);

class SkypeVideoManager : public SkypeVideoRTPInterface
{
public:
  SkypeVideoManager (SkypeVideoRTPInterfaceCb *callback, GMainContext *context);
  ~SkypeVideoManager ();

	int GetLastError();
  
	bool Init();
  
	bool Uninit();
  
	bool GetCodecCount(int &codec_count);
  
	bool GetCodec(int index, VideoCodec & codec);
  
	bool GetSenderResolutionCount(int &res_cap_count);

	bool GetSenderResolution(int index, FixedRes & resolution);

	bool GetReceiverResolutionCount(int &resolution_count);

	bool GetReceiverResolution(int index, ResCap & resolution);

	bool GetIntProperty(int prop_id, int index, int &value);
  
	bool SetIntProperty(int prop_id, int index, int value);
  
	bool GetStringProperty(int prop_id, int index, Sid::String & value);
  
	bool SetStringProperty(int prop_id, int index, const Sid::String &value);
  
	bool GetBinaryProperty(int prop_id, int index, Sid::Binary& value);
  
	bool SetBinaryProperty(int prop_id, int index, const Sid::Binary &value);
  
	bool RequestKeyFrame();
  
	bool RequestRecoveryFrame(int reserved);
  
	bool SetBitrate(int bitrate);
  
	bool SetFramerate(int framerate_fps);
  
	bool SetMaxPacketSize(int max_packet_size);
  
	bool StartPlayback(int decoderfourcc, int width, int height);
  
	void RtpPacketReceived(const Sid::Binary &data);

	void FrameReceived(const Sid::Binary &data, int timestamp_ms);

	bool StopPlayback();

	bool StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps);
  
	bool StopRecording();

	bool CustomCommand(const Sid::String &cmd, Sid::String & result_str);

	bool VideoCallStart() { return true; };

	bool VideoCallEnd() { return true; };

  void SetNewStreamCallback (SkypeVideoManagerNewStreamCallback callback,
      gpointer user_data);

  void SetSourceName (const gchar *source_name);
  void SetSourceDevice (const gchar *source_device);
  void SetSourcePipeline (const gchar *source_pipeline);
  void SetSinkName (const gchar *sink_name);
  void SetSinkDevice (const gchar *sink_device);
  void SetSinkPipeline (const gchar *sink_pipeline);

private:
  SkypeVideoStream *m_stream;
  FsElementAddedNotifier *m_notifier;
  GList *m_codecs;

  SkypeVideoRTPInterfaceCb *m_videortpif_cb;
  GMainContext *m_context;

  gchar *m_source_name;
  gchar *m_source_device;
  gchar *m_source_pipeline;
  gchar *m_sink_name;
  gchar *m_sink_device;
  gchar *m_sink_pipeline;

  SkypeVideoManagerNewStreamCallback m_new_stream_cb;
  gpointer m_new_stream_user_data;

  friend void received_data_callback (SkypeBaseStream *stream, gsize len,
      const gchar *buf, gpointer data);
};

#endif /* __SKYPE_VIDEO_MANAGER_HPP__ */
