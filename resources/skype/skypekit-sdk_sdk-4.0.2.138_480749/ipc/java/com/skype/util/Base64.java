package com.skype.util;

public abstract class Base64
{

    public static String encode(byte abyte0[])
    {
        if (abyte0 == null)
            return null;
        else
        	return inst.encode(abyte0, null, 0).toString();
    }

    public static byte[] decode(String s)
    {
        if (s == null)
            return null;
        else
        	return inst.decode(s);
    }

    private Base64()
    {
    }

    private static BaseX inst = new BaseX("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", true, '=');
}
