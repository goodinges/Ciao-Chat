package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * Wrapper class that includes voicemail-specific methods and properties. In the Skype Conversation API, Voicemail is actually something of a misnomer for what would be more accurately called Voice Message. <br><br>The traditional Voicemail use case involves recording a voice message when an incoming call does not get answered in a pre-determined amount of time. In the Skype Conversation API, voicemail does not depend on a call going unanswered - you can post a voice message asynchronously into any dialog conversation at any time. <br><br>In fact, a high-level action flow directing unanswered incoming live sessions to voicemail is not something provided by the Conversation API - implementation of this use case is largely up to your UI. <br><br>The fact that your UI must retrieve incoming Voicemails by monitoring changes to a Conversation instance's Messages illustrates this conceptual difference between traditional voicemail and voice messages. The message type Message.POSTED_VOICE_MESSAGE indicates that a Message instance should be handled as a voice message instead of by displaying its body text in the Conversation UI. Message.GetVoiceMessage enables you to retrieve the associated Voicemail instance; Voicemail.StartPlayback enables you to listen to the message audio. <br><br>To put it another way, the object chain goes like this:  <br>@code <br>Contact->Conversation->Message->Voicemail  <br></CODE>  <br><br>There are three basic types of Voicemail objects: <br> - INCOMING - received voice messages that can be retrieved from Message objects; <br> - OUTGOING - outbound voice messages that can be created with Conversation.StartVoiceMessage; <br> - GREETING - voice messages that represent auto-answer greetings, either recorded by the user (CUSTOM_GREETING) or included as part of SkypeKit (DEFAULT_GREETING). This is the audio that gets played back to sender of the voice message before he can start recording his voice message. <br><br>Before trying to send out a voicemail, you should ensure that target Contact has the capability to receive them. Use Contact.GetCapabilityStatus to check for Contact.CAPABILITY_CAN_BE_SENT_VM. <br><br>Recording and Sending a Voice Message <br><br>The first step is to obtain a dialog Conversation with the target Contact. In that conversation, you can initiate the outgoing voice message with Conversation.StartVoiceMessage <br><br>Note that this call does not return an actual Voicemail object. To catch when an object gets created, you will need to check Conversation.P_ACTIVE_VM_ID property updates. <br><br>After invoking Conversation.StartVoiceMessage, SkypeKit instantiates a Voicemail instance for the target Contact's greeting (with type CUSTOM_GREETING or DEFAULT_GREETING). At this point, the Conversation.P_ACTIVE_VM_ID property update fires, newVM contains a reference to the greeting, and playback of the greeting for the sender starts automatically.  <br><br>Once the greeting playback finishes, SkypeKit instantiates a second Voicemail instance for the outgoing voice message. At this point, the Conversation.P_ACTIVE_VM_ID property update fires again, newVM now contains a reference to the outgoing message, and recording starts automatically. If you want to include notification and/or error handling for whether this message was sent successfully, you should make a copy of newVM now. <br><br>Once the user finishes (or abandons) recording of their message, they want to either send the message or to cancel it. To send the message, use Conversation.PostVoiceMessage; to cancel the message, use Conversation.LeaveLiveSession. <br><br>Both of these actions results in the Conversation.P_ACTIVE_VM_ID property update firing for a third time, setting the value to NULL. However, the Voicemail object will actually continue its existence past this point. Saving a reference to the message's Voicemail object when you start recording it enables you to keep receiving Voicemail property updates. This in turn enables your UI to check whether voice message send succeeded or failed. <br><br>The relevant terminal state Voicemail.P_STATUS property values are: <br> - Voicemail.CANCELLED - recording and/or sending of this message was cancelled <br> - Voicemail.UPLOADED - message sent <br> - Voicemail.FAILED - message could not be sent <br><br>Receiving and Playing Back a Voice Message <br><br>On the remote side, the Voicemail appears as a Message object of type Message.POSTED_VOICE_MESSAGE. The message's author property contains the Skype Name of the Voicemail originator, and its BodyXml property contains the message length and title text in following format: <br><br>@code <br><voicemail alt="Sent voicemail to people in this conversation."><message length="5" ></message></voicemail>  <br></CODE> <br><br>Receiver side UI can then retrieve the Voicemail object from the message with Message.GetVoiceMessage and <br>start local playback with Message.StartPlayback. <br>
 */


public class Voicemail extends SkypeObject {


	public interface VoicemailListener {
		/** This event gets called when there are changes to Voicemail properties defined in Voicemail.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
	}
	
	public Voicemail(int oid, Skype skype) {
		super(oid,skype);
	}
	
	private static final int MODULE_ID = 7;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the Voicemail class */
	public enum PROPERTY {
	
		/** type: TYPE */
		type(100),
		
		/** registered username of the other party, type: String */
		partner_handle(101),
		
		/** user's display name of the other party, type: String */
		partner_dispname(102),
		
		/** type: STATUS */
		status(103),
		
		/** type: FAILUREREASON */
		failurereason(104),
		
		/** DEPRECATED: subject line, type: String */
		subject(105),
		
		/** timestamp of creation, type: int */
		timestamp(106),
		
		/** duration in seconds, type: int */
		duration(107),
		
		/** max allowed duration in seconds, type: int */
		allowed_duration(108),
		
		/** VM playback progress in seconds, type: int */
		playback_progress(109),
		
		/** CONVERSATION_ID of corresponding conversation, type: Conversation */
		convo_id(830),
		
		/** GUID of the message that the VM is tied to, type: byte[] */
		chatmsg_guid(831);
		
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
		GetPropertyRequest request = new GetPropertyRequest(7, mObjectId, prop.id);
		
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
		GetPropertyRequest request = new GetPropertyRequest(7, mObjectId, prop.id);
		
		byte [] data = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null) {
			data = r.GetAsBinary();
		}
		return data;
	}
	
	/**
	 */
	public enum TYPE {
	
		/** */
		INCOMING(1),
		
		/** */
		DEFAULT_GREETING(4),
		
		/** */
		CUSTOM_GREETING(2),
		
		/** */
		OUTGOING(3);
		
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
	public enum STATUS {
	
		/** */
		NOTDOWNLOADED(1),
		
		/** */
		DOWNLOADING(2),
		
		/** */
		UNPLAYED(3),
		
		/** */
		BUFFERING(4),
		
		/** */
		PLAYING(5),
		
		/** */
		PLAYED(6),
		
		/** */
		BLANK(7),
		
		/** */
		RECORDING(8),
		
		/** */
		RECORDED(9),
		
		/** */
		UPLOADING(10),
		
		/** */
		UPLOADED(11),
		
		/** */
		DELETING(12),
		
		/** */
		FAILED(13),
		
		/** */
		DELETING_FAILED(14),
		
		/** */
		CHECKING(15),
		
		/** */
		CANCELLED(16);
		
		private static final Map<Integer,STATUS> lookup = new HashMap<Integer,STATUS>();
		
		static {
			for(STATUS s : EnumSet.allOf(STATUS.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private STATUS(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static STATUS get(int code) {
			return lookup.get(code);
		}
		
		public static STATUS fromString(String s) {
			for (STATUS p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 */
	public enum FAILUREREASON {
	
		/** */
		MISC_ERROR(1),
		
		/** */
		CONNECT_ERROR(2),
		
		/** */
		NO_VOICEMAIL_CAPABILITY(3),
		
		/** */
		NO_SUCH_VOICEMAIL(4),
		
		/** */
		FILE_READ_ERROR(5),
		
		/** */
		FILE_WRITE_ERROR(6),
		
		/** */
		RECORDING_ERROR(7),
		
		/** */
		PLAYBACK_ERROR(8),
		
		/** */
		NO_PERMISSION(9),
		
		/** receiver turned off voicemail*/
		RECEIVER_DISABLED_VOICEMAIL(10),
		
		/** receiver has not authorized you and privacy is not set to anyone*/
		SENDER_NOT_AUTHORIZED(11),
		
		/** receiver blocked sender*/
		SENDER_BLOCKED(12);
		
		private static final Map<Integer,FAILUREREASON> lookup = new HashMap<Integer,FAILUREREASON>();
		
		static {
			for(FAILUREREASON s : EnumSet.allOf(FAILUREREASON.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private FAILUREREASON(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static FAILUREREASON get(int code) {
			return lookup.get(code);
		}
		
		public static FAILUREREASON fromString(String s) {
			for (FAILUREREASON p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *Start recording your own auto-answer greeting message (leave message after the beep...) only. Recording of outgoing Voicemail messages start automatically (using Conversation.StartVoiceMessage) after playback of the remote side greeting message has finished. <br>
	 */
	public void StartRecording() {
	
		Request request = null;
		try {
			request = new XCallRequest(7,3);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *Stop recording of your own auto-answer greeting message only. To stop recording of and send an outgoing Voicemail, use Conversation.PostVoiceMessage. <br>
	 */
	public void StopRecording() {
	
		Request request = null;
		try {
			request = new XCallRequest(7,4);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *Initiates playback of a voice message <br>
	 */
	public void StartPlayback() {
	
		Request request = null;
		try {
			request = new XCallRequest(7,5);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *Terminates playback of a voice message <br>
	 */
	public void StopPlayback() {
	
		Request request = null;
		try {
			request = new XCallRequest(7,6);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *first from server, and then the local copy
	 */
	public void Delete() {
	
		Request request = null;
		try {
			request = new XCallRequest(7,7);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *Canceling recording of your own auto-answer greeting message. To stop recording of and cancel an outgoing Voicemail, use Conversation.LeaveLiveSession. <br>
	 */
	public void Cancel() {
	
		Request request = null;
		try {
			request = new XCallRequest(7,8);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *check if we can send voicemail (unauth,blocked,no priv etc cases). only OUTGOING
	 * @return result
	 */
	public boolean CheckPermission() {
	
		Request request = null;
		try {
			request = new XCallRequest(7,13);
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
	

}