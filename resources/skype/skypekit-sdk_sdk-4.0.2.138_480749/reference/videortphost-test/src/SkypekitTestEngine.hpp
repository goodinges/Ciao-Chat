
#ifndef __SkypektiTestEngine_hpp__
#define __SkypektiTestEngine_hpp__

#include "SidVideoRTPInterface.hpp"

#include "Configuration.hpp"
#include "VideoInterfaces.hpp"
#include "H264AnxbToRtp.hpp"

#include "BitrateStat.hpp"

class SendVideoWrapper;
class ReceiveVideoWrapper;

struct SendPacketCallback {
	virtual ~SendPacketCallback() {};
	virtual void TransportSendRTPPacket(const void *data, int len) = 0;
	virtual void TransportSendFrame(VideoData &frame) = 0;
	virtual SkypeVideoRTPInterface::PayloadFmtType GetPayloadFmt() = 0;
};


struct SkypekitTestEngine : public SkypeVideoRTPInterface, public SendPacketCallback {
public:
	SkypekitTestEngine(SkypeVideoRTPInterfaceCb *callback);
	~SkypekitTestEngine();

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

	bool VideoCallStart();

	bool VideoCallEnd();

private:
	void TransportSendRTPPacket(const void *data, int len);

	void TransportSendFrame(VideoData &frame);

	SkypeVideoRTPInterface::PayloadFmtType GetPayloadFmt();

	uint32_t m_BitRate;

	SkypeVideoRTPInterfaceCb *m_pCb;
	BitrateStat m_ReceiveBStat;
	BitrateStat m_SendBStat;
	bool m_KeyFrameRequested;
	bool m_RecordingStarted;
	int m_ReceivePcnt;
	int m_SendPcnt;
	int m_ReceiveFcnt;
	int m_SendFcnt;
	PayloadFmtType m_PayloadFormat;
	SendVideoWrapper *m_pSendWrapper;
	ReceiveVideoWrapper *m_pReceiveWrapper;	
};

extern "C" SkypeVideoRTPInterface * SkypeVideoRTPInterfaceGet(SkypeVideoRTPInterfaceCb *callback);

extern "C" void SkypeVideoRTPInterfaceRelease(SkypeVideoRTPInterface* rtpvideo);

#endif /* !__SkypeRTPInterface_hpp__ */
