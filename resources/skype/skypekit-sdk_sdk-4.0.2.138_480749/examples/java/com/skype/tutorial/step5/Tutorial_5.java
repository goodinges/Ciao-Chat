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

package com.skype.tutorial.step5;

import java.io.IOException;

import com.skype.api.Participant;
import com.skype.api.Skype;

import com.skype.tutorial.appkeypair.AppKeyPairMgr;
import com.skype.tutorial.util.MySession;

/**
 * Getting Started With SkypeKit: Tutorial Application, Step 5.
 *
 * Answering calls.
 *
 * Getting a list of audio devices involves two separate commands:
 * <ul>
 *   <li>Skype.GetAvailableOutputDevices</li>
 *   <li>Skype.GetAvailableRecordingDevices</li>
 * </ul>
 * 
 * Each returns three String arrays:
 * <ol>
 *   <li>device handles that can be used as arguments
 *       to other audio-related commands, such as Skype.SelectSoundDevices</li>
 *   <li>descriptive device names</li>
 *   <li>product IDs that can safely be ignored in most cases</li>
 * </ol>
 * 
 * Once the lists have been obtained, set the current sound devices to the first entry
 * in the lists. If your speakers or microphone fail to work, you might need to change
 * these values.
 * 
 * @author Andrea Drane (ported/refactored from existing C++ tutorial code)
 * 
 * @since 1.0
 */
public class Tutorial_5 { 
	/**
	 * List of callers.
	 * Maintain this list here so it won't get garbage collected during a conversation!
	 * 
	 * @since 1.0
	 * 
	 */
	Participant[] callerList;

    /**
	 * Info/Debug console output message prefix/identifier tag.
	 * Corresponds to class name.
	 * 
	 * @since 1.0
	 */
    public static final String MY_CLASS_TAG = "Tutorial_5";

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
    private static Tutorial_5 myTutorial = new Tutorial_5(); 

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
	 * @since 1.0#
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
			myTutorial.doAcceptCalls(mySession);
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
	 * Find available input/output devices, then wait for incoming calls..
	 * <ol>
	 *   <li>List the available playback and recording devices.</li>
	 *   <li>Set the current devices (input, output, notification) to the first device in their respective list.</li>
	 *   <li>Initialize the speaker volume level.</li>
	 *   <li>Wait for in-coming calls.</li>
	 * </ol>
	 * 
	 * @param mySession
	 *	Populated session object
	 * 
	 * @since 1.0
	 */
	void doAcceptCalls(MySession mySession) {

		int i;
		int j;

		Skype.GetAvailableOutputDevicesResult outputDevices = mySession.mySkype.GetAvailableOutputDevices();
		Skype.GetAvailableRecordingDevicesResult inputDevices = mySession.mySkype.GetAvailableRecordingDevices();

 		// Getting a list of audio output devices.
		MySession.myConsole.println("** Playback devices:");
		j = outputDevices.handleList.length;
	    for (i = 0; i < j; i++) {
        	MySession.myConsole.printf("\t%d. %s (%s)%n", i, outputDevices.nameList[i], outputDevices.productIdList[i]);
        }
        MySession.myConsole.println("");

		// Getting a list of audio input devices.
        MySession.myConsole.println("** Recording devices:");
		j = inputDevices.handleList.length;
        for (i = 0; i < j; i++) {
        	MySession.myConsole.printf("\t%d. %s (%s)%n", i, inputDevices.nameList[i], inputDevices.productIdList[i]);
		}
		MySession.myConsole.println("");

		// Currently setting the sound devices to the first input/output device.
		// The output and notification are routed through the same device. If you want more control,
		// don't invoke SetupAudioDevices -- instead invoke:
		// 	mySession.mySkype.SelectSoundDevices(inputDevices.handleList[0],
		//											outputDevices.handleList[0],
		//											outputDevices.handleList[0]);
		//	mySession.mySkype.SetSpeakerVolume(100);
		//
		// If your microphone or speakers fail to work, you might want
		// to change these values.
	
		if (mySession.mySkype.SetupAudioDevices(0,0)) {
			MySession.myConsole.printf("%s: Audio device set-up completed!%n", mySession.myTutorialTag);
		}
		else {
			MySession.myConsole.printf("%s: Audio device set-up failed - exiting!%n", mySession.myTutorialTag);
			return;
		}

		MySession.myConsole.printf("%s: Now accepting incoming calls...%nPress Enter to quit.%n%n", mySession.myTutorialTag);
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

		MySession.myConsole.println("");
	}
}
