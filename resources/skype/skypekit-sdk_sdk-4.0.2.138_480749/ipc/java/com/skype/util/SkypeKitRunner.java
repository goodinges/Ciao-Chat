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

package com.skype.util;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.OutputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

import android.content.Context;
import android.content.res.Resources;
import android.util.Log;

public class SkypeKitRunner implements Runnable {
    private static final String TAG = "SkypeKitRunner";

    // "ANDROID_PROPERTY_WORKSPACE"
    // simple string obfuscation
    private static final char[] ENVNAMECOMPONENTS = {
        0x73, 0x28, 0xf4,
        0x5b, 0x59, 0xf6,
        (char) ('A' ^ 0x05 ^ 0xe2), 0x05, 0xe2,
        (char) ('N' ^ 0x55 ^ 0x33), 0x55, 0x33,
        (char) ('D' ^ 0x2f ^ 0x11), 0x2f, 0x11,
        (char) ('R' ^ 0xf6 ^ 0x81), 0xf6, 0x81,
        (char) ('O' ^ 0x68 ^ 0x72), 0x68, 0x72,
        (char) ('I' ^ 0x89 ^ 0xf6), 0x89, 0xf6,
        (char) ('D' ^ 0xd2 ^ 0xf5), 0xd2, 0xf5,
        (char) ('_' ^ 0xbb ^ 0x12), 0xbb, 0x12,
        (char) ('P' ^ 0xb9 ^ 0x67), 0xb9, 0x67,
        (char) ('R' ^ 0x52 ^ 0xb2), 0x52, 0xb2,
        (char) ('O' ^ 0x33 ^ 0xb3), 0x33, 0xb3,
        (char) ('P' ^ 0x13 ^ 0x31), 0x13, 0x31,
        (char) ('E' ^ 0x24 ^ 0x95), 0x24, 0x95,
        (char) ('R' ^ 0x48 ^ 0xa3), 0x48, 0xa3,
        (char) ('T' ^ 0x7f ^ 0xff), 0x7f, 0xff,
        (char) ('Y' ^ 0x77 ^ 0xe9), 0x77, 0xe9,
        (char) ('_' ^ 0x11 ^ 0x15), 0x11, 0x15,
        (char) ('W' ^ 0x02 ^ 0x36), 0x02, 0x36,
        (char) ('O' ^ 0x08 ^ 0x01), 0x08, 0x01,
        (char) ('R' ^ 0xa4 ^ 0x89), 0xa4, 0x89,
        (char) ('K' ^ 0xa3 ^ 0x13), 0xa3, 0x13,
        (char) ('S' ^ 0xca ^ 0x74), 0xca, 0x74,
        (char) ('P' ^ 0xbc ^ 0x19), 0xbc, 0x19,
        (char) ('A' ^ 0x98 ^ 0x20), 0x98, 0x20,
        (char) ('C' ^ 0x54 ^ 0x03), 0x54, 0x03,
        (char) ('E' ^ 0x98 ^ 0x04), 0x98, 0x04,
        0x99, 0x1e, 0x88,
        0x33, 0xe9, 0xee,
        0xc2, 0x10, 0x29,
        0xb5, 0x59, 0xa6
    };
    // spice for md5 calculation of csf file contents.
    private static final byte[] MAITSEAINE = {
        (byte) 0x99, 0x62, 0x48, (byte) 0xda, (byte) 0xa9, (byte) 0xf4, 0x06, (byte) 0xc2
    };

    private Context             context;
    public Listener             listener;
    private SkypeKitStartOptions startOptions = null;
    private static boolean      isAppRunning;
    private Process             appProcess = null;
    private boolean             stopNotify = false;

    /**
     * Interface to be implemented by listener.
     *
     * @author Skype
     *
     */
    public interface Listener
    {
        /**
         * The SkypeKit process was stopped normally.
         */
        public void skypeKitStopped();
    }

    /**
     * Constructor setting the required environment to do the handling. Create
     * an instance or this class and use {@link #startApp()} to have the
     * application launched in a separate thread.
     *
     * @param context
     *            Main application context to load the resource with
     * @param resid
     *            Main application resource to load as application
     * @param listener
     *            Listener implementation to notify
     * @see #startApp()
     */
    public SkypeKitRunner(Context context) {
        this.context = context;
        this.listener = null;
    }

    /**
     * Try to start the native executable with custom mobile parameters.
     * This function will block the caller thread
     * until the application process is started in a new thread. Use
     * {@link #isAppRunning()} to check if the application is actually started
     * and running when this function returns.
     *
     * @see #isAppRunning()
     */
    public void startMobileKitWithOptions(SkypeKitStartOptions startOptions)
    {
        this.startOptions = startOptions;
        startApp();
    }

    /**
     * Try to start the native executable with default mobile parameters.
     * Logging disabled.
     * This function will block the caller thread
     * until the application process is started in a new thread. Use
     * {@link #isAppRunning()} to check if the application is actually started
     * and running when this function returns.
     *
     * @see #isAppRunning()
     */

    public void startMobileKit()
    {
        startOptions = new SkypeKitStartOptions(context);
        startApp();
    }

    /**
     * Try to start the application. This function will block the caller thread
     * until the application process is started in a new thread. Use
     * {@link #isAppRunning()} to check if the application is actually started
     * and running when this function returns.
     *
     * @param options
     *            Provide the options needed
     * @see #isAppRunning()
     */
    private synchronized void startApp()
    {
        if (isAppRunning())
            return;
        // Create a new thread to run the application from.
        Thread runthread = new Thread(this);
        // We start the thread and wait for it to notify us.
        runthread.start();
        try {
            this.wait();
        } catch (InterruptedException e) {
            // Probably not fully started yet, they will find out later
        }
    }

    /**
     * Check if the application is still running
     *
     * @return true if process is created successfully and not interrupted,
     *         false otherwise
     */
    public static boolean isAppRunningStatic()
    {
        return isAppRunning;
    }

    public boolean isAppRunning()
    {
        return isAppRunning;
    }

    /**
     * Interrupt the application and kill it. Use with caution!!
     * This is not part of normal shutdown procedure. (see AAAJN-3486)
     */
    public synchronized void killApp()
    {
        if ( ! isAppRunning()) {
            return;
        }
        // Calling destroy on the Process object to kill skypekit
        appProcess.destroy();
    }

    private String getHex(byte[] ba)
    {
        String s = new String("");
        for (int n = 0; n < ba.length; n++) {
            s += Integer.toHexString(ba[n] & 0xff);
        }
        return s;
    }

    private boolean chmod(File path, String mode)
    {
        Process proc;
        try {
            proc = Runtime.getRuntime().exec("chmod " + mode + " " + path.getCanonicalPath());
        }
        catch (IOException e) {
            return false;
        }

        try {
            proc.waitFor();
            proc.getOutputStream().close();
            proc.getInputStream().close();
            proc.getErrorStream().close();
        }
        catch (InterruptedException e) {
            return false;
        }
        catch (IOException e) {
            return false;
        }

        if (proc.exitValue() != 0) {
            return false;
        }

        return true;
    }

    private void fixPermissions(File filesList[])
    {
        if (filesList != null) {
            for (File file : filesList) {
                // Don't change permissions of skypekit binary:
                if (file.getName().equalsIgnoreCase("skypekit")) {
                    continue;
                }

                if (file.isDirectory()) {
                    chmod(file, "700");
                    fixPermissions(file.listFiles());
                }
                else {
                    chmod(file, "600");
                }
            }
        }
    }

    private void ensureNoExistingSkypeKitProcess()
    {
        try {
            String ls_str;
            Process ls_proc = Runtime.getRuntime().exec("ps | grep skypekit");
            BufferedReader ls_in = new BufferedReader(new InputStreamReader(ls_proc.getInputStream()), 8 * 1024);
            try {
                while ((ls_str = ls_in.readLine()) != null) {
                    if (ls_str.indexOf("skypekit") > 0) {
                        String[] tmp = ls_str.split("\\s+");
                        int pid = Integer.parseInt(tmp[1]);
                        android.os.Process.killProcess(pid);
                    }
                }
            } catch (IOException e) {
            }
            ls_proc.getOutputStream().close();
            ls_proc.getInputStream().close();
            ls_proc.getErrorStream().close();
        } catch (IOException e1) {

        }
    }

    @Override
    public void run()
    {

        Log.d(TAG, "SkypeKitRunner thread started");

        ensureNoExistingSkypeKitProcess();

        // First retrieve the filename and path where it will be stored
        String filename = null;
        try {
            // Get the filename
            // filename = context.getResources().getResourceEntryName(mResourceId);
            filename = "skypekit";
        }
        catch (Resources.NotFoundException e) {
            // Notify start failure
            synchronized (this) {
                this.notify();
            }
        }

        Log.d(TAG, "Checkpoint 1, filename='" + filename + "', overwritefile='csf'");

        // Get the full expected path of the file
        File file = context.getFileStreamPath(filename);
        File csf = context.getFileStreamPath("csf");

        // Check if we got to (re)write skypekit executable
        boolean fileExists;
        boolean exeWritten = false;
        boolean invalidCsf = false;
        byte[] myversionhash = new byte[32];

        // load my version of csf
        int mycsfresource = context.getResources().getIdentifier("csf", "raw", context.getPackageName());
        try
        {
            InputStream csfis = context.getResources().openRawResource(mycsfresource);
            csfis.read(myversionhash, 0, 32);
            csfis.close();
        }
        catch(Exception e)
        {
            Log.e(TAG,"csf resource could not be read!");
            invalidCsf = true;
        }
        // compare my csf to one in file
        if ( ! invalidCsf) {
            if (csf.exists()) {
                if (file.exists()) {
                    // check if version upgrade has occurred
                    try {
                        InputStream is = new BufferedInputStream(context.openFileInput("csf"));
                        byte[] csfversionhash = new byte[32];
                        is.read(csfversionhash, 0, 32);
                        is.close();
                        Log.d(TAG, "csf='" + new String(csfversionhash) + "', miv='" + new String(myversionhash) + "' result="
                              + Arrays.equals(csfversionhash, myversionhash));
                        if (Arrays.equals(csfversionhash, myversionhash)) {
                            // skypekit does not need to be overwritten
                            fileExists = true;
                        }
                        else {
                            // Skype has been upgraded or reinstalled
                            fileExists = false;
                        }
                    }
                    catch (Exception e) {
                        Log.e(TAG, "Exception when deciding whether to overwrite executable. Defaulting to overwrite.");
                        e.printStackTrace();
                        fileExists = false;
                    }
                }
                else {
                    // skypekit file is missing for some reason
                    Log.w(TAG, filename + " file is missing but csf is found.");
                    fileExists = false;
                }
            }
            else {
                if (file.exists()) {
                    // csf file is missing for some reason
                    Log.w(TAG, "csf file is missing but " + filename + " is found.");
                    fileExists = false;
                }
                else {
                    // a fresh installation or files directory wiped
                    fileExists = false;
                }
            }
        }
        else {
            // csf mechanism cannot be used - just overwrite
            fileExists = false;
        }

        // Write executable from resources into file system if needed
        if ( ! fileExists) {

            Log.d(TAG, "Checkpoint 2 - Going to overwrite or rewrite skypekit exe");

            InputStream is = null;
            OutputStream os = null;

            // glue the resources together to build an skypekit executable
            /*
             * int[] skypekitTable; int numresourcechunks=5; skypekitTable=new
             * int[numresourcechunks]; skypekitTable[0]=R.raw.skypekit_a;
             * skypekitTable[1]=R.raw.skypekit_b;
             * skypekitTable[2]=R.raw.skypekit_c;
             * skypekitTable[3]=R.raw.skypekit_d;
             * skypekitTable[4]=R.raw.skypekit_e;
             */

            // load dynamically runtime any number of resources
            // skypekit.exe must be split into max. 1MB resource chunks named
            // "skypekit_*" where *=a..z
            int[] skypekitTable = new int[10]; // Maximum size of skypekit
                                               // executable is set here to
                                               // 10MB.

            // String resourcenameprefix="com.skype.ipc:res.raw/skypekit_";
            String resourceNamePrefix = "skypekit_";
            char resourceNameIterator = 'a';
            int numResourceChunks;
            for (numResourceChunks = 0; numResourceChunks < 10; numResourceChunks++) {
                Log.d(TAG, "Getting resource handle for '" + resourceNamePrefix + resourceNameIterator
                        + "' of type 'raw' from package '" + context.getPackageName() + "'");

                skypekitTable[numResourceChunks] = context.getResources().getIdentifier(
                        resourceNamePrefix + resourceNameIterator, "raw", context.getPackageName());// .raw.skypekit_a;
                if (skypekitTable[numResourceChunks] == 0) {
                    Log.d(TAG, "No handle found.");
                    break;
                }
                Log.d(TAG, "Handle=" + skypekitTable[numResourceChunks]);
                resourceNameIterator += 1;
            }

            try {

                // Now open an output stream to a file
                os = new BufferedOutputStream(context.openFileOutput(filename, Context.MODE_WORLD_READABLE
                        | Context.MODE_WORLD_WRITEABLE));

                int n = 0;

                for (int i = 0; i < numResourceChunks; i++) {
                    // Get the InputStream of the resource
                    is = context.getResources().openRawResource(skypekitTable[i]);
                    // Now copy the InputStream to the OutputStream
                    byte[] b = new byte[1000];
                    int read = 0;
                    while ((read = is.read(b)) > 0) {
                        os.write(b, 0, read);
                        n++;
                    }
                    Log.d(TAG, "skypekit Resource iteration=" + i + " totalsize=" + (n * 1000));
                    // Final flush of the file
                    os.flush();
                }
                os.close();
                os = null;
                is.close();
                is = null;
                // We have the file now
                exeWritten = true;
                fileExists = true;
            }
            catch (Resources.NotFoundException e) {
                Log.e(TAG, "Executable resources missing or not found");
                // Notify that starting finished, failing
                synchronized (this) {
                    this.notify();
                }
            }
            catch (FileNotFoundException e) {
                Log.e(TAG, "FileNotFoundException when creating executable");
                // Notify that starting finished, failing
                synchronized (this) {
                    this.notify();
                }
            }
            catch (IOException e) {
                Log.e(TAG, "IOException when creating executable");
                // Error copying the file, starting will fail later
            }

            try {
                if (os != null) {
                    os.close();
                }
                if (is != null) {
                    is.close();
                }
            }
            catch (IOException e) {
                // Exception in closing, should be closed
            }
        }

        Log.d(TAG, "Checkpoint 3 - Skypekit exe injected or already ok");

        // Check if csf need to be updated
        if (exeWritten && fileExists && ( ! invalidCsf)) {
            try {
                OutputStream os = new BufferedOutputStream
                    (context.openFileOutput("csf", Context.MODE_WORLD_READABLE
                                            | Context.MODE_WORLD_WRITEABLE));
                os.write(myversionhash);
                os.close();
            }
            catch (Exception e) {
                Log.w(TAG, "Could not write new csf. Exe will be rewritten at next startup.");
            }
        }

        // Skypekit executable should exist by now
        if (fileExists) {
            try {
                // Now we need to change the attributes to executable
                appProcess = Runtime.getRuntime().exec("chmod 750 " + file.getCanonicalPath());
                appProcess.waitFor();
                appProcess.getOutputStream().close();
                appProcess.getInputStream().close();
                appProcess.getErrorStream().close();

                if (appProcess.exitValue() != 0) {
                    Log.e(TAG, "Checkpoint 3.5 - could not chmod for '" + file.getCanonicalPath() + "'");
                    throw new IOException("Application attributes could not be changed");
                }

                Log.d(TAG, "Checkpoint 4 - Chmod done for '" + file.getCanonicalPath() + "'");

                appProcess = null;
                // Set the HOME environment variable
                String[] env;
                if(startOptions.sys_property_support)
                    env = new String[4];
                else
                    env = new String[3];

                env[0] = new String("HOME="
                        + context.getFilesDir().getAbsolutePath());

                env[1] = new String("SCOREDUMP_DISABLED=" + (startOptions.scoreDumpEnabled ? "no" : "yes"));
                env[2] = new String("SCOREDUMP_PATH="+startOptions.scoreDumpPath);


                if(startOptions.sys_property_support)
                {
                    String propfileenvprop = "";
                    for(int n = 2 * 3; n < (26 + 2) * 3; n++)
                    {
                        propfileenvprop += (char) (ENVNAMECOMPONENTS[n] ^ ENVNAMECOMPONENTS[n + 1] ^ ENVNAMECOMPONENTS[n + 2]);
                        n += 2;
                    }
                    env[2] = propfileenvprop + "=" + System.getenv(propfileenvprop);
                    // i.e. env[1] = "ANDROID_PROPERTY_WORKSPACE=" + System.getenv("ANDROID_PROPERTY_WORKSPACE");
                }

                // And now, we will launch the application
                //TODO: when adding sdcard support there needs to be getExternalFilesDir() call (and privileges to do so)
                String skypekitcommand = (file.getCanonicalPath() + startOptions.getCmdLineArgs());

                String log_str = new String("about to launch '"+skypekitcommand+"' with ");
                for(int i = 0;i<env.length;i++)
                {
                    log_str += " env"+i+"='"+ env[i]+"'";
                }
                Log.d(TAG,log_str);

                File files[] = new File[1];
                files[0] = context.getFilesDir(); // include the root data folder
                fixPermissions(files);
                appProcess = Runtime.getRuntime().exec(skypekitcommand, env, context.getFilesDir());
                isAppRunning = true;
                synchronized (this) {
                    this.notify();
                }

                Log.d(TAG, "Checkpoint 5 - skypekit launched, appProcess=" + appProcess);

                InputStreamReader isr = new InputStreamReader(appProcess.getInputStream());
                BufferedReader br = new BufferedReader(isr, 8 * 1024);
                String line = null;
                Log.d(TAG, "skypekit output:");
                while ((line = br.readLine()) != null) {
                    Log.d(TAG, line);
                }
                // This can lead to an interrupted
                int retcode = appProcess.waitFor();
                appProcess.getOutputStream().close();
                appProcess.getInputStream().close();
                appProcess.getErrorStream().close();
                Log.d(TAG, "skypekit has exited with exit code " + retcode);

                // TODO: How do we handle skypekit being killed or crashing?

                appStopped();
                appProcess = null;
            }
            catch (InterruptedException e) {

                Log.d(TAG, "Checkpoint 6.5 - appProcess.waitFor threw with isAppRunning==false");

                // Caused by process.waitFor()
                if ( ! isAppRunning) {
                    synchronized (this) {
                        this.notify();
                    }
                }
                else {
                    // We were running, so notify the interrupt

                    Log.d(TAG, "Checkpoint 7 - appProcess.waitFor threw with isAppRunning==true");

                    appStopped();
                }
            }
            catch (Exception e) {
                Log.d(TAG, "Checkpoint 7.5 - exception" + e);
                if ( ! isAppRunning) {
                    Log.d(TAG, "Checkpoint 7.6");
                    synchronized (this) {
                        this.notify();
                    }
                }
            }

            Log.d(TAG, "Checkpoint 7.7");

            if (appProcess != null) {
                Log.d(TAG, "Checkpoint 7.8");
                appProcess = null;
            }
        }
        else {
            Log.d(TAG, "Checkpoint 8 - file does not exist!!");
            // Notify start failure
            synchronized (this) {
                this.notify();
            }
        }
    }

    private synchronized void appStopped()
    {
        Log.d(TAG, "appStopped called");
        isAppRunning = false;
        if (stopNotify) {
            this.notify();
        }
        if (listener != null) {
            listener.skypeKitStopped();
        }
    }
}
