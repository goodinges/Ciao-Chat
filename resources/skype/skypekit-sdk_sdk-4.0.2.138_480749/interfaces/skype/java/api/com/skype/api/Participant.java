package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * Conversation participant class. Instances of this class represent contacts when in the context of conversations. Amongst others, this class has a Ring method for requesting live status with the target contact. This class also holds typing indicator property and access rights for the contact in context of public conversations. <br>
 */


public class Participant extends SkypeObject {


	public interface ParticipantListener {
		/** This event gets called when there are changes to Participant properties defined in Participant.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
		/**This event gets fired on receiving a DTMF signal sent by Participant. Note that this event will only fire if the Participant is also using a Skype client. Skype audio library does not monitor incoming voice streams for dial tones. DTMF events are propagated to remote participants via data channel. Incoming DTMF tones transmitted from, for example, mobile phones, will not cause this event to fire. In case of incoming DTMF signals from Skype clients, DTMF tones are also inserted into the audio stream. You don't have to inject those into local audio playback yourself. <br>*/
		public void OnIncomingDTMF(SkypeObject obj, DTMF dtmf);
		
	}
	
	public Participant(int oid, Skype skype) {
		super(oid,skype);
		/**get default properties for this module **/
		GetDefaultProps();
	}
	
	private static final int MODULE_ID = 19;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the Participant class */
	public enum PROPERTY {
	
		/** [ALL] ID of corresponding conversation. Here and below, [ALL] tag indicates that the property has meaning and is set in context of all participants in the conversation. [OTHERS] tag has meaning only for participants who are not the local user. <br>, type: Conversation */
		convo_id(930),
		
		/** [ALL] skypename OR pstn_number OR namespace:identity, type: String */
		identity(931),
		
		/** [ALL] Participant.RANK, type: RANK */
		rank(932),
		
		/** Not set (should be: requested Participant.RANK, higher than the current one), type: RANK */
		requested_rank(933),
		
		/** [ALL] the typing indicator, type: TEXT_STATUS */
		text_status(934),
		
		/** [ALL] voice status, type: VOICE_STATUS */
		voice_status(935),
		
		/** [ALL] video status, type: VIDEO_STATUS */
		video_status(936),
		
		/** [ALL] identity that was used to establish current live session with that participant (can be different from participant identity), type: String */
		live_identity(943),
		
		/** [OTHERS] 'price_per_minute_float currency' - eg '0.01 EUR'. Note that this property obtains value only after the participant goes into live state. It cannot be used to display call rates before the call starts. <br>, type: String */
		live_price_for_me(938),
		
		/** [OTHERS] list of identities where the live session is being forwarded (if they are disclosed), space separated, type: String */
		live_fwd_identities(948),
		
		/** [ALL] time of joining the live session, type: int */
		live_start_timestamp(939),
		
		/** [ALL] current 'loudness' level when SPEAKING (0..10), type: int */
		sound_level(941),
		
		/** [OTHERS] call (audio and video) debug info, type: String */
		debuginfo(942),
		
		/** [OTHERS] DEPRECATED, use last_leavereason instead, type: String */
		last_voice_error(947),
		
		/** [ALL] space separated tokens values: CPU_INUSE CPU_SLOW CPU_HIGH HIGH_ECHO HIGH_NOISE MUTED_INPUT LOW_INPUT MUTED_INPUT_ACTIVITY FW_STRONG FW_BAD NOT_UDP CALL_BW_LOW RECORD_ERROR + values in video debug info, type: String */
		quality_problems(949),
		
		/** [ALL] participant type during livesession as specified in IDENTITYTYPE, type: contact.IDENTITYTYPE */
		live_type(950),
		
		/** [OTHERS] participant livesession country code - used for emergency calls only atm, type: String */
		live_country(951),
		
		/** [OTHERS] Transferor identity (transferee side) <br>, type: String */
		transferred_by(952),
		
		/** [OTHERS] Identity of recipient of transfer (transferor side, caller side) <br>, type: String */
		transferred_to(953),
		
		/** [ALL] Identity of the user who added this participant to the conversation, type: Sid.String  <br>, type: String */
		adder(954),
		
		/** [OTHERS] Last reason for leaving conversation or live session. Provides an enum alternative to last_voice_error as well as the reason this participant RETIRED from the conversation.  <br>, type: LEAVE_REASON */
		last_leavereason(955);
		
		private static final Map<Integer,PROPERTY> lookup = new HashMap<Integer,PROPERTY>();
		
		static {
			for(PROPERTY s : EnumSet.allOf(PROPERTY.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private PROPERTY(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static PROPERTY get(int code) {
			return lookup.get(code);
		}
		
		public static PROPERTY fromString(String s) {
			for (PROPERTY p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	public Object GetPropertyAsEnum(int propid) {
		return PROPERTY.get(propid);
	}
	
	public String GetStrProperty(PROPERTY prop) {
		//check in propcache if so then return
		if (mPropCache.containsKey(new Integer(prop.id))){
			String value =  (String)mPropCache.get(prop.id);
			if (value != null && !(value.length() == 0) ){
				return value;
			}
		}
		//else get from skypekit...
		GetPropertyRequest request = new GetPropertyRequest(19, mObjectId, prop.id);
		
		String string = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null){
			string = r.GetAsString();
		}
		
		if (string != null)
		{
			mPropCache.put(new Integer(prop.id), string);
		}
		return string;
	}
	
	public int GetIntProperty(PROPERTY prop) {
		//check in propcache if so then return
		if (mPropCache.containsKey(new Integer(prop.id))){
			int value = ((Integer)mPropCache.get(prop.id)).intValue();
			if (value != 0){
				return value;
			}
		}
		//else get from skypekit...
		GetPropertyRequest request = new GetPropertyRequest(moduleID(), mObjectId, prop.id);
		
		Integer integer = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null){
			integer  = r.GetAsInt();
		}
		
		if (integer != null)
		{
			mPropCache.put(new Integer(prop.id), integer);
			return integer.intValue();
		}
		else
		{
			return 0;
		}
	}
	
	public boolean GetBooleanProperty(PROPERTY prop) {
		//check in propcache if so then return
		if (mPropCache.containsKey(new Integer(prop.id))){
			return ((Boolean)mPropCache.get(prop.id)).booleanValue();
		}
		//else get from skypekit...
		GetPropertyRequest request = new GetPropertyRequest(moduleID(), mObjectId, prop.id);
		
		Boolean boolResp = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null){
			boolResp  = r.GetAsBoolean();
		}
		
		if (boolResp != null)
		{
			mPropCache.put(new Integer(prop.id), boolResp);
			return boolResp.booleanValue();
		}
		else
		{
			return false;
		}
	}
	
	public byte [] GetBinProperty(PROPERTY prop) {
		//get from skypekit...
		GetPropertyRequest request = new GetPropertyRequest(19, mObjectId, prop.id);
		
		byte [] data = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null) {
			data = r.GetAsBinary();
		}
		return data;
	}
	
	/**default array of Participant Properties that get fetched & cached upon class construction*/
	private static PROPERTY [] defaultProperties = { PROPERTY.convo_id, PROPERTY.rank, PROPERTY.identity, PROPERTY.adder};
	
	private void GetDefaultProps() {
		MultiGetPropertyRequest request = null;
		ArrayList<Integer> proparray = null;
			/**Add the single oid into array*/
			ArrayList<Integer> oidarray=new ArrayList<Integer>();
			oidarray.add(mObjectId);
			
			/**Add all requested propids into array*/
			proparray=new ArrayList<Integer>();
			for (PROPERTY defaultProp : defaultProperties) {
				proparray.add(defaultProp.getId());
			}
			/**Generate the request*/
			request = new MultiGetPropertyRequest(moduleID(), oidarray,proparray);
			
			/** Make Multi Get call*/
			GetPropertyResponse r=skype.MultiGetProperty(request);
			/**Verify that it is a proper multiresponse*/
			if(!r.isMultiresponse())
			{
				return;
			}
			/**update property cache with results*/
			mPropCache.putAll(r.GetAsMap(mObjectId, proparray));
		}
		
		/**
		Recognized values for the P_RANK property. The P_RANK controls participant's privileges in the conversation. See Participant.CanSetRankTo and Participant.SetRankTo methods. <br> */
		public enum RANK {
		
			/** Creator of the chat. There can be only one participant with this type per conversation. Other participants cannot be promoted to Creator rank. <br>*/
			CREATOR(1),
			
			/** Participant who has administrator privileges <br>*/
			ADMIN(2),
			
			/** Participant who can speak and write <br>*/
			SPEAKER(3),
			
			/** Participant who can write but not speak <br>*/
			WRITER(4),
			
			/** Participant who can read but not write/speak <br>*/
			SPECTATOR(5),
			
			/** Participant who is applying to join the conversation. Member cannot be demoted to applicants once they have been accepted.  <br>*/
			APPLICANT(6),
			
			/** Participant who has eft or has been kicked from the conversation <br>*/
			RETIRED(7),
			
			/** Participant who has been banned from the chat <br>*/
			OUTLAW(8);
			
			private static final Map<Integer,RANK> lookup = new HashMap<Integer,RANK>();
			
			static {
				for(RANK s : EnumSet.allOf(RANK.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private RANK(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static RANK get(int code) {
				return lookup.get(code);
			}
			
			public static RANK fromString(String s) {
				for (RANK p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_TEXT_STATUS property. The P_TEXT_STATUS property has two uses. Firstly, you can use it to implement typing indicators in your UI, to notify the local user that an incoming chat message from this Participant is imminent.  <br><br>To set the P_TEXT_STATUS value, so that remote client UIs can display the local user's typing indicator in their UI, use Conversation.SetMyTextStatusTo method. <br><br>Transmission of P_TEXT_STATUS updates to remote participants of conversations is controlled via SETUPKEY_DISABLE_CHAT_ACTIVITY_INDICATION setup key. <br><br>Secondly, the TEXT_NA value enables you to detect participants who are running clients with no chat capability. <br> */
		public enum TEXT_STATUS {
		
			/** Fallback state in case the text status is not (yet) deternmined. <br>*/
			TEXT_UNKNOWN(0),
			
			/** Text status is not applicable as the participant is using a Skype client that does not support chat (for example: voice-only USB phones). <br>*/
			TEXT_NA(1),
			
			/** Participant is currently not typing. <br>*/
			READING(2),
			
			/** Participant is currently typing. <br>*/
			WRITING(3),
			
			/** This state should be set when following two conditions are true: <br> - interval between keypresses are less than 20 ms; <br> - at least one of the keys adjacent to current key are pressed down. <br>*/
			WRITING_AS_ANGRY(4),
			
			/** The "Cat on keyboard detection" algorthm in Skype is implemented in the UI level, and as such is not present in the SkypeKit API. Should you wish to implement similar algorthm in your own UI, you can get the basic logic from the PawSense FAQ - http://www.bitboost.com/pawsense/pawsense-faq.html <br>*/
			WRITING_AS_CAT(5);
			
			private static final Map<Integer,TEXT_STATUS> lookup = new HashMap<Integer,TEXT_STATUS>();
			
			static {
				for(TEXT_STATUS s : EnumSet.allOf(TEXT_STATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private TEXT_STATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static TEXT_STATUS get(int code) {
				return lookup.get(code);
			}
			
			public static TEXT_STATUS fromString(String s) {
				for (TEXT_STATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_VOICE_STATUS property.  <br> */
		public enum VOICE_STATUS {
		
			/** Participant voice status is not (yet) determined. <br>*/
			VOICE_UNKNOWN(0),
			
			/** Participant is using a Skype client with no audio capability. <br>*/
			VOICE_NA(1),
			
			/** Participant is using a Skype client that supports audio. <br>*/
			VOICE_AVAILABLE(2),
			
			/** Participant is in process of joining current live session. This is a transitional state. <br>*/
			VOICE_CONNECTING(3),
			
			/** Participant has been invited to join the live session but has not yet accepted. <br>*/
			RINGING(4),
			
			/** Participant is in process of joining current live session. This is another transitional state. <br>*/
			EARLY_MEDIA(5),
			
			/** Participant has joined the current live session but is currently not transmitting audio. <br>*/
			LISTENING(6),
			
			/** Participant has joined the current live session and is transmitting audio. The UI can either use this state to display appropriate "speaking" notification, or alternatively use Participant.P_SOUND_LEVEL if you want your speaking indicator to also reflect audio volume coming from the Participant. <br>*/
			SPEAKING(7),
			
			/** Participant has joined the current live session but the audio is currently on hold. <br>*/
			VOICE_ON_HOLD(8),
			
			/** Participant will be placed in this state for some seconds after live session has finished. This is another transitional state. <br>*/
			VOICE_STOPPED(9);
			
			private static final Map<Integer,VOICE_STATUS> lookup = new HashMap<Integer,VOICE_STATUS>();
			
			static {
				for(VOICE_STATUS s : EnumSet.allOf(VOICE_STATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private VOICE_STATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static VOICE_STATUS get(int code) {
				return lookup.get(code);
			}
			
			public static VOICE_STATUS fromString(String s) {
				for (VOICE_STATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_VIDEO_STATUS property. This property applies to Participant's video send capability, not capability to receive video. <br> */
		public enum VIDEO_STATUS {
		
			/** Video status is not (yet) determined. <br>*/
			VIDEO_UNKNOWN(0),
			
			/** Indicates that this Participant does not have video available.. <br>*/
			VIDEO_NA(1),
			
			/** Indicates that video is available for this participant. When the Participant.P_VIDEO_STATUS obtains this state, it is possible to retrieve the Video object, using Participant.GetVideo method. Further operations, such as starting or stopping the video send/receive will then go through the Video object. <br>*/
			VIDEO_AVAILABLE(2),
			
			/** Transitional state indicating that the Participant is attempting to initiate video send. <br>*/
			VIDEO_CONNECTING(3),
			
			/** Indicates that the participant is currently sending video. <br>*/
			STREAMING(4),
			
			/** Indicates that the participant video send is currently paused. <br>*/
			VIDEO_ON_HOLD(5);
			
			private static final Map<Integer,VIDEO_STATUS> lookup = new HashMap<Integer,VIDEO_STATUS>();
			
			static {
				for(VIDEO_STATUS s : EnumSet.allOf(VIDEO_STATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private VIDEO_STATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static VIDEO_STATUS get(int code) {
				return lookup.get(code);
			}
			
			public static VIDEO_STATUS fromString(String s) {
				for (VIDEO_STATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 */
		public enum DTMF {
		
			/** */
			DTMF_0(0),
			
			/** */
			DTMF_1(1),
			
			/** */
			DTMF_2(2),
			
			/** */
			DTMF_3(3),
			
			/** */
			DTMF_4(4),
			
			/** */
			DTMF_5(5),
			
			/** */
			DTMF_6(6),
			
			/** */
			DTMF_7(7),
			
			/** */
			DTMF_8(8),
			
			/** */
			DTMF_9(9),
			
			/** */
			DTMF_STAR(10),
			
			/** */
			DTMF_POUND(11);
			
			private static final Map<Integer,DTMF> lookup = new HashMap<Integer,DTMF>();
			
			static {
				for(DTMF s : EnumSet.allOf(DTMF.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private DTMF(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static DTMF get(int code) {
				return lookup.get(code);
			}
			
			public static DTMF fromString(String s) {
				for (DTMF p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *Checks whether the current user can set this Participant's conversation privileges to the specified RANK. This enables you to gray out or disable in your UI all the unavailable options for Participant.SetRankTo method. <br>
		 * @param rank Participant.RANK value to check for. <br>
		 * @return result Returns true if local user can set participant's rank to the value given in rank argument. <br>
		 */
		public boolean CanSetRankTo( RANK rank) {
		
			Request request = null;
			try {
				request = new XCallRequest(19,1);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,rank.getId());
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return false;
				
			boolean result = false;
			result = r.GetAsBoolean(1);
			return result;
		}
		
		/**
		 *Sets Participant's conversation privileges to the given RANK <br>
		 * @param rank Target Participant.RANK value. <br>
		 */
		public void SetRankTo( RANK rank) {
		
			Request request = null;
			try {
				request = new XCallRequest(19,2);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,rank.getId());
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Initiates live conversation attempt with Participant. <br>
		 * @param identityToUse Ring an alternate identity, such as a PSTN number. <br>
		 * @param videoCall Enable video. <br>
		 * @param nrofRedials Unused. <br>
		 * @param redialPeriod Unused. <br>
		 * @param autoStartVM Unused. On dialog, if falling on VM, greeting and recording will be automatically started. <br>
		 * @param origin When call is initiated from web link, must contain the URI that was used
		 */
		public void Ring( String identityToUse, boolean videoCall, int nrofRedials, int redialPeriod, boolean autoStartVM, String origin) {
		
			Request request = null;
			try {
				request = new XCallRequest(19,3);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,identityToUse);
			request.addParm('b',2,videoCall);
			request.addParm('u',3,nrofRedials);
			request.addParm('u',4,redialPeriod);
			request.addParm('b',5,autoStartVM);
			request.addParm('S',6,origin);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Rings this participant, using P_LIVE_IDENTITY property if set. <br>
		 */
		public void RingIt() {
		
			Request request = null;
			try {
				request = new XCallRequest(19,8);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Sets LIVE_IDENTITY property, an alternate identity to use when ringing, such as a PSTN. <br>
		 * @param identityToUse Empty string will reset it to default, i.e IDENTITY property value <br>
		 */
		public void SetLiveIdentityToUse( String identityToUse) {
		
			Request request = null;
			try {
				request = new XCallRequest(19,7);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,identityToUse);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Retrieves a reference to the Video object that corresponds to the Participant. It can be either local video - you can check if this participant's name (P_IDENTITY property) matches the name of the currently logged in account (P_SKYPENAME property) or incoming video from a remote participant.  <br><br>Note that for GetVideo to be successful, the video has to be available for that participant. This can be checked for by examining Participant VIDEO_STATUS property - once it becomes VIDEO_AVAILABLE - you can use GetVideo to obtain the Video object. <br>
		 * @return video Returns reference to a constructed video object. <br>
		 */
		public Video GetVideo() {
		
			Request request = null;
			try {
				request = new XCallRequest(19,4);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			int oid = 0;
			Video video = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) {
				video = (Video)skype.factory(Video.moduleID(), oid, skype);
			}
			return video;
		}
		
		/**
		 *Removes this participant from the current live session. Note that this does not remove the participant from conversation (for this, use Participant.Retire). It only removes participant from live state. <br>
		 */
		public void Hangup() {
		
			Request request = null;
			try {
				request = new XCallRequest(19,5);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Forcibly removes this participant from conversation. This method is for removing other people from conversations (for example: as administrative punishment for flooding conversation with spam messages). For local user to leave a conversation, use Conversation.RetireFrom instead. <br>
		 */
		public void Retire() {
		
			Request request = null;
			try {
				request = new XCallRequest(19,6);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
	
	}