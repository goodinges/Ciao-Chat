/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Skype.Util;

/**
 *
 * @author Kamran
 */
import java.io.*;

import com.skype.ipc.Transport;
import com.skype.ipc.TCPSocketTransport;
import com.skype.ipc.TLSServerTransport;

import com.skype.api.Account;
import com.skype.api.SkypeObject;

/**
 * Tutorial Session Object.
 *
 * Encapsulates common aspects of the SkypeKit-based Java tutorials.
 * These include:
 * <ul>
 * 	<li>target account name</li>
 *	<li>console PrintStream (based off <code>System.out</code>) that specifies
 *		<code>autoFlush</code> as true</li>
 *	<li>activity indicators, such as the associated Account's login status and
 *		whether a call is in progress</li>
 *	<li>transport-related members:
 *	  <ul>
 *	    <li>SkypeKit runtime IP address</li>
 *	    <li>SkypeKit runtime port number</li>
 *	    <li>com.skype.ipc.TCPSocketTransport instance</li>
 *	    <li>com.skype.ipc.TLSServerTransport instance</li>
 *	  </ul>
 *	</li>
 *	<li>{@link MySkype} instance (tutorials-specific {@link com.skype.api.Skype} class extension)</li>
 *	<li>{@link com.skype.api.Account} instance</li>
 *	<li>{@link SignInMgr} instance</li>
 *	<li>{@link Listeners} instance (tutorials-specific "helper" class instance that
 *	 	implements the various "listener" interfaces)</li>
 * </ul>
 * 
 * @author Andrea Drane (ported/refactored from existing C++ tutorial code)
 * 
 * @since 1.0
 */
public class MySession {

    /**
     * Default value for the Info/Debug console output message prefix/identifier tag,
     * in case we get passed a null or empty string.
     * 
     * @since 1.0
     */
    public static final String T_TAG_DFLT = "Tutorial_X";
    /**
     * Info/Debug console output message prefix/identifier tag.
     * Corresponds to the tutorial's class name.
     * 
     * @since 1.0
     */
    public String myTutorialTag;
    /**
     * Console PrintStream.
     * <br /><br />
     * Based off <code>System.out</code>, but specifies <code>autoFlush</code> as true to ensure
     * that console output does not intermingle since both the tutorial code and the
     * event handlers write to the console.
     * 
     * @since 1.0
     */
    public static PrintStream myConsole = new PrintStream(System.out, true);
    /**
     * Name of the target Skype account, which is actually the Skype Name
     * of the user that created it.
     * 
     * @since 1.0
     */
    public String myAccountName;
    public SignInMgr mySignInMgr = new SignInMgr();
    /**
     * Skype instance for this tutorial session.
     * 
     * @see com.skype.tutorial.util.MySkype
     * @see com.skype.api.Skype
     * 
     * @since 1.0
     */
    public MySkype mySkype = null;
    /**
     * SkypeKit version number parse instance for this tutorial session.
     * <br /><br />
     * Do <em>not</em> attempt to instantiate this instance until <em>after initializing</em>
     * {@link #mySkype}!
     * 
     * @see com.skype.tutorial.util.ParseSkypeKitVersion
     * 
     * @since 1.0
     */
    public ParseSkypeKitVersion myParseSkypeKitVersion = null;
    /**
     * Account instance for this tutorial session.
     * Set on successful login, <i>not</i> during session creation!
     * 
     * @see com.skype.api.Account
     * 
     * @since 1.0
     */
    public Account myAccount = null;
    /**
     * Whether we are currently in a call.
     * <br /><br />
     * Set to true when a Conversation goes live (<code>Conversation.LOCAL_LIVESTATUS.RINGING_FOR_ME</code>)
     * after a successful <code>Conversation.join</code> or <code>Conversation.JoinLiveSession</code>;
     * set to false when a Conversation goes non-live (<code>Conversation.LOCAL_LIVESTATUS.RECENTLY_LIVE</code> or
     * <code>Conversation.LOCAL_LIVESTATUS.NONE</code>).
     * 
     * @see com.skype.tutorial.util.Listeners#OnPropertyChange(SkypeObject, com.skype.api.Conversation.PROPERTY, Object)
     * @see com.skype.tutorial.util.Listeners#OnConversationListChange(com.skype.api.Conversation, com.skype.api.Conversation.LIST_TYPE, boolean)
     *  
     * @since 1.0
     */
    public boolean callActive = false;
    /**
     * Cached status of this session's associated Account.
     * <br /><br />
     * Initialized to <code>Account.STATUS.CONNECTING_TO_P2P</code> (initial login process state);
     * updated by Account onPropertyChange handler. We don't initialize it to
     * <code>Account.STATUS.LOGGED_OUT</code> so we can catch a failed login,
     * for example, due to <code>Account.LOGOUTREASON.INCORRECT_PASSWORD</code> 
     * 
     * @see com.skype.tutorial.util.Listeners#OnPropertyChange(SkypeObject, com.skype.api.Account.PROPERTY, Object)
     * @see com.skype.api.Account
     * 
     * @since 1.2
     */
    public Account.STATUS loginStatus = Account.STATUS.CONNECTING_TO_P2P;
    /**
     * Callbacks/event handlers for this tutorial session.
     * 
     * @since 1.0
     */
    public Listeners myJavaTutorialListeners = null;
    /**
     * Server IP Address.
     * 
     * @since 1.0
     */
    public static final String IP_ADDR = "127.0.0.1";
    /**
     * Server Port.
     * <br /><br />
     * If you modify this compiled-in default, you will need to start the matching SkypeKit runtime with option:<br />
     * &nbsp;&nbsp;&nbsp;&nbsp;<code>-p <em>9999</em></code><br />
     * where <code>-p <em>9999</em></code> reflects this value. 
     * 
     * @since 1.0
     */
    public static final int PORT_NUM = 8963;
    public Transport myTransport = new TCPSocketTransport(IP_ADDR, PORT_NUM);
    public TLSServerTransport myTLSTransport;

    /**
     * Creates <em>most</em> everything needed for a tutorial session; the Account instance is populated during sign-in. 
     * 
     * @param tutorialTag
     *  The tutorial's class name. If null or the empty string, default it to <code>T_TAG_DFLT</code>.
     * @param accountName
     *  The <em>name</em> of the account to use for this tutorial. If null or the empty string,
     *  <em>fail</em> by throwing a RuntimeException indicating that fact.
     * @param myAppKeyPairMgr
     * 	AppKeyPairMgr instance containing the certificate and key extracted from this tutorial's
     *  associated PEM file.
     * 
     * @return
     * <ul>
     *   <li>true: session initialized</li>
     *   <li>false: session initialization failed due to:
     *   	<ul>
     *   		<li>no or empty account name</li>
     *   		<li>com.skype.api.Skype.Init failed - most likely from an invalid AppKeyPair</li>
     *   		<li>could not obtain an Account instance</li>
     *   	</ul>
     *   </li>
     * </ul>
     *  
     * @see com.skype.tutorial.util.SignInMgr
     * 
     * @since 1.0
     */
    public boolean doCreateSession(String tutorialTag, String accountName, AppKeyPairMgr myAppKeyPairMgr) {

        if ((tutorialTag != null) && (tutorialTag.length() != 0)) {
            myTutorialTag = tutorialTag;
        } else {
            myTutorialTag = T_TAG_DFLT;
        }

        if ((accountName != null) && (accountName.length() != 0)) {
            myAccountName = accountName; // All tutorials minimally require an account name
        } else {
            throw new RuntimeException((myTutorialTag + ": Cannot initialize session instance - no account name!"));
        }

        // Set up our session with the SkypeKit runtime...
        // Note that most of the Skype methods - including static methods and GetVersionString - will
        // fail and/or throw an exception if invoked prior to successful initialization!
        mySkype = new MySkype(myTutorialTag);
        myTLSTransport = new TLSServerTransport(myTransport,
                myAppKeyPairMgr.getAppKeyPairCertificate(),
                myAppKeyPairMgr.getAppKeyPairPrivateKey());

        try {
            mySkype.Init(myTLSTransport);

            myParseSkypeKitVersion = new ParseSkypeKitVersion(mySkype);
            myConsole.printf("%s: Initialized MySkype instance - version = %s (%d.%d.%d)%n",
                    myTutorialTag, myParseSkypeKitVersion.getVersionStr(),
                    myParseSkypeKitVersion.getMajorVersion(),
                    myParseSkypeKitVersion.getMinorVersion(),
                    myParseSkypeKitVersion.getPatchVersion());
        } catch (IOException e1) {
            myConsole.printf("%s: Could not initialize MySkype!%n", myTutorialTag);
            return (false);
        }

        myJavaTutorialListeners = new Listeners(this);
        myConsole.printf("%s: Created JavaTutorialListeners instance...%n", myTutorialTag);

        // Get the Account
        if ((myAccount = mySkype.GetAccount(myAccountName)) == null) {
            myConsole.printf("%s: Could not get Account for %s!%n", myTutorialTag, myAccountName);
            myConsole.printf("%s: Session initialization failed!%n", myTutorialTag);
            return (false);
        }

        myConsole.printf("%s: Got Account for %s%n", myTutorialTag, myAccountName);
        myConsole.printf("%s: Initialized session!%n", myTutorialTag);
        return (true);
    }

    /**
     * Tears down a tutorial session.
     * <br /><br />
     * Specifically, this involves:
     * <ol>
     *   <li>Un-registering the listeners</li>
     *   <li>Disconnecting the transport</li>
     *   <li>"Closing" our Skype instance, which terminates the SkypeKit runtime</li> 
     * </ol> 
     * 
     * @see Listeners#unRegisterAllListeners()
     * @see com.skype.ipc.TCPSocketTransport#disconnect()
     * @see com.skype.api.Skype#Close()
     * 
     * @since 1.0
     */
    public void doTearDownSession() {

        try {
            if (myJavaTutorialListeners != null) {
                myJavaTutorialListeners.unRegisterAllListeners();
                myJavaTutorialListeners = null;
            }
            // closing Skype also disconnects the transport
            if (mySkype != null) {
                mySkype.Close();
                mySkype = null;
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        myConsole.printf("%s: Tore down session instance%n", myTutorialTag);
    }

    /**
     * Retrieves the current login status of this session's Account.
     * @return
     * 	Cached login status of this session's Account.
     * 
     * @see com.skype.tutorial.util.Listeners#OnPropertyChange(SkypeObject, com.skype.api.Account.PROPERTY, Object)
     * 
     * @since 1.0
     */
    public Account.STATUS getLoginStatus() {

        return (this.loginStatus);
    }

    /**
     * Establishes the login status of this session's Account.
     * @param loginStatus
     * 	Reported login status of this session's Account.
     * 
     * @see com.skype.tutorial.util.Listeners#OnPropertyChange(SkypeObject, com.skype.api.Account.PROPERTY, Object)
     * 
     * @since 1.0
     */
    public void setLoginStatus(Account.STATUS loginStatus) {

        this.loginStatus = loginStatus;
        return;
    }

    /**
     * Determines if an Account is signed in.
     * <br /><br />
     * Specifically, this involves examining the last cached value for
     * the associated Account's status property. Caching the status avoids
     * having to query the DB. For mobile devices, WiFi-connected laptops
     * running on battery power, and so forth this typically avoids expending
     * battery charge to transmit the server request.
     * <br /><br />
     * Essentially, <em>only</em> a current status of <code>Account.STATUS.LOGGED_IN</code>
     * returns true.
     * <b><i>"Not logged in" is not the same as "Logged out"!</i></b>
     * 
     * @return
     * <ul>
     *   <li>true: currently signed in</li>
     *   <li>false: currently signed out <em>or</em> target Account is null</li>
     * </ul>
     * 
     * @see com.skype.tutorial.util.SignInMgr#isLoggedIn(Account)
     * 
     * @since 1.0
     */
    public boolean isLoggedIn() {

        if (this.loginStatus == Account.STATUS.LOGGED_IN) {
            return (true);
        }
        return (false);
    }

    /**
     * Determines if an Account is signed out.
     * <br /><br />
     * Specifically, this involves examining the last cached value for
     * the associated Account's status property. Caching the status avoids
     * having to query the DB. For mobile devices, WiFi-connected laptops
     * running on battery power, and so forth this typically avoids expending
     * battery charge to transmit the server request.
     * <br /><br />
     * Essentially, <em>only</em> a current status of <code>Account.STATUS.LOGGED_OUT</code>
     * returns true.
     * <b><i>"Not logged out" is not the same as "Logged in"!</i></b>
     * 
     * @return
     * <ul>
     *   <li>true: currently signed in</li>
     *   <li>false: currently signed out <em>or</em> target Account is null</li>
     * </ul>
     * 
     * @see com.skype.tutorial.util.SignInMgr#isLoggedIn(Account)
     * 
     * @since 1.0
     */
    public boolean isLoggedOut() {

        if (this.loginStatus == Account.STATUS.LOGGED_OUT) {
            return (true);
        }
        return (false);
    }
}
