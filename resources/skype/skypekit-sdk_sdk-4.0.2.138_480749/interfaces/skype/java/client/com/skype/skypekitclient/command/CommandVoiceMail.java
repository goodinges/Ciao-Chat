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
import com.skype.api.Conversation.GetLastMessagesResult;
import com.skype.api.Message;
import com.skype.api.Voicemail;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandVoiceMail extends ClientCommandInterface {
//    private static final String TAG = "CommandVoiceMail";

    private SkypekitClient skClient;

	protected CommandVoiceMail(SkypekitClient skypekitClient) {
		super();
		skClient = skypekitClient;
	}
    
	@Override
	public String getName() { return "VoiceMail"; }

	public void executeHelp()
	{
        skClient.out("\n[VOICE MAIL] - V\n" +
                "\tVv - start recording new voicemail\n" +
                "\tVV - end recording and send voicemail\n" +
                "\tVw - cancel voicemail recording/send\n" +
                "\tVp - play voicemails (first use 'cc' to select conversation)\n" +
                "\tVs - stop voicemail playback\n"
        );
	}


	public void execute_v()	// start recording new voicemail
    {
        if (skClient.currentConversation != null) {
        	skClient.currentConversation.StartVoiceMessage();
        }
        else
            skClient.out("You are not logged in or there is no selected conversation (use cc)");

    }

    public void execute_V()	// end recording and send voicemail
    {
        Conversation conversation = skClient.currentConversation;
        if (conversation != null) {
            int vm_id = conversation.GetIntProperty(Conversation.PROPERTY.active_vm_id);
            skClient.out("CommandVoiceMail.execute_V() - active_vm_id = " + vm_id);
            Voicemail vm = skClient.skype.GetVoiceMailFromId(vm_id,skClient.skype);   
            if (vm != null) {
                conversation.PostVoiceMessage(vm, "empty body");
            }
            else 
            {
                skClient.out("Unable to get active vm"); 
            }
        }
        else
            skClient.out("You are not logged in or there is no selected conversation (use cc)");
    }

    public void execute_w()	// cancel voicemail recording/send
    {
        Conversation conversation = skClient.currentConversation;
        if (conversation != null) {
            int vm_id = conversation.GetIntProperty(Conversation.PROPERTY.active_vm_id);
            skClient.out("CommandVoiceMail.execute_w() - active_vm_id = " + vm_id);
            Voicemail vm = skClient.skype.GetVoiceMailFromId(vm_id,skClient.skype);   
            if (vm != null) {
                     vm.Cancel();
            }
            else {
                    skClient.out("Unable to get active vm"); 
            }
        }
        else
            skClient.out("You are not logged in or there is no selected conversation (use cc)");
    }

    public void execute_s()	// stop voicemail playback
    {
        if (skClient.currentConversation == null) {
            skClient.out("You are not logged in or there is no selected conversation (use cc)");
        }
        else {
            int id = Ask.ask_int("select voicemail id : ");

            GetLastMessagesResult messages;
            messages = skClient.currentConversation.GetLastMessages(0);
            if (messages == null) {
                skClient.error("Unable to get messages from selected conversation");
                return;
            }
            Voicemail vm = null;
            for (int i= 0; i < messages.unconsumedMessages.length; i++ ) {
                if ( messages.unconsumedMessages[i].getOid() == id) {
                    vm = messages.unconsumedMessages[i].GetVoiceMessage();
                    break;
                }
            }
            
            if ( vm == null) {
                for (int i= 0; i < messages.contextMessages.length; i++ ) {
                    if ( messages.contextMessages[i].getOid() == id) {
                        vm = messages.contextMessages[i].GetVoiceMessage();
                        break;
                    }
                }
            }
            if (vm != null) {
                vm.StopPlayback();
            }
        }
    }

    public void execute_p()	// play voicemails
    {
        if (skClient.currentConversation != null) {

            GetLastMessagesResult messages;
            messages = skClient.currentConversation.GetLastMessages(0);
            if (messages == null) {
                skClient.error("Unable to get messages from selected conversation");
                return;
            }

            for (int i = 0; i < messages.unconsumedMessages.length; i++) {
            	Message message = messages.unconsumedMessages[i];
                if (message.GetIntProperty(Message.PROPERTY.type) == Message.TYPE.POSTED_VOICE_MESSAGE.getId()) {
                    skClient.out("id " + message.getOid() + 
                    		", from: " + message.GetStrProperty(Message.PROPERTY.author));
                }
            }
            for (int i = 0; i < messages.contextMessages.length; i++) {
            	Message message = messages.contextMessages[i];
            	if (message.GetIntProperty(Message.PROPERTY.type) == Message.TYPE.POSTED_VOICE_MESSAGE.getId()) {
            		skClient.out("id " + message.getOid() + 
            				", from: " + message.GetStrProperty(Message.PROPERTY.author));
            	}
            }

            int id = Ask.ask_int("select voicemail id : ");
            
            Voicemail vm = null;
            for (int i= 0; i < messages.unconsumedMessages.length; i++ ) {
            	if ( messages.unconsumedMessages[i].getOid() == id) {
            		vm = messages.unconsumedMessages[i].GetVoiceMessage();
            		break;
            	}
            }
            
            if ( vm == null) {
	            for (int i= 0; i < messages.contextMessages.length; i++ ) {
	            	if ( messages.contextMessages[i].getOid() == id) {
	            		vm = messages.contextMessages[i].GetVoiceMessage();
	            		break;
	            	}
	            }
            }

            if (vm != null) {
            	skClient.out("Playing");
            	vm.StartPlayback();
            } else {
            	skClient.error("Unable to play voice message");
            }
        }
        else
            skClient.out("You are not logged in or there is no selected conversation (use cc)");

   }
    
}
