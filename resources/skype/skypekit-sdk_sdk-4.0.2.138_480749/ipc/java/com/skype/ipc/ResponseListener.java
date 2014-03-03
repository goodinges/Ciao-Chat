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

package com.skype.ipc;

import java.io.EOFException;
import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import com.skype.ipc.RootObject.ErrorListener;
import com.skype.util.Log;

public class ResponseListener extends Thread
{
    private static final String                      TAG                    = "ResponseListener";
    private static final int                         XCALL_RESPONSE_TIMEOUT = 60000;
    private Thread                                   my_thread;
    private Transport                                ioTransport;
    private boolean                                  die;
    private final Map<Integer, Response>             response_table;
    private final BlockingQueue<GetPropertyResponse> get_prop_response_queue;
    private EventDispatcher                          myEdp;

    private boolean                                  responseWaitInterrupted;
    private ErrorListener                            errorListener;

    public ResponseListener(EventDispatcher edp, RootObject root)
    {
        myEdp = edp;
        ioTransport = root.getTransport();
        errorListener = root.errorListener;
        setName("response listener thread");
        die = false;
        response_table = Collections.synchronizedMap(new HashMap<Integer, Response>());
        get_prop_response_queue = new LinkedBlockingQueue<GetPropertyResponse>();
        responseWaitInterrupted = false;
    }

    public boolean isDead()
    {
        return die;
    }
    
    public void die()
    {
        die = true;
        my_thread.interrupt(); // trigger interrupt in case of waiting
    }

    synchronized private byte readByte() throws IOException
    {
        int res = ioTransport.read();
        byte resByte = (byte) (res & 0xFF);
        Log.d(TAG, "ResponseListener::readByte: " + Integer.toHexString(res) + " ('" + (char) res + "') " + resByte);

        return resByte;
    }

    public void run()
    {
        my_thread = Thread.currentThread();
        Log.d(TAG, "Running! Threadid=" + my_thread.getId());

        try {
            byte b1 = readByte();
            byte b2 = readByte();
            if (b1 == 'O' && b2 == 'K') {
                Log.d(TAG, "Skypekit executable handshake successful");
            }
            else if (b1 == 'E' && b2 == 'R') {
                // service unavailable
                Log.e(TAG, "Skypekit executable handshake failed. Code '" + (char) b1 + (char) b2 + "'.");
            }
            else if ((b1 == 'A' && b2 == 'N') ||
                    (b1 == 'O' && b2 == 'B') ||
                    (b1 == 'S' && b2 == 'N') ||
                    (b1 == 'O' && b2 == 'M') ||
                    (b1 == 'A' && b2 == 'B') ||
                    (b1 == 'S' && b2 == 'B') ||
                    (b1 == 'P' && b2 == 'B'))
            {
                // application token rejected for one reason or another
                Log.e(TAG, "Skypekit executable handshake failed. Code '" + (char) b1 + (char) b2 + "'.");
            }
            else {
                // unknown error in handshake
                Log.e(TAG, "Unknown Skypekit executable handshake failure. Code '" + (char) b1 + (char) b2 + "'.");
            }

        }
        catch (IOException e) {
            Log.e(TAG, "exception=", e);
            e.printStackTrace();
        }
        catch (Exception e) {
            Log.e(TAG, "Unknown exception caught = ", e);
            e.printStackTrace();
            die = true;
        }

        while ( ! die) {
            try {
                int prefix = readByte();
                Log.d(TAG, "Prefix read:" + prefix);
                
                if (prefix == 'Z') {
                    int cmd = readByte();
                    Log.d(TAG, "Processing command " + cmd + " (" + (char) cmd + ")");

                    switch (cmd) {
                    case 'r':
                        x_call_response();
                        break;
                    case 'C':
                        decode_property_change();
                        break;
                    case 'E':
                        decode_event();
                        break;
                    case 'g':
                        decode_get_response();
                        break;
                    default:
                        Log.e(TAG, "Unknown command " + cmd + " (" + (char) cmd + ")");
                    }
                }
                else {
                    Log.e(TAG, "Bad symbol in the stream: " + prefix + " (" + (char) prefix + ") - discarding");
                }
            }
            catch (java.net.SocketException e) {
                Log.d(TAG, "Socket exception after normal socket shutdown");
            }
            catch (EOFException e) {
                Log.e(TAG, "EOF Exception, no socket connection:" + e.getMessage());
                if (errorListener != null)
                    errorListener.OnSkypeKitConnectionClosed();
                die = true;
            }
            catch (IOException e) {
                Log.e(TAG, "Listener IO Exception:" + e.getMessage());
                e.printStackTrace();
                die = true;
            }
            catch (Exception e) {
                Log.e(TAG, "Unknown exception caught in Listener:", e);
                e.printStackTrace();
                die = true;
            }
        }
        Log.d(TAG, "Exiting!");
    }

    void x_call_response() throws IOException
    {
        int rid = AbstractDecoder.decodeOneVaruint(ioTransport);
        Log.d(TAG, "x_call_response rid: " + rid);

        // Tell SkypeKit that we got new response in case it's waiting for it
        synchronized (response_table) {
            response_table.put(rid, new Response(ioTransport, rid));

            response_table.notifyAll();
            Log.d(TAG, "Notified of incoming response rid=" + rid);
        }
        ;
    }

    void decode_property_change() throws IOException
    {
        Log.d(TAG, "->Property change - decode_property_change starting...");
        try {
            myEdp.AddPropertyChange(new PropertyChange(ioTransport));
        }
        catch (InterruptedException e) {
            throw new IOException("AddPropertyChange interrupted at EventDispatcher");
        }
        Log.d(TAG, "->decode_property_change done.");
    }

    void decode_event() throws IOException
    {
        Log.d(TAG, "->Event - decode_event starting...");
        try {
            myEdp.AddEvent(new Event(ioTransport));
        }
        catch (InterruptedException e) {
            throw new IOException("AddEvent interrupted at EventDispatcher");
        }
        Log.d(TAG, "->decode_event done.");
    }

    void decode_get_response() throws IOException
    {
        // SkypeKit guarantees that get responses always arrive in same order
        // they were requested
        Log.d(TAG, "->Event - decode_get_response starting...");
        try {
            if (!responseWaitInterrupted) {
                // place the response in the queue
                get_prop_response_queue.put(new GetPropertyResponse(ioTransport));
            }
            else {
                new GetPropertyResponse(ioTransport); // discard the response
                responseWaitInterrupted = false;
            }
        }
        catch (InterruptedException e) {
            throw new IOException("Interrupted while get_prop_response_queue.put(..).");
        }
        Log.d(TAG, "->Decoding done, placed GetResponse to queue");
    }

    // This is called by SkypeKit thread
    // NOTE: That sending property request and calling get_prop_response() to get
    // the response MUST be synchronized
    public GetPropertyResponse get_prop_response()
    {
        try {
            Log.d(TAG, "Going to take GetResponse from queue");
            GetPropertyResponse temp = get_prop_response_queue.take();
            if (temp.isMultiresponse()) {
                Log.d(TAG, "Took GetResponse from queue. It is a multiresponse.");
            }
            else {
                Log.d(TAG, "Took GetResponse from queue. Propid=" + temp.getPropId());
            }
            return temp;
        }
        catch (InterruptedException e) {
            responseWaitInterrupted = true;
            Log.d(TAG, "get_prop_response_queue.take() was interrupted! Next property response will be discarded to keep queue in sync.");
            return null;
        }
    }

    // This is called by SkypeKit thread
    public Response get_x_call_response(int rid) throws InterruptedException
    {
        Response myresponse;
        while (!die) {
            myresponse = response_table.remove(rid);
            if (myresponse == null) {
                try {
                    Log.d(TAG, "Response not ready for rid=" + rid + ". Going to wait.");
                    synchronized (response_table) {
                        if (!response_table.containsKey(rid))
                            response_table.wait(XCALL_RESPONSE_TIMEOUT);
                    }
                    Log.d(TAG, "Woken up for rid=" + rid);
                }
                catch (InterruptedException e) {
                    Log.e(TAG, "Waiting for rid=" + rid + " interrupted. e:" + e.toString());
                    throw e;
                }
            }
            else {
                // response has arrived
                Log.d(TAG, "Response has arrived for rid=" + rid + ". Processing...");
                return (myresponse);
            }
        }
        Log.d(TAG, "Exiting GetXCallResponse due to Response Listener being killed.");
        throw new InterruptedException("Response listener killed.");
    }
}
