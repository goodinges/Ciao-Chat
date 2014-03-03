/**
 * Copyright (C) 2010, 2011 Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
*/


package com.skype.tutorial.util;

import java.text.DateFormat;
import java.util.Calendar;
import java.util.Date;

import com.skype.api.Account;
import com.skype.api.Account.AccountListener;
import com.skype.api.Contact;
import com.skype.api.Contact.ContactListener;
import com.skype.api.ContactGroup;
import com.skype.api.ContactGroup.ContactGroupListener;
import com.skype.api.ContactSearch;
import com.skype.api.ContactSearch.ContactSearchListener;
import com.skype.api.Conversation;
import com.skype.api.Conversation.ConversationListener;
import com.skype.api.Conversation.LIST_TYPE;
import com.skype.api.Message;
import com.skype.api.Message.MessageListener;
import com.skype.api.Participant;
import com.skype.api.Participant.DTMF;
import com.skype.api.Participant.ParticipantListener;
import com.skype.api.Skype;
import com.skype.api.Skype.PROXYTYPE;
import com.skype.api.Skype.SkypeListener;
import com.skype.api.Sms;
import com.skype.api.Sms.SmsListener;
import com.skype.api.Transfer;
import com.skype.api.Transfer.TransferListener;
import com.skype.api.Video;
import com.skype.api.Video.VideoListener;
import com.skype.api.Voicemail;
import com.skype.api.Voicemail.VoicemailListener;
import com.skype.util.Log;

/**
 * SkypeKit API Event Handlers: Java Tutorials.
 * 
 * @author Andrea Drane (based on skype.javawrapper.client.JavaWrapperListeners and
 * 		   ported/refactored from existing C++ tutorial code)
 * 
 * @since 1.0
 */
public class JavaTutorialListeners implements 
							AccountListener, 
							SkypeListener,
							ContactListener, 
							ContactGroupListener, 
							ContactSearchListener,
							ConversationListener, 
							MessageListener, 
							ParticipantListener,
							SmsListener, 
							TransferListener, 
							VideoListener, 
							VoicemailListener {
	/**
	 * Initialized session instance providing access to this sessions's Skype instance,
	 * Account instance, tutorial tag, and so forth
	 * 
	 * @since 1.0
	 */
    private MySession mySession;

    /**
	 * Whether a call is in progress: Part I.
	 * <br /><br />
	 * Holds a reference to the affected Conversation instance so it doesn't get garbage-collected
	 * in the middle of the call. Initialized to null by caller; set to
	 * non-null by Conversation onPropertyChange and ConversationList onChange handlers.
	 * 
	 * @see com.skype.api.Conversation
	 * @see com.skype.api.Skype.SkypeListener#OnConversationListChange(com.skype.api.Conversation, com.skype.api.Conversation.LIST_TYPE, boolean)
	 * 
	 * @since 1.0
	 */
    Conversation activeConversation = null;

    /**
	 * Whether a call is in progress: Part II.
	 * <br /><br />
	 * Holds a reference to <em>Participants</em> of the affected Conversation instance so they
	 * don't get garbage-collected in the middle of the call. Initialized to null by caller; set to
	 * non-null by Conversation onPropertyChange and ConversationList onChange handlers.
	 * 
	 * @see com.skype.api.Conversation
	 * @see com.skype.api.Skype.SkypeListener#OnConversationListChange(com.skype.api.Conversation, com.skype.api.Conversation.LIST_TYPE, boolean)
	 * 
	 * @since 1.0
	 */
    Participant[] activeConversationParticipants = null;

    /**
	 * Indicates whether OnApp2AppStreamListChange was ever fired
	 * with a non-zero stream count.
	 * <br />
	 * Basically, this is for "prettiness" - we'll only display the
	 * "connection" message <em>once</em> per session.
	 * 
	 * @since 1.0
	 */
	public boolean appConnected = false;
	
	public JavaTutorialListeners (MySession mySession) {
		this.mySession = mySession;
		Log.d(mySession.myTutorialTag, "Registering the listeners...");
		MySession.myConsole.println("\tRegistering the listeners...");
		registerAllListeners();
	}
	
	public void registerListener ( int modid ) {
		// Register the listener with Skype service
		mySession.mySkype.RegisterListener( modid, this );
	}
	
	public void registerAllListeners () {
		registerListener( Account.moduleID());
		registerListener( Contact.moduleID());
		registerListener( ContactGroup.moduleID());
		registerListener( ContactSearch.moduleID());
		registerListener( Conversation.moduleID());
		registerListener( Message.moduleID());
		registerListener( Participant.moduleID());
		registerListener( Sms.moduleID());
		registerListener( Transfer.moduleID());
		registerListener( Video.moduleID());
		registerListener( Voicemail.moduleID());
		registerListener( Skype.getmoduleid());

	}

	public void unRegisterListener ( int modid ) {
		// Register the listener with Skype service
		mySession.mySkype.UnRegisterListener(modid, this);
	}
	
	public void unRegisterAllListeners () {
		unRegisterListener( Account.moduleID());
		unRegisterListener( Contact.moduleID());
		unRegisterListener( ContactGroup.moduleID());
		unRegisterListener( ContactSearch.moduleID());
		unRegisterListener( Conversation.moduleID());
		unRegisterListener( Message.moduleID());
		unRegisterListener( Participant.moduleID());
		unRegisterListener( Sms.moduleID());
		unRegisterListener( Transfer.moduleID());
		unRegisterListener( Video.moduleID());
		unRegisterListener( Voicemail.moduleID());
		unRegisterListener( Skype.getmoduleid());
	}

	/**
	 * VoicemailListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Voicemail.VoicemailListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Voicemail.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.Voicemail.PROPERTY prop, Object value) {
	}

	/**
	 * VideoListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Video.VideoListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Video.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.Video.PROPERTY prop, Object value) {
	}

	/**
	 * VideoListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Video.VideoListener#OnCaptureRequestCompleted(com.skype.api.SkypeObject, int, boolean, byte[], int, int)
	 */
	public void OnCaptureRequestCompleted(com.skype.api.SkypeObject obj, int requestId, boolean isSuccessful,
			byte[] image, int width, int height) {
		Log.d("Tutorial", "OnCaptureRequestCompleted(" + requestId + ", " + isSuccessful + ", image.length:" + image.length
			  + ", " + width + ", " + height + ")");

	}

	/**
	 * TransferListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Transfer.TransferListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Transfer.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.Transfer.PROPERTY prop, Object value) {
	}

	/**
	 * SmsListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Sms.SmsListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Sms.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.Sms.PROPERTY prop, Object value) {
	}

	/**
	 * ParticipantListener Override: Tutorial Handler - Sound Levels and Voice Status.
	 * <ul>
	 *   <li>Tutorial 5 - sound level property changes.</li>
	 *   <li>Tutorial 6/7 - voice status property changes.</li>
	 * </ul>
	 *
	 * @param obj
	 * 	The affected Participant.
	 * @param prop
	 * 	The Participant property that triggered this event.
	 * @param value
	 * 	Ignored.
	 * 
	 * @since 1.0
	 * 
	 * @see com.skype.api.Participant.ParticipantListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Participant.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.Participant.PROPERTY prop, Object value) {
		Participant affectedParticipant = (Participant)obj;

		if (prop == Participant.PROPERTY.sound_level)
		{
			MySession.myConsole.printf("Sound level changed to %d for %s%n",
					affectedParticipant.GetIntProperty(Participant.PROPERTY.sound_level),
					affectedParticipant.GetStrProperty(Participant.PROPERTY.identity));
		}
		else if (prop == Participant.PROPERTY.voice_status)
		{
			Participant.VOICE_STATUS voiceStatus =
				Participant.VOICE_STATUS.get(affectedParticipant.GetIntProperty(Participant.PROPERTY.voice_status));
			MySession.myConsole.printf("Voice status changed to %s for %s%n",
					voiceStatus.toString(),
					affectedParticipant.GetStrProperty(Participant.PROPERTY.identity));
		}
    }

	/**
	 * ParticipantListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Participant.ParticipantListener#OnIncomingDTMF(com.skype.api.SkypeObject, com.skype.api.Participant.DTMF)
	 */
	public void OnIncomingDTMF(com.skype.api.SkypeObject obj, DTMF dtmf) {
	}

	/**
	 * MessageListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Message.MessageListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Message.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.Message.PROPERTY prop, Object value) {
	}

	/**
	 * ConversationListener Override: Tutorial Handler - Conversation "Live Status".
	 * <br /><br />
	 * If it's <em>not</em> a "live status" change, ignore it. Otherwise:
	 * <ul>
	 *   <li>display changes to our live status</li>
	 *   <li>handle answering calls for us</li>
	 * </ul>
	 * 
	 * Tutorial 6/7 - Looks for:
	 * <ul>
	 *   <li>RINGING_FOR_ME so we can pick up the call</li>
	 *   <li>IM_LIVE to indicate that a call is in progress</li>
	 *   <li>RECENTLY_LIVE/NONE to indicate that a call has ended</li>
	 *  </ul>
	 * 
	 * @param obj
	 * 	The affected Conversation.
	 * @param prop
	 * 	The Conversation property that triggered this event.
	 * @param value
	 * 	Ignored.
	 * 
	 * @since 1.0
	 * 
	 * @see com.skype.api.Conversation.ConversationListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Conversation.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.Conversation.PROPERTY prop, Object value) {

		if (prop == Conversation.PROPERTY.local_livestatus) {
 			Conversation affectedConversation = (Conversation)obj;
			Conversation.LOCAL_LIVESTATUS liveStatus =
				Conversation.LOCAL_LIVESTATUS.get(affectedConversation.GetIntProperty(Conversation.PROPERTY.local_livestatus));
			MySession.myConsole.printf("%s: Live status changed to %s%n",
								mySession.myTutorialTag, liveStatus.toString());
			switch (liveStatus) {
			case RINGING_FOR_ME:
				MySession.myConsole.println("RING RING...");
				if (doPickUpCall()) {
					MySession.myConsole.println("Conv: Call answered!");
					activeConversation = affectedConversation;
					activeConversationParticipants = affectedConversation.GetParticipants(Conversation.PARTICIPANTFILTER.ALL);
					mySession.callActive = true;
				}
				break;
			case RECENTLY_LIVE:
			case NONE:
				activeConversation = null;
				activeConversationParticipants = null;
				mySession.callActive = false;
				MySession.myConsole.println("Conv: Call has ended/never started.");
				break;
			case IM_LIVE:
				MySession.myConsole.println("Conv: Live session is up!");
				break;
			default:
				Log.d(mySession.myTutorialTag, "Conv: Ignoring LiveStatus " + liveStatus.toString());
				break;
			}
        }
	}

	/**
	 * ConversationListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Conversation.ConversationListener#OnParticipantListChange(com.skype.api.SkypeObject)
	 */
	public void OnParticipantListChange(com.skype.api.SkypeObject obj) {
	}

	/**
	 * ConversationListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Conversation.ConversationListener#OnMessage(com.skype.api.SkypeObject, com.skype.api.Message)
	 */
	public void OnMessage(com.skype.api.SkypeObject obj, Message message) {

		Message.TYPE msgType = Message.TYPE.get(message.GetIntProperty(Message.PROPERTY.type));
		if (msgType == Message.TYPE.POSTED_TEXT) {
		   Integer conversationID = message.GetIntProperty(Message.PROPERTY.convo_guid);
		   Conversation conversation =
			   mySession.mySkype.GetConversationByIdentity(conversationID.toString());
            String msgAuthor = message.GetStrProperty(Message.PROPERTY.author);
            String msgBody = message.GetStrProperty(Message.PROPERTY.body_xml);
            if (!msgAuthor.equals(mySession.myAccountName)) {
                // Get timestamp -- it's in seconds, and the Date constructor needs milliseconds!
            	Integer msgTimeStamp = new Integer(message.GetIntProperty(Message.PROPERTY.timestamp));
                Date dateTimeStamp = new Date((msgTimeStamp.longValue() * 1000L));
            	DateFormat targetDateFmt = DateFormat.getDateTimeInstance();
            	MySession.myConsole.printf("%s: [%s] %s posted message%n%s%n",
            			mySession.myTutorialTag, targetDateFmt.format(dateTimeStamp), msgAuthor, msgBody);
            	Calendar targetDate = Calendar.getInstance();
            	conversation.PostText((targetDateFmt.format(targetDate.getTime()) + ": This is an automated reply"), false);
            }
        }
		else {
			MySession.myConsole.printf("%s: Ignoring ConversationListener.OnMessage of type %s%n",
				mySession.myTutorialTag, msgType.toString());
		}	
	}

	/**
	 * ConversationListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Conversation.ConversationListener#OnSpawnConference(com.skype.api.SkypeObject, com.skype.api.Conversation)
	 */
	public void OnSpawnConference(com.skype.api.SkypeObject obj, Conversation spawned) {
        Log.d("Tutorial", "OnSpawnConference(" + spawned + ")");
	}

	/**
	 * ContactSearchListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.ContactSearch.ContactSearchListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.ContactSearch.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.ContactSearch.PROPERTY prop, Object value) {
	}

	/**
	 * ContactSearchListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.ContactSearch.ContactSearchListener#OnNewResult(com.skype.api.SkypeObject, com.skype.api.Contact, int)
	 */
	public void OnNewResult(com.skype.api.SkypeObject obj, Contact contact, int rankValue) {
        Log.d("Tutorial", "OnNewResult(" + contact + ", " + rankValue + ")");
	}

	/**
	 * ContactGroupListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.ContactGroup.ContactGroupListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.ContactGroup.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.ContactGroup.PROPERTY prop, Object value) {
	}

	/**
	 * ContactGroup Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.ContactGroup.ContactGroupListener#OnChangeConversation(com.skype.api.SkypeObject, com.skype.api.Conversation)
	 */
	public void OnChangeConversation(com.skype.api.SkypeObject obj, Conversation conversation) {
        Log.d("Tutorial", "OnChangeConversation(" + conversation + ")");
	}

	/**
	 * ContactGroupListener Override: Tutorial Handler - ContactGroup TYPE.
	 * <br /><br />
	 * This handler fires for all ContactGroups. If it's <em>not</em>
	 * a pending authorization request, log and ignore it.
	 * <br /><br />
     * Tutorial 9 - Process authorization requests
     * 
	 * @since 1.0
	 * 
	 * @see com.skype.api.ContactGroup.ContactGroupListener#OnChange(com.skype.api.SkypeObject, com.skype.api.Contact)
	 */
	public void OnChange(com.skype.api.SkypeObject obj, Contact contact) {

		String contactSkypeName = contact.GetStrProperty(Contact.PROPERTY.skypename); // Find out who it's from
		String contactDisplayName = contact.GetStrProperty(Contact.PROPERTY.displayname);
		
		ContactGroup waitingAuth =
				mySession.mySkype.GetHardwiredContactGroup(ContactGroup.TYPE.CONTACTS_WAITING_MY_AUTHORIZATION);
		Contact[] waitingAuthMembers = waitingAuth.GetContacts();
		int waitingAuthMembersCnt = waitingAuthMembers.length;
		
		if (waitingAuthMembersCnt == 0) {
			MySession.myConsole.printf("%s: Ignoring ContactGroup change for %s (%s); no Contacts awaiting authorization %n",
					mySession.myTutorialTag, contactSkypeName, contactDisplayName);
			return;
		}
		
		
		if (contact.GetStrProperty(Contact.PROPERTY.received_authrequest).length() == 0){
			MySession.myConsole.printf("%s: Ignoring ContactGroup change; Contact  %s (%s) is not awaiting authorization %n",
					mySession.myTutorialTag, contactSkypeName, contactDisplayName);
			return;
		}

		String authRequestText = contact.GetStrProperty(Contact.PROPERTY.received_authrequest);	// Get any intro text...
   		if ((authRequestText == null) || (authRequestText.length() == 0)) {						// ...and default it if missing
   			authRequestText = "-- NO INTRODUCTORY TEXT --";
   		}
		MySession.myConsole.printf("%s: Authorization request from: %s (%s):%n\t%s",
				mySession.myTutorialTag, contactSkypeName, contactDisplayName, authRequestText);
		contact.SetBuddyStatus(true, true);
/*
 * SetBuddyStatus should really return a boolean...
 * If and when it does, replace the above SetBuddyStatus invocation with the following...
   			if (contact.SetBuddyStatus(true, true)) {
   				MySession.myConsole.printf("%s: %s is now authorized%n", mySession.myTutorialTag, contactSkypeName);
   			}
   			else {
   				MySession.myConsole.printf("%s: Authorization failed.%n", mySession.myTutorialTag);
   			}
*/
   			MySession.myConsole.printf("%s: %s is now authorized!%n", mySession.myTutorialTag, contactSkypeName);
	}

	/**
	 * ContactListener Override: Tutorial Handler - Availability.
	 * <br /><br />
     * Maps an availability property ENUM code to a text string, and writes that string to
     * the console. If it's <em>not</em> an availability change, ignore it.
     * <br /><br />
     * The implementation here follows the C++ pattern - a switch statement with cases for each
     * defined code. For Java 5.0, the Enum class' <code>toString</code> method eliminates the
     * need for the entire switch statement. For example, you could code the printf as:
     * <pre>
     *     MySession.myConsole.printf("%s: Availability of %s is now %s%n",
	 *                       mySession.myTutorialTag,
	 *                       affectedContact.GetStrProperty(Contact.PROPERTY.displayname),
	 *                       availability.toString());
	 * </pre>
	 * Implementing this functionality as a switch statement, however,
	 * enables you to perform additional processing for specific cases.
     * 
	 * @param obj
	 * 	The affected Contact.
	 * @param prop
	 * 	The Contact property that triggered this event.
	 * @param value
	 * 	Ignored.
	 * 
	 * @since 1.0
	 * 
	 * @see com.skype.api.Contact.ContactListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Contact.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.Contact.PROPERTY prop, Object value) {
		Contact affectedContact = (Contact)obj;

		if (prop == Contact.PROPERTY.availability)
		{
			String statusAsText;
			Contact.AVAILABILITY availability =
				Contact.AVAILABILITY.get(affectedContact.GetIntProperty(Contact.PROPERTY.availability));

			switch (availability)
			{
			case UNKNOWN:
				statusAsText = "UNKNOWN";
				break;
			case PENDINGAUTH:
				statusAsText = "PENDINGAUTH";
				break;
			case BLOCKED:
				statusAsText = "BLOCKED";
				break;
			case BLOCKED_SKYPEOUT:
				statusAsText = "BLOCKED_SKYPEOUT";
				break;
			case SKYPEOUT:
				statusAsText = "SKYPEOUT";
				break;
			case OFFLINE:
				statusAsText = "OFFLINE";
				break;
			case OFFLINE_BUT_VM_ABLE:
				statusAsText = "OFFLINE_BUT_VM_ABLE";
				break;
			case OFFLINE_BUT_CF_ABLE:
				statusAsText = "OFFLINE_BUT_CF_ABLE";
				break;
			case ONLINE:
				statusAsText = "ONLINE";
				break;
			case AWAY:
				statusAsText = "AWAY";
				break;
			case NOT_AVAILABLE:
				statusAsText = "NOT_AVAILABLE";
				break;
			case DO_NOT_DISTURB:
				statusAsText = "DO_NOT_DISTURB";
				break;
			case SKYPE_ME:
				statusAsText = "SKYPE_ME";
				break;
			case INVISIBLE:
				statusAsText = "INVISIBLE";
				break;
			case CONNECTING:
				statusAsText = "CONNECTING";
				break;
			case ONLINE_FROM_MOBILE:
				statusAsText = "ONLINE_FROM_MOBILE";
				break;
			case AWAY_FROM_MOBILE:
				statusAsText = "AWAY_FROM_MOBILE";
				break;
			case NOT_AVAILABLE_FROM_MOBILE:
				statusAsText = "NOT_AVAILABLE_FROM_MOBILE";
				break;
			case DO_NOT_DISTURB_FROM_MOBILE:
				statusAsText = "DO_NOT_DISTURB_FROM_MOBILE";
				break;
			case SKYPE_ME_FROM_MOBILE:
				statusAsText = "SKYPE_ME_FROM_MOBILE";
				break;
			default:
				statusAsText = "UNKNOWN";
				break;
			}
			
			MySession.myConsole.printf("%s: Availability of %s is now %s (%s)%n",
					mySession.myTutorialTag,
					affectedContact.GetStrProperty(Contact.PROPERTY.displayname), statusAsText,
					availability.toString());
		}
	}
	
	/**
	 * SkypeListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnNewCustomContactGroup(com.skype.api.ContactGroup)
	 */
	public void OnNewCustomContactGroup(ContactGroup group) {
        Log.d("Tutorial", "OnNewCustomContactGroup(" + group + ")");
	}

	/**
	 * SkypeListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnContactOnlineAppearance(com.skype.api.Contact)
	 */
	public void OnContactOnlineAppearance(Contact contact) {
        Log.d("Tutorial", "OnContactOnlineAppearance(" + contact + ")");
	}

	/**
	 * SkypeListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnContactGoneOffline(com.skype.api.Contact)
	 */
	public void OnContactGoneOffline(Contact contact) {
	}

	/**
	 * SkypeListener Override: Tutorial Handler - Conversation LIST_TYPE.
	 * <br /><br />
	 * If it's <em>not</em> a LIVE_CONVERSATIONS change, ignore it; if it's not a
	 * type we're interested in, simply write it to the console.
	 * <ul>
	 *   <li>Tutorial 5 - Looks for RINGING_FOR_ME so we can join in.</li>
	 *   <li>Tutorial 6 - Looks for IM_LIVE or RECENTLY_LIVE/NONE. Former case, indicates
	 *       that a call is in progress; latter case, indicates that a call has ended.</li>
	 * </ul>
	 *
	 * @param conversation
	 * 	The affected Conversation.
	 * @param type
	 * 	The Conversation list type that triggered this event.
	 * @param added
	 * 	Ignored.
	 * 
	 * @since 1.0
	 * 
	 */
	public void OnConversationListChange(Conversation conversation, LIST_TYPE type, boolean added) {
		
		MySession.myConsole.printf("%s: ConversationListChange fired on: %s%n",
				mySession.myTutorialTag, conversation.GetStrProperty(Conversation.PROPERTY.displayname));

		if (type == Conversation.LIST_TYPE.LIVE_CONVERSATIONS) {
			Conversation.LOCAL_LIVESTATUS liveStatus =
				Conversation.LOCAL_LIVESTATUS.get(conversation.GetIntProperty(Conversation.PROPERTY.local_livestatus));
			MySession.myConsole.printf("%s: Live status changed to %s%n",
								mySession.myTutorialTag, liveStatus.toString());
			switch (liveStatus) {
			case RINGING_FOR_ME:
				activeConversation = conversation;
				activeConversationParticipants = conversation.GetParticipants(Conversation.PARTICIPANTFILTER.ALL);
				conversation.Join();
				mySession.callActive = true;
				break;
			case RECENTLY_LIVE:
			case NONE:
				MySession.myConsole.printf("%s: Call finished.%n", mySession.myTutorialTag);
				activeConversation = null;
				activeConversationParticipants = null;
				mySession.callActive = false;
				break;
			case IM_LIVE:
				MySession.myConsole.printf("%s: Live session is up.%n", mySession.myTutorialTag);
				break;
			default:
				MySession.myConsole.printf("%s: Ignoring Conversation status %s%n",
									mySession.myTutorialTag, liveStatus.toString());
				break;
			}
		}
	}

	/**
	 * SkypeListener Override: Tutorial Handler - Posted chat message.
	 * <br /><br />
	 * If it's <em>not</em> a POSTED_TEXT message, ignore it.
	 *
	 * @param message
	 * 	The affected Message.
	 * @param changesInboxTimestamp
	 * 	Ignored.
	 * @param supersedesHistoryMessage
	 * 	Ignored.
	 * @param conversation
	 * 	The affected Conversation.
	 * 
	 * @since 1.0
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnMessage(com.skype.api.Message, boolean, com.skype.api.Message, com.skype.api.Conversation)
	 */
	public void OnMessage(Message message,
			boolean changesInboxTimestamp, Message supersedesHistoryMessage, Conversation conversation) {
		Message.TYPE msgType = Message.TYPE.get(message.GetIntProperty(Message.PROPERTY.type));

        if (msgType == Message.TYPE.POSTED_TEXT) {
            String msgAuthor = message.GetStrProperty(Message.PROPERTY.author);
            String msgBody = message.GetStrProperty(Message.PROPERTY.body_xml);
            if (!msgAuthor.equals(mySession.myAccountName)) {
                // Get timestamp -- it's in seconds, and the Date constructor needs milliseconds!
            	Integer msgTimeStamp = new Integer(message.GetIntProperty(Message.PROPERTY.timestamp));
                Date dateTimeStamp = new Date((msgTimeStamp.longValue() * 1000L));
            	DateFormat targetDateFmt = DateFormat.getDateTimeInstance();
            	MySession.myConsole.printf("%s: [%s] %s posted message%n\t%s%n",
            			mySession.myTutorialTag, targetDateFmt.format(dateTimeStamp), msgAuthor, msgBody);
            	Calendar targetDate = Calendar.getInstance();
            	conversation.PostText((targetDateFmt.format(targetDate.getTime()) + ": This is an automated reply"), false);
            }
        }
		else {
			MySession.myConsole.printf("%s: Ignoring SkypeListener.OnMessage of type %s%n",
					mySession.myTutorialTag, msgType.toString());
		}	
	}

	/**
	 * SkypeListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnAvailableVideoDeviceListChange()
	 */
	public void OnAvailableVideoDeviceListChange() {
        Log.d("Tutorial", "OnAvailableVideoDeviceListChange()");
	}

	/**
	 * SkypeListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnAvailableDeviceListChange()
	 */
	public void OnAvailableDeviceListChange() {
        Log.d("Tutorial", "OnAvailableDeviceListChange()");
	}

	/**
	 * SkypeListener Override.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnNrgLevelsChange()
	 */
	public void OnNrgLevelsChange() {
        Log.d("Tutorial", "OnNrgLevelsChange()");
	}

	/**
	 * SkypeListener Override.
	 *
	 * Fires on P2P connection failure during account login, which (by default)
	 * attempts direct connection to the network, then falls back to connection via proxy.
	 * 
	 * @since 1.0
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnProxyAuthFailure(Skype.PROXYTYPE)
	 */
	public void OnProxyAuthFailure(PROXYTYPE type) {
	}

	/**
	 * SkypeListener Override.
	 *
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnH264Activated()
	 */
	@Override
    public void OnH264Activated()
    {
    }

	
	/**
	 * SkypeListener Override: Tutorial Handler - Datagrams.
	 * <br /><br />
	 * In the context of Tutorial 11 datagrams, this event fires when we receive a datagram.
	 * Writes the name of the affected stream and the actual datagram content to the console.
	 *
	 * @param appname
	 * 	The name of the application associated with this datagram connection.
	 * @param stream
	 * 	The name of the affected stream.
	 * @param data
	 * 	The payload of this datagram.
	 * 
	 * @since 1.0
	 * 
	 * see com.skype.api.Skype.SkypeListener#OnApp2AppDatagram(java.lang.String, java.lang.String, byte[])
	 */
	public void OnApp2AppDatagram(String appname, String stream, byte[] data) {
		String dataStr = new String(data); // Java 5.0 and up can use data.toString() directly

		MySession.myConsole.printf("Got datagram in stream %s for app %s: %s%n", stream, appname, dataStr);
	}


/*
 	 **
	 * SkypeListener Override: Tutorial Handler - App2App Stream List.
	 * <br /><br />
	 * In the context of Tutorial 11 datagrams, this event fires when:
	 * <ol>
	 *   <li>Connection is established between two app2app applications. That is, when
	 *	     both parties have an app up with the same name and <em>both</em> used App2AppConnect
	 *	     In that case, both parties get this event, with listType ALL_STREAMS</li>
	 *   <li>When a datagram is sent, the sender will get this event with listType SENDING_STREAMS
	 *	     Receiver of the datagram will get OnApp2AppDatagram event instead.</li>
	 *   <li>When the remote party drops app2app connection, the local user will get
	 *	     OnApp2AppStreamListChange with listType ALL_STREAMS and streams.length zero,
	 *	     which is useful for detecting remote drops.</li>
	 * </ol>
	 * 
	 * @param appname
	 * 	The name of the application associated with this datagram connection.
	 * @param listType
	 * 	The type of the affected stream(s) - sending, receiving, or "all"
	 * @param streams
	 * 	The names of the affected stream(s). In the context of Tutorial 11 datagrams,
	 *  we assume that there are only 2 participants and so th enumber of streams should be either
	 *  0 (zero; remote shutdown) or 1 (one; initiated/datagram sent).
	 * 
	 * @since 1.0
	 * 
	 * @see com.skype.api.Skype.SkypeListener#OnApp2AppStreamListChange(java.lang.String, com.skype.api.Skype.APP2APP_STREAMS, java.lang.String[])
	 *
	public void OnApp2AppStreamListChange(String appname, APP2APP_STREAMS listType, String[] streams) {
		int streamsCount = streams.length;
		
		if (streamsCount != 0) {
	        // Normally the streamCount in this event should be either 1 or 0.
	        // More streams are possible when there are more than 2 connected
	        // participants running the same application. For purposes of this
	        // example, we will assume that there are only 2 participants.
			int i;
	        for (i = 0; i < streamsCount; i++) {
	            MySession.myConsole.printf("OnApp2AppStreamListChange: %s %s %s%n",
	            		mySession.mySkype.StreamListType(listType), appname, streams[i]);
	            mySession.mySkype.streamName = streams[i]; // We're assuming only one stream!
	            												// If two or more, last one wins!
	        }

	        if (!appConnected) {
	            appConnected = true;
	            MySession.myConsole.printf("You can now type and send datagrams to remote instance.%n");
	        }
	    }
	    else if (listType == Skype.APP2APP_STREAMS.ALL_STREAMS) {
	            // Remote side dropped connection.
	            MySession.myConsole.printf("No more app2app streams.%n");
	            mySession.mySkype.streamName = "";
	    }
	}
*/

	/**
	 * AccountListener Override: Tutorial Handler - Account Properties.
	 * <br /><br />
	 * Status changes: Specifically looks for/handles login/logout status changes, and reports
	 * those changes to the associated MySession instance. Writes notice of <em>all</em>
	 * property changes to both the log and the console.
	 * <br /><br />
	 * Logout reasons: Writes reason for logout to both the log and the console. Useful for
	 * differentiating explicit logout by user (Account.LOGOUTREASON.LOGOUT_CALLED) and
	 * forced logout by the SkypeKit runtime.
	 * <br /><br />
	 * Other property changes: Writes the name of the changed property to the console.
	 * 
	 * @param obj
	 * 	Ignored - always assumes <em>our</em> account, so references effected through
	 * 	<code>mySession.myAccount</code>.
	 * @param prop
	 * 	The Account property that triggered this event.
	 * @param value
	 * 	Ignored.
	 *
	 * @since 1.0
	 * 
	 * @see com.skype.api.Account.AccountListener#OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Account.PROPERTY, java.lang.Object)
	 */
	public void OnPropertyChange(com.skype.api.SkypeObject obj, com.skype.api.Account.PROPERTY prop, Object value) {
		Log.d(mySession.myTutorialTag, "onPropertyChange - Account" );
		MySession.myConsole.println(mySession.myTutorialTag + ": " + "onPropertyChange - Account" );

	    if (prop == Account.PROPERTY.status) {
			Account.STATUS accountStatus =
				Account.STATUS.get(mySession.myAccount.GetIntProperty(Account.PROPERTY.status));
            mySession.setLoginStatus(accountStatus);
	        if (accountStatus == Account.STATUS.LOGGED_IN) {
	            Log.d(mySession.myTutorialTag, "Login complete.");
	    		MySession.myConsole.println(mySession.myTutorialTag + ": " + "Login complete." );
	        }
	        else if ((accountStatus == Account.STATUS.LOGGED_OUT) ||
	        		(accountStatus == Account.STATUS.LOGGED_OUT_AND_PWD_SAVED)) {
	            Log.d(mySession.myTutorialTag, "Login in progress/Logout complete.");
	    		MySession.myConsole.println(mySession.myTutorialTag + ": " + "Login in progress/Logout complete." );
	        }
	        else {
	        	String otherStatus = new String("Account Status = " + accountStatus.toString());
	        	Log.d(mySession.myTutorialTag, otherStatus);
	        	MySession.myConsole.printf("%s: %s%n", mySession.myTutorialTag, otherStatus);
	        }
	    }
	    else if (prop == Account.PROPERTY.logoutreason) {
 	    	Account.LOGOUTREASON logoutReason =
	    		Account.LOGOUTREASON.get(mySession.myAccount.GetIntProperty(Account.PROPERTY.logoutreason));
	    	MySession.myConsole.printf("%s: logoutreason = %s%n",
	    						mySession.myTutorialTag, logoutReason.toString());
	    }
	    else {
	    	String propName = prop.toString();
	    	MySession.myConsole.printf("%s: %s property changed!%n",
	    						mySession.myTutorialTag, propName);
	    }
	}


	/**
	 * Business logic for answering a call (Tutorial_5 - Find conversation to join).
	 * <br /><br />
	 * Since this method is invoked from the Conversation event handler
	 * {@link #OnPropertyChange(com.skype.api.SkypeObject, com.skype.api.Conversation.PROPERTY, Object )},
 	 * it's most convenient to place it here in the JavaTutorialListeners class.
	 * 
	 * @return
	 * <ul>
	 *   <li>true: call picked up</li>
	 *   <li>false: no call to pick up/call not answered/error</li>
	 * </ul>
	 * 
	 * @since 1.0
	 */
	public boolean doPickUpCall() {
		Conversation[] liveConversations = mySession.mySkype.GetConversationList(Conversation.LIST_TYPE.LIVE_CONVERSATIONS);
		if (liveConversations.length == 0) {
			MySession.myConsole.printf("%s: No live conversations to pick up!%n", mySession.myTutorialTag);
			return (false);
		}
		
		Conversation targetConversation = liveConversations[0];

		Participant[] callerList = targetConversation.GetParticipants(Conversation.PARTICIPANTFILTER.OTHER_CONSUMERS);
		StringBuffer displayParticipantsStr = new StringBuffer();

		displayParticipantsStr.setLength(0);
		int i;
		int j = callerList.length;
		for (i = 0; i < j; i++) {
			displayParticipantsStr.append((" " + callerList[i].GetStrProperty(Participant.PROPERTY.identity)));
		}

		Conversation.LOCAL_LIVESTATUS liveStatus =
			Conversation.LOCAL_LIVESTATUS.get(targetConversation.GetIntProperty(Conversation.PROPERTY.local_livestatus));
		switch (liveStatus) {
		case RINGING_FOR_ME:
			MySession.myConsole.println("RING RING...");
			MySession.myConsole.printf("Incoming call from: %s %n", displayParticipantsStr.toString());
			targetConversation.JoinLiveSession(targetConversation.GetJoinBlob());
			return (true);
			// break;
		case IM_LIVE:
			MySession.myConsole.printf("Another call is coming in from : %s %n", displayParticipantsStr.toString());
			MySession.myConsole.println("As we already have a live session up, we will reject it.");
			targetConversation.LeaveLiveSession(true);
			break;
		default:
			Log.d(mySession.myTutorialTag, "Ignoring LiveStatus " + liveStatus.toString());
			break;
		}

		return (false);
	}
}
