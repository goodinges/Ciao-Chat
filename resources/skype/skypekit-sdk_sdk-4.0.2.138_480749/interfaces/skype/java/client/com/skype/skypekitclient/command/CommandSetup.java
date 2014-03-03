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

import com.skype.api.Conversation;
import com.skype.api.Skype;
import com.skype.api.Video;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandSetup extends ClientCommandInterface {
//    private static final String TAG = "CommandSetup";

    private SkypekitClient skClient;

	protected CommandSetup(SkypekitClient skypekitClient) {
		super();
		skClient = skypekitClient;
	}
    
	@Override
	public String getName() { return "Setup";}
	
	public void executeHelp()
	{
        skClient.out("\n[SETUP] - e\n" +
                "\tef - enable/disable file transfer auto-accept feature\n" +
                "\tea - set idle time (in seconds) for away\n" +
                "\ten - set idle time (in seconds) for not available\n" +
                "\tet - [CALL] set timeout (in seconds) for call forwarding / send to voicemail\n" +
                "\tev - [CALL] send call to voicemail after the timeout\n" +
                "\tec - [CALL] apply call forwarding rules. Overrides 'ev' command\n" +
                "\tes - start sending video automatically when call starts\n" +
                "\ted - enable/disable video functionality\n"
        );
	}
	
	
	public void execute_f()	// enable/disable file transfer auto-accept feature
	{
        int result = skClient.skype.GetInt(Skype.FT_AUTOACCEPT);
        if (0 == result) {
            if (Ask.ask_yesno("Autoaccept is disabled, enable?")) {
            	skClient.skype.SetInt(Skype.FT_AUTOACCEPT, 1);
            	skClient.skype.SetStr(Skype.FT_SAVEPATH, 
            			Ask.ask("full path to save files (used for AutoAccept feature):")); 
            	skClient.out("enabled");       
            }
        } else {
            if (Ask.ask_yesno("Autoaccept is enabled, disable?")) {
            	skClient.skype.SetInt(Skype.FT_AUTOACCEPT, 0);
            	skClient.out("disabled");         
            }
        }
	}

	public void execute_n()	// set idle time (in seconds) for not available
	{
        print_int_val(Skype.IDLE_TIME_FOR_NA, "Current value:");
        skClient.skype.SetInt(Skype.IDLE_TIME_FOR_NA, 
                Ask.ask_int("Idle time (in seconds) after which availability is set to NOT_AVAILABLE:"));
        print_int_val(Skype.IDLE_TIME_FOR_NA, "New idle time for NOT_AVAILABLE:");
	}

	public void execute_a()	// set idle time (in seconds) for away
	{
        print_int_val(Skype.IDLE_TIME_FOR_AWAY, "Current value:");
        skClient.skype.SetInt(Skype.IDLE_TIME_FOR_AWAY, 
                Ask.ask_int("Idle time (in seconds) after which availability is set to AWAY:"));
        print_int_val(Skype.IDLE_TIME_FOR_AWAY, "New idle time for AWAY:");               
	}

	public void execute_t()	// [CALL] set timeout (in seconds) for call forwarding / send to voicemail
	{
        print_int_val(Skype.IDLE_TIME_FOR_NA, "Current value:");
        skClient.skype.SetInt(Skype.IDLE_TIME_FOR_NA, 
                Ask.ask_int("Timeout (in seconds) for call forwarding / send to voicemail:"));
        print_int_val(Skype.IDLE_TIME_FOR_NA, "New call answer timeout:");
	}

	public void execute_v()	// [CALL] send call to voicemail after the timeout
	{
        print_int_val(Conversation.CALL_SEND_TO_VM, "Current value:");
        skClient.skype.SetInt(Conversation.CALL_SEND_TO_VM, 
                Ask.ask_yesno("Send call to voicemail after the timeout?") ? 1 : 0);
        print_int_val(Conversation.CALL_SEND_TO_VM, "Call sent to vm after timeout:");
	}

	public void execute_c()	// [CALL] apply call forwarding rules. Overrides 'ev' command
	{
        print_int_val(Conversation.CALL_APPLY_CF, "Current value:");
        skClient.skype.SetInt(Conversation.CALL_APPLY_CF, 
                Ask.ask_yesno("Apply call forwarding rules?") ? 1 : 0);
        print_int_val(Conversation.CALL_APPLY_CF, "Call sent to cf after timeout:");
	}

	public void execute_s()	// start sending video automatically when call starts
	{
        print_int_val(Video.VIDEO_AUTOSEND, "Current value:");
        skClient.skype.SetInt(Video.VIDEO_AUTOSEND, 
        		Ask.ask_yesno("Start sending video automatically when call starts?") ? 1 : 0);
        print_int_val(Video.VIDEO_AUTOSEND, "Video is started automatically:");
	}

	public void execute_d()	// enable/disable video functionality
	{
        int result = skClient.skype.GetInt(Video.VIDEO_DISABLE);
        if (0 == result) {
        	if (Ask.ask_yesno("Video is enabled, disable?")) {
        		skClient.skype.SetInt(Video.VIDEO_DISABLE, 1);
        		skClient.out("disabled");       
        	}
        } else {
        	if (Ask.ask_yesno("Video is disabled, enable?")) {
        		skClient.skype.SetInt(Video.VIDEO_DISABLE, 0);
        		skClient.out("enabled");         
        	}
        }
	}

	public void print_int_val(String key, String descr)
	{
		skClient.out("\n" + descr + " " + skClient.skype.GetInt(key));
	}

}
