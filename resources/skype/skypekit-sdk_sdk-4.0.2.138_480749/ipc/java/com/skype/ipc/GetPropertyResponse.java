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
import java.util.HashMap;
import java.util.Collection;

import com.skype.util.Log;

public class GetPropertyResponse extends AbstractDecoder
{
	private static final String TAG="GetResponse";

	private static final class NotFound {}
	private class PropertyResponseKey
	{
		public int oid;
		public int propid;
		public PropertyResponseKey() {};
		public PropertyResponseKey(int inoid,int inpropid)
		{
			oid=inoid;
			propid=inpropid;
		}
		public int Key()
		{
			return (oid*0x10000)+propid;
		}
	}
	private HashMap<Integer,Object> props; // if the Object instance of NotFound then property message was valid but value was not found
	private Object single_prop;
	
	private int moid;
	private boolean invalid;
	private int single_oid;
	private int single_propid;
	private boolean multiresponse;
	
	// null response object, used for returning an error condition, like when there is no runtime conneciton
	public GetPropertyResponse()
    {
	    super(null);
	    invalid = true;
	    multiresponse = false;
    }
	
	GetPropertyResponse(Transport in) throws IOException
	{
		// G propid modid oid*;
		// g (moid (oid (N tag | kind tag value)*)*)* z
	
		super(in);
		invalid=false;
		moid=-1;
		multiresponse=false;
		PropertyResponseKey currentkey;
		try {
			do
			{
				if(moid!=-1 && multiresponse==false)
				{
					//this is a multiresponse
					multiresponse=true;
					props=new HashMap<Integer,Object>();
					PropertyResponseKey k=new PropertyResponseKey(single_oid,single_propid);
					props.put(k.Key(), single_prop);
				}
				int temp=decodeVaruint();
				if(moid!=temp && moid!=-1)
				{
					throw new IOException("Multiple moids in single GetPropertyResponse are not supported! (previous="+moid+", new="+temp+").");
				}
				moid=temp;
				currentkey=new PropertyResponseKey();
				currentkey.oid=decodeVaruint();
			}
			while(DecodeNextParm(currentkey)==true);
		} catch (IOException e) {
			Log.e(TAG,"IOException while decoding property response="+e.getMessage());
			invalid=true;
		}
		if(multiresponse)
		{
			Log.d(TAG,"Property multiresponse decoded. module_id="+moid+", validity="+(!invalid)+", numelements="+props.size()+".");
		}
		else
		{
			Log.d(TAG,"Property response decoded. module_id="+moid+", validity="+(!invalid)+".");			
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ACCESSORIES FOR GETPROPERTY RESPONSES
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	public int getPropId()
	{
		if(isMultiresponse())
		{
			Log.e(TAG,"getPropId() called for a multiresponse. Returnvalue will be nonsense.");
			return 0;
		}
		return single_propid;
	}
	public int getOid()
	{
		if(isMultiresponse())
		{
			Log.e(TAG,"getOid() called for a multiresponse. Returnvalue will be nonsense.");
			return 0;
		}
		return single_oid;
	}
	public String GetAsString()
	{
		Object retval=validateAndGetSingleParm();
		if(retval==null)
		{
			Log.d(TAG,"Missing String value. Defaulting to \"\".");
			return new String("");
		}
		if(retval instanceof NotFound)
		{
			Log.d(TAG,"String value not found 'N'.");
			return null;
		}
		return (String)retval;
	}
	public byte[] GetAsBinary()
	{
		
		Object retval=validateAndGetSingleParm();
		if(retval==null || retval instanceof NotFound)
		{
			Log.d(TAG,"Missing or not found binary value. Defaulting to null.");
			return null;
		}
		return (byte[])retval;
	}
	public Integer GetAsInt()
	{
		Object retval=validateAndGetSingleParm();
		if(retval==null)
		{
			// default value of an omitted boolean value
			Log.d(TAG,"Missing integer value. Defaulting to 0.");
			return 0;
		}
		if(retval instanceof NotFound)
		{
			Log.d(TAG,"Integer value not found 'N'.");
			return null;
		}
		return (Integer)retval;
	}
	public Boolean GetAsBoolean()
	{
		Object retval=validateAndGetSingleParm();
		if(retval==null)
		{
			// default value of an omitted boolean value
			Log.d(TAG,"Missing boolean value. Defaulting to false.");
			return false;
		}
		if(retval instanceof NotFound)
		{
			Log.d(TAG,"Boolean value not found 'N'.");
			return null;
		}
		return (((Number)retval).intValue()!=0);
	}
	// returns true, if there is more occurrences of the last piece of data read. I.e. if GetAsInt
	public boolean HasMore()
	{
		if(single_prop!=null)
		{
			if(single_prop instanceof ResponseListElement)
			{
				return ((ResponseListElement)single_prop).HasMore();
			}
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ACCESSORIES FOR MULTIGETPROPERTY RESPONSES
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	public String GetAsString(int oid,int propid)
	{
		Object retval=validateAndGetMultiParm(oid,propid);
		if(retval==null)
		{
			Log.d(TAG,"Missing String value for oid="+oid+", propid="+propid+". Defaulting to \"\".");
			return new String("");
		}
		if(retval instanceof NotFound)
		{
			Log.d(TAG,"String value not found 'N'.");
			return null;
		}
		return (String)retval;
	}
	public byte[] GetAsBinary(int oid,int propid)
	{
		Object retval=validateAndGetMultiParm(oid,propid);
		if(retval==null || retval instanceof NotFound)
		{
			Log.d(TAG,"Missing binary value for oid="+oid+", propid="+propid+". Defaulting to null.");
			return null;
		}
		return (byte[])retval;
	}
	public Integer GetAsInt(int oid,int propid)
	{
		Object retval=validateAndGetMultiParm(oid,propid);
		if(retval==null)
		{
			Log.d(TAG,"Missing integer value for oid="+oid+", propid="+propid+". Defaulting to 0.");
			return 0;
		}
		if(retval instanceof NotFound)
		{
			Log.d(TAG,"Integer value not found 'N'.");
			return null;
		}
		return (Integer)retval;
	}
	
	public Boolean GetAsBoolean(int oid,int propid)
	{
		Object retval=validateAndGetMultiParm(oid,propid);
		if(retval==null)
		{
			Log.d(TAG,"Missing boolean value for oid="+oid+", propid="+propid+". Defaulting to false.");
			return false;
		}
		if(retval instanceof NotFound)
		{
			Log.d(TAG,"Boolean value not found 'N'.");
			return null;
		}
		return (((Number)retval).intValue()!=0);
	}

	// returns true, if there is more occurrences of the last piece of data read. I.e. if GetAsInt
	public boolean HasMore(int oid,int propid)
	{
		PropertyResponseKey k=new PropertyResponseKey(oid,propid);
		Object o=props.get(k);
		if(o!=null)
		{
			if(o instanceof ResponseListElement)
			{
				return ((ResponseListElement)o).HasMore();
			}
		}
		return false;
	}

	/** retrieves HashMap<Integer,Object> of multiresponse values for a given oid*/
	public HashMap<Integer, Object> GetAsMap(int oid, Collection<Integer> propId)
	{
		if(!multiresponse)
		{
			Log.e(TAG,"GetAsMap called for a singleresponse. Returnvalue will be nonsense.");
			return null;
		}	
		
		HashMap<Integer, Object> map = new HashMap<Integer, Object>();
		for (Integer id : propId) { 
			Object retval=validateAndGetMultiParm(oid, id);
			if(retval==null)
			{				
				Log.d(TAG,"Missing value for oid="+oid+", propid="+id+". Not added.");
			}
			else if(retval instanceof NotFound)
			{
				Log.d(TAG,"NotFound value for oid="+oid+", propid="+id+". Not added.");				
			}
			else
			{
				map.put(id, retval);
			}
		}			
		return map;
	}
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// COMMON ACCESSORIES
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	public boolean isMultiresponse()
	{
		return(multiresponse);
	}

	public boolean isValid()
	{
		return (!invalid);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// PRIVATE
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	private Object validateAndGetSingleParm()
	{
		if(invalid)
		{
			Log.e(TAG,"ValidateAndGetSingleParm called for an invalid property response message. Returnvalue will be nonsense.");
			return null;
		}
		if(multiresponse)
		{
			Log.e(TAG,"validateAndGetSingleParm called for a multiresponse. Returnvalue will be nonsense.");
			return null;
		}
		if(single_prop==null)
		{
			return null;
		}
		if(single_prop instanceof ResponseListElement)
		{
			Object oelement=((ResponseListElement)single_prop).GetNext();
			if(oelement==null)
			{
				Log.e(TAG,"validateAndGetSingleParm() called for ResponseListElement while hasMore="+((ResponseListElement)single_prop).HasMore()+". Returnvalue will be nonsense.");
			}
			return oelement;
		}
		return single_prop;
	}

	private Object validateAndGetMultiParm(int oid,int propid)
	{
		PropertyResponseKey k=new PropertyResponseKey(oid,propid);
		if(invalid)
		{
			Log.e(TAG,"ValidateAndGetMultiParm called for an invalid property response message. Returnvalue will be nonsense.");
			return null;
		}
		if(!multiresponse)
		{
			Log.e(TAG,"validateAndGetMultiParm called for a singleresponse. Returnvalue will be nonsense.");
			return null;
		}
		Object o=props.get(k.Key());
		if(o==null)
		{
			return null;
		}
		if(o instanceof ResponseListElement)
		{
			Object oelement=((ResponseListElement)o).GetNext();
			if(oelement==null)
			{
				Log.e(TAG,"validateAndGetMultiParm() called for ResponseListElement while hasMore="+((ResponseListElement)o).HasMore()+". Returnvalue will be nonsense.");
			}
			return oelement;
		}
		return o;
	}

	private boolean HasNext() throws IOException
	{
		int kind=ioTransport.peek();
		while(kind==']')
		{
			ioTransport.read();
			kind=ioTransport.peek();
		}
		if(kind=='z')
		{
			ioTransport.read();
			return false;
		}
		if(kind==',')
		{
			ioTransport.read();
			return true;
		}
		invalid=true;
		return false;
	}
	
	private boolean DecodeNextParm(PropertyResponseKey key) throws IOException
	{
		int kind=ioTransport.read();
		while(kind==']')
		{
			kind=ioTransport.read();
		}
		if(kind=='N')
		{
			key.propid=decodeVaruint();
			if(multiresponse)
			{
				props.put(key.Key(),new NotFound());
			}
			else
			{
				single_propid=key.propid;
				single_oid=key.oid;
				single_prop=new NotFound();
			}
		}
		else
		{
			key.propid=decodeVaruint();
			if(multiresponse)
			{
				props.put(key.Key(), decodeOneOfKind(kind));
			}
			else
			{
				single_propid=key.propid;
				single_oid=key.oid;
				single_prop=decodeOneOfKind(kind);
			}
		}
		Log.d(TAG,"Decoded propid="+key.propid+", oid="+key.oid+"kind='"+(char)kind+"'.");
		return HasNext();
	}
}
