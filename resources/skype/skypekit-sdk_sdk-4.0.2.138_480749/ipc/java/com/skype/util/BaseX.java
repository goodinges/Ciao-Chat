package com.skype.util;

import java.util.Arrays;


public class BaseX
{

    public BaseX(String s, boolean flag, char c) {
        for (bits = 1; 1 << bits < s.length(); bits++)
            ;
        if (1 << bits != s.length())
            throw new IllegalArgumentException("The size of the encoding alphabet is not a power of 2");
        block = 8 / gcd(8, bits);
        chars = s.toCharArray();
        min = max = - 1;
        if (flag) {
            addMinMax(s);
            values = new int[(max - min) + 1];
            Arrays.fill(values, - 1);
            addChars(s);
        }
        else {
            addMinMax(s.toUpperCase());
            addMinMax(s.toLowerCase());
            values = new int[(max - min) + 1];
            Arrays.fill(values, - 1);
            addChars(s.toUpperCase());
            addChars(s.toLowerCase());
        }
        if (c >= min && c <= max && values[c - min] != - 1) {
            throw new IllegalArgumentException("The padding character appears in the encoding alphabet");
        }
        else {
            pad = c;
            return;
        }
    }

    private void addMinMax(String s)
    {
        for (int i = 0; i < s.length(); i++) {
            int j = s.codePointAt(i);
            if (min == - 1 || min > j)
                min = j;
            if (max == - 1 || max < j)
                max = j;
        }

    }

    private void addChars(String s)
    {
        for (int i = 0; i < s.length(); i++) {
            int j = s.codePointAt(i) - min;
            if (values[j] != - 1 && values[j] != i)
                throw new IllegalArgumentException("Duplicate characters in the encoding alphapbet");
            values[j] = i;
        }

    }

    public StringBuffer encode(byte abyte0[], String s, int i)
    {
        if (s == null) {
            i = 0;
        }
        else {
            for (int j = 0; j < s.length(); j++) {
                int l = s.codePointAt(j);
                if (l >= min && l <= max && values[l - min] != - 1)
                    throw new IllegalArgumentException("The separator contains characters from the encoding alphabet");
            }

        }
        int k = ((8 * abyte0.length + bits) - 1) / bits;
        k = (((k + block) - 1) / block) * block;
        if (i > 0)
            k += ((k - 1) / i) * s.length();
        StringBuffer stringbuffer = new StringBuffer(k);
        int i1 = 0;
        int j1 = 0;
        int k1 = 0;
        int l1 = 0;
        int i2 = (1 << bits) - 1;
        for (; bits * i1 < 8 * abyte0.length; i1++) {
            if (i > 0 && i1 > 0 && i1 % i == 0)
                stringbuffer.append(s);
            for (; l1 < bits; l1 += 8) {
                byte byte0 = j1 >= abyte0.length ? 0 : abyte0[j1];
                j1++;
                k1 = k1 << 8 | byte0 & 0xff;
            }

            stringbuffer.append(chars[k1 >>> l1 - bits & i2]);
            l1 -= bits;
        }

        for (; i1 % block != 0; i1++) {
            if (i > 0 && i1 > 0 && i1 % i == 0)
                stringbuffer.append(s);
            stringbuffer.append(pad);
        }

        return stringbuffer;
    }

    public byte[] decode(String s)
    {
        byte abyte0[] = new byte[(s.length() * bits) / 8];
        int i = 0;
        int j = 0;
        int k = 0;
        int l = 0;
        do {
            if (j >= s.length())
                break;
            int i1 = s.codePointAt(j);
            j++;
            if (i1 >= min && i1 <= max) {
                i1 = values[i1 - min];
                if (i1 != - 1) {
                    k = k << bits | i1;
                    l += bits;
                    while (l >= 8) {
                        abyte0[i] = (byte)(k >>> l - 8 & 0xff);
                        l -= 8;
                        i++;
                    }
                }
            }
        }
        while (true);
        if (i < abyte0.length) {
            byte abyte1[] = abyte0;
            abyte0 = new byte[i];
            System.arraycopy(abyte1, 0, abyte0, 0, i);
        }
        return abyte0;
    }

    private static int gcd(int i, int j)
    {
        i = Math.abs(i);
        j = Math.abs(j);
        int k;
        for (; i > 0; i = k) {
            k = j % i;
            j = i;
        }

        return j;
    }

    private char    chars[];
    private int  values[];
    private int  min;
    private int  max;
    private int  bits;
    private int  block;
    private char    pad;
}
