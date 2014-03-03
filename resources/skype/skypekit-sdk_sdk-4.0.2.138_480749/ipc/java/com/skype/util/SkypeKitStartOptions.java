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

import android.content.Context;

public class SkypeKitStartOptions
{
	public boolean scoreDumpEnabled;
	public boolean netloggerEnabled;
	public boolean tlsEnabled;
	public boolean moreIpcThreadsEnabled;
	// enable if skypekit exe needs to be provided android system property file descriptor. If unsure set to false.
	public boolean sys_property_support;
	public String scoreDumpPath;
	public String logfileName;
	public String  audioIpcKey;
	public String videoIpcKey; 
	public String dbPath;
	public int port; // when -1 localsocket is used.

	public SkypeKitStartOptions(Context context)
	{
		scoreDumpEnabled = false;
		netloggerEnabled = false;
		tlsEnabled = false;
		moreIpcThreadsEnabled = false;
		sys_property_support = false;
		scoreDumpPath = new String("");
		logfileName = new String("");
		audioIpcKey = new String("/tmp/");
		videoIpcKey = new String("/tmp/");
		dbPath = new String("") + context.getFilesDir().getAbsolutePath();
		port = -1;
	}

	public String getCmdLineArgs()
	{

		String cmdLineArgs = new String("");
		
		if(port == -1)
		{
			cmdLineArgs += " -l SkypeKit ";
		}
		else
		{
			cmdLineArgs += " -P " + port;
		}

		if(!netloggerEnabled)
		{
			cmdLineArgs += " -x ";
		}

		if(!tlsEnabled)
		{
			cmdLineArgs += " -n ";	
		}	
	
		if(moreIpcThreadsEnabled)
		{
			cmdLineArgs += " -T 3 -t 3 "; 
		}		

		cmdLineArgs += " -sa " + audioIpcKey;
		cmdLineArgs += " -sv " + videoIpcKey;
		
		if(logfileName.length() != 0)
		{
			cmdLineArgs += " -d " + logfileName;
		}
		
		if(dbPath.length() != 0)
		{
			cmdLineArgs += " -f " + dbPath;
		}

		return cmdLineArgs;
	}
}

