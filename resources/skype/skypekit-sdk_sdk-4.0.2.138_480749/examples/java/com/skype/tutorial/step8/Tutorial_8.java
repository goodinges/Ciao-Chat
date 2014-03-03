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

package com.skype.tutorial.step8;

import com.skype.api.Contact;
import com.skype.api.ContactGroup;
import com.skype.api.Skype;

import com.skype.tutorial.appkeypair.AppKeyPairMgr;
import com.skype.tutorial.util.MySession;

/**
 * Getting Started With SkypeKit: Tutorial Application, Step 8.
 *
 * In this example, we'll write a command-line utility for adding, deleting, and 
 * listing PSTN (telephone number) contacts.
 * 
 * This example illustrates a simple SkypeKit-based program that:
 * <ol>
 *   <li>Takes a Skype Name, password, target Contact information, command, and
 *       optional AppKeyPair PEM file pathname as command-line arguments</li>
 *   <li>Initiates login for that user</li>
 *   <li>Waits until the login process is complete</li>
 *   <li>Depending on the value of the <em>command</em> argument:
 *     <ul>
 *       <li>adds the target Contact</li>
 *       <li>deletes the target Contact</li>
 *       <li>lists the user&#8217;s Contacts</li>
 *     </ul>
 *   </li>
 *   <li>Initiates logout</li>
 *   <li>Waits until logout is complete</li>
 *   <li>Cleans up and exits</li>
 * </ol>
 * 
 * @author Andrea Drane (ported from existing C++ tutorial code)
 * 
 * @since 1.0
 */
public class Tutorial_8 {
    /**
     * Command option string for adding a Contact.
     * 
     * @since 1.0
     * 
     */
	public static final String ADD_CONTACT = "-a";

    /**
     * Command option string for deleting a Contact.
     * 
     * @since 1.0
     * 
     */
	public static final String DELETE_CONTACT = "-d";

    /**
     * Command option string for listing Contact.
     * 
     * @since 1.0
     * 
     */
    public static final String LIST_CONTACTS = "-l";

	/**
	 * Info/Debug console output message prefix/identifier tag.
	 * Corresponds to class name.
	 * 
	 * @since 1.0
	 */
    public static final String MY_CLASS_TAG = "Tutorial_8";

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
	 * Index of the command option key in the command line argument list.
	 * 
	 * @since 1.0
	 */
    public static final int COMMAND_OPT_IDX = 2;

	/**
	 * Number of required arguments in the command line argument list.
	 * 
	 * @since 1.0
	 */
    public static final int REQ_ARG_CNT = 3;

	/**
	 * Index of the "optional" phone number in the command line argument list.
	 * <em>Required</em> if {@link #COMMAND_OPT_IDX} specified as -a or -d. 
	 * 
	 * @since 1.0
	 */
    public static final int PSTN_IDX = 3;

	/**
	 * Index of the "optional" Contact display name in the command line argument list.
	 * <em>Required</em> if {@link #COMMAND_OPT_IDX} specified as -a. 
	 * 
	 * @since 1.0
	 */
    public static final int DISPLAY_NAME_IDX = 4;

    /**
	 * Number of <em>optional</em> arguments in the command line argument list.
	 * This includes the <em>optional</em> AppKeyPair PEM file pathname in the
	 * command line argument list, which must always appears last.
	 * 
	 * @since 1.0
	 */
    public static final int OPT_ARG_CNT = 3;

	/**
	 * What to do with my Contacts
	 *
	 * @since 1.0
	 */
    private static String commandOpt;

    /**
	 * Phone number for the PSTN Contact
	 *
	 * @since 1.0
	 */
    private static String newPstn = "Omitted";

	/**
	 * Display name for the PSTN Contact
	 *
	 * @since 1.0
	 */
    private static String displayName = "Omitted";

    private static AppKeyPairMgr myAppKeyPairMgr = new AppKeyPairMgr();
    private static MySession mySession = new MySession();

    /**
     * "Extraneous" instance of this tutorial so we can invoke our business logic
     * method from {@link #main(String[])} without having to declare it as being "static".
     * 
     * @since 1.0
     */
    private static Tutorial_8 myTutorial = new Tutorial_8(); 

	/**
	 * Main loop
     * 
     * @param args
     * <ol>
     *   <li>Name of the target Skype account.</li>
     *   <li>Password for the target Skype account.</li>
     *   <li>One of the following commands:
     *     <ul>
     *       <li>-l (list all PSTN contacts)</li>
     *       <li>-a (add a contact)</li>
     *       <li>-d (delete a contact)</li>
     *     </ul>
     *   </li>
     *   <li>Contact's phone number (to be added or removed).</li>
     *   <li>Contact's displayname (to be added)</li>
     *   <li>Optional pathname of an AppKeyPair PEM file.</li>
     * </ol>
     * 
     * @since 1.0
     */
	public static void main(String[] args) {
		int	argsLimit = REQ_ARG_CNT;	// Command-specific maximum number of arguments so we can tell
										// if there's an optional AppKeyPair pathname specification.
										// Default to number of arguments for "list" command

		if (args.length < REQ_ARG_CNT) {
			MySession.myConsole.printf("Usage is %s accountName accountPassword -l|-a|-d| [phoneNumber displayName appKeyPairPathname]%n%n", MY_CLASS_TAG);
			return;
		}
		if (args.length > (REQ_ARG_CNT + OPT_ARG_CNT)) {
			MySession.myConsole.printf("%s: Ignoring %d extraneous arguments.%n", MY_CLASS_TAG, (args.length - REQ_ARG_CNT));
		}

		commandOpt = args[COMMAND_OPT_IDX].toString();
		// Don't get confused by just an AppKeyPair path!
		// (Command opts are all the same length...)
		if (commandOpt.length() != LIST_CONTACTS.length()) {
			MySession.myConsole.printf("%s: Required command option appears to be missing; found %s. Exiting!%n",
								MY_CLASS_TAG, commandOpt);
			return;
		}
		
		if (commandOpt.equals(DELETE_CONTACT)) {
			argsLimit = (REQ_ARG_CNT + 1);
			if (args.length < argsLimit) {
				MySession.myConsole.printf("%s: Delete Contact requires Contact's phone number!.%n", MY_CLASS_TAG);
				return;
			}
			newPstn	= args[PSTN_IDX].toString();
		}
		else if (commandOpt.equals(ADD_CONTACT)) {
			argsLimit = (REQ_ARG_CNT + 2);
			if (args.length < argsLimit) {
				MySession.myConsole.printf("%s: Add Contact requires Contact's phone number and display name!.%n", MY_CLASS_TAG);
				return;
			}
			newPstn	= args[PSTN_IDX].toString();
			displayName	= args[DISPLAY_NAME_IDX].toString();
		}

		MySession.myConsole.printf("%s: main - Account: %s%n", MY_CLASS_TAG, args[ACCOUNT_NAME_IDX]);
		MySession.myConsole.printf("\t\tCommand: %s%n", commandOpt);
		MySession.myConsole.printf("\t\tPhone Number: %s%n", newPstn);
		MySession.myConsole.printf("\t\tContact Name: %s%n", displayName);

		if (args.length > argsLimit) {
			// AppKeyPairMgr.setAppKeyPairFromFile will issue all appropriate status and/or error messages!
			if (!myAppKeyPairMgr.setAppKeyPairFromFile(args[argsLimit])) {
				return;
			}
		}

		MySession.myConsole.printf("%s: main - Creating session - Account = %s%n",
							MY_CLASS_TAG, args[ACCOUNT_NAME_IDX]);
		mySession.doCreateSession(MY_CLASS_TAG, args[ACCOUNT_NAME_IDX], myAppKeyPairMgr);

		MySession.myConsole.printf("%s: main - Logging in w/ password %s%n",
				MY_CLASS_TAG, args[ACCOUNT_PWORD_IDX]);
		if (mySession.mySignInMgr.Login(MY_CLASS_TAG, mySession, args[ACCOUNT_PWORD_IDX])) {
			myTutorial.doPstnContact(mySession, newPstn, displayName, commandOpt);
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
	 * List, add, or delete Contacts.
	 * 
	 * @param mySession
	 *	Populated session object.
	 * @param pstn
	 * 	Properly formatted phone number of the target PSTN Contact.
	 * @param displayName
	 * 	Display name of the target PSTN Contact (used by add <em>only</em>).
	 * @param commandOpt
	 * 	Command string, which must be one of:
	 * 	<ul>
	 * 	  <li>{@link #ADD_CONTACT}</li>
	 * 	  <li>{@link #DELETE_CONTACT}</li>
	 * 	  <li>{@link #LIST_CONTACTS}</li>
	 * 	</ul>
	 * 
	 * @see com.skype.tutorial.util.MySkype#GetNormalizationStr(String)
	 * 
	 * @since 1.0
	 */
	void doPstnContact(MySession mySession, String pstn, String displayName, String commandOpt) {
		
		// Verify that our command option is valid -- something our invoker should have already done!
		if ((!(commandOpt.equals(LIST_CONTACTS))) && (!(commandOpt.equals(ADD_CONTACT))) &&	
			(!(commandOpt.equals(DELETE_CONTACT)))) {
			// We shouldn't get here -- the invoker should have already validated the command.
			MySession.myConsole.printf("%s: Unrecognized command option: %s%n", mySession.myTutorialTag, commandOpt);
			return;
		}

		int	contactIdx;
		Skype.NormalizeIdentityResult nrmlResult = null;

    	ContactGroup soContactGroup = mySession.mySkype.GetHardwiredContactGroup(ContactGroup.TYPE.SKYPEOUT_BUDDIES);
		Contact[] soContactList = soContactGroup.GetContacts();
		int contactCount = soContactList.length;

		// Handle list operations...
		if (commandOpt.equals(LIST_CONTACTS)) {
			// Make sure there's something to list!
			if (contactCount == 0) {
				MySession.myConsole.printf("%s: There are no PSTN contacts.%n", mySession.myTutorialTag);
				return;
			}
			MySession.myConsole.printf("%s: Current list of PSTN contacts:%n", mySession.myTutorialTag);
			for (contactIdx = 0; contactIdx < contactCount; contactIdx++) {
 	           	MySession.myConsole.printf("%s: %d. %s (%s)%n", mySession.myTutorialTag, (contactIdx + 1),
 	            						soContactList[contactIdx].GetStrProperty(Contact.PROPERTY.pstnnumber),
 	            						soContactList[contactIdx].GetStrProperty(Contact.PROPERTY.displayname));
			}
			return;
		}

		//Handle add & delete operations...
		String contactPstn;
		boolean contactAlreadyListed = false;

		// Ensure that the pstn argument contains a valid contact identity
		nrmlResult = mySession.mySkype.GetNormalizationStr(pstn);
		if (nrmlResult.result != Skype.NORMALIZERESULT.IDENTITY_OK) {
			MySession.myConsole.printf("%s: Cannot normalize pstn %s using %s%n",
									mySession.myTutorialTag, pstn,
									mySession.mySkype.GetISOCountryInfo().countryPrefixList[0]);
			return;
		}

		// Check whether the PSTN contact already exists, which is relevant to both
		// adding and removing contacts. In current wrapper version, the only way to do this
		// is to loop over a contact group. 
		for (contactIdx = 0; contactIdx < contactCount; contactIdx++) {
        	contactPstn = soContactList[contactIdx].GetStrProperty(Contact.PROPERTY.pstnnumber);
        	if (contactPstn.equals(nrmlResult.normalized)) {
        		contactAlreadyListed = true;
        	}
   		}
		
		// Handle adding a Contact. The Contact must not exist in that group!
		if (commandOpt.equals(ADD_CONTACT)) {
			if (contactAlreadyListed) {
				MySession.myConsole.printf("%s: Error: %s already present in ContactGroup.%n",
										mySession.myTutorialTag, nrmlResult.normalized);
				return;
			}
			MySession.myConsole.printf("%s: Adding PSTN Contact...%n", mySession.myTutorialTag);
			Contact newContact = mySession.mySkype.GetContact(nrmlResult.normalized);
			if ((newContact != null) && (soContactGroup.CanAddContact(newContact))) {
				newContact.GiveDisplayName(displayName);
				soContactGroup.AddContact(newContact);
				MySession.myConsole.printf("%s: Contact %s (%s) added.%n",
										mySession.myTutorialTag, nrmlResult.normalized, displayName);
			}
			else {
				ContactGroup.TYPE soContactGroupType =
					ContactGroup.TYPE.get(soContactGroup.GetIntProperty(ContactGroup.PROPERTY.type));
				MySession.myConsole.printf("%s: Cannot add Contact %s (%s) to ContactGroup %s (\"%s\") using AddContact():%n",
										mySession.myTutorialTag, nrmlResult.normalized, displayName,
										soContactGroupType.toString(),
										soContactGroup.GetStrProperty(ContactGroup.PROPERTY.given_displayname));
				if (newContact == null) {
					MySession.myConsole.println("\tCould not create new Contact (normalized PSTN likely invalid)");
				}
				else if (!(soContactGroup.CanAddContact(newContact))) {
					MySession.myConsole.println("\tCannot add Contacts to target ContactGroup");
				}
				else {
					MySession.myConsole.println("\tReason unknown?!?%n");
				}
			}
			return;
		}

		// Handle deleting a Contact. The Contact must exist in that group!
		if (!contactAlreadyListed) {
			MySession.myConsole.printf("%s: PSTN Contact %s not present in ContactGroup.%n",
									mySession.myTutorialTag, nrmlResult.normalized);
			return;
		}

		MySession.myConsole.printf("%s: Removing PSTN Contact...%n", mySession.myTutorialTag);
		Contact removableContact = mySession.mySkype.GetContact(nrmlResult.normalized);
		if ((removableContact != null) && (soContactGroup.CanRemoveContact())) {
			String removableDisplayName = removableContact.GetStrProperty(Contact.PROPERTY.displayname);
			soContactGroup.RemoveContact(removableContact);
			// Can't include any Contact-specific identifying information in the message that we haven't already
			// extracted since RemoveContact leaves the target Contact instance in an undefined (mostly nulled-out) state!
			MySession.myConsole.printf("%s: Removed PSTN Contact %s (\"%s\").%n",
					mySession.myTutorialTag, nrmlResult.normalized, removableDisplayName);
		}
		else {
			ContactGroup.TYPE soContactGroupType =
				ContactGroup.TYPE.get(soContactGroup.GetIntProperty(ContactGroup.PROPERTY.type));
			MySession.myConsole.printf("%s: Cannot remove Contact %s from ContactGroup %s (\"%s\") using RemoveContact():%n",
									mySession.myTutorialTag, nrmlResult.normalized,
									soContactGroupType.toString(),
									soContactGroup.GetStrProperty(ContactGroup.PROPERTY.given_displayname));
			if (removableContact == null) {
				MySession.myConsole.println("\tCould not remove Contact (normalized PSTN likely invalid)");
			}
			else if (!(soContactGroup.CanRemoveContact())) {
				MySession.myConsole.println("\tCannot remove Contacts from target ContactGroup");
			}
			else {
				MySession.myConsole.println("\tReason unknown?!?%n");
			}
		}
		
		return;
	}
}
