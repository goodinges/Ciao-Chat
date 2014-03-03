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

import java.io.IOException;

import com.skype.util.Log;

public class Response extends AbstractTaggedInputMessage
{
	private static final String TAG="Response";
	private int myrid;
	private boolean err_call;
	
    // null response object, used for returning an error condition, like when there is no runtime conneciton
	Response() {
	    super(null);
	    err_call = true;
	}
	
	public Response(Transport in,int rid) throws IOException
	{
		super(in);
		Log.d(TAG,"New response");
		err_call=false;
		myrid=rid;
		// Check for N response
		try {
			if(ioTransport.peek()=='N')
			{
				// ERR_CALL
				ioTransport.read(); // the 'N'
				int c=ioTransport.read();
				while(c==']')
				{
					c=ioTransport.read();
				}
				if(c!='z')
				{
					Log.e(TAG,"Invalid response 'N' with unknown extra byte=0x"+ Integer.toHexString(c) +".");
					invalid=true;
					return;
				}
				else
				{
					err_call=true;
					invalid=true;
					return;
				}
			}
		} catch (IOException e) {
			Log.e(TAG,"IOException while checking response (rid="+myrid+") validity.");
			invalid=true;
			return;
		}
		// Decode parms
		try {
			while(DecodeNextParm()==true) {};
		} catch (IOException e) {
			Log.e(TAG,"IOException while decoding response (rid="+myrid+") validity.");
			invalid=true;
		}
		Log.d(TAG,"Response rid="+myrid+" decoded. Validity="+(!invalid)+", err_call="+err_call);
	}

	public int GetRid()
	{
		return myrid;
	}
	
	public boolean isErrCall()
	{
		return err_call;
	}
	
	private boolean DecodeNextParm() throws IOException
	{
		int kind=ioTransport.read();
		while(kind==']')
		{
			kind=ioTransport.read();
		}
		if(kind=='z')
		{
			return false;
		}
		int tag=ioTransport.read();
		addParm(tag, decodeOneOfKind(kind));
		Log.d(TAG,"Decoded one parm of rid="+myrid+", tag="+tag+", kind='"+(char)(kind&0xff)+"'.");
		return true;		
	}
	
}
