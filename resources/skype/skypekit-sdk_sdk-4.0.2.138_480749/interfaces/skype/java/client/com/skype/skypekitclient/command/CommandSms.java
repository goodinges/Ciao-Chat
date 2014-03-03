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

import java.util.ArrayList;

import com.skype.api.Conversation;
import com.skype.api.Sms;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandSms extends ClientCommandInterface {
//    private static final String TAG = "CommandSms";

    private SkypekitClient skClient;

	protected CommandSms(SkypekitClient skypekitClient) {
		super();
		skClient = skypekitClient;
	}
    
	@Override
	public String getName() { return "Sms"; }

	public void executeHelp()
	{
        skClient.out("\n[SMS] - S\n" +
                "\ts - send\n" +
                "\tp - price\n" +
                "\tc - char count\n" +
                "\tm - message count"
        );
	}

	
    private Sms create_sms()	
    {
            Sms sms = skClient.skype.CreateOutgoingSms();
            if (sms == null) {
                skClient.error("Can not create sms");
                return null;
            }
            Sms.SetSMSBodyResult smsBodyResult = sms.SetBody(Ask.ask("sms body: "));
            if (smsBodyResult == null) {
                skClient.error("can not set body");
                return null;
            }
            skClient.out("body set with result " + smsBodyResult.result.toString() + " and " + smsBodyResult.charsUntilNextChunk + " bytes left in last chunk");
            for (int i = 0; i < smsBodyResult.chunks.length; i++)
            	skClient.out("CHUNK[" + i + "] " + smsBodyResult.chunks[i]);
                    
            return sms;
    }

    public void execute_s() // send
    {        
        String target = Ask.ask("add target number:");
        Conversation conversation = skClient.skype.GetConversationByIdentity(target);
        if (conversation != null) {
            Sms sms = skClient.skype.CreateOutgoingSms();
            boolean result = false;
            
            if (sms == null) {
                skClient.error("can not create sms");
                return;
            }
                                                    
            Sms.SetSMSBodyResult smsBodyResult = sms.SetBody(Ask.ask("sms body: "));
            if (smsBodyResult == null) {
                skClient.error("can not set body");
                return;
            }
                    
            String[] targets = new String[1]; 
            targets[0] = target; 
            result = sms.SetTargets(targets);
            if (!result) {
                skClient.error("can not set target number");
                return;
            }                                  
                           
            conversation.PostSMS(sms, "");
//            if (!conversation.PostSMS(sms, "")) {
//            	skClient.error("can not send sms to participants of " + conversation.GetStrProp(Conversation.P_META_TOPIC));
//            } 
                                   
            skClient.out("\nsms sent");                
        } else {
        	skClient.error("unable to create conversation from identity " + target); 
        }                           
    }

    public void execute_p() // price
    {
        Sms sms = create_sms();
        if (sms == null) return;

        ArrayList<String> targets = new ArrayList<String>();
        String[] stargets = {};
        
        String target = null;
        int numTargets = 0;
        while (true) {
            target = Ask.ask("add phone number: ");
            targets.add(target);
            numTargets++;
            if (!Ask.ask_yesno("Add another?")) break;
        }

        stargets = new String[numTargets];
        for (int i=0;i<numTargets;i++)
        {
            stargets[i] = (String)targets.get(i);
        }

        if (!sms.SetTargets(stargets)) {
            skClient.error("some invalid targets");
            return;
        }

        skClient.out("sms price = " + sms.GetIntProperty(Sms.PROPERTY.price));

        for (int i = 0; i< stargets.length; i++) {
            int p = sms.GetTargetPrice(stargets[i]);
            skClient.out("	" + stargets[i] + " = " + p);
        }
    }

    public void execute_c() // char count
    {
        Sms sms = create_sms();
        if (sms == null) return;

        Sms.GetBodyChunksResult bodyChunksResult = sms.GetBodyChunks();
        int numchar = 0;
        for (int i = 0; i < bodyChunksResult.textChunks.length; i++)
        	numchar += bodyChunksResult.textChunks[i].length();
 
        skClient.out("num char = " + numchar + "(sum of chunks) " + (sms.GetStrProperty(Sms.PROPERTY.body)).length() + " (body string)");
    }

    public void execute_m() // message count
    {
        Sms sms = create_sms();
        if (sms == null) return;
        Sms.GetBodyChunksResult bodyChunksResult = sms.GetBodyChunks();
        for (int i = 0; i < bodyChunksResult.textChunks.length; i++)
        	skClient.out("CHUNK[" + i + "] " + bodyChunksResult.textChunks[i]);
    }

}
