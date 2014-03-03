
#include "command-conversation.h"

void CommandConversation::executeHelp()
{
        printf("\n[CONVERSATION] - c\n"
               "\tcc - select a conversation as current one\n"
               "\tcx - select a conversation as current one (filtered list)\n"
               "\tcu - open conversation using skype name\n"
               "\tct - set topic of current conversation\n"
               "\tcp - set picture of current conversation\n"
               "\tcB - bookmark current conversation\n"
               "\tcb - unbookmark current conversation\n"
               "\tcg - set guidelines\n"
               "\tcn - set display name\n"
               "\tcl - retire from current conversation\n"
               "\tck - start/stop typing\n"
               "\tco - set rank\n"
               "\tcw - set text status write\n"
               "\tcW - set text status angry\n"
               "\tcr - set text status reading\n"
               "\tca - set alert string\n"
               "\tcf - find message with text\n"
               "\tcm - list messages\n"
               "\tcP - print properties from selected message\n"
              );
        fflush(stdout);
}

void CommandConversation::execute_u()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        List_String names;
        names.append(ask("enter skype name: "));
        Conversation::Ref conversation;
        if (!skype->GetConversationByParticipants(names, conversation, true, false))
        {
                printf("Unable to get conversation\n");
                fflush(stdout);
                return;
        }
        skype->set_current_conversation(conversation);     
        ParticipantRefs participants;
        conversation->GetParticipants(participants);
        for (uint i = 0; i < (uint)participants.size(); i++) 
            participants[i]->getOID();    
}

void CommandConversation::execute_c()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }

        Conversation::Refs conversations;
        if (!skype->GetConversationList(conversations)) { //LIVE_CONVERSATIONS))
                printf("Unable to get conversation list\n");
                fflush(stdout);
                return;
        }
        
        if (!conversations.size()) {
                printf("0 conversations\n");
                return;
        }
        
        List_uint props;
        props.append(Conversation::P_DISPLAYNAME);
        props.append(Conversation::P_IDENTITY);
        skype->FetchMultipleObjectsProps(Conversation::MODULE_ID, conversations, props);        
        
        for (uint i = 0; i < (uint)conversations.size(); i++) {
                CConversation::Ref conversation = conversations[i];
                printf("\t%d %s (%s)\n", conversation->getOID(), 
                        (const char*)conversation->GetStrProp(Conversation::P_DISPLAYNAME), 
                        (const char*)conversation->GetStrProp(Conversation::P_IDENTITY));
        }
        
        fflush(stdout);
        skype->set_current_conversation(0);
        Conversation::Ref conversation(ask_int("select conversation id: "));
        if (conversation) {
                skype->set_current_conversation(conversation);     
                //create participant objects to receive propchanges 
                ParticipantRefs participants;
                conversation->GetParticipants(participants);
                for (uint i = 0; i < (uint)participants.size(); i++) {
                        participants[i]->getOID();    
                }                           
        }
}

void CommandConversation::execute_x()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }

        const char* labels[] = { "Conversation::ALL_CONVERSATIONS", "Conversation::INBOX_CONVERSATIONS", "Conversation::BOOKMARKED_CONVERSATIONS",
                "Conversation::LIVE_CONVERSATIONS", "" };
        int values[] = { Conversation::ALL_CONVERSATIONS, Conversation::INBOX_CONVERSATIONS, 
                Conversation::BOOKMARKED_CONVERSATIONS, Conversation::LIVE_CONVERSATIONS };

        Conversation::Refs conversations;
        int type = values[ask_list("select filter:", labels)];
        printf("type %d\n", type);
        if (!skype->GetConversationList(conversations, (Conversation::LIST_TYPE)type)) { 
                printf("Unable to get conversation list\n");
                fflush(stdout);
                return;
        }
        
        if (!conversations.size()) {
                printf("0 conversations\n");
                return;
        }
        
        List_uint props;
        props.append(Conversation::P_DISPLAYNAME);
        props.append(Conversation::P_IDENTITY);
        skype->FetchMultipleObjectsProps(Conversation::MODULE_ID, conversations, props);        
        
        for (uint i = 0; i < (uint)conversations.size(); i++) {
                CConversation::Ref conversation = conversations[i];
                printf("\t%d %s (%s)\n", conversation->getOID(), 
                        (const char*)conversation->GetStrProp(Conversation::P_DISPLAYNAME), 
                        (const char*)conversation->GetStrProp(Conversation::P_IDENTITY));
        }
        
        fflush(stdout);
        skype->set_current_conversation(0);
        Conversation::Ref conversation(ask_int("select conversation id: "));
        if (conversation) {
                skype->set_current_conversation(conversation); 
                //create participant objects to receive propchanges 
                ParticipantRefs participants;
                conversation->GetParticipants(participants);
                for (uint i = 0; i < (uint)participants.size(); i++) {
                        participants[i]->getOID();    
                }                               
        }
}

void CommandConversation::execute_t() //set topic of current conversation
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->SetTopic(ask("topic"),false);
        }
}

void CommandConversation::execute_p() //set picture of current conversation
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                String filename = ask("filename: (jpg image, size less then 50 kb)");
                Binary pic;
                if (load_file(filename, pic, "rb")) {
                        if (pic.size() >= 50000) {
                                printf("unable to set picture! pic size too large: %d\n", (int) pic.size());
                                return;        
                        }
                        if (!conversation->SetPicture(pic)) {
                                printf("unable to set picture!\n"); 
                        }
                } else {
                        printf("can't load picture %s\n", (const char*)filename);
                }
        }
}

void CommandConversation::execute_B() //bookmark current conversation
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->SetBookmark(true);
        }
}

void CommandConversation::execute_b() //unbookmark current conversation
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->SetBookmark(false);
        }
}

void CommandConversation::execute_g() // set guidelines
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->SetGuidelines(ask("guideline"),false);
        }
}

void CommandConversation::execute_n() //set display name
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->RenameTo(ask("new name: "));
        }
}

void CommandConversation::execute_l() //retire from current conversation
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->RetireFrom();
                skype->set_current_conversation(Conversation::Ref());
        }
}

void CommandConversation::execute_o() //set rank
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->SetOption(Conversation::P_OPT_ENTRY_LEVEL_RANK,ask_int("rank: "));
        }
}

void CommandConversation::execute_w() //set text status to write
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->SetMyTextStatusTo(Participant::WRITING);
        }
}

void CommandConversation::execute_W() //set text status to write as angry
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->SetMyTextStatusTo(Participant::WRITING_AS_ANGRY);
        }
}

void CommandConversation::execute_r() //set text status to reading
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->SetMyTextStatusTo(Participant::READING);
        }
}

void CommandConversation::execute_a() //set alert string
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->SetAlertString(ask("alert: "));
        }
}

void CommandConversation::execute_f() //find message
{
        Message::Ref msg;
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->FindMessage(ask("message: "), msg);
                printf("found %s\n", 
                        msg ? (const char*)msg->getPropDebug(Message::P_TYPE, msg->GetUintProp(Message::P_TYPE))[2] : "(none)");
        }
}

void CommandConversation::execute_m() //list messages
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                List_uint l;
                l.append(Conversation::P_UNCONSUMED_SUPPRESSED_MESSAGES);
                l.append(Conversation::P_UNCONSUMED_NORMAL_MESSAGES);
                l.append(Conversation::P_UNCONSUMED_ELEVATED_MESSAGES);
                l.append(Conversation::P_UNCONSUMED_MESSAGES_VOICE);
                conversation->FetchProps(l);
                
                printf ("UNCONSUMED MESSAGES: SUPPRESSED: %u; NORMAL: %u; ELEVATED: %u; VOICE: %s\n",
                        conversation->GetUintProp(Conversation::P_UNCONSUMED_SUPPRESSED_MESSAGES),
                        conversation->GetUintProp(Conversation::P_UNCONSUMED_NORMAL_MESSAGES),
                        conversation->GetUintProp(Conversation::P_UNCONSUMED_ELEVATED_MESSAGES),
                        conversation->GetBoolProp(Conversation::P_UNCONSUMED_MESSAGES_VOICE) ? "there are some" : "none");      
                
                MessageRefs contextMessages;
                MessageRefs unconsumedMessages;
                if (!conversation->GetLastMessages(contextMessages, unconsumedMessages)) {
                        printf("Unable to get messages from selected conversation\n");
                        return;
                }
                
                printf ("UNCONSUMED MESSAGES:\n"); 
                for (uint i = 0; i < (uint)unconsumedMessages.size(); i++) {
                        CMessage::Ref message = unconsumedMessages[i];
                        if (message) {
                                printf("id %d, from: %s, type %s\n", message->getOID(), 
                                       (const char*)message->GetStrProp(Message::P_AUTHOR),
                                        (const char*)message->getPropDebug(Message::P_TYPE, message->GetUintProp(Message::P_TYPE))[2]);    
                        }
                }
                
                if (!unconsumedMessages.size()) {
                        printf("NONE\n"); 
                }
                
                printf ("CONTEXT MESSAGES:\n");
                for (uint i = 0; i < (uint)contextMessages.size(); i++) {
                        CMessage::Ref message = contextMessages[i];
                        if (message) {
                                printf("id %d, from: %s, type %s\n", message->getOID(), 
                                        (const char*)message->GetStrProp(Message::P_AUTHOR),
                                        (const char*)message->getPropDebug(Message::P_TYPE, message->GetUintProp(Message::P_TYPE))[2]);  
                        }
                } 
                
                if (!contextMessages.size()) {
                        printf("NONE\n"); 
                }                                       
        }
}

void CommandConversation::execute_P()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                Message::Ref message(ask_int("select message id: "));
                if (message) {
                        List_uint props;
                        List_String names;

                        props.append(Message::P_CONVO_ID);
                        props.append(Message::P_CONVO_GUID);
                        props.append(Message::P_AUTHOR);
                        props.append(Message::P_AUTHOR_DISPLAYNAME);
                        props.append(Message::P_GUID);
                        props.append(Message::P_ORIGINALLY_MEANT_FOR);
                        props.append(Message::P_TIMESTAMP);
                        props.append(Message::P_TYPE);
                        props.append(Message::P_SENDING_STATUS);
                        props.append(Message::P_CONSUMPTION_STATUS);
                        props.append(Message::P_EDITED_BY);
                        props.append(Message::P_EDIT_TIMESTAMP);
                        props.append(Message::P_PARAM_KEY);
                        props.append(Message::P_PARAM_VALUE);
                        props.append(Message::P_BODY_XML);
                        props.append(Message::P_IDENTITIES);
                        props.append(Message::P_REASON);
                        props.append(Message::P_LEAVEREASON);
                        props.append(Message::P_PARTICIPANT_COUNT);
                
                        message->FetchProps(props);
                        Dict_int d = message->GetProps(props);
                        for (int i = 0, e = props.size(); i < e; i++) {
                                String value = d.value(props[i]);
                                List_String dbg = message->getPropDebug(props[i], value);
                                printf("        %s - %s\n",
                                        (const char*)dbg[1], 
                                        (const char*)dbg[2]);
                        }               
                                                        
                }
        }
}

