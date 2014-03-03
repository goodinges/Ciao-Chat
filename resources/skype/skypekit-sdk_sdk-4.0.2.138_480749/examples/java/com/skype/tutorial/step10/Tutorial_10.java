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

package com.skype.tutorial.step10;

import com.skype.api.Conversation;
import com.skype.api.Participant;

import com.skype.tutorial.appkeypair.AppKeyPairMgr;
import com.skype.tutorial.util.MySession;

/**
 * Getting Started With SkypeKit. Tutorial Application, Step 10.
 * 
 * This example illustrates a simple SkypeKit-based program that:
 * <ol>
 *   <li>Takes a Skype Name, password, target Contact information, command, and
 *       optional AppKeyPair PEM file pathname as command-line arguments</li>
 *   <li>Initiates login for that user</li>
 *   <li>Waits until the login process is complete</li>
 *	 <li>Creates an HTML fragment that can be used to join public chats
 * 	     by clicking on a Web page.</li>
 *   <li>Initiates logout</li>
 *   <li>Waits until logout is complete</li>
 *   <li>Cleans up and exits</li>
 * 
 * @author Andrea Drane (ported from existing C++ tutorial code)
 * 
 * @since 1.0
 */
public class Tutorial_10 { 
	/**
	 * Info/Debug console output message prefix/identifier tag.
	 * Corresponds to class name.
	 * 
	 * @since 1.0
	 */
    public static final String MY_CLASS_TAG = "Tutorial_10";

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
    private static Tutorial_10 myTutorial = new Tutorial_10(); 

	/**
	 * Main loop
	 * 
	 * @param args
	 * <ol>
	 *   <li>Name of the target Skype account.</li>
	 *   <li>Password for the target Skype account.</li>
     *   <li>Pathname of a PEM file.</li>
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
			myTutorial.doPublicChat(mySession);
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
	 * Join a conversation.
	 * <ol>
	 *   <li>Creates a conference conversation.</li>
	 *   <li>Establishes its properties to enable joining as a SPEAKER with history visible.</li>
	 *   <li>Obtains the conversation's join BLOB.</li>
	 *   <li>Writes the href for joining the conversation to the console.</li>
	 * </ol> 
	 * 
	 * @param mySession
	 *	Populated session object
	 * 
	 * @since 1.0
	 */
	void doPublicChat(MySession mySession) {
		
		Conversation myConversation;
		
		if ((myConversation = mySession.mySkype.CreateConference()) != null) {			
			// NB! Setting chat options must be done before asking for a join BLOB.
			myConversation.SetOption(Conversation.PROPERTY.opt_joining_enabled.getId(), 1);
			myConversation.SetOption(Conversation.PROPERTY.opt_entry_level_rank.getId(), Participant.RANK.SPEAKER.getId());
			myConversation.SetOption(Conversation.PROPERTY.opt_disclose_history.getId(), 1);
			
			String convBlob;
			if ((convBlob = myConversation.GetJoinBlob()) != null) {
				MySession.myConsole.printf("You can copy/paste the following HTML link and use it in a Web page to join Public Chat:%n%n");
				MySession.myConsole.printf("<a href=\"skype:?chat&blob=%s\">Click here.</a>%n%n", convBlob);
				MySession.myConsole.printf("Note that the creator of this chat - %s - needs to be online for you to actually join.%n", mySession.myAccountName);
			}
			else {
				MySession.myConsole.printf("%s: Unable to retrieve join BLOB from conversation.%n", mySession.myTutorialTag);
			}
		}
		else {
			MySession.myConsole.printf("%s: Unable to create conversation.%n", mySession.myTutorialTag);
		}
	}
}
