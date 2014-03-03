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

public class Event extends AbstractTaggedInputMessage {
	private static final String TAG="Event";
	private int module_id;
	private int event_id;
	
	Event(Transport in) throws IOException
	{
		super(in);
		
		module_id = decodeVaruint();
	    event_id  = decodeVaruint();
		Log.d(TAG,"Event decoding started. module_id="+module_id+", event_id="+event_id+".");
		// Decode parms
		try {
			while(decodeNextParm()==true) {};
		} catch (IOException e) {
			Log.e(TAG,"IOException while decoding " + e.getMessage());
			invalid=true;
			//e.printStackTrace();
		}
		Log.d(TAG,"Event decoded. module_id="+module_id+", event_id="+event_id+", validity="+(!invalid));
	}

	public int getEventId()
	{
		return event_id;
	}
	public int getModuleId()
	{
		return module_id;
	}

	private boolean decodeNextParm() throws IOException
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
		Log.d(TAG,"Decoded one parm of event_id="+event_id+", tag="+tag+", kind='"+(char)(kind&0xff)+"'.");
		return true;
	}

}
