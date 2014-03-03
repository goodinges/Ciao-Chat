package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * Transfer in this context refers to transferring (sending/receiving) files among Skype Contacts, not transferring a call to another Skype or PSTN Contact. This class includes file transfer-specific properties and methods, such as FILESIZE, BYTESPERSECOND, Pause and Resume. Recipients of these file transfers must explicitly accept (or decline) any incoming transfer. Transfer instances represent files being sent and received within a Conversation context. Each Transfer instance represents a single file transfer - if a conversation has multiple remote participants, a separate Transfer instance must be instantiated for each remote participant (a Transfer instance is not instantiated for the sender). <br><br>Transfer instances cannot be instantiated directly. Instead, you initiate a file transfer by invoking Conversation.PostFiles. This instantiates a Message instance of type POSTED_FILES, which is added to the Conversation for all the participants (including the sender). The Transfer instance is associated with <br>this Message instance, and the Message.P_BODY_XML looks like this: <br><br>@code <br>Some text<files alt=""><file size="2336020" index="0">test.zip</file></files>  <br></CODE> <br><br>To put it another way, the object chain goes like this: <br>@code <br>Conversation->Message->Transfer  <br></CODE> <br><br>The first part of the message (before the files section) comes from the Conversation.PostFiles body argument. For each file in the message, a file section is provided with three fields: <br> - file size in bytes <br> - index - more or less arbitrary order in which the transfers should be displayed in the UI <br> - file name. <br><br>For practical purposes, the Message.P_BODY_XML property is not all that useful in this context. The Transfer instances, however, contain the state and progress feedback for your UI. You retrieve these Transfer instances using Message.GetTransfers method. Since the sender can post multiple files to <br>a Conversation using the same Message, Message:GetTransfers retrieves a list of Transfer instances - one per file per recipient. <br><br>You can determine the direction of particular Transfer instance by querying Transfer.P_TYPE (INCOMING/OUTGOING).  <br><br>You can implement a progress indicator by monitoring Transfer.P_BYTESTRANSFERRED. Note that when testing this on your local network, you will most likely catch these property change events at the beginning and the end of the transfer only - which does not look like too useful. However, for non-local network transfers where the transfer speeds are in the neighborhood of 200-300 KB per second, you should consider implementing progress feedback as being mandatory and expect to catch multiple property change events for all but the smallest files. <br><br>Another property of interest is Transfer.P_STATUS. The normal transfer status sequence during successful outgoing transfer is this: <br> - TRANSFER STATUS -> NEW <br> - TRANSFER STATUS -> WAITING_FOR_ACCEPT <br> - TRANSFER STATUS -> CONNECTING <br> - TRANSFER STATUS -> TRANSFERRING <br> - TRANSFER STATUS -> CONNECTING <br> - TRANSFER STATUS -> COMPLETED <br><br>The list of all terminal Transfer statuses is: <br> - COMPLETED <br> - FAILED <br> - CANCELLED <br> - CANCELLED_BY_REMOTE <br><br>In-progress transfers can be canceled with Transfer.Cancel and paused/resumed with Transfer.Pause and Transfer.Resume. For transfers that complete with a status of FAILED, your UI should provide feedback based on the value of Transfer.P_FAILUREREASON. <br><br>Incoming transfers, once accepted, overwrite existing files with the same name. Before accepting an incoming file transfer, <br>your UI should prompt the recipient to: <br> - accept or decline the file <br> - if accepted, specify the directory of here to save the file (with a pre-filled default) <br> - if accepted and a file having the same name already exists at the specified destination, your UI should prompt for confirmation to overwrite and provide a way to alter the file name before accepting it <br><br>Similarly, your UI should verify the existence of outgoing files prior to invoking Conversation.PostFiles. <br><br>Note that you should provide both Conversation.PostFiles and Transfer.Accept methods fully-qualified paths. Otherwise, the paths will be assumed to be relative to the path of SkypeKit runtime, since the methods are actually executed in the runtime context. <br>
 */


public class Transfer extends SkypeObject {


	public interface TransferListener {
		/** This event gets called when there are changes to Transfer properties defined in Transfer.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
	}
	
	public Transfer(int oid, Skype skype) {
		super(oid,skype);
	}
	
	private static final int MODULE_ID = 6;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the Transfer class */
	public enum PROPERTY {
	
		/** INCOMING / OUTGOING <br>, type: TYPE */
		type(80),
		
		/** Skype Name of the remote party of the file transfer. If a file is posted in a conversation with more than one participant, Transfer objects are created for each of them - so a transfer is always to one single remote target. <br>, type: String */
		partner_handle(81),
		
		/** Display name of the remote participant. <br>, type: String */
		partner_dispname(82),
		
		/** Current state of the transfer <br>, type: STATUS */
		status(83),
		
		/** Set whenever P_STATUS transitions to FAILED. <br>, type: FAILUREREASON */
		failurereason(84),
		
		/** UNIX timestamp of when this Transfer instance was instantiated, not when the transfer process actually started (was accepted from receiver side). Do not use this property when calculate the data transfer speed! Instead, monitor changes to P_BYTESPERSECOND. <br>, type: int */
		starttime(85),
		
		/** UNIX timestamp of when this Transfer COMPLETED or FAILED. This property is never set if the receiving side (local or remote) canceled the transfer. <br>, type: int */
		finishtime(86),
		
		/** The path -and- filename of the file being transfered (typically fully qualified). For the receiver, SkypeKit sets this property upon acceptance of the incoming transfer. If not fully qualified, the path is assumed to be relative to the path of the SkypeKit runtime. <br>, type: String */
		filepath(87),
		
		/** The filename -only- of the file being transfered. The receiver side can use this property to pre-populate relevant UI components while prompting the user to accept the incoming transfer. <br>, type: String */
		filename(88),
		
		/** The size of the file being transferred in bytes. Depending on the magnitude of this value, your UI might want to display the size in terms of kilobytes or even megabytes. <br>, type: String */
		filesize(89),
		
		/** The number of bytes already transferred. Calculate the percentage of the file transferred so far as: <br>@code <br>P_BYTESTRANSFERRED / (P_FILESIZE / 100);  <br></CODE> <br><br>Use float variables to avoid problems with files smaller than 100 bytes! <br>, type: String */
		bytestransferred(90),
		
		/** Current data transfer speed in bytes per second. Typically, your UI will want to display this value as kilobytes per second (KBps). <br>, type: int */
		bytespersecond(91),
		
		/** The "global ID" of this Transfer's associated Message instance. GUIDs are shared across Skype client instances and across all users that can see this Message. <br>, type: byte[] */
		chatmsg_guid(92),
		
		/** A more or less arbitrary index for ordering multiple file transfers within the UI. <br>, type: int */
		chatmsg_index(93),
		
		/** The "global ID" of this Transfer's associated Conversation (as chained through its associated Message). GUIDs are shared across Skype client instances and across all users that can see this Conversation. <br><br>Note that currently SkypeKit sets this property for INCOMING file transfers only and returns 0 (zero) for all sending side transfers. This is a known bug. <br>, type: Conversation */
		convo_id(98);
		
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
		GetPropertyRequest request = new GetPropertyRequest(6, mObjectId, prop.id);
		
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
		GetPropertyRequest request = new GetPropertyRequest(6, mObjectId, prop.id);
		
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
		OUTGOING(2);
		
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
	Recognized values for the P_STATUS property. Reflects current state of this Transfer. <br> */
	public enum STATUS {
	
		/** The file has either not been posted (sent) (OUTGOING), or not accepted (received) (INCOMING). <br>*/
		NEW(0),
		
		/** A temporary state that transitions either into TRANSFERRING (relayed or direct) or to FAILED. For unknown reasons, outgoing transfers tend go into this state twice - immediately before the actual data transfer starts and immediately after it ends. <br>*/
		CONNECTING(1),
		
		/** The files have been posted but the recipient has not yet accepted (or has declined) the transfer. <br>*/
		WAITING_FOR_ACCEPT(2),
		
		/** The transfer has been accepted and file data is being sent/received. Periodic updates of P_BYTESTRANSFERRED property should occur. <br>*/
		TRANSFERRING(3),
		
		/** The transfer has been accepted and file data is being sent/received but is going over at least one relay. Since relayed transfers tend to be significantly slower than direct transfers, you might want to differentiate the two in your UI and notify the user that relayed transfer typically take significantly longer to finish. <br>*/
		TRANSFERRING_OVER_RELAY(4),
		
		/** The local user (either the sender or a receiver) has paused the transfer. <br>*/
		PAUSED(5),
		
		/** A remote user has paused the transfer. For senders, a receiver has paused the transfer; for receivers, the sender has paused the transfer. <br>*/
		REMOTELY_PAUSED(6),
		
		/** Local side (either sender or receiver) has canceled the transfer. This is a final state of the STATE property. <br>*/
		CANCELLED(7),
		
		/** File transfer has completed. This is a terminal state. <br>*/
		COMPLETED(8),
		
		/** File transfer has failed. This is a terminal state. UI should provide feedback, based on value of P_FAILUREREASON. <br>*/
		FAILED(9),
		
		/** Transfer whose existence has been hinted by corresponding chat message, but which is yet to arrive. <br>*/
		PLACEHOLDER(10),
		
		/** Outgoing transfer object from another instance of the same account as current local login, running on another system. Hinted through chat message - only implies an offer was made; not necessarily accepted, failed, or completed.  <br>*/
		OFFER_FROM_OTHER_INSTANCE(11),
		
		/** Remote side (either sender or receiver) has canceled the transfer. This is a final state of the STATE property. <br>*/
		CANCELLED_BY_REMOTE(12);
		
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
		SENDER_NOT_AUTHORISED(1),
		
		/** */
		REMOTELY_CANCELLED(2),
		
		/** */
		FAILED_READ(3),
		
		/** */
		FAILED_REMOTE_READ(4),
		
		/** */
		FAILED_WRITE(5),
		
		/** */
		FAILED_REMOTE_WRITE(6),
		
		/** */
		REMOTE_DOES_NOT_SUPPORT_FT(7),
		
		/** */
		REMOTE_OFFLINE_FOR_TOO_LONG(8),
		
		/** */
		TOO_MANY_PARALLEL(9),
		
		/** */
		PLACEHOLDER_TIMEOUT(10);
		
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
	 *Accepts an incoming file transfer and saves it to specified file on the local file system. If the specified file exists, SkypeKit will silently overwrite it. Your UI should prompting the user for confirmation in this case and provide a means for canceling the file transfer or specifying a different target file. <br>
	 * @param filenameWithPath Where on the local file system to save the file being transferred. Note that you should specify the path as being fully-qualified. Otherwise, SkypeKit will be assume it to be relative to the SkypeKit runtime path, since the method is actually executed in the runtime context. <br>
	 * @return success Set to true if the specified target file was successfully created on the local file system -and- the initial write(s) succeeded. However, the transfer itself can subsequently fail before completion due to its being canceled (either locally or remotely), network failure, local file system space/write issues, and so forth. <br>
	 */
	public boolean Accept( String filenameWithPath) {
	
		Request request = null;
		try {
			request = new XCallRequest(6,3);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('f',1,filenameWithPath);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean success = false;
		success = r.GetAsBoolean(1);
		return success;
	}
	
	/**
	 *Temporarily pauses an in-progress incoming or outgoing file transfer. For incoming transfers, only this affects the sender and the invoking recipient only. For outgoing transfers, this affects the sender and all recipients. <br>
	 */
	public void Pause() {
	
		Request request = null;
		try {
			request = new XCallRequest(6,4);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *Resumes a previously paused file transfer. <br>
	 */
	public void Resume() {
	
		Request request = null;
		try {
			request = new XCallRequest(6,5);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *Cancels an in-progress file transfer. Transfer.STATUS will transition to CANCELLED for incoming file transfers and to CANCELLED_BY_REMOTE for outgoing transfers. <br>
	 */
	public void Cancel() {
	
		Request request = null;
		try {
			request = new XCallRequest(6,6);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	

}