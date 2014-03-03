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
import com.skype.api.Participant;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandConversationChat extends ClientCommandInterface
{
    // private static final String TAG = "CommandConversationChat";

    private SkypekitClient skClient;

    protected CommandConversationChat(SkypekitClient skypekitClient)
    {
        super();
        skClient = skypekitClient;
    }

    @Override
    public String getName()
    {
        return "ConversationChat";
    }

    public void executeHelp()
    {
        skClient.out("\n[CONVERSATION CHAT] - h\n" +
                "\ths - send chat message\n" +
                "\the - edit last chat message"
                );
    }

    public void execute_s() // send chat message
    {
        Conversation conversation = skClient.currentConversation;
        if (conversation != null) {
            conversation.SetMyTextStatusTo(Participant.TEXT_STATUS.WRITING);
            conversation.PostText(Ask.ask("Enter chat message:"), false);
        }
        else
            skClient.out("You are not logged in or there is no selected conversation (use cc)");
    }

    public void execute_e() // edit last chat message
    {
        if (skClient.notLoggedIn()) {
            skClient.out("You are not logged in");
            return;
        }

        if (skClient.currentConversation == null) {
            skClient.out("Receive some message first");
            return;
        }
        GetLastMessagesResult messages = skClient.currentConversation.GetLastMessages(0);
        if (messages.equals(null)) {
            skClient.out("Send a message first");
            return;
        }

        Message m = null;
        int numMessages = messages.unconsumedMessages.length;
        if (numMessages > 0) {
            m = messages.unconsumedMessages[numMessages-1];
            if (m.CanEdit()) {
                skClient.out("\nMessage text is: " + m.GetStrProperty(Message.PROPERTY.body_xml));
                m.Edit(Ask.ask("New text:"), false, false);
                return;
            }
        }
        skClient.error("No editable messages found.");
    }
}
