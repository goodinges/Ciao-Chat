package com.skype.api;

import java.io.IOException;
import java.util.Collections;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import com.skype.api.Account.AccountListener;
import com.skype.api.Contact.ContactListener;
import com.skype.api.ContactGroup.ContactGroupListener;
import com.skype.api.ContactSearch.ContactSearchListener;
import com.skype.api.Conversation.ConversationListener;
import com.skype.api.Message.MessageListener;
import com.skype.api.Participant.DTMF;
import com.skype.api.Participant.ParticipantListener;
import com.skype.api.Sms.SmsListener;
import com.skype.api.Transfer.TransferListener;
import com.skype.api.Video.VideoListener;
import com.skype.api.Voicemail.VoicemailListener;
import com.skype.ipc.AbstractDecoder;
import com.skype.ipc.Event;
import com.skype.ipc.PropertyChange;
import com.skype.ipc.Request;
import com.skype.ipc.Response;
import com.skype.ipc.RootObject;
import com.skype.ipc.XCallRequest;

public class Skype extends RootObject {

	
	public interface SkypeListener {
		public void OnNewCustomContactGroup(ContactGroup group);
		
		/**This contact has appeared online. display alert*/
		public void OnContactOnlineAppearance(Contact contact);
		
		/**This contact has gone offline. display alert*/
		public void OnContactGoneOffline(Contact contact);
		
		/**This event gets fired when a Conversation item is added or removed from the list specified in the type argument. The primary use of this event is to detect creation of new Conversation objects. It can also be used for detecting occurance of live sessions - by monitoring added = true in Conversation.LIVE_CONVERSATIONS. Note that this method is not entirely sufficient for detecting live session termination (added = false and type = Conversation.LIVE_CONVERSATIONS). When the live session goes down, the default behaviour is that the Conversation object remains in the LIVE_CONVERSATIONS list for approximately 10 seconds. When another live session comes up within the same Conversation, the OnConversationListChange event will not fire - because the conversation was already in that list. There are two ways of getting around that. Firstly you can have all the conversations referenced at all times and then monitor Conversation.P_LOCAL_LIVESTATUS property changes, in which case you can pick up incoming live sessions from there. Alternatively, you can remove the delay between live session termination and conversation's removal from the LIVE_CONVERSATIONS list. This delay is controlled by the SETUPKEY_RECENTLY_LIVE_TIMEOUT setup key. To remove the delay, use Skype.SetInt(SETUPKEY_RECENTLY_LIVE_TIMEOUT, 0). Note that this setup key is account-based. You will need to have an account logged in in order to modify its value. <br>*/
		public void OnConversationListChange(Conversation conversation, Conversation.LIST_TYPE type, boolean added);
		
		public void OnMessage(Message message, boolean changesInboxTimestamp, Message supersedesHistoryMessage, Conversation conversation);
		
		/**This callback gets fired when there are changes in the system video device list (USB webcam gets plugged in or is detached.) <br>*/
		public void OnAvailableVideoDeviceListChange();
		
		/**Event is implemented only in SkypeKit builds. Fired when Skype video library uses software H264 codec for the first time on the particular hardware by particular SkypeKit-based application*/
		public void OnH264Activated();
		
		/**This callback gets fired when there are changes in the system audio device list (USB headset gets plugged in or is detached.) <br>*/
		public void OnAvailableDeviceListChange();
		
		/**This callback gets fired when the audio strength changes in either playback or recording audio streams. Useful for providing visual indicators of audio activity in your UI. <br>*/
		public void OnNrgLevelsChange();
		
		public void OnProxyAuthFailure(PROXYTYPE type);
		
	}
	
	public HashMap<Integer, SkypeObject> object_list_base;
	public HashMap<Integer, Object> listeners_base;
	public Map<Integer, SkypeObject> object_list;
	public Map<Integer, Object> listeners;
	
	private Skype skype;
	
	public Skype() {
		object_list_base = new HashMap<Integer, SkypeObject>();
		listeners_base = new HashMap<Integer, Object>();
		
		object_list = Collections.synchronizedMap(object_list_base);
		listeners = Collections.synchronizedMap(listeners_base);
		skype = this;
	}
	
	private static final int module_id = 0;
	
	public static final int getmoduleid() {
		return module_id;
	}
	
	/** Register listener interface, need to pass module id as first param*/
	public void RegisterListener(int modid, Object listener)
	{
		listeners.put(modid, listener);
	}
	
	/** Remove listener interface, need to pass module id as first param*/
	public void UnRegisterListener(int modid, Object listener)
	{
		if (listeners.containsKey(modid))
			listeners.remove(modid);
	}
	
	public void HandlePropertyChange(PropertyChange pc){
		SkypeObject obj = null;
		Object val = null;
		Object prop = null;
		
		//check object exists in list and get value 
		if (object_list.containsKey(pc.oid))
		{
			obj = object_list.get(pc.oid);
			int kind = pc.GetKind();
			
			switch (kind) {
				case 'i':
				case 'u':
				case 'e':
					try {
						val = new Integer(pc.GetAsInt());
					} catch (IOException e) {
						e.printStackTrace();
						if (errorListener!=null)
							errorListener.OnSkypeKitFatalError();
					}
					break;
				case 'T':
				case 'F':
					try {
						val = new Boolean(pc.GetAsBoolean());
					} catch (IOException e) {
						e.printStackTrace();
						if (errorListener!=null)
							errorListener.OnSkypeKitFatalError();
					}
					break;
				case 'S':
				case 'X':
				case 'f':
					try {
						val = new String(pc.GetAsString());
					} catch (IOException e) {
						e.printStackTrace();
						if (errorListener!=null)
							errorListener.OnSkypeKitFatalError();
					}
					break;
				case 'B':
					if (pc.hasValue()){
						try {
							val = new String(pc.GetAsBinary(0));
						} catch (IOException e) {
							e.printStackTrace();
							if (errorListener!=null)
								errorListener.OnSkypeKitFatalError();
						}
					}
					break;
				default:
					break;
			}
			
			//update the property cache
			if (val!=null) {
				obj.mPropCache.put(pc.propid, val);
			}
			else {
				obj.mPropCache.remove(pc.propid);
			}
			//call listener if this exists?
			if (listeners.containsKey(pc.moid))
			 {
				Object listener = listeners.get(pc.moid);
				if (listener != null){
					prop = obj.GetPropertyAsEnum(pc.propid);
					
					switch (pc.moid)
					{
					case 5: //Account
						((AccountListener) listener).OnPropertyChange(obj, (Account.PROPERTY)prop, val);
						break;
						
					case 2: //Contact
						((ContactListener) listener).OnPropertyChange(obj, (Contact.PROPERTY)prop, val);
						break;
						
					case 10: //ContactGroup
						((ContactGroupListener) listener).OnPropertyChange(obj, (ContactGroup.PROPERTY)prop, val);
						break;
						
					case 1: //ContactSearch
						((ContactSearchListener) listener).OnPropertyChange(obj, (ContactSearch.PROPERTY)prop, val);
						break;
						
					case 18: //Conversation
						((ConversationListener) listener).OnPropertyChange(obj, (Conversation.PROPERTY)prop, val);
						break;
						
					case 9: //Message
						((MessageListener) listener).OnPropertyChange(obj, (Message.PROPERTY)prop, val);
						break;
						
					case 19: //Participant
						((ParticipantListener) listener).OnPropertyChange(obj, (Participant.PROPERTY)prop, val);
						break;
						
					case 12: //Sms
						((SmsListener) listener).OnPropertyChange(obj, (Sms.PROPERTY)prop, val);
						break;
						
					case 6: //Transfer
						((TransferListener) listener).OnPropertyChange(obj, (Transfer.PROPERTY)prop, val);
						break;
						
					case 11: //Video
						((VideoListener) listener).OnPropertyChange(obj, (Video.PROPERTY)prop, val);
						break;
						
					case 7: //Voicemail
						((VoicemailListener) listener).OnPropertyChange(obj, (Voicemail.PROPERTY)prop, val);
						break;
						
					default:
						break;
					}
				}
			}
		}
	}
	
	@Override
	public void Close() throws IOException {
		FlushObjects();
		super.Close();
	}
	
	/**Clear all Skypekit Objects and PROPERTIES in object cache */
	public void FlushObjects()
	{
		Iterator<Integer> iter = object_list.keySet().iterator();
		while (iter.hasNext()) {
			Integer key = iter.next();
			SkypeObject obj  = object_list.get(key);
			obj.flush_cache();
		}
		object_list.clear();
	}
	
	/** Mobile Mode related keys for Skypekit runtime*/
	private static final String mobileModeEnabled        = "*Lib/PM/MobileModeEnabled";
	private static final String superNodeKeepalivePeriod = "*Lib/Connection/SupernodeKeepalivePeriod";
	private static final String statsUpdatePeriod        = "*Lib/Connection/StatsUpdatePeriod";
	
	private static final String IgnoreReadChatMsg     = "*Lib/Chat/IgnoreReadChatMsg";
	
	/** Turn on Mobile Optimisations in Skypekit runtime and configure Skypekit not to send already read messages*/
	public void SetMobileMode () {
		SetInt(mobileModeEnabled, 1);
		SetInt(superNodeKeepalivePeriod, 600);
		SetInt(statsUpdatePeriod, 600);
		SetInt(IgnoreReadChatMsg, 1);}
	
	/**Static method to retrieve Voicemail object from object id*/
	public Voicemail GetVoiceMailFromId(int oid, Skype skype)
	{
		Voicemail vm = null;
		if (skype.object_list.containsKey(oid))
		{
			vm = (Voicemail) skype.object_list.get(oid);
		}
		else
		{
			vm = new Voicemail(oid,skype);
		}
		return vm;
	}
	
	
	@Override
	public void HandleEvent(Event e) {
	
		HandleEvent_inner(e);
	
	}
	public void HandleEvent_inner(Event e) {
		if (listeners.containsKey(e.getModuleId())) {
		
			switch (e.getModuleId()) {
			case 0: // Skype Listener Events
				SkypeListener skypelistener = (SkypeListener) listeners.get(e.getModuleId());
				if (skypelistener != null) {
				
					switch (e.getEventId()) {
					case 1:  // OnNewCustomContactGroup
						int  group01_oid = 0;
						ContactGroup group01 = null;
						group01_oid = e.GetOid(1);
						if (group01_oid != AbstractDecoder.NULL_VALUE) {
							group01 = (ContactGroup)skype.factory(ContactGroup.moduleID(), group01_oid, skype);
						}
						skypelistener.OnNewCustomContactGroup(group01);
						break;
						
					case 2:  // OnContactOnlineAppearance
						int  contact02_oid = 0;
						Contact contact02 = null;
						contact02_oid = e.GetOid(1);
						if (contact02_oid != AbstractDecoder.NULL_VALUE) {
							contact02 = (Contact)skype.factory(Contact.moduleID(), contact02_oid, skype);
						}
						skypelistener.OnContactOnlineAppearance(contact02);
						break;
						
					case 3:  // OnContactGoneOffline
						int  contact03_oid = 0;
						Contact contact03 = null;
						contact03_oid = e.GetOid(1);
						if (contact03_oid != AbstractDecoder.NULL_VALUE) {
							contact03 = (Contact)skype.factory(Contact.moduleID(), contact03_oid, skype);
						}
						skypelistener.OnContactGoneOffline(contact03);
						break;
						
					case 4:  // OnConversationListChange
						int  conversation04_oid = 0;
						Conversation conversation04 = null;
						conversation04_oid = e.GetOid(1);
						if (conversation04_oid != AbstractDecoder.NULL_VALUE) {
							conversation04 = (Conversation)skype.factory(Conversation.moduleID(), conversation04_oid, skype);
						}
						Conversation.LIST_TYPE type04 = null;
						type04 = Conversation.LIST_TYPE.get(e.GetAsInt(2));
						boolean added04 = false;
						added04 = e.GetAsBoolean(3);
						skypelistener.OnConversationListChange(conversation04, type04, added04);
						break;
						
					case 5:  // OnMessage
						int  message05_oid = 0;
						Message message05 = null;
						message05_oid = e.GetOid(1);
						if (message05_oid != AbstractDecoder.NULL_VALUE) {
							message05 = (Message)skype.factory(Message.moduleID(), message05_oid, skype);
						}
						boolean changesInboxTimestamp05 = false;
						changesInboxTimestamp05 = e.GetAsBoolean(2);
						int  supersedesHistoryMessage05_oid = 0;
						Message supersedesHistoryMessage05 = null;
						supersedesHistoryMessage05_oid = e.GetOid(3);
						if (supersedesHistoryMessage05_oid != AbstractDecoder.NULL_VALUE) {
							supersedesHistoryMessage05 = (Message)skype.factory(Message.moduleID(), supersedesHistoryMessage05_oid, skype);
						}
						int  conversation05_oid = 0;
						Conversation conversation05 = null;
						conversation05_oid = e.GetOid(4);
						if (conversation05_oid != AbstractDecoder.NULL_VALUE) {
							conversation05 = (Conversation)skype.factory(Conversation.moduleID(), conversation05_oid, skype);
						}
						skypelistener.OnMessage(message05, changesInboxTimestamp05, supersedesHistoryMessage05, conversation05);
						break;
						
					case 7:  // OnAvailableVideoDeviceListChange
						skypelistener.OnAvailableVideoDeviceListChange();
						break;
						
					case 44:  // OnH264Activated
						skypelistener.OnH264Activated();
						break;
						
					case 10:  // OnAvailableDeviceListChange
						skypelistener.OnAvailableDeviceListChange();
						break;
						
					case 11:  // OnNrgLevelsChange
						skypelistener.OnNrgLevelsChange();
						break;
						
					case 12:  // OnProxyAuthFailure
						PROXYTYPE type012 = null;
						type012 = PROXYTYPE.get(e.GetAsInt(1));
						skypelistener.OnProxyAuthFailure(type012);
						break;
						
					default:
						break;
						
					}
				}
				break;
				
				case 10: // Module ContactGroup
					ContactGroupListener contactgrouplistener = (ContactGroupListener)listeners.get(e.getModuleId());
					if (contactgrouplistener != null) {
						int contactgroup_oid = e.GetOid(0);
						SkypeObject obj = skype.factory(e.getModuleId(), contactgroup_oid, skype);
						
						switch (e.getEventId()){ 
						case 1: // Event OnChangeConversation
							int conversation101_oid = 0;
							Conversation conversation101 = null;
							conversation101_oid = e.GetOid(1);
							if (conversation101_oid != AbstractDecoder.NULL_VALUE) { 
								conversation101 = (Conversation)skype.factory(Conversation.moduleID(), conversation101_oid, skype);
							}
							contactgrouplistener.OnChangeConversation(obj, conversation101);
							break;
							
						case 2: // Event OnChange
							int contact102_oid = 0;
							Contact contact102 = null;
							contact102_oid = e.GetOid(1);
							if (contact102_oid != AbstractDecoder.NULL_VALUE) { 
								contact102 = (Contact)skype.factory(Contact.moduleID(), contact102_oid, skype);
							}
							contactgrouplistener.OnChange(obj, contact102);
							break;
							
						default:
							break;
							
						}
					}
					break;
					
				case 1: // Module ContactSearch
					ContactSearchListener contactsearchlistener = (ContactSearchListener)listeners.get(e.getModuleId());
					if (contactsearchlistener != null) {
						int contactsearch_oid = e.GetOid(0);
						SkypeObject obj = skype.factory(e.getModuleId(), contactsearch_oid, skype);
						
						switch (e.getEventId()){ 
						case 1: // Event OnNewResult
							int contact11_oid = 0;
							Contact contact11 = null;
							contact11_oid = e.GetOid(1);
							if (contact11_oid != AbstractDecoder.NULL_VALUE) { 
								contact11 = (Contact)skype.factory(Contact.moduleID(), contact11_oid, skype);
							}
							int rankValue11 = 0;
							rankValue11 = e.GetAsInt(2);
							contactsearchlistener.OnNewResult(obj, contact11, rankValue11);
							break;
							
						default:
							break;
							
						}
					}
					break;
					
				case 19: // Module Participant
					ParticipantListener participantlistener = (ParticipantListener)listeners.get(e.getModuleId());
					if (participantlistener != null) {
						int participant_oid = e.GetOid(0);
						SkypeObject obj = skype.factory(e.getModuleId(), participant_oid, skype);
						
						switch (e.getEventId()){ 
						case 1: // Event OnIncomingDTMF
							DTMF dtmf191 = null;
							dtmf191 = DTMF.get(e.GetAsInt(1));
							participantlistener.OnIncomingDTMF(obj, dtmf191);
							break;
							
						default:
							break;
							
						}
					}
					break;
					
				case 18: // Module Conversation
					ConversationListener conversationlistener = (ConversationListener)listeners.get(e.getModuleId());
					if (conversationlistener != null) {
						int conversation_oid = e.GetOid(0);
						SkypeObject obj = skype.factory(e.getModuleId(), conversation_oid, skype);
						
						switch (e.getEventId()){ 
						case 1: // Event OnParticipantListChange
							conversationlistener.OnParticipantListChange(obj);
							break;
							
						case 2: // Event OnMessage
							int message182_oid = 0;
							Message message182 = null;
							message182_oid = e.GetOid(1);
							if (message182_oid != AbstractDecoder.NULL_VALUE) { 
								message182 = (Message)skype.factory(Message.moduleID(), message182_oid, skype);
							}
							conversationlistener.OnMessage(obj, message182);
							break;
							
						case 3: // Event OnSpawnConference
							int spawned183_oid = 0;
							Conversation spawned183 = null;
							spawned183_oid = e.GetOid(1);
							if (spawned183_oid != AbstractDecoder.NULL_VALUE) { 
								spawned183 = (Conversation)skype.factory(Conversation.moduleID(), spawned183_oid, skype);
							}
							conversationlistener.OnSpawnConference(obj, spawned183);
							break;
							
						default:
							break;
							
						}
					}
					break;
					
				case 11: // Module Video
					VideoListener videolistener = (VideoListener)listeners.get(e.getModuleId());
					if (videolistener != null) {
						int video_oid = e.GetOid(0);
						SkypeObject obj = skype.factory(e.getModuleId(), video_oid, skype);
						
						switch (e.getEventId()){ 
						case 2: // Event OnCaptureRequestCompleted
							int requestId112 = 0;
							requestId112 = e.GetAsInt(1);
							boolean isSuccessful112 = false;
							isSuccessful112 = e.GetAsBoolean(2);
							byte[] image112 = null;
							image112 = e.GetAsBinary(3);
							int width112 = 0;
							width112 = e.GetAsInt(4);
							int height112 = 0;
							height112 = e.GetAsInt(5);
							videolistener.OnCaptureRequestCompleted(obj, requestId112, isSuccessful112, image112, width112, height112);
							break;
							
						default:
							break;
							
						}
					}
					break;
					
				default:
					break;
					
			}
		}
	}
	
	/** Setupkey SETUPKEY_DB_STORAGE_QUOTA_KB type:int default value:"0" <br>Use this key to limit the size of the main.db file. Value is in KB. Quota are disabled by default. <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DB_STORAGE_QUOTA_KB = "*Lib/DbManager/StorageQuotaKb";
	
	/**
	 *returns the runtime version as a string
	 * @return version
	 */
	public String GetVersionString() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,28);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		String version = null;
		version = r.GetAsString(1);
		return version;
	}
	
	/**
	 *Returns the time as used in SkypeKit, in the form of a Unix timestamp (number of seconds since 1.1.1970).                   If the local system time is incorrect my more than one year, the time provided                   by the Skype network will be provided, which is correct. Therefore this function                   can be used to adjust the system time if set incorrectly (e.g. if set to 1.1.1970).
	 * @return timestamp
	 */
	public int GetUnixTimestamp() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,134);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return 0;
			
		int timestamp = 0;
		timestamp = r.GetAsInt(1);
		return timestamp;
	}
	
	/**
	 *Takes TYPE argument (TYPE comes from ContactGroup class) and returns reference to the corresponding hardwired contact group. For example (C++): skype->GetHardwiredContactGroup(ContactGroup.ONLINE_BUDDIES, GroupRef) would return the list of all contacts that are currently online. <br>
	 * @param type
	 * @return contactGroup
	 */
	public ContactGroup GetHardwiredContactGroup( ContactGroup.TYPE type) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,1);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('e',1,type.getId());
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		int oid = 0;
		ContactGroup contactGroup = null;
		oid = r.GetOid(1);
		if (oid != AbstractDecoder.NULL_VALUE) {
			contactGroup = (ContactGroup)skype.factory(ContactGroup.moduleID(), oid, skype);
		}
		return contactGroup;
	}
	
	/**
	 *Returns a list of custom contact group references, i.e. all contact groups that are not hardwired. <br>
	 * @return groups
	 */
	public ContactGroup [] GetCustomContactGroups() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,2);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		Vector<ContactGroup> groups = new Vector<ContactGroup>();
		while (r.HasMore(1))
		{
			int oid = 0;
			ContactGroup contactgroup = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) { 
				contactgroup = (ContactGroup)skype.factory(ContactGroup.moduleID(), oid, skype);
			}
			groups.add(contactgroup);
		}
		return groups.toArray(new ContactGroup[groups.size()]);
		
	}
	
	/**
	 *Creates a new empty contact group object and returns a reference to it. The group will then show up in the custom group list that you can get with Skype class GetCustomContactGroups method. Existing contacts can be added to the new group with ContactGroup class AddContact method and a custom name can be given to it with GiveDisplayName method. <br>Note that no check is made for existing of displaynames with the same name - if you wish to force uniqueness in custom group names you will have to check that yourself before creating the group. <br>
	 * @return group
	 */
	public ContactGroup CreateCustomContactGroup() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,3);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		int oid = 0;
		ContactGroup group = null;
		oid = r.GetOid(1);
		if (oid != AbstractDecoder.NULL_VALUE) {
			group = (ContactGroup)skype.factory(ContactGroup.moduleID(), oid, skype);
		}
		return group;
	}
	
	/**
	 *analyzes the identity for contact type
	 * @param identity
	 * @return type
	 */
	public Contact.TYPE GetContactType( String identity) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,5);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,identity);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		Contact.TYPE type = null;
		type = Contact.TYPE.get(r.GetAsInt(1));
		return type;
	}
	
	/**
	 *Returns a Contact object reference. If a matching contact is not found in the existing contact list, a new Contact object will be created. Note that if you pass in a phone number in the identity argument, the type for the newly created Contact will be automatically set to Contact.PSTN (Contact.SKYPE otherwise). <br>
	 * @param identity Either skypename or a phone number <br>
	 * @return contact Returns a contact object. <br>
	 */
	public Contact GetContact( String identity) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,6);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,identity);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		int oid = 0;
		Contact contact = null;
		oid = r.GetOid(2);
		if (oid != AbstractDecoder.NULL_VALUE) {
			contact = (Contact)skype.factory(Contact.moduleID(), oid, skype);
		}
		return contact;
	}
	
	/**
	 * @param number
	 * @return FindContactByPstnNumberResult
	 */
	public FindContactByPstnNumberResult FindContactByPstnNumber( String number) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,8);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,number);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		FindContactByPstnNumberResult result = new FindContactByPstnNumberResult();
		
		boolean found = false;
		found = r.GetAsBoolean(1);
		result.found = found;
		
		int oid = 0;
		Contact contact = null;
		oid = r.GetOid(2);
		if (oid != AbstractDecoder.NULL_VALUE) {
			contact = (Contact)skype.factory(Contact.moduleID(), oid, skype);
		}
		result.contact = contact;
		
		int foundInKey = 0;
		foundInKey = r.GetAsInt(3);
		result.foundInKey = foundInKey;
		
		return result;
	}
	
	public class FindContactByPstnNumberResult {
		public boolean found;
		public Contact contact;
		public int foundInKey; /** type is actually PROPKEY */
	}
	
	/**
	 */
	public enum IDENTITYTYPE {
	
		/** */
		UNRECOGNIZED(0),
		
		/** */
		SKYPE(1),
		
		/** */
		SKYPE_MYSELF(2),
		
		/** */
		SKYPE_UNDISCLOSED(3),
		
		/** */
		PSTN(4),
		
		/** */
		PSTN_EMERGENCY(5),
		
		/** */
		PSTN_FREE(6),
		
		/** */
		PSTN_UNDISCLOSED(7),
		
		/** */
		CONFERENCE(8),
		
		/** */
		EXTERNAL(9);
		
		private static final Map<Integer,IDENTITYTYPE> lookup = new HashMap<Integer,IDENTITYTYPE>();
		
		static {
			for(IDENTITYTYPE s : EnumSet.allOf(IDENTITYTYPE.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private IDENTITYTYPE(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static IDENTITYTYPE get(int code) {
			return lookup.get(code);
		}
		
		public static IDENTITYTYPE fromString(String s) {
			for (IDENTITYTYPE p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *This takes skypename or a phone number string as argument and returns corresponding identity type (SKYPE, SKYPE_MYSELF, PSTN, etc.) <br>
	 * @param identity
	 * @return type
	 */
	public IDENTITYTYPE GetIdentityType( String identity) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,19);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,identity);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		IDENTITYTYPE identitytype = null;
		identitytype = IDENTITYTYPE.get(r.GetAsInt(1));
		return identitytype;
	}
	
	/**
	 */
	public enum NORMALIZERESULT {
	
		/** */
		IDENTITY_OK(0),
		
		/** */
		IDENTITY_EMPTY(1),
		
		/** */
		IDENTITY_TOO_LONG(2),
		
		/** */
		IDENTITY_CONTAINS_INVALID_CHAR(3),
		
		/** */
		PSTN_NUMBER_TOO_SHORT(4),
		
		/** identity looks like pstn number but does not start with +/00/011*/
		PSTN_NUMBER_HAS_INVALID_PREFIX(5),
		
		/** */
		SKYPENAME_STARTS_WITH_NONALPHA(6),
		
		/** returned only when isNewSkypeName*/
		SKYPENAME_SHORTER_THAN_6_CHARS(7);
		
		private static final Map<Integer,NORMALIZERESULT> lookup = new HashMap<Integer,NORMALIZERESULT>();
		
		static {
			for(NORMALIZERESULT s : EnumSet.allOf(NORMALIZERESULT.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private NORMALIZERESULT(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static NORMALIZERESULT get(int code) {
			return lookup.get(code);
		}
		
		public static NORMALIZERESULT fromString(String s) {
			for (NORMALIZERESULT p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *compares two identities to see if they match
	 * @param identityA
	 * @param identityB
	 * @return result
	 */
	public boolean IdentitiesMatch( String identityA, String identityB) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,88);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,identityA);
		request.addParm('S',2,identityB);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean result = false;
		result = r.GetAsBoolean(1);
		return result;
	}
	
	/**
	 *This method is deprecated. Use ValidateProfileString method instead. <br>
	 * @param original
	 * @param isNewSkypeName
	 * @return NormalizeIdentityResult
	 */
	public NormalizeIdentityResult NormalizeIdentity( String original, boolean isNewSkypeName) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,9);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,original);
		request.addParm('b',2,isNewSkypeName);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		NormalizeIdentityResult result = new NormalizeIdentityResult();
		
		NORMALIZERESULT normalizeresult = null;
		normalizeresult = NORMALIZERESULT.get(r.GetAsInt(1));
		result.result = normalizeresult;
		
		String normalized = null;
		normalized = r.GetAsString(2);
		result.normalized = normalized;
		
		return result;
	}
	
	public class NormalizeIdentityResult {
		public NORMALIZERESULT result;
		public String normalized;
	}
	
	/**
	 *NormalizePSTNWithCountry checks if the phone number starts with + if it doesn't, it prefixes the output with +XXX (where XXX is the country code). It also converts letters to numbers based on the standard phone keypad, so that the phone number string 212CALLME1 with country code 372 (Estonia) would be normalized to +3722122255631. If the method cannot normalize the phone number (because it's too long, too short, etc.), it returns an error code in &result. <br>
	 * @param original
	 * @param countryPrefix
	 * @return NormalizePSTNWithCountryResult
	 */
	public NormalizePSTNWithCountryResult NormalizePSTNWithCountry( String original, int countryPrefix) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,205);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,original);
		request.addParm('u',2,countryPrefix);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		NormalizePSTNWithCountryResult result = new NormalizePSTNWithCountryResult();
		
		NORMALIZERESULT normalizeresult = null;
		normalizeresult = NORMALIZERESULT.get(r.GetAsInt(1));
		result.result = normalizeresult;
		
		String normalized = null;
		normalized = r.GetAsString(2);
		result.normalized = normalized;
		
		return result;
	}
	
	public class NormalizePSTNWithCountryResult {
		public NORMALIZERESULT result;
		public String normalized;
	}
	
	/**
	 *list of (min,max) pairs
	 * @return rangeList
	 */
	public int [] GetOptimalAgeRanges() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,77);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		Vector<Integer> rangeList = new Vector<Integer>();
		while (r.HasMore(1))
		{
			Integer integer = null;
			integer = new Integer(r.GetAsInt(1));
			rangeList.add(integer);
		}
		
		int[] intArray = new int[rangeList.size()];
		for (int i = 0; i < rangeList.size(); i++) {
			intArray[i] = rangeList.get(i);
		}
		return intArray;
	}
	
	/**
	 *Creates a blank contact search object, in which you can add your custom search terms. For more information how asynchronous contact search works, see ContactSearch class details. <br>
	 * @return search Returns blank ContactSearch object. <br>
	 */
	public ContactSearch CreateContactSearch() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,10);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		int oid = 0;
		ContactSearch search = null;
		oid = r.GetOid(1);
		if (oid != AbstractDecoder.NULL_VALUE) {
			search = (ContactSearch)skype.factory(ContactSearch.moduleID(), oid, skype);
		}
		return search;
	}
	
	/**
	 *searches skypenames, aliases, fullnames, emails. false if not valid
	 * @param text
	 * @return search
	 */
	public ContactSearch CreateBasicContactSearch( String text) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,11);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,text);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		int oid = 0;
		ContactSearch search = null;
		oid = r.GetOid(1);
		if (oid != AbstractDecoder.NULL_VALUE) {
			search = (ContactSearch)skype.factory(ContactSearch.moduleID(), oid, skype);
		}
		return search;
	}
	
	/**
	 *searches skypenames and aliases. returns 0 or 1 results. false if not valid
	 * @param identity
	 * @return search
	 */
	public ContactSearch CreateIdentitySearch( String identity) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,12);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,identity);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		int oid = 0;
		ContactSearch search = null;
		oid = r.GetOid(1);
		if (oid != AbstractDecoder.NULL_VALUE) {
			search = (ContactSearch)skype.factory(ContactSearch.moduleID(), oid, skype);
		}
		return search;
	}
	
	/**
	sync failure reasons when starting a transfer */
	public enum TRANSFER_SENDFILE_ERROR {
	
		/** */
		TRANSFER_OPEN_SUCCESS(0),
		
		/** */
		TRANSFER_BAD_FILENAME(1),
		
		/** */
		TRANSFER_OPEN_FAILED(2),
		
		/** */
		TRANSFER_TOO_MANY_PARALLEL(3);
		
		private static final Map<Integer,TRANSFER_SENDFILE_ERROR> lookup = new HashMap<Integer,TRANSFER_SENDFILE_ERROR>();
		
		static {
			for(TRANSFER_SENDFILE_ERROR s : EnumSet.allOf(TRANSFER_SENDFILE_ERROR.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private TRANSFER_SENDFILE_ERROR(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static TRANSFER_SENDFILE_ERROR get(int code) {
			return lookup.get(code);
		}
		
		public static TRANSFER_SENDFILE_ERROR fromString(String s) {
			for (TRANSFER_SENDFILE_ERROR p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	Conversation and live state leave reasons for Participants. This type is used for the Participant.P_LAST_LEAVEREASON property. The primary use of this type is to provide detailed cause in case of a call failure. <br> */
	public enum LEAVE_REASON {
	
		/** */
		LEAVE_REASON_NONE(0),
		
		/** automatic, user cannot chat (only some older versions might set this)*/
		RETIRED_USER_INCAPABLE(2),
		
		/** automatic*/
		RETIRED_ADDER_MUST_BE_FRIEND(3),
		
		/** automatic*/
		RETIRED_ADDER_MUST_BE_AUTHORIZED(4),
		
		/** manual reason (afaik no UI uses this)*/
		RETIRED_DECLINE_ADD(5),
		
		/** manual reason*/
		RETIRED_UNSUBSCRIBE(6),
		
		/** */
		LIVE_NO_ANSWER(100),
		
		/** live: User hung up*/
		LIVE_MANUAL(101),
		
		/** */
		LIVE_BUSY(102),
		
		/** */
		LIVE_CONNECTION_DROPPED(103),
		
		/** */
		LIVE_NO_SKYPEOUT_SUBSCRIPTION(104),
		
		/** */
		LIVE_INSUFFICIENT_FUNDS(105),
		
		/** */
		LIVE_INTERNET_CONNECTION_LOST(106),
		
		/** */
		LIVE_SKYPEOUT_ACCOUNT_BLOCKED(107),
		
		/** */
		LIVE_PSTN_COULD_NOT_CONNECT_TO_SKYPE_PROXY(108),
		
		/** */
		LIVE_PSTN_INVALID_NUMBER(109),
		
		/** */
		LIVE_PSTN_NUMBER_FORBIDDEN(110),
		
		/** */
		LIVE_PSTN_CALL_TIMED_OUT(111),
		
		/** */
		LIVE_PSTN_BUSY(112),
		
		/** */
		LIVE_PSTN_CALL_TERMINATED(113),
		
		/** */
		LIVE_PSTN_NETWORK_ERROR(114),
		
		/** */
		LIVE_NUMBER_UNAVAILABLE(115),
		
		/** */
		LIVE_PSTN_CALL_REJECTED(116),
		
		/** */
		LIVE_PSTN_MISC_ERROR(117),
		
		/** */
		LIVE_INTERNAL_ERROR(118),
		
		/** */
		LIVE_UNABLE_TO_CONNECT(119),
		
		/** live: Voicemail recording failed*/
		LIVE_RECORDING_FAILED(120),
		
		/** live: Voicemail playback failed*/
		LIVE_PLAYBACK_ERROR(121),
		
		/** */
		LIVE_LEGACY_ERROR(122),
		
		/** */
		LIVE_BLOCKED_BY_PRIVACY_SETTINGS(123),
		
		/** live: Fallback error*/
		LIVE_ERROR(124),
		
		/** */
		LIVE_TRANSFER_FAILED(125),
		
		/** */
		LIVE_TRANSFER_INSUFFICIENT_FUNDS(126),
		
		/** */
		LIVE_BLOCKED_BY_US(127),
		
		/** */
		LIVE_EMERGENCY_CALL_DENIED(128);
		
		private static final Map<Integer,LEAVE_REASON> lookup = new HashMap<Integer,LEAVE_REASON>();
		
		static {
			for(LEAVE_REASON s : EnumSet.allOf(LEAVE_REASON.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private LEAVE_REASON(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static LEAVE_REASON get(int code) {
			return lookup.get(code);
		}
		
		public static LEAVE_REASON fromString(String s) {
			for (LEAVE_REASON p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *Creates a new empty conversation object and returns a reference to it. <br>
	 * @return conference
	 */
	public Conversation CreateConference() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,13);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
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
	 *Returns reference tp conversation object by conversation ID string (equivalent of old chat ID). NB! ID here is that of conversation, rather than skypename of dialog partner. If you want to retrieve a conversation object with any particular person, then Skype class GetConversationByParticipants method is what you are looking for. <br>
	 * @param convoIdentity
	 * @return conversation
	 */
	public Conversation GetConversationByIdentity( String convoIdentity) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,15);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,convoIdentity);
		
		Response r = XCall((XCallRequest)request);
		
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
	 *myself not included
	 * @param participantIdentities
	 * @param createIfNonExisting
	 * @param ignoreBookmarkedOrNamed
	 * @return conversation
	 */
	public Conversation GetConversationByParticipants( String [] participantIdentities, boolean createIfNonExisting, boolean ignoreBookmarkedOrNamed) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,16);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addListStart(1);
		for (int i=0;i<participantIdentities.length;i++) {
			request.addParm('S',participantIdentities[i]);
		}
		request.addParm('b',2,createIfNonExisting);
		request.addParm('b',3,ignoreBookmarkedOrNamed);
		
		Response r = XCall((XCallRequest)request);
		
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
	 *Retrieves a Conversation object by Public Conversation BLOB. Public conversation blobs are globally unique conversation IDs that provide a method for joining conversation without explicitly being added to the conversation by someone already in it. Programmatically, a Conversation BLOB can be retrieved with Conversation.GetJoinBlob method. In Skype desktop clients, the BLOB can be retrieved by typing "/get uri" in a conversation. The conversation can then be joined by people who have somehow received that BLOB. <br>
	 * @param joinBlob The BLOB string. <br>
	 * @param alsoJoin If set to true, automatically joins current user into the Conversation. <br>
	 * @return conversation Returns Conversation object if successful. <br>
	 */
	public Conversation GetConversationByBlob( String joinBlob, boolean alsoJoin) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,17);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,joinBlob);
		request.addParm('b',2,alsoJoin);
		
		Response r = XCall((XCallRequest)request);
		
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
	 *Returns a list of Conversation objects by Conversation.LIST_TYPE filter. <br>
	 * @param type Filter. <br>
	 * @return conversations List of conversations matching the filter. <br>
	 */
	public Conversation [] GetConversationList( Conversation.LIST_TYPE type) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,18);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('e',1,type.getId());
		
		Response r = XCall((XCallRequest)request);
		
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
	 *Retrieves a Message object by the P_GUID property (globally unique ID, same for all the participants of the conversation, in which this message occured). <br>
	 * @param guid Globally unique ID of the message. <br>
	 * @return message Returns a Message object if a match was found. <br>
	 */
	public Message GetMessageByGuid( byte[] guid) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,21);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('B',1,guid);
		
		Response r = XCall((XCallRequest)request);
		
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
	 *Returns all messages of the given type,                   the most recent POSTED_EMOTE or POSTED_TEXT of each conversation.
	 * @param type Type of messages requested
	 * @param latestPerConvOnly Whether to return only the most recent message per conversation
	 * @param fromTimestampInc Starting timestamp for reqested range, inclusive
	 * @param toTimestampExc Ending timestamp for requested range, exclusive
	 * @return messages
	 */
	public Message [] GetMessageListByType( Message.TYPE type, boolean latestPerConvOnly, int fromTimestampInc, int toTimestampExc) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,136);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('e',1,type.getId());
		request.addParm('b',2,latestPerConvOnly);
		request.addParm('u',3,fromTimestampInc);
		request.addParm('u',4,toTimestampExc);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		Vector<Message> messages = new Vector<Message>();
		while (r.HasMore(1))
		{
			int oid = 0;
			Message message = null;
			oid = r.GetOid(1);
			if (oid != AbstractDecoder.NULL_VALUE) { 
				message = (Message)skype.factory(Message.moduleID(), oid, skype);
			}
			messages.add(message);
		}
		return messages.toArray(new Message[messages.size()]);
		
	}
	
	/**
	 *This method returns a table in form of two string lists of equal length and an uint argument that returns the count of items i both lists. The first list contains video recording device handles and the second list descriptive names of those devices. NB! This method requires videortphost to be running, otherwise it will return empty lists. <br>
	 * @return GetAvailableVideoDevicesResult
	 */
	public GetAvailableVideoDevicesResult GetAvailableVideoDevices() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,80);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		GetAvailableVideoDevicesResult result = new GetAvailableVideoDevicesResult();
		
		Vector<String> deviceNames = new Vector<String>();
		while (r.HasMore(1))
		{
			String string = null;
			string  = r.GetAsString(1);
			deviceNames.add(string);
		}
		result.deviceNames = deviceNames.toArray(new String[deviceNames.size()]);
		
		Vector<String> devicePaths = new Vector<String>();
		while (r.HasMore(2))
		{
			String string = null;
			string  = r.GetAsString(2);
			devicePaths.add(string);
		}
		result.devicePaths = devicePaths.toArray(new String[devicePaths.size()]);
		
		int count = 0;
		count = r.GetAsInt(3);
		result.count = count;
		
		return result;
	}
	
	public class GetAvailableVideoDevicesResult {
		public String [] deviceNames;
		public String [] devicePaths;
		public int count;
	}
	
	/**
	 *Queries whether the given video device has a specific Video.VIDEO_DEVICE_CAPABILITY. Use Skype.GetAvailableVideoDevices method to retrieve sstring lists with available deviceName and devicePath values. <br>
	 * @param deviceName Human readable device name. <br>
	 * @param devicePath Device ID. <br>
	 * @param cap Any of the Video.VIDEO_DEVICE_CAPABILITY values. <br>
	 * @return result Returns true if the given device has given capability. <br>
	 */
	public boolean HasVideoDeviceCapability( String deviceName, String devicePath, Video.VIDEO_DEVICE_CAPABILITY cap) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,33);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,deviceName);
		request.addParm('S',2,devicePath);
		request.addParm('e',3,cap.getId());
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean result = false;
		result = r.GetAsBoolean(1);
		return result;
	}
	
	/**
	 * @param deviceName
	 * @param devicePath
	 */
	public void DisplayVideoDeviceTuningDialog( String deviceName, String devicePath) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,34);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,deviceName);
		request.addParm('S',2,devicePath);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *Warning: Will be deprecated soon
	 * @param type
	 * @param deviceName name and path to be used only with media type VIDEO
	 * @param devicePath
	 * @return video
	 */
	public Video GetPreviewVideo( Video.MEDIATYPE type, String deviceName, String devicePath) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,35);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('e',1,type.getId());
		request.addParm('S',2,deviceName);
		request.addParm('S',3,devicePath);
		
		Response r = XCall((XCallRequest)request);
		
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
	 *Avaible to Video Engines using the Video RTP API
	 * @param command
	 * @return response
	 */
	public String VideoCommand( String command) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,59);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,command);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		String response = null;
		response = r.GetAsString(1);
		return response;
	}
	
	/**
	 * @param skypeName
	 * @return greeting
	 */
	public Voicemail GetGreeting( String skypeName) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,45);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,skypeName);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		int oid = 0;
		Voicemail greeting = null;
		oid = r.GetOid(1);
		if (oid != AbstractDecoder.NULL_VALUE) {
			greeting = (Voicemail)skype.factory(Voicemail.moduleID(), oid, skype);
		}
		return greeting;
	}
	
	/** Setupkey SETUPKEY_DISABLED_CODECS type:string  <br>Space-separated array of disabled codecs <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DISABLED_CODECS = "*Lib/Audio/DisableCodecs";
	
	/** Setupkey SETUPKEY_DISABLE_AEC type:boolean  <br>Disables Skype echo canceller <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DISABLE_AEC = "*Lib/Audio/DisableAEC";
	
	/** Setupkey SETUPKEY_DISABLE_NOISE_SUPPRESSOR type:boolean  <br>Disables Skype noise suppressor <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DISABLE_NOISE_SUPPRESSOR = "*Lib/Audio/DisableNS";
	
	/** Setupkey SETUPKEY_DISABLE_AGC type:boolean  Disables Skype automatic gain controller <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DISABLE_AGC = "*Lib/Audio/DisableAGC";
	
	/** Setupkey SETUPKEY_DISABLE_DIGITAL_NEAR_END_AGC type:boolean  <br>Disables Skype digital near-end gain controller <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DISABLE_DIGITAL_NEAR_END_AGC = "*Lib/Audio/DisableDigitalNearEndAGC";
	
	/** Setupkey SETUPKEY_DISABLE_DIGITAL_FAR_END_AGC type:boolean  <br>Disables Skype digital far-end gain controller <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DISABLE_DIGITAL_FAR_END_AGC = "*Lib/Audio/DisableDigitalFarEndAGC";
	
	/** Setupkey SETUPKEY_BEAMFORMER_MIC_SPACING type:string  <br>Space-separated array of 1 (in case of 2 microphones) or 2 (in case of 4 microphones) integers. SAL beamforming currently only supports 2 and 4-microphone configurations. The values represent the spacing between microphones (in millimeters). <br>In case of 2-microphone setup, Only the first value is used. <br><br>In case of 4-microphone setup, The first value is the distance between inner pair of microphones. The second value is the distance between inner pair of microphones and the outer pair. Like this: <br><br>Let the microphones be on straight line, A B C D. <br>Microphones B and C form the inner pair, while A and D form the outer pair. <br>The first value in the setup string would be distance between B and C. <br>The second value would be distance between A and B (which is the same as distance between C and D). <br><br>With 4-mic setup, you will need to use two channels. The inner pair should go to one channel (left) and the outer pair should go to another (right). <br><br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String BEAMFORMER_MIC_SPACING = "*Lib/Audio/BeamformerMicSpacing";
	
	/** Setupkey SETUPKEY_DISABLE_AUDIO_DEVICE_PROBING type:boolean  <br>Disables audio devices probing <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DISABLE_AUDIO_DEVICE_PROBING = "*Lib/QualityMonitor/DisableAudioDeviceProbing";
	
	/**
	 */
	public enum PREPARESOUNDRESULT {
	
		/** */
		PREPARESOUND_SUCCESS(0),
		
		/** */
		PREPARESOUND_MISC_ERROR(1),
		
		/** */
		PREPARESOUND_FILE_NOT_FOUND(2),
		
		/** */
		PREPARESOUND_FILE_TOO_BIG(3),
		
		/** */
		PREPARESOUND_FILE_READ_ERROR(4),
		
		/** */
		PREPARESOUND_UNSUPPORTED_FILE_FORMAT(5),
		
		/** */
		PREPARESOUND_PLAYBACK_NOT_SUPPORTED(6);
		
		private static final Map<Integer,PREPARESOUNDRESULT> lookup = new HashMap<Integer,PREPARESOUNDRESULT>();
		
		static {
			for(PREPARESOUNDRESULT s : EnumSet.allOf(PREPARESOUNDRESULT.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private PREPARESOUNDRESULT(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static PREPARESOUNDRESULT get(int code) {
			return lookup.get(code);
		}
		
		public static PREPARESOUNDRESULT fromString(String s) {
			for (PREPARESOUNDRESULT p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 */
	public enum AUDIODEVICE_CAPABILITIES {
	
		/** */
		HAS_VIDEO_CAPTURE(1),
		
		/** */
		HAS_USB_INTERFACE(2),
		
		/** */
		POSSIBLY_HEADSET(4),
		
		/** */
		HAS_AUDIO_CAPTURE(8),
		
		/** */
		HAS_AUDIO_RENDERING(16),
		
		/** */
		HAS_LOWBANDWIDTH_CAPTURE(32),
		
		/** */
		IS_WEBCAM(64),
		
		/** */
		IS_HEADSET(128),
		
		/** */
		POSSIBLY_WEBCAM(256),
		
		/** */
		HAS_VIDEO_RENDERING(2048),
		
		/** */
		HAS_BLUETOOTH_INTERFACE(4096);
		
		private static final Map<Integer,AUDIODEVICE_CAPABILITIES> lookup = new HashMap<Integer,AUDIODEVICE_CAPABILITIES>();
		
		static {
			for(AUDIODEVICE_CAPABILITIES s : EnumSet.allOf(AUDIODEVICE_CAPABILITIES.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private AUDIODEVICE_CAPABILITIES(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static AUDIODEVICE_CAPABILITIES get(int code) {
			return lookup.get(code);
		}
		
		public static AUDIODEVICE_CAPABILITIES fromString(String s) {
			for (AUDIODEVICE_CAPABILITIES p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *Takes audio data that comes from the sound argument and mixes it into playback or notification device, depending on the value passed in the useCallOutDevice argument. The sound argument contains the audio data in in follwing format: first 4 bytes of the binary contain the sample rate, followed by 16 bit (mono) samples. The soundid argument is an arbitrary ID that you can pass in and then later use as an argument for Skype class PlayStop method. To mix the audio into playback device stream, set useCallOutDevice to true, to mic it into notification stream, set useCallOutDevice to false. <br>
	 * @param soundid
	 * @param sound
	 * @param loop
	 * @param useCallOutDevice
	 */
	public void PlayStart( int soundid, byte[] sound, boolean loop, boolean useCallOutDevice) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,48);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('u',1,soundid);
		request.addParm('B',2,sound);
		request.addParm('b',3,loop);
		request.addParm('b',4,useCallOutDevice);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 * @param soundid
	 * @param datafile
	 * @param loop
	 * @param useCallOutDevice
	 * @return result
	 */
	public PREPARESOUNDRESULT PlayStartFromFile( int soundid, String datafile, boolean loop, boolean useCallOutDevice) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,212);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('u',1,soundid);
		request.addParm('f',2,datafile);
		request.addParm('b',3,loop);
		request.addParm('b',4,useCallOutDevice);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		PREPARESOUNDRESULT preparesoundresult = null;
		preparesoundresult = PREPARESOUNDRESULT.get(r.GetAsInt(1));
		return preparesoundresult;
	}
	
	/**
	 *Stops playback of the soundfile. The argument is the same ID you passed in the Skype class StartPlayback method. <br>
	 * @param soundid
	 */
	public void PlayStop( int soundid) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,49);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('u',1,soundid);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 * @param recordAndPlaybackData
	 */
	public void StartRecordingTest( boolean recordAndPlaybackData) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,50);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('b',1,recordAndPlaybackData);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 */
	public void StopRecordingTest() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,51);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *This method returns a table in form of three string lists of equal lengths. The first list contains audio output device handles ('hw:0,0', 'hw:0,1', etc.) The second list contains descriptive names of those devices (Ensoniq AudioPCI etc.) The third list contains device product IDs. Note that the values in these lists depend on which audio engine you are running (SAL, PCM, RTP). <br>
	 * @return GetAvailableOutputDevicesResult
	 */
	public GetAvailableOutputDevicesResult GetAvailableOutputDevices() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,53);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		GetAvailableOutputDevicesResult result = new GetAvailableOutputDevicesResult();
		
		Vector<String> handleList = new Vector<String>();
		while (r.HasMore(1))
		{
			String string = null;
			string  = r.GetAsString(1);
			handleList.add(string);
		}
		result.handleList = handleList.toArray(new String[handleList.size()]);
		
		Vector<String> nameList = new Vector<String>();
		while (r.HasMore(2))
		{
			String string = null;
			string  = r.GetAsString(2);
			nameList.add(string);
		}
		result.nameList = nameList.toArray(new String[nameList.size()]);
		
		Vector<String> productIdList = new Vector<String>();
		while (r.HasMore(3))
		{
			String string = null;
			string  = r.GetAsString(3);
			productIdList.add(string);
		}
		result.productIdList = productIdList.toArray(new String[productIdList.size()]);
		
		return result;
	}
	
	public class GetAvailableOutputDevicesResult {
		public String [] handleList;
		public String [] nameList;
		public String [] productIdList;
	}
	
	/**
	 *This method returns a table in form of three string lists of equal length. The first list contains audio recording device handles ('hw:0,0', 'hw:0,1', etc.) The second list contains descriptive names of those devices (Ensoniq AudioPCI etc.) The third list contains device product IDs. Note that the values in these lists depend on which audio engine you are running (SAL, PCM, RTP). <br>
	 * @return GetAvailableRecordingDevicesResult
	 */
	public GetAvailableRecordingDevicesResult GetAvailableRecordingDevices() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,54);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		GetAvailableRecordingDevicesResult result = new GetAvailableRecordingDevicesResult();
		
		Vector<String> handleList = new Vector<String>();
		while (r.HasMore(1))
		{
			String string = null;
			string  = r.GetAsString(1);
			handleList.add(string);
		}
		result.handleList = handleList.toArray(new String[handleList.size()]);
		
		Vector<String> nameList = new Vector<String>();
		while (r.HasMore(2))
		{
			String string = null;
			string  = r.GetAsString(2);
			nameList.add(string);
		}
		result.nameList = nameList.toArray(new String[nameList.size()]);
		
		Vector<String> productIdList = new Vector<String>();
		while (r.HasMore(3))
		{
			String string = null;
			string  = r.GetAsString(3);
			productIdList.add(string);
		}
		result.productIdList = productIdList.toArray(new String[productIdList.size()]);
		
		return result;
	}
	
	public class GetAvailableRecordingDevicesResult {
		public String [] handleList;
		public String [] nameList;
		public String [] productIdList;
	}
	
	/**
	 *Sets audio devices given in arguments as active audio devices. This command selects all three devices - microphone, playback and the notification channel. Valid input values for this method come from the first string list you get back from Skype class GetAvailableOutputDevices (handleList). <br>
	 * @param callInDevice
	 * @param callOutDevice
	 * @param waveOutDevice
	 */
	public void SelectSoundDevices( String callInDevice, String callOutDevice, String waveOutDevice) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,55);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,callInDevice);
		request.addParm('S',2,callOutDevice);
		request.addParm('S',3,waveOutDevice);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *The uint argument returns AUDIODEVICE_CAPABILITIES (declared in Skype class) <br>
	 * @param deviceHandle
	 * @return GetAudioDeviceCapabilitiesResult
	 */
	public GetAudioDeviceCapabilitiesResult GetAudioDeviceCapabilities( String deviceHandle) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,56);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,deviceHandle);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		GetAudioDeviceCapabilitiesResult result = new GetAudioDeviceCapabilitiesResult();
		
		String interfaceString = null;
		interfaceString = r.GetAsString(1);
		result.interfaceString = interfaceString;
		
		int capabilities = 0;
		capabilities = r.GetAsInt(2);
		result.capabilities = capabilities;
		
		return result;
	}
	
	public class GetAudioDeviceCapabilitiesResult {
		public String interfaceString;
		public int capabilities; /** bit set of AUDIODEVICE_CAPABILITIES */
	}
	
	/**
	 *Returns current audio stream volume for both playback and microphone streams. Useful for displaying visual audio indicators in you UI. See also Skype class OnNrgLevelsChange callback that gets fired each time the these values are changed. <br>
	 * @return GetNrgLevelsResult
	 */
	public GetNrgLevelsResult GetNrgLevels() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,57);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		GetNrgLevelsResult result = new GetNrgLevelsResult();
		
		int micLevel = 0;
		micLevel = r.GetAsInt(1);
		result.micLevel = micLevel;
		
		int speakerLevel = 0;
		speakerLevel = r.GetAsInt(2);
		result.speakerLevel = speakerLevel;
		
		return result;
	}
	
	public class GetNrgLevelsResult {
		public int micLevel;
		public int speakerLevel;
	}
	
	/**
	 *NB! This command only works if its implemented in external audiohost (RTP or PCM host). The command can be is used for passing custom commands from client UI to the audio implementation. <br>
	 * @param command
	 * @return response
	 */
	public String VoiceCommand( String command) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,58);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,command);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		String response = null;
		response = r.GetAsString(1);
		return response;
	}
	
	/**
	 *Returns value of audio playback volume setting (0..100). <br>
	 * @return volume
	 */
	public int GetSpeakerVolume() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,60);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return 0;
			
		int volume = 0;
		volume = r.GetAsInt(1);
		return volume;
	}
	
	/**
	 *This method is for setting speaker volume. It will set the level for Skype digital gain control. Skype audio library will not control gain of audio device itself. <br>
	 * @param volume
	 */
	public void SetSpeakerVolume( int volume) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,61);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('u',1,volume);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *Returns value of microphone volume setting (0..100). It will return the analog gain of audio device set by Skype AGC. For real-time microphone volume, use GetNrgLevels method or OnNrgLevelsChange callback (both are methods of Skype class). <br>
	 * @return micVolume
	 */
	public int GetMicVolume() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,62);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return 0;
			
		int micVolume = 0;
		micVolume = r.GetAsInt(1);
		return micVolume;
	}
	
	/**
	 *This method is for setting the microphone volume level. This does not work when Skype AGC (Automatic Gain Control) is enabled, which it is by default. It is currently impossible to disable AGC, so for now this method is here for purpose of future compatibility. <br>
	 * @param volume
	 */
	public void SetMicVolume( int volume) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,63);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('u',1,volume);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *Returns true in &muted argument if the currently selected playback device is muted. <br>
	 * @return muted
	 */
	public boolean IsSpeakerMuted() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,64);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean muted = false;
		muted = r.GetAsBoolean(1);
		return muted;
	}
	
	/**
	 *Returns true in &muted argument if the currently selected microphone is muted. <br>
	 * @return muted
	 */
	public boolean IsMicrophoneMuted() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,65);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean muted = false;
		muted = r.GetAsBoolean(1);
		return muted;
	}
	
	/**
	 *Sets currently selected playback device mute status according to argument. <br>
	 * @param mute
	 */
	public void MuteSpeakers( boolean mute) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,66);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('b',1,mute);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *Sets currently selected microphone mute status according to argument. <br>
	 * @param mute
	 */
	public void MuteMicrophone( boolean mute) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,67);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('b',1,mute);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 */
	public enum OPERATING_MEDIA {
	
		/** */
		OM_UNKNOWN(0),
		
		/** */
		OM_FREE(1),
		
		/** */
		OM_FREE_WIRELESS(2),
		
		/** */
		OM_3G(3),
		
		/** */
		OM_4G(4);
		
		private static final Map<Integer,OPERATING_MEDIA> lookup = new HashMap<Integer,OPERATING_MEDIA>();
		
		static {
			for(OPERATING_MEDIA s : EnumSet.allOf(OPERATING_MEDIA.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private OPERATING_MEDIA(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static OPERATING_MEDIA get(int code) {
			return lookup.get(code);
		}
		
		public static OPERATING_MEDIA fromString(String s) {
			for (OPERATING_MEDIA p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 * @param media
	 * @param maxUplinkBps
	 * @param maxDownlinkBps
	 */
	public void SetOperatingMedia( OPERATING_MEDIA media, int maxUplinkBps, int maxDownlinkBps) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,255);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('e',1,media.getId());
		request.addParm('u',2,maxUplinkBps);
		request.addParm('u',3,maxDownlinkBps);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *creates and sends a CONFIRMATION_CODE_REQUEST message                   this sends a confirmation code to the number provided
	 * @param type
	 * @param number
	 * @return sms
	 */
	public Sms RequestConfirmationCode( Sms.CONFIRM_TYPE type, String number) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,29);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('e',1,type.getId());
		request.addParm('S',2,number);
		
		Response r = XCall((XCallRequest)request);
		
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
	 *creates and sends a CONFIRMATION_CODE_SUBMIT message                   this authorizes the number with the server for the purpose given in RequestConfirmationCode
	 * @param number
	 * @param code
	 * @return sms
	 */
	public Sms SubmitConfirmationCode( String number, String code) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,30);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,number);
		request.addParm('S',2,code);
		
		Response r = XCall((XCallRequest)request);
		
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
	 *creates an OUTGOING/COMPOSING SMS message
	 * @return sms
	 */
	public Sms CreateOutgoingSms() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,70);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
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
	
	/** Setupkey SETUPKEY_FT_AUTOACCEPT type:int  <br>Controls file transfer auto-accept.  <br> - 0 - off <br> - 1 - on <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
	public static final String FT_AUTOACCEPT = "Lib/FileTransfer/AutoAccept";
	
	/** Setupkey SETUPKEY_FT_SAVEPATH type:string  <br>Full local path to save incoming file transfers (used for AutoAccept feature) <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
	public static final String FT_SAVEPATH = "Lib/FileTransfer/SavePath";
	
	/** Setupkey SETUPKEY_FT_INCOMING_LIMIT type:uint  <br>Number of simultaneous incoming file transfers (per user). Value 0 means no limitation.  <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
	public static final String FT_INCOMING_LIMIT = "Lib/FileTransfer/IncomingLimit";
	
	/** Setupkey SETUPKEY_IDLE_TIME_FOR_AWAY type:int  <br>Number of seconds since the last keyboard or mouse activity, after which the online status of currently logged in account should be set to AWAY. See Account.SetAvailability method for more information. <br>This is account-specific setup key. It can only be used while an account is logged in. <br> */
	public static final String IDLE_TIME_FOR_AWAY = "Lib/Account/IdleTimeForAway";
	
	/** Setupkey SETUPKEY_IDLE_TIME_FOR_NA type:int  <br>The Contact.AVAILABILITY.NOT_AVAILABLE online status has been deprecated. This setup key is no longer in use. <br> */
	public static final String IDLE_TIME_FOR_NA = "Lib/Account/IdleTimeForNA";
	
	/**
	 *Retrieves an Account object by Skype name (identity). This should normally be one of the first method calls after Skype object initialization. Nearly all the other methods require successful account login in order to work properly. The list of accounts that have been used on the local machine/database can be retrieved with Skype.GetExistingAccounts method. If a matching identity is not found, a new Account object is created. This object can then be used to populate requred fields and then use Account.Register method for new account creation. This method returns false on error. <br>
	 * @param identity Account skypename. <br>
	 * @return account Returns account object if successful. <br>
	 */
	public Account GetAccount( String identity) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,115);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,identity);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		int oid = 0;
		Account account = null;
		oid = r.GetOid(1);
		if (oid != AbstractDecoder.NULL_VALUE) {
			if (! object_list.containsKey(oid)) {
				object_list.clear(); // #AABIP-7: Clear any other account's cached info from previous logins
			}
			account = (Account)skype.factory(Account.moduleID(), oid, skype);
		}
		return account;
	}
	
	/**
	 *Returns a list of possible profiles used before on this machine
	 * @return accountNameList
	 */
	public String [] GetExistingAccounts() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,113);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		Vector<String> accountNameList = new Vector<String>();
		while (r.HasMore(1))
		{
			String string = null;
			string  = r.GetAsString(1);
			accountNameList.add(string);
		}
		return accountNameList.toArray(new String[accountNameList.size()]);
		
	}
	
	/**
	 *return most recently used account that has pwd saved. empty string if none
	 * @return account
	 */
	public String GetDefaultAccountName() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,114);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		String account = null;
		account = r.GetAsString(1);
		return account;
	}
	
	/**
	 *suggest a nice skypename to go with given fullname
	 * @param fullname
	 * @return suggestedName
	 */
	public String GetSuggestedSkypename( String fullname) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,116);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,fullname);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		String suggestedName = null;
		suggestedName = r.GetAsString(1);
		return suggestedName;
	}
	
	/**
	A value of this type can be returned by one of the following methods (of Skype class): ValidateAvatar, ValidateProfileString, ValidatePassword.  <br> */
	public enum VALIDATERESULT {
	
		/** Given property could not be validated. The length of the field was within limits and the value is assumed to be Ok. Your client should treat this value as equivalent to VALIDATED_OK. <br>*/
		NOT_VALIDATED(0),
		
		/** Avatar or profile string validation succeeded. <br>*/
		VALIDATED_OK(1),
		
		/** Password is too short. <br>*/
		TOO_SHORT(2),
		
		/** The value exceeds max size limit for the given property. <br>*/
		TOO_LONG(3),
		
		/** Value contains illegal characters. <br>*/
		CONTAINS_INVALID_CHAR(4),
		
		/** Value contains whitespace. <br>*/
		CONTAINS_SPACE(5),
		
		/** Password cannot be the same as skypename. <br>*/
		SAME_AS_USERNAME(6),
		
		/** Value has invalid format. <br>*/
		INVALID_FORMAT(7),
		
		/** Value contains invalid word. <br>*/
		CONTAINS_INVALID_WORD(8),
		
		/** Password is too simple. <br>*/
		TOO_SIMPLE(9),
		
		/** Value starts with an invalid character. <br>*/
		STARTS_WITH_INVALID_CHAR(10);
		
		private static final Map<Integer,VALIDATERESULT> lookup = new HashMap<Integer,VALIDATERESULT>();
		
		static {
			for(VALIDATERESULT s : EnumSet.allOf(VALIDATERESULT.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private VALIDATERESULT(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static VALIDATERESULT get(int code) {
			return lookup.get(code);
		}
		
		public static VALIDATERESULT fromString(String s) {
			for (VALIDATERESULT p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 * @param value
	 * @return ValidateAvatarResult
	 */
	public ValidateAvatarResult ValidateAvatar( byte[] value) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,119);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('B',1,value);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		ValidateAvatarResult result = new ValidateAvatarResult();
		
		VALIDATERESULT validateresult = null;
		validateresult = VALIDATERESULT.get(r.GetAsInt(1));
		result.result = validateresult;
		
		int freeBytesLeft = 0;
		freeBytesLeft = r.GetAsInt(2);
		result.freeBytesLeft = freeBytesLeft;
		
		return result;
	}
	
	public class ValidateAvatarResult {
		public VALIDATERESULT result;
		public int freeBytesLeft;
	}
	
	/**
	 *This method should be used for validating skypenames before registering new accounts, if the propKey is set to SKYPENAME (Contact class) and forRegistration argument is set to true. If the forRegistration argument is false, only string length check is applied. It is also useful to probe, what the size limits are, for each string property (e.g. 300 characters for moodmessage) <br>
	 * @param propKey
	 * @param strValue
	 * @param forRegistration
	 * @return ValidateProfileStringResult
	 */
	public ValidateProfileStringResult ValidateProfileString( int propKey, String strValue, boolean forRegistration) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,102);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('e',1,propKey);
		request.addParm('S',2,strValue);
		request.addParm('b',3,forRegistration);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		ValidateProfileStringResult result = new ValidateProfileStringResult();
		
		VALIDATERESULT validateresult = null;
		validateresult = VALIDATERESULT.get(r.GetAsInt(1));
		result.result = validateresult;
		
		int freeBytesLeft = 0;
		freeBytesLeft = r.GetAsInt(2);
		result.freeBytesLeft = freeBytesLeft;
		
		return result;
	}
	
	public class ValidateProfileStringResult {
		public VALIDATERESULT result;
		public int freeBytesLeft;
	}
	
	/**
	 *This method is for pre-validating account passwords before account creation or password change. The result will return either VALIDATED_OK or one of many possible reasons the password is unacceptable (too short, too simple, etc.) <br>
	 * @param username
	 * @param password
	 * @return result
	 */
	public VALIDATERESULT ValidatePassword( String username, String password) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,71);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,username);
		request.addParm('S',2,password);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		VALIDATERESULT validateresult = null;
		validateresult = VALIDATERESULT.get(r.GetAsInt(1));
		return validateresult;
	}
	
	/** Setupkey SETUPKEY_PORT type:int  <br>Suggested port number (lib will *try* to use that) <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String PORT = "*Lib/Connection/Port";
	
	/** Setupkey SETUPKEY_HTTPS_PROXY_ENABLE type:int  <br>Set to 0 for automatic proxy detect, 1 to use proxy config below <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String HTTPS_PROXY_ENABLE = "*Lib/Connection/HttpsProxy/Enable";
	
	/** Setupkey SETUPKEY_HTTPS_PROXY_ADDR type:string  <br>name:port of HTTP proxy server <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String HTTPS_PROXY_ADDR = "*Lib/Connection/HttpsProxy/Addr";
	
	/** Setupkey SETUPKEY_HTTPS_PROXY_USER type:string  <br>HTTPS proxy server username <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String HTTPS_PROXY_USER = "*Lib/Connection/HttpsProxy/User";
	
	/** Setupkey SETUPKEY_HTTPS_PROXY_PWD type:string  <br>HTTPS proxy server password (base64 encoded) <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String HTTPS_PROXY_PWD = "*Lib/Connection/HttpsProxy/Pwd";
	
	/** Setupkey SETUPKEY_SOCKS_PROXY_ENABLE type:int  <br>Set to non-zero to enable socks proxy support <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String SOCKS_PROXY_ENABLE = "*Lib/Connection/SocksProxy/Enable";
	
	/** Setupkey SETUPKEY_SOCKS_PROXY_ADDR type:string  <br>name:port of SOCKS proxy server <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String SOCKS_PROXY_ADDR = "*Lib/Connection/SocksProxy/Addr";
	
	/** Setupkey SETUPKEY_SOCKS_PROXY_USER type:string  <br>SOCKS proxy server username <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String SOCKS_PROXY_USER = "*Lib/Connection/SocksProxy/User";
	
	/** Setupkey SETUPKEY_SOCKS_PROXY_PWD type:string  <br>SOCKS proxy server password (base64 encoded) <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String SOCKS_PROXY_PWD = "*Lib/Connection/SocksProxy/Pwd";
	
	/** Setupkey SETUPKEY_LOCALADDRESS type:string  <br>local interface to listen to <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String LOCALADDRESS = "*Lib/Connection/LocalAddress";
	
	/** Setupkey SETUPKEY_DISABLE_PORT80 type:int  <br>1 disables listening of alternative ports (80, 443) <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DISABLE_PORT80 = "*Lib/Connection/DisablePort80";
	
	/** Setupkey SETUPKEY_DISABLE_UDP type:int  <br>1 disables UDP port binding. should be set before connect <br>This setup key is machine-specific and affects all local accounts. <br> */
	public static final String DISABLE_UDP = "*Lib/Connection/DisableUDP";
	
	/**
	 */
	public enum PROXYTYPE {
	
		/** */
		HTTPS_PROXY(0),
		
		/** */
		SOCKS_PROXY(1);
		
		private static final Map<Integer,PROXYTYPE> lookup = new HashMap<Integer,PROXYTYPE>();
		
		static {
			for(PROXYTYPE s : EnumSet.allOf(PROXYTYPE.class))
				lookup.put(s.getId(), s);
		}
		
		private final int id;
		
		private PROXYTYPE(int value) {
			this.id = value;
		}
		
		public int getId() { return id; }
		
		public static PROXYTYPE get(int code) {
			return lookup.get(code);
		}
		
		public static PROXYTYPE fromString(String s) {
			for (PROXYTYPE p : lookup.values()) {
				if (p.toString() == s) {
					return p;
				}
			}
			return null;
		}
	}
	
	/**
	 *Port number that the runtime ended up listening to. Usually equal to SETUPKEY_PORT. 0 if none used (disconnected or binding failed). <br>
	 * @return port
	 */
	public int GetUsedPort() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,130);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return 0;
			
		int port = 0;
		port = r.GetAsInt(1);
		return port;
	}
	
	/**
	 *This is used for retrieving local setup keys of type string. For more information, see Defines section in the skype-embedded_2.h  <br>
	 * @param key
	 * @return value
	 */
	public String GetStr( String key) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,120);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,key);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		String value = null;
		value = r.GetAsString(1);
		return value;
	}
	
	/**
	 *This is used for retrieving local setup keys of type int. For more information, see Defines section in the skype-embedded_2.h  <br>
	 * @param key
	 * @return value
	 */
	public int GetInt( String key) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,121);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,key);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return 0;
			
		int value = 0;
		value = r.GetAsInt(1);
		return value;
	}
	
	/**
	 *This is used for retrieving local setup keys of type binary. For more information, see Defines section in the skype-embedded_2.h  <br>
	 * @param key
	 * @return value
	 */
	public byte[] GetBin( String key) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,122);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,key);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		byte[] value = null;
		value = r.GetAsBinary(1);
		return value;
	}
	
	/**
	 *This is used for setting local setup keys of type string. For more information, see Defines section in the skype-embedded_2.h  <br>
	 * @param key
	 * @param value
	 */
	public void SetStr( String key, String value) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,123);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,key);
		request.addParm('S',2,value);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *This is used for setting local setup keys of type int. For more information, see Defines section in the skype-embedded_2.h  <br>
	 * @param key
	 * @param value
	 */
	public void SetInt( String key, int value) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,124);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,key);
		request.addParm('i',2,value);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *This is used for setting local setup keys of type binary. For more information, see Defines section in the skype-embedded_2.h  <br>
	 * @param key
	 * @param value
	 */
	public void SetBin( String key, byte[] value) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,125);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,key);
		request.addParm('B',2,value);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 *Returns true if the given setup key is defined in local setup. For more information, see Defines section in the skype-embedded_2.h  <br>
	 * @param key
	 * @return value
	 */
	public boolean IsDefined( String key) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,126);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,key);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return false;
			
		boolean value = false;
		value = r.GetAsBoolean(1);
		return value;
	}
	
	/**
	 * @param key
	 */
	public void Delete( String key) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,127);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,key);
		
		XCall((XCallRequest)request);
	}
	
	/**
	 * @param key
	 * @return value
	 */
	public String [] GetSubKeys( String key) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,128);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,key);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		Vector<String> value = new Vector<String>();
		while (r.HasMore(1))
		{
			String string = null;
			string  = r.GetAsString(1);
			value.add(string);
		}
		return value.toArray(new String[value.size()]);
		
	}
	
	/**
	 *Returns two string lists. First of them will contain list of two-letter language codes (ISO 639-1) The second list contains names of corresponding languages. <br>
	 * @return GetISOLanguageInfoResult
	 */
	public GetISOLanguageInfoResult GetISOLanguageInfo() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,207);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		GetISOLanguageInfoResult result = new GetISOLanguageInfoResult();
		
		Vector<String> languageCodeList = new Vector<String>();
		while (r.HasMore(1))
		{
			String string = null;
			string  = r.GetAsString(1);
			languageCodeList.add(string);
		}
		result.languageCodeList = languageCodeList.toArray(new String[languageCodeList.size()]);
		
		Vector<String> languageNameList = new Vector<String>();
		while (r.HasMore(2))
		{
			String string = null;
			string  = r.GetAsString(2);
			languageNameList.add(string);
		}
		result.languageNameList = languageNameList.toArray(new String[languageNameList.size()]);
		
		return result;
	}
	
	public class GetISOLanguageInfoResult {
		public String [] languageCodeList;
		public String [] languageNameList; /** assumes UI has set correct language */
	}
	
	/**
	 *Returns three string lists and one int array, containing 2-letter country code, country name, dialing prefix and example dial string (not available for all items). This method does currently return 0 for South Georgia and the South Sandwich Islands. <br>
	 * @return GetISOCountryInfoResult
	 */
	public GetISOCountryInfoResult GetISOCountryInfo() {
	
		Request request = null;
		try {
			request = new XCallRequest(0,208);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		GetISOCountryInfoResult result = new GetISOCountryInfoResult();
		
		Vector<String> countryCodeList = new Vector<String>();
		while (r.HasMore(1))
		{
			String string = null;
			string  = r.GetAsString(1);
			countryCodeList.add(string);
		}
		result.countryCodeList = countryCodeList.toArray(new String[countryCodeList.size()]);
		
		Vector<String> countryNameList = new Vector<String>();
		while (r.HasMore(2))
		{
			String string = null;
			string  = r.GetAsString(2);
			countryNameList.add(string);
		}
		result.countryNameList = countryNameList.toArray(new String[countryNameList.size()]);
		
		Vector<Integer> countryPrefixList = new Vector<Integer>();
		while (r.HasMore(3))
		{
			Integer integer = null;
			integer = new Integer(r.GetAsInt(3));
			countryPrefixList.add(integer);
		}
		
		int[] intArray = new int[countryPrefixList.size()];
		for (int i = 0; i < countryPrefixList.size(); i++) {
			intArray[i] = countryPrefixList.get(i);
		}
		result.countryPrefixList = intArray;
		Vector<String> countryDialExampleList = new Vector<String>();
		while (r.HasMore(4))
		{
			String string = null;
			string  = r.GetAsString(4);
			countryDialExampleList.add(string);
		}
		result.countryDialExampleList = countryDialExampleList.toArray(new String[countryDialExampleList.size()]);
		
		return result;
	}
	
	public class GetISOCountryInfoResult {
		public String [] countryCodeList;
		public String [] countryNameList; /** assumes UI has set correct language */
		public int [] countryPrefixList;
		public String [] countryDialExampleList;
	}
	
	/**
	 *Returns 2-letter country code based on PSTN number. The input argument has to be without + in from of it - '37212345678' will return 'ee' while '+37212345678' will return an empty string. <br>
	 * @param number
	 * @return countryCode
	 */
	public String GetISOCountryCodebyPhoneNo( String number) {
	
		Request request = null;
		try {
			request = new XCallRequest(0,211);
		} catch (IOException e) {
			e.printStackTrace();
			if (errorListener != null)
				errorListener.OnSkypeKitFatalError();
		}
		request.addParm('S',1,number);
		
		Response r = XCall((XCallRequest)request);
		
		if (r.isErrCall())
			return null;
			
		String countryCode = null;
		countryCode = r.GetAsString(1);
		return countryCode;
	}
	
	/**
	 * Format property value, typically as seen in an onPropertyChange()
	 * listener, as a string.
	 * 
	 * @param moid
	 *            - module ID, from (apiClass).moduleID()
	 * @param pid
	 *            - property id, fromm prop.getId()
	 * @param value
	 *            - generic property value, as found in onPropertyChange()
	 *            listner interface
	 * @return property value formatted as a String
	 */
	public static String getPropertyAsString(int moid, int pid, Object value)
	{
		// assuming we do not want to get string properties here, if the value is null, return null
		if (value == null) {
			return null;
		}
		switch (moid) {
		case 10:
			switch (pid) {
				case 155: return ContactGroup.TYPE.get((Integer)value).toString();
				case 154: return value.toString();
				case 151: return value.toString();
				case 152: return value.toString();
				case 153: return value.toString();
			}
			break;
		case 2:
			switch (pid) {
				case 202: return Contact.TYPE.get((Integer)value).toString();
				case 4: return value.toString();
				case 6: return value.toString();
				case 5: return value.toString();
				case 7: return value.toString();
				case 8: return value.toString();
				case 9: return value.toString();
				case 10: return value.toString();
				case 11: return value.toString();
				case 12: return value.toString();
				case 13: return value.toString();
				case 14: return value.toString();
				case 15: return value.toString();
				case 16: return value.toString();
				case 17: return value.toString();
				case 18: return value.toString();
				case 37: return "<binary>";
				case 26: return value.toString();
				case 205: return value.toString();
				case 27: return value.toString();
				case 36: return "<binary>";
				case 19: return value.toString();
				case 28: return value.toString();
				case 29: return value.toString();
				case 182: return value.toString();
				case 183: return value.toString();
				case 20: return value.toString();
				case 25: return value.toString();
				case 35: return value.toString();
				case 34: return Contact.AVAILABILITY.get((Integer)value).toString();
				case 21: return value.toString();
				case 22: return value.toString();
				case 23: return Contact.AUTHLEVEL.get((Integer)value).toString();
				case 33: return value.toString();
				case 180: return value.toString();
				case 39: return value.toString();
				case 41: return value.toString();
				case 184: return value.toString();
				case 185: return value.toString();
				case 186: return value.toString();
				case 187: return value.toString();
				case 188: return value.toString();
				case 189: return value.toString();
				case 42: return value.toString();
			}
			break;
		case 1:
			switch (pid) {
				case 200: return ContactSearch.STATUS.get((Integer)value).toString();
			}
			break;
		case 19:
			switch (pid) {
				case 930: return value.toString();
				case 931: return value.toString();
				case 932: return Participant.RANK.get((Integer)value).toString();
				case 933: return Participant.RANK.get((Integer)value).toString();
				case 934: return Participant.TEXT_STATUS.get((Integer)value).toString();
				case 935: return Participant.VOICE_STATUS.get((Integer)value).toString();
				case 936: return Participant.VIDEO_STATUS.get((Integer)value).toString();
				case 943: return value.toString();
				case 938: return value.toString();
				case 948: return value.toString();
				case 939: return value.toString();
				case 941: return value.toString();
				case 942: return value.toString();
				case 947: return value.toString();
				case 949: return value.toString();
				case 950: return IDENTITYTYPE.get((Integer)value).toString();
				case 951: return value.toString();
				case 952: return value.toString();
				case 953: return value.toString();
				case 954: return value.toString();
				case 955: return LEAVE_REASON.get((Integer)value).toString();
			}
			break;
		case 18:
			switch (pid) {
				case 972: return value.toString();
				case 902: return Conversation.TYPE.get((Integer)value).toString();
				case 918: return value.toString();
				case 974: return value.toString();
				case 996: return value.toString();
				case 920: return value.toString();
				case 921: return value.toString();
				case 925: return value.toString();
				case 924: return value.toString();
				case 927: return Conversation.LOCAL_LIVESTATUS.get((Integer)value).toString();
				case 928: return value.toString();
				case 973: return value.toString();
				case 975: return value.toString();
				case 976: return value.toString();
				case 977: return value.toString();
				case 970: return value.toString();
				case 971: return value.toString();
				case 979: return value.toString();
				case 981: return value.toString();
				case 915: return value.toString();
				case 903: return value.toString();
				case 904: return value.toString();
				case 919: return Conversation.MY_STATUS.get((Integer)value).toString();
				case 922: return value.toString();
				case 906: return Participant.RANK.get((Integer)value).toString();
				case 907: return value.toString();
				case 909: return Conversation.ALLOWED_ACTIVITY.get((Integer)value).toString();
				case 980: return value.toString();
				case 910: return value.toString();
				case 911: return value.toString();
				case 913: return value.toString();
				case 914: return "<binary>";
			}
			break;
		case 9:
			switch (pid) {
				case 960: return value.toString();
				case 120: return value.toString();
				case 122: return value.toString();
				case 123: return value.toString();
				case 792: return "<binary>";
				case 790: return value.toString();
				case 121: return value.toString();
				case 961: return Message.TYPE.get((Integer)value).toString();
				case 962: return Message.SENDING_STATUS.get((Integer)value).toString();
				case 968: return Message.CONSUMPTION_STATUS.get((Integer)value).toString();
				case 222: return value.toString();
				case 223: return value.toString();
				case 963: return value.toString();
				case 964: return value.toString();
				case 127: return value.toString();
				case 125: return value.toString();
				case 966: return value.toString();
				case 126: return LEAVE_REASON.get((Integer)value).toString();
				case 982: return value.toString();
			}
			break;
		case 11:
			switch (pid) {
				case 130: return Video.STATUS.get((Integer)value).toString();
				case 131: return value.toString();
				case 132: return value.toString();
				case 133: return value.toString();
				case 134: return Video.MEDIATYPE.get((Integer)value).toString();
				case 1104: return value.toString();
				case 1105: return value.toString();
			}
			break;
		case 7:
			switch (pid) {
				case 100: return Voicemail.TYPE.get((Integer)value).toString();
				case 101: return value.toString();
				case 102: return value.toString();
				case 103: return Voicemail.STATUS.get((Integer)value).toString();
				case 104: return Voicemail.FAILUREREASON.get((Integer)value).toString();
				case 105: return value.toString();
				case 106: return value.toString();
				case 107: return value.toString();
				case 108: return value.toString();
				case 109: return value.toString();
				case 830: return value.toString();
				case 831: return "<binary>";
			}
			break;
		case 12:
			switch (pid) {
				case 190: return Sms.TYPE.get((Integer)value).toString();
				case 191: return Sms.STATUS.get((Integer)value).toString();
				case 192: return Sms.FAILUREREASON.get((Integer)value).toString();
				case 48: return value.toString();
				case 198: return value.toString();
				case 193: return value.toString();
				case 49: return value.toString();
				case 194: return value.toString();
				case 199: return value.toString();
				case 195: return value.toString();
				case 196: return "<binary>";
				case 197: return value.toString();
				case 840: return value.toString();
			}
			break;
		case 6:
			switch (pid) {
				case 80: return Transfer.TYPE.get((Integer)value).toString();
				case 81: return value.toString();
				case 82: return value.toString();
				case 83: return Transfer.STATUS.get((Integer)value).toString();
				case 84: return Transfer.FAILUREREASON.get((Integer)value).toString();
				case 85: return value.toString();
				case 86: return value.toString();
				case 87: return value.toString();
				case 88: return value.toString();
				case 89: return value.toString();
				case 90: return value.toString();
				case 91: return value.toString();
				case 92: return "<binary>";
				case 93: return value.toString();
				case 98: return value.toString();
			}
			break;
		case 5:
			switch (pid) {
				case 70: return Account.STATUS.get((Integer)value).toString();
				case 71: return Account.PWDCHANGESTATUS.get((Integer)value).toString();
				case 73: return Account.LOGOUTREASON.get((Integer)value).toString();
				case 78: return Account.COMMITSTATUS.get((Integer)value).toString();
				case 72: return value.toString();
				case 74: return value.toString();
				case 75: return value.toString();
				case 804: return value.toString();
				case 76: return value.toString();
				case 79: return Account.CBLSYNCSTATUS.get((Integer)value).toString();
				case 77: return value.toString();
				case 160: return Account.CHATPOLICY.get((Integer)value).toString();
				case 161: return Account.SKYPECALLPOLICY.get((Integer)value).toString();
				case 162: return Account.PSTNCALLPOLICY.get((Integer)value).toString();
				case 163: return Account.AVATARPOLICY.get((Integer)value).toString();
				case 164: return Account.BUDDYCOUNTPOLICY.get((Integer)value).toString();
				case 165: return Account.TIMEZONEPOLICY.get((Integer)value).toString();
				case 166: return Account.WEBPRESENCEPOLICY.get((Integer)value).toString();
				case 168: return Account.PHONENUMBERSPOLICY.get((Integer)value).toString();
				case 169: return Account.VOICEMAILPOLICY.get((Integer)value).toString();
				case 773: return value.toString();
				case 800: return value.toString();
				case 801: return value.toString();
				case 802: return value.toString();
				case 4: return value.toString();
				case 5: return value.toString();
				case 7: return value.toString();
				case 8: return value.toString();
				case 9: return value.toString();
				case 10: return value.toString();
				case 11: return value.toString();
				case 12: return value.toString();
				case 13: return value.toString();
				case 14: return value.toString();
				case 15: return value.toString();
				case 16: return value.toString();
				case 17: return value.toString();
				case 18: return value.toString();
				case 19: return value.toString();
				case 26: return value.toString();
				case 27: return value.toString();
				case 28: return value.toString();
				case 34: return Contact.AVAILABILITY.get((Integer)value).toString();
				case 37: return "<binary>";
				case 182: return value.toString();
				case 183: return value.toString();
				case 205: return value.toString();
			}
			break;
		}
		return null;
	}
}
