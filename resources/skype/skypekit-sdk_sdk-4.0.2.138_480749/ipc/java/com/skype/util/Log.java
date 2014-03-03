package com.skype.util;

public class Log
{
    /**
     * enable debug output by setting this property to true
     */
    public enum Severity {
        kNone, kDebug, kError
    }

    public static Severity  level   = Severity.kNone;

    public static void cycleDebugLevel()
    {
        if (level == Severity.kNone) {
            level = Severity.kDebug;
        }
        else if (level == Severity.kDebug) {
            level = Severity.kError;
        }
        else {
            level = Severity.kNone;
        }
    }

    public static String debugLevel()
    {
        if (level == Severity.kDebug)
            return "Debug";
        if (level == Severity.kError)
            return "Error";
        return "None";
    }

    public static int d(String tag, String msg)
    {
        if (isDebug())
            write_log("DEBUG", tag, msg);
        return 0;
    }

    public static int e(String tag, String msg)
    {
        if (isError())
            write_log("ERROR", tag, msg);
        return 0;
    }

    public static int e(String tag, String msg, Throwable e)
    {
        if (isError()) {
            write_log("ERROR", tag, msg + " : " + e.getMessage());
        }
        return 0;
    }

    /**
     * Write hex dump of a byte array, 16 bytes per line, with printable
     * characters displayed in a text block on the right
     *
     * @param buff
     *            - array of bytes to dump
     */
    public static void hexdump(byte[] buff)
    {
        hexdump(buff, buff.length);
    }

    public static void hexdump(byte[] buff, int len)
    {
        if ( ! isDebug())
            return;

        String hex = "", prn = "", ln;
        for (int r = 0; r < len; r += 16) {
            hex = "";
            prn = "";
            int maxcol = Math.min(16, len - r);
            for (int c = 0; c < maxcol; c++) {
                int value = (int)buff[c + r] & 0xFF;
                Character ch = (char)(buff[c + r]);
                if (c % 2 == 0)
                    hex += " ";
                hex += padLeft(Integer.toHexString(value), 2);
                prn += isPrintableChar(ch) ? ch : '.';
            }
            ln = String.format("%-42s - %s", hex, prn);
            write_log("", padLeft(Integer.toHexString(r), 8), ln);
        }
    }

    private static boolean isDebug()
    {
        return level == Severity.kDebug;
    }

    private static boolean isError()
    {
        return level == Severity.kDebug || level == Severity.kError;
    }

    private static void write_log(String lvl, String tag, String msg)
    {
        System.err.println(lvl + ":" + tag + ":" + msg);
    }

    private static String padLeft(String s, int n)
    {
        String padded = "";
        for (int i = 0, im = n - s.length(); i < im; i++)
            padded += '0';
        return padded + s;
    }

    private static boolean isPrintableChar(char c)
    {
        Character.UnicodeBlock block = Character.UnicodeBlock.of(c);
        return ( ! Character.isISOControl(c)) && block != null && block != Character.UnicodeBlock.SPECIALS;
    }
}
