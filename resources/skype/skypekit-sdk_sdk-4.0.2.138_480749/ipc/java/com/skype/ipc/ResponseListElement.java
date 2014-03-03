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
import java.util.ArrayList;

import com.skype.util.Log;


public class ResponseListElement extends AbstractDecoder
{
	private static final String TAG="ResponseList";
	public static final int INITIAL_LIST_SIZE=16;
	private ArrayList<Object> items;
	private int getindex;
	
	ResponseListElement(Transport in) throws IOException
	{
		super(in);
		getindex=0;
		items=new ArrayList<Object>(INITIAL_LIST_SIZE);
		while(true)
		{
			int kind=ioTransport.read();
			// there are no tags in lists
			Object o=decodeOneOfKind(kind);
			if(o==null)
			{
				Log.d(TAG,"In list: Decoding returned null");
				items.trimToSize();
				return;
			}
			items.add(o);
			Log.d(TAG,"In list: added parm of kind='"+kind+"'");			
		}
	}

	public Object GetNext()
	{
		try
		{
			return items.get(getindex++);
		}
		catch(ArrayIndexOutOfBoundsException e)
		{
			Log.e(TAG,"GetNext() called even though there is no more items in list");
			return null;			
		}
	}

	public boolean HasMore()
	{
		return(getindex<items.size());
	}
	
}
