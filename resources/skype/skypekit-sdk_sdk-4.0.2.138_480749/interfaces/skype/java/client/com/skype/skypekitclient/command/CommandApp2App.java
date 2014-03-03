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

package com.skype.skypekitclient.command;

import com.skype.skypekitclient.SkypekitClient;

/**
 *
 */
public class CommandApp2App extends ClientCommandInterface {
//    private static final String TAG = "CommandApp2App";

//    private SkypekitClient skClient;

	protected CommandApp2App(SkypekitClient skypekitClient) {
		super();
//		skClient = skypekitClient;
	}
   
	@Override
	public String getName() { return "App2App"; }

	public void executeHelp()
	{
        System.out.println("\n[APP2APP] - 2\n" +
                "\t2C - create\n" +
                "\t2D - delete\n" +
                "\t2c - connect\n" +
                "\t2d - disconnect\n" +
                "\t2w - write\n" +
                "\t2m - write max bytes\n" +
                "\t2M - write max + 1 bytes\n" +
                "\t2g - datagram\n" +
                "\t2x - max datagram\n" +
                "\t2X - max + 1 datagram\n" +
                "\t2r - read\n" +
                "\t2u - get connectable users\n" +
                "\t2L - get stream list\n"
        );
	}

}
