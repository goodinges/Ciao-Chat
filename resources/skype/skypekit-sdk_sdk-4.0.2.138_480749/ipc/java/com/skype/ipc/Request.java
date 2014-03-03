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

import java.io.ByteArrayOutputStream;
import java.io.IOException;

import com.skype.util.Log;

public class Request {
  private static final String TAG = "Request";
  ByteArrayOutputStream tokens;
  protected int oid;
  private int listdepth;
  
  public Request() {
	Log.d(TAG,"New request...");
    tokens = new ByteArrayOutputStream(128);
    tokens.write('Z');
    listdepth=0;
  }

  public byte[] send() {
	terminateList();
    tokens.write('z');
    byte[] res = tokens.toByteArray();
    tokens = null;
    return res;
  }

  void encodeVarint(int i) {
	//Log.d(TAG,"encoding Varint "+i);
	int number = i;
    if (i >= 0) number = number << 1;
    else number = (number << 1) ^ (~0);
    encodeVaruint(number);
  }

  void encodeVaruint(int i) {
	//Log.d(TAG,"encoding Varuint "+i);
	int number = i;
    while(true) {
      int towrite = number & 0x7f;
      number = number >> 7;
      if (number == 0) {
        tokens.write(towrite);
        break;
      }
      tokens.write(0x80|towrite);
    }
  }

  /*
    'u' - encodeVaruint
    'e' - encodeVaruint
    'o' - encodeVaruint
   */

  void encodeObjectid(Object o) {
		Log.d(TAG,"next encoded varuint "+o+" is objectid");
		encodeVaruint(o.hashCode());
  }

  /*
    'O' - encodeObjectid
   */

  void encodeString(String s)  {
	//Log.d(TAG,"encoding string '"+s+"'");
	byte[] bytes = s.getBytes();
    encodeVaruint(bytes.length);
    try {
      if (bytes.length > 0)
        tokens.write(bytes);
    } catch (IOException e) {
      e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
    }

    /*
    'S' - encodeString
    'X' - encodeString
    'f' - encodeString
     */
  }

  void encodeBinary(byte[] bytes)  {
    encodeVaruint(bytes.length);
    try {
      if (bytes.length > 0)
        tokens.write(bytes);
    } catch (IOException e) {
      e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
    }

    /*
    'B' - encodeBinary
     */
  }

  public void addParm(int kind, int tag, String val) {
	terminateList();
	//Log.d(TAG,"addParm kind="+kind+",tag="+tag+",(String)val="+val);
	if (kind != 'S' && kind != 'X' && kind != 'f' && kind != 'B')
	{
		Log.e(TAG,"Illegal kind '" + (char)kind + " is not allowed to Strings.");
		//throw new IllegalArgumentException("Kind '" + (char)kind + " is not allowed to Strings");
	}
    tokens.write(kind);
    if (tag == 0) oid = val.hashCode();
    encodeVaruint(tag);
    encodeString(val);
  }

  public void addParm(int kind, int tag, byte[] val) {
		terminateList();
		if(kind!='B')
		{
			Log.e(TAG,"Illegal kind '" + (char)kind + " is not allowed to bytearrays.");
			//throw new IllegalArgumentException("Kind '" + (char)kind + " is not allowed to Strings");
		}
		tokens.write(kind);
	    encodeVaruint(tag);
	    encodeBinary(val);
  }
  
  public void addParm(int kind, int tag, Number val) {
	terminateList();
	//Log.d(TAG,"addParm kind="+kind+",tag="+tag+",(Number)val="+val);
	if (kind == 'u' || kind == 'e' || kind == 'o' || kind == 'O')
	{
	    tokens.write(kind);
	    if (tag == 0) oid = val.hashCode();
	    encodeVaruint(tag);
	    encodeVaruint(val.intValue());
	}
	else if (kind == 'i')
	{
	    tokens.write(kind);
	    if (tag == 0) oid = val.hashCode();
	    encodeVaruint(tag);
	    encodeVarint(val.intValue());
	}
	else
	{
		Log.e(TAG,"Illegal kind '" + (char)kind + "' is not allowed to Number. Parameter not added.");
		//throw new IllegalArgumentException("Kind '" + (char)kind + "' is not allowed to Number");
	}
  }

  public void addParm(int kind, int tag, int val) {
	terminateList();
	//Log.d(TAG,"addParm kind="+kind+",tag="+tag+",(int)val="+val);
	addParm(kind, tag, new Integer(val));
  }

  public void addParm(int kind, int tag, boolean val) {
	terminateList();
	//Log.d(TAG,"addParm kind="+kind+",tag="+tag+",(boolean)val="+val);
    if (kind != 'b')
    	{
    		Log.e(TAG,"Illegal kind '" + (char)kind + " is not allowed to booleans.");
    		//throw new IllegalArgumentException("Kind '" + (char)kind + " is not allowed to booleans");
    	}
    tokens.write((val)?'T':'F');
    tokens.write(tag);
  }

  // LIST FUNCTIONS

  public void addListStart(int tag)
  {
      tokens.write('[');
      encodeVaruint(tag);
      listdepth++;
      if(listdepth>1)
      {
    	  Log.e(TAG,"Nested lists are not supported!!");
      }
  }

  private void terminateList()
  {
	  if(listdepth>0)
	  {
	      tokens.write(']');
	      listdepth--;
	  }
  }
  
  public void addParm(int kind, String val) {
	//terminateList();
	//Log.d(TAG,"addParm kind="+kind+",tag="+tag+",(String)val="+val);
	if (kind != 'S' && kind != 'X' && kind != 'f' && kind != 'B')
      throw new IllegalArgumentException("Kind '" + (char)kind + " is not allowed to Strings");
    tokens.write(kind);
    encodeString(val);
  }

  public void addParm(int kind, byte[] val) {
		//terminateList();
		if(kind!='B')
			throw new IllegalArgumentException("Kind '" + (char)kind + " is not allowed to Strings");
		tokens.write(kind);
	    String temps=new String(val);
	    encodeString(temps);
  }
  
  public void addParm(int kind, Number val) {
	//terminateList();
	//Log.d(TAG,"addParm kind="+kind+",tag="+tag+",(Number)val="+val);
	if (kind == 'u' || kind == 'e' || kind == 'o' || kind == 'O')
	{
	    tokens.write(kind);
	    encodeVaruint(val.intValue());		
	}
	else if (kind == 'i')
	{
	    tokens.write(kind);
	    encodeVarint(val.intValue());		
	}
	else
	{
      throw new IllegalArgumentException("Kind '" + (char)kind + " is not allowed to Number");
	}
  }

  public void addParm(int kind, int val) {
	//terminateList();
	//Log.d(TAG,"addParm kind="+kind+",tag="+tag+",(int)val="+val);
	addParm(kind, new Integer(val));
  }

  public void addParm(int kind, boolean val) {
	//terminateList();
	//Log.d(TAG,"addParm kind="+kind+",tag="+tag+",(boolean)val="+val);
    if (kind != 'b') throw new IllegalArgumentException("Kind '" + (char)kind + " is not allowed to booleans");
    tokens.write((val)?'T':'F');
  }
/*  
  public void addParm(int kind, int tag, Object val) {
	//Log.d(TAG,"addParm kind="+kind+",tag="+tag+",(Object)val="+val);
	//Log.d(TAG,val);
    if (val instanceof boolean[] || val instanceof int[] || val instanceof String[] || val instanceof Collection) {
      tokens.write('[');
      encodeVaruint(tag);
      if (val instanceof String[]) Log.d(TAG,"is array");
      if (val instanceof Collection) Log.d(TAG,"is Collection");
      // TODO: arrays support.
      tokens.write(']');
    }
  }
*/
}

	  