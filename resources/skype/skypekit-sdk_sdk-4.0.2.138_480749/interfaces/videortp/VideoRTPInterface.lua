package.path = package.path .. ';../../sid/?.lua'

require 'xsid'

-- Decorators
RETURN = "RETURN"

videortpif = SID "SkypeVideoRTPInterface" {
	comment = "\
PVE (Platform Video Engine) implements the interface described by SkypeVideoRTPInterface.\
Skypekit then calls methods described in this interface to control PVE",


	ACTION(1) "GetInterfaceVersion"
	  {comment = "Returns the interface version. This helps skypekit to identify what version of SID sources are used when compiling videohost. This is already implemented in header file for videhost."}
	- { 
		FIELD(2, RETURN) "rv" { type = "boolean" }, 
		FIELD(3) "version" { type = "int" }
	  };

	ACTION(2) "GetLastError"
	  {
		comment = "\
All functions can return false when error occurs. This function\
is used to retrieve the error code in such a case. This error code\
is used for debugging and feedback purposes.\
Return:\
	error code for the last operation"
	  }
	- { 
		FIELD(1, RETURN) "error" { type = "int" } 
	  };

	ACTION(3) "Init"
	  { 
		comment = "\
Initialize the VideoEngine. This is called when SkypeKit is started.\
If the VideoEngine being initialized means consuming significant\
ressources (CPU or memory), it would be better to delay the init\
until StartPlayback() or StartRecording() is called, and shutdown\
after StopPlayback() or StopRecording() (deferred init)."
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };
	
	ACTION(4) "Uninit"
	  {
		comment = "\
Shutdown the VideoEngine. This is called when SkypeKit is stopped."
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ENUMERATED "PayloadFmtType" {
		ENUM(0) "FORMAT_RTP" {},
		ENUM(1) "FORMAT_FRAMES" {}
	};

	STRUCT "VideoCodec"
	{
		comment = "\
VideoCodec structure describes video codec supported by PVE.\
Fields:\
	fourcc - codec identificator. Use MAKEFOURCC macro to get this.\
		Currently we support only one codec: MAKEFOURCC('H', '2', '6', '4')\
	recording_capable - whether the codec can only do playback or it can do recording also.\
	payload_format - Payload format - either RTP or encoded frames coming from encoder/going to decoder(Annex-B format in case of H264).",

		FIELD(1) "fourcc"			{ type = "int" },
		FIELD(2) "recording_capable" { type = "int" },
		FIELD(3) "payload_format"	{ type = "PayloadFmtType" }
	};

	ACTION(5) "GetCodecCount"
	  {
		comment = "Get number of supported codecs"
	  }
	- { 
		FIELD(1) "codec_count" { type = "int" } ,
		FIELD(2, RETURN) "rv" { type = "boolean" }
	  };

	ACTION(6) "GetCodec"
	  {
		comment = "\
Get index-th codec that is supported by PVE.\
Parameters:\
	index - index of the structure, index starts from 0 and may not exceed the value codec_count - 1\
	where codec_count is acquired with GetCodecCount function\
	codec - an empty VideoCodec structure that will be filled by PVE",
		FIELD(1) "index" { type = "int" } ,
	  }
	- { 
		FIELD(3, RETURN) "rv" { type = "boolean" } , 
		FIELD(4) "codec" { type = "VideoCodec" }
	  };

	STRUCT "FixedRes"
	{
		comment = "\
To indicate rendering capabilities receiver can define either a resolution ranges or\
fixed resolutions. Sender can only define number of fixed resolutions. Both sender and\
receiver have to support at least 160x120 and are very much recommended to support\
320x240 and 640x480.\
\
Preference indicates how much some resolution is preferred compared to others in given\
list. Skypekit constructs a resolution list from resolutions supported by both sender and\
receiver. The resolution list has preference value calculated in following way:\
receiver_preference*100+sender_preference. So the receiver preference counts more than\
senders's one. But if receiver preference is same for both resolutions, then sender's\
preference counts when doing final descision. For example when receiver defined resolution\
range (160x120...640x480, pref 1) and sender defined resolutions (160x120, pref 1)\
(172x144, pref 2), (320x240, pref 3), (352x288, pref 4) and (704x576, pref 5)\
then the final resolutions list will be (160x120, pref 101), (172x144, pref 102),\
(320x240, pref 103) (352x288, pref 104)\
1) picking startup resolution:\
	Skypekit selects a resolution from list which has highest preference value and which\
	has imageHeight smaller or equal to 480.\
2) switching resolutions:\
	Skypekit divides resolution list into 5 resolution ranges based on image height:\
	* range 1: height = 0...120\
	* range 2: height = 121...240\
	* range 3: height = 241...480\
	* range 4: height = 481...960\
	* range 5: height > 960\
	In case of resolution switch it will switch to lower or higher range and select a\
	resolution from that range which has highest preference value.\
\
Fields:\
	width - picture width\
	height - picture height\
	preference - preference value for current resolution compared to other resolutions in the list.\
		May be in range 0...99",

		FIELD(1) "width" { type = "int" },
		FIELD(2) "height" { type = "int" },
		FIELD(3) "preference" { type = "int" }
	};
	
	STRUCT "ResCap"
	{
		comment = "\
Receiver can define either a resolution ranges or fixed resolutions. In case of fixed resolution\
min_width==max_width and min_height==max_height in this structure\
\
Fields:\
	max_width - maximum picture width\
	max_height - maximum picture height\
	min_width - minimum picture width\
	min_height - minimum picture height\
	preference - preference value for current resolution compared to other resolutions in the list.\
		May be in range 0...99",

		FIELD(1) "max_width" { type = "int" },
		FIELD(2) "max_height" { type = "int" },
		FIELD(3) "min_width" { type = "int" },
		FIELD(4) "min_height" { type = "int" },
		FIELD(5) "preference" { type = "int" }
	};
	
	ACTION(30) "GetSenderResolutionCount"
	  {
		comment = "Get number of FixefRes structures describing sender resolutions"
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } , 
		FIELD(2) "resolution_count" { type = "int" }
	  };

	ACTION(31) "GetSenderResolution"
	  {
		comment = "Get index-th FixefRes structure describing sender",
		FIELD(1) "index" { type = "int" } ,
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } , 
		FIELD(2) "resolution" { type = "FixedRes" }
	  };

	ACTION(32) "GetReceiverResolutionCount"
	  {
		comment = "\
Get number of ResCap structures describing receiver."
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } , 
		FIELD(2) "resolution_count" { type = "int" }
	  };

	ACTION(33) "GetReceiverResolution"
	  {
		comment = "Get index-th ResCap structure describing receiver",
		FIELD(1) "index" { type = "int" } ,
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } , 
		FIELD(2) "resolution" { type = "ResCap" }
	  };
	
	ENUMERATED "PropId" {
		comment = "\
Property ID-s used with Get*Property & Set*Property methods:\
GET_NUMBER_OF_H264_SENDER_PARAMETERS - used with GetIntProperty to read number of sender parameter structures\
GET_H264_SENDER_PARAMETERS - used with GetBinaryProperty to read a sender parameter structure\
GET_NUMBER_OF_H264_RECEIVER_PARAMETERS - used with GetIntProperty to read number of receiver parameter structures\
GET_H264_RECEIVER_PARAMETERS - used with GetBinaryProperty to read a receiver parameter structure\
SET_H264_SENDER_CONSTRAINTS - used with SetBinaryProperty to set sender constraints",
		ENUM(0) "GET_NUMBER_OF_H264_SENDER_PARAMETERS" {},
		ENUM(1) "GET_H264_SENDER_PARAMETERS" {},
		ENUM(2) "GET_NUMBER_OF_H264_RECEIVER_PARAMETERS" {},
		ENUM(3) "GET_H264_RECEIVER_PARAMETERS" {},
		ENUM(4) "SET_H264_SENDER_CONSTRAINTS" {}
	};
	
	ACTION(7) "GetIntProperty"
	  { 
		FIELD(1) "prop_id" { type = "int" } ,
		FIELD(2) "index" { type = "int" }
	  }
	- { 
		FIELD(1) "value" { type = "int" } ,
		FIELD(2, RETURN) "rv" { type = "boolean" } 
	  };
	  
	ACTION(8) "SetIntProperty"
	  { 
		FIELD(1) "prop_id" { type = "int" } ,
		FIELD(2) "index" { type = "int" } ,
		FIELD(3) "value" { type = "int" }
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(9) "GetStringProperty"
	  { 
		FIELD(1) "prop_id" { type = "int" } ,
		FIELD(2) "index" { type = "int" }
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } , 
		FIELD(2) "value" { type = "string" }
	  };
	  
	ACTION(10) "SetStringProperty"
	  { 
		FIELD(1) "prop_id" { type = "int" } ,
		FIELD(2) "index" { type = "int" } ,
		FIELD(3) "value" { type = "string" } ,
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ENUMERATED "H264ProfileIop" {
		ENUM(128) "PROFILE_IOP_CONSTRAINTS_SET0" {},
		ENUM(64) "PROFILE_IOP_CONSTRAINTS_SET1" {},
		ENUM(32) "PROFILE_IOP_CONSTRAINTS_SET2" {},
		ENUM(16) "PROFILE_IOP_CONSTRAINTS_SET3" {}
	};	

	STRUCT "H264SenderParameters"
	{
		comment = "\
This structure describes the capabilities of H264 sender. Sender may support many\
different H264 profile & profile_iop combinations. For each supported combination\
it has to report one H264SenderParameters structure. It has to support at least\
constrained baseline (profile_idc=66, profile_iop=0xe0)\
\
Fields:\
	profile_idc - profile_idc from H264 SPS\
	profile_iop - constriants_set flags from H264 SPS\
	level_idc - maximum level_idc from H264 SPS",

		FIELD(1) "profile_idc" { type = "int" },
		FIELD(2) "profile_iop" { type = "H264ProfileIop" },
		FIELD(3) "level_idc" { type = "int" }
	};	

	STRUCT "H264ReceiverParameters"
	{
		comment = "\
This structure describes the capabilities of H264 receiver. Receiver may support many\
different H264 profile & profile_iop combinations. For each supported combination\
it has to report one H264ReceiverParameters structure. It has to support at least\
constrained baseline (profile_idc=66, profile_iop=0xe0)\
\
Fields:\
profile_idc - profile_idc from H264 SPS\
profile_iop - constriants_set flags from H264 SPS\
level_idc - max level_idc from H264 SPS\
max_mbps - maximum decoder's macroblock processing rate in macroblocks per\
		second. May only specify greater values than inferred from level_idc. 0 if not specified\
max_fs - maximum decoder's frame size in macroblocks.\
		May only specify greater values than inferred from level_idc. 0 if not specified\
max_cpb - maximum decoder's coded picture buffer size in units of 1000 bits.\
		May only specify greater values than inferred from by level_idc. 0 if not specifie\
max_dpb - maximum decoder's decoded picture buffer size in units of 1024 bytes.\
		May only specify greater values than inferred from by level_idc. 0 if not specified\
max_br - maximum decoder's supported video bit rate in units of 1000 bits per second.\
		As an exception can specify smaller values than inferred from level_idc. 0 if not specified\
max_smbps - maximum number of static macroblocks per second. 0 if not specified\
		has to be larger than max_mbps, if specified.\
     (check for more info: http://tools.ietf.org/id/draft-ietf-avt-rtp-rfc3984bis-08.txt)\
redundant_pic_cap - Specifies if receiver is capable of using redundant coded\
     pictures to correct incorrectly decoded primary coded pictures. 0 if not capable, 1 if capable.",

     FIELD(1) "profile_idc" { type = "int" },
     FIELD(2) "profile_iop" { type = "H264ProfileIop" },
     FIELD(3) "level_idc" { type = "int" },
     FIELD(4) "max_mbps" { type = "int" },
     FIELD(5) "max_fs" { type = "int" },
     FIELD(6) "max_cpb" { type = "int" },
     FIELD(7) "max_dpb" { type = "int" },
     FIELD(8) "max_br" { type = "int" },
     FIELD(9) "max_smbps" { type = "int" },
     FIELD(10) "redundant_pic_cap" { type = "int" }
	};
	
	STRUCT "H264SenderConstraints"
	{
		comment = "\
This structure describes the constraints for H264 sender - it has to use profile\
specified by profile_idc and profile_iop and should not exceed any other parameters, (if given)\
\
Fields:\
profile_idc - profile_idc from H264 SPS\
profile_iop - constriants_set flags from H264 SPS\
level_idc - max level_idc from H264 SPS\
max_mbps - maximum decoder's macroblock processing rate in macroblocks per\
		second. May only specify greater values than inferred from level_idc. 0 if not specified\
max_fs - maximum decoder's frame size in macroblocks.\
		May only specify greater values than inferred from level_idc. 0 if not specified\
max_cpb - maximum decoder's coded picture buffer size in units of 1000 bits.\
		May only specify greater values than inferred from by level_idc. 0 if not specifie\
max_dpb - maximum decoder's decoded picture buffer size in units of 1024 bytes.\
		May only specify greater values than inferred from by level_idc. 0 if not specified\
max_br - maximum decoder's supported video bit rate in units of 1000 bits per second.\
		As an exception can specify smaller values than inferred from level_idc. 0 if not specified\
max_smbps - maximum number of static macroblocks per second. 0 if not specified\
		has to be larger than max_mbps, if specified.\
     (check for more info: http://tools.ietf.org/id/draft-ietf-avt-rtp-rfc3984bis-08.txt)\
redundant_pic_cap - Specifies if receiver is capable of using redundant coded\
     pictures to correct incorrectly decoded primary coded pictures. 0 if not capable, 1 if capable.",

     FIELD(1) "profile_idc" { type = "int" },
     FIELD(2) "profile_iop" { type = "H264ProfileIop" },
     FIELD(3) "level_idc" { type = "int" },
     FIELD(4) "max_mbps" { type = "int" },
     FIELD(5) "max_fs" { type = "int" },
     FIELD(6) "max_cpb" { type = "int" },
     FIELD(7) "max_dpb" { type = "int" },
     FIELD(8) "max_br" { type = "int" },
     FIELD(9) "max_smbps" { type = "int" },
     FIELD(10) "redundant_pic_cap" { type = "int" }
	};
	ACTION(11) "GetBinaryProperty"
	  { 
		FIELD(1) "prop_id" { type = "int" } ,
		FIELD(2) "index" { type = "int" }
	  }
	- { 
		FIELD(1) "value" { type = "binary" } ,
		FIELD(2, RETURN) "rv" { type = "boolean" } 
	  };
	  
	ACTION(12) "SetBinaryProperty"
	  { 
		FIELD(1) "prop_id" { type = "int" } ,
		FIELD(2) "index" { type = "int" } , 
		FIELD(3) "value" { type = "binary" }
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(13) "RequestKeyFrame"
	  {
		comment = "\
Requests that the codec generate an I-frame.\
\
The SkypeKit Runtime ensures that no incomplete frames are delivered to the remote side decoder, and\
will request a new keyframe (by invoking this method) to recover from any frame loss. Your VideoHost\
should not send periodic keyframes for the purpose of error resilience, as this is fully handled by\
SkypeKit. So, for optimal video performance, your VideoHost should send keyframes only:\
\
    * after invoking StartRecording for a particular video stream.\
    * when explicitly requested by the SkypeKit Runtime, as a result of invoking this RequestKeyFrame method.\
\
For FORMAT_RTP SPS, PPS and IDR have to be inside same frame - all packets containing SPS, PPS and IDR will\
have same timestamp and final packet has marker bit set. In case of FORMAT_FRAMES they just need to\
be inside single annex-B frame and given to skypekit with one SendFrame call."
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(14) "RequestRecoveryFrame"
	  {
		comment = "Reserved for future use",

		FIELD(1) "reserved" { type = "int" }
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(15) "SetBitrate"
	  {
		comment = "\
Adjust recording bitrate.\
Parameters:\
	bitrate_bytes_sec - bitrate in bytes per second.",
 
		FIELD(1) "bitrate_bytes_sec" { type = "int" }
	  }
	- {
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(16) "SetFramerate"
	  {
		comment = "\
If the call is between desktop and SkypeKit, then this is called\
during the call according to the cpu usage on desktop. Videohost\
should not exeed the framerate specified with this function\
Parameters:\
	framerate_fps - framerate in frames per second",

		FIELD(1) "framerate_fps" { type = "int" }
	  }
	- {
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(17) "SetMaxPacketSize"
	  {
		comment = "\
Set the maximum allowed RTP packet size.\
Parameters:\
	max_packet_size - packet size in bytes.",

		FIELD(1) "max_packet_size" { type = "int" }
	  }
	- {
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(18) "StartPlayback"
	  { 
		comment = "\
Start playback.\
Parameters:\
	decoderfourcc - FOURCC specifying decoder\
	width - video width in pixels\
	height - video height in pixels",

		FIELD(1) "decoderfourcc" { type = "int" },
	    FIELD(2) "width" { type = "int" } ,
	    FIELD(3) "height" { type = "int" } 
	  }
	- {
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(19) "RtpPacketReceived"
	  { 
		comment = "\
Submit an incoming RTP packet for playback. Used if codec specified FORMAT_RTP as payload format\
Parameters:\
	data - buffer containing RTP packet\
	len - buffer length",

		FIELD(1) "data" { type = "binary" } 
	  };

	ACTION(34) "FrameReceived"
	  { 
		comment = "\
Submit an frame for playback. Used if codec specified FORMAT_FRAMES as payload format.\
Format of frame is codec specific. For H264 Annex-B is used.\
Parameters:\
	data - buffer containing RTP packet\
	len - buffer length",

		FIELD(1) "data" { type = "binary" } ,
		FIELD(2) "timestamp_ms" { type = "int" }
	  };

	ACTION(20) "StopPlayback"
	  {
		comment = "Stop playback"
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(21) "StartRecording"
	  { 
		comment = "\
Start recording video, encoding it and sending RTP packets\
to the given transport.\
Parameters:\
	fourcc - defines the encoder\
	initial_max_packet_size - max packet size to use. Later this may be modified by max_packet_size calls.\
		Rtp packet size may not exceed this limit. Frames can exceed this and skypekit will then break\
		them into smaller packets internally if needed.\
	initial_bitrate_bytes_sec - initial bitrate for video call. Later the bitrate is modified with SetBitrate calls\
	width - recording width\
	height - recording height",
	
		FIELD(1) "fourcc" { type = "int" },
		FIELD(2) "initial_max_packet_size" { type = "int" },
		FIELD(3) "initial_bitrate_bytes_sec" { type = "int" },
		FIELD(4) "width" { type = "int" },
		FIELD(5) "height" { type = "int" },
		FIELD(6) "initial_framerate_fps" { type = "int" },
	  }
	- { 
		FIELD(3, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(22) "StopRecording"
	  {
		comment = "Stop Recording"
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };

	ACTION(23) "CustomCommand"
	  {
		comment = "\
Send custom string command to PVE from UI, and get an answer back.\
Can be used for example when UI wants to adjust camera specific settings like brightness or zoom",

		FIELD(1) "command" { type = "string" }
	  }
	- { 
		FIELD(1) "result_str" { type = "string" } ,
		FIELD(2, RETURN) "rv" { type = "boolean" } 
	  };
	ACTION(24) "VideoCallStart"
	  {
		comment = "\
Called once during a call before StartPlayback or StartRecording is called. Purpose of this function is to provide \
initialization point for resources which need to be initialized once at the start of the videocall. ",
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };	  
	ACTION(25) "VideoCallEnd"
	  {
		comment = "\
Called at the end of the call after both StopPlayback and StopRecording are called. Purpose of this function is to provide \
uninitialization point for resources which were initialized at the start of the videocall. ",
	  }
	- { 
		FIELD(1, RETURN) "rv" { type = "boolean" } 
	  };	  	    
}

videortpif_cb = SID "SkypeVideoRTPInterfaceCb" {
	comment = "\
This interface is implemented by Skypekit and passed to PVE factory method SkypeVideoRTPInterfaceGet\
PVE uses this to invoke methods on skypekit side",

	ACTION(1) "SendRTPPacket" {
		FIELD(1) "data" { type = "binary" } 
	};
	ACTION(2) "SendFrame" {
		FIELD(1) "data" { type = "binary" } ,
		FIELD(2) "timestamp_ms" { type = "int" }
	}
}
