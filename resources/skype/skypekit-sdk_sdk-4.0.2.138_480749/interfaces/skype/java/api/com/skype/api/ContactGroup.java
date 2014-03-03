package com.skype.api;

import java.io.IOException;
import java.util.*;
import com.skype.ipc.*;
/**
 * Collects and manages Contacts related by type, status, or some other arbitrary criteria. SkypeKit recognizes two distinct ContactGroup flavors - "hardwired" and "custom". SkypeKit both defines the criteria for and dynamically manages all "hardwired" ContactGroups. Individual users explicitly create and manage all "custom" ContactGroups.  <br><br>"Hardwired" groups are primarily organizational tools, for example, they enable you to display a list of all Contacts awaiting authorization by you. "Custom" groups are also organizational tools, for example, they enable you to display a list of all Contacts in a particular geographical area or belonging to a particular professional association, social clubs, and so forth. Primarily, though, "custom" groups are functional tools that enable you to establish conference calls, group chats, and so forth. <br><br>"Hardwired" ContactGroups are defined for and available to all users. SkypeKit determines membership in a particular "hardwired" group dynamically whenever a user invokes Skype.GetHardwiredContactGroup for that group. Subsequent changes to a Contact's status might result in its being added to (for example, the Contact is now authorized) or removed from (for example, the Contact is now removed or blocked) one or more "hardwired" groups. <br><br>SkypeKit fires OnChange events for all affected ContractGroup instances. Essentially all ContactGroup methods related to explicitly adding and removing members and conversations from the group return false, and CanAdd and CanRemove additionally return a false result. <br><br>"Custom" ContactGroups can be defined by a particular Skype user through the UI. Your UI should implement Creation, deletion and filtering contact list by custom contact groups, as well as adding and removing contacts in those groups. <br><br>A Contact can belong to multiple non-mutually exclusive "hardwired" groups at the same time, for example, an authorized contact is typically in your "buddy" group, but a Contact cannot belong to CONTACTS_AUTHORIZED_BY_ME if they are awaiting authorization. Similarly, a Contact can belong to multiple "custom" groups and mutual exclusivity is typically not an issue. <br>
 */


public class ContactGroup extends SkypeObject {


	public interface ContactGroupListener {
		/** This event gets called when there are changes to ContactGroup properties defined in ContactGroup.PROPERTY  */
		public void OnPropertyChange(SkypeObject obj, PROPERTY prop, Object value);
		
		/**conversation added or removed from this group*/
		public void OnChangeConversation(SkypeObject obj, Conversation conversation);
		
		/**A contact has been added or removed to this ContactGroup. <br>NB! On rare occasions, the ContectRef argument to this callback can be NULL. You should always check whether the reference is valid, before accessing methods or properties. <br>*/
		public void OnChange(SkypeObject obj, Contact contact);
		
	}
	
	public ContactGroup(int oid, Skype skype) {
		super(oid,skype);
		/**get default properties for this module **/
		GetDefaultProps();
	}
	
	private static final int MODULE_ID = 10;
	
	public static final int moduleID() {
		return MODULE_ID;
	}
	
	/** Properties of the ContactGroup class */
	public enum PROPERTY {
	
		/** ContactGroup.TYPE, type: TYPE */
		type(155),
		
		/** unique 32-bit ID for custom groups, type: int */
		custom_group_id(154),
		
		/** change via ContactGroup.GiveDisplayname(), type: String */
		given_displayname(151),
		
		/** Number of contacts in the group. NB! The value of this property does not get updated until 5 seconds after account login. During these initial 5 seconds, the value of this property remains 0. The reason for this 5 second delay is to reduce the flurry of property update traffic that occurs during the CBL synchronization phase, following successful login. Note that if you absolutely need to have this value immediately after login, you can still get it by retrieving the contact list with ContactGroup.GetContacts method and examining its size.  <br>, type: int */
		nrofcontacts(152),
		
		/** number of contacts online in the group, type: int */
		nrofcontacts_online(153);
		
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
		GetPropertyRequest request = new GetPropertyRequest(10, mObjectId, prop.id);
		
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
		GetPropertyRequest request = new GetPropertyRequest(10, mObjectId, prop.id);
		
		byte [] data = null;
		GetPropertyResponse r = skype.GetProperty(request);
		if (r != null) {
			data = r.GetAsBinary();
		}
		return data;
	}
	
	/**default array of ContactGroup Properties that get fetched & cached upon class construction*/
	private static PROPERTY [] defaultProperties = { PROPERTY.type, PROPERTY.custom_group_id, PROPERTY.given_displayname, PROPERTY.nrofcontacts, PROPERTY.nrofcontacts_online};
	
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
		The list of all possible ContactGroup types. A value of this type can be passed to Skype class GetHardwiredContactGroup to retrieve the relevant ContactGroup object. <br> */
		public enum TYPE {
		
			/** The superset of all "hardwired" contact groups. <br>*/
			ALL_KNOWN_CONTACTS(1),
			
			/** The set of all authorized contacts, that is, contacts that were last the target of Contact.SetBuddyStatus(false) plus all SkypeOut contacts. <br>*/
			ALL_BUDDIES(2),
			
			/** The set of all authorized Skype contacts (Contact:_SKYPENAME is non-null).  <br>Note that this excludes Skype contacts that have either never been the target of Contact.SetBuddyStatus(true) or were last the target of Contactact.SetBuddyStatus(false). <br>*/
			SKYPE_BUDDIES(3),
			
			/** The set of all SkypeOut contacts (Contact:_PSTNNUMBER is non-null). PSTN contacts can be added to the contact list by retrieving a new contact object with Skype.GetContact, passing in the phone number as string, and then either using Contact.SetBuddyStatus(true) or adding the contact to the SKYPEOUT_BUDDIES group with ContactGroup.AddContact. <br>*/
			SKYPEOUT_BUDDIES(4),
			
			/** The subset of ALL_BUDDIES that are currently online, including those currently marked as DO_NOT_DISTURBED and AWAY. <br>*/
			ONLINE_BUDDIES(5),
			
			/** The set of all contacts whose Contact:_TYPE reflects UNRECOGNIZED OR have not authorized the local user yet. <br>*/
			UNKNOWN_OR_PENDINGAUTH_BUDDIES(6),
			
			/** This filter returns top 10 most recently contacted contacts, based on Contact.P_LASTUSED_TIMESTAMP property values. This is not configurable. Note that the P_LASTUSED_TIMESTAMP property does not propagate between different Skype instances - thus this filter only works in context of the local database. Recent contacts that were in touch with the user on some other Skype installation will not show up in this filter. <br>*/
			RECENTLY_CONTACTED_CONTACTS(7),
			
			/** Contacts to whose authorization request the user has not responded yet. The UI should enable the user to accept, decline the authorization request and in case of decline, optionally block further incoming communication from the contact. See: Contact.SetBuddyStatus, Contact.SetBlocked and Contact.IgnoreAuthRequest for more information. <br>*/
			CONTACTS_WAITING_MY_AUTHORIZATION(8),
			
			/** All contacts authorized by the user. <br>*/
			CONTACTS_AUTHORIZED_BY_ME(9),
			
			/** Group of contacts the user has blocked from further incoming communications. If the UI enables contact blocking, it should also provide interface for the user to unblock the blocked contacts. Note that a contact can simultaneously be in both CONTACTS_BLOCKED_BY_ME and CONTACTS_AUTHORIZED_BY_ME groups. <br>*/
			CONTACTS_BLOCKED_BY_ME(10),
			
			/** The set of all "buddies" that are not also a member of a custom group. <br>*/
			UNGROUPED_BUDDIES(11),
			
			/** A custom group defined by user. <br>*/
			CUSTOM_GROUP(12),
			
			/** The shared contact group functionality is no longer supported. This contact group type can be ignored. <br>*/
			PROPOSED_SHARED_GROUP(13),
			
			/** The shared contact group functionality is no longer supported. This contact group type can be ignored. <br>*/
			SHARED_GROUP(14),
			
			/** The set of all contacts that were originally imported from an external address book. <br>*/
			EXTERNAL_CONTACTS(15);
			
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
		 *Setter for ContactGroup class GIVEN_DISPLAYNAME property. <br>
		 * @param name
		 */
		public void GiveDisplayName( String name) {
		
			Request request = null;
			try {
				request = new XCallRequest(10,1);
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
		 *Removes the contact group. This is synced across instances logged in with the same account - which can take several minutes for the sync to happen. <br>
		 * @return result
		 */
		public boolean Delete() {
		
			Request request = null;
			try {
				request = new XCallRequest(10,2);
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
		 *Returns list of conversations in the ContactGroup. <br>
		 * @return conversations
		 */
		public Conversation [] GetConversations() {
		
			Request request = null;
			try {
				request = new XCallRequest(10,3);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return null;
				
			Vector<Conversation> conversations = new Vector<Conversation>();
			while (r.HasMore(1))
			{
				int oid = 0;
				Conversation conversation = null;
				oid = r.GetOid(1);
				if (oid != AbstractDecoder.NULL_VALUE) { 
					conversation = (Conversation)skype.factory(Conversation.moduleID(), oid, skype);
				}
				conversations.add(conversation);
			}
			return conversations.toArray(new Conversation[conversations.size()]);
			
		}
		
		/**
		 *Checks if the current user can add given conversation to the ContactGroup. Returns false for most of the hardwired contact groups for example. <br>
		 * @param conversation Conversation to be checked. <br>
		 * @return result Returns true if Conversation can be added to this ContactGroup. <br>
		 */
		public boolean CanAddConversation( Conversation conversation) {
		
			Request request = null;
			try {
				request = new XCallRequest(10,4);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,conversation.getOid());
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return false;
				
			boolean result = false;
			result = r.GetAsBoolean(1);
			return result;
		}
		
		/**
		 *Adds given conversation to the ContactGroup. <br>
		 * @param conversation
		 */
		public void AddConversation( Conversation conversation) {
		
			Request request = null;
			try {
				request = new XCallRequest(10,5);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,conversation.getOid());
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Checks if the current user can remove given conversation from the ContactGroup. Again, returns false for most hardwired contact groups. <br>
		 * @return result true if RemoveConversation(contact) works on this group
		 */
		public boolean CanRemoveConversation() {
		
			Request request = null;
			try {
				request = new XCallRequest(10,6);
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
		 *Removes given conversation from the ContactGroup. <br>
		 * @param conversation
		 */
		public void RemoveConversation( Conversation conversation) {
		
			Request request = null;
			try {
				request = new XCallRequest(10,7);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,conversation.getOid());
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Retrieves contact list. <br>
		 * @return contacts
		 */
		public Contact [] GetContacts() {
		
			Request request = null;
			try {
				request = new XCallRequest(10,8);
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
		 *Checks if the current user can add given contact to the ContactGroup.  <br>
		 * @param contact Contact to be checked. <br>
		 * @return result returns true if AddContact(contact) works on this group. <br>
		 */
		public boolean CanAddContact( Contact contact) {
		
			Request request = null;
			try {
				request = new XCallRequest(10,9);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,contact.getOid());
			
			Response r = skype.XCall((XCallRequest)request);
			
			if (r.isErrCall())
				return false;
				
			boolean result = false;
			result = r.GetAsBoolean(1);
			return result;
		}
		
		/**
		 *Adds contact to a contact group. This only works for non-hardwired contact groups. <br>
		 * @param contact
		 */
		public void AddContact( Contact contact) {
		
			Request request = null;
			try {
				request = new XCallRequest(10,10);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,contact.getOid());
			
			skype.XCall((XCallRequest)request);
		}
		
		/**
		 *Checks if the current user can remove given contact from the ContactGroup. <br>
		 * @return result true if RemoveContact(contact) works on this group
		 */
		public boolean CanRemoveContact() {
		
			Request request = null;
			try {
				request = new XCallRequest(10,11);
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
		 *Removes contact from the ContactGroup. <br>
		 * @param contact
		 */
		public void RemoveContact( Contact contact) {
		
			Request request = null;
			try {
				request = new XCallRequest(10,12);
			} catch (IOException e) {
				e.printStackTrace();
				if (skype.errorListener != null)
					skype.errorListener.OnSkypeKitFatalError();
			}
			request.addParm('O',0,mObjectId);
			request.addParm('O',1,contact.getOid());
			
			skype.XCall((XCallRequest)request);
		}
		
	
	}