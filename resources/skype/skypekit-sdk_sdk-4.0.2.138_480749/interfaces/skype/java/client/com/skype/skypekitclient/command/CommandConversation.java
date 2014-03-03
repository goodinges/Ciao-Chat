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

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.HashMap;

import com.skype.api.Conversation;
import com.skype.api.Conversation.GetLastMessagesResult;
import com.skype.api.Conversation.LIST_TYPE;
import com.skype.api.Conversation.PARTICIPANTFILTER;
import com.skype.api.Conversation.PROPERTY;
import com.skype.api.Message;
import com.skype.api.Participant;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandConversation extends ClientCommandInterface
{
    // private static final String TAG = "CommandConversation";

    private SkypekitClient skClient;

    protected CommandConversation(SkypekitClient skypekitClient)
    {
        super();
        skClient = skypekitClient;
    }

    @Override
    public String getName()
    {
        return "Conversation";
    }

    public void executeHelp()
    {
        skClient.out("\n[CONVERSATION] - c\n" +
                "\tcc - select a conversation as current one\n" +
                "\tcx - select a conversation as current one (filtered list)\n" +
                "\tcu - open conversation using skype name\n" +
                "\tct - set topic of current conversation\n" +
                "\tcp - set picture of current conversation\n" +
                "\tcB - bookmark current conversation\n" +
                "\tcb - unbookmark current conversation\n" +
                "\tcg - set guidelines\n" +
                "\tcn - set display name\n" +
                "\tcl - retire from current conversation\n" +
                "\tco - set rank\n" +
                "\tcw - set text status write\n" +
                "\tcW - set text status angry\n" +
                "\tcr - set text status reading\n" +
                "\tca - set alert string\n" +
                "\tcf - find message with text\n" +
                "\tcm - list messages\n" +
                "\tcP - list properties from selected message\n" +
                "\tcX - list participant properties for current coversation\n"
                );
    }

    boolean hasNoConversation()
    {
        if (skClient.currentConversation == null) {
            skClient.error("No conversation selected.");
            return true;
        }
        return false;
    }

    @Override
    public void execute_u() // Open conversation using skype name
    {
        if (skClient.notLoggedIn())
            return;

        String[] names = { Ask.ask("enter skype name: ") };
        if (names.length == 0) {
            skClient.error("No participant entered, unable to get conversation");
            return;
        }

        Conversation conversation = skClient.skype.GetConversationByParticipants(names, true, false);
        if (conversation == null) {
            System.err.println("Error: Unable to get conversation");
            return;
        }

        skClient.currentConversation = conversation;
        Participant[] participants;
        participants = skClient.currentConversation.GetParticipants(PARTICIPANTFILTER.ALL);
        for (int i = 0; i < participants.length; i++)
            participants[i].getOid();
    }

    @Override
    public void execute_c() // Select a conversation as current one
    {
        if (skClient.notLoggedIn())
            return;

        Conversation[] conversations = skClient.skype.GetConversationList(LIST_TYPE.ALL_CONVERSATIONS);
        if (conversations == null) {
            skClient.error("Unable to get conversation list\n");
            return;
        }

        if (conversations.length == 0) {
            skClient.out("0 conversations\n");
            return;
        }

        HashMap<Integer, Conversation> map = new HashMap<Integer, Conversation>();
        for (int i = 0; i < conversations.length; i++) {
            Conversation c = conversations[i];
            int key = c.getOid();
            skClient.out("\t" + key + " " + c.GetStrProperty(PROPERTY.displayname) +
                    " (" + c.GetStrProperty(PROPERTY.identity) + ")");
            map.put(key, c);
        }

        skClient.currentConversation = null;
        int sel = Ask.ask_int("select conversation id: ");
        if (map.containsKey(sel)) {
            skClient.currentConversation = map.get(sel);

            // create participant objects to receive propchanges
            Participant[] participants = skClient.currentConversation.GetParticipants(PARTICIPANTFILTER.ALL);
            for (int i = 0; i < participants.length; i++) {
                participants[i].getOid();
            }
        }
        else {
            skClient.error("unknown conversation id, current conversation unchanged.");
        }
    }

    @Override
    public void execute_x() // Select a conversation as current one (filtered
                            // list)
    {
        if (skClient.notLoggedIn())
            return;

        String labels[] = { "Conversation::ALL_CONVERSATIONS", "Conversation::INBOX_CONVERSATIONS",
                "Conversation::BOOKMARKED_CONVERSATIONS",
                    "Conversation::LIVE_CONVERSATIONS", "" };
        int values[] = { LIST_TYPE.ALL_CONVERSATIONS.getId(), LIST_TYPE.INBOX_CONVERSATIONS.getId(),
                LIST_TYPE.BOOKMARKED_CONVERSATIONS.getId(), LIST_TYPE.LIVE_CONVERSATIONS.getId() };

        int type = values[Ask.ask_list("select filter:", labels)];
        skClient.out("type " + type);
        Conversation[] conversations = skClient.skype.GetConversationList(LIST_TYPE.get(type));
        if (conversations == null) {
            skClient.error("Unable to get conversation list\n");
            return;
        }

        if (conversations.length == 0) {
            skClient.out("0 conversations\n");
            return;
        }

        for (int i = 0; i < conversations.length; i++) {
            skClient.out("\t" + conversations[i].getOid() +
                    " " + conversations[i].GetStrProperty(PROPERTY.displayname) +
                    " (" + conversations[i].GetStrProperty(PROPERTY.identity) + ")");
        }

        skClient.currentConversation = null;
        Conversation conversation = conversations[(Ask.ask_int("select conversation id: "))];
        if (conversation != null) {
            skClient.currentConversation = conversation;
            // create participant objects to receive propchanges
            Participant[] participants = skClient.currentConversation.GetParticipants(PARTICIPANTFILTER.ALL);
            for (int i = 0; i < participants.length; i++) {
                participants[i].getOid();
            }
        }
    }

    @Override
    public void execute_t() // set topic of current conversation
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.SetTopic(Ask.ask("topic"), false);
    }

    @Override
    public void execute_p() // set picture of current conversation
    {
        if (hasNoConversation())
            return;

        String filename = Ask.ask("filename: (jpg image, size less then 50 kb)");
        byte[] pic = null;
        try {
            pic = load_file(filename);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        if (pic != null) {
            if (pic.length >= 50000) {
                skClient.error("unable to set picture! pic size too large: " + pic.length);
                return;
            }
            skClient.currentConversation.SetPicture(pic);
        }
        else {
            skClient.error("can't load picture " + filename);
        }
    }

    public byte[] load_file(String filename) throws Exception
    {
        InputStream in = null;
        byte[] buf = null; // output buffer
        byte[] tmp = null;
        int bufLen = 20000 * 3;
        try {
            in = new BufferedInputStream(new FileInputStream(filename));
            buf = new byte[bufLen];
            int len = 0;
            while ((len = in.read(buf, 0, bufLen)) != -1) {
                tmp = new byte[len];
                System.arraycopy(buf, 0, tmp, 0, len); // still need to do copy
            }
        }
        finally {
            if (in != null)
                try {
                    in.close();
                }
                catch (Exception e) {
                }
        }
        return tmp;
    }

    @Override
    public void execute_B() // bookmark current conversation
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.SetBookmark(true);
    }

    @Override
    public void execute_b() // unbookmark current conversation
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.SetBookmark(false);
    }

    @Override
    public void execute_g() // set guidelines
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.SetGuidelines(Ask.ask("guideline"), false);
    }

    @Override
    public void execute_n() // set display name
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.RenameTo(Ask.ask("new name: "));
    }

    @Override
    public void execute_l() // retire from current conversation
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.RetireFrom();
        skClient.currentConversation = null;
    }

    @Override
    public void execute_o() // set rank
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.SetOption(Conversation.PROPERTY.opt_entry_level_rank.getId(),
                Ask.ask_int("rank: "));
    }

    @Override
    public void execute_w() // set text status to write
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.SetMyTextStatusTo(Participant.TEXT_STATUS.WRITING);
    }

    @Override
    public void execute_W() // set text status to write as angry
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.SetMyTextStatusTo(Participant.TEXT_STATUS.WRITING_AS_ANGRY);
    }

    @Override
    public void execute_r() // set text status to reading
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.SetMyTextStatusTo(Participant.TEXT_STATUS.READING);
    }

    @Override
    public void execute_a() // set alert string
    {
        if (hasNoConversation())
            return;

        skClient.currentConversation.SetAlertString(Ask.ask("alert: "));
    }

    @Override
    public void execute_f() // find message
    {
        if (hasNoConversation())
            return;

        Message msg;
        msg = skClient.currentConversation.FindMessage(Ask.ask("message: "), 0);
        skClient.out("found " +
                    ((msg != null) ? Message.TYPE.get(msg.GetIntProperty(Message.PROPERTY.type)).toString() : "(none)"));
    }

    @Override
    public void execute_m() // list messages
    {
        if (hasNoConversation())
            return;

        skClient.out(
                "UNCONSUMED MESSAGES: SUPPRESSED: " +
                        skClient.currentConversation.GetIntProperty(PROPERTY.unconsumed_suppressed_messages) +
                        "; NORMAL: " +
                        skClient.currentConversation.GetIntProperty(PROPERTY.unconsumed_normal_messages) +
                        "; ELEVATED: " +
                        skClient.currentConversation.GetIntProperty(PROPERTY.unconsumed_elevated_messages) +
                        "; VOICE: " +
                        ((skClient.currentConversation.GetIntProperty(PROPERTY.unconsumed_messages_voice) != 0) ? "there are some"
                                : "none"));

        GetLastMessagesResult messages;
        messages = skClient.currentConversation.GetLastMessages(0);
        if (messages == null) {
            skClient.error("Unable to get messages from selected conversation");
            return;
        }

        skClient.out("UNCONSUMED MESSAGES:");
        for (int i = 0; i < messages.unconsumedMessages.length; i++) {
            Message message = messages.unconsumedMessages[i];
            if (message != null) {
                skClient.out("id " + message.getOid() +
                        ", from: " + message.GetStrProperty(Message.PROPERTY.author) +
                        ", type ");// + message.getPropDebug(Message.TYPE,
                // message.GetintProperty(Message.PROPERTY.type))[2]);
            }
        }

        if (messages.unconsumedMessages.length == 0) {
            skClient.out("NONE\n");
        }

        skClient.out("CONTEXT MESSAGES:\n");
        for (int i = 0; i < messages.contextMessages.length; i++) {
            Message message = messages.contextMessages[i];
            if (message != null) {
                skClient.out("id " + message.getOid() +
                        ", from: " + message.GetStrProperty(Message.PROPERTY.author) +
                        ", type ");// + message.getPropDebug(Message.TYPE,
                // message.GetintProperty(Message.PROPERTY.type))[2]);
            }
        }

        if (messages.contextMessages.length == 0) {
            skClient.out("NONE\n");
        }
    }

    @Override
    public void execute_P() // print properties from selected message
    {
        if (hasNoConversation())
            return;

        GetLastMessagesResult messages;
        messages = skClient.currentConversation.GetLastMessages(0);
        if (messages == null) {
            skClient.error("No messages found in selected conversation");
            return;
        }

        int oid = Ask.ask_int("select message id: ");
        Message m = null;

        for (int i = 0; m == null && i < messages.contextMessages.length; i++) {
            if (messages.contextMessages[i].getOid() == oid) {
                m = messages.contextMessages[i];
            }
        }
        for (int i = 0; m == null && i < messages.unconsumedMessages.length; i++) {
            if (messages.unconsumedMessages[i].getOid() == oid) {
                m = messages.unconsumedMessages[i];
            }
        }

        if (m == null) {
            skClient.error("No message selected");
            return;
        }

        skClient.out("\t" + "convo_id\t\t" + m.GetIntProperty(Message.PROPERTY.convo_id));
        skClient.out("\t" + "convo_guid\t\t" + m.GetStrProperty(Message.PROPERTY.convo_guid));
        skClient.out("\t" + "author\t\t\t" + m.GetStrProperty(Message.PROPERTY.author));
        skClient.out("\t" + "author_displayname\t" + m.GetStrProperty(Message.PROPERTY.author_displayname));
        skClient.out("\t" + "guid\t\t\t" + m.GetBinProperty(Message.PROPERTY.guid));
        skClient.out("\t" + "originally_meant_for\t" + m.GetStrProperty(Message.PROPERTY.originally_meant_for));
        skClient.out("\t" + "timestamp\t\t" + m.GetIntProperty(Message.PROPERTY.timestamp));
        skClient.out("\t" + "type\t\t\t" + m.GetIntProperty(Message.PROPERTY.type));
        skClient.out("\t" + "sending_status\t\t" + m.GetIntProperty(Message.PROPERTY.sending_status));
        skClient.out("\t" + "consumption_status\t" + m.GetIntProperty(Message.PROPERTY.consumption_status));
        skClient.out("\t" + "edited_by\t\t" + m.GetStrProperty(Message.PROPERTY.edited_by));
        skClient.out("\t" + "edit_timestamp\t\t" + m.GetIntProperty(Message.PROPERTY.edit_timestamp));
        skClient.out("\t" + "param_key\t\t" + m.GetIntProperty(Message.PROPERTY.param_key));
        skClient.out("\t" + "param_value\t\t" + m.GetIntProperty(Message.PROPERTY.param_value));
        skClient.out("\t" + "body_xml\t\t" + m.GetStrProperty(Message.PROPERTY.body_xml));
        skClient.out("\t" + "identities\t\t" + m.GetStrProperty(Message.PROPERTY.identities));
        skClient.out("\t" + "reason\t\t" + m.GetStrProperty(Message.PROPERTY.reason));
        skClient.out("\t" + "leavereason\t\t" + m.GetIntProperty(Message.PROPERTY.leavereason));
        skClient.out("\t" + "participant_count\t" + m.GetIntProperty(Message.PROPERTY.participant_count));
    }

    @Override
    public void execute_X() // debug SKYPEKIT-351
    {
        if (hasNoConversation())
            return;

        Conversation x = skClient.currentConversation;
        skClient.error("Conversation id:" + x.getOid());
        Participant [] xp = x.GetParticipants(PARTICIPANTFILTER.OTHER_CONSUMERS);
        for (int i = 0; i < xp.length; i++) {
            skClient.error("Particpant identity:" + xp[i].GetStrProperty(Participant.PROPERTY.identity));
            skClient.error("... adder:" + xp[i].GetStrProperty(Participant.PROPERTY.adder));
            skClient.error("... convo_id:" + xp[i].GetIntProperty(Participant.PROPERTY.convo_id));
            skClient.error("... debuginfo:" + xp[i].GetStrProperty(Participant.PROPERTY.debuginfo));
            skClient.error("... last_voice_error:" + xp[i].GetStrProperty(Participant.PROPERTY.last_voice_error));
            skClient.error("... live_country:" + xp[i].GetStrProperty(Participant.PROPERTY.live_country));
            skClient.error("... live_fwd_identities:" + xp[i].GetStrProperty(Participant.PROPERTY.live_fwd_identities));
            skClient.error("... live_identity:" + xp[i].GetStrProperty(Participant.PROPERTY.live_identity));
            skClient.error("... live_price_for_me:" + xp[i].GetStrProperty(Participant.PROPERTY.live_price_for_me));
            skClient.error("... live_start_timestamp:" + xp[i].GetIntProperty(Participant.PROPERTY.live_start_timestamp));
            skClient.error("... live_type:" + xp[i].GetIntProperty(Participant.PROPERTY.live_type));
            skClient.error("... quality_problems:" + xp[i].GetStrProperty(Participant.PROPERTY.quality_problems));
            skClient.error("... rank:" + xp[i].GetIntProperty(Participant.PROPERTY.rank));
            skClient.error("... requested_rank:" + xp[i].GetIntProperty(Participant.PROPERTY.requested_rank));
            skClient.error("... sound_level:" + xp[i].GetIntProperty(Participant.PROPERTY.sound_level));
            skClient.error("... text_status:" + xp[i].GetIntProperty(Participant.PROPERTY.text_status));
            skClient.error("... transferred_by:" + xp[i].GetStrProperty(Participant.PROPERTY.transferred_by));
            skClient.error("... transferred_to:" + xp[i].GetStrProperty(Participant.PROPERTY.transferred_to));
            skClient.error("... video_status:" + xp[i].GetIntProperty(Participant.PROPERTY.video_status));
            skClient.error("... voice_status:" + xp[i].GetIntProperty(Participant.PROPERTY.voice_status));
        }
    }
}
