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

public class PropertyChange extends AbstractDecoder
{
    private static final String TAG = "PropertyChange";
    public int                  moid;
    public int                  oid;
    public int                  propid;
    public int                  kind;
    private Object              val;
    private boolean             invalid;

    PropertyChange(Transport in) throws IOException
    {
        super(in);
        // <modid><oid><typetag or N><propid><propvalueaccordingtotype or
        // nothing if N>]]]z
        invalid = false;
        val = null;
        moid = decodeVaruint();
        oid = decodeVaruint();
        kind = ioTransport.read();
        propid = decodeVaruint();

        Log.d(TAG, "Property change moid=" + moid + ",oid=" + oid + ",kind='" + (char) kind + "',propid=" + propid + ".");

        if (kind != 'N') {
            val = decodeOneOfKind(kind);
        }
        
        // check for known future protocol change
        if (ioTransport.peek() == ',') {
            invalid = true;
            throw new IOException("Multiple property changes in single message are not supported (yet)!");
        }
        
        if (ioTransport.read() != ']' ||
                ioTransport.read() != ']' ||
                ioTransport.read() != ']' ||
                ioTransport.read() != 'z')
        {
            Log.e(TAG, "Propertychange with unregognized extra bytes.");
            invalid = true;
        }
        else {
            Log.d(TAG, "Propertychange moid=" + moid + ",oid=" + oid + ",kind='" + (char) kind + "',propid=" + propid + " decoded successfully.");
            return;
        }
    }

    private void CheckValidity() throws IOException
    {
        if (invalid) {
            throw new IOException("Attempt to access malformed PropertyChange.");
        }
        
        if (val == null) {
            Log.e(TAG, "Attempt to read PropertyChange which has no value. Please add check for hasValue().");
        }
    }

    public boolean hasValue()
    {
        return (!invalid && val != null);
    }

    public boolean isValid()
    {
        return (!invalid);
    }

    public int GetKind()
    {
        return kind;
    }

    public int GetOid()
    {
        return oid;
    }

    public int GetPropid()
    {
        return propid;
    }

    public int GetAsInt() throws IOException
    {
        CheckValidity();
        return ((Number) val).intValue();
    }

    public boolean GetAsBoolean() throws IOException
    {
        CheckValidity();
        return (((Number) val).intValue() != 0);
    }

    public String GetAsString() throws IOException
    {
        CheckValidity();
        return (String) val;
    }

    public byte[] GetAsBinary(int tag) throws IOException
    {
        CheckValidity();
        return (byte[]) val;
    }

}
