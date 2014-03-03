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

public abstract class AbstractDecoder
{
    private static final String TAG        = "AbstractDecoder";
    public static final int     NULL_VALUE = -1;
    protected Transport         ioTransport;

    AbstractDecoder(Transport in) // ,EventCallback.eType type)
    {
        ioTransport = in;
    }

    protected Object decodeOneOfKind(int kind) throws IOException
    {
        switch (kind) {
        case 'z':
            // no more parms
            Log.d(TAG, "No more params");
            return null;
        case 'S':
        case 'X':
        case 'f':
            // decode string
            return decodeString();
        case 'B':
            // decode binary
            return decodeBinary();
        case '[':
            // decode list
            Log.d(TAG, "Parm is list. Starting list decoding...");
            return decodeList();
        case ']':
            // end decoding list
            Log.d(TAG, "Ending list decoding");
            return null;
        case 'O':
            // decode object
            return decodeObjectid();
        case 'u':
        case 'e':
        case 'o':
            // decode varuint
            return decodeVaruint();
        case 'i':
            // decode varint
            return decodeVarint();
        case 'T':
            // decode boolean true
            return 1;
        case 'F':
            // decode boolean false
            return 0;
        default:
            Log.e(TAG, "Unknown response parm " + kind + " '" + (char) kind + "'");
            throw new IOException();
        }
        // return null;

    }

    protected String decodeString() throws IOException
    {
        int length = decodeVaruint();
        if (length > 0) {
            byte[] bytes = new byte[length];
            ioTransport.read(length, bytes, true);
            String s = new String(bytes);
            return s;
        }
        return "";
    }

    protected byte[] decodeBinary() throws IOException
    {
        int length = decodeVaruint();
        if (length > 0) {
            byte[] bytes = new byte[length];
            ioTransport.read(length, bytes, true);
            return bytes;
        }
        return new byte[0];
    }

    protected int decodeVarint() throws IOException
    {

        int number = decodeVaruint();
        if ((1 & number) != 0) {
            // negative
            number = (number ^ (~0)) >> 1;
        }
        else {
            // positive
            number = (number >> 1);
        }

        return number;
    }

    protected int decodeVaruint() throws IOException
    {
        return (decodeOneVaruint(ioTransport));
    }

    protected static int decodeOneVaruint(Transport i) throws IOException
    {
        Log.d(TAG,"Going to decode Varuint");
        int shift = 0;
        int result = 0;
        while (true) {
            int value = i.read() & 0xFF;
            result = result | ((value & 0x7f) << shift);
            shift = shift + 7;
            Log.d(TAG, "processing byte " + value + ", result=" + result + ",shift=" + shift);
            if ((value & 0x80) == 0)
                break;
        }
        return result;
    }

    protected int decodeObjectid() throws IOException
    {
        return (decodeVaruint());
    }

    protected boolean decodeBoolean() throws IOException
    {
        int v = ioTransport.read();
        if (v == 'T') {
            return true;
        }
        else if (v == 'F') {
            return false;
        }
        else {
            Log.e(TAG, "Invalid boolean value " + v + "'" + (char) v + "'");
            throw new IOException();
        }
    }

    protected ResponseListElement decodeList() throws IOException
    {
        ResponseListElement rlist = new ResponseListElement(ioTransport);
        return rlist;
    }
}
