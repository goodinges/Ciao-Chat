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

package com.skype.tutorial.step2;

import com.skype.tutorial.appkeypair.AppKeyPairMgr;

import com.skype.api.Conversation;
import com.skype.api.Participant;
import com.skype.tutorial.util.MySession;

/**
 * Getting Started With SkypeKit: Tutorial Application, Step 2.
 *
 * This example illustrates a simple SkypeKit-based program that:
 * <ol>
 *   <li>Takes a Skype Name, password, and optional AppKeyPair PEM file pathname as command-line arguments</li>
 *   <li>Initiates login for that user</li>
 *   <li>Waits until the login process is complete</li>
 *   <li>Retrieves a list of Conversation objects</li>
 *   <li>Retrieves the display name (<code>displayname</code>) property of each Conversation</li>
 *   <li>Retrieves multiple properties of the Conversation using the Get<i>xyz</>Property methods</li>
 *   <li>Initiates logout</li>
 *   <li>Waits until logout is complete</li>
 *   <li>Cleans up and exits</li>
 * </ol>
 * 
 * @author Andrea Drane (ported/refactored from existing C++ tutorial code)
 * 
 * @since 1.0
 * 
 */
public class Tutorial_2 { 
	/**
	 * List of target participants.
	 * <b><em>Manually edit</em></b> this list and re-compile this class to
	 *  search for conversations involving other participants.
	 * 
	 * @since 1.0
	 */
	public static final String[] participantNames =
											{
												"targetParticipant1",
												"targetParticipant2",
												"targetParticipant3"
											};

	/**
	 * Info/Debug console output message prefix/identifier tag.
	 * Corresponds to class name.
	 * 
	 * @since 1.0
	 */
    public static final String MY_CLASS_TAG = "Tutorial_2";

    /**
	 * Index of the account name in the command line argument list.
	 * 
	 * @since 1.0
	 */
    public static final int ACCOUNT_NAME_IDX = 0;

	/**
	 * Index of the account password in the command line argument list.
	 * 
	 * @since 1.0
	 */
    public static final int ACCOUNT_PWORD_IDX = 1;

	/**
	 * Number of required arguments in the command line argument list.
	 * 
	 * @since 1.0
	 */
    public static final int REQ_ARG_CNT = 2;

    /**
	 * Number of <em>optional</em> arguments in the command line argument list.
	 * 
	 * @since 1.0
	 */
    public static final int OPT_ARG_CNT = 1;

	/**
	 * Index of the <em>optional</em> AppKeyPair PEM file pathname in
	 * the command line argument list, which is always last.
	 * 
	 * @since 1.0
	 */
    public static final int APP_KEY_PAIR_IDX = ((REQ_ARG_CNT + OPT_ARG_CNT) - 1);

    private static AppKeyPairMgr myAppKeyPairMgr = new AppKeyPairMgr();
    private static MySession mySession = new MySession();

    /**
     * "Extraneous" instance of this tutorial so we can invoke our business logic
     * method from {@link #main(String[])} without having to declare it as being "static".
     * 
     * @since 1.0
     */
    private static Tutorial_2 myTutorial = new Tutorial_2(); 

	/**
	 * Main loop
	 * 
	 * @param args
	 * <ol>
	 *   <li>Name of the target Skype account.</li>
	 *   <li>Password for the target Skype account.</li>
     *   <li>Optional pathname of an AppKeyPair PEM file.</li>
	 * </ol>
	 * 
	 * @since 1.0
	 */
	public static void main(String[] args) {

		if (args.length < REQ_ARG_CNT) {
			MySession.myConsole.printf("Usage is %s accountName accountPassword [appTokenPathname]%n%n", MY_CLASS_TAG);
			return;
		}
		if (args.length > (REQ_ARG_CNT + OPT_ARG_CNT)) {
			MySession.myConsole.printf("%s: Ignoring %d extraneous arguments.%n", MY_CLASS_TAG, (args.length - REQ_ARG_CNT));
		}

		if (args.length > REQ_ARG_CNT) {
			// AppKeyPairMgr.setAppKeyPairFromFile will issue all appropriate status and/or error messages!
			if (!myAppKeyPairMgr.setAppKeyPairFromFile(args[APP_KEY_PAIR_IDX])) {
				return;
			}
		}
		else {
			if (!myAppKeyPairMgr.setAppKeyPairFromFile()) {
				return;
			}
		}

		MySession.myConsole.printf("%s: main - Creating session - Account = %s%n",
							MY_CLASS_TAG, args[ACCOUNT_NAME_IDX]);
		mySession.doCreateSession(MY_CLASS_TAG, args[ACCOUNT_NAME_IDX], myAppKeyPairMgr);

		MySession.myConsole.printf("%s: main - Logging in w/ password %s%n",
				MY_CLASS_TAG, args[ACCOUNT_PWORD_IDX]);
		if (mySession.mySignInMgr.Login(MY_CLASS_TAG, mySession, args[ACCOUNT_PWORD_IDX])) {
			myTutorial.doConversation(mySession);
			mySession.mySignInMgr.Logout(MY_CLASS_TAG, mySession);
		}
		else {
			MySession.myConsole.printf("%s: Account for %s does not exist.%n", MY_CLASS_TAG, mySession.myAccountName);
		}

		MySession.myConsole.printf("%s: Cleaning up...%n", MY_CLASS_TAG);
		if (mySession != null) {
			mySession.doTearDownSession();
		}
		MySession.myConsole.printf("%s: Done!%n", MY_CLASS_TAG);
	}


	/**
	 * Finds and list conversations
	 * <ol>
	 *   <li>List the display name of my conversations associated with the participants listed in {@link #participantNames}.</li>
	 *   <li>List the type and display name of <em>all</em> my conversations.</li>
	 * </ol>
	 * 
	 * @param mySession
	 *	Populated session object
	 * 
	 * @since 1.0
	 */
	void doConversation(MySession mySession) {
		int i;			// Index variable
		int j;			// Loop counter variable
		Conversation myConversation;
		String displayName = null;

		// One Conversation (using GetConversationByParticipants)
		boolean createIfNonExisting = false;
		boolean ignoreBookmarkedOrNamed = false;

		j = participantNames.length;
		if ((myConversation = mySession.mySkype.GetConversationByParticipants(participantNames, createIfNonExisting, ignoreBookmarkedOrNamed)) != null) {
			displayName = myConversation.GetStrProperty(Conversation.PROPERTY.displayname);
			MySession.myConsole.printf("Conversation display name is %s%n", displayName);
			String pluralSfx = (j < 2) ? "" : "s";
			MySession.myConsole.printf("%d other Participant%s:%n", j, pluralSfx);
		}
		else {
			MySession.myConsole.println("No conversation on this account with participants:");
		}
		j = participantNames.length;
		for (i = 0; i < j; i++) {
			MySession.myConsole.printf("\t%s%n", participantNames[i]);
		}
		
		MySession.myConsole.println("");

		// List of Conversations (using GetConversationList)
		Conversation[] myInbox = mySession.mySkype.GetConversationList(Conversation.LIST_TYPE.INBOX_CONVERSATIONS);
		
		j = myInbox.length;
		MySession.myConsole.printf("%d Conversations in myInbox:%n", j);
		for (i = 0; i < j; i++) {
			myConversation = myInbox[i];
			
			// Getting Conversation properties
			int conversationType = myConversation.GetIntProperty(Conversation.PROPERTY.type);
			displayName = myConversation.GetStrProperty(Conversation.PROPERTY.displayname);
			MySession.myConsole.printf("\tType = %d (%s); Display Name = %s%n",
				conversationType, Conversation.TYPE.get(conversationType).toString(), displayName);

			// Getting Conversation participants, which always includes ourselves
			Participant[] convParticipants = myConversation.GetParticipants(Conversation.PARTICIPANTFILTER.ALL);
			int p;
			int q = convParticipants.length;
			String pluralSfx = (q <= 2) ? "" : "s";
			MySession.myConsole.printf("\t%d other Participant%s:%n", (q - 1), pluralSfx);
			for (p = 0; p < q; p++) {
				Participant myParticipant = convParticipants[p];
				String identity = myParticipant.GetStrProperty(Participant.PROPERTY.identity);

				if (identity.compareTo(mySession.myAccountName) == 0) {
					continue;	// Don't list ourselves!
				}
				MySession.myConsole.printf("\t\t%s", identity);
				String liveIdentity = myParticipant.GetStrProperty(Participant.PROPERTY.live_identity);
				if ((liveIdentity != null) && (liveIdentity.length() != 0)) {
					MySession.myConsole.printf(" (%s)%n", liveIdentity);
				}
				else {
					MySession.myConsole.println("");
				}
			}
			MySession.myConsole.println("");
		}
	}
}
