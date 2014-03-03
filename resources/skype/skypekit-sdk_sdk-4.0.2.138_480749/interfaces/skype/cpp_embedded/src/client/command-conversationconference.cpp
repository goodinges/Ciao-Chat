
#include "command-conversationconference.h"

void CommandConversationConference::executeHelp()
{
        printf("\n[CONVERSATION CONFERENCE] - M\n"
               "\tMs - spawn a conf\n"
               "\tMp - add a participant\n"
               "\tMm - mute/unmute\n"
               "\tMh - hold/unhold\n"
               "\tMd - dtmf\n"
               "\tMj - join live conversation\n"
              );
}

void CommandConversationConference::execute_s()
{
        CConversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                List_String participants;
				bool temp = true; 
                while (temp) {
                        participants.push_back(ask("Add participant:"));
                        temp = (ask_yesno("Add another?") == 0);
                }

                Conversation::Ref conference;
                conversation->SpawnConference(participants, conference);
                if (conference) skype->set_current_conversation(conference);
        }
}

void CommandConversationConference::execute_p()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                Conversation::TYPE ctype;
                conversation->GetPropType(ctype);
                if (ctype == Conversation::DIALOG) {
                        printf("Conversation is not a conference, choose 'ms - spawn a conf' instead\n");
                        return;
                }

                List_String participants;
				bool temp = true; 
                while (temp) {
                        participants.push_back(ask("Add participant:"));
                        temp = (ask_yesno("Add another?") == 0);
                }

                conversation->AddConsumers(participants);
        }
}

void CommandConversationConference::execute_m()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                bool is_muted = false;
                conversation->GetPropLiveIsMuted(is_muted);
                if (!is_muted) {
                        conversation->MuteMyMicrophone();
                } else {
                        conversation->UnmuteMyMicrophone();
                }
        }
}

void CommandConversationConference::execute_h()
{
       Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                Conversation::LOCAL_LIVESTATUS lstatus;
                conversation->GetPropLocalLivestatus(lstatus);
                if (lstatus != Conversation::ON_HOLD_LOCALLY) {
                        conversation->HoldMyLiveSession();
                } else {
                        conversation->ResumeMyLiveSession();
                }
        }
}

void CommandConversationConference::execute_d()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                String dtmfstr = ask("Enter dtmf (1,2,3,4,5,6,7,8,9,0,*,#):");
                Participant::DTMF dtmf;
                if (dtmfstr == "*") {
                        dtmf = Participant::DTMF_STAR;
                } else if (dtmfstr == "#") {
                        dtmf = Participant::DTMF_POUND;
                } else {
                        dtmf = (Participant::DTMF)atoi((const char*)dtmfstr);
                }

                conversation->SendDTMF(dtmf,260);
        }
}

void CommandConversationConference::execute_j()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->JoinLiveSession();
        }
}


