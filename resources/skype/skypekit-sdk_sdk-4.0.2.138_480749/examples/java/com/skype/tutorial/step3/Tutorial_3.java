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

package com.skype.tutorial.step3;

import java.io.IOException;

import com.skype.tutorial.appkeypair.AppKeyPairMgr;
import com.skype.tutorial.util.MySession;

/**
 * Getting Started With SkypeKit: Tutorial Application, Step 3.
 *
 * This example illustrates a simple SkypeKit-based program that:
 * <ol>
 *  <li>Takes a Skype Name, password, and optional AppKeyPair PEM file pathname as command-line arguments</li>
 *  <li>Initiates login for that user</li>
 *  <li>Waits until the login process is complete</li>
 *	<li>Until the user presses <strong>ENTER</strong> to quit:
 *		<ul>
 *			<li>Waits for and displays all incoming text chat messages</li>
 *			<li>Sends a &#8220;canned&#8221; message in response</li>
 *		</ul>
 *	</li>
 *  <li>Initiates logout</li>
 *  <li>Waits until logout is complete</li>
 *  <li>Cleans up and exits</li>
 * </ol>
 * 
 * @author Andrea Drane (ported/refactored from existing C++ tutorial code)
 * 
 * @since 1.0
 */
public class Tutorial_3 { 
    /**
	 * Info/Debug console output message prefix/identifier tag.
	 * Corresponds to class name.
	 * 
	 * @since 1.0
	 */
    public static final String MY_CLASS_TAG = "Tutorial_3";

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
     * method from {@link #main(String[])} without having to declare it as being "static"
     * 
     * @since 1.0
     */
    private static Tutorial_3 myTutorial = new Tutorial_3(); 

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
			myTutorial.doChat(mySession);
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
	 * Capture chat messages sent to us, and echo it to the console.
	 * <br /><br />
	 * Stop capturing chat messages when the user presses <strong>ENTER</strong>
	 * 
	 * @param mySession
	 *	Populated session object
	 * 
	 * @since 1.0
	 */
	void doChat(MySession mySession) {
	
		MySession.myConsole.printf("%s: Waiting for incoming chat messages...%nPress Enter to quit.%n%n",
				mySession.myTutorialTag);
		try {
			while (true) {
				int keyboardChar = System.in.read();
				// Some platforms think ENTER is 0x0D; others think it's 0x0A...
				if ((keyboardChar == 0x0D) || (keyboardChar == 0x0A)) {
					break;
				}
	   	   	}
		}
		catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		}

		// End capturing chat text
		MySession.myConsole.println();
	}
}
