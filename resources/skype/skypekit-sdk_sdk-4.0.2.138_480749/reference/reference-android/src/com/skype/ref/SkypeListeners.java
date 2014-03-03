/**
 * Copyright (C) 2011, Skype Limited
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

package com.skype.ref;

import android.util.Log;

import com.skype.api.Account;
import com.skype.api.Contact;
import com.skype.api.ContactGroup;
import com.skype.api.ContactSearch;
import com.skype.api.Conversation;
import com.skype.api.Message;
import com.skype.api.Participant;
import com.skype.api.Skype;
import com.skype.api.SkypeObject;
import com.skype.api.Sms;
import com.skype.api.Transfer;
import com.skype.api.Video;
import com.skype.api.Voicemail;
import com.skype.api.Account.AccountListener;
import com.skype.api.Contact.ContactListener;
import com.skype.api.ContactGroup.ContactGroupListener;
import com.skype.api.ContactSearch.ContactSearchListener;
import com.skype.api.Conversation.ConversationListener;
import com.skype.api.Conversation.LIST_TYPE;
import com.skype.api.Conversation.LOCAL_LIVESTATUS;
import com.skype.api.Message.MessageListener;
import com.skype.api.Participant.DTMF;
import com.skype.api.Participant.ParticipantListener;
import com.skype.api.Participant.VIDEO_STATUS;
import com.skype.api.Participant.VOICE_STATUS;
//import com.skype.api.Skype.APP2APP_STREAMS;
import com.skype.api.Skype.PROXYTYPE;
import com.skype.api.Skype.SkypeListener;
import com.skype.api.Sms.SmsListener;
import com.skype.api.Transfer.TransferListener;
import com.skype.api.Video.MEDIATYPE;
import com.skype.api.Video.STATUS;
import com.skype.api.Video.VideoListener;
import com.skype.api.Voicemail.VoicemailListener;

public class SkypeListeners implements AccountListener, SkypeListener,
		VideoListener, ParticipantListener, ConversationListener,
		ContactListener, ContactGroupListener, ContactSearchListener,
		MessageListener, SmsListener, TransferListener, VoicemailListener {

	private Skype skype;
	private updateObserver mObserver;

	public interface updateObserver {
        public abstract void onAccountStatusChange(Account account,
	            com.skype.api.Account.PROPERTY prop, Object value);

		public abstract void onLiveStatusChange(Conversation conv, Conversation.LOCAL_LIVESTATUS status);

		public abstract void onVideoStatusChange(Video v, STATUS status);

		public abstract void onVideoErrorChange(Video v, String error);

		public abstract void onVideoMediaTypeChange(Video v, MEDIATYPE mediatype);

		public abstract void onParticipantVideoStatusChange(Participant obj, VIDEO_STATUS videoStatus);

		public abstract void OnConversationListChange(Conversation conversation, LIST_TYPE type, boolean added);

		public abstract void onParticipantVoiceStatusChange(Participant p, VOICE_STATUS voiceSTATUS);

		public abstract void onVideoDimensionsChange(Video v, String value);
	}

	public SkypeListeners(Skype skype, updateObserver obs) {
		this.skype = skype;
		mObserver = obs;
		registerAllListeners();
	}

	protected void finalize() throws Throwable {
		unRegisterAllListeners();
	}

	public void registerListener(int modid) {
		// Register the listener with Skype service
		skype.RegisterListener(modid, this);
	}

	public void registerAllListeners() {
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

	public void unRegisterListener(int modid) {
		// Register the listener with Skype service
		skype.UnRegisterListener(modid, this);
	}

	public void unRegisterAllListeners() {
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

	public void OnApp2AppDatagram(String appname, String stream, byte[] data) {
		// TODO Auto-generated method stub

	}

//	public void OnApp2AppStreamListChange(String appname,
//			APP2APP_STREAMS listType, String[] streams) {
//		// TODO Auto-generated method stub
//
//	}

	public void OnAvailableDeviceListChange() {
		// TODO Auto-generated method stub

	}

	public void OnAvailableVideoDeviceListChange() {
		// TODO Auto-generated method stub

	}

	public void OnContactGoneOffline(Contact contact) {
		// TODO Auto-generated method stub

	}

	public void OnContactOnlineAppearance(Contact contact) {
		// TODO Auto-generated method stub

	}

	public void OnConversationListChange(Conversation conversation, LIST_TYPE type, boolean added) {
		mObserver.OnConversationListChange(conversation, type, added);
	}

	public void OnMessage(Message message, boolean changesInboxTimestamp, Message supersedesHistoryMessage, Conversation conversation) {
		// TODO Auto-generated method stub

	}

	public void OnNewCustomContactGroup(ContactGroup group) {
		// TODO Auto-generated method stub

	}

	public void OnNrgLevelsChange() {
		// TODO Auto-generated method stub

	}

	public void OnProxyAuthFailure(PROXYTYPE type) {
		// TODO Auto-generated method stub

	}

	public void OnCaptureRequestCompleted(SkypeObject obj, int requestId, boolean isSuccessful,
			byte[] image, int width, int height) {
		// TODO Auto-generated method stub

	}

	public void OnLastFrameCapture(byte[] image, int width, int height) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj, com.skype.api.Video.PROPERTY prop, Object value) {
		Log.e("TaG", "videopropchange:");
		Video v = (Video) obj;
		switch (prop) {
		case dimensions:
			mObserver.onVideoDimensionsChange(v, (String) value);
			break;
		case error:
			mObserver.onVideoErrorChange(v, (String) value);
			break;
		case status:
			mObserver.onVideoStatusChange(v, Video.STATUS.get((Integer) value));
			break;
		case media_type:
			mObserver.onVideoMediaTypeChange(v, Video.MEDIATYPE.get((Integer) value));
		}

	}

	public void OnIncomingDTMF(SkypeObject obj, DTMF dtmf) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj,
			com.skype.api.Participant.PROPERTY prop, Object value) {
		Participant p = (Participant) obj;
		switch (prop) {
		case video_status:
			mObserver.onParticipantVideoStatusChange(p, Participant.VIDEO_STATUS.get((Integer) value));
			break;
		case voice_status:
			Log.e("TaG", "participant voice_status change");
			mObserver.onParticipantVoiceStatusChange(p, Participant.VOICE_STATUS.get((Integer) value));
			break;
		}
	}

	public void OnMessage(SkypeObject obj, Message message) {
		// TODO Auto-generated method stub

	}

	public void OnParticipantListChange(SkypeObject obj) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj,
			com.skype.api.Conversation.PROPERTY prop, Object value) {
		switch (prop) {
		case local_livestatus:
			Integer enVal = (Integer) value;
			mObserver.onLiveStatusChange((Conversation) obj, LOCAL_LIVESTATUS.values()[enVal]);
		}

	}

	public void OnSpawnConference(SkypeObject obj, Conversation spawned) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj,
			com.skype.api.Contact.PROPERTY prop, Object value) {
		// TODO Auto-generated method stub
	}

	public void OnChange(SkypeObject obj, Contact contact) {
		// TODO Auto-generated method stub

	}

	public void OnChangeConversation(SkypeObject obj, Conversation conversation) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj,
			com.skype.api.ContactGroup.PROPERTY prop, Object value) {
		// TODO Auto-generated method stub

	}

	public void OnNewResult(SkypeObject obj, Contact contact, int rankValue) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj,
			com.skype.api.ContactSearch.PROPERTY prop, Object value) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj,
			com.skype.api.Message.PROPERTY prop, Object value) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj,
			com.skype.api.Sms.PROPERTY prop, Object value) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj,
			com.skype.api.Transfer.PROPERTY prop, Object value) {
		// TODO Auto-generated method stub

	}

	public void OnPropertyChange(SkypeObject obj,
			com.skype.api.Voicemail.PROPERTY prop, Object value) {
		// TODO Auto-generated method stub

	}

    public void OnPropertyChange(SkypeObject obj,
            com.skype.api.Account.PROPERTY prop, Object value)
    {
        mObserver.onAccountStatusChange((Account)obj, prop, value);
    }

	public void OnH264Activated()
    {
	    // TODO Auto-generated method stub
	    
    }
}
