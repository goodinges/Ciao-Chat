package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * The Conversation class encapsulates all types of communication possible with Skype client. Instant messaging, calls, video calls, file transfers, SMS, screen sharing - all take place within the context of a Conversation. Contacts are represented in Conversation as Participant objects. This also applies to contacts of PSTN type. All events in a conversation are represented as Message objects.  <br>
 */


public class Conversation extends SkypeObject {


	public interface ConversationListener {
		/** This event gets called when there are changes to Conversation properties defined in Conversation.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
		/**This callback gets fired when participants join or leave the conversation. <br>*/
		public void OnParticipantListChange(SkypeObject obj);
		
		/**Called for each message in currently loaded conversations*/
		public void OnMessage(SkypeObject obj, Message message);
		
		/**This callback gets fired when a new Conversation is created using SpawnConference. <br>*/
		public void OnSpawnConference(SkypeObject obj, Conversation spawned);
		
	}
	
	public Conversation(int oid, Skype skype) {
		super(oid,skype);
		/**get default properties for this module **/
		GetDefaultProps();
	}
	
	private static final int MODULE_ID = 18;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the Conversation class */
	public enum PROPERTY {
	
		/** contact identity in case of dialogs, chat name in case of conferences, type: String */
		identity(972),
		
		/** type of the conversation, type: Conversation.TYPE */
		type(902),
		
		/** host of current live session. none => no session. myself in case of 1:1 calls, type: String */
		live_host(918),
		
		/** moment when first participant other than host joined the current or last live session, type: int */
		live_start_timestamp(974),
		
		/** if live session is muted, type: boolean */
		live_is_muted(996),
		
		/** '' everything matches, '=' nothing matches, '=string' string matches, type: String */
		alert_string(920),
		
		/** if conversation is bookmarked/flagged, type: boolean */
		is_bookmarked(921),
		
		/** name assigned via GiveDisplayname, type: String */
		given_displayname(925),
		
		/** resulting display name of the conversation (based on given name, topic, participant list, etc), type: String */
		displayname(924),
		
		/** if the conversation is live and in which status it is then, type: LOCAL_LIVESTATUS */
		local_livestatus(927),
		
		/** timestamp to sort the conversations in inbox by. 0 means not in inbox, type: int */
		inbox_timestamp(928),
		
		/** ID of the message that caused INBOX_TIMESTAMP to be set, type: Message */
		inbox_message_id(973),
		
		/** number of messages in UNCONSUMED_SUPPRESSED consumption status, type: int */
		unconsumed_suppressed_messages(975),
		
		/** number of messages in UNCONSUMED_NORMAL consumption status, type: int */
		unconsumed_normal_messages(976),
		
		/** DEPRECATED, not set anymore, type: int */
		unconsumed_elevated_messages(977),
		
		/** if there are unconsumed voice or call messages in the conversation, type: boolean */
		unconsumed_messages_voice(970),
		
		/** ID of voice message that is being played or recorded in this conversation, type: Voicemail */
		active_vm_id(971),
		
		/** consumption cutoff timestamp: messages after (but not including) this are considered unconsumed, type: int */
		consumption_horizon(979),
		
		/** timestamp of last activity in conversation, type: int */
		last_activity_timestamp(981),
		
		/** dialog this conference was spawned from, type: Conversation */
		spawned_from_convo_id(915),
		
		/** identity of conversation creator (doesn't apply to dialogs), type: String */
		creator(903),
		
		/** timestamp of creation, tells you how far you can retrieve messages, type: int */
		creation_timestamp(904),
		
		/** my status in this conversation (connecting, participating, retired, etc) (doesn't apply to dialogs), type: MY_STATUS */
		my_status(919),
		
		/** if it's a public conversation (doesn't apply to dialogs), type: boolean */
		opt_joining_enabled(922),
		
		/** rank that is auto-assigned at join (doesn't apply to dialogs), type: Participant.RANK */
		opt_entry_level_rank(906),
		
		/** if history visible to new consumers (doesn't apply to dialogs), type: boolean */
		opt_disclose_history(907),
		
		/** activities that only ADMIN can do. Bitmap of ALLOWED_ACTIVITY values (doesn't apply to dialogs), type: ALLOWED_ACTIVITY */
		opt_admin_only_activities(909),
		
		/** public conversation password hint, use SetPassword to set (doesn't apply to dialogs), type: String */
		passwordhint(980),
		
		/** user-set name, separate from topic (doesn't apply to dialogs), type: String */
		meta_name(910),
		
		/** conversation topic (doesn't apply to dialogs), type: String */
		meta_topic(911),
		
		/** guidelines (doesn't apply to dialogs), type: String */
		meta_guidelines(913),
		
		/** conversation picture, in jpeg format (doesn't apply to dialogs), type: byte[] */
		meta_picture(914);
		
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
		GetPropertyRequest request = new GetPropertyRequest(18, mObjectId, prop.id);
		
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
		GetPropertyRequest request = new GetPropertyRequest(18, mObjectId, prop.id);
		
		byte [] data = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null) {
			data = r.GetAsBinary();
		}
		return data;
	}
	
	/**default array of Conversation Properties that get fetched & cached upon class construction*/
	private static PROPERTY [] defaultProperties = { PROPERTY.identity, PROPERTY.type, PROPERTY.given_displayname, PROPERTY.displayname, PROPERTY.last_activity_timestamp, PROPERTY.live_host, PROPERTY.consumption_horizon, PROPERTY.unconsumed_suppressed_messages, PROPERTY.unconsumed_normal_messages};
	
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
		
		/** Setupkey SETUPKEY_ENABLE_BIRTHDAY_NOTIFICATION type:int default value:"1" <br>Enables/disables birthday notification messages. <br> - 0 - disable;  <br> - 1 - enable; <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String ENABLE_BIRTHDAY_NOTIFICATION = "Lib/Conversation/EnableBirthday";
		
		/** Setupkey SETUPKEY_INBOX_UPDATE_TIMEOUT type:int  <br>Timeout in seconds, how old the Conversation.P_INBOX_TIMESTAMP has to be for it to be re-sorted in the inbox. <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String INBOX_UPDATE_TIMEOUT = "Lib/Conversation/InboxUpdateTimeout";
		
		/** Setupkey SETUPKEY_RECENTLY_LIVE_TIMEOUT type:int default value:"20" <br>The number of seconds a formerly live conversation will remain the Conversation.LIVE_CONVERSATIONS filter. Note that while the conversation remains in Conversation.LIVE_CONVERSATIONS filter, Skype.OnConversationListChange events will not fire if there is another call coming up within the same conversation. Seeting this key to 0 will cause conversations to exit the Conversation.LIVE_CONVERSATIONS list immediately, after live state drops. <br><br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String RECENTLY_LIVE_TIMEOUT = "Lib/Conversation/RecentlyLiveTimeout";
		
		/** Setupkey SETUPKEY_DISABLE_CHAT type:int  Disables chat (for voice only clients). <br>This setup key is machine-specific and affects all local accounts. <br> */
		public static final String DISABLE_CHAT = "Lib/Chat/DisableChat";
		
		/** Setupkey SETUPKEY_DISABLE_CHAT_HISTORY type:int  <br>Disables storage of chat history. <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String DISABLE_CHAT_HISTORY = "Lib/Message/DisableHistory";
		
		/** Setupkey SETUPKEY_CHAT_HISTORY_DAYS type:int  <br>Time limit for keeping local chat message history. <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String CHAT_HISTORY_DAYS = "Lib/Chat/HistoryDays";
		
		/** Setupkey SETUPKEY_CHATDB_LIMIT_KB type:int default value:"0" Use this key to limit the size of the chat db. Value is in KB. By default there is no limit. A minimum of 16 MB is recommended. */
		public static final String CHATDB_LIMIT_KB = "Lib/Chat/ChatDBLimitKb";
		
		/** Setupkey SETUPKEY_DISABLE_CHAT_ACTIVITY_INDICATION type:int  <br>Enables/disables transmitting typing indicator signals to othe participants of conversations. <br> - 0 - disable;  <br> - 1 - enable; <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String DISABLE_CHAT_ACTIVITY_INDICATION = "Lib/Chat/DisableActivityIndication";
		
		/** Setupkey SETUPKEY_CALL_NOANSWER_TIMEOUT type:int default value:"15" <br>Timeout in seconds after which the incoming live session will stop ringing (and if possible, proceed to voicemail or call forward). <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String CALL_NOANSWER_TIMEOUT = "Lib/Call/NoAnswerTimeout";
		
		/** Setupkey SETUPKEY_CALL_SEND_TO_VM type:int  <br>Autoforwarding of incoming calls to voicemail. <br> - 0 - off <br> - 1 - on <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String CALL_SEND_TO_VM = "Lib/Call/SendToVM";
		
		/** Setupkey SETUPKEY_CALL_APPLY_CF type:int  <br>Enables/disables call forwarding. <br> - 0 - disable;  <br> - 1 - enable; <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String CALL_APPLY_CF = "Lib/Call/ApplyCF";
		
		/** Setupkey SETUPKEY_CALL_EMERGENCY_COUNTRY type:string  <br>Country code for emergency calls <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
		public static final String CALL_EMERGENCY_COUNTRY = "Lib/Call/EmergencyCountry";
		
		/**
		 */
		public enum TYPE {
		
			/** 1:1 conversations, there is a one dialog per identity*/
			DIALOG(1),
			
			/** equivalent of a multichat*/
			CONFERENCE(2),
			
			/** a conference that has been terminated (disbanded chat)*/
			TERMINATED_CONFERENCE(3),
			
			/** voice-only conference, when host is using a legacy non-conversation client*/
			LEGACY_VOICE_CONFERENCE(4),
			
			/** chat used for legacy shared groups, can be ignored*/
			LEGACY_SHAREDGROUP(5);
			
			private static final Map<Integer,TYPE> lookup = new HashMap<Integer,TYPE>();
			
			static {
				for(TYPE s : EnumSet.allOf(TYPE.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private TYPE(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static TYPE get(int code) {
				return lookup.get(code);
			}
			
			public static TYPE fromString(String s) {
				for (TYPE p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 */
		public enum MY_STATUS {
		
			/** connecting to conference*/
			CONNECTING(1),
			
			/** */
			RETRY_CONNECTING(2),
			
			/** unused*/
			DOWNLOADING_MESSAGES(3),
			
			/** conference is full for now, being queued*/
			QUEUED_TO_ENTER(4),
			
			/** I'm applying to join the conference*/
			APPLICANT(5),
			
			/** My application to join the conference was denied*/
			APPLICATION_DENIED(6),
			
			/** The password I provided is incorrect*/
			INVALID_ACCESS_TOKEN(7),
			
			/** I'm part of the conference, I can participate*/
			CONSUMER(8),
			
			/** I was kicked from the conference*/
			RETIRED_FORCEFULLY(9),
			
			/** I left the conference*/
			RETIRED_VOLUNTARILY(10);
			
			private static final Map<Integer,MY_STATUS> lookup = new HashMap<Integer,MY_STATUS>();
			
			static {
				for(MY_STATUS s : EnumSet.allOf(MY_STATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private MY_STATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static MY_STATUS get(int code) {
				return lookup.get(code);
			}
			
			public static MY_STATUS fromString(String s) {
				for (MY_STATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 */
		public enum LOCAL_LIVESTATUS {
		
			/** there isn't a live session*/
			NONE(0),
			
			/** trying to start or join a live session*/
			STARTING(1),
			
			/** there is a live session ringing*/
			RINGING_FOR_ME(2),
			
			/** the conference is live for me*/
			IM_LIVE(3),
			
			/** I put the live session on hold*/
			ON_HOLD_LOCALLY(5),
			
			/** the live session was put on hold by someone else*/
			ON_HOLD_REMOTELY(6),
			
			/** there is a live session on-going, I'm not participating but I could join*/
			OTHERS_ARE_LIVE(7),
			
			/** there is a live session on-going without me, but I can't join because it's full*/
			OTHERS_ARE_LIVE_FULL(11),
			
			/** playing a voicemail (dialog only)*/
			PLAYING_VOICE_MESSAGE(8),
			
			/** recording a voicemail (dialog only)*/
			RECORDING_VOICE_MESSAGE(9),
			
			/** a live session just finished, we stay in this state for RECENTLY_LIVE_TIMEOUT setup key*/
			RECENTLY_LIVE(10),
			
			/** call is being transferred*/
			TRANSFERRING(12);
			
			private static final Map<Integer,LOCAL_LIVESTATUS> lookup = new HashMap<Integer,LOCAL_LIVESTATUS>();
			
			static {
				for(LOCAL_LIVESTATUS s : EnumSet.allOf(LOCAL_LIVESTATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private LOCAL_LIVESTATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static LOCAL_LIVESTATUS get(int code) {
				return lookup.get(code);
			}
			
			public static LOCAL_LIVESTATUS fromString(String s) {
				for (LOCAL_LIVESTATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		values for opt_admin_only_activities property */
		public enum ALLOWED_ACTIVITY {
		
			/** allowed to set the CONVERSATION_META properties*/
			SET_META(1),
			
			/** allowed to add participants to the conference*/
			ADD_CONSUMERS(2),
			
			/** allowed to speak, but not write*/
			SPEAK(4),
			
			/** allowed to speak and write*/
			SPEAK_AND_WRITE(8);
			
			private static final Map<Integer,ALLOWED_ACTIVITY> lookup = new HashMap<Integer,ALLOWED_ACTIVITY>();
			
			static {
				for(ALLOWED_ACTIVITY s : EnumSet.allOf(ALLOWED_ACTIVITY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private ALLOWED_ACTIVITY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static ALLOWED_ACTIVITY get(int code) {
				return lookup.get(code);
			}
			
			public static ALLOWED_ACTIVITY fromString(String s) {
				for (ALLOWED_ACTIVITY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *Setter method for Conversation option properties. Option properties are all Conversation properties starting with OPT_ prefix. <br>
		 * @param propKey Conversation property key, for example: Conversation.OPT_JOINING_ENABLED <br>
		 * @param value New value for the option property. <br>
		 */
		public void SetOption( int propKey, int value) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,1);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,propKey);
			request.addParm('u',2,value);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Setter for Conversation class META_TOPIC. This topic will be set for remote participants as well. <br>
		 * @param topic New conversation topic. <br>
		 * @param isXML Notifies remote UIs that the new topic contains xml tags. <br>
		 */
		public void SetTopic( String topic, boolean isXML) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,2);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,topic);
			request.addParm('b',2,isXML);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Setter for Conversation META_GUIDELINES. This property will be visible to remote participants of the conversation. <br>
		 * @param guidelines New value for the META_GUIDELINES property. <br>
		 * @param isXML Set true to notify remote UIs that the new guideline contains XML tags. <br>
		 */
		public void SetGuidelines( String guidelines, boolean isXML) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,3);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,guidelines);
			request.addParm('b',2,isXML);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Sets the conversation's avatar to the specified JPEG image, which is propagated to both local and remote participants. Before calling this method, you should use Skype.ValidateAvatar to verify that jpeg references a valid JPEG image. <br>
		 * @param jpeg Conversation avatar binary. <br>
		 */
		public void SetPicture( byte[] jpeg) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,4);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('B',1,jpeg);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *When called from dialog conversation, this spawns a new conversation, with existing two dialog participants plus new contact identities given in the identitiesToAdd list. You do not need to add existing dialog participants to the string list. In fact, passing only the existing participants in the identities list will cause the method call to fail (return false), the same as if the list was empty. This method will also return false if the original conversation was not a dialog (contained more than two participants). Also note that this method always creates a new Conversation - even if a conversation with exactly the same participant list existed before. <br>
		 * @param identitiesToAdd String list of additional participant identities. You do not need to add existing two participants from the original dialog to this list. <br>
		 * @return conference Returns the resulting conversation or 0 if the method call failed. <br>
		 */
		public Conversation SpawnConference( String [] identitiesToAdd) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,6);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addListStart(1);
			for (int i=0;i<identitiesToAdd.length;i++) {
				request.addParm('S',identitiesToAdd[i]);
			}
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			int oid = 0;
			Conversation conference = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) {
				conference = (Conversation)skype.factory(Conversation.moduleID(), oid, skype);
			}
			return conference;
		}
		
		/**
		 *Takes one or more Contact identities and creates corresponding Participant objects within the context of this Conversation, which must be of type CONFERENCE. If you have an existing dialog conversation, use SpawnConference instead. <br>
		 * @param identities Contact identities to be added to the Conversation. <br>
		 */
		public void AddConsumers( String [] identities) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,7);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addListStart(1);
			for (int i=0;i<identities.length;i++) {
				request.addParm('S',identities[i]);
			}
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Merges two live conversations. For example, if the user already has a live conversation up - let's call it conversation A. Then a new incoming call occurs - another conversation obtains LOCAL_LIVESTATUS == Conversation.RINGING_FOR_ME, let's call it conversation B. The user wishes to pick up the new incoming call and add it to the existing one. For this you can first call B->JoinLiveSession and then merge two calls with A->Assimilate(B, A). The second argument will return the merged conversation. Note that there are actually three conversation objects involved: A (before merge), B and C (after the merge). Normally it would make sense to have the first conversation (A) as the second argument, so that it gets overwritten with the assimilation result. <br>
		 * @param otherConversation The new conversation to be merged with the one already in live state. <br>
		 * @return conversation Returns a 3rd live conversation, result of merging two existing ones. <br>
		 */
		public Conversation Assimilate( Conversation otherConversation) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,9);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,otherConversation.getOid());
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			int oid = 0;
			Conversation conversation = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) {
				conversation = (Conversation)skype.factory(Conversation.moduleID(), oid, skype);
			}
			return conversation;
		}
		
		/**
		 *starts, answers or joins a live session (first one to join becomes LIVE_HOST)
		 * @param accessToken if starting a live session, allows to set a custom access token
		 */
		public void JoinLiveSession( String accessToken) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,10);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,accessToken);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *This is an alternative to calling Ring method for each Participant individually. This also works with dialogs (with identities containing only one item). <br>
		 * @param identities List of Participants to ring. Leaving the list empty will result in ringing all participants of at least speaker level. <br>
		 * @param videoCall If true, indicates that we want to do a video call (video still needs to be separately enabled) <br>
		 * @param origin When call is initiated from web link, this argument must contain the URI that was used <br>
		 */
		public void RingOthers( String [] identities, boolean videoCall, String origin) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,36);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addListStart(1);
			for (int i=0;i<identities.length;i++) {
				request.addParm('S',identities[i]);
			}
			request.addParm('b',2,videoCall);
			request.addParm('S',3,origin);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Sets VOICE_STATUS to LISTENING in the Participant instance associated with us, causing any input from our microphone to be ignored. This is a Conversation class method, rather than Participant class, because this only applies to local participant. <br>
		 */
		public void MuteMyMicrophone() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,11);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Sets VOICE_STATUS to SPEAKING in the Participant instance associated with us, causing any input from our microphone to be sent to the call host. This is a Conversation class method, rather than Participant class, because this only applies to local participant. <br>
		 */
		public void UnmuteMyMicrophone() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,12);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Puts the conversation on hold - Conversation LOCAL_LIVESTATUS changes to ON_HOLD_LOCALLY and to ON_HOLD_REMOTELY for remote participants. <br>
		 */
		public void HoldMyLiveSession() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,13);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Resumes call from local hold. <br>
		 */
		public void ResumeMyLiveSession() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,14);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Hang up or refuse to answer an incoming call. Set postVoiceAutoresponse to true to enable a caller to leave a voicemail message. <br>
		 * @param postVoiceAutoresponse
		 */
		public void LeaveLiveSession( boolean postVoiceAutoresponse) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,15);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('b',1,postVoiceAutoresponse);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Begin recording a voice mail for this conversation's remote participant. Applies to conversations of type DIALOG only. <br>
		 */
		public void StartVoiceMessage() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,45);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *This method is for doing call transfers. NB! Call transfers only work in one-on-one conversations (dialogs). Before attempting call transfer, you should check availability of transfer recipients with Conversation class CanTransferLiveSession method. If the current conversation has a live session up, that session (call) will be transferred to targets specified in the identities list. Note that identities is a string list - all identities in that list will get incoming calls. The first one of them to pick up the call - wins, and rest of the transfer targets will stop ringing. <br><br>Let's take a closer look how this works in practice. We have three call participants involved in the process, and two separate conversations. Let there be three callers: Caller A (call originator), Caller B (transferor) and Caller C (recipient of transfer). <br><br> - Caller A - calls Caller B; Caller B picks up the call - live conversation C1 is now up with A and B in it. <br> - After awhile, Caller B initiates call transfers to Caller C (and optionally to Callers D, E, F.. ). LOCAL_LIVESTATUS of C1 will get set to TRANSFERRING for both A and B. <br> - Caller C picks up the call. Conversation C1 will go off live status. For Caller B, conversation C1 LOCAL_LIVESTATUS will change to RECENTLY_LIVE. Another live conversation - C2 gets spawned, with Caller A and Caller C in it. For caller C, participant object representing caller A will have TRANSFERRED_BY property set to identity of caller A. For Caller B (in now no longer live conversation C1), participant object representing caller A gets its TRANSFERRED_TO property set to identity of caller C. <br>
		 * @param identities String list of transfer target identities. As soon as first one in this list picks up the call, others will stop ringing. <br>
		 * @param transferTopic Optional conversation topic. This value will get set as META_TOPIC property of the conversation at the transferee end. Note that this is the only case where META_TOPIC field is used in context of dialog conversations. Thus assumption that remote UI will display topic field in case of dialogs may not be 100% correct. <br>
		 */
		public void TransferLiveSession( String [] identities, String transferTopic) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,40);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addListStart(1);
			for (int i=0;i<identities.length;i++) {
				request.addParm('S',identities[i]);
			}
			request.addParm('S',2,transferTopic);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Checks if the identity is available for receiving a transferred live session. If you are going to attempt to go for multiple transfer targets, you should use this check for all the target identities. <br>
		 * @param identity Target identity. <br>
		 * @return result Returns true if call transfer to given target is possible. <br>
		 */
		public boolean CanTransferLiveSession( String identity) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,46);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,identity);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return false;
				
			boolean result = false;
			result = r.GetAsBoolean(1);
			return result;
		}
		
		/**
		 *Sends DTMF tone to a live conversation. <br>
		 * @param dtmf Outgoing dtmf tone, possible values come from Participant.DTMF enumerator. <br>
		 * @param lengthInMS Duration in milliseconds. Defaults to 260 ms. Note that the DTMF tone can be also cancelled with Conversation.StopSendDTMF method. <br>
		 */
		public void SendDTMF( Participant.DTMF dtmf, int lengthInMS) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,16);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,dtmf.getId());
			request.addParm('u',2,lengthInMS);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Stops the current DTMF tone being played into conversation. For example, use this method to cancel DTMF signals started with Conversation.SendDTMF before the duration given in lengthInMS runs out. <br>
		 */
		public void StopSendDTMF() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,48);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Sets local user typing indicator in the Conversation. Remote Participants can display these in their UI. <br>
		 * @param status Typing indicator status value - Participant.TEXT_STATUS <br>
		 */
		public void SetMyTextStatusTo( Participant.TEXT_STATUS status) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,18);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,status.getId());
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Posts the specified text the conversation, and populates message with a reference to the corresponding Message object (if no error occurred during execution). The isXML argument can be used if the client UI has already taken care of converting message text to xml (for example, your UI might enable users to use bold tags in text messages.) <br>
		 * @param text Text value of the outgoing message (gets set as BODY_XML property of the Message object). <br>
		 * @param isXML For cases where the text argument was already encoded as xml message. <br>
		 * @return message Returns the Message object created as a result of this method (if successful). <br>
		 */
		public Message PostText( String text, boolean isXML) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,19);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,text);
			request.addParm('b',2,isXML);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			int oid = 0;
			Message message = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) {
				message = (Message)skype.factory(Message.moduleID(), oid, skype);
			}
			return message;
		}
		
		/**
		 *Takes a list of Contacts as an argument and posts the list into the Conversation. The purpose of this feature is to enable sharing contacts between users, without resorting to contact search. Instead, if user A has contacts B and C, he can post contact C into chat with contact B. At this point, Contact B can add contact C to his contact list. From remote side, the posted contacts will appear as messages with type Message.POSTED_CONTACTS appearing in the conversation. The UI should allow adding these contacts from messages with this type into the contact list. <br><br>The list of posted contacts can be retrieved with the Message.GetContacts method.  <br><br>Additionally, the UI then can parse the posted Contact data out of the Message.P_BODY_XML property. The list of contacts is wrapped between <contacts ..> </contacts> tags. Each contact item in the xml has following format: <br> - t - contact type. "s" - skype contact; "p" - phone number; <br> - s - skypename, present only in skypename contacts (t="s") <br> - p - phone number, present only in phone number contacts (t="p") <br> - f - contact's full name, if available <br> - d - contact's display name, if available <br><br>Note that only the type (t) field is mandatory. Depending on type, either skypename (s) or phone number (p) fields are always present. Full name and display name fields are optional. <br><br>Example BODY_XML with skypname contact:  <br>@code <br><contacts alt="alt text"><c t="s" s="skypename" f="full name"/></contacts> <br></CODE> <br><br>Example BODY_XML with PSTN contact:  <br>@code <br><contacts alt="alt text"><c t="p" p="+37212345678" d="Some PSTN number"/></contacts> <br></CODE>  <br><br>Example BODY_XML with multiple contacts:  <br>@code <br><contacts alt="alt text"><c t="p" p="+37212345678" d="Some PSTN number"/><c t="s" s="someskypename"/></contacts> <br></CODE> <br>
		 * @param contacts List of Contact objects, to be posted in the conversation. <br>
		 */
		public void PostContacts( Contact [] contacts) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,20);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addListStart(1);
			for (int i=0;i<contacts.length;i++) {
				request.addParm('O',contacts[i].getOid());
			}
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Takes a list of fully-qualified filenames and initiates corresponding file transfers in the conversation. From the remote side, incoming file transfers will appear as a conversation message with type POSTED_FILES. Once such a message is detected, the list of file transfer objects can be retrieved with Message.GetTransfers. At that point, remote participants will need to accept or decline those transfers. <br>
		 * @param paths list of fully-qualified filenames to be transferred <br>
		 * @param body Optional BODY_XML property for POSTED_FILES type messages that show up in remote UI. <br>
		 * @return PostFilesResult
		 */
		public PostFilesResult PostFiles( String [] paths, String body) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,21);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addListStart(1);
			for (int i=0;i<paths.length;i++) {
				request.addParm('f',paths[i]);
			}
			request.addParm('S',2,body);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			PostFilesResult result = new PostFilesResult();
			
			Skype.TRANSFER_SENDFILE_ERROR error_code = null;
			error_code = Skype.TRANSFER_SENDFILE_ERROR.get(r.GetAsInt(1));
			result.error_code = error_code;
			
			String error_file = null;
			error_file = r.GetAsString(2);
			result.error_file = error_file;
			
			return result;
		}
		
		public class PostFilesResult {
			public Skype.TRANSFER_SENDFILE_ERROR error_code; /** Error code, possible values come from the TRANSFER_SENDFILE_ERROR enumerator. This will be set for the first failed fail. The failed file is identified in the error_file return argument. <br> */
			public String error_file; /** Filename of the file that triggered error. <br> */
		}
		
		/**
		 * Stop recording a voice mail for this conversation's remote participant and send it. Applies to conversations of type DIALOG only. <br>
		 *Stops the active voicemail recording and sends it (dialog only)
		 * @param voicemail This argument is deprecated as of SDK version 3.2. Instead of manually constructing Voicemail object, you can call Conversation.StartVoiceMessage method to start recording a voicemail in context of a dialog. PostVoiceMessage will stop recording this voicemail and post it in the dialog. If instead of sending Voicemail, the user decides to cancel it, you should use Conversation.LeaveLiveSession method (Voicemail.Cancel is deprecated). <br>
		 * @param body Optional text message that remote UI can display in conversation, to notify the user of a new voicemail. <br>
		 */
		public void PostVoiceMessage( Voicemail voicemail, String body) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,22);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,voicemail.getOid());
			request.addParm('S',2,body);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Takes an SMS instance created by Skype.CreateOutgoingSms and posts it in the conversation. Note that you will need to set both Sms body text (Sms.SetBody) and recipient list (Sms.SetTargets) before you can post the object. <br>
		 * @param sms SMS object. <br>
		 * @param body This argument is currently ignored. The message text needs to be set with Sms.SetBody method, prior to passing the Sms object to this method <br>
		 */
		public void PostSMS( Sms sms, String body) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,23);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,sms.getOid());
			request.addParm('S',2,body);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Retrieves a binary join blob for joining public conversations, which are always of type CONFERENCE. If called for a dialog, the blob argument will contain the empty string. The best way to create a Public Chat is to first create a fresh conversation with Skype class CreateConference, then minimally apply the public chat options OPT_JOINING_ENABLED and OPT_ENTRY_LEVEL_RANK - options, like this (C++):  <br>@code <br>C->SetOption(Conversation.OPT_JOINING_ENABLED, true); <br></CODE> <br><br>When that is done, you can call GetJoinBlob to retrieve the blob string. Use the blob string to generate and post an HTML link whose href attribute looks like this: href="skype:?chat&blob=_BLOB_GOES_HERE" A person running Skype desktop client can click this link to join the conversation and have that conversation opened in his UI. Note that the conversation host (creator) needs to be online for new joiners-via-link to start participating in the Public Chat.
		 * @return blob Returns the public conversation join blob. <br>
		 */
		public String GetJoinBlob() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,24);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			String blob = null;
			blob = r.GetAsString(1);
			return blob;
		}
		
		/**
		 *Tries to join a public conversation (aka public chat). This method is only useful if you have used Skype.GetConversationByBlob method with alsoJoin argument set to false. <br>
		 */
		public void Join() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,25);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Submits password for joining password-protected conversation. <br>
		 * @param password Password string. <br>
		 */
		public void EnterPassword( String password) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,26);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,password);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Sets password protection/new password for the conversation. <br>
		 * @param password New password. <br>
		 * @param hint Password hint. <br>
		 */
		public void SetPassword( String password, String hint) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,27);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,password);
			request.addParm('S',2,hint);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Leaves the conference. Not applicable to dialogs. <br>
		 */
		public void RetireFrom() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,28);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Deletes this conversation, which must be of type CONFERENCE - dialogs between local user and any of his contacts are always persistant. Note that this also removes corresponding Message and Participant objects. <br>
		 */
		public void Delete() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,47);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Changes the META_NAME property of the conversation. Note that unlike topic and guidelines, this rename is just local - remote participants can have their own private names for conversations. <br>
		 * @param name New name for the conversation. Passing an empty string in this argument causes the META_NAME to unset. <br>
		 */
		public void RenameTo( String name) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,29);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,name);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Setter for Conversation class IS_BOOKMARKED. <br>
		 * @param bookmark Set true to set the bookmark, false to remove the bookmark. <br>
		 */
		public void SetBookmark( boolean bookmark) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,30);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('b',1,bookmark);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Setter for Conversation class ALERT_STRING property. The main use of this property is checking bodies of incoming messages in the conversation for the alert string and producing notifications in UI for the user, when appropriate. <br>
		 * @param alertString Substring to check in BODY_XML property of incoming messages. <br>
		 */
		public void SetAlertString( String alertString) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,31);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,alertString);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Removes conversation from Inbox. <br>
		 */
		public void RemoveFromInbox() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,32);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Sets Conversation inbox_timestamp property. If the timestamp argument is left empty or is greater than conversation consumption horizon, then the conversation will be restored to the inbox. <br>
		 * @param timestamp If left empty or set to 0, the inbox_timestamp property is set to current time. <br>
		 */
		public void AddToInbox( int timestamp) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,33);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('u',1,timestamp);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *This method can be used to set the consumption (read) status of messages in the conversation. It sets Message.CONSUMPTION_STATUS to Message.CONSUMED for all messages in the conversation, older than the given timestamp. If the second argument is set to true, it also modifies messages more recent than the timestamp, by marking them as unconsumed. <br>
		 * @param timestamp Consumption cutoff timestamp. Setting this to current time will mark all messages in the conversation as consumed. <br>
		 * @param also_unconsume If set to true, this also marks messages newer than the cutoff timestamp as unconsumed. For example, setting timestamp to 0 and also_unconsumed to true, will unconsume all messages in the conversation. <br>
		 */
		public void SetConsumedHorizon( int timestamp, boolean also_unconsume) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,34);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('u',1,timestamp);
			request.addParm('b',2,also_unconsume);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *sets consumption horizon to last inbox message id timestamp
		 */
		public void MarkUnread() {
		
			Request request = null;
			try {
				request = new XCallRequest(18,35);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Checks if the conversation is a member of the given ContactGroup <br>
		 * @param group ContactGroup <br>
		 * @return result True if this conversation is a member of the ContactGroup specified by the group argument contains the conversation <br>
		 */
		public boolean IsMemberOf( ContactGroup group) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,37);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,group.getOid());
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return false;
				
			boolean result = false;
			result = r.GetAsBoolean(1);
			return result;
		}
		
		/**
		 */
		public enum PARTICIPANTFILTER {
		
			/** All participants (may included some that are RETIRED or OUTLAW, but not all of them)*/
			ALL(0),
			
			/** Participants that can receive messages, including myself*/
			CONSUMERS(1),
			
			/** Only people who are applying to join the conversation*/
			APPLICANTS(2),
			
			/** Consumers and applicants*/
			CONSUMERS_AND_APPLICANTS(3),
			
			/** Myself*/
			MYSELF(4),
			
			/** All consumers except myself*/
			OTHER_CONSUMERS(5);
			
			private static final Map<Integer,PARTICIPANTFILTER> lookup = new HashMap<Integer,PARTICIPANTFILTER>();
			
			static {
				for(PARTICIPANTFILTER s : EnumSet.allOf(PARTICIPANTFILTER.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private PARTICIPANTFILTER(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static PARTICIPANTFILTER get(int code) {
				return lookup.get(code);
			}
			
			public static PARTICIPANTFILTER fromString(String s) {
				for (PARTICIPANTFILTER p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *Retrieves the list of this conversation's current participants, which you can optionally request to be filtered. If no Participants pass the filter, an empty list will be returned (the method itself still returns true). <br>
		 * @param filter Conversation.PARTICIPANTFILTER - defaults to Conversation.ALL <br>
		 * @return participants List of conversation Participant objects that passed the filter. <br>
		 */
		public Participant [] GetParticipants( Conversation.PARTICIPANTFILTER filter) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,38);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,filter.getId());
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			Vector<Participant> participants = new Vector<Participant>();
			while (r.HasMore(1))
			{
				int oid = 0;
				Participant participant = null;
				oid = r.GetOid(1);
				if (oid != AbstractDecoder.NULL_VALUE) { 
					participant = (Participant)skype.factory(Participant.moduleID(), oid, skype);
				}
				participants.add(participant);
			}
			return participants.toArray(new Participant[participants.size()]);
			
		}
		
		/**
		 *Returns recent messages. The messages are returned in two lists - new messages (unconsumed) and recent message history (context messages). The context message list contains messages that are already above the consumption horizon but are fairly recent, making it likely that displaying them in UI would be good default behaviour. <br>
		 * @param requireTimestamp If set to a non-zero value, includes messages no earlier than this timestamp, if not, includes messages from the last 24 hours only <br>
		 * @return GetLastMessagesResult
		 */
		public GetLastMessagesResult GetLastMessages( int requireTimestamp) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,39);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('u',1,requireTimestamp);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			GetLastMessagesResult result = new GetLastMessagesResult();
			
			Vector<Message> contextMessages = new Vector<Message>();
			while (r.HasMore(1))
			{
				int oid = 0;
				Message message = null;
				oid = r.GetOid(1);
				if (oid != AbstractDecoder.NULL_VALUE) { 
					message = (Message)skype.factory(Message.moduleID(), oid, skype);
				}
				contextMessages.add(message);
			}
			result.contextMessages = contextMessages.toArray(new Message[contextMessages.size()]);
			
			Vector<Message> unconsumedMessages = new Vector<Message>();
			while (r.HasMore(2))
			{
				int oid = 0;
				Message message = null;
				oid = r.GetOid(2);
				if (oid != AbstractDecoder.NULL_VALUE) { 
					message = (Message)skype.factory(Message.moduleID(), oid, skype);
				}
				unconsumedMessages.add(message);
			}
			result.unconsumedMessages = unconsumedMessages.toArray(new Message[unconsumedMessages.size()]);
			
			return result;
		}
		
		public class GetLastMessagesResult {
			public Message [] contextMessages; /** Already consumed messages, provided for context */
			public Message [] unconsumedMessages; /** Unconsumed messages */
		}
		
		/**
		 *Finds the most recent Message object in the conversation that contains the substring specified by the text argument. If no matching messages are found, this method will return false. The search proceeds backwards in time, starting from the timestamp argument. To continue searching, you can start with timestamp=MAX_UINT, retrieve the TIMESTAMP property of the matching message, decrement it by one, and submit it as timestamp for the next FindMessage call. <br>
		 * @param text Substring to search for. <br>
		 * @param fromTimestampUp
		 * @return message Returns matching message or 0 if there was no match. As the likelihood of this object being invalid is quite high, you should always check for method return value before you start calling methods of this object. <br>
		 */
		public Message FindMessage( String text, int fromTimestampUp) {
		
			Request request = null;
			try {
				request = new XCallRequest(18,41);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,text);
			request.addParm('u',2,fromTimestampUp);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			int oid = 0;
			Message message = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) {
				message = (Message)skype.factory(Message.moduleID(), oid, skype);
			}
			return message;
		}
		
		/**
		 */
		public enum LIST_TYPE {
		
			/** bookmarked or in_inbox or live or with_meta_info or activity in last 30 days*/
			ALL_CONVERSATIONS(0),
			
			/** only last 6 months conversations are kept there*/
			INBOX_CONVERSATIONS(1),
			
			/** is_bookmarked is set*/
			BOOKMARKED_CONVERSATIONS(2),
			
			/** local_livestatus is different from NONE*/
			LIVE_CONVERSATIONS(3),
			
			/** all conversations, without any of the limits of ALL_CONVERSATIONS*/
			REALLY_ALL_CONVERSATIONS(5);
			
			private static final Map<Integer,LIST_TYPE> lookup = new HashMap<Integer,LIST_TYPE>();
			
			static {
				for(LIST_TYPE s : EnumSet.allOf(LIST_TYPE.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private LIST_TYPE(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static LIST_TYPE get(int code) {
				return lookup.get(code);
			}
			
			public static LIST_TYPE fromString(String s) {
				for (LIST_TYPE p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
	
	}