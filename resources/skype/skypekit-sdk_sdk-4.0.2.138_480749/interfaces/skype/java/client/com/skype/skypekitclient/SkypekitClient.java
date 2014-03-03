/**
 * Copyright (C) 2010, Skype Limited
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

package com.skype.skypekitclient;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.PrivateKey;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;
import java.util.Scanner;

import com.skype.api.Account;
import com.skype.api.Account.GetStatusWithProgressResult;
import com.skype.api.Contact;
import com.skype.api.ContactGroup;
import com.skype.api.ContactSearch;
import com.skype.api.Conversation;
import com.skype.api.Contact.AVAILABILITY;
import com.skype.api.Conversation.LIST_TYPE;
import com.skype.api.Conversation.LOCAL_LIVESTATUS;
import com.skype.api.Message;
import com.skype.api.Participant;
import com.skype.api.Participant.DTMF;
import com.skype.api.Skype;
import com.skype.api.SkypeObject;
import com.skype.api.Transfer;
import com.skype.api.Video;
import com.skype.api.Voicemail;
import com.skype.ipc.TCPSocketTransport;
import com.skype.ipc.TLSServerTransport;
import com.skype.ipc.Transport;
import com.skype.skypekitclient.SkypekitListeners.jwcObserver;
import com.skype.skypekitclient.command.CommandInvoker;
import com.skype.util.Log;
import com.skype.util.PemReader;

/**
 *
 */
public class SkypekitClient implements jwcObserver
{
    private static final String TAG             = "SkypekitClient";

    private String              inetAddr        = "127.0.0.1";
    private String              pemFileName;
    private String              transportLogName = null;
    private boolean             internal        = false;
    private Transport           transport;
    private int                 numEventThreads = 1;
    private int                 portNum         = 8963;

    public Account              account;
    public Conversation         currentConversation;
    public Skype                skype;
    public SkypekitListeners    theListeners;

    /**
     * Wrap System.out to make clear the different kinds of messages emitted by
     * the client
     * 
     * @param out
     */
    public void out(String out)
    {
        System.out.println(out);
    }

    public void error(String out)
    {
        System.err.println(out);
    }

    public void fatal_exception(Exception e)
    {
        e.printStackTrace();
        System.exit(1);
    }

    private void usage(String message)
    {
        if (!message.isEmpty())
            error(message);

        System.err.println(
                "Usage: " + TAG + " [CONNECTION][OPTIONS]\n\n"
                + "CONNECTION:\n"
                + "either tcp\n"
                + "\t-i ip addr \t\t- ip address (default " + inetAddr + ").\n"
                + "\t-p port \t\t- tcp port port (default " + portNum + ").\n"
                + "or local\n"
                + "\t-l local_connection_name \n"
                + "OPTIONS:\n"
                + "\t-t app_token_filename \t- file must contain a valid application token.\n"
                + "\t-r log_file_base \t- record transport streams in log path. \"-r ./transport\" will produce ./transport_log_in.1 and  ./transport_log_out.1\n"
                + "\t-e numeventthreads\t- set number of threads to deal with events 1 (default) <= numeventthreads <= 4\n");

        System.exit(1);
    }

    private void parseArgs(String[] args)
    {
        for (int i = 0, numArgs = args.length; i < numArgs; i++) {
            if ((args[i].charAt(0) == '-') || (args[i].charAt(0) == '/')) {
                switch (args[i].charAt(1)) {
                case 'i': { // internet address
                    if ((i + 1 == numArgs) || (args[i + 1].charAt(0) == '-')) {
                        usage("malformed argument list");
                    }
                    else {
                        inetAddr = args[++i].toString();
                    }
                    break;
                }
                case 'p': { // port number
                    if ((i + 1 == numArgs) || (args[i + 1].charAt(0) == '-')) {
                        usage("malformed argument list");
                    }
                    else {
                        portNum = Integer.parseInt(args[++i]);
                    }
                    break;
                }
                case 'e': { // num threads for events
                    if ((i + 1 == numArgs) || (args[i + 1].charAt(0) == '-')) {
                        usage("malformed argument list");
                    }
                    else {
                        numEventThreads = Integer.parseInt(args[++i]);
                        // Bound number of threads to range from 1..4
                        if (numEventThreads < 1)
                            numEventThreads = 1;
                        else if (numEventThreads > 4)
                            numEventThreads = 4;
                    }
                    break;
                }
                case 'l': { // local connection?
                    if ((i + 1 == numArgs) || (args[i + 1].charAt(0) == '-')) {
                        usage("malformed argument list");
                    }
                    else {
                        // FIXME: missing feature
                        error("-l option support missing.");
                        // localName = args[++i].toString();
                    }
                    break;
                }
                case 'r': { // log transport streams?
                    if ((i + 1 == numArgs) || (args[i + 1].charAt(0) == '-')) {
                        usage("malformed argument list");
                    }
                    else {
                        transportLogName = args[++i].toString();
                    }
                    break;
                }
                case 't': { // token
                    if ((i + 1 == numArgs) || (args[i + 1].charAt(0) == '-')) {
                        usage("malformed argument list");
                    }
                    else {
                        pemFileName = args[++i].toString();
                    }
                    break;
                }
                case 'n': { // internal - no-TLS
                    internal = true;
                    break;
                }
                case 'd': { // internal - enable debug
                    Log.level = Log.Severity.kDebug;
                    break;
                }
                default:
                    usage("Unsupported argument found:" + args[i]);
                    break;
                }
            }
            else {
                usage("malformed argument list");
                break;
            }
        }
        if (pemFileName.isEmpty()) {
            usage("-t Certificate file path argument missing.");
        }
    }

    private String getPemContents() throws IOException
    {
        File tokenFile = new File(pemFileName);
        InputStream in = new FileInputStream(tokenFile);
        long fileSize = tokenFile.length();
        byte[] bytes = new byte[(int) fileSize];
        int offset = 0;
        int count = 0;
        while (offset < fileSize) {
            count = in.read(bytes, offset, (int) fileSize - offset);
            if (count >= 0)
                offset += count;
            else
                throw new IOException("Unable to read App Token file: " + tokenFile.getName());
        }
        if (in != null)
            in.close();

        String rawString = new String(bytes);
        return rawString.trim();
    }

    @SuppressWarnings("deprecation")
    private void skypeConnect()
    {
        Log.d(TAG, "skypeConnect()");

        skypeCleanup();

        skype = new Skype();

        Log.d(TAG, "create theListeners");
        theListeners = new SkypekitListeners(this, skype);

        try {
            if (internal) {
                transport = new TCPSocketTransport(inetAddr, portNum);

                Log.d(TAG, "calling skype.Init() with non-TLS connection...");
                transport.startLogging(transportLogName);

                skype.InitNonTLSInsecure(getPemContents(), transport);
            }
            else {
                PemReader donkey = new PemReader(pemFileName);
                X509Certificate c = donkey.getCertificate();
                PrivateKey p = donkey.getKey();
                Transport t = new TCPSocketTransport(inetAddr, portNum);
                
                transport = new TLSServerTransport(t, c, p);
                
                transport.startLogging(transportLogName);

                Log.d(TAG, "new TLSServerTransport created, calling skype.Init()...");
                skype.Init(transport);
            }
        }
        catch (IOException e) {
            usage("Unable to read certificate file.");
        }
        catch (InvalidKeySpecException e) {
            usage("Invalid certficate data.");
        }

        try {
            if (transport.isConnected()) {
                Log.d(TAG, "calling GetVersionString");
                String version = skype.GetVersionString();
                out("SkypeKit Version: " + version);

                Log.d(TAG, "calling loginDefaultAccount()");
                loginDefaultAccount();

            }
            else {
                error("\n::: Error connecting to skypekit, enter 'r' to reconnect...\n");
                return;
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void loginDefaultAccount()
    {
        String accname = skype.GetDefaultAccountName();
        if ((accname.length() != 0) && ((account = skype.GetAccount(accname)) != null)) {
            out("Logging in with  default account: " + accname);
            account.Login(Contact.AVAILABILITY.ONLINE);
        }
        else {
            out("Default account is not available, use aL command to login manually");
        }
    }

    public boolean isLoggedIn()
    {
        if (account != null) {
            GetStatusWithProgressResult r = account.GetStatusWithProgress();
            if (r != null && r.status.equals(Account.STATUS.LOGGED_IN)) {
                return true;
            }
        }
            
        return false;
    }

    public boolean notLoggedIn()
    {
        if (isLoggedIn())
            return false;
        error("You are not logged in");
        return true;
    }

    private void skypeCleanup()
    {
        try {
            if (theListeners != null)
                theListeners.unRegisterAllListeners();
            theListeners = null;
            if (skype != null)
                skype.Close();
            if (transport != null && transport.isConnected())
                transport.disconnect();
            skype = null;
        }
        catch (IOException e) {
            fatal_exception(e);
        }
    }

    private void eventLoop()
    {
        boolean quitEventLoop = false;
        Scanner scan = new Scanner(System.in);
        String scanLine;
        Character command;
        Character subCommand;

        CommandInvoker invoker = new CommandInvoker(this);

        try {
            while (!quitEventLoop) {
                
                if ( ! transport.isConnected()) {
                    error("\n::: Disconnected from skypekit, enter 'r' to reconnect...\n\n");
                }
                
                if (theListeners.loggedIn)
                    out("Currently Logged In as: " + account.GetStrProperty(Account.PROPERTY.skypename));

                scanLine = scan.nextLine();
                int scanLength = scanLine.length();
                if (scanLength > 0) {
                    if (scanLength != 2) {
                        switch (scanLine.charAt(0)) {
                        case '?':
                            invoker.executeHelp(transport.isConnected()); // skype.SkypekitConnected());
                            break;

                        case 'd':
                            Log.cycleDebugLevel();
                            out("\n::: setting log level to " + Log.debugLevel());
                            break;

                        case 'q':
                            out("\n::: Quiting " + TAG);
                            quitEventLoop = true;
                            break;
                        
                        case 'r':
                            out("\n::: Reconnecting...\n");
                            skypeConnect();
                            break;
                        
                        default:
                            out("\n::: Type ? for list of available commands\n");
                        }
                    }
                    else {
                        command = scanLine.charAt(0);
                        subCommand = scanLine.charAt(1);
                        invoker.executeCommand(command, subCommand);
                    }
                }
                else {
                    out("\n::: Type ? for list of available commands\n");
                }
            }

        }
        catch (IOException e) {
            fatal_exception(e);
        }
    }

    public SkypekitClient(String[] args) throws IOException
    {
        parseArgs(args);

        skypeConnect();

        eventLoop();

        skypeCleanup();

        System.exit(0);
    }

    /**
     * Java command line client for SkypeKit
     * 
     * @param args
     */
    public static void main(String[] args) throws IOException
    {
        // System.setProperty("javax.net.debug","all");
        new SkypekitClient(args);
    }

    @Override
    public void onAccountStatusChange()
    {
        Log.d(TAG, "onAccountStatusChange()");
    }

    @Override
    public void onLiveStatusChange(Conversation conv, LOCAL_LIVESTATUS status)
    {
        Log.d(TAG, "onLiveStatusChange(" + conv + ", " + status + ")");
    }

    @Override
    public void onVideoErrorChange(Video v, String error)
    {
        Log.d(TAG, "onVideoErrorChange(" + v + ", error:" + error + ")");
    }

    @Override
    public void OnConversationListChange(Conversation conversation, LIST_TYPE type, boolean added)
    {
        if (added)
            out("OnConversationListChange:" + conversation.GetStrProperty(Conversation.PROPERTY.displayname) + ", list_type " + type + ", was added");
        else
            out("OnConversationListChange:" + conversation.GetStrProperty(Conversation.PROPERTY.displayname) + ", list_type " + type + ", was removed");
            
    }

    @Override
    public void onVideoDimensionsChange(Video v, String value)
    {
        Log.d(TAG, "onVideoDimensionsChange(" + v.GetStrProperty(Video.PROPERTY.device_path) + ", " + value + ")");
    }

    @Override
    public void OnNewCustomContactGroup(ContactGroup group)
    {
        Log.d(TAG, "OnNewCustomContactGroup(" + group.GetStrProperty(ContactGroup.PROPERTY.given_displayname) + ")");
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Contact.PROPERTY prop, Object value)
    {
        Contact c = (Contact)obj;
        String skypeName = c.GetIdentity();
        if (prop == Contact.PROPERTY.availability) {
            int i = (Integer)value;
            AVAILABILITY a = Contact.AVAILABILITY.get(i);
            String as = a.toString();
            out("CONTACT." + skypeName + ":AVAILABILITY = " + as);
        }
        else if (prop == Contact.PROPERTY.mood_text) { 
            out("CONTACT." + skypeName + ":MOOD = " + c.GetStrProperty(prop));
        }
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Voicemail.PROPERTY prop, Object value)
    {
        String partnerHandle;
        String partnerDispName;
        Voicemail vm = (Voicemail)obj;
        partnerHandle = vm.GetStrProperty(Voicemail.PROPERTY.partner_handle);
        partnerDispName = vm.GetStrProperty(Voicemail.PROPERTY.partner_dispname);
        Log.d(TAG, "OnPropertyChange(" + obj + ", (Voicemail)" + prop + ", " + value + ")");
        Log.d(TAG, "Partner_handle = " + partnerHandle + ", Partner_DisplayName = " + partnerDispName);

        //String v = value.toString();
        out("VOICEMAIL." + obj.getOid() + ":" + prop.name() + " = " + value);
        out("Voicemail.OnPropertyChange - partner_handle = " + partnerHandle + ", Partner_DisplayName = " + partnerDispName);
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Video.PROPERTY prop, Object value)
    {
        Video v = (Video)obj;
        out("Video.convid = " + v.GetIntProperty(Video.PROPERTY.convo_id) + ", " + prop.name() + " = " + value);
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Transfer.PROPERTY prop, Object value)
    {
        String filePath;
        Transfer t = (Transfer)obj;
        filePath = t.GetStrProperty(Transfer.PROPERTY.filepath);
        if (prop == Transfer.PROPERTY.status) {
            Transfer.STATUS status = Transfer.STATUS.get((Integer)value);
            switch (status) {
            case CANCELLED:
            case CANCELLED_BY_REMOTE:
            case FAILED:
            case COMPLETED:
            case PAUSED:
            case REMOTELY_PAUSED:
            case NEW:
                out("TRANSFER." + filePath + ", " + t.GetStrProperty(Transfer.PROPERTY.partner_dispname) + ", STATUS = " + status);
            }
        }
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Sms.PROPERTY prop, Object value)
    {
        out("SMS." + obj.getOid() + ":" + prop.name() + " = " + value);
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Participant.PROPERTY prop, Object value)
    {
        String identity;
        Participant p = (Participant)obj;
        identity = p.GetStrProperty(Participant.PROPERTY.identity);
        // Disabling output since these notifications are mostly noise.
        Log.d(TAG, "PARTICIPANT." + identity + ": " + prop.name() + " = " + value);
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Message.PROPERTY prop, Object value)
    {
        String author;
        Message m = (Message)obj;
        author = m.GetStrProperty(Message.PROPERTY.author);
        
        out("MESSAGE." + m.getOid() + ":" + author + ", " + prop.name() + " = " + value);
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Conversation.PROPERTY prop, Object value)
    {
        String displ_name;
        Conversation c = (Conversation)obj;
        displ_name = c.GetStrProperty(Conversation.PROPERTY.displayname);
        out("CONVERSATION." + c.getOid() + ":\"" + displ_name + "\":" + prop + " = " + value);
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.ContactSearch.PROPERTY prop, Object value)
    {
        if (prop == ContactSearch.PROPERTY.contact_search_status) {
            int i = (Integer)value;
            switch (ContactSearch.STATUS.get(i)) {
            case PENDING:
                out("CONTACTSEARCH.STARTING");
                break;
            case FINISHED:
                out("CONTACTSEARCH.FINISHED");
                break;
            }
        }
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.ContactGroup.PROPERTY prop, Object value)
    {
        // TODO - figure out how to implement whether contact was added or removed  
        ContactGroup cg = (ContactGroup)obj;
        Log.d(TAG, "OnPropertyChange (ContactGroup)" + cg.GetStrProperty(ContactGroup.PROPERTY.given_displayname) + ", " + prop.name() + ", " + value + ")");
    }

    @Override
    public void OnPropertyChange(SkypeObject obj, com.skype.api.Account.PROPERTY prop, Object value)
    {
        Log.d(TAG, "OnPropertyChange" + obj + ", (Account)" + prop + ", " + value + ")");

        String skypename = "(not logged in)";
        if (account != null) {
            skypename = account.GetStrProperty(Account.PROPERTY.skypename);
        }

        if (account != null && prop == Account.PROPERTY.status) {
            Account.GetStatusWithProgressResult loginStatus = account.GetStatusWithProgress();

            if (loginStatus.status == Account.STATUS.LOGGED_IN) {
                out("Login complete.");

                // (re)create CONTACTS_WAITING_MY_AUTHORIZATION upon login so
                // the new authrequest events will be received
                ContactGroup auth_reqs_group = skype
                        .GetHardwiredContactGroup(ContactGroup.TYPE.CONTACTS_WAITING_MY_AUTHORIZATION);
                if (auth_reqs_group != null) {
                    int waiting = auth_reqs_group.GetIntProperty(ContactGroup.PROPERTY.nrofcontacts);
                    if (waiting == 0)
                        out("No pending contact authorizations.");
                    else if (waiting == 1)
                        out("One pending contact authorization.");
                    else
                        out(waiting + " pending contact authorizations.");
                }
            }
            else if ((loginStatus.status == Account.STATUS.LOGGED_OUT)
                    || (loginStatus.status == Account.STATUS.LOGGED_OUT_AND_PWD_SAVED)) {
                out("Logout complete.");
            }
            else {
                out("Account Status: " + loginStatus.status + " Progress: " + loginStatus.progress);
            }
        }
        else if (prop == Account.PROPERTY.availability) {
            int i = (Integer)(value);
            Contact.AVAILABILITY v = Contact.AVAILABILITY.get(i);
            out("ACCOUNT." + skypename + ":" + prop.name() + " = " + v.name());
        }
        else if (prop == Account.PROPERTY.suggested_skypename) {
        	String suggestions = account.GetStrProperty(prop);
            out("ACCOUNT." + skypename + ":" + prop.name() + " = " + suggestions);
        }
        else if (prop == Account.PROPERTY.logoutreason) {
            int i = (Integer)(value);
            out("ACCOUNT." + skypename + ":LOGOUTREASON" + " = " + Account.LOGOUTREASON.get(i));
        }
        else {
            out("ACCOUNT." + skypename + ":" + prop.name() + " = " + value);
        }
    }

    @Override
    public void OnIncomingDTMF(SkypeObject obj, DTMF dtmf)
    {
        out("OnIncomingDTMF(" + dtmf + ")");
    }

    @Override
    public void OnMessage(Message message, boolean changesInboxTimestamp, Message supersedesHistoryMessage,
            Conversation conversation)
    {
        Log.d(TAG, "OnMessage(message:" + message.getOid() + ", " + changesInboxTimestamp + ", " + supersedesHistoryMessage + ", conversationq:"
                + conversation.getOid() + ")");
        
        String author = message.GetStrProperty(Message.PROPERTY.author);
        int type = message.GetIntProperty(Message.PROPERTY.type);
        out("SKYPE.OnMessage." + author + " Message::TYPE = " + Message.TYPE.get(type));
        
        if (type == Message.TYPE.POSTED_FILES.getId()) {
            Transfer [] transfers = message.GetTransfers();
            for (Transfer t : transfers) {
                out("POSTED." + author + ":" + t.GetStrProperty(Transfer.PROPERTY.filename));
            }
        }
        else if (type == Message.TYPE.POSTED_TEXT.getId()) {
            out("CHAT." + author + ":" + conversation.getOid() + ":" + message.GetStrProperty(Message.PROPERTY.body_xml));
        }
        else if (type == Message.TYPE.POSTED_SMS.getId()) {
            out("SMS." + author + ":" + conversation.getOid() + ":" + message.GetStrProperty(Message.PROPERTY.body_xml));
        }
        else if (type == Message.TYPE.ADDED_CONSUMERS.getId()) {
            out("CONVERSATION." + author + ": Added Participants to conversation:" + conversation.getOid());
        }
        else if (type == Message.TYPE.RETIRED_OTHERS.getId()) {
            out("CONVERSATION." + author + ": Removed Participants from conversation:" + conversation.getOid());
        }
        else if (type == Message.TYPE.POSTED_VOICE_MESSAGE.getId()) {
            out("Voicemail from " + author);
        }
    }
	
    @Override
    public void OnH264Activated(SkypeObject obj)
    {
        out("OnH264Activated");
    }
	
}
