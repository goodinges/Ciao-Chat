/**
 * Copyright (C) 2010, Skype Limited
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

package com.skype.skypekitclient;

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
import com.skype.api.SkypeObject;
import com.skype.api.Sms;
import com.skype.api.Sms.SmsListener;
import com.skype.api.Transfer;
import com.skype.api.Transfer.TransferListener;
import com.skype.api.Video;
import com.skype.api.Video.VideoListener;
import com.skype.api.Voicemail;
import com.skype.api.Voicemail.VoicemailListener;
import com.skype.ipc.RootObject.ErrorListener;
import com.skype.util.Log;

/**
 *
 */
public class SkypekitListeners implements AccountListener, SkypeListener, ContactListener, ContactGroupListener,
    ContactSearchListener, ConversationListener, MessageListener, ParticipantListener, SmsListener, TransferListener,
    VideoListener, VoicemailListener, ErrorListener
{
    private static final String TAG      = "SkypekitListeners";

    public boolean              loggedIn = false;

    private Skype               skype;

    private jwcObserver         mObserver;
    private SkypekitClient      mClient;

    public interface jwcObserver
    {
        public abstract void OnConversationListChange(Conversation conversation, LIST_TYPE type, boolean added);
        public abstract void OnIncomingDTMF(SkypeObject obj, DTMF dtmf);
        public abstract void OnMessage(Message message, boolean changesInboxTimestamp, Message supersedesHistoryMessage, Conversation conversation);
        public abstract void OnNewCustomContactGroup(ContactGroup group);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.Account.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.Contact.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.ContactGroup.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.ContactSearch.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.Conversation.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.Message.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.Participant.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.Sms.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.Transfer.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.Video.PROPERTY prop, Object value);
        public abstract void OnPropertyChange(SkypeObject obj, com.skype.api.Voicemail.PROPERTY prop, Object value);
        public abstract void onAccountStatusChange();
        public abstract void onLiveStatusChange(Conversation conv, Conversation.LOCAL_LIVESTATUS status);
        public abstract void onVideoDimensionsChange(Video v, String value);
        public abstract void onVideoErrorChange(Video v, String error);
		public abstract void OnH264Activated(SkypeObject obj);
    }

    public SkypekitListeners(SkypekitClient observer, Skype theSkype)
    {
        this.skype = theSkype;
        this.mObserver = observer;
        this.mClient = observer;
        Log.d(TAG, "ctor time to register the listeners");
        registerAllListeners();
        
        mClient.skype.SetErrorListener(this);
    }

    public void registerListener(int modid)
    {
        // Register the listener with Skype service
        skype.RegisterListener(modid, this);
    }

    public void registerAllListeners()
    {
        registerListener(Skype.getmoduleid());
        
        registerListener(Account.moduleID());
        registerListener(Contact.moduleID());
        registerListener(ContactGroup.moduleID());
        registerListener(ContactSearch.moduleID());
        registerListener(Conversation.moduleID());
        registerListener(Message.moduleID());
        registerListener(Participant.moduleID());
        registerListener(Sms.moduleID());
        registerListener(Transfer.moduleID());
        registerListener(Video.moduleID());
        registerListener(Voicemail.moduleID());
    }

    public void unRegisterListener(int modid)
    {
        // Register the listener with Skype service
        skype.UnRegisterListener(modid, this);
    }

    public void unRegisterAllListeners()
    {
        unRegisterListener(Account.moduleID());
        unRegisterListener(Contact.moduleID());
        unRegisterListener(ContactGroup.moduleID());
        unRegisterListener(ContactSearch.moduleID());
        unRegisterListener(Conversation.moduleID());
        unRegisterListener(Message.moduleID());
        unRegisterListener(Participant.moduleID());
        unRegisterListener(Sms.moduleID());
        unRegisterListener(Transfer.moduleID());
        unRegisterListener(Video.moduleID());
        unRegisterListener(Voicemail.moduleID());
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Voicemail.VoicemailListener#OnPropertyChange(com.skype.
     * api.SkypeObject, com.skype.api.Voicemail.PROPERTY, java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Voicemail.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Video.VideoListener#OnPropertyChange(com.skype.api.SkypeObject
     * , com.skype.api.Video.PROPERTY, java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Video.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see com.skype.api.Video.VideoListener#OnCaptureRequestCompleted(int,
     * boolean, byte[], int, int)
     */
    @Override
    public void OnCaptureRequestCompleted(SkypeObject obj, int requestId, boolean isSuccessful, byte[] image, int width, int height)
    {
        Log.d(TAG, "OnCaptureRequestCompleted(" + requestId + ", " + isSuccessful + ", image.length:" + image.length
                + ", " + width + ", " + height + ")");
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Transfer.TransferListener#OnPropertyChange(com.skype.api
     * .SkypeObject, com.skype.api.Transfer.PROPERTY, java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Transfer.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Sms.SmsListener#OnPropertyChange(com.skype.api.SkypeObject,
     * com.skype.api.Sms.PROPERTY, java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Sms.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Participant.ParticipantListener#OnPropertyChange(com.skype
     * .api.SkypeObject, com.skype.api.Participant.PROPERTY, java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Participant.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Participant.ParticipantListener#OnIncomingDTMF(com.skype
     * .api.Participant.DTMF)
     */
    @Override
    public void OnIncomingDTMF(SkypeObject obj, DTMF dtmf)
    {
        mObserver.OnIncomingDTMF(obj, dtmf);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Message.MessageListener#OnPropertyChange(com.skype.api.
     * SkypeObject, com.skype.api.Message.PROPERTY, java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Message.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Conversation.ConversationListener#OnPropertyChange(com.
     * skype.api.SkypeObject, com.skype.api.Conversation.PROPERTY,
     * java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Conversation.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Conversation.ConversationListener#OnParticipantListChange()
     */
    @Override
    public void OnParticipantListChange(SkypeObject obj)
    {
    	Conversation c = (Conversation)obj;
        Log.d(TAG, "OnParticipantListChange(" + c.getOid() + ")");
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Conversation.ConversationListener#OnMessage(com.skype.api
     * .Message)
     */
    @Override
    public void OnMessage(SkypeObject obj, Message message)
    {
    	Conversation c = (Conversation)obj;
        Log.d(TAG, "OnMessage(" + message + ") in conversation:" + c.getOid());
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Conversation.ConversationListener#OnSpawnConference(com
     * .skype.api.Conversation)
     */
    @Override
    public void OnSpawnConference(SkypeObject obj, Conversation spawned)
    {
        Log.d(TAG, "OnSpawnConference(" + spawned + ")");
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.ContactSearch.ContactSearchListener#OnPropertyChange(com
     * .skype.api.SkypeObject, com.skype.api.ContactSearch.PROPERTY,
     * java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.ContactSearch.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.ContactSearch.ContactSearchListener#OnNewResult(com.skype
     * .api.Contact, int)
     */
    @Override
    public void OnNewResult(SkypeObject obj, Contact contact, int rankValue)
    {
		String identity = contact.GetIdentity();
		System.out.printf("ContactSearch.OnNewResult (%d): %s\n", rankValue, identity);;
        Log.d(TAG, "OnNewResult(" + contact + ", " + rankValue + ")");
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.ContactGroup.ContactGroupListener#OnPropertyChange(com.
     * skype.api.SkypeObject, com.skype.api.ContactGroup.PROPERTY,
     * java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.ContactGroup.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.ContactGroup.ContactGroupListener#OnChangeConversation(
     * com.skype.api.Conversation)
     */
    @Override
    public void OnChangeConversation(SkypeObject obj, Conversation conversation)
    {
        Log.d(TAG, "OnChangeConversation(" + conversation + ")");
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.ContactGroup.ContactGroupListener#OnChange(com.skype.api
     * .Contact)
     */
    @Override
    public void OnChange(SkypeObject obj, Contact contact)
    {
        Log.d(TAG, "OnChange(" + contact + ")");
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Contact.ContactListener#OnPropertyChange(com.skype.api.
     * SkypeObject, com.skype.api.Contact.PROPERTY, java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Contact.PROPERTY prop, Object value)
    {
         mObserver.OnPropertyChange(obj, prop, value);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Skype.SkypeListener#OnNewCustomContactGroup(com.skype.api
     * .ContactGroup)
     */
    @Override
    public void OnNewCustomContactGroup(ContactGroup group)
    {
        Log.d(TAG, "OnNewCustomContactGroup(" + group + ")");
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Skype.SkypeListener#OnContactOnlineAppearance(com.skype
     * .api.Contact)
     */
    @Override
    public void OnContactOnlineAppearance(Contact contact)
    {
        mClient.out("OnContactOnlineAppearance: " + contact.GetStrProperty(Contact.PROPERTY.skypename));
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Skype.SkypeListener#OnContactGoneOffline(com.skype.api.
     * Contact)
     */
    @Override
    public void OnContactGoneOffline(Contact contact)
    {
        mClient.out("SKYPE.OnContactGoneOffline: " + contact.GetStrProperty(Contact.PROPERTY.skypename));
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Skype.SkypeListener#OnConversationListChange(com.skype.
     * api.Conversation, com.skype.api.Conversation.LIST_TYPE, boolean)
     */
    @Override
    public void OnConversationListChange(Conversation conversation, LIST_TYPE type, boolean added)
    {
        mObserver.OnConversationListChange(conversation, type, added);
    }

    /*
     * (non-Javadoc)
     * 
     * @see com.skype.api.Skype.SkypeListener#OnMessage(com.skype.api.Message,
     * boolean, com.skype.api.Message, com.skype.api.Conversation)
     */
    @Override
    public void OnMessage(Message message, boolean changesInboxTimestamp, Message supersedesHistoryMessage,
            Conversation conversation)
    {
        mObserver.OnMessage(message, changesInboxTimestamp, supersedesHistoryMessage, conversation);
    }

    /*
     * (non-Javadoc)
     * 
     * @see com.skype.api.Skype.SkypeListener#OnAvailableVideoDeviceListChange()
     */
    @Override
    public void OnAvailableVideoDeviceListChange()
    {
        Log.d(TAG, "OnAvailableVideoDeviceListChange()");
    }

    /*
     * (non-Javadoc)
     * 
     * @see com.skype.api.Skype.SkypeListener#OnAvailableDeviceListChange()
     */
    @Override
    public void OnAvailableDeviceListChange()
    {
        Log.d(TAG, "OnAvailableDeviceListChange()");
    }

    /*
     * (non-Javadoc)
     * 
     * @see com.skype.api.Skype.SkypeListener#OnNrgLevelsChange()
     */
    @Override
    public void OnNrgLevelsChange()
    {
        Log.d(TAG, "OnNrgLevelsChange()");
    }

    /**
     * This event can get fired on P2P connection failure during account login.
     * Note that by default, direct connection to the network is attempted. If
     * that fails, the fallback will be to connect via proxy. If that fails as
     * well, this event will get fired.
     */
    /*
     * (non-Javadoc)
     * 
     * @see com.skype.api.Skype.SkypeListener#OnProxyAuthFailure(PROXYTYPE type)
     */
    @Override
    public void OnProxyAuthFailure(PROXYTYPE type)
    {
        mClient.error("Proxy Authorization Failure:" + type);
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.skype.api.Account.AccountListener#OnPropertyChange(com.skype.api.
     * SkypeObject, com.skype.api.Account.PROPERTY, java.lang.Object)
     */
    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Account.PROPERTY prop, Object value)
    {
        mObserver.OnPropertyChange(obj, prop, value);
    }

    public void OnSkypeKitFatalError()
    {
        mClient.error("SkypeKit fatal error reported.  Continue at your own risk.");
        mClient.error("Real applications should shut down at this point.");
    }
    
    public void OnSkypeKitConnectionClosed()
    {
        mClient.error("The connection to the SkypeKit runtime has closed.");
        mClient.error("Recovery is possible is the runtime resumes and the user reconnects to it.");
    }

	@Override
    public void OnH264Activated()
    {
    }
}
