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
import com.skype.api.Conversation.GetLastMessagesResult;
import com.skype.api.Message;
import com.skype.api.Skype;
import com.skype.api.Transfer;
import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandFileTransfer extends ClientCommandInterface {
//    private static final String TAG = "CommandFileTransfer";

    private SkypekitClient skClient;

	protected CommandFileTransfer(SkypekitClient skypekitClient) {
		super();
		skClient = skypekitClient;
	}
    
	@Override
	public String getName() { return "FileTransfer"; }

	public void executeHelp()
	{
        skClient.out("\n[FILE TRANSFER] - c\n" +
                "\tfs - send a file\n" +
                "\tfr - receive a file\n" +
                "\tfc - cancel\n" +
                "\tfp - pause or resume a transfer\n"
        );
	}

	void execute_s() // send a file
	{
        if (skClient.currentConversation != null) {
            GetLastMessagesResult messages;
            messages = skClient.currentConversation.GetLastMessages(0);
            if (messages == null) {
                skClient.out("no transfers are available in current conversation");
                return;
            }

            ArrayList<String> files = new ArrayList<String>();
            String fnms = "", pref = "";
            for (String fn = Ask.ask("filename: "); (fn.length() != 0); fn = Ask.ask("filename: ")) {
                files.add(fn);
                fnms += pref + fn;
                pref = ", ";
            }
            
            String [] fileArray = new String [files.size()];
            files.toArray(fileArray);
            Conversation.PostFilesResult pfResult = skClient.currentConversation.PostFiles(fileArray, fnms);
            if (pfResult.error_code != Skype.TRANSFER_SENDFILE_ERROR.TRANSFER_OPEN_SUCCESS)
            	skClient.error("failed to post " + pfResult.error_file + " (from " + fnms + ") because " + pfResult.error_code);
            else
            	skClient.out("posting " + pfResult.error_file);
        }
	}

	private void pauseresume(Message[] messages)
	{
        for (int m = 0; m < messages.length; m++) {
            Message msg = messages[m];
            if (msg.GetIntProperty(Message.PROPERTY.type) == Message.TYPE.POSTED_FILES.getId()) {
                Transfer[] transfers = msg.GetTransfers();
                if (transfers != null) {
                    for (int i = 0; i < transfers.length; i++) {
                        if (transfers[i].GetIntProperty(Transfer.PROPERTY.status) == Transfer.STATUS.PAUSED.getId()) {
                            transfers[i].Resume();
                        } else {
                            transfers[i].Pause();
                        }
                    }
                }
            }
        }
	}

	private void cancel(Message[] messages)
	{
        for (int m = 0; m < messages.length; m++) {
            Message msg = messages[m];
            if (msg.GetIntProperty(Message.PROPERTY.type) == Message.TYPE.POSTED_FILES.getId()) {
                Transfer[] transfers = msg.GetTransfers();
                if (transfers != null) {
                    for (int i = 0; i < transfers.length; i++) {
                    	transfers[i].Cancel();
                    }
                }
            }
        }
	}

	private void receive(Message[] messages)
	{
        for (int m = 0; m < messages.length; m++) {
            Message msg = messages[m];
            if (msg.GetIntProperty(Message.PROPERTY.type) == Message.TYPE.POSTED_FILES.getId()) {
                Transfer[] transfers = msg.GetTransfers();
                if (transfers != null) {
                    for (int i = 0; i < transfers.length; i++) {
                        Transfer.TYPE type; 
                        type = Transfer.TYPE.get(transfers[i].GetIntProperty(Transfer.PROPERTY.type));
                        Transfer.STATUS tStatus;
                        if (type == Transfer.TYPE.INCOMING && 
                        		(((tStatus = Transfer.STATUS.get(transfers[i].GetIntProperty(Transfer.PROPERTY.status))) == Transfer.STATUS.NEW) ||
                        				(tStatus == Transfer.STATUS.CONNECTING) ||
                        				(tStatus == Transfer.STATUS.WAITING_FOR_ACCEPT))) {
                                if (!transfers[i].Accept(Ask.ask("save incoming file to (path + filename): "))) {
                                        skClient.error("failed to accept tranfer\n");       
                                }   
                        }
                    }
                }
            }
        }
	}

	void execute_p()
	{
        if (skClient.currentConversation != null) {
            GetLastMessagesResult messages;
            messages = skClient.currentConversation.GetLastMessages(0);
            if (messages == null) {
                skClient.out("no transfers are available in current conversation");
                return;
            }

            pauseresume(messages.contextMessages);
            pauseresume(messages.unconsumedMessages);
        }
	}

	void execute_r() // receive
	{
        if (skClient.currentConversation != null) {
            GetLastMessagesResult messages;
            messages = skClient.currentConversation.GetLastMessages(0);
            if (messages == null) {
                skClient.out("no transfers are available in current conversation");
                return;
            }

            receive(messages.contextMessages);
            receive(messages.unconsumedMessages);
        }
	}

	void execute_c() // cancel
	{
        if (skClient.currentConversation != null) {
            GetLastMessagesResult messages;
            messages = skClient.currentConversation.GetLastMessages(0);
            if (messages == null) {
                skClient.out("no transfers are available in current conversation");
                return;
            }

            cancel(messages.contextMessages);
            cancel(messages.unconsumedMessages);
        }
	}

}
