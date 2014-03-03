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

import java.io.IOException;
import java.io.File;
import java.io.FileReader;
import java.nio.CharBuffer;
import java.security.PrivateKey;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;
import java.util.ArrayList;
import java.util.List;

import com.skype.api.Account;
import com.skype.api.ContactGroup;
import com.skype.api.Contact;
import com.skype.api.Conversation;
import com.skype.api.Participant;
import com.skype.api.Skype;
import com.skype.api.Video;
import com.skype.api.Conversation.LIST_TYPE;
import com.skype.api.Conversation.LOCAL_LIVESTATUS;
import com.skype.api.Participant.VIDEO_STATUS;
import com.skype.api.Participant.VOICE_STATUS;
import com.skype.api.Video.MEDIATYPE;
import com.skype.api.Video.STATUS;
import com.skype.ipc.TCPSocketTransport;
import com.skype.ipc.TLSServerTransport;
import com.skype.ipc.Transport;
import com.skype.util.PemReader;

public class SkypeApp implements SkypeListeners.updateObserver {
	private static final String SKYPEKIT_ROOT_PATH = "/data/data/com.skype.ref/files/";
	private static final String SKYPEKIT_APP_TOKEN = "AppToken_v31.txt"; // SKYPEKIT31
	private static final String SKYPEKIT_PEM_KEY = "skypekit.pem";
	private static final String SKYPEKIT_TCP_ADDRESS = "127.0.0.1";
	private static final int SKYPEKIT_TCP_PORT = 1663;
	private static final boolean useUnsecureConnection = false;

	private String mAccountName;
	private com.skype.api.Account mAccount = null;
	private com.skype.api.Conversation mCurrentConversation;
	private com.skype.api.Video mVideoSend;
	private com.skype.api.Video mVideoRecv;
	private com.skype.api.Conversation mIncomingCallConversation;

	private updateObserver mObserver;

	public interface updateObserver {
		public abstract void onDefaultAccountChange(String defAccount);

		public abstract void onOnlineStatusChange(State state);

		public abstract void onBuddyListChange(List<Buddy> buddies);
		
		public abstract void onAlertMessage(String message);
	}

	public SkypeApp(updateObserver obs) {
		mObserver = obs;
	}

	public String getAccountName() {
		return mAccountName;
	}

	public void setAccountName(String accountName) {
		mAccountName = accountName;
	}

	abstract class State {
		protected void changeState(SkypeApp context, State state) {
			context.changeState(state);
			mObserver.onOnlineStatusChange(state);
		}

		public abstract void handle(SkypeApp context);
	}

	class LoggedOutState extends State {
		public void handle(SkypeApp context) {
		}
	}

	class LoggedInState extends State {
		public void handle(SkypeApp context) {
		}
	}

	class InCallState extends State {
		public void handle(SkypeApp context) {
		}
	}

	class CallRingingState extends State {
		public void handle(SkypeApp context) {
		}
	}

	class Buddy {
		private String mID;
		private String mDisplayName;

		Buddy(String id, String displayName) {
			mID = id;
			mDisplayName = displayName;
		}

		@Override
		public String toString() {
			return mDisplayName;
		}
	};

	private List<Buddy> mBuddies = new ArrayList<Buddy>();

	private static Skype skype;
	private Transport transport = null;
	private SkypeListeners listeners;
	private State fState = new LoggedOutState();

	public State getState() {
		return fState;
	}

	public void changeState(State state) {
		fState = state;
		mObserver.onOnlineStatusChange(fState);
	}

	public void request() {
		fState.handle(this);
	}

	private void Log(String msg) {
		android.util.Log.e("skypeapp", msg);
	}

	public boolean isConnected() {
		try {
			if (transport != null) {
				return transport.isConnected();
			}
		} catch (IOException e) {
		}
		return false;
	}

	public boolean connect() {
		if ((listeners != null) || (transport != null) || (skype != null)) {
			Log("Already connected");
			return false;
		}
		skype = new Skype();
		transport = new TCPSocketTransport(SKYPEKIT_TCP_ADDRESS, SKYPEKIT_TCP_PORT);

		try {
			if (useUnsecureConnection) {
				File file = new File(SKYPEKIT_ROOT_PATH, SKYPEKIT_APP_TOKEN);
				String appToken = null;

				int fileLength = (int) file.length();
				CharBuffer buffer = CharBuffer.allocate(fileLength);
				try {
					FileReader reader = new FileReader(file);
					reader.read(buffer.array(), 0, fileLength);
				} catch (IOException e) {
					Log("Failed to load application token: '" + SKYPEKIT_ROOT_PATH + SKYPEKIT_APP_TOKEN + "'");
					e.printStackTrace();
					disconnect();
					return false;
				}
				appToken = buffer.toString();
				Log("Loaded application token: '" + SKYPEKIT_ROOT_PATH + SKYPEKIT_APP_TOKEN + "'");

//				skype.Init(appToken, transport, false); // SKYPEKIT31
				skype.InitNonTLSInsecure(appToken, transport);
			} else {
				File file = new File(SKYPEKIT_ROOT_PATH, SKYPEKIT_PEM_KEY);
				PemReader pemreader = new PemReader(file.getPath());
				X509Certificate c = pemreader.getCertificate();
				PrivateKey p = pemreader.getKey();
				TLSServerTransport tls = new TLSServerTransport(transport, c, p);
				Log("Loaded pem key: '" + SKYPEKIT_ROOT_PATH + SKYPEKIT_PEM_KEY + "'");

				skype.Init(tls);
			}

			listeners = new SkypeListeners(skype, this);
			Log("Connecting to skypekit");
			if (!transport.isConnected()) {
				Log("Error connecting to skypekit");
				disconnect();
				return false;
			}
			Log("Connected to skypekit");
			String version = skype.GetVersionString();
			Log("skypekit version: " + version);
			mAccountName = skype.GetDefaultAccountName();
			mObserver.onDefaultAccountChange(mAccountName);
			Log("default account:" + mAccountName);
		} catch (IOException e) {
			e.printStackTrace();
			disconnect();
			return false;
		} catch (InvalidKeySpecException e) {
			e.printStackTrace();
			disconnect();
			return false;
		}
		return true;
	}

	public void disconnect() {
		try {
			if (listeners != null) {
				listeners.unRegisterAllListeners();
				listeners = null;
			}
			if (transport != null) {
				transport.disconnect();
				transport = null;
			}
			if (skype != null) {
				skype.Close();
				skype = null;
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public boolean login(String password) {
		mAccount = skype.GetAccount(getAccountName());
		mAccount.LoginWithPassword(password, true, true);
		Log("Logged in");  
		return true;
	}

	public boolean login() {
		mAccountName = skype.GetDefaultAccountName();
		if ( (mAccountName.length() != 0) && ((mAccount = skype.GetAccount( mAccountName )) != null)) {
			Log("Logging in with default account: " + mAccountName);  
			mAccount.Login(Contact.AVAILABILITY.ONLINE);
			Log("Logged in with default account");  
			return true;
		} else {
			Log("Default account is not available");
			return false;
		}
	}

	public void logout() {
	    mBuddies.clear();
	    mObserver.onBuddyListChange(mBuddies);
		mAccount.Logout(false);
	}

	public void getBuddyList() {
		ContactGroup cg = skype.GetHardwiredContactGroup(ContactGroup.TYPE.ALL_BUDDIES);
		Contact[] buddies;
		if ((cg == null) || (null == (buddies = cg.GetContacts()))) {
			return;
		}
		mBuddies.clear();
		for (Contact buddy : buddies) {
			Log(buddy.GetIdentity());
			mBuddies.add(new Buddy(buddy.GetIdentity(), buddy.GetStrProperty(Contact.PROPERTY.displayname)));
		}
		mObserver.onBuddyListChange(mBuddies);
	}

	public boolean startVideoCall(Buddy buddy) {
		if (mIncomingCallConversation != null) { // Ignore buddy parameter, go live with incoming call
			mIncomingCallConversation.JoinLiveSession("");
			mCurrentConversation = mIncomingCallConversation;
		} else {
			String names[] = { buddy.mID };
			mCurrentConversation = skype.GetConversationByParticipants(names, true, false);
			assert (mCurrentConversation != null);
			Participant[] participants = mCurrentConversation.GetParticipants(Conversation.PARTICIPANTFILTER.OTHER_CONSUMERS);
			assert (participants != null);
			assert (participants.length > 0);
			participants[0].Ring("", true, 0, 0, false, "");
		}
		return true;
	}

	public boolean stopCall() {
		assert (mCurrentConversation != null);
		mCurrentConversation.LeaveLiveSession(false);
		return false;
	}

    public void onAccountStatusChange(Account account,
            com.skype.api.Account.PROPERTY prop, Object value)
    {
        if (mAccount == null || account == null || mAccount.getOid() != account.getOid()) {
            Log("Invalid accountStatusChange event..");
            return;
        }

        if (prop == Account.PROPERTY.status) {
            Account.GetStatusWithProgressResult loginStatus = mAccount.GetStatusWithProgress();

            switch (loginStatus.status) {
            case LOGGED_IN:
                changeState(new LoggedInState());
                Log("Logged in");
                break;
            case LOGGED_OUT:
            case LOGGED_OUT_AND_PWD_SAVED:
                changeState(new LoggedOutState());
                Log("Logged out");
                break;
            }
        }
        else if (prop == Account.PROPERTY.logoutreason) {
            int i = (Integer)(value);
            Log("ACCOUNT:LOGOUTREASON = " + Account.LOGOUTREASON.get(i));

            // If the credentials are bad, die immediately
            if (i == Account.LOGOUTREASON.INVALID_APP_ID.getId()) {
                // mObserver.onAlertMessage("Fatal error: application credential failure.");
                Log("Fatal error: application credential failure.");
                System.exit(1);
            }
        }
    }
    
	private void getAvailableVideo(Participant p, boolean myStart) {
		Participant[] myself = mCurrentConversation.GetParticipants(Conversation.PARTICIPANTFILTER.MYSELF);
		Participant[] participants = mCurrentConversation.GetParticipants(Conversation.PARTICIPANTFILTER.OTHER_CONSUMERS);
		assert (myself != null);
		assert (myself.length == 1);
		assert (participants != null);
		assert (participants.length > 0);
		if (p == myself[0] || myStart) {
			Log("Getting my video");
			mVideoSend = myself[0].GetVideo();
			if (mVideoSend != null) {
				mVideoSend.Start();
			} else {
				Log("Couldn't get local video!");
			}
		}
		if (p == participants[0]) {
			Log("Getting remote video");
			mVideoRecv = participants[0].GetVideo();
			if (mVideoRecv != null) {
				mVideoRecv.Start();
			} else {
				Log("Couldn't get receive video!");
			}
		}
	}

	public void onLiveStatusChange(Conversation conversation, LOCAL_LIVESTATUS status) {
		String identity = conversation.GetStrProperty(Conversation.PROPERTY.identity);
		Log("livestatus change '" + identity + "'");
		mIncomingCallConversation = null;

		switch (status) {
		case STARTING:
			Log("STARTING");
			break;
		case IM_LIVE:
			Log("IM_LIVE");
			changeState(new InCallState());
			getAvailableVideo(null, true);
			break;
		case OTHERS_ARE_LIVE:
			Log("OTHERS_ARE_LIVE");
			break;
		case RINGING_FOR_ME:
			Log("RINGING_FOR_ME");
			mIncomingCallConversation = conversation;
			changeState(new CallRingingState());
			break;
		case RECENTLY_LIVE:
			Log("RECENTLY_LIVE");
			changeState(new LoggedInState());
			if (mVideoSend != null) {
				mVideoSend.Stop();
				mVideoSend = null;
			}
			if (mVideoRecv != null) {
				mVideoRecv.Stop();
				mVideoRecv = null;
			}
		}
	}

	public void onVideoErrorChange(Video v, String error) {
	}

	public void onVideoMediaTypeChange(Video v, MEDIATYPE mediatype) {
	}

	public void onVideoStatusChange(Video v, STATUS status) {
		if (v == mVideoSend) {
			Log("Send video status:");
		}
		if (v == mVideoRecv) {
			Log("Recv video status:");
			if (status == STATUS.AVAILABLE)
				mVideoRecv.Start();
		}
		switch (status) {
		case NOT_AVAILABLE:
			Log("NOT_AVAILABLE");
			break;
		case AVAILABLE:
			Log("AVAILABLE");
			break;
		case STARTING:
			Log("STARTING");
			break;
		case REJECTED:
			Log("REJECTED");
			break;
		case RUNNING:
			Log("RUNNING");
			break;
		case STOPPING:
			Log("STOPPING");
			break;
		case PAUSED:
			Log("PAUSED");
			break;
		case NOT_STARTED:
			Log("NOT_STARTED");
			break;
		case HINT_IS_VIDEOCALL_RECEIVED:
			Log("HINT_IS_VIDEOCALL_RECEIVED");
			break;
		case UNKNOWN:
			Log("UNKNOWN");
			break;
		case RENDERING:
			Log("RENDERING");
			break;
		case CHECKING_SUBSCRIPTION:
			Log("CHECKING_SUBSCRIPTION");
			break;
		case SWITCHING_DEVICE:
			Log("SWITCHING_DEVICE");
			break;
		}

	}

	public void onParticipantVideoStatusChange(Participant obj, VIDEO_STATUS videoStatus) {
		switch (videoStatus) {
		case VIDEO_AVAILABLE:
			getAvailableVideo(obj, false);
			break;
		}
	}

	public void OnConversationListChange(Conversation conversation, LIST_TYPE type, boolean added) {
		mIncomingCallConversation = null;
		String identity = conversation.GetStrProperty(Conversation.PROPERTY.displayname);
		Integer status = conversation.GetIntProperty(Conversation.PROPERTY.local_livestatus);
		if (added) {
			Log("onconversationListChange added '" + identity + "' status:" + status.toString());
		} else {
			Log("onconversationListChange '" + identity + "' status:" + status.toString());
		}
		if (Conversation.LOCAL_LIVESTATUS.get(status) == Conversation.LOCAL_LIVESTATUS.RINGING_FOR_ME) {
			mIncomingCallConversation = conversation;
			changeState(new CallRingingState());
		}
	}

	public void onParticipantVoiceStatusChange(Participant p, VOICE_STATUS voiceSTATUS) {
	}

	public void onVideoDimensionsChange(Video v, String value) {
		if (v == mVideoSend) {
			Log("Send video dimensions change:" + value);
		}
		if (v == mVideoRecv) {
			Log("Recv video dimensions change:" + value);
		}
	}

}
