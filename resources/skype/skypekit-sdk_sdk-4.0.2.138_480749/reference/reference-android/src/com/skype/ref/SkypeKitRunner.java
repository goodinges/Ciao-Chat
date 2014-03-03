/**
 * Copyright (C) 2011, Skype Limited
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

package com.skype.ref;

import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.BufferedReader;

import android.content.Context;
import android.util.Log;

public class SkypeKitRunner implements Runnable {
	private static final String TAG = "SkypeKitRunner";

	// public static final String SKYPEKIT_PARAMETERS = " -p 1663 -d skypekit- -f /data/data/com.skype.ref/files/";
	public static final String SKYPEKIT_PARAMETERS_PREFIX_TCPSOCKET = " -P ";
	public static final String SKYPEKIT_PARAMETERS_PREFIX_LOCALSOCKET = " -l SkypeKit";
	public static final String SKYPEKIT_PARAMETERS_NETLOGGING_DISABLED = " -x";
	public static final String SKYPEKIT_PARAMETERS_NO_TLS = " -n";
	public static final String SKYPEKIT_PARAMETERS_NETLOGGING_ENABLED = "";
	public static final String SKYPEKIT_PARAMETERS_POSTFIX_LOGGING = " -d skypekit- -f ";
	public static final String SKYPEKIT_PARAMETERS_POSTFIX_NOT_LOGGING = " -d /data/data/com.skype.ref/files/log -f ";
	public static final String SKYPEKIT_PARAMETERS_AUDIO_IPC_KEY = " -sa ";
	public static final String SKYPEKIT_PARAMETERS_VIDEO_IPC_KEY = " -sv ";
	public static final String SKYPEKIT_PARAMETERS_INCREASE_IPC_THREADS = " -T 3 -t 3 ";
	public static final int SKYPEKIT_DEFAULT_PORT = 1663;

	private Context context;
	private Listener listener;
	private String skypeKitOptions = null;

	private static boolean isAppRunning;
	private Process appProcess = null;
	private boolean stopNotify = false;

	/**
	 * Interface to be implemented by listener.
	 * 
	 * @author Skype
	 * 
	 */
	public interface Listener {
		/**
		 * The SkypeKit process was stopped normally.
		 */
		public void skypeKitStopped();
	}

	protected void finalize() {
		stopApp();
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
		startApp(false,-1,false, "/tmp/", "/tmp/", false, true);
	}

	/**
	 * Try to start the native executable with default mobile parameters.
	 * Logging is enabled.
	 * This function will block the caller thread
	 * until the application process is started in a new thread. Use
	 * {@link #isAppRunning()} to check if the application is actually started
	 * and running when this function returns.
	 * 
	 * @see #isAppRunning()
	 */
	public void startMobileKitWithLogging()
	{
		startApp(true,-1,false, "/tmp/", "/tmp/", false, true);
	}

	/**
	 * Try to start the native executable with default mobile parameters.
	 * Netlogging is enabled.
	 * This function will block the caller thread
	 * until the application process is started in a new thread. Use
	 * {@link #isAppRunning()} to check if the application is actually started
	 * and running when this function returns.
	 * 
	 * @see #isAppRunning()
	 */
	public void startMobileKitWithNetLogging()
	{
		startApp(false,-1,true, "/tmp/", "/tmp/", false, true);
	}


	/**
	 * Try to start the application. This function will block the caller thread
	 * until the application process is started in a new thread. Use
	 * {@link #isAppRunning()} to check if the application is actually started
	 * and running when this function returns. Default options are used.
	 * 
	 * @see #isAppRunning()
	 */
	public void startApp() {
		startApp(false, SKYPEKIT_DEFAULT_PORT, false, "/tmp/", "/tmp/", false, false);
	}

	public void startApp(boolean logging) {
//		startApp(logging, SKYPEKIT_DEFAULT_PORT, false, "/tmp/", "/tmp/", false, false);
	    startApp(false, SKYPEKIT_DEFAULT_PORT, true, "/tmp/", "/tmp/", false, false);
	}

	public void startApp(boolean logging, int port) {
		startApp(logging, port, false, "/tmp/", "/tmp/", false, false);
	}

	public void startApp(boolean logging, int port, boolean netlogging) {
		startApp(logging, port, netlogging, "/tmp/", "/tmp/", false, false);
	}

	public void startApp(boolean logging, int port, boolean netLogging, String audioIpcKey, String videoIpcKey) {
		startApp(logging, port, netLogging, "/tmp/", "/tmp/", false, false);
	}

	public void startApp(boolean logging, int port, boolean netLogging, String audioIpcKey, String videoIpcKey, boolean moreIpcThreads) {
		startApp(logging, port, netLogging, audioIpcKey, videoIpcKey, moreIpcThreads, false);
	}

	public void startApp(boolean logging, int port, boolean netLogging, String audioIpcKey, String videoIpcKey, boolean moreIpcThreads, boolean disableTls) {
		if (isAppRunning()) {
			return;
		}
		String parameterString = new String("");

		if (port == -1) {
			parameterString += SKYPEKIT_PARAMETERS_PREFIX_LOCALSOCKET;
		} else {
			parameterString += SKYPEKIT_PARAMETERS_PREFIX_TCPSOCKET + port;
		}
		if (netLogging) {
			parameterString += SKYPEKIT_PARAMETERS_NETLOGGING_ENABLED;
		} else {
			parameterString += SKYPEKIT_PARAMETERS_NETLOGGING_DISABLED;
		}
		if (disableTls) {
			parameterString += SKYPEKIT_PARAMETERS_NO_TLS;
		}
		if (moreIpcThreads) {
			parameterString += SKYPEKIT_PARAMETERS_INCREASE_IPC_THREADS;
		}

		parameterString += SKYPEKIT_PARAMETERS_AUDIO_IPC_KEY + audioIpcKey;
		parameterString += SKYPEKIT_PARAMETERS_VIDEO_IPC_KEY + videoIpcKey;

		if (logging) {
			parameterString += SKYPEKIT_PARAMETERS_POSTFIX_LOGGING;
		} else {
			parameterString += SKYPEKIT_PARAMETERS_POSTFIX_NOT_LOGGING;
		}
		startApp(parameterString);
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
	private synchronized void startApp(String options) {
		skypeKitOptions = options;
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
	public static boolean isAppRunningStatic() {
		return isAppRunning;
	}

	public boolean isAppRunning() {
		return isAppRunning;
	}

	/**
	 * Interrupt the application.
	 */
	public synchronized void stopApp() {
		if (!isAppRunning()) {
			return;
		}
		// Calling destroy on the Process object to kill skypekit
		appProcess.destroy();
	}

	public void run() {

		Log.d(TAG, "SkypeKitRunner thread started");
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
		} catch (IOException e1) {
		}

		// First retrieve the filename and path where it will be stored
		String filename = "linux-armv6-skypekit-voicepcm-videortp";
		String path = context.getFilesDir().getPath();
		Log.d(TAG, "Checkpoint 1, path='" + path + "', filename='" + filename + "'");

		// Get the full expected path of the file
		File file = new File(path, filename);

		// Skypekit executable should exist by now
		if (file.exists()) {
			try {
				// Now we need to change the attributes to executable
				appProcess = Runtime.getRuntime().exec("chmod 777 " + file.getCanonicalPath());
				appProcess.waitFor();
				if (appProcess.exitValue() != 0) {
					Log.d(TAG, "Checkpoint 2 - could not chmod for '" + file.getCanonicalPath() + "'");
					throw new IOException("Application attributes could not be changed");
				}

				Log.d(TAG, "Checkpoint 3 - Chmod done for '" + file.getCanonicalPath() + "'");

				appProcess = null;
				// Set the HOME environment variable
				String[] env = new String[1];
				env[0] = new String("HOME="	+ context.getFilesDir().getAbsolutePath());
				// And now, we will launch the application
				String skypekitcommand = (file.getCanonicalPath() + skypeKitOptions + context.getFilesDir().getAbsolutePath());
				Log.d(TAG, "Checkpoint 4 - about to launch '" + skypekitcommand + "' with env='" + env[0] + "'");

				appProcess = Runtime.getRuntime().exec(skypekitcommand, env, context.getFilesDir());
				Thread.sleep(1000);
				isAppRunning = true;
				synchronized (this) {
					this.notify();
				}

				Log.d(TAG, "Checkpoint 5 - skypekit launched, mAppProcess=" + appProcess);

				InputStreamReader isr = new InputStreamReader(appProcess.getInputStream());
				BufferedReader br = new BufferedReader(isr, 8 * 1024);
				String line = null;
				Log.d(TAG, "skypekit output:");
				while ((line = br.readLine()) != null) {
					Log.d(TAG, line);
				}
				// This can lead to an interrupted
				int retcode = appProcess.waitFor();
				Log.d(TAG, "skypekit has exited with exit code " + retcode);

				// TODO How do we handle skypekit being killed or crashing?

				appStopped();
				appProcess = null;
			} catch (InterruptedException e) {

				Log.d(TAG, "Checkpoint 6 - mAppProcess.waitFor threw with mIsAppRunning==false");

				// Caused by process.waitFor()
				if (!isAppRunning) {
					synchronized (this) {
						this.notify();
					}
				} else {
					// We were running, so notify the interrupt

					Log.d(TAG, "Checkpoint 7 - mAppProcess.waitFor threw with mIsAppRunning==true");

					appStopped();
				}
			} catch (Exception e) {
				Log.d(TAG, "Checkpoint 8 - exception" + e);
				if (!isAppRunning) {
					Log.d(TAG, "Checkpoint 9");
					synchronized (this) {
						this.notify();
					}
				}
			}

			Log.d(TAG, "Checkpoint 10");

			if (appProcess != null) {
				Log.d(TAG, "Checkpoint 11");
				appProcess = null;
			}
		} else {
			Log.d(TAG, "Checkpoint 12 - file does not exist!!");
			// Notify start failure
			synchronized (this) {
				this.notify();
			}
		}
	}

	private synchronized void appStopped() {
		Log.d(TAG, "Checkpoint 13 - appStopped called");
		isAppRunning = false;
		if (stopNotify) {
			this.notify();
		}
		if (listener != null) {
			listener.skypeKitStopped();
		}
	}
}
