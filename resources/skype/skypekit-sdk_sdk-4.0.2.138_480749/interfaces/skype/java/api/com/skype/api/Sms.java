package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * Wrapper class that includes SMS-specific properties and methods, such as P_BODY and GetTargetPrice. Instantiate SMS instances using Skype.CreateOutgoingSms; post SMS messages to a Conversation using Conversation.PostSMS.  <br><br>Each SMS can have multiple targets (normalized phone numbers). Note that in a Conversation context, every SMS instance has a corresponding Message instance. Once you've posted an SMS, you can retrieve its corresponding Message instance using Sms.GetPropChatmsgId. That Message instance's P_BODY_XML property contains the SMS message data, such as price, target phone number(s), failure codes, and so forth, which you can parsed out and display in the UI. To put it another way, the object chain goes like this: <br><br>@code <br>Conversation->Message->SMS  <br></CODE> <br><br>Note that SkypeKit SDK supports outgoing SMS messages only. SkypeKit clients, even when logged in with accounts that have SkypeIn numbers, cannot receive SMS messages. <br>
 */


public class Sms extends SkypeObject {


	public interface SmsListener {
		/** This event gets called when there are changes to Sms properties defined in Sms.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
	}
	
	public Sms(int oid, Skype skype) {
		super(oid,skype);
	}
	
	private static final int MODULE_ID = 12;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the Sms class */
	public enum PROPERTY {
	
		/** type: TYPE */
		type(190),
		
		/** type: STATUS */
		status(191),
		
		/** Set asynchronously and meaningful only after invoking Conversation.PostSMS and detecting Sms.STATUS of SOME_TARGETS_FAILED or FAILED. <br>, type: FAILUREREASON */
		failurereason(192),
		
		/** set to 1 when status goes to FAILED. use MarkSeen() to clear, type: boolean */
		is_failed_unseen(48),
		
		/** unix timestamp of message submission, type: int */
		timestamp(198),
		
		/** The total price of sending this SMS message (sum of the individual prices to send to each recipient). Defaults to -1 on instantiation and incremented by the price for each recipient once that recipient's status reflects TARGET_ACCEPTABLE. Use Sms.GetTargetPrice to retrieve individual target prices. <br><br>A value of MAX_UINT indicates that SkypeKit is actively querying and/or updating the value. Note that P_PRICE is an integer value. Calculate the actual price (in units specified by P_PRICE_CURRENCY) using P_PRICE_PRECISION as: <br><br>@code <br>actualPrice = price / 10^pricePrecision;  <br></CODE> <br>, type: int */
		price(193),
		
		/** The decimal precision of the SMS price values, both individual and total. For example, a value of 2 indicates that you should divide the price (represented as an integer) by 100 (10^2) to obtain the actual price. <br>, type: int */
		price_precision(49),
		
		/** should be same as account currency at the time of composing/sending, type: String */
		price_currency(194),
		
		/** number that should receive the replies, type: String */
		reply_to_number(199),
		
		/** space-separated normalised pstn numbers, type: String */
		target_numbers(195),
		
		/** binary blob. track with OnPropertyChange(), access with GetTargetStatus(target), type: byte[] */
		target_statuses(196),
		
		/** actual payload, type: String */
		body(197),
		
		/** reference to Message, type: Message */
		chatmsg_id(840);
		
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
		GetPropertyRequest request = new GetPropertyRequest(12, mObjectId, prop.id);
		
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
		GetPropertyRequest request = new GetPropertyRequest(12, mObjectId, prop.id);
		
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
	
		/** a normal outgoing SMS message*/
		OUTGOING(2),
		
		/** a message requesting a SMS confirmation code be sent to the number provided*/
		CONFIRMATION_CODE_REQUEST(3),
		
		/** a message returning the SMS confirmation code received as a result of a CONFIRMATION_CODE_REQUEST to authorize it*/
		CONFIRMATION_CODE_SUBMIT(4);
		
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
		COMPOSING(3),
		
		/** */
		SENDING_TO_SERVER(4),
		
		/** */
		SENT_TO_SERVER(5),
		
		/** */
		DELIVERED(6),
		
		/** */
		SOME_TARGETS_FAILED(7),
		
		/** */
		FAILED(8);
		
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
		SERVER_CONNECT_FAILED(2),
		
		/** */
		NO_SMS_CAPABILITY(3),
		
		/** */
		INSUFFICIENT_FUNDS(4),
		
		/** */
		INVALID_CONFIRMATION_CODE(5),
		
		/** */
		USER_BLOCKED(6),
		
		/** */
		IP_BLOCKED(7),
		
		/** */
		NODE_BLOCKED(8),
		
		/** */
		NO_SENDERID_CAPABILITY(9);
		
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
	 */
	public enum TARGETSTATUS {
	
		/** */
		TARGET_ANALYZING(1),
		
		/** */
		TARGET_UNDEFINED(2),
		
		/** */
		TARGET_ACCEPTABLE(3),
		
		/** */
		TARGET_NOT_ROUTABLE(4),
		
		/** */
		TARGET_DELIVERY_PENDING(5),
		
		/** */
		TARGET_DELIVERY_SUCCESSFUL(6),
		
		/** */
		TARGET_DELIVERY_FAILED(7);
		
		private static final Map<Integer,TARGETSTATUS> lookup = new HashMap<Integer,TARGETSTATUS>();
		
		static {
			for(TARGETSTATUS s : EnumSet.allOf(TARGETSTATUS.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private TARGETSTATUS(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static TARGETSTATUS get(int code) {
			return lookup.get(code);
		}
		
		public static TARGETSTATUS fromString(String s) {
			for (TARGETSTATUS p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *Retrieves the send status of this SMS to a particular recipient (P_TARGET_STATUSES) either prior to or after invoking Conversation.PostSMS. <br>
	 * @param target The normalized phone number of the target recipient. <br>
	 * @return status The send status of the target recipient, for example, TARGET_ANALYZING, TARGET_DELIVERY_PENDING, TARGET_DELIVERY_SUCCESSFUL, TARGET_DELIVERY_FAILED, and so forth. TARGET_UNDEFINED implies that the specified target is not a recipient of this SMS. <br>
	 */
	public TARGETSTATUS GetTargetStatus( String target) {
	
		Request request = null;
		try {
			request = new XCallRequest(12,4);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('S',1,target);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		TARGETSTATUS targetstatus = null;
		targetstatus = TARGETSTATUS.get(r.GetAsInt(1));
		return targetstatus;
	}
	
	/**
	 *Retrieves the amount of Skype credit necessary to send the SMS to a particular recipient. Defaults to -1 on instantiation and set only when that recipient's status reflects TARGET_ACCEPTABLE. Use Sms.GetPropPrice to retrieve the total cost of this SMS. <br><br>Note that the target price is an integer value. Calculate the actual price (in units specified by P_PRICE_CURRENCY) using P_PRICE_PRECISION as: <br>@code <br>actualTargetPrice = targetPrice / 10^pricePrecision;  <br></CODE> <br>
	 * @param target The normalized phone number of the target recipient. <br>
	 * @return price The price of sending this SMS message to the target recipient. <br>
	 */
	public int GetTargetPrice( String target) {
	
		Request request = null;
		try {
			request = new XCallRequest(12,13);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('S',1,target);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return 0;
			
		int price = 0;
		price = r.GetAsInt(1);
		return price;
	}
	
	/**
	 */
	public enum SETBODYRESULT {
	
		/** body not set. message status wrong or invalid, or body not valid utf8 string*/
		BODY_INVALID(0),
		
		/** body too long. set, but truncated. charsUntilNextChunk contains maxChunks value*/
		BODY_TRUNCATED(1),
		
		/** body was set OK*/
		BODY_OK(2),
		
		/** last unicode char was ignored, as some of the text would be deleted due to conversion*/
		BODY_LASTCHAR_IGNORED(3);
		
		private static final Map<Integer,SETBODYRESULT> lookup = new HashMap<Integer,SETBODYRESULT>();
		
		static {
			for(SETBODYRESULT s : EnumSet.allOf(SETBODYRESULT.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private SETBODYRESULT(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static SETBODYRESULT get(int code) {
			return lookup.get(code);
		}
		
		public static SETBODYRESULT fromString(String s) {
			for (SETBODYRESULT p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *Sets the recipient(s) of this SMS. Note that each invocation replaces the target list and re-calculates all prices - they are not additive! <br>
	 * @param numbers Normalized phone number(s) of the intended recipient(s). <br>
	 * @return success Set to true if the target list appears to contain valid, normalized telephone numbers. Note that this check is not very reliable. Actual target validity checking occurs asynchronously in the background, and manifests itself as a series of Sms.P_TARGET_STATUSES property change events. <br>
	 */
	public boolean SetTargets( String [] numbers) {
	
		Request request = null;
		try {
			request = new XCallRequest(12,6);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addListStart(1);
		for (int i=0;i<numbers.length;i++) {
			request.addParm('S',numbers[i]);
		}
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean success = false;
		success = r.GetAsBoolean(1);
		return success;
	}
	
	/**
	 *-The- method for setting the body text of this SMS. While Conversation.PostSMS does have a body argument, that argument is currently unused. <br>
	 * @param text Message body text. <br>
	 * @return SetSMSBodyResult
	 */
	public SetSMSBodyResult SetBody( String text) {
	
		Request request = null;
		try {
			request = new XCallRequest(12,7);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('S',1,text);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		SetSMSBodyResult result = new SetSMSBodyResult();
		
		SETBODYRESULT setbodyresult = null;
		setbodyresult = SETBODYRESULT.get(r.GetAsInt(1));
		result.result = setbodyresult;
		
		Vector<String> chunks = new Vector<String>();
		while (r.HasMore(2))
		{
			String string = null;
			string  = r.GetAsString(2);
			chunks.add(string);
		}
		result.chunks = chunks.toArray(new String[chunks.size()]);
		
		int charsUntilNextChunk = 0;
		charsUntilNextChunk = r.GetAsInt(3);
		result.charsUntilNextChunk = charsUntilNextChunk;
		
		return result;
	}
	
	public class SetSMSBodyResult {
		public SETBODYRESULT result; /** Whether the Message body was successfully set and if not, why not. <br> */
		public String [] chunks; /** The Message body as a list of individual chunks. <br> */
		public int charsUntilNextChunk; /** Number of available characters until creation of the next chunk becomes necessary. <br> */
	}
	
	/**
	 *Retrieves string list of SMS text chunks in first argument, while the second argument contains the number of available characters until creation of the next chunk becomes necessary. <br>
	 * @return GetBodyChunksResult
	 */
	public GetBodyChunksResult GetBodyChunks() {
	
		Request request = null;
		try {
			request = new XCallRequest(12,8);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		GetBodyChunksResult result = new GetBodyChunksResult();
		
		Vector<String> textChunks = new Vector<String>();
		while (r.HasMore(1))
		{
			String string = null;
			string  = r.GetAsString(1);
			textChunks.add(string);
		}
		result.textChunks = textChunks.toArray(new String[textChunks.size()]);
		
		int charsUntilNextChunk = 0;
		charsUntilNextChunk = r.GetAsInt(2);
		result.charsUntilNextChunk = charsUntilNextChunk;
		
		return result;
	}
	
	public class GetBodyChunksResult {
		public String [] textChunks; /** List of text chunk strings <br> */
		public int charsUntilNextChunk; /** Number of available characters until creation of the next chunk becomes necessary. <br> */
	}
	
	/**
	 */
	public enum CONFIRM_TYPE {
	
		/** Confirm mobile number as SMS sender number*/
		ID_SMS(1),
		
		/** Confirm mobile number as CLI for SkypeOut calls*/
		ID_MOBILE(2),
		
		/** unused currently*/
		ID_SKYPEIN(3);
		
		private static final Map<Integer,CONFIRM_TYPE> lookup = new HashMap<Integer,CONFIRM_TYPE>();
		
		static {
			for(CONFIRM_TYPE s : EnumSet.allOf(CONFIRM_TYPE.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private CONFIRM_TYPE(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static CONFIRM_TYPE get(int code) {
			return lookup.get(code);
		}
		
		public static CONFIRM_TYPE fromString(String s) {
			for (CONFIRM_TYPE p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	

}