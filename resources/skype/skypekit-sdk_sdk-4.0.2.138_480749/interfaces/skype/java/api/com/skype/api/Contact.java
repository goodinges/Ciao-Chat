package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * Address book entry. Encapsulates methods like GetType, GetIdentity, GetAvatar, SendAuthRequest, OpenConversation etc. Single contact can have additional phone numbers attached to it (ASSIGNED_PHONE1 .. ASSIGNED_PHONE3). Note that in the context of a conversation, Contacts are represented by Participant objects. Contact member functions all return a Boolean indicating the success (true) or failure (false) of processing the request itself (transport, runtime availability, and so forth)?not the success or failure of its associated functionality. For example, Contact.IsMemberOf returns true if it was able to make a determination, and its result parameter reflects whether this Contact is a member of the target group. Similarly, Contact.IsMemberOf returns false if it was unable to make a determination, and the value of its result parameter is undefined. <br>
 */


public class Contact extends SkypeObject {


	public interface ContactListener {
		/** This event gets called when there are changes to Contact properties defined in Contact.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
	}
	
	public Contact(int oid, Skype skype) {
		super(oid,skype);
		/**get default properties for this module **/
		GetDefaultProps();
	}
	
	private static final int MODULE_ID = 2;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the Contact class */
	public enum PROPERTY {
	
		/** type: Contact.TYPE */
		type(202),
		
		/** defined if it is a SKYPE contact, type: String */
		skypename(4),
		
		/** type: String */
		pstnnumber(6),
		
		/** type: String */
		fullname(5),
		
		/** integer of YYYYMMDD format, type: int */
		birthday(7),
		
		/** 1-male, 2-female, type: int */
		gender(8),
		
		/** ISO language code list, space separated, type: String */
		languages(9),
		
		/** ISO country code, type: String */
		country(10),
		
		/** type: String */
		province(11),
		
		/** type: String */
		city(12),
		
		/** This corresponds to the Account.P_PHONE_HOME property. The value is set by the remote user in Account profile. As Contact property, this is read-only. <br>, type: String */
		phone_home(13),
		
		/** This corresponds to the Account.P_PHONE_OFFICE property. The value is set by the remote user in Account profile. As Contact property, this is read-only. If the Contact has not populated his Account profile with sufficient phone numbers, the UI should implement locally adding additional phone numbers to Contact P_ASSIGNED_PHONE1 .. P_ASSIGNED_PHONE3 properties (and corresponding labels). See Contact.SetPhoneNumber method for more information. <br>, type: String */
		phone_office(14),
		
		/** This corresponds to the Account.P_PHONE_MOBILE property. The value is set by the remote user in Account profile. As Contact property, this is read-only. <br>, type: String */
		phone_mobile(15),
		
		/** will be hashed before advertising/querying, space separated, type: String */
		emails(16),
		
		/** type: String */
		homepage(17),
		
		/** arbitrary introductory text, type: String */
		about(18),
		
		/** Contact avatar pictures are in JPG format. The original size of avatar pictures are in no more than 96 x 96 pixels in size. However, as they can be smaller, scaling the pictures up too much to fit your UI can lead to distorted images. <br>, type: byte[] */
		avatar_image(37),
		
		/** Personal mood message (visible to authorized users only).  <br>, type: String */
		mood_text(26),
		
		/** XML version of personal mood text, type: String */
		rich_mood_text(205),
		
		/** 24*3600+diff_to_UTC_in_seconds. NB! changes with DST <br>, type: int */
		timezone(27),
		
		/** binary tag that can be queried via Contact.HasCapability(), type: byte[] */
		capabilities(36),
		
		/** UNIX timestamp of last profile change, type: int */
		profile_timestamp(19),
		
		/** count of this user's authorized contacts <br>, type: int */
		nrof_authed_buddies(28),
		
		/** ISO country code assigned by the IP, type: String */
		ipcountry(29),
		
		/** UNIX timestamp of when current avatar was set, type: int */
		avatar_timestamp(182),
		
		/** NOT SET FOR CONTACTS. For Account object, UNIX timestamp of when current mood was set, type: int */
		mood_timestamp(183),
		
		/** set if the contact is waiting to be authorized. The value contains auth. request text <br>, type: String */
		received_authrequest(20),
		
		/** timestamp of last received auth-request, type: int */
		authreq_timestamp(25),
		
		/** X timestamp of last successful ping to that user, type: int */
		lastonline_timestamp(35),
		
		/** Contact.AVAILABILITY, type: AVAILABILITY */
		availability(34),
		
		/** always set (assigned by lib by looking at various fields), type: String */
		displayname(21),
		
		/** true if querying additional information from p2p or server <br>, type: boolean */
		refreshing(22),
		
		/** Contact.AUTHLEVEL, change via Contact.GiveAuthlevel(), type: AUTHLEVEL */
		given_authlevel(23),
		
		/** change via Contact.GiveDisplayname(), type: String */
		given_displayname(33),
		
		/** change via Contact.AssignComment(), type: String */
		assigned_comment(180),
		
		/** UNIX timestamp of last outbound session (call, chat, FT, etc), type: int */
		lastused_timestamp(39),
		
		/** for contacts that have CONTACT_RECEIVED_AUTHREQUEST, how many times in a row they have requested it without positive answer, type: int */
		authrequest_count(41),
		
		/** Office phone no. of the contact. This property should NOT be used for SkypeOut contacts <br>, type: String */
		assigned_phone1(184),
		
		/** Value "1" in this property will be translated as "Office" by Skype Windows desktop client, according to UI language settings <br>, type: String */
		assigned_phone1_label(185),
		
		/** Mobile phone no. of the contact <br>, type: String */
		assigned_phone2(186),
		
		/** Value "2" in this property will be translated as "Mobile" by Skype Windows desktop client, according to UI language settings <br>, type: String */
		assigned_phone2_label(187),
		
		/** "Other phone no. of the contact, type: String */
		assigned_phone3(188),
		
		/** Value "3" in this property will be translated as "Other" by Skype Windows desktop client, according to UI language settings <br>, type: String */
		assigned_phone3_label(189),
		
		/** Contact's order by presence popularity <br>, type: int */
		popularity_ord(42);
		
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
		GetPropertyRequest request = new GetPropertyRequest(2, mObjectId, prop.id);
		
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
		GetPropertyRequest request = new GetPropertyRequest(2, mObjectId, prop.id);
		
		byte [] data = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null) {
			data = r.GetAsBinary();
		}
		return data;
	}
	
	/**default array of Contact Properties that get fetched & cached upon class construction*/
	private static PROPERTY [] defaultProperties = { PROPERTY.type, PROPERTY.skypename, PROPERTY.fullname, PROPERTY.displayname, PROPERTY.mood_text, PROPERTY.pstnnumber, PROPERTY.availability, PROPERTY.given_displayname};
	
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
		Same as with CAPABILITY, enumerator is used by both Contact and Account objects. <br> */
		public enum TYPE {
		
			/** Contact/account has no pre-identified type. This type is reported by default for SkypeKit clients. <br>*/
			UNRECOGNIZED(0),
			
			/** Normal Skype contact. <br>*/
			SKYPE(1),
			
			/** Normal PSTN contact. <br>*/
			PSTN(2),
			
			/** Emergency number (i.e. 911). <br>*/
			EMERGENCY_PSTN(3),
			
			/** */
			FREE_PSTN(4),
			
			/** Undisclosed PSTN number. <br>*/
			UNDISCLOSED_PSTN(5),
			
			/** This type is currently used by Windows desktop clients for contacts imported from Outlook. <br>*/
			EXTERNAL(6);
			
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
		 *Getter method for the TYPE property. <br>
		 * @return type
		 */
		public TYPE GetType() {
		
			Request request = null;
			try {
				request = new XCallRequest(2,1);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			TYPE type = null;
			type = TYPE.get(r.GetAsInt(1));
			return type;
		}
		
		/**
		Describes the recognized relational states between a local account and a remote contact. <br> */
		public enum AUTHLEVEL {
		
			/** Authorization request is either ignored or pending. In this state several functionalities may be blocked, depending on settings. For example, accounts may only allow seeing online presence to be viewable or only receive calls from authorized contacts. <br>*/
			NONE(0),
			
			/** Contact has been authorized by the local account. <br>*/
			AUTHORIZED_BY_ME(1),
			
			/** Contact has been blocked by the local account. This prevents incoming calls, chat messages, additional authorization requests etc. <br>*/
			BLOCKED_BY_ME(2);
			
			private static final Map<Integer,AUTHLEVEL> lookup = new HashMap<Integer,AUTHLEVEL>();
			
			static {
				for(AUTHLEVEL s : EnumSet.allOf(AUTHLEVEL.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private AUTHLEVEL(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static AUTHLEVEL get(int code) {
				return lookup.get(code);
			}
			
			public static AUTHLEVEL fromString(String s) {
				for (AUTHLEVEL p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Describes the superset list of possible Account and Contact online statuses. In case of Account they apply to local user, in case of Contact they apply to remote contacts. <br> */
		public enum AVAILABILITY {
		
			/** Contact online status cannot be determined. This availability state should not normally reach the SkypeKit UI level. <br>*/
			UNKNOWN(0),
			
			/** Seeing Contact online status is blocked because authorization between contact and local account has not taken place. <br>*/
			PENDINGAUTH(8),
			
			/** Remote contact has been blocked by local account. This applies to online accounts. <br>*/
			BLOCKED(9),
			
			/** Remote SkypeOut contact has been blocked by local account. <br>*/
			BLOCKED_SKYPEOUT(11),
			
			/** Contact does not have an online status because he is a PSTN contact. <br>*/
			SKYPEOUT(10),
			
			/** Contact appears to be offline. <br>*/
			OFFLINE(1),
			
			/** Contact appears to be offline but has voicemail enabled. <br>*/
			OFFLINE_BUT_VM_ABLE(12),
			
			/** Contact appears to be offline but has enabled call forwarding, so calls may actually get through to him. <br>*/
			OFFLINE_BUT_CF_ABLE(13),
			
			/** Contact / Account is online <br>*/
			ONLINE(2),
			
			/** Contact / Account is online but away from keyboard. This can be either turned on manually or by automatic timer. In Windows desktop client, the timer can be configured with minute precision. <br>*/
			AWAY(3),
			
			/** This online status is marked as deprecated. If a remote contact indicates its status as NOT_AVAILABLE, the UI should handle this as equivalent of AWAY status. <br>*/
			NOT_AVAILABLE(4),
			
			/** Contact / Account is online but does not wish to be disturbed. This status supersedes AWAY status when the account is DO_NOT_DISTURB the AWAY timer should not modify the status. <br>*/
			DO_NOT_DISTURB(5),
			
			/** This online status is marked as deprecated. If a remote contact indicates its status as SKYPE_ME, the UI should handle this as equivalent of ONLINE status. <br>*/
			SKYPE_ME(7),
			
			/** Account status is set to INVISIBLE. This status in not applicable to remote Contacts. When the remote contact has set his availability to INVISIBLE, he will appear as OFFLINE to others. <br>*/
			INVISIBLE(6),
			
			/** only possible for local user/account*/
			CONNECTING(14),
			
			/** */
			ONLINE_FROM_MOBILE(15),
			
			/** *_FROM_MOBILE only possible for remote user*/
			AWAY_FROM_MOBILE(16),
			
			/** */
			NOT_AVAILABLE_FROM_MOBILE(17),
			
			/** */
			DO_NOT_DISTURB_FROM_MOBILE(18),
			
			/** */
			SKYPE_ME_FROM_MOBILE(20);
			
			private static final Map<Integer,AVAILABILITY> lookup = new HashMap<Integer,AVAILABILITY>();
			
			static {
				for(AVAILABILITY s : EnumSet.allOf(AVAILABILITY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private AVAILABILITY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static AVAILABILITY get(int code) {
				return lookup.get(code);
			}
			
			public static AVAILABILITY fromString(String s) {
				for (AVAILABILITY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 * @return identity returns CONTACT_SKYPENAME or CONTACT_PSTNNUMBER value
		 */
		public String GetIdentity() {
		
			Request request = null;
			try {
				request = new XCallRequest(2,2);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			String identity = null;
			identity = r.GetAsString(1);
			return identity;
		}
		
		/**
		 *Returns Conrtact's avatar image (JPG). <br>
		 * @return GetAvatarResult
		 */
		public GetAvatarResult GetAvatar() {
		
			Request request = null;
			try {
				request = new XCallRequest(2,4);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			GetAvatarResult result = new GetAvatarResult();
			
			boolean present = false;
			present = r.GetAsBoolean(1);
			result.present = present;
			
			byte[] avatar = null;
			avatar = r.GetAsBinary(2);
			result.avatar = avatar;
			
			return result;
		}
		
		public class GetAvatarResult {
			public boolean present; /** <br> - true: the Contact has a custom avatar image <br> - false: the Contact does not have a custom avatar image <br> */
			public byte[] avatar; /** The avatar image data (JPG). If present is false, this will be the Skype-assigned default avatar <br> */
		}
		
		/**
		 *returns verified-by-Skype e-mail for this contact if exists and verifiable
		 * @return email
		 */
		public String GetVerifiedEmail() {
		
			Request request = null;
			try {
				request = new XCallRequest(2,3);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			String email = null;
			email = r.GetAsString(1);
			return email;
		}
		
		/**
		 *returns verified-by-Skype company for this contact if exists and verifiable
		 * @return company
		 */
		public String GetVerifiedCompany() {
		
			Request request = null;
			try {
				request = new XCallRequest(2,8);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			String company = null;
			company = r.GetAsString(1);
			return company;
		}
		
		/**
		 *Checks whether the contact is member of a contact group given in group reference argument. <br>
		 * @param group The target contact group <br>
		 * @return result <br> - true: the Contact is a member of the target contact group <br> - false: the Contact is not a member of the target contact group <br>
		 */
		public boolean IsMemberOf( ContactGroup group) {
		
			Request request = null;
			try {
				request = new XCallRequest(2,6);
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
		 *Checks whether the contact is member of a pre-defined contact group given in the TYPE argument (type for this property comes from the ContactGroup class). <br>
		 * @param groupType The type designator of the target pre-defined contact group. For example, specify this parameter as ContactGroup.TYPE.RECENTLY_CONTACTED_CONTACTS to determine if you've had a recent conversation with this Contact. <br>
		 * @return result <br> - true: the Contact is a member of the target contact group <br> - false: the Contact is not a member of the target contact group <br>
		 */
		public boolean IsMemberOfHardwiredGroup( ContactGroup.TYPE groupType) {
		
			Request request = null;
			try {
				request = new XCallRequest(2,7);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,groupType.getId());
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return false;
				
			boolean result = false;
			result = r.GetAsBoolean(1);
			return result;
		}
		
		/**
		 *Blocks or unblocks any further incoming communication attempts from this contact. <br>
		 * @param blocked <br> - true: block this contact <br> - false: unblock this contact <br>
		 * @param abuse Optional parameter to report abuse by this Skype user when blocking this Contact. Note that you can specifiy this parameter as true only when blocking a Contact. Defaults to false if omitted. <br>
		 */
		public void SetBlocked( boolean blocked, boolean abuse) {
		
			Request request = null;
			try {
				request = new XCallRequest(2,22);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('b',1,blocked);
			request.addParm('b',2,abuse);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Rejects and removes a pending authorization request from this Contact. <br>
		 */
		public void IgnoreAuthRequest() {
		
			Request request = null;
			try {
				request = new XCallRequest(2,21);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *sets CONTACT_GIVEN_DISPLAYNAME. clears if size(name)==0
		 * @param name
		 */
		public void GiveDisplayName( String name) {
		
			Request request = null;
			try {
				request = new XCallRequest(2,10);
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
		 *Adds or removes this Contact from the ALL_BUDDIES hardwired group. <br>
		 * @param isMyBuddy <br> - true: add this contact to the ALL_BUDDIES group <br> - false: delete contact from the ALL_BUDDIES group <br>
		 * @param syncAuth This argument is deprecated and should not be used. <br>
		 */
		public void SetBuddyStatus( boolean isMyBuddy, boolean syncAuth) {
		
			Request request = null;
			try {
				request = new XCallRequest(2,12);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('b',1,isMyBuddy);
			request.addParm('b',2,syncAuth);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 */
		public enum EXTRA_AUTHREQ_FIELDS {
		
			/** send verified e-mail blob with this auth request*/
			SEND_VERIFIED_EMAIL(1),
			
			/** send verified company blob with this auth request*/
			SEND_VERIFIED_COMPANY(2);
			
			private static final Map<Integer,EXTRA_AUTHREQ_FIELDS> lookup = new HashMap<Integer,EXTRA_AUTHREQ_FIELDS>();
			
			static {
				for(EXTRA_AUTHREQ_FIELDS s : EnumSet.allOf(EXTRA_AUTHREQ_FIELDS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private EXTRA_AUTHREQ_FIELDS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static EXTRA_AUTHREQ_FIELDS get(int code) {
				return lookup.get(code);
			}
			
			public static EXTRA_AUTHREQ_FIELDS fromString(String s) {
				for (EXTRA_AUTHREQ_FIELDS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *Sends a contact authorization request to this user. <br>
		 * @param message Text that typically introduces the requesting user and details the reason for the authorization request. This text will be set as Message.P_BODY_XML property of the notification Message the remote user will receive upon getting the authortization request. While this can be the empty string, it cannot be null. <br>
		 * @param extras_bitmask Indicates additional information to include with this authorization request: <br> - 0 (zero): do not include any additional information <br> - SEND_VERIFIED_EMAIL: include the requestor's verified e-mail address <br> - SEND_VERIFIED_COMPANY: include verified information regarding the requestor's company <br> - SEND_VERIFIED_EMAIL + SEND_VERIFIED_COMPANY: include both e-mail and company information <br>
		 */
		public void SendAuthRequest( String message, int extras_bitmask) {
		
			Request request = null;
			try {
				request = new XCallRequest(2,13);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,message);
			request.addParm('u',2,extras_bitmask);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *the contact has accepted my auth request
		 * @return result
		 */
		public boolean HasAuthorizedMe() {
		
			Request request = null;
			try {
				request = new XCallRequest(2,14);
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
		 *Sets the three P_ASSIGNED_PHONEx and P_ASSIGNED_PHONEx_LABEL properties, where x reflects the value of num.  <br><br>The Skype Windows desktop client uses the following conventions when interpreting and assigning values to these properties. While your solution can assign arbitrary values to these properties, we strongly recommend using these conventions to ensure interoperability with the Skype Windows desktop client. Keep in mind that the "number" of these property pairs has no relationship to how the Skype Windows client interprets their label property value strings. For example, the Skype Windows client will interpret P_ASSIGNED_PHONE3_LABEL as "Home" if its value is the string "0".  <br><br>Label strings: <br><br>Populate the label properties with string representations of the numbers "0" through "3", rather than descriptive strings like "Home", "Mobile", and so forth. The Skype desktop clients interpret the numeric string values as: <br> - "0" (zero) - "Home" <br> - "1" (one) - "Office" <br> - "2" - "Mobile" <br> - "3" - "Other" <br><br>Keep in mind that the "number" of a property pair has no relationship to its label string. For example, the Skype Windows client will interpret P_ASSIGNED_PHONE3_LABEL as "Home" if its value is the string "0". <br><br>Phone number strings: <br><br>The Skype Windows desktop client has distinct conventions for Skype Contacts and PSTN (SkypeOut) Contacts: any or all of the property pairs can be used for Skype Contacts; P_ASSIGNED_PHONE1 cannot be used for PSTN Contacts and P_ASSIGNED_PHONE1_LABEL has special meaning for PSTN Contacts. <br>Specifically, the Skype desktop clients use P_ASSIGNED_PHONE1_LABEL as the label for a PSTN Contact's primary number (regardless of whether it's home, mobile, or office), and use P_PSTNNUMBER to hold the actual number. <br><br>
		 * @param num The property pair being set, which must be in the range 0..3 <br>
		 * @param label The label text for the property being set <br>
		 * @param number The phone number for the property being set <br>
		 */
		public void SetPhoneNumber( int num, String label, String number) {
		
			Request request = null;
			try {
				request = new XCallRequest(2,15);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('u',1,num);
			request.addParm('S',2,label);
			request.addParm('S',3,number);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Retrieves a dialog conversation with the Contact. <br>
		 * @return conversation Retrieved dialog. <br>
		 */
		public Conversation OpenConversation() {
		
			Request request = null;
			try {
				request = new XCallRequest(2,17);
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
			Conversation conversation = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) {
				conversation = (Conversation)skype.factory(Conversation.moduleID(), oid, skype);
			}
			return conversation;
		}
		
		/**
		This enumerator is used by both Contact and Account objects. Thus the items here can have slightly different meaning, depending on which context you will examine their values. In case of Contact, the values apply to a user - across all the instances that user has logged in with Skype. In case of Account, the capability is that of a local, currently logged in instance of Skype client. <br><br>The values that CAPABILITY items can have are also dependant on class context. In context of Contact, a capability can be CAPABILITY_MIXED. Which in case of CAPABILITY_VIDEO, for example, would mean that the remote use has logged in with different clients, some of which support video calls and some of which don't. In context of Account - there are no mixed result. Currently logged in Skype instance either supports video or it doesn't. <br> */
		public enum CAPABILITY {
		
			/** For Account object, this is the same as CAPABILITY_CAN_BE_SENT_VM - it indicates that the currently logged in Skype instance supports voicemails. For Contact objects, it means that their remote system supports sending voicemails - there is no technical method for Skype to detect whether they are capable of receiving voicemails, so the assumption is that they can. <br>*/
			CAPABILITY_VOICEMAIL(0),
			
			/** Indicates that the contact/account has SkypeOut and is thus capable of making PSTN calls. <br>*/
			CAPABILITY_SKYPEOUT(1),
			
			/** Indicates that the contact/account has SkypeIn and is thus capable of answering PSTN calls. <br>*/
			CAPABILITY_SKYPEIN(2),
			
			/** For contacts, this is a combination of CAPABILITY_VOICEMAIL for local account (local Skype client supports sending voicemails) and CAPABILITY_VOICEMAIL of the Contact - if the contact supports sending voicemails then hopefully they can also receive them. <br>*/
			CAPABILITY_CAN_BE_SENT_VM(3),
			
			/** Indicates that Account/Contact supports call forwarding. <br>*/
			CAPABILITY_CALL_FORWARD(4),
			
			/** Indicates that Account/Contact supports call video calls. <br>*/
			CAPABILITY_VIDEO(5),
			
			/** In context of Contact, this indicates that the user is noticed running at least one Skype implementation that supports text messaging. Basically, it applies to a user. When applied to Account, the meaning is slightly different. In that case it indicates that currently running Skype implementation supports chat messaging. So, for Account objects, this is a node (rather than user) capability. <br>*/
			CAPABILITY_TEXT(6),
			
			/** Indicates that the contact/account is flagged as SkypePrime service provider. This is linked to Account class SERVICE_PROVIDER_INFO property. <br>*/
			CAPABILITY_SERVICE_PROVIDER(7),
			
			/** This is a legacy item, from the old times when conference calls with more than 5 people were limited to SkypePro accounts. In other words, this item is no longer relevant and will likely be removed at some point in future. <br>*/
			CAPABILITY_LARGE_CONFERENCE(8),
			
			/** */
			CAPABILITY_COMMERCIAL_CONTACT(9),
			
			/** Indicates that Account/Contact supports call transfers to PSTN numbers. <br>*/
			CAPABILITY_PSTN_TRANSFER(10),
			
			/** Indicates that the user has had his chat capability removed by Skype. Basically, this means that the user is reported as spammer too many times. This applies for both Contact and Account objects - which means your client can check locally, if the currently logged in user has been marked as a spammer. <br>*/
			CAPABILITY_TEXT_EVER(11),
			
			/** Indicates that the user (Account or Contact) has had his voice call capability removed by Skype. <br>*/
			CAPABILITY_VOICE_EVER(12),
			
			/** Indicates that the instance of Skype client Account/Contact is or in Contact's case has at least occasionally been flagged as a mobile device. <br>*/
			CAPABILITY_MOBILE_DEVICE(13),
			
			/** */
			CAPABILITY_PUBLIC_CONTACT(14);
			
			private static final Map<Integer,CAPABILITY> lookup = new HashMap<Integer,CAPABILITY>();
			
			static {
				for(CAPABILITY s : EnumSet.allOf(CAPABILITY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private CAPABILITY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static CAPABILITY get(int code) {
				return lookup.get(code);
			}
			
			public static CAPABILITY fromString(String s) {
				for (CAPABILITY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		List of possible states of each of the Contact class CAPABILITY items. <br> */
		public enum CAPABILITYSTATUS {
		
			/** Contact does not have the capability <br>*/
			NO_CAPABILITY(0),
			
			/** Contact has occasionally logged in with Skype client that supports the capability. For example, a contact may have Skype client on several machines, only some of which have webcam - in which case CAPABILITY_VIDEO would have its value set as CAPABILITY_MIXED. <br>*/
			CAPABILITY_MIXED(1),
			
			/** Contact has the capability <br>*/
			CAPABILITY_EXISTS(2);
			
			private static final Map<Integer,CAPABILITYSTATUS> lookup = new HashMap<Integer,CAPABILITYSTATUS>();
			
			static {
				for(CAPABILITYSTATUS s : EnumSet.allOf(CAPABILITYSTATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private CAPABILITYSTATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static CAPABILITYSTATUS get(int code) {
				return lookup.get(code);
			}
			
			public static CAPABILITYSTATUS fromString(String s) {
				for (CAPABILITYSTATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *Retrieves a Contact.Capability value. Sets P_REFRESHING to true while querying from server. This method is functionally the same as Contact.GetCapabilityStatus except that it returns a bool value rather than Contact.CAPABILITYSTATUS <br>
		 * @param capability The target capability <br>
		 * @param queryServer <br> - true: obtains the data from the p2p network/server, and sets P_REFRESHING to true for the duration  <br> - false: obtains the data from the local client <br>
		 * @return result <br> - true: the Contact has the target capability through at least one of the Skype clients they have logged into. Corresponds to CAPABILITY_MIXED and CAPABILITY_EXISTS <br> - false: the Contact does not have the target capability. Corresponds to NO_CAPABILITY <br>
		 */
		public boolean HasCapability( CAPABILITY capability, boolean queryServer) {
		
			Request request = null;
			try {
				request = new XCallRequest(2,18);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,capability.getId());
			request.addParm('b',2,queryServer);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return false;
				
			boolean result = false;
			result = r.GetAsBoolean(1);
			return result;
		}
		
		/**
		 *Retrieves a Contact.Capability value. Sets P_REFRESHING to true while querying from server. <br>
		 * @param capability The target capability, see Contact.Capability enumerator. <br>
		 * @param queryServer <br> - true: obtains the data from the p2p network/server, and sets P_REFRESHING to true for the duration  <br> - false: obtains the data from the local client <br>
		 * @return status Status of the target capability. <br>
		 */
		public CAPABILITYSTATUS GetCapabilityStatus( CAPABILITY capability, boolean queryServer) {
		
			Request request = null;
			try {
				request = new XCallRequest(2,19);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,capability.getId());
			request.addParm('b',2,queryServer);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			CAPABILITYSTATUS capabilitystatus = null;
			capabilitystatus = CAPABILITYSTATUS.get(r.GetAsInt(1));
			return capabilitystatus;
		}
		
		/**
		 *Refreshes all properties, capabilities, and statuses associated with this Contact from p2p/CBL, and sets P_REFRESHING to true for the duration. <br>
		 */
		public void RefreshProfile() {
		
			Request request = null;
			try {
				request = new XCallRequest(2,20);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
	
	}