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

package com.skype.tutorial.step11;

import com.skype.tutorial.appkeypair.AppKeyPairMgr;
import com.skype.tutorial.util.MySession;

/**
 * Getting Started With SkypeKit: Tutorial Application, Step 11.
 *
 * This example illustrates a simple SkypeKit-based program that:
 * <ol>
 *   <li>Takes a Skype Name, password, target Contact information, and
 *       optional AppKeyPair PEM file pathname as command-line arguments</li>
 *   <li>Initiates login for that user</li>
 *   <li>Waits until the login process is complete</li>
 *	 <li>Uses Skype application to application (app2app) features to make a datagram-based
 *	     command-line chat between two instances of the same SkypeKit client</li>
 *   <li>Initiates logout</li>
 *   <li>Waits until logout is complete</li>
 *   <li>Cleans up and exits</li>
 * </ol>
 * 
 * @author Andrea Drane (ported from existing C++ tutorial code)
 * 
 * @since 1.0
 */
public class Tutorial_11 { 
	/**
	 * The name used to create/identify our chat. Each participating Skype client must use this
	 * name to create/connect to the chat.
	 * 
	 * @since 1.0
	 */
	static final String appName = "TestApp1";
	
	/**
	 * Maximum number of characters to accumulate before sending them off to
	 * whoever we're chatting with. Used both to size our buffers and
	 * as a loop limit.
	 * 
	 * @since 1.0
	 */
	static final int TXT_CHUNK_SZ = 80;

	/**
	 * Command "string" denoting exit from app2app chat.
	 * 
	 * @since 1.0
	 */
	static final int QUIT_CMD_CHAR = 'q';

	/**
	 * Info/Debug console output message prefix/identifier tag.
	 * Corresponds to class name.
	 * 
	 * @since 1.0
	 */
    public static final String MY_CLASS_TAG = "Tutorial_11";

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
	 * Index of the Contact name in the command line argument list.
	 * 
	 * @since 1.0
	 */
    public static final int CONTACT_NAME_IDX = 2;

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
    public static final int OPT_ARG_CNT = 2;

	/**
	 * Index of the <em>optional</em> AppKeyPair PEM file pathname in
	 * the command line argument list, which is always last.
	 * 
	 * @since 1.0
	 */
    public static final int APP_KEY_PAIR_IDX = ((REQ_ARG_CNT + OPT_ARG_CNT) - 1);

	/**
	 * Target Contact name.
	 * @since 1.0
	 */
    private static String myContactName;

    private static AppKeyPairMgr myAppKeyPairMgr = new AppKeyPairMgr();
    private static MySession mySession = new MySession();

    /**
     * "Extraneous" instance of this tutorial so we can invoke our business logic
     * method from {@link #main(String[])} without having to declare it as being "static".
     * 
     * @since 1.0
     */
    private static Tutorial_11 myTutorial = new Tutorial_11(); 

	/**
	 * Main loop - App2App Datagram
	 * 
	 * @param args
	 * <ol>
	 *   <li>Name of the target Skype account.</li>
	 *   <li>Password for the target Skype account.</li>
	 *   <li>Skype Name of the target chat participant.</li>
     *   <li>Pathname of a PEM file.</li>
	 * </ol>
	 * 
	 * @since 1.0
	 */
	public static void main(String[] args) {

		if (args.length < REQ_ARG_CNT) {
			MySession.myConsole.printf("Usage is %s accountName accountPassword contactName [appTokenPathname]%n%n", MY_CLASS_TAG);
			return;
		}
		if (args.length > (REQ_ARG_CNT + OPT_ARG_CNT)) {
			MySession.myConsole.printf("%s: Ignoring %d extraneous arguments.%n", MY_CLASS_TAG, (args.length - REQ_ARG_CNT));
		}

	    myContactName = args[CONTACT_NAME_IDX].toString();
		MySession.myConsole.printf("%s: Contact name = %s%n", MY_CLASS_TAG, myContactName);

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
			myTutorial.doApp2AppDatagram(mySession, myContactName);
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
	 * Make a datagram-based command-line chat between two instances of the same SkypeKit client.
	 * 
	 * @param mySession
	 *	Populated session object
	 *	@param myContactName
	 *	 Skype Name of person to connect with.
	 * 
   	 * @since 1.0
   	 */
   	void doApp2AppDatagram(MySession mySession, String myContactName) {

   	}
}
