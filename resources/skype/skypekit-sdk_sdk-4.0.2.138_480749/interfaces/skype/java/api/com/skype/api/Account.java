package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * Represents a local account. Encapsulates methods for Skype account creation, login and logout as well as account profile setting properties. NB! Unlike all the other SkypeKit classes, most of the Account class properties are actually read-write. In fact, there are two sorts of Account's read-write properties: server-side properties and local properties. Different setter methods need to be used for those two kinds. <br><br>The subset of server-side properties consists of all the policy properties (everything with _POLICY suffix) that are all of type int and can be set with SetServersideIntProperty method. There is currently only one server-side string property - OFFLINE_CALLFORWARD which can be set with SetServersideStrProperty setter. <br><br>The set of writeable account profile properties (local profile) is as follows; <br> - P_FULLNAME, <br> - P_BIRTHDAY, <br> - P_GENDER, <br> - P_LANGUAGES, <br> - P_COUNTRY, <br> - P_PROVINCE, <br> - P_CITY, <br> - P_PHONE_HOME, <br> - P_PHONE_OFFICE, <br> - P_PHONE_MOBILE, <br> - P_EMAILS, <br> - P_HOMEPAGE, <br> - P_ABOUT, <br> - P_MOOD_TEXT, <br> - P_TIMEZONE, <br> - P_AVATAR_IMAGE, <br> - P_RICH_MOOD_TEXT <br><br>These can be set with SetIntProperty, SetStrProperty and SetBinProperty setter methods. Note that there are special methods for changing account online status (availability) and enabling/disabling auto-login - SetAvailability and SetSavePwd. <br><br>Note that to set Account properties, you that Account needs to be logged in. Generally, assumption is that the first thing you do with an Account object after you retrieve it with Skype.GetAccount is login, with exception of creating a new account. <br><br>The Account class has no default constructor and creating an Account instance is not the same as creating a Skype account. To create a Skype account: <br><br> - obtain an Account instance by invoking Skype.GetAccount. This automatically sets P_SKYPENAME. <br> - set any additional profile-related properties. Skype recommends that you minimally set the following: <br> - their email address so that they can retrieve a lost password (P_EMAILS) <br> - the user's full name (P_FULLNAME) <br> - at least one phone number (P_PHONE_HOME, P_PHONE_HOME, P_PHONE_OFFICE) <br> - invoke Account.Register to actually create the account <br>
 */


public class Account extends SkypeObject {


	public interface AccountListener {
		/** This event gets called when there are changes to Account properties defined in Account.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
	}
	
	public Account(int oid, Skype skype) {
		super(oid,skype);
		/**get default properties for this module **/
		GetDefaultProps();
	}
	
	private static final int MODULE_ID = 5;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the Account class */
	public enum PROPERTY {
	
		/** Account.STATUS, type: STATUS */
		status(70),
		
		/** Account.PWDCHANGESTATUS, type: PWDCHANGESTATUS */
		pwdchangestatus(71),
		
		/** This property should only be examined when Account.P_STATUS is LOGGED_OUT or LOGGED_OUT_AND_PWD_SAVED. That is, you should not monitor changes to this property in Account.OnChange callback, other than after already having checked that P_STATUS property has appropriate value. The reason for this is that the LOGOUTREASON does not get reset before the next login attempt. For example: if a user tried to log in with a wrong password, the LOGOUTREASON gets set to INCORRECT_PASSWORD. Now, if the user tries to log in again, and yet again submits an incorrect password, the value of the LOGOUTREASON does not change anymore, because it already is set to INCORRECT_PASSWORD. Consequently, Account.OnChange will not fire in this case. <br>, type: LOGOUTREASON */
		logoutreason(73),
		
		/** Account.COMMITSTATUS, type: COMMITSTATUS */
		commitstatus(78),
		
		/** suggested skypenames, semi-colon separated. present if logoutreason==SKYPENAME_TAKEN, type: String */
		suggested_skypename(72),
		
		/** 'EUR', 'USD', etc., type: String */
		skypeout_balance_currency(74),
		
		/** balance in 'cents', type: int */
		skypeout_balance(75),
		
		/** decimal points in ACCOUNT_SKYPEOUT_BALANCE, type: int */
		skypeout_precision(804),
		
		/** space-separated list of skypein numbers, type: String */
		skypein_numbers(76),
		
		/** Account.CBLSYNCSTATUS, type: CBLSYNCSTATUS */
		cblsyncstatus(79),
		
		/** space-separated list of 'begSecond,endSecond,identity' tokens, type: String */
		offline_callforward(77),
		
		/** Server-side account property, use SetServerside*Property() to set, type: CHATPOLICY */
		chat_policy(160),
		
		/** Server-side account property, use SetServerside*Property() to set, type: SKYPECALLPOLICY */
		skype_call_policy(161),
		
		/** Server-side account property, use SetServerside*Property() to set, type: PSTNCALLPOLICY */
		pstn_call_policy(162),
		
		/** Server-side account property, use SetServerside*Property() to set, type: AVATARPOLICY */
		avatar_policy(163),
		
		/** Server-side account property, use SetServerside*Property() to set, type: BUDDYCOUNTPOLICY */
		buddycount_policy(164),
		
		/** Server-side account property, use SetServerside*Property() to set, type: TIMEZONEPOLICY */
		timezone_policy(165),
		
		/** Server-side account property, use SetServerside*Property() to set, type: WEBPRESENCEPOLICY */
		webpresence_policy(166),
		
		/** Server-side account property, use SetServerside*Property() to set, type: PHONENUMBERSPOLICY */
		phonenumbers_policy(168),
		
		/** Server-side account property, use SetServerside*Property() to set, type: VOICEMAILPOLICY */
		voicemail_policy(169),
		
		/** Alerts: opted out partner id's, space separated, type: String */
		partner_optedout(773),
		
		/** service information if the user is a paid service provider, type: String */
		service_provider_info(800),
		
		/** NB! Unlike your common UNIX timestamps, the registration_timestamp is special, as it counts MINUTES rather than seconds, from Epoch (January 1, 1970) <br>, type: int */
		registration_timestamp(801),
		
		/** number of times this user is logged in from other computers, type: int */
		nr_of_other_instances(802),
		
		/** type: String */
		skypename(4),
		
		/** type: String */
		fullname(5),
		
		/** YYYYMMDD, type: int */
		birthday(7),
		
		/** 1-male, 2-female, type: int */
		gender(8),
		
		/** ISO language codes, space-separated, type: String */
		languages(9),
		
		/** ISO country code, type: String */
		country(10),
		
		/** type: String */
		province(11),
		
		/** type: String */
		city(12),
		
		/** NB! string not integer, type: String */
		phone_home(13),
		
		/** type: String */
		phone_office(14),
		
		/** type: String */
		phone_mobile(15),
		
		/** Space separated, type: String */
		emails(16),
		
		/** type: String */
		homepage(17),
		
		/** arbitrary introductory text, type: String */
		about(18),
		
		/** UNIX timestamp of last profile change, type: int */
		profile_timestamp(19),
		
		/** Personal mood text (visible to authorised users only). Max length 300 bytes. <br>, type: String */
		mood_text(26),
		
		/** 24*3600+diff_to_UTC_in_seconds. nb! changes with DST, type: int */
		timezone(27),
		
		/** Count of this user's authorized contacts. <br>, type: int */
		nrof_authed_buddies(28),
		
		/** Contact.AVAILABILITY, type: Contact.AVAILABILITY */
		availability(34),
		
		/** Account avatar picture can be set with Account.SetBinProperty method. The contact avatar picture is limited to max 96x96 pixels and 32000 bytes. If the submitted avatar picture exceeds either of these size limits, it is the responsibility of your client to scale the image down to appropriate size. <br><br>The avatar pictures have to be in JPG format. A SkypeKit client can enable the user to set the Account avatar in other picture formats, in which case the picture should be converted to JPG before submitting it.  <br><br>In any case, the avatar picture should be pre-validated with the Skype.ValidateAvatar method. <br>, type: byte[] */
		avatar_image(37),
		
		/** UNIX timestamp of when current avatar was set, type: int */
		avatar_timestamp(182),
		
		/** UNIX timestamp of when current mood was set, type: int */
		mood_timestamp(183),
		
		/** XML version of CONTACT_MOOD_TEXT. Max length 1000 bytes. <br>, type: String */
		rich_mood_text(205);
		
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
		GetPropertyRequest request = new GetPropertyRequest(5, mObjectId, prop.id);
		
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
		GetPropertyRequest request = new GetPropertyRequest(5, mObjectId, prop.id);
		
		byte [] data = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null) {
			data = r.GetAsBinary();
		}
		return data;
	}
	
	/**default array of Account Properties that get fetched & cached upon class construction*/
	private static PROPERTY [] defaultProperties = { PROPERTY.skypename, PROPERTY.fullname, PROPERTY.mood_text, PROPERTY.birthday, PROPERTY.skypeout_balance, PROPERTY.skypeout_balance_currency, PROPERTY.skypeout_precision, PROPERTY.availability, PROPERTY.cblsyncstatus};
	
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
		 */
		public enum STATUS {
		
			/** */
			LOGGED_OUT(1),
			
			/** the account is logged out, but password is not needed for re-login*/
			LOGGED_OUT_AND_PWD_SAVED(2),
			
			/** connecting to P2P network*/
			CONNECTING_TO_P2P(3),
			
			/** connecting to login server*/
			CONNECTING_TO_SERVER(4),
			
			/** waiting for response from server*/
			LOGGING_IN(5),
			
			/** response OK. initialising account-specific lib structures*/
			INITIALIZING(6),
			
			/** alright, we're good to go!*/
			LOGGED_IN(7),
			
			/** Logout() has been called but not processed yet*/
			LOGGING_OUT(8);
			
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
		 * This is a legacy method. The progress argument does not return overall login progress. Instead, it used to return database mount or conversion progress in case of logging in with a newer client on an older database version. Currently, the progress value should always return 0 under normal circumstances. There is currently no functionality in the SkypeKit API to accurately guess how long the login will take, as the P2P connect times are unpredictable. The maximum progress granularity available through SkypeKit API is currently the Account.P_STATUS property values. <br>
		 * @return GetStatusWithProgressResult
		 */
		public GetStatusWithProgressResult GetStatusWithProgress() {
		
			Request request = null;
			try {
				request = new XCallRequest(5,1);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			GetStatusWithProgressResult result = new GetStatusWithProgressResult();
			
			STATUS status = null;
			status = STATUS.get(r.GetAsInt(1));
			result.status = status;
			
			int progress = 0;
			progress = r.GetAsInt(2);
			result.progress = progress;
			
			return result;
		}
		
		public class GetStatusWithProgressResult {
			public STATUS status; /** Current value of this account's P_STATUS property */
			public int progress; /** This argument returns 0. <br> */
		}
		
		/**
		Recognized values for the P_CBLSYNCSTATUS property. CBL stands for Central Buddy List. In principle, this property and its states can be ignored by most UI developers. However, it can help to optimize UI buildup and behaviour, particularly in case of limited resources (such as mobile devices). <br><br>CBL is used to backup your contact list, contact groups, and profile information, and also used to synchronize this information with other Skype instances of your account (i.e. on another device). CBL sync can occur both during login and during normal operation. Note that CBL synchronization does not take place immediately after an Account property is changed. A delay between the first property change and CBL sync initiation enables the client to accumulate changes and do the synchronization in bulk. <br><br>Clients with limited resources might want to wait for CBL_IN_SYNC status before generating their UI's contact list representation. Otherwise it might be forced to redraw the contact list multiple times, as new updates get retrieved from the server-side. Similarly, applications that modify an account's mood message might want to know when the P_MOOD_TEXT or P_RICH_MOOD_TEXT property is synchronized to the server. Note that this sync is only for CBL and other logged in instances of the same account - other contacts will receive the mood message update directly. <br> */
		public enum CBLSYNCSTATUS {
		
			/** status is not clear (yet)*/
			CBL_INITIALIZING(0),
			
			/** first sync with empty profile*/
			CBL_INITIAL_SYNC_PENDING(1),
			
			/** Account properties are considered to be out of sync with CBL - attempt at synchronization is imminent. You might wish to wait with updating UI components that display the data that is about to change anyway.*/
			CBL_SYNC_PENDING(2),
			
			/** CBL synchronization is currently taking place.*/
			CBL_SYNC_IN_PROGRESS(3),
			
			/** Account properties are up-to-date.*/
			CBL_IN_SYNC(4),
			
			/** CBL sync has. Another attempt will be made after several minutes. If a second attempt also fails, subsequent attempts at synchronization will be made at ever increasing intervals.*/
			CBL_SYNC_FAILED(5),
			
			/** we have received a hint that there is a remote data change in CBL*/
			CBL_REMOTE_SYNC_PENDING(6);
			
			private static final Map<Integer,CBLSYNCSTATUS> lookup = new HashMap<Integer,CBLSYNCSTATUS>();
			
			static {
				for(CBLSYNCSTATUS s : EnumSet.allOf(CBLSYNCSTATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private CBLSYNCSTATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static CBLSYNCSTATUS get(int code) {
				return lookup.get(code);
			}
			
			public static CBLSYNCSTATUS fromString(String s) {
				for (CBLSYNCSTATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the Account.P_LOGOUTREASON. Note that this property should only be examined when Account.P_STATUS is LOGGED_OUT or LOGGED_OUT_AND_PWD_SAVED. That is, you should not monitor changes to this property in Account.OnChange callback, other than after already having checked that P_STATUS property has appropriate value. The reason for this is that the LOGOUTREASON does not get reset before the next login attempt. For example: if a user tried to log in with a wrong password, the LOGOUTREASON gets set to INCORRECT_PASSWORD. Now, if the user tries to log in again, and yet again submits an incorrect password, the value of the LOGOUTREASON does not change anymore, because it already is set to INCORRECT_PASSWORD. Consequently, Account.OnChange will not fire in this case. <br> */
		public enum LOGOUTREASON {
		
			/** manual logout (or unknown reason from previous session)*/
			LOGOUT_CALLED(1),
			
			/** sync errors at login/registration*/
			HTTPS_PROXY_AUTH_FAILED(2),
			
			/** sync errors at login/registration*/
			SOCKS_PROXY_AUTH_FAILED(3),
			
			/** sync errors at login/registration*/
			P2P_CONNECT_FAILED(4),
			
			/** sync errors at login/registration*/
			SERVER_CONNECT_FAILED(5),
			
			/** sync errors at login/registration*/
			SERVER_OVERLOADED(6),
			
			/** sync errors at login/registration*/
			DB_IN_USE(7),
			
			/** sync errors at registration*/
			INVALID_SKYPENAME(8),
			
			/** sync errors at registration*/
			INVALID_EMAIL(9),
			
			/** sync errors at registration*/
			UNACCEPTABLE_PASSWORD(10),
			
			/** sync errors at registration*/
			SKYPENAME_TAKEN(11),
			
			/** sync errors at registration*/
			REJECTED_AS_UNDERAGE(12),
			
			/** sync errors at login*/
			NO_SUCH_IDENTITY(13),
			
			/** sync errors at login*/
			INCORRECT_PASSWORD(14),
			
			/** sync errors at login*/
			TOO_MANY_LOGIN_ATTEMPTS(15),
			
			/** async errors (can happen anytime while logged in)*/
			PASSWORD_HAS_CHANGED(16),
			
			/** async errors (can happen anytime while logged in)*/
			PERIODIC_UIC_UPDATE_FAILED(17),
			
			/** async errors (can happen anytime while logged in)*/
			DB_DISK_FULL(18),
			
			/** async errors (can happen anytime while logged in)*/
			DB_IO_ERROR(19),
			
			/** async errors (can happen anytime while logged in)*/
			DB_CORRUPT(20),
			
			/** deprecated (superceded by more detailed DB_* errors)*/
			DB_FAILURE(21),
			
			/** platform sdk*/
			INVALID_APP_ID(22),
			
			/** platform sdk*/
			APP_ID_FAILURE(23),
			
			/** forced upgrade/discontinuation*/
			UNSUPPORTED_VERSION(24);
			
			private static final Map<Integer,LOGOUTREASON> lookup = new HashMap<Integer,LOGOUTREASON>();
			
			static {
				for(LOGOUTREASON s : EnumSet.allOf(LOGOUTREASON.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private LOGOUTREASON(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static LOGOUTREASON get(int code) {
				return lookup.get(code);
			}
			
			public static LOGOUTREASON fromString(String s) {
				for (LOGOUTREASON p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *Login an auto-login enabled account (P_STATUS is LOGGED_OUT_AND_PWD_SAVED) and optionally set the availability, for example login in as Contact.DO_NOT_DISTURB. To find out whether there is an auto-login enabled account available, you can use Skype.GetDefaultAccountName to retrieve the skypename of that account. You can then get an Account instance for login by invoking Skype.GetAccount. <br><br>If no accounts with stored login credentials are available (GetDefaultAccountName returns an empty string), then you will have to prompt the user for account name and password and then use LoginWithPassword. Account name field in the UI can be pre-populated with strings retrieved with Skype.GetExistingAccounts <br>
		 * @param setAvailabilityTo force this account's initial online status to the specified Contact.AVAILABILITY value.
		 */
		public void Login( Contact.AVAILABILITY setAvailabilityTo) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,5);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,setAvailabilityTo.getId());
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *  <br>Login in an account by specifying its password. To retrieve an account instance for login, invoke Skype.GetAccount. <br>
		 * @param password Password string (plaintext) for this account, which should be pre-validated (Skype.ValidatePassword) <br>
		 * @param savePwd <br> - true: Saves the password, ensuring that auto-login is enabled. <br> - false (default): Does not save the password, and so the user might not be able to effect auto-login until they explicitly invoke Account.SetPasswordSaved(true).
		 * @param saveDataLocally For internal use only.
		 */
		public void LoginWithPassword( String password, boolean savePwd, boolean saveDataLocally) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,6);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,password);
			request.addParm('b',2,savePwd);
			request.addParm('b',3,saveDataLocally);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *This command can be used to create a new Skype account, based on the Account object. When successful, this command will also log in with the newly created account. If the new account registration was not successful, Account status property will change to LOGGED_OUT. A common reason for registration failures is that the an account with that name already exists. In that case, Account LOGOUT_REASON will be set to SKYPENAME_TAKEN. Also, Account SUGGESTED_SKYPENAME property will be set to a list of similar but still available skypenames that could be used instead. The property will contain up to 5 semicolon-separated alternative skypenames. In if no suggested skypenames were available, then this property will contain an empty string. <br>NB! You should pre-validate your P_SKYPENAME value and any email string (Skype.ValidateProfileString) prior to invoking this method. <br>
		 * @param password Password string (plaintext) for this account, which should be pre-validated (Skype.ValidatePassword)
		 * @param savePwd <br> - true: Saves the password and enables auto-login. <br> - false (default): Does not save the password, and the user needs to be prompted for password on the next login attempt.
		 * @param saveDataLocally For internal use only.
		 * @param email An email address for retrieving lost passwords and receiving news and information from Skype.
		 * @param allowSpam enable/disable news and information from Skype being sent to account's e-mail.
		 */
		public void Register( String password, boolean savePwd, boolean saveDataLocally, String email, boolean allowSpam) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,7);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,password);
			request.addParm('b',2,savePwd);
			request.addParm('b',3,saveDataLocally);
			request.addParm('S',4,email);
			request.addParm('b',5,allowSpam);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Logs out current account. Note that calling this on client application exit is optional. <br>
		 * @param clearSavedPwd <br> - true: Clears any saved password use with auto-login and so disables auto-login until you explicitly invoke Account.SetPasswordSaved(true). <br> - false (default): Does not clear any saved password and so does not affect existing auto-login behavior.
		 */
		public void Logout( boolean clearSavedPwd) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,8);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('b',1,clearSavedPwd);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		Recognized values for the P_PWDCHANGESTATUS property that provides information on whether a password change succeeded or failed, giving detailed failure reason. After successful return from the Change Password method, clients should monitor the P_PWDCHANGESTATUS property changes. <br> - PWD_CHANGING - consider displaying an "in progress" indicator and continue polling <br> - PWD_OK - consider displaying an updated indicator and stop polling <br> - PWD_OK_BUT_CHANGE_SUGGESTED - consider displaying an updated indicator, along with a recommendation to change again to a stronger password. <br> */
		public enum PWDCHANGESTATUS {
		
			/** Password change succeeded. <br>*/
			PWD_OK(0),
			
			/** Password change is in progress. <br>*/
			PWD_CHANGING(1),
			
			/** Old password was incorrect. <br>*/
			PWD_INVALID_OLD_PASSWORD(2),
			
			/** Failed to verify password because of no connection to server. <br>*/
			PWD_SERVER_CONNECT_FAILED(3),
			
			/** Password was set but server didn't like it much. <br>*/
			PWD_OK_BUT_CHANGE_SUGGESTED(4),
			
			/** New password was exactly the same as old one. <br>*/
			PWD_MUST_DIFFER_FROM_OLD(5),
			
			/** The new password was unacceptable. (too short, too simple, etc.) <br>*/
			PWD_INVALID_NEW_PWD(6),
			
			/** Account was currently not logged in. <br>*/
			PWD_MUST_LOG_IN_TO_CHANGE(7);
			
			private static final Map<Integer,PWDCHANGESTATUS> lookup = new HashMap<Integer,PWDCHANGESTATUS>();
			
			static {
				for(PWDCHANGESTATUS s : EnumSet.allOf(PWDCHANGESTATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private PWDCHANGESTATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static PWDCHANGESTATUS get(int code) {
				return lookup.get(code);
			}
			
			public static PWDCHANGESTATUS fromString(String s) {
				for (PWDCHANGESTATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *Changes this account's password. Returns false if the change failed. NB! You should pre-validate your password strings (Skype.ValidatePassword) and ensure that they are different prior to invoking this method. <br>
		 * @param oldPassword "Current password string (plaintext) for this account
		 * @param newPassword New password string (plaintext) for this account
		 * @param savePwd <br> - true: Saves the new password and enables auto-login. <br> - false (default): Clears any existing saved password and so the user cannot effect auto-login until they explicitly invoke Account.SetPasswordSaved(true)
		 */
		public void ChangePassword( String oldPassword, String newPassword, boolean savePwd) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,11);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('S',1,oldPassword);
			request.addParm('S',2,newPassword);
			request.addParm('b',3,savePwd);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *save or clear credentials for auto-login whilst already logged in
		 * @param savePwd
		 */
		public void SetPasswordSaved( boolean savePwd) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,25);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('b',1,savePwd);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Setter for integer (and enum) server-side properties. For a list of writeable server-side properties, see the detailed description of the Account class. <br>
		 * @param propKey
		 * @param value
		 */
		public void SetServersideIntProperty( int propKey, int value) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,12);
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
		 *Setter for String server-side properties. For a list of writeable server-side properties, see the detailed description of the Account class. <br>
		 * @param propKey
		 * @param value
		 */
		public void SetServersideStrProperty( int propKey, String value) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,13);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,propKey);
			request.addParm('S',2,value);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		The list of possible values of Account class COMMITSTATUS property. Note that this property and its values have nothing to do with (automatic) CBL synchronization. Rather, the COMMITSTATUS reflects commit status to account's server side properties initiated with calls to Account class SetServersideIntProperty and Account class SetServersideStrProperty methods. After those methods, your client UI may want to wait until the COMMITSTATUS becomes COMMITTING_TO_SERVER followed by COMMITTED and inform the user if the value becomes COMMIT_FAILED. SetServerside<type>Property methods are used for writing privacy policy related and call forwarding related Account properties to the server. Unlike CBL synchronization, those updates are executed immediately. <br> */
		public enum COMMITSTATUS {
		
			/** No pending updates to the server. <br>*/
			COMMITTED(1),
			
			/** Update to the server in progress. <br>*/
			COMMITTING_TO_SERVER(2),
			
			/** Server update has failed. <br>*/
			COMMIT_FAILED(3);
			
			private static final Map<Integer,COMMITSTATUS> lookup = new HashMap<Integer,COMMITSTATUS>();
			
			static {
				for(COMMITSTATUS s : EnumSet.allOf(COMMITSTATUS.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private COMMITSTATUS(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static COMMITSTATUS get(int code) {
				return lookup.get(code);
			}
			
			public static COMMITSTATUS fromString(String s) {
				for (COMMITSTATUS p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *Cancels an attempt to commit a server-side P_XXX_POLICY or the P_OFFLINE_CALLFORWARD server-side property. Invoking this cancellation only makes sense whilst the P_COMMITTSTATUS is in COMMITTING_TO_SERVER state. <br>
		 */
		public void CancelServerCommit() {
		
			Request request = null;
			try {
				request = new XCallRequest(5,15);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		Recognized values for the P_CHAT_POLICY property that controls whether non-authorized users can initiate text chat with the currently logged in account. Note that since this set of values is associated with a server-side property, you must set that property using Account.SetServersideIntProperty <br> */
		public enum CHATPOLICY {
		
			/** Unauthorized contacts can initiate text chat. <br>*/
			EVERYONE_CAN_ADD(0),
			
			/** Only authorized contacts can initiate chat (default policy). <br>*/
			BUDDIES_OR_AUTHORIZED_CAN_ADD(2);
			
			private static final Map<Integer,CHATPOLICY> lookup = new HashMap<Integer,CHATPOLICY>();
			
			static {
				for(CHATPOLICY s : EnumSet.allOf(CHATPOLICY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private CHATPOLICY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static CHATPOLICY get(int code) {
				return lookup.get(code);
			}
			
			public static CHATPOLICY fromString(String s) {
				for (CHATPOLICY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_SKYPECALLPOLICY property that controls acceptance of incoming Skype calls. Note that since this set of values is associated with a server-side property, you must set that property using Account.SetServersideIntPropertyserver-side. <br> */
		public enum SKYPECALLPOLICY {
		
			/** Skype calls accepted from unauthorized contacts.*/
			EVERYONE_CAN_CALL(0),
			
			/** Skype calls not accepted from unauthorized contacts.*/
			BUDDIES_OR_AUTHORIZED_CAN_CALL(2);
			
			private static final Map<Integer,SKYPECALLPOLICY> lookup = new HashMap<Integer,SKYPECALLPOLICY>();
			
			static {
				for(SKYPECALLPOLICY s : EnumSet.allOf(SKYPECALLPOLICY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private SKYPECALLPOLICY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static SKYPECALLPOLICY get(int code) {
				return lookup.get(code);
			}
			
			public static SKYPECALLPOLICY fromString(String s) {
				for (SKYPECALLPOLICY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_PSTNCALLPOLICY property that controls whether (and from whom) this account accepts incoming PSTN calls. Note that since this set of values is associated with a server-side property, you must set that property using Account.SetServersideIntProperty <br> */
		public enum PSTNCALLPOLICY {
		
			/** All incoming PSTN calls are accepted.*/
			ALL_NUMBERS_CAN_CALL(0),
			
			/** Only PSTN calls that report caller ID are accepted.*/
			DISCLOSED_NUMBERS_CAN_CALL(1),
			
			/** Only calls from PSTN numbers found in local contact list are accepted.*/
			BUDDY_NUMBERS_CAN_CALL(2);
			
			private static final Map<Integer,PSTNCALLPOLICY> lookup = new HashMap<Integer,PSTNCALLPOLICY>();
			
			static {
				for(PSTNCALLPOLICY s : EnumSet.allOf(PSTNCALLPOLICY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private PSTNCALLPOLICY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static PSTNCALLPOLICY get(int code) {
				return lookup.get(code);
			}
			
			public static PSTNCALLPOLICY fromString(String s) {
				for (PSTNCALLPOLICY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_AVATAR_POLICY property that controls whether remote contacts can view local account's avatar image. Note that since this set of values is associated with a server-side property, you must set that property using Account.SetServersideIntPropertyserver-side. <br>Note that setting account's AVATAR_POLICY to BUDDIES_OR_AUTHORIZED_CAN_SEE does not guarantee that remote users will be able to immediately retrieve the avatar picture via corresponding Contact object. Avatar changes propagate between clients only when direct sessions between clients are established. Direct sessions are established during live sessions or whilst online contacts are engaged in chat. <br> */
		public enum AVATARPOLICY {
		
			/** Only authorized remote users can see this user's avatar image*/
			BUDDIES_OR_AUTHORIZED_CAN_SEE(0),
			
			/** Everyone can see this user's avatar image, once the contact/account avatar property has been synchronized during a direct session. The avatar image may also become viewable on some Skype Web-based services.*/
			EVERYONE_CAN_SEE(2);
			
			private static final Map<Integer,AVATARPOLICY> lookup = new HashMap<Integer,AVATARPOLICY>();
			
			static {
				for(AVATARPOLICY s : EnumSet.allOf(AVATARPOLICY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private AVATARPOLICY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static AVATARPOLICY get(int code) {
				return lookup.get(code);
			}
			
			public static AVATARPOLICY fromString(String s) {
				for (AVATARPOLICY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_BUDDYCOUNT_POLICY property that controls whether the number of this user's authorized contacts is visible to other users, either through Account.GetPropNrofAuthedBuddies or Contact.GetPropNrofAuthedBuddies when those instances reference this user. Note that since this set of values is associated with a server-side property, you must set that property using Account.SetServersideIntProperty, like this: <br>account->SetServersideIntProperty(Account.P_BUDDYCOUNT_POLICY, Account.DISCLOSE_TO_AUTHORIZED); <br>account->SetServersideIntProperty(Account.P_BUDDYCOUNT_POLICY, Account.DISCLOSE_TO_NOONE ); <br> */
		public enum BUDDYCOUNTPOLICY {
		
			/** Authorized remote users can retrieve the number of this user's authorized contacts (Contact.P_NROF_AUTHED_BUDDIES)*/
			DISCLOSE_TO_AUTHORIZED(0),
			
			/** No remote user - regardless their authorization status - can retrieve the number of this user's authorized contacts. Account.GetPropNrofAuthedBuddies and Contact.GetPropNrofAuthedBuddies will always return 0*/
			DISCLOSE_TO_NOONE(1);
			
			private static final Map<Integer,BUDDYCOUNTPOLICY> lookup = new HashMap<Integer,BUDDYCOUNTPOLICY>();
			
			static {
				for(BUDDYCOUNTPOLICY s : EnumSet.allOf(BUDDYCOUNTPOLICY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private BUDDYCOUNTPOLICY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static BUDDYCOUNTPOLICY get(int code) {
				return lookup.get(code);
			}
			
			public static BUDDYCOUNTPOLICY fromString(String s) {
				for (BUDDYCOUNTPOLICY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_TIMEZONEPOLICY property that sets the rules for timezone offset so remote clients can determine your local time. Note that since this set of values is associated with a server-side property, you must set that property using Account.SetServersideIntPropertyserver-side. */
		public enum TIMEZONEPOLICY {
		
			/** Account's timezone setting is determined automatically. <br>*/
			TZ_AUTOMATIC(0),
			
			/** Account's timezone setting is set manually. <br>*/
			TZ_MANUAL(1),
			
			/** Remote users will have no idea what your local time is. <br>*/
			TZ_UNDISCLOSED(2);
			
			private static final Map<Integer,TIMEZONEPOLICY> lookup = new HashMap<Integer,TIMEZONEPOLICY>();
			
			static {
				for(TIMEZONEPOLICY s : EnumSet.allOf(TIMEZONEPOLICY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private TIMEZONEPOLICY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static TIMEZONEPOLICY get(int code) {
				return lookup.get(code);
			}
			
			public static TIMEZONEPOLICY fromString(String s) {
				for (TIMEZONEPOLICY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_WEBPRESENCEPOLICY property that controls whether your online status (presence) can be seen using the "Skype buttons" ( http://www.skype.com/share/buttons/ ) embedded in web pages. Note that since this set of values is associated with a server-side property, you must set that property using Account.SetServersideIntPropertyserver-side. <br> */
		public enum WEBPRESENCEPOLICY {
		
			/** Disable displaying online status on web for this account.*/
			WEBPRESENCE_DISABLED(0),
			
			/** Enable displaying online status on web for this account.*/
			WEBPRESENCE_ENABLED(1);
			
			private static final Map<Integer,WEBPRESENCEPOLICY> lookup = new HashMap<Integer,WEBPRESENCEPOLICY>();
			
			static {
				for(WEBPRESENCEPOLICY s : EnumSet.allOf(WEBPRESENCEPOLICY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private WEBPRESENCEPOLICY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static WEBPRESENCEPOLICY get(int code) {
				return lookup.get(code);
			}
			
			public static WEBPRESENCEPOLICY fromString(String s) {
				for (WEBPRESENCEPOLICY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_PHONENUMBERSPOLICY property that controls whether unauthorized remote users can see associated phone numbers in their UI (for reference, see the different phone number tabs in Windows desktop Client contact view). Note that since this set of values is associated with a server-side property, you must set that property using Account.SetServersideIntProperty <br> */
		public enum PHONENUMBERSPOLICY {
		
			/** Only authorized contacts can see the phone numbers.*/
			PHONENUMBERS_VISIBLE_FOR_BUDDIES(0),
			
			/** Everyone can see the phone numbers.*/
			PHONENUMBERS_VISIBLE_FOR_EVERYONE(1);
			
			private static final Map<Integer,PHONENUMBERSPOLICY> lookup = new HashMap<Integer,PHONENUMBERSPOLICY>();
			
			static {
				for(PHONENUMBERSPOLICY s : EnumSet.allOf(PHONENUMBERSPOLICY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private PHONENUMBERSPOLICY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static PHONENUMBERSPOLICY get(int code) {
				return lookup.get(code);
			}
			
			public static PHONENUMBERSPOLICY fromString(String s) {
				for (PHONENUMBERSPOLICY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		Recognized values for the P_VOICEMAILPOLICY property that controls acceptance of incoming voicemail messages. Note that since this set of values is associated with a server-side property, you must set that property using Account.SetServersideIntPropertyserver-side. <br> */
		public enum VOICEMAILPOLICY {
		
			/** Incoming voicemails enabled.*/
			VOICEMAIL_ENABLED(0),
			
			/** Incoming voicemails disabled.*/
			VOICEMAIL_DISABLED(1);
			
			private static final Map<Integer,VOICEMAILPOLICY> lookup = new HashMap<Integer,VOICEMAILPOLICY>();
			
			static {
				for(VOICEMAILPOLICY s : EnumSet.allOf(VOICEMAILPOLICY.class))
					lookup.put(s.getId(), s);
			}
			
			private final int id;
			
			private VOICEMAILPOLICY(int value) {
				this.id = value;
			}
			
			public int getId() { return id; }
			
			public static VOICEMAILPOLICY get(int code) {
				return lookup.get(code);
			}
			
			public static VOICEMAILPOLICY fromString(String s) {
				for (VOICEMAILPOLICY p : lookup.values()) {
					if (p.toString() == s) {
						return p;
					}
				}
				return null;
			}
		}
		
		/**
		 *Setter for integer properties. For a list of writeable account profile properties, see the detailed description of the Account class. <br>
		 * @param propKey
		 * @param value
		 */
		public void SetIntProperty( int propKey, int value) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,16);
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
		 *Setter for String properties. For a list of writeable account profile properties, see the detailed description of the Account class. NB! You should pre-validate your about and mood message strings (Skype.ValidateProfileString) prior to invoking this method. <br>
		 * @param propKey
		 * @param value
		 */
		public void SetStrProperty( int propKey, String value) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,17);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,propKey);
			request.addParm('S',2,value);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Setter for BLOB properties, such as its avatar image. For a list of writeable account profile properties, see the detailed description of the Account class. NB! You should pre-validate your avatar image (Skype.ValidateAvatar) prior to invoking this method. <br>
		 * @param propKey
		 * @param value
		 */
		public void SetBinProperty( int propKey, byte[] value) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,18);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,propKey);
			request.addParm('B',2,value);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Sets online status of the currently logged in account to one of the values from Contact class AVAILABILITY enumerator. <br>
		 * @param availability only subset of all contact availabilities allowed
		 */
		public void SetAvailability( Contact.AVAILABILITY availability) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,19);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,availability.getId());
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Transitions the availability of this account's associated user, who is assumed to be logged in. <br>
		 * @param standby <br> - true: Saves the user's current availability, then sets it to CONTACT.AVAILABILITY.OFFLINE <br> - false: Reconnects the user and restores their previous availability
		 */
		public void SetStandby( boolean standby) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,10);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('b',1,standby);
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		Account capabability statuses are possible values of Contact class CAPABILITY enumerator, when that enumerator is used in context of account. Compared to Contact class CAPABILITYSTATUS enums, Account class CAPABILITYSTATUS has additional items for subscription expiration warnings. <br> */
		public enum CAPABILITYSTATUS {
		
			/** Capability is not supported by the currently logged in SkypeKit client.*/
			NO_CAPABILITY(0),
			
			/** Capability is supported by the currently logged in SkypeKit client. <br>*/
			CAPABILITY_EXISTS(1),
			
			/** Support for this capability ends this month (within 30 days) <br>*/
			FIRST_EXPIRY_WARNING(2),
			
			/** Support for this capability ends this week (within 7 days)  <br>*/
			SECOND_EXPIRY_WARNING(3),
			
			/** Support for this capability ends today <br>*/
			FINAL_EXPIRY_WARNING(4);
			
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
		 *Returns state of a given account capability. Takes Contact class CAPABILITY property as input argument and returns its state and expiration timestamp where appropriate. For example (C++ wrapper, with other wrappers the syntax may vary but the idea is the same) MyAccount.GetCapabilityStatus(Contact.CAPABILITY_SKYPEOUT, Cap, T); will return Account.CAPABILITY_EXISTS if local account has SkypeOut enabled. <br>
		 * @param capability
		 * @return GetCapabilityStatusResult
		 */
		public GetCapabilityStatusResult GetCapabilityStatus( Contact.CAPABILITY capability) {
		
			Request request = null;
			try {
				request = new XCallRequest(5,21);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('e',1,capability.getId());
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			GetCapabilityStatusResult result = new GetCapabilityStatusResult();
			
			CAPABILITYSTATUS capabilitystatus = null;
			capabilitystatus = CAPABILITYSTATUS.get(r.GetAsInt(1));
			result.status = capabilitystatus;
			
			int expiryTimestamp = 0;
			expiryTimestamp = r.GetAsInt(2);
			result.expiryTimestamp = expiryTimestamp;
			
			return result;
		}
		
		public class GetCapabilityStatusResult {
			public CAPABILITYSTATUS status;
			public int expiryTimestamp;
		}
		
		/**
		 *Response is empty when called with an inactive or invalid account
		 * @return skypenameHash
		 */
		public String GetSkypenameHash() {
		
			Request request = null;
			try {
				request = new XCallRequest(5,22);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			String skypenameHash = null;
			skypenameHash = r.GetAsString(1);
			return skypenameHash;
		}
		
		/**
		 *returns verified-by-Skype e-mail for this account if exists and verifiable
		 * @return email
		 */
		public String GetVerifiedEmail() {
		
			Request request = null;
			try {
				request = new XCallRequest(5,2);
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
		 *returns verified-by-Skype company for this account if exists and verifiable
		 * @return company
		 */
		public String GetVerifiedCompany() {
		
			Request request = null;
			try {
				request = new XCallRequest(5,3);
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
		 *Deletes all account data stored locally. Does not remove any account data from the server! <br>
		 */
		public void Delete() {
		
			Request request = null;
			try {
				request = new XCallRequest(5,24);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			skype.XCall((XCallRequest)request);
		}
		
	
	}