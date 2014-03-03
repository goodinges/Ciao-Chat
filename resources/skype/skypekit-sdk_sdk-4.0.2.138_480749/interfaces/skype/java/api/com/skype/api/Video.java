package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * This class contains basic video control functionality for live conversations with video. Basically, Video objects represent specific Participant's video state in a live Conversation. The Video class can represent both local (outgoing) and remote (incoming) video streams. Note that as of SkypeKit SDK version 3.2, this class no longer handles video rendering in the UI. Currently available SkypeKit runtimes do not support multi-party video. The API however is designed with future compatibility in mind, so the Video class is attached to Participant class rather than Conversation class. Once multi-party video will become available for SkypeKit, the logic should go like this: <br><br>Let there be 4-way live conversation C and participants P1, P2, P3 and P4. P1 is the local user. Remote participants P2 and P3 are capable of sending video. Remote user P4 is not capable of sending video. You would then end up with 4 video objects: V1, V2, V3 and V0. <br><br> - C->P1->V1-> outgoing video stream <br> - C->P2->V2-> incoming video stream 1 <br> - C->P3->V3-> incoming video stream 2 <br> - C->P4-> no video object as participant P4 does not advertise supporting video <br> - V0-> local webcam preview - this is not attached to any particular conversation, however the corresponding video object can be retrieved with Skype.GetPreviewVideo method. <br><br>As getting from a live conversation to running video streams involves three classes, it can be somewhat less than obvious. The basic state transition logic goes like this: <br><br>You start out with a Conversation, that suddenly becomes live <br><br>CONVERSATION.LOCAL_LIVESTATUS = IM_LIVE <br>At this point you have access to participant list of that conversation. The next step will be to catch Partcipant property changes for PARTICIPANT.VIDEO_STATUS to see if any of the people in conversation have Video available. Note that you should not make assumptions on when this availability happens. Remote users may switch their video on-off as they please. <br><br>PARTICIPANT.VIDEO_STATUS = VIDEO_AVAILABLE <br>If you get to VIDEO_AVAILABLE (not necessarily for all Participants), you can retrieve Video object, with Participant.GetVideo method. <br><br>Now you will need to handle Video.STATUS property changes. In case of successful video call, the sequence of Video.STATUS and Participant.VIDEO_STATUS changes for each Participant and Video object should look like this: <br><br> - Video.STATUS = AVAILABLE <br> - Video.STATUS = STARTING <br> - Video.STATUS = CHECKING_SUBSCRIPTION <br> - Video.STATUS = STARTING <br><br>Participant.VIDEO_STATUS = VIDEO_CONNECTING <br> - Video.STATUS = RUNNING <br> - Participant.VIDEO_STATUS = STREAMING <br>Both Video.STATUS == RUNNING and Participant.VIDEO_STATUS == STREAMING are indicative that the video for that particular participant is up and running, and your UI should update itself accordingly. <br><br>NB! Note that it is not enough to check for Video.STATUS property updates. By the time you obtain the Video object in your client, it may already it may already have progressed to a further status. You should always check the status property immediately after obtaining the Video object. <br>
 */


public class Video extends SkypeObject {


	public interface VideoListener {
		/** This event gets called when there are changes to Video properties defined in Video.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
		/**This method has no effect in current version. <br>*/
		public void OnCaptureRequestCompleted(SkypeObject obj, int requestId, boolean isSuccessful, byte[] image, int width, int height);
		
	}
	
	public Video(int oid, Skype skype) {
		super(oid,skype);
	}
	
	private static final int MODULE_ID = 11;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the Video class */
	public enum PROPERTY {
	
		/** Video.STATUS, type: STATUS */
		status(130),
		
		/** 'errorcode errortext' , type: String */
		error(131),
		
		/** space-separated string of tokens, type: String */
		debuginfo(132),
		
		/** This property does not currently work, always containing an empty string. For desktop video, you can get the frame dimensions from the video frame buffers API instead - the buffer struct retrieved with ipc.getFrame() or ipc.getNewFrame() has width and height fields, which you can then use in your UI. With RTP video solutions, you already have the frame sizes in your videohost code. Communicating these to the UI process is currently up to you. <br>, type: String */
		dimensions(133),
		
		/** Indicates whether the video object is streaming webcam video or screensharing session, values: MEDIA_SCREENSHARING, MEDIA_VIDEO <br>, type: MEDIATYPE */
		media_type(134),
		
		/** conference id to be able to identify remote/local video in the same call, type: int */
		convo_id(1104),
		
		/** device path used by video object, type: String */
		device_path(1105);
		
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
		GetPropertyRequest request = new GetPropertyRequest(11, mObjectId, prop.id);
		
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
		GetPropertyRequest request = new GetPropertyRequest(11, mObjectId, prop.id);
		
		byte [] data = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null) {
			data = r.GetAsBinary();
		}
		return data;
	}
	
	/** Setupkey SETUPKEY_VIDEO_DEVICE type:string  <br>Selected video device name <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
	public static final String VIDEO_DEVICE = "Lib/Video/Device";
	
	/** Setupkey SETUPKEY_VIDEO_DEVICE_PATH type:string  <br>Currently selected video device path. <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
	public static final String VIDEO_DEVICE_PATH = "Lib/Video/DevicePath";
	
	/** Setupkey SETUPKEY_VIDEO_AUTOSEND type:int  <br>Setting this to 1 starts sending video automatically when call starts <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
	public static final String VIDEO_AUTOSEND = "Lib/Video/AutoSend";
	
	/** Setupkey SETUPKEY_VIDEO_DISABLE type:int  <br>Setting this to 1 disables all video functionality. <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String VIDEO_DISABLE = "*Lib/Video/Disable";
	
	/** Setupkey SETUPKEY_VIDEO_RECVPOLICY type:string default value:"contacts" <br>noone | contacts | callpolicy <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
	public static final String VIDEO_RECVPOLICY = "Lib/Video/RecvPolicy";
	
	/** Setupkey SETUPKEY_VIDEO_ADVERTPOLICY type:string default value:"contacts" <br>noone | contacts | everyone <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
	public static final String VIDEO_ADVERTPOLICY = "Lib/Video/AdvertPolicy";
	
	/**
	 */
	public enum STATUS {
	
		/** */
		NOT_AVAILABLE(0),
		
		/** */
		AVAILABLE(1),
		
		/** */
		STARTING(2),
		
		/** */
		REJECTED(3),
		
		/** */
		RUNNING(4),
		
		/** */
		STOPPING(5),
		
		/** */
		PAUSED(6),
		
		/** */
		NOT_STARTED(7),
		
		/** */
		HINT_IS_VIDEOCALL_RECEIVED(8),
		
		/** */
		UNKNOWN(9),
		
		/** */
		RENDERING(10),
		
		/** */
		CHECKING_SUBSCRIPTION(11),
		
		/** */
		SWITCHING_DEVICE(12);
		
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
	public enum MEDIATYPE {
	
		/** */
		MEDIA_SCREENSHARING(1),
		
		/** */
		MEDIA_VIDEO(0);
		
		private static final Map<Integer,MEDIATYPE> lookup = new HashMap<Integer,MEDIATYPE>();
		
		static {
			for(MEDIATYPE s : EnumSet.allOf(MEDIATYPE.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private MEDIATYPE(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static MEDIATYPE get(int code) {
			return lookup.get(code);
		}
		
		public static MEDIATYPE fromString(String s) {
			for (MEDIATYPE p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 */
	public enum VIDEO_DEVICE_CAPABILITY {
	
		/** */
		VIDEOCAP_HQ_CAPABLE(0),
		
		/** */
		VIDEOCAP_HQ_CERTIFIED(1),
		
		/** */
		VIDEOCAP_REQ_DRIVERUPDATE(2),
		
		/** */
		VIDEOCAP_USB_HIGHSPEED(3);
		
		private static final Map<Integer,VIDEO_DEVICE_CAPABILITY> lookup = new HashMap<Integer,VIDEO_DEVICE_CAPABILITY>();
		
		static {
			for(VIDEO_DEVICE_CAPABILITY s : EnumSet.allOf(VIDEO_DEVICE_CAPABILITY.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private VIDEO_DEVICE_CAPABILITY(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static VIDEO_DEVICE_CAPABILITY get(int code) {
			return lookup.get(code);
		}
		
		public static VIDEO_DEVICE_CAPABILITY fromString(String s) {
			for (VIDEO_DEVICE_CAPABILITY p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *This is a legacy method, used in earlier version of desktop video, for passing window handles from the UI to the rendering engine in the runtime. In current version of desktop video, the rendering takes place in the client. The video objects now need to be associated not with a specific window, but rather with an IPC channel that the Video object sends decoded video frames from the runtime to the client UI. See Video.SetRemoteRendererId for more information. <br>
	 * @param windowh
	 */
	public void SetScreen( int windowh) {
	
		Request request = null;
		try {
			request = new XCallRequest(11,1);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('u',1,windowh);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *This method starts either video send or video receive, depending on whether the video object is sender or receiver. In case of desktop video, the receiver side needs to instantiate a renderer object and associate it with the receiveing video (Video.SetRemoteRendererId).  <br>
	 */
	public void Start() {
	
		Request request = null;
		try {
			request = new XCallRequest(11,2);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *This method stops either video send or video receive, depending on whether the video object is sender or receiver. In case of desktop video, the receiver side needs to dis-associate the video object from the renderer, by calling Video.SetRemoteRendererId(0).  <br>
	 */
	public void Stop() {
	
		Request request = null;
		try {
			request = new XCallRequest(11,3);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *This method has no known effect in current version. <br>
	 * @return SubmitCaptureRequestResult
	 */
	public SubmitCaptureRequestResult SubmitCaptureRequest() {
	
		Request request = null;
		try {
			request = new XCallRequest(11,11);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		SubmitCaptureRequestResult result = new SubmitCaptureRequestResult();
		
		boolean ret = false;
		ret = r.GetAsBoolean(1);
		result.ret = ret;
		
		int requestId = 0;
		requestId = r.GetAsInt(2);
		result.requestId = requestId;
		
		return result;
	}
	
	public class SubmitCaptureRequestResult {
		public boolean ret;
		public int requestId;
	}
	
	/**
	 *This method has no known effect in current version. <br>
	 * @param x0
	 * @param y0
	 * @param width
	 * @param height
	 * @param monitorNumber
	 * @param windowHandle
	 */
	public void SetScreenCaptureRectangle( int x0, int y0, int width, int height, int monitorNumber, int windowHandle) {
	
		Request request = null;
		try {
			request = new XCallRequest(11,5);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('i',1,x0);
		request.addParm('i',2,y0);
		request.addParm('u',3,width);
		request.addParm('u',4,height);
		request.addParm('i',5,monitorNumber);
		request.addParm('u',6,windowHandle);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *Sets video render rectangle co-ordinates, relative to upper left corner of the rendering window's client area (there us no need to compensate for the title bar area or window borders). Note that the rendering rectangle does not get automatically updated after video window resize occurs. You will need to catch the resize events and update the rendering rectangle according to the new size of the video window. <br><br>NB! Currently, when the updating the render rectangle to the new size of the video window, you will also need to call Video class SetScreen method - before SetRenderRectangle. <br>
	 * @param x0
	 * @param y0
	 * @param width
	 * @param height
	 */
	public void SetRenderRectangle( int x0, int y0, int width, int height) {
	
		Request request = null;
		try {
			request = new XCallRequest(11,6);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('i',1,x0);
		request.addParm('i',2,y0);
		request.addParm('u',3,width);
		request.addParm('u',4,height);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *This method is specific to working with desktop video runtimes. In case of desktop video, this method associates a Video object with a rendering object. Once this association is in place, the Video object will start sending decoded video frames to the renderer object, over shared memory IPC transport. The renderer then periodically reads the shared memory buffer and draws bitmaps on the screen.  <br><br>The "remote renderer ID" value you are expected to pass in this method comes from the renderer. The renderer class is normally implemented by you (for a specific GUI framework). However, the shared memory transport mechanism, for receiving decoded frames, is independant of GUI framework and are provided with the SDK for all three supporeted operating systems.  <br><br>Your renderer class can include instance of the IPC transport as VideoTransportClient <WinShm> ipc or VideoTransportClient <PosixShm>  ipc; The ipc object then exposes ipc.key() function that returns the IPC channel ID. This ID can then be passed to the Video.SetRemoteRendererId method. A reference implementation of desktop video is included in the SDK (examples/cpp/qt_videocalls). <br><br>When the incoming video stream stops (or rendering is stopped by the user), your client should explicitly stop the frame transport by calling Video.SetRemoteRendererId(0). <br>
	 * @param id IPC channel ID retrieved from the shared memory transport class (see ipc/cpp/VideoBuffers in the SDK). <br>
	 */
	public void SetRemoteRendererId( int id) {
	
		Request request = null;
		try {
			request = new XCallRequest(11,14);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('u',1,id);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 *This method has no effect in current version. <br>
	 * @param mediaType
	 * @param webcamName
	 * @param devicePath
	 * @param updateSetup
	 */
	public void SelectVideoSource( Video.MEDIATYPE mediaType, String webcamName, String devicePath, boolean updateSetup) {
	
		Request request = null;
		try {
			request = new XCallRequest(11,7);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		request.addParm('e',1,mediaType.getId());
		request.addParm('S',2,webcamName);
		request.addParm('S',3,devicePath);
		request.addParm('b',4,updateSetup);
		
		skype.XCall((XCallRequest)request);
	}
	
	/**
	 * @return GetCurrentVideoDeviceResult
	 */
	public GetCurrentVideoDeviceResult GetCurrentVideoDevice() {
	
		Request request = null;
		try {
			request = new XCallRequest(11,10);
		} catch (IOException e) {
			e.printStackTrace();
			if (skype.errorListener != null)
				skype.errorListener.OnSkypeKitFatalError();
		}
		request.addParm('O',0,mObjectId);
		
		Response r = skype.XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		GetCurrentVideoDeviceResult result = new GetCurrentVideoDeviceResult();
		
		MEDIATYPE mediatype = null;
		mediatype = MEDIATYPE.get(r.GetAsInt(1));
		result.mediatype = mediatype;
		
		String deviceName = null;
		deviceName = r.GetAsString(2);
		result.deviceName = deviceName;
		
		String devicePath = null;
		devicePath = r.GetAsString(3);
		result.devicePath = devicePath;
		
		return result;
	}
	
	public class GetCurrentVideoDeviceResult {
		public MEDIATYPE mediatype;
		public String deviceName;
		public String devicePath;
	}
	

}