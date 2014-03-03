/*
* Copyright (C) 2007-2009 Skype Technologies S.A. Confidential and proprietary
*
* All intellectual property rights, including but not limited to copyrights,
* trademarks and patents, as well as know how and trade secrets contained
* in, relating to, or arising from the internet telephony software of Skype
* Limited (including its affiliates, "Skype"), including without limitation
* this source code, Skype API and related material of such software
* proprietary to Skype and/or its licensors ("IP Rights") are and shall
* remain the exclusive property of Skype and/or its licensors. The recipient
* hereby acknowledges and agrees that any unauthorized use of the IP Rights
* is a violation of intellectual property laws.
*
* Skype reserves all rights and may take legal action against infringers of
* IP Rights.
*
* The recipient agrees not to remove, obscure, make illegible or alter any
* notices or indications of the IP Rights and/or Skype's rights and ownership
* thereof.
*/
#include "SidPlatform.hpp"

#ifndef __SkypeVideoRTPInterface_hpp__
#define __SkypeVideoRTPInterface_hpp__

#ifndef MAKEFOURCC
#if defined(__IPP3__) || defined(__ARMEB__) || defined(__MIPSEB__)
	#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
		        ((unsigned int)(unsigned char)(ch3) | ((unsigned int)(unsigned char)(ch2) << 8) |   \
		        ((unsigned int)(unsigned char)(ch1) << 16) | ((unsigned int)(unsigned char)(ch0) << 24 ))
#else
	#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
		        ((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) |   \
		        ((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24 ))
#endif
#endif                  
                  
/** 
PVE (Platform Video Engine) implements the interface described by SkypeVideoRTPInterface.
Skypekit then calls methods described in this interface to control PVE */
struct SkypeVideoRTPInterface
{
	/** Returns the interface version. This helps skypekit to identify what version of SID sources are used when compiling videohost. This is already implemented in header file for videhost. */
	virtual bool  GetInterfaceVersion(int &version)
	#ifndef SID_SKYPEKIT_SIDE
	{ version = 32; return true; }
	#else
	=0;
	#endif
	/** 
	All functions can return false when error occurs. This function
	is used to retrieve the error code in such a case. This error code
	is used for debugging and feedback purposes.
	Return:
		error code for the last operation */
	virtual int  GetLastError() = 0;
	
	/** 
	Initialize the VideoEngine. This is called when SkypeKit is started.
	If the VideoEngine being initialized means consuming significant
	ressources (CPU or memory), it would be better to delay the init
	until StartPlayback() or StartRecording() is called, and shutdown
	after StopPlayback() or StopRecording() (deferred init). */
	virtual bool  Init() = 0;
	
	/** 
	Shutdown the VideoEngine. This is called when SkypeKit is stopped. */
	virtual bool  Uninit() = 0;
	
	enum PayloadFmtType
	{
		FORMAT_RTP=0,
		FORMAT_FRAMES=1,
	};
	
	/** 
	VideoCodec structure describes video codec supported by PVE.
	Fields:
		fourcc - codec identificator. Use MAKEFOURCC macro to get this.
			Currently we support only one codec: MAKEFOURCC('H', '2', '6', '4')
		recording_capable - whether the codec can only do playback or it can do recording also.
		payload_format - Payload format - either RTP or encoded frames coming from encoder/going to decoder(Annex-B format in case of H264). */
	struct VideoCodec
	{
		int fourcc;
		int recording_capable;
		PayloadFmtType payload_format;
	};
	
	/** Get number of supported codecs */
	virtual bool  GetCodecCount(int &codec_count) = 0;
	
	/** 
	Get index-th codec that is supported by PVE.
	Parameters:
		index - index of the structure, index starts from 0 and may not exceed the value codec_count - 1
		where codec_count is acquired with GetCodecCount function
		codec - an empty VideoCodec structure that will be filled by PVE */
	virtual bool  GetCodec(int index, VideoCodec & codec) = 0;
	
	/** 
	To indicate rendering capabilities receiver can define either a resolution ranges or
	fixed resolutions. Sender can only define number of fixed resolutions. Both sender and
	receiver have to support at least 160x120 and are very much recommended to support
	320x240 and 640x480.
	
	Preference indicates how much some resolution is preferred compared to others in given
	list. Skypekit constructs a resolution list from resolutions supported by both sender and
	receiver. The resolution list has preference value calculated in following way:
	receiver_preference*100+sender_preference. So the receiver preference counts more than
	senders's one. But if receiver preference is same for both resolutions, then sender's
	preference counts when doing final descision. For example when receiver defined resolution
	range (160x120...640x480, pref 1) and sender defined resolutions (160x120, pref 1)
	(172x144, pref 2), (320x240, pref 3), (352x288, pref 4) and (704x576, pref 5)
	then the final resolutions list will be (160x120, pref 101), (172x144, pref 102),
	(320x240, pref 103) (352x288, pref 104)
	1) picking startup resolution:
		Skypekit selects a resolution from list which has highest preference value and which
		has imageHeight smaller or equal to 480.
	2) switching resolutions:
		Skypekit divides resolution list into 5 resolution ranges based on image height:
		* range 1: height = 0...120
		* range 2: height = 121...240
		* range 3: height = 241...480
		* range 4: height = 481...960
		* range 5: height > 960
		In case of resolution switch it will switch to lower or higher range and select a
		resolution from that range which has highest preference value.
	
	Fields:
		width - picture width
		height - picture height
		preference - preference value for current resolution compared to other resolutions in the list.
			May be in range 0...99 */
	struct FixedRes
	{
		int width;
		int height;
		int preference;
	};
	
	/** 
	Receiver can define either a resolution ranges or fixed resolutions. In case of fixed resolution
	min_width==max_width and min_height==max_height in this structure
	
	Fields:
		max_width - maximum picture width
		max_height - maximum picture height
		min_width - minimum picture width
		min_height - minimum picture height
		preference - preference value for current resolution compared to other resolutions in the list.
			May be in range 0...99 */
	struct ResCap
	{
		int max_width;
		int max_height;
		int min_width;
		int min_height;
		int preference;
	};
	
	/** Get number of FixefRes structures describing sender resolutions */
	virtual bool  GetSenderResolutionCount(int &resolution_count) = 0;
	
	/** Get index-th FixefRes structure describing sender */
	virtual bool  GetSenderResolution(int index, FixedRes & resolution) = 0;
	
	/** 
	Get number of ResCap structures describing receiver. */
	virtual bool  GetReceiverResolutionCount(int &resolution_count) = 0;
	
	/** Get index-th ResCap structure describing receiver */
	virtual bool  GetReceiverResolution(int index, ResCap & resolution) = 0;
	
	/** 
	Property ID-s used with Get*Property & Set*Property methods:
	GET_NUMBER_OF_H264_SENDER_PARAMETERS - used with GetIntProperty to read number of sender parameter structures
	GET_H264_SENDER_PARAMETERS - used with GetBinaryProperty to read a sender parameter structure
	GET_NUMBER_OF_H264_RECEIVER_PARAMETERS - used with GetIntProperty to read number of receiver parameter structures
	GET_H264_RECEIVER_PARAMETERS - used with GetBinaryProperty to read a receiver parameter structure
	SET_H264_SENDER_CONSTRAINTS - used with SetBinaryProperty to set sender constraints */
	enum PropId
	{
		GET_NUMBER_OF_H264_SENDER_PARAMETERS=0,
		GET_H264_SENDER_PARAMETERS=1,
		GET_NUMBER_OF_H264_RECEIVER_PARAMETERS=2,
		GET_H264_RECEIVER_PARAMETERS=3,
		SET_H264_SENDER_CONSTRAINTS=4,
	};
	
	virtual bool  GetIntProperty(int prop_id, int index, int &value) = 0;
	
	virtual bool  SetIntProperty(int prop_id, int index, int value) = 0;
	
	virtual bool  GetStringProperty(int prop_id, int index, Sid::String & value) = 0;
	
	virtual bool  SetStringProperty(int prop_id, int index, const Sid::String &value) = 0;
	
	enum H264ProfileIop
	{
		PROFILE_IOP_CONSTRAINTS_SET0=128,
		PROFILE_IOP_CONSTRAINTS_SET1=64,
		PROFILE_IOP_CONSTRAINTS_SET2=32,
		PROFILE_IOP_CONSTRAINTS_SET3=16,
	};
	
	/** 
	This structure describes the capabilities of H264 sender. Sender may support many
	different H264 profile & profile_iop combinations. For each supported combination
	it has to report one H264SenderParameters structure. It has to support at least
	constrained baseline (profile_idc=66, profile_iop=0xe0)
	
	Fields:
		profile_idc - profile_idc from H264 SPS
		profile_iop - constriants_set flags from H264 SPS
		level_idc - maximum level_idc from H264 SPS */
	struct H264SenderParameters
	{
		int profile_idc;
		H264ProfileIop profile_iop;
		int level_idc;
	};
	
	/** 
	This structure describes the capabilities of H264 receiver. Receiver may support many
	different H264 profile & profile_iop combinations. For each supported combination
	it has to report one H264ReceiverParameters structure. It has to support at least
	constrained baseline (profile_idc=66, profile_iop=0xe0)
	
	Fields:
	profile_idc - profile_idc from H264 SPS
	profile_iop - constriants_set flags from H264 SPS
	level_idc - max level_idc from H264 SPS
	max_mbps - maximum decoder's macroblock processing rate in macroblocks per
			second. May only specify greater values than inferred from level_idc. 0 if not specified
	max_fs - maximum decoder's frame size in macroblocks.
			May only specify greater values than inferred from level_idc. 0 if not specified
	max_cpb - maximum decoder's coded picture buffer size in units of 1000 bits.
			May only specify greater values than inferred from by level_idc. 0 if not specifie
	max_dpb - maximum decoder's decoded picture buffer size in units of 1024 bytes.
			May only specify greater values than inferred from by level_idc. 0 if not specified
	max_br - maximum decoder's supported video bit rate in units of 1000 bits per second.
			As an exception can specify smaller values than inferred from level_idc. 0 if not specified
	max_smbps - maximum number of static macroblocks per second. 0 if not specified
			has to be larger than max_mbps, if specified.
	     (check for more info: http://tools.ietf.org/id/draft-ietf-avt-rtp-rfc3984bis-08.txt)
	redundant_pic_cap - Specifies if receiver is capable of using redundant coded
	     pictures to correct incorrectly decoded primary coded pictures. 0 if not capable, 1 if capable. */
	struct H264ReceiverParameters
	{
		int profile_idc;
		H264ProfileIop profile_iop;
		int level_idc;
		int max_mbps;
		int max_fs;
		int max_cpb;
		int max_dpb;
		int max_br;
		int max_smbps;
		int redundant_pic_cap;
	};
	
	/** 
	This structure describes the constraints for H264 sender - it has to use profile
	specified by profile_idc and profile_iop and should not exceed any other parameters, (if given)
	
	Fields:
	profile_idc - profile_idc from H264 SPS
	profile_iop - constriants_set flags from H264 SPS
	level_idc - max level_idc from H264 SPS
	max_mbps - maximum decoder's macroblock processing rate in macroblocks per
			second. May only specify greater values than inferred from level_idc. 0 if not specified
	max_fs - maximum decoder's frame size in macroblocks.
			May only specify greater values than inferred from level_idc. 0 if not specified
	max_cpb - maximum decoder's coded picture buffer size in units of 1000 bits.
			May only specify greater values than inferred from by level_idc. 0 if not specifie
	max_dpb - maximum decoder's decoded picture buffer size in units of 1024 bytes.
			May only specify greater values than inferred from by level_idc. 0 if not specified
	max_br - maximum decoder's supported video bit rate in units of 1000 bits per second.
			As an exception can specify smaller values than inferred from level_idc. 0 if not specified
	max_smbps - maximum number of static macroblocks per second. 0 if not specified
			has to be larger than max_mbps, if specified.
	     (check for more info: http://tools.ietf.org/id/draft-ietf-avt-rtp-rfc3984bis-08.txt)
	redundant_pic_cap - Specifies if receiver is capable of using redundant coded
	     pictures to correct incorrectly decoded primary coded pictures. 0 if not capable, 1 if capable. */
	struct H264SenderConstraints
	{
		int profile_idc;
		H264ProfileIop profile_iop;
		int level_idc;
		int max_mbps;
		int max_fs;
		int max_cpb;
		int max_dpb;
		int max_br;
		int max_smbps;
		int redundant_pic_cap;
	};
	
	virtual bool  GetBinaryProperty(int prop_id, int index, Sid::Binary& value) = 0;
	
	virtual bool  SetBinaryProperty(int prop_id, int index, const Sid::Binary &value) = 0;
	
	/** 
	Requests that the codec generate an I-frame.
	
	The SkypeKit Runtime ensures that no incomplete frames are delivered to the remote side decoder, and
	will request a new keyframe (by invoking this method) to recover from any frame loss. Your VideoHost
	should not send periodic keyframes for the purpose of error resilience, as this is fully handled by
	SkypeKit. So, for optimal video performance, your VideoHost should send keyframes only:
	
	    * after invoking StartRecording for a particular video stream.
	    * when explicitly requested by the SkypeKit Runtime, as a result of invoking this RequestKeyFrame method.
	
	For FORMAT_RTP SPS, PPS and IDR have to be inside same frame - all packets containing SPS, PPS and IDR will
	have same timestamp and final packet has marker bit set. In case of FORMAT_FRAMES they just need to
	be inside single annex-B frame and given to skypekit with one SendFrame call. */
	virtual bool  RequestKeyFrame() = 0;
	
	/** Reserved for future use */
	virtual bool  RequestRecoveryFrame(int reserved) = 0;
	
	/** 
	Adjust recording bitrate.
	Parameters:
		bitrate_bytes_sek - bitrate in bytes per second. */
	virtual bool  SetBitrate(int bitrate_bytes_sek) = 0;
	
	/** 
	If the call is between desktop and SkypeKit, then this is called
	during the call according to the cpu usage on desktop. Videohost
	should not exeed the framerate specified with this function
	Parameters:
		framerate_fps - framerate in frames per second */
	virtual bool  SetFramerate(int framerate_fps) = 0;
	
	/** 
	Set the maximum allowed RTP packet size.
	Parameters:
		max_packet_size - packet size in bytes. */
	virtual bool  SetMaxPacketSize(int max_packet_size) = 0;
	
	/** 
	Start playback.
	Parameters:
		decoderfourcc - FOURCC specifying decoder
		width - video width in pixels
		height - video height in pixels */
	virtual bool  StartPlayback(int decoderfourcc, int width, int height) = 0;
	
	/** 
	Submit an incoming RTP packet for playback. Used if codec specified FORMAT_RTP as payload format
	Parameters:
		data - buffer containing RTP packet
		len - buffer length */
	virtual void RtpPacketReceived(const Sid::Binary &data) = 0;
	
	/** 
	Submit an frame for playback. Used if codec specified FORMAT_FRAMES as payload format.
	Format of frame is codec specific. For H264 Annex-B is used.
	Parameters:
		data - buffer containing RTP packet
		len - buffer length */
	virtual void FrameReceived(const Sid::Binary &data, int timestamp_ms) = 0;
	
	/** Stop playback */
	virtual bool  StopPlayback() = 0;
	
	/** 
	Start recording video, encoding it and sending RTP packets
	to the given transport.
	Parameters:
		fourcc - defines the encoder
		initial_max_packet_size - max packet size to use. Later this may be modified by max_packet_size calls.
			Rtp packet size may not exceed this limit. Frames can exceed this and skypekit will then break
			them into smaller packets internally if needed.
		initial_bitrate_bytes_sec - initial bitrate for video call. Later the bitrate is modified with SetBitrate calls
		width - recording width
		height - recording height */
	virtual bool  StartRecording(int fourcc, int initial_max_packet_size, int initial_bitrate_bytes_sec, int width, int height, int initial_framerate_fps) = 0;
	
	/** Stop Recording */
	virtual bool  StopRecording() = 0;
	
	/** 
	Send custom string command to PVE from UI, and get an answer back.
	Can be used for example when UI wants to adjust camera specific settings like brightness or zoom */
	virtual bool  CustomCommand(const Sid::String &command, Sid::String & result_str) = 0;
	
	/** 
	Called once during a call before StartPlayback or StartRecording is called. Purpose of this function is to provide 
	initialization point for resources which need to be initialized once at the start of the videocall.  */
	virtual bool  VideoCallStart() = 0;
	
	/** 
	Called at the end of the call after both StopPlayback and StopRecording are called. Purpose of this function is to provide 
	uninitialization point for resources which were initialized at the start of the videocall.  */
	virtual bool  VideoCallEnd() = 0;
	
};

/** 
This interface is implemented by Skypekit and passed to PVE factory method SkypeVideoRTPInterfaceGet
PVE uses this to invoke methods on skypekit side */
struct SkypeVideoRTPInterfaceCb
{
	virtual void SendRTPPacket(const Sid::Binary &data) = 0;
	
	virtual void SendFrame(const Sid::Binary &data, int timestamp_ms) = 0;
	
};


/*!
 * Skypekit calls this method to create an instance of PVE. Skypekit 
 * passes a callback to this method to enable PVE to call methods on skypekit side  
 */
extern "C" SkypeVideoRTPInterface * SkypeVideoRTPInterfaceGet(SkypeVideoRTPInterfaceCb *callback);

extern "C" void SkypeVideoRTPInterfaceRelease(SkypeVideoRTPInterface* rtpvideo);

#endif /* !__SkypeRTPInterface_hpp__ */
