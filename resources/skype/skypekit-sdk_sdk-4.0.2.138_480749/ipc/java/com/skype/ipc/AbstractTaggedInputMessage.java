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

import java.util.HashMap;

import com.skype.util.Log;

public abstract class AbstractTaggedInputMessage extends AbstractDecoder
{
    private static final String      TAG = "AbstractTaggedInputMessage";
    private HashMap<Integer, Object> parms;
    protected boolean                invalid;

    // CTOR
    public AbstractTaggedInputMessage(Transport in)
    {
        super(in);
        invalid = false;
        parms = new HashMap<Integer, Object>();
    }

    // PROTECTED
    protected Object validateAndGetParm(int tag)
    {
        if (invalid) {
            Log.e(TAG, "validateAndGetParm called for invalid message. Returnvalue will be nonsense.");
            return null;
        }
        Object o = parms.get(tag);
        if (o == null) {
            return null;
        }
        if (o instanceof ResponseListElement) {
            Object oelement = ((ResponseListElement) o).GetNext();
            if (oelement == null) {
                Log.e(TAG, "validateAndGetParm(" + tag + ") called for ResponseListElement while hasMore="
                        + ((ResponseListElement) o).HasMore() + ". Returnvalue will be nonsense.");
            }
            return oelement;
        }
        return o;
    }

    protected void addParm(int key, Object o)
    {
        parms.put(key, o);
    }

    // PUBLIC

    // use this to check whether this Response can be used and read, or not
    public boolean isValid()
    {
        return (!invalid);
    }

    public String GetAsString(int tag)
    {
        Object retval = validateAndGetParm(tag);
        if (retval == null) {
            // default value of an omitted String value
            Log.d(TAG, "Missing string value at tag=" + tag + ". Defaulting to \"\".");
            return new String("");
        }
        return (String) retval;
    }

    public byte[] GetAsBinary(int tag)
    {
        Object retval = validateAndGetParm(tag);
        if (retval == null) {
            // default value of an omitted binary[] value
            Log.d(TAG, "Missing binary value at tag=" + tag + ". Defaulting to null.");
        }
        return (byte[]) retval;
    }

    public int GetAsInt(int tag)
    {
        Object retval = validateAndGetParm(tag);
        if (retval == null) {
            // default value of an omitted integer value
            Log.d(TAG, "Missing integer value at tag=" + tag + ". Defaulting to 0.");
            return 0;
        }
        return ((Number) retval).intValue();
    }

    public boolean GetAsBoolean(int tag)
    {
        Object retval = validateAndGetParm(tag);
        if (retval == null) {
            // default value of an omitted boolean value
            Log.d(TAG, "Missing boolean value at tag=" + tag + ". Defaulting to false.");
            return false;
        }
        return (((Number) retval).intValue() != 0);
    }

    // returns oid of particular tagged list
    public int GetOid(int tag)
    {
        Object retval = validateAndGetParm(tag);
        if (retval == null) {
            // default value of an omitted integer value
            Log.d(TAG, "Missing oid value at tag=" + tag + ". Defaulting to NULL_VALUE.");
            return NULL_VALUE;
        }
        return ((Number) retval).intValue();
    }

    // returns true, if there is more occurrences of the last piece of data
    // read. I.e. if GetAsInt
    public boolean HasMore(int tag)
    {
        Object o = parms.get(tag);
        if (o != null) {
            if (o instanceof ResponseListElement) {
                return ((ResponseListElement) o).HasMore();
            }
        }
        return false;
    }

}
