
#include "command-voicemail.h"

void CommandVoiceMail::executeHelp()
{
        printf("\n[VOICE MAIL] - V\n"
               "\tVv - start recording new voicemail\n"
               "\tVV - end recording and send voicemail\n"
               "\tVw - cancel voicemail recording/send\n"
               "\tVp - play voicemails (first use 'cc' to select conversation)\n"
               "\tVs - stop voicemail playback\n"
              );
}

void CommandVoiceMail::execute_v()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                conversation->StartVoiceMessage();
        }
}

void CommandVoiceMail::execute_V()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                Voicemail::Ref vm;
                conversation->GetPropActiveVmId(vm);
                if (vm) {
                         vm->StopRecording();
                         conversation->PostVoiceMessage(vm, "empty body");
                }
                else 
                {
                        printf("Unable to get active vm"); 
                }
        }
}

void CommandVoiceMail::execute_w()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                Voicemail::Ref vm;
                conversation->GetPropActiveVmId(vm);                
                if (vm) {
                        vm->Cancel();
                }
        }
}

void CommandVoiceMail::execute_s()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                uint id = ask_int("select voicemail id : ");
                CMessage::Ref message(id);
                if (message) {
                        Voicemail::Ref vm;
                        if (message->GetVoiceMessage(vm)) {
                                vm->StopPlayback();
                        } else {
                                printf("Unable to stop playback\n");
                        }
                }
	}
}

void CommandVoiceMail::execute_p()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {

                MessageRefs contextMessages;
                MessageRefs unconsumedMessages;
                if (!conversation->GetLastMessages(contextMessages, unconsumedMessages)) {
                        printf("Unable to get messages from selected conversation\n");
                        return;
                }

                for (uint i = 0; i < (uint)unconsumedMessages.size(); i++) {
                        CMessage::Ref message = unconsumedMessages[i];
                        if ((Message::TYPE)message->GetUintProp(Message::P_TYPE) == Message::POSTED_VOICE_MESSAGE) {
                                printf("id %d, from: %s\n", message->getOID(), 
                                        (const char*)message->GetStrProp(Message::P_AUTHOR));
                        }
                }

                for (uint i = 0; i < (uint)contextMessages.size(); i++) {
                        CMessage::Ref message = contextMessages[i];
                        if ((Message::TYPE)message->GetUintProp(Message::P_TYPE) == Message::POSTED_VOICE_MESSAGE) {
                                printf("id %d, from: %s\n", message->getOID(), (const char*)message->GetStrProp(Message::P_AUTHOR));
                        }
                }

                uint id = ask_int("select voicemail id : ");
                CMessage::Ref message(id);
                if (message) {
                        Voicemail::Ref vm;
                        if (message->GetVoiceMessage(vm)) {
                                printf("Playing\n");
                                vm->StartPlayback();
                        } else {
                                printf("Unable to play voice message\n");
                        }
                }
        }
}


