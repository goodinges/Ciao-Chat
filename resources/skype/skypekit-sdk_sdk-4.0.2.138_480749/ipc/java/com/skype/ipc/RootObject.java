package com.skype.ipc;

import java.io.IOException;

import com.skype.api.Account;
import com.skype.api.Contact;
import com.skype.api.ContactGroup;
import com.skype.api.ContactSearch;
import com.skype.api.Conversation;
import com.skype.api.Message;
import com.skype.api.Participant;
import com.skype.api.Skype;
import com.skype.api.SkypeObject;
import com.skype.api.Sms;
import com.skype.api.Transfer;
import com.skype.api.Video;
import com.skype.api.Voicemail;
import com.skype.util.Log;

public class RootObject implements EventHandler
{
    private static final String TAG   = "RootObject";

    private Transport           t;
//    private static boolean      debug;
    private ResponseListener    rl;
    private EventDispatcher     edp;
    private static Object       mLock = new Object();
    public ErrorListener        errorListener;

    public interface ErrorListener
    {

        /**
         * A non-recoverable error has occurred. The application is given a
         * chance to terminate gracefully.
         */
        public void OnSkypeKitFatalError();

        /**
         * The connection to the SkypeKit runtime has closed. Recovery is
         * theoretically possible if the runtime can be restarted, the
         * connection re-initialized, and session logins restored, but more
         * often this is indicative of a fatal error condition.
         */
        public void OnSkypeKitConnectionClosed();
    }

    /** Set Error Listener for Skypekit errors */
    public void SetErrorListener(ErrorListener listener)
    {
        errorListener = listener;
    }

    protected Transport getTransport()
    {
        return t;
    }

    public void HandleEvent(Event e)
    {
    }

    public void HandlePropertyChange(PropertyChange pc)
    {
    }

    public void Init(Transport t) throws IOException
    {
        InitImpl(t, null);
    }

    /**
     * For Skype internal testing use only.
     */
    @Deprecated
    public void InitNonTLSInsecure(String appToken, Transport t) throws IOException
    {
        InitImpl(t, appToken);
    }

    private void InitImpl(Transport t, String appToken) throws IOException
    {
        this.t = t;
//        RootObject.debug = debug;
        int retries = 0;
        while (true) {
            if (t.connect())
                break;

            if ((retries++) >= 10) {
                Log.e(TAG, "Transport.connect failed 10 times. Stopped trying.");
                return;
            }
            try {
                Thread.sleep(1000);
            }
            catch (InterruptedException e) {
                throw new IOException("root-object init exception");
            }
        }

        if (edp == null) {
            edp = new EventDispatcher(this);
            edp.start();
        }
        if (rl == null) {
            Log.d(TAG, "Init - creating new ResponseListener");
            rl = new ResponseListener(edp, this);
            Log.d(TAG, "Init - starting ResponseListener");
            rl.start();
            Log.d(TAG, "Init - ResponseListener started");
        }

        // for internal, non-TLS connections, send the app token
        if (appToken != null)
            SetApplicationToken( appToken );
    }

    private void SetApplicationToken( String applicationToken)
    {
        String appTokenLenHexStr=Integer.toHexString(applicationToken.length());
        while(appTokenLenHexStr.length()<8)
        {
            appTokenLenHexStr="0"+appTokenLenHexStr;
        }
        try
        {
            t.write(appTokenLenHexStr.getBytes().length,appTokenLenHexStr.getBytes());
            t.write(applicationToken.getBytes().length,applicationToken.getBytes());
        }
        catch (IOException e)
        {
            Log.e(TAG,"SetApplicationToken failure.");
            e.printStackTrace();
        }
    }

    public void Close() throws IOException
    {
        if (rl != null)
            rl.die();
        rl = null;
        if (edp != null)
            edp.Die();
        edp = null;
        if (t.isConnected())
            t.disconnect();
        t = null;
    }

    private void writeBytes(byte[] array)
    {
        try {
            t.write(array.length, array);
        }
        catch (IOException e) {
            e.printStackTrace();
            if (errorListener != null)
                errorListener.OnSkypeKitFatalError();
        }
    }

    public Response XCall(XCallRequest request)
    {
        if (notConnected()) {
            // return a nil Response object that indicates an error, required by some callers
            return new Response();
        }

        synchronized (mLock) {
            writeBytes(request.send());

            int rid = request.getRid();
            try {
                return rl.get_x_call_response(rid);
            }
            catch (InterruptedException e) {
                e.printStackTrace();
                return new Response();
            }
        }
    }

    public GetPropertyResponse GetProperty(GetPropertyRequest request)
    {
        if (notConnected()) {
            // GetProperty callers all handle null returns safely
            return null;
        }

        synchronized (mLock) {
            writeBytes(request.send());

            return rl.get_prop_response();
        }
    }

    public GetPropertyResponse MultiGetProperty(MultiGetPropertyRequest request)
    {
        if (notConnected()) {
            // MultiGetProperty callers require a non-null return that is checked for error status,
            // so we construct an empty response that indicates an error
            return new GetPropertyResponse();
        }

        synchronized (mLock) {
            writeBytes(request.send());

            return rl.get_prop_response();
        }
    }

    private boolean notConnected()
    {
        boolean notListening = rl == null || rl.isDead();
        if (notListening) {
            try {
                Close();
            }
            catch (IOException e) { }

            if (errorListener != null) {
                errorListener.OnSkypeKitFatalError();
            }
        }
        return notListening;
    }

    public SkypeObject factory(int moduleId, int objectId, Skype skype)
    {
        SkypeObject object = null;
        if (skype.object_list.containsKey(objectId)) {
            object = skype.object_list.get(objectId);
        }
        else {
            switch (moduleId) {
            case  1:  // ContactSearch
                object = new ContactSearch(objectId, skype);
                break;
            case  2:  // Contact
                object = new Contact(objectId, skype);
                break;
            case  5:  // Account
                object = new Account(objectId, skype);
                break;
            case  6:  // Transfer
                object = new Transfer(objectId, skype);
                break;
            case  7:  // Voicemail
                object = new Voicemail(objectId, skype);
                break;
            case  9:  // Message
                object = new Message(objectId, skype);
                break;
            case 10:  // ContactGroup
                object = new ContactGroup(objectId, skype);
                break;
            case 11:  // Video
                object = new Video(objectId, skype);
                break;
            case 12:  // Sms
                object = new Sms(objectId, skype);
                break;
            case 18:  // Conversation
                object = new Conversation(objectId, skype);
                break;
            case 19:  // Participant
                object = new Participant(objectId, skype);
                break;
            }
        }
        return object;
    }

}
