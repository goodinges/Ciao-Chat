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

import com.skype.api.Account;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandPrivacy extends ClientCommandInterface {
//    private static final String TAG = "CommandPrivacy";

    private SkypekitClient skClient;

	protected CommandPrivacy(SkypekitClient skypekitClient) {
		super();
		skClient = skypekitClient;
	}
    
	@Override
	public String getName() { return "Privacy"; }

	public void executeHelp()
	{
        skClient.out("\n[Privacy] - p\n" +
                "	pa - change avatar policy\n" +
                "	pb - change contact count policy\n" +
                "	pc - change chat policy\n" +
                "	pi - change skypein policy\n" +
                "	po - change skypeout policy\n" +
                "	pz - change timezone policy\n" +
                "	pv - change voicemail policy\n" +
                "	pw - change web presence policy\n"
        );
	}

	public void execute_a() // change avatar policy
	{
        if (skClient.account != null)
        	skClient.account.SetServersideIntProperty(
        			Account.PROPERTY.avatar_policy.getId(), 
        			Ask.ask_yesno("every one can see") ? Account.AVATARPOLICY.EVERYONE_CAN_SEE.getId() : Account.AVATARPOLICY.BUDDIES_OR_AUTHORIZED_CAN_SEE.getId() );
	}

	public void execute_b() // change contact count policy
	{
        if (skClient.account != null) {
	        String[] possibilities = { "authorized", "noone" };
	        int values[] = { Account.BUDDYCOUNTPOLICY.DISCLOSE_TO_AUTHORIZED.getId(), Account.BUDDYCOUNTPOLICY.DISCLOSE_TO_NOONE.getId()};
        	int value = Ask.ask_list("show contact count to", possibilities);
        	if (( value > 0) && (value <= values.length))
        		skClient.account.SetServersideIntProperty(Account.PROPERTY.buddycount_policy.getId(), values[value]);
        }
	}

	public void execute_c() // change chat policy
	{
        if (skClient.account != null) {
        	skClient.account.SetServersideIntProperty(
        			Account.PROPERTY.chat_policy.getId(), 
        			Ask.ask_yesno("every one can add") ? 
        					Account.CHATPOLICY.EVERYONE_CAN_ADD.getId() : 
        					Account.CHATPOLICY.BUDDIES_OR_AUTHORIZED_CAN_ADD.getId());
        }
	}

	public void execute_i() // change skypein policy
	{
        if (skClient.account != null) {
	        String[] possibilities = { "all", "disclosed", "buddies" };
	        int values[] = { Account.PSTNCALLPOLICY.ALL_NUMBERS_CAN_CALL.getId(), Account.PSTNCALLPOLICY.DISCLOSED_NUMBERS_CAN_CALL.getId(), Account.PSTNCALLPOLICY.BUDDY_NUMBERS_CAN_CALL.getId()};
        	int value = Ask.ask_list("show contact count to", possibilities);
        	if (( value > 0) && (value <= values.length))
        		skClient.account.SetServersideIntProperty(Account.PROPERTY.pstn_call_policy.getId(), values[value]);
        }
	}

	public void execute_o() // change skypeout policy
	{
        if (skClient.account != null) {
	        String[] possibilities = { "buddies", "everyone" };
	        int values[] = { Account.PHONENUMBERSPOLICY.PHONENUMBERS_VISIBLE_FOR_BUDDIES.getId(), Account.PHONENUMBERSPOLICY.PHONENUMBERS_VISIBLE_FOR_EVERYONE.getId()};
        	int value = Ask.ask_list("show number to", possibilities);
        	if (( value > 0) && (value <= values.length))
        		skClient.account.SetServersideIntProperty(Account.PROPERTY.phonenumbers_policy.getId(), values[value]);
        }
	}

	public void execute_z() // change timezone policy
	{
        if (skClient.account != null) {
	        String[] possibilities = { "auto", "manual", "undisclosed", "" };
	        int values[] = { Account.TIMEZONEPOLICY.TZ_AUTOMATIC.getId(), Account.TIMEZONEPOLICY.TZ_MANUAL.getId(), Account.TIMEZONEPOLICY.TZ_UNDISCLOSED.getId()};
        	int value = Ask.ask_list("time zone policy", possibilities);
        	if (( value > 0) && (value <= values.length))
        		skClient.account.SetServersideIntProperty(Account.PROPERTY.timezone_policy.getId(), values[value]);
        }
	}

	public void execute_v() // change voicemail policy
	{
        if (skClient.account != null) {
            skClient.account.SetServersideIntProperty(Account.PROPERTY.voicemail_policy.getId(),
            		Ask.ask_yesno("enable voicemail") ? 
            				Account.VOICEMAILPOLICY.VOICEMAIL_DISABLED.getId() : 
            				Account.VOICEMAILPOLICY.VOICEMAIL_ENABLED.getId());
        }
	}

	public void execute_w() // change web presence policy
	{
        if (skClient.account != null) {
        	skClient.account.SetServersideIntProperty(Account.PROPERTY.webpresence_policy.getId(),
        			Ask.ask_yesno("enable web presence") ? 
        					Account.WEBPRESENCEPOLICY.WEBPRESENCE_DISABLED.getId() : 
        					Account.WEBPRESENCEPOLICY.WEBPRESENCE_ENABLED.getId());
        }
	}

}
