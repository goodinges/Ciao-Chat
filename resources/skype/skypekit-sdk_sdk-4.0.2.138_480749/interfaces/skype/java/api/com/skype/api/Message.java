package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * Events in a conversation context are expressed as Messages. It is therefore useful to think of Message objects as events, rather than specifically text chat messages. <br><br>Message member functions all return a Boolean indicating the success (true) or failure (false) of processing the request itself (transport, runtime availability, and so forth) - not the success or failure of its associated functionality. For example, Message.Edit returns true if it was able to make a determination, and its result parameter reflects whether this Message can be edited. Similarly, Message.Edit returns false if it was unable to make a determination, and the value of its result parameter is undefined. <br><br>Message member functions that are specific to a Message TYPE return false if this Message is not of that type. For example, Message.GetVoiceMessage will return false if this Message's type is not POSTED_VOICE_MESSAGE. <br><br>The actual meaning of a Message can be determined by its P_TYPE property. The meanings of most other Message properties depend on the value of P_TYPE. For example, let's take P_BODY_XML property. <br><br>Following messages have a text entered by the user as a body. It may contain emoticons, URLs, etc. <br> - POSTED_TEXT <br> - POSTED_EMOTE <br> - SET_METADATA <br> - REQUESTED_AUTH <br><br>Following messages have a custom XML format for the body (see the specific section on these message types for details): <br> - POSTED_CONTACTS <br> - POSTED_VOICE_MESSAGE <br> - POSTED_FILES <br> - POSTED_SMS <br> - STARTED_LIVESESSION and ENDED_LIVESESSION (same format) <br><br>Following messages do not use the body property: <br> - SPAWNED_CONFERENCE <br> - ADDED_CONSUMERS <br> - ADDED_APPLICANTS <br> - RETIRED_OTHERS <br> - RETIRED <br> - SET_RANK <br> - HAS_BIRTHDAY <br> - GRANTED_AUTH <br> - BLOCKED <br><br>Messages such as POSTED_TEXT use a small subset of a HTML-like markup to control the visual representation of the text. This markup is used by POSTED_TEXT and POSTED_EMOTE, but also for the conversation topic (CONVERSATION_META_TOPIC property and the body of the SET_METADATA message) and for authorization requests. <br><br>Having chat messages in XML format means that all formatting is indicated by XML tags. This includes emoticons and URls. The big advantage is that it makes the parsing of the message by the UI much easier. The UI does not need to do emoticons or URL detection, this is already done and it only needs to look for the XML tags. <br><br>For text messages, it is possible for the UI to simply ignore (meaning strip) the XML and the message will be understandable fine, it will only have lost some formatting. <br><br>But it is obviously nicer to display at least the most commonly used tags. <br><br>To strip the XML: <br> - if they have the alt="sometext" attribute set, return sometext as the output of that tag and ignore the rest of tag and all nested sub tags <br> - if no alt="" attribute set, use tag content as output - <sometag>hereissomething</sometag> is output as hereissomething<br> - if no alt="" and no tag content, ignore the tag altogether (return nothing) <br>Skype for Windows supports displaying many XML tags, but only a sub-set is regularly used and should be supported by the UI for a good experience. These are the ones described here. <br>Animated emoticons <br>Emoticons are encoded with the "ss" tag. The element content is the plain text representation. It has a "type" attribute indicating the emoticons canonical name. Example:  <br>@code <br>Hi <ss type="smile">:-)</ss>  <br></CODE> <br><br>Flag emoticons <br>Flag emoticons are little flags. They are encoded with the "flag" tag. The element contents is the plain text representation and it has a "country" attribute which is a 2-letter ISO-3166 country code. The user can enter a flag using "(flag:XX)", where XX is a valid ISO country code. Example:  <br>@code <br>I am in <flag country="cc">CC</flag>  <br></CODE> <br><br>Links <br>If the library detects a URL, it will encode it using the html "a" tag, with the "href" attribute indicating the URL. The plain text representation is what the user originally typed. Example:  <br>@code <br>I am in <a href="http://wwww.skype.com">www.skype.com</a> <br></CODE> <br><br>Alert matches <br>When a conversation is configured to display only alerts if specific words are present in the message (see "/alertson [text to match]" command), if a message matches the alert, it will be marked with the <alertmatch> tag. This allows the UI to highlight the word matching. Example:  <br>@code <br>Maybe <alertmatch>Vincent</alertmatch> knows the answer  <br></CODE> <br><br>Bold, italic, etc <br>Skype for Windows also supports displaying bold and italic text, using the "b" and "i" tags. <br><br>Encoding messages <br>When sending a chat message via PostText(), there is the possibility to indicate if the library should do the XML encoding, or if the message has already been encoded. Usually, the UI can let library do the encoding. This is the case when the message does not contain any specific formatting. It may contain emoticons or URls, which will be detected by the library encoder and converted into XML tags. <br>If the message has some more complex encoding, such as a quote or some bold text, it is up to the UI to encode the message. <br>
 */


public class Message extends SkypeObject {


	public interface MessageListener {
		/** This event gets called when there are changes to Message properties defined in Message.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
	}
	
	public Message(int oid, Skype skype) {
		super(oid,skype);
		/**get default properties for this module **/
		GetDefaultProps();
	}
	
	private static final int MODULE_ID = 9;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the Message class */
	public enum PROPERTY {
	
		/** DB ID of corresponding conversation, type: Conversation */
		convo_id(960),
		
		/** GUID of the Conversation. The GUID is a "global ID" - these values are shared accross Skype client instances and accross all the participants of the conversation. <br>, type: String */
		convo_guid(120),
		
		/** Identity of the sender. While this is almost always the same as SKYPENAME property of the Contact, in some rare cases it can also be a phone number - for example, incoming voicemail notification Messages (message type = POSTED_VOICE_MESSAGE). <br>, type: String */
		author(122),
		
		/** displayname of the sender at the time of posting, type: String */
		author_displayname(123),
		
		/** Unlike the message id, the GUID is the same on all instances and for all participants. <br>, type: byte[] */
		guid(792),
		
		/** This property gets set when a conference is spawned from dialog Conversation. In that case recent message history from the original dialog is copied to the target conversation. For all the copied messages, the ORIGINALLY_MEANT_FOR property will be set to identity of the remote participant of the original dialog. <br>, type: String */
		originally_meant_for(790),
		
		/** UNIX timestamp (sent time, adjusted for local clock), type: int */
		timestamp(121),
		
		/** type: Message.TYPE */
		type(961),
		
		/** type: Message.SENDING_STATUS */
		sending_status(962),
		
		/** type: Message.CONSUMPTION_STATUS */
		consumption_status(968),
		
		/** Identity of the author that last edited this message. NULL if message has not been edited <br>, type: String */
		edited_by(222),
		
		/** UNIX timestamp of last edit, type: int */
		edit_timestamp(223),
		
		/** Message type-specific parameter. See Message.SET_METADATA_KEY for more information. <br>, type: int */
		param_key(963),
		
		/** Message type-specific parameter <br>, type: int */
		param_value(964),
		
		/** Message type-specific parameter <br>, type: String */
		body_xml(127),
		
		/** Message type-specific parameter. Depending of Message type, this property contains: <br> - STARTED_LIVESESSION - list of participants in the cal; <br> - ENDED_LIVESESSION - list of participants in the call; <br> - POSTED_SMS - list of recipients of the message; <br> - SPAWNED_CONFERENCE - the list of identities that were added; <br> - ADDED_CONSUMERS - the list of identities that were added; <br> - RETIRED_OTHERS - the skypename of the participant who was kicked; <br> - SET_RANK - the skypename of the participant whose rank was changed; <br> - REQUESTED_AUTH - Message.P_AUTHOR and Message.P_IDENTITIES are set to the users receiving and requesting the authorization, depending if the message was received or sent; <br> - GRANTED_AUTH - the skypename of the user we granted authorization; <br> - BLOCKED - the skypename of the user who was blocked; <br> - HAS_BIRTHDAY - skypename of current logged in user. <br>, type: String */
		identities(125),
		
		/** Message type-specific parameter. Possible values for STARTED/ENDED_LIVESESSION (only set for dialogs): <br> - no_answer  <br> - manual  <br> - busy  <br> - connection_dropped <br> - no_skypeout_subscription; <br> - insufficient_funds <br> - internet_connection_lost <br> - skypeout_account_blocked <br> - pstn_could_not_connect_to_skype_proxy <br> - pstn_invalid_number <br> - pstn_number_forbidden <br> - pstn_call_timed_out <br> - pstn_busy <br> - pstn_call_terminated <br> - pstn_network_error <br> - number_unavailable <br> - pstn_call_rejected <br> - pstn_misc_error <br> - internal_error <br> - unable_to_connect <br> - connection_dropped <br> - recording_failed <br> - playback_error <br> - legacy_error <br> - blocked_by_privacy_settings <br> - error <br> - transfer_failed <br> - transfer_insufficient_funds <br> - blocked_by_us <br> - emergency_call_denied <br><br>This information is now available as an enum in LEAVEREASON <br>, type: String */
		reason(966),
		
		/** Leave reason for message of the RETIRED type, and STARTED/ENDED_LIVESESSION.                   Use for STARTED/ENDED_LIVESESSION is to provide simpler, enum based                   handling and deprecates the reason property (only set for dialogs), type: LEAVE_REASON */
		leavereason(126),
		
		/** Number of people who received this message (including local user) <br>, type: int */
		participant_count(982);
		
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
		GetPropertyRequest request = new GetPropertyRequest(9, mObjectId, prop.id);
		
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
		GetPropertyRequest request = new GetPropertyRequest(9, mObjectId, prop.id);
		
		byte [] data = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null) {
			data = r.GetAsBinary();
		}
		return data;
	}
	
	/**default array of Message Properties that get fetched & cached upon class construction*/
	private static PROPERTY [] defaultProperties = { PROPERTY.convo_id, PROPERTY.type, PROPERTY.author, PROPERTY.author_displayname, PROPERTY.identities, PROPERTY.timestamp, PROPERTY.body_xml, PROPERTY.sending_status, PROPERTY.consumption_status};
	
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
		The P_TYPE property determines the actual meaning of the Message object. Only Messages of POSTED_TEXT type contain actual text messages. The meaning and content of the rest of the message properties are largely dependant of the value of the Message.P_TYPE. <br> */
		public enum TYPE {
		
			/** Conference metadata were changed*/
			SET_METADATA(2),
			
			/** A conference was spawned from this dialog*/
			SPAWNED_CONFERENCE(4),
			
			/** Some users were added to the conference*/
			ADDED_CONSUMERS(10),
			
			/** Some users are applying to be added to the conference*/
			ADDED_APPLICANTS(11),
			
			/** User was kicked from the conference*/
			RETIRED_OTHERS(12),
			
			/** User left the conference*/
			RETIRED(13),
			
			/** Changed the rank of a user in the Conversation (multichat administration) <br>*/
			SET_RANK(21),
			
			/** A live session started*/
			STARTED_LIVESESSION(30),
			
			/** A live session ended*/
			ENDED_LIVESESSION(39),
			
			/** User requested authorization*/
			REQUESTED_AUTH(50),
			
			/** User was granted authorization. Notification message that user is now an authorized contact (of the local user). <br>*/
			GRANTED_AUTH(51),
			
			/** User was blocked*/
			BLOCKED(53),
			
			/** A text message*/
			POSTED_TEXT(61),
			
			/** An emote ('John Doe is laughing', cf /me chat command)*/
			POSTED_EMOTE(60),
			
			/** The message represents (a set of) contact card(s) posted in the conversation. One message can contain more than one contact cards. The contacts can be retrieved from the message by parsing them out from the P_BODY_XML property. For more information, see Conversation.PostContacts <br>*/
			POSTED_CONTACTS(63),
			
			/** The message represents an SMS object that was posted in the Conversation. See Conversation.PostSMS for more details. The Sms object itself can be retrieved from the Message with Message.GetSms The message BODY_XML contains a set of SMS properties, such as status, failurereason, targets, price and timestamp. <br>*/
			POSTED_SMS(64),
			
			/** Deprecated, never sent*/
			POSTED_ALERT(65),
			
			/** A voicemail*/
			POSTED_VOICE_MESSAGE(67),
			
			/** The message represents a (list of) file transfers that were posted in the Conversation with Conversation.PostFiles. Transfer objects can be retrieved from the Message with Message.GetTransfers <br>*/
			POSTED_FILES(68),
			
			/** Currently unused. <br>*/
			POSTED_INVOICE(69),
			
			/** The message represents a Contact birthday notification. <br>*/
			HAS_BIRTHDAY(110);
			
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
		public enum SENDING_STATUS {
		
			/** Message has not been delivered to at least one of the participants <br>*/
			SENDING(1),
			
			/** Message has been delivered to at least one other participant <br>*/
			SENT(2),
			
			/** Message could not be delivered (for SMS this reflects the actual SMS, not the chat message) <br>*/
			FAILED_TO_SEND(3);
			
			private static final Map<Integer,SENDING_STATUS> lookup = new HashMap<Integer,SENDING_STATUS>();
			
			static {
				for(SENDING_STATUS s : EnumSet.allOf(SENDING_STATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private SENDING_STATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static SENDING_STATUS get(int code) {
				return lookup.get(code);
			}
			
			public static SENDING_STATUS fromString(String s) {
				for (SENDING_STATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Indicates if a message has been consumed (meaning read) or not */
		public enum CONSUMPTION_STATUS {
		
			/** Message has been read. Note that this is a read-only property. Consumption status of individual messages can not be set selectively. Message consumption status is determined at the conversation level, based conversation consumption horizon and individual message timestamps. Conversation consumption horizon can be updated with Conversation.SetConsumedHorizon method.  <br>*/
			CONSUMED(0),
			
			/** Do not notify the user that they have this unread message <br>*/
			UNCONSUMED_SUPPRESSED(1),
			
			/** Notify the user that they have this unread message <br>*/
			UNCONSUMED_NORMAL(2),
			
			/** This message consumption state is marked as DEPRECATED <br>*/
			UNCONSUMED_ELEVATED(3);
			
			private static final Map<Integer,CONSUMPTION_STATUS> lookup = new HashMap<Integer,CONSUMPTION_STATUS>();
			
			static {
				for(CONSUMPTION_STATUS s : EnumSet.allOf(CONSUMPTION_STATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private CONSUMPTION_STATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static CONSUMPTION_STATUS get(int code) {
				return lookup.get(code);
			}
			
			public static CONSUMPTION_STATUS fromString(String s) {
				for (CONSUMPTION_STATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		For messages of type SET_METADATA that alert participants to changes to the associated Conversation's metadata, indicates which metadata property changed and its P_BODY_XML property contains the changed data. Your UI is expected to detect messages with PARAM_KEY set and to update its visual representation of Conversation accordingly. <br>You can use the associated Conversation's properties and methods to obtain the updated metadata rather than parse the message body XML, for example, Conversation.P_META_PICTURE and Conversation.Conversation.GetPropMetaPicture. <br> */
		public enum SET_METADATA_KEY {
		
			/** Notification message that conversation name has changed. <br>*/
			SET_META_NAME(3640),
			
			/** Notification message that conversation topic has changed. <br>*/
			SET_META_TOPIC(3644),
			
			/** Notification message that conversation guidelines have changed. <br>*/
			SET_META_GUIDELINES(3652),
			
			/** Notification message that conversation picture has changed. <br>*/
			SET_META_PICTURE(3658);
			
			private static final Map<Integer,SET_METADATA_KEY> lookup = new HashMap<Integer,SET_METADATA_KEY>();
			
			static {
				for(SET_METADATA_KEY s : EnumSet.allOf(SET_METADATA_KEY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private SET_METADATA_KEY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static SET_METADATA_KEY get(int code) {
				return lookup.get(code);
			}
			
			public static SET_METADATA_KEY fromString(String s) {
				for (SET_METADATA_KEY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Indicates the reason a user could not join or left a Conversation. SkypeKit automatically sets "could not join"-related values. "Left voluntarily"-related values are set as a result of explicit user actions. <br> */
		public enum LEAVEREASON {
		
			/** User cannot chat (user is currently logged in with a client that has chat disabled - see Contact.CAPABILITY.CAPABILITY_TEXT) <br>*/
			USER_INCAPABLE(2),
			
			/** Attempt to add local user to a conversation by an unknown contact <br>*/
			ADDER_MUST_BE_FRIEND(3),
			
			/** Attempt to add local user to a conversation by an unauthorized contact <br>*/
			ADDER_MUST_BE_AUTHORIZED(4),
			
			/** Local user declined an "invitation" to join a chat <br>*/
			DECLINE_ADD(5),
			
			/** User decided to end participation in an on-going multi-chat <br>*/
			UNSUBSCRIBE(6);
			
			private static final Map<Integer,LEAVEREASON> lookup = new HashMap<Integer,LEAVEREASON>();
			
			static {
				for(LEAVEREASON s : EnumSet.allOf(LEAVEREASON.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private LEAVEREASON(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static LEAVEREASON get(int code) {
				return lookup.get(code);
			}
			
			public static LEAVEREASON fromString(String s) {
				for (LEAVEREASON p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *For Message types having a body, determines whether that body is editable by the user.  <br>
		 * @return result
		 */
		public boolean CanEdit() {
		
			Request request = null;
			try {
				request = new XCallRequest(9,1);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return false;
				
			boolean result = false;
			result = r.GetAsBoolean(1);
			return result;
		}
		
		/**
		 *For Message types that include a body and are editable:  <br> - alters BODY_XML of the message object <br> - sets EDITED_BY and EDIT_TIMESTAMP properties  <br> - propagates the changes to remote users. <br>
		 * @param newText New value of the message BODY_XML property. <br>
		 * @param isXML Specify isXML as true if the message body is formatted as XML; omit it or specify it as false if the message body is plain text. <br>
		 * @param undo Reverts the message body to the original version. newText argument is ignored when this is set. <br>
		 */
		public void Edit( String newText, boolean isXML, boolean undo) {
		
			Request request = null;
			try {
				request = new XCallRequest(9,2);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,newText);
			request.addParm('b',2,isXML);
			request.addParm('b',3,undo);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *For messages of type POSTED_CONTACTS, parses the body XML and formats the data as a list of Contact instances. <br>
		 * @return contacts
		 */
		public Contact [] GetContacts() {
		
			Request request = null;
			try {
				request = new XCallRequest(9,3);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			Vector<Contact> contacts = new Vector<Contact>();
			while (r.HasMore(1))
			{
				int oid = 0;
				Contact contact = null;
				oid = r.GetOid(1);
				if (oid != AbstractDecoder.NULL_VALUE) { 
					contact = (Contact)skype.factory(Contact.moduleID(), oid, skype);
				}
				contacts.add(contact);
			}
			return contacts.toArray(new Contact[contacts.size()]);
			
		}
		
		/**
		 *For messages of type POSTED_FILES, parses the body XML and creates a list of Transfer instances. <br>
		 * @return transfers
		 */
		public Transfer [] GetTransfers() {
		
			Request request = null;
			try {
				request = new XCallRequest(9,4);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			Vector<Transfer> transfers = new Vector<Transfer>();
			while (r.HasMore(1))
			{
				int oid = 0;
				Transfer transfer = null;
				oid = r.GetOid(1);
				if (oid != AbstractDecoder.NULL_VALUE) { 
					transfer = (Transfer)skype.factory(Transfer.moduleID(), oid, skype);
				}
				transfers.add(transfer);
			}
			return transfers.toArray(new Transfer[transfers.size()]);
			
		}
		
		/**
		 *For messages of type POSTED_VOICE_MESSAGE, parses the body XML and creates a Voicemail instance. <br>
		 * @return voicemail
		 */
		public Voicemail GetVoiceMessage() {
		
			Request request = null;
			try {
				request = new XCallRequest(9,5);
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
			Voicemail voicemail = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) {
				voicemail = (Voicemail)skype.factory(Voicemail.moduleID(), oid, skype);
			}
			return voicemail;
		}
		
		/**
		 *For messages of type POSTED_SMS, parses the body XML and creates an SMS instances <br>
		 * @return sms
		 */
		public Sms GetSMS() {
		
			Request request = null;
			try {
				request = new XCallRequest(9,6);
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
			Sms sms = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) {
				sms = (Sms)skype.factory(Sms.moduleID(), oid, skype);
			}
			return sms;
		}
		
		/**
		 *Deletes this message from the local database. These deletions do not propagate to the other Skype instances that the user may have on other computers. Nor do they affect other participants that have the same message. This method is specifically from removing Message objects from the database - not for removing Messages from conversations. To remove a Message from a conversation, use Message.Edit method to replace the existing body text with an empty string. <br>
		 */
		public void DeleteLocally() {
		
			Request request = null;
			try {
				request = new XCallRequest(9,8);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
	
	}