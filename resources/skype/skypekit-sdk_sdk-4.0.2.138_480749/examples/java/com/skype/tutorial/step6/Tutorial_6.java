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

package com.skype.tutorial.step6;

import com.skype.api.Account;
import com.skype.api.Conversation;
import com.skype.api.Participant;

import com.skype.tutorial.appkeypair.AppKeyPairMgr;
import com.skype.tutorial.util.MySession;
import com.skype.tutorial.util.SignInMgr;

/**
 * Getting Started With SkypeKit: Tutorial Application, Step 6.
 *
 * This example illustrates a simple SkypeKit-based program that:
 * <ol>
 *   <li>Takes a Skype Name, password, target Contact information, and
 *       optional AppKeyPair PEM file pathname as command-line arguments</li>
 *   <li>Initiates login for that user</li>
 *   <li>Waits until the login process is complete</li>
 *   <li>Attempts to place an audio call to the specified Contact</li>
 *   <li>Initiates logout when the call completes or fails to connect</li>
 *   <li>Waits until logout is complete</li>
 *   <li>Cleans up and exits</li>
 * </ol>
 * 
 * @author Andrea Drane (ported from existing C++ tutorial code)
 * 
 * @since 1.0
 */
public class Tutorial_6 { 
    /**
	 * Info/Debug console output message prefix/identifier tag.
	 * Corresponds to class name.
	 * 
	 * @since 1.0
	 */
    public static final String MY_CLASS_TAG = "Tutorial_6";

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
	 * Index of the call target name in the command line argument list.
	 * 
	 * @since 1.0
	 */
    public static final int CALL_TGT_IDX = 2;

	/**
	 * Number of required arguments in the command line argument list.
	 * 
	 * @since 1.0
	 */
    public static final int REQ_ARG_CNT = 3;

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

    /**
	 * Call target Skype Name.
	 * 
	 * @since 1.0
	 */
    private static String myCallTarget;

    private static AppKeyPairMgr myAppKeyPairMgr = new AppKeyPairMgr();
    private static MySession mySession = new MySession();

    /**
     * "Extraneous" instance of this tutorial so we can invoke our business logic
     * method from {@link #main(String[])} without having to declare it as being "static".
     * 
     * @since 1.0
     */
    private static Tutorial_6 myTutorial = new Tutorial_6(); 

	/**
	 * Main loop
	 * 
	 * @param args
	 * <ol>
	 *   <li>Name of the target Skype account.</li>
	 *   <li>Password for the target Skype account.</li>
	 *   <li>Skype Name of the person to call.</li>
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

		myCallTarget = args[CALL_TGT_IDX].toString();
		MySession.myConsole.printf("%s: Call target = %s%n", MY_CLASS_TAG, myCallTarget);

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
			myTutorial.doMakeCall(mySession, myCallTarget);
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
	 * Call someone in your Contacts.
	 * <ol>
	 *   <li>Obtain the list of available playback and recording devices.</li>
	 *   <li>Set the current devices (input, output, notification) to the first device
	 *   	 in their respective list, and display their names.</li>
	 *   <li>Obtain the list of my Contacts, and find the target caller in it.
	 *   	 If not found, display an appropriate message and return.</li>
	 *   <li>Attempt to call that Contact.</li>
	 *   <li>Wait until the call finishes</li>
	 * </ol>
	 * 
	 * @param mySession
	 *	Populated session object
	 * @param myCallTarget
	 * 	The Skype Name of the person to call.
	 *  
	 * @since 1.0
	 */
	void doMakeCall(MySession mySession, String myCallTarget) {

		// Get available playback/recording devices; choose first of each
		if (mySession.mySkype.SetupAudioDevices(0,0)) {
			MySession.myConsole.printf("%s: Audio device set-up completed!%n", mySession.myTutorialTag);
		}
		else {
			MySession.myConsole.printf("%s: Audio device set-up failed - exiting!%n", mySession.myTutorialTag);
			return;
		}

	    String[] callTargets = {new String(myCallTarget)}; // Create & initialize the array in one step...
		Conversation myConversation =
			(Conversation)mySession.mySkype.GetConversationByParticipants(callTargets, true, true);

		Participant[] convParticipantList = myConversation.GetParticipants(Conversation.PARTICIPANTFILTER.ALL);

		int	i;
		int j = convParticipantList.length;
		boolean callTargetFound = false;
		for (i = 0; i < j; i++) {
			if (convParticipantList[i].GetStrProperty(Participant.PROPERTY.identity).equals(myCallTarget)) {
				callTargetFound = true;
				break;
			}
		}

		if (!callTargetFound) {
			MySession.myConsole.printf("Could not find call target  %s%n", myCallTarget);
			return;
		}

		MySession.myConsole.printf("Calling %s%n", myCallTarget);	// Initiate the call
		mySession.callActive = true;
		convParticipantList[i].Ring(myCallTarget, false, 0, 10, false,
									mySession.myAccount.GetStrProperty(Account.PROPERTY.skypename));
		
		// Loop until the call finishes
		while (mySession.callActive) {
			try {
				Thread.sleep(SignInMgr.DELAY_INTERVAL);
			}
			catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return;
			}
		}
	}
}

