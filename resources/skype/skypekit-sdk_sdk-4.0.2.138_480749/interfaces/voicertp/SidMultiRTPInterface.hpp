/*
 * Copyright (C) 2009 Skype Technologies S.A. Confidential and proprietary
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

#ifndef __SidMultiRTPInterface_hpp__
#define __SidMultiRTPInterface_hpp__

#include "SidPlatform.hpp"

/* Interface Changelog:
 * ver 1: Original version
 * ver 2: Added virtual destructors
 * ver 3: ChangeConversationId added, to reconfigure channels in conference
 */
#ifndef SKYPE_VOICE_MULTI_RTP_INTERFACE_VERSION
#define SKYPE_VOICE_MULTI_RTP_INTERFACE_VERSION 5
#endif
/*!
 * \brief RTP audio interface, multi-channel version
 *
 * This class provides an RTP based interface to integrate different
 * voice solutions into Skype/SkypeKit. The voice software (VoiceEngine)
 * must handle all of the RTP packaging, jitter buffer, encoding/decoding
 * and DSP interfacing functionality.
 *
 * The crude outline of the process for voice integration
 * into Skype is following:
 * - The voice integrator creates a class inheriting from SkypeMultiRTPInterface,
 *   implements it and implements the SkypeMultiRTPInterfaceGet().
 * - After that he must create a library (libVE.a) that involves the
 *   created components and also the VoiceEngine functionality and sends
 *   it to Skype along with this file.
 * - Skype will link that library into SkypeKit to get a working
 *   voice solution.
 *
 * All the calls into SkypeMultiRTPInterface are made from Skype's thread.
 * These functions should not block for a long time, as they would block
 * Skype or SkypeKit itself.
 * VoiceEngine may send packets to SendTransport::SendRTPPacket()
 * from any thread.
 */
struct SkypeMultiRTPInterface {


				#if SKYPE_VOICE_MULTI_RTP_INTERFACE_VERSION > 1
		virtual ~SkypeMultiRTPInterface() {}
		#endif
		
				class Codec
		{
		public:
								Sid::String Name;
								int PayloadType;
								int Freq;
								int Channels;
								Sid::List_uint SupportedPacketSizes;
								int DefaultPacketSize;
								int MinBitrate;
								int MaxBitrate;
								uint MaxDesiredPacketLoss;
				};
				
				
				/*!
		 * Return free form version information about the voice library.
		 * It should involve the version of the library, name and version
		 * of the VoiceEngine used and also the name of the organisation
		 * who did the integration.
		 */
				virtual int  GetVersion(Sid::String & version) = 0;
				
				/*!
		 * All functions can return false when error occurs. This function
		 * is used to retrieve the error code in such a case. This error code
		 * is used for debugging and feedback purposes.
		 */
				virtual int  GetLastError() = 0;
				
				/*!
		 * Initialize the VoiceEngine. This is called when SkypeKit is started.
		 * If the VoiceEngine being initialized means consuming significant
		 * ressources (CPU or memory), it would be better to delay the init
		 * until StartPlayback() or StartRecording() is called, and shutdown
		 * after StopPlayback() or StopRecording() (deferred init).
		 */
				virtual int  Init() = 0;
				
				/*! Shutdown the VoiceEngine. This is called when SkypeKit is stopped. */
				virtual int  Uninit() = 0;
				
				/*!
		 * Create voice channel for call.
		 * CreateChannel returns channel_id, used for all subsequent voice operations, or -1 if 
		 * creating channel fails.
		 * call_member_id is SkypeKit's CallMember object id that the voice channel is created for.
		 * conversation_id is SkypeKit's Call object id that the voice channel belongs to. If 
		 * beeing conference host is supported by VoiceEngine, all channels with same conversation_id
		 * should be mixed together
		 */
				virtual int  CreateChannel(uint call_member_id, uint conversation_id, int &channel) = 0;
				
				/*! DeleteChannel voice channel. */
				virtual int  DeleteChannel(int channel_id) = 0;
				
				/*! Change the conversation and callmember . */
				virtual int  ChangeConversationId(int channel_id, uint conversation_id, uint call_member_id) = 0;
				
				/*!
		 * Iterate available codecs.
		 * If the Voice Engine supports more than one codec, the list must be ordered
		 * so that more preferred codecs are listed first.
		 * For example G729 must come before EG711A.
		 * At least G729 codec with 20 and 30 ms packet size must be supported. If not,
		 * some Skype or SkypeOut calls will fail.
		 */
				virtual uint  GetCodec(uint id, Codec & codec) = 0;
				
				virtual int  GetCodecCount(uint &count) = 0;
				
				/*!
		 * Set the codec and packet size used for recording.
		 * SetSendCodec is called before and during recording, but not before playback. SkypeKit
		 * can decide which codec to use for recording, but for playback the Voice Engine
		 * has to play whatever is in the RTP packets. It means when playing a voicemail,
		 * SetSendCodec will not be called.
		 * While the codec name and payload type is decided before starting recording and doesn't change during
		 * recording, the packet size will change during recording. Also for variable bitrate
		 * codecs, bitrate will change.
		 */
				virtual int  SetSendCodec(int channel_id, const Sid::String &Name, int PacketSize, int BitRate, int PayloadType) = 0;
				
				virtual void InformSendPacketLoss(int channel_id, uint packetLossBeforeFEC, uint packetLossAfterFEC, uint jitterBeforeFEC, uint jitterAfterFEC) = 0;
				
				/*! Start playback. */
				virtual int  StartPlayback(int channel_id) = 0;
				
				/*! Stop playback. */
				virtual int  StopPlayback(int channel_id) = 0;
				
				/*!
		 * Start recording voice, encoding it and sending RTP packets
		 * to the given transport. The transport is provided and implemented
		 * by Skype.
		 */
				virtual int  StartRecording(int channel_id) = 0;
				
				/*! Stop recording. */
				virtual int  StopRecording(int channel_id) = 0;
				
				/*! Submit an incoming RTP packet for playback. */
				virtual void SubmitRTPPacket(int channel_id, const Sid::Binary &data) = 0;
				
				/*!
		 * Send given DTMF code; eventnr is RFC 2833 event code,
		 * currently 0-9, * and # are used. Also play to speakers
		 * for user feedback.
		 * When rtp_type is 0 then mix the DTMF tone into voice stream,
		 * otherwise send as an RTP packet.
		 */
				virtual int  SendDTMF(int channel_id, int eventnr, int rtp_type) = 0;
				
				/*!
		 * Sets the output amplification of the playback.
		 * 100 means no amplication, 150 would mean 1.5 amplification.
		 * Scale between 50 (x 0.5 amplification) and 300 (x 3 amplification) should 
		 * be supported if possible, but currently in practice only 150 is used.
		 *
		 * SetOutputAmplification gets called after StartPlayback.
		 * StartPlayback should always assume 100 (i.e. no amplification). If necessary 
		 * SetOutputAmplification will be called with 150, then the amplicatification
		 * should be set.
		 */
				virtual int  SetOutputAmplification(int channel_id, int scale) = 0;
				
				/*!
		 * API meant for UI->VoiceEngine direct custom commands that do
		 * not involve SkypeKit directly.
		 * Basically under here you will put things like volume control,
		 * muting, various custom hw related functionality like (ring)tone
		 * playing, switching headset-handset mode, etc.
		 * Returned string pointer must be alive until next call to CustomCommand().
		 */
				virtual int  CustomCommand(const Sid::String &cmd, Sid::String & result_str) = 0;
				
};

struct SkypeMultiRTPCallbackInterface {
	virtual ~SkypeMultiRTPCallbackInterface() {}
	virtual void SendRTPPacket(int channel_id, const Sid::Binary &data) = 0;
};


/*!
 * Skype calls this method to get an instance of the SkypeMultiRTPInterface.
 * This function must be implemented and provided in the archive by
 * the voice integrator. Skype may call this function repeatedly.
 */
extern "C" SkypeMultiRTPInterface * SkypeMultiRTPInterfaceGet(SkypeMultiRTPCallbackInterface* );

extern "C" void SkypeMultiRTPInterfaceRelease(SkypeMultiRTPInterface* rtpi);

#endif /* !__SkypeMultiRTPInterface_hpp__ */
