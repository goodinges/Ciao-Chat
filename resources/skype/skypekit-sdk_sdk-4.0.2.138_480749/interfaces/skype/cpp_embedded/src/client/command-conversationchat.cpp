
#include "command-conversationchat.h"

void CommandConversationChat::executeHelp()
{
        printf("\n[CONVERSATION CHAT] - h\n"
               "\ths - send chat message\n"
               "\the - edit last chat message\n"
              );
}

void CommandConversationChat::execute_s()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                Message::Ref message;             
                conversation->SetMyTextStatusTo(Participant::WRITING);    
                conversation->PostText(ask("Enter chat message:"), message);
        }
}

void CommandConversationChat::execute_e()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                return;
        }

        CMessage::Ref cmessage = skype->get_last_message();
        if (cmessage) {
                if ((Message::TYPE)cmessage->GetUintProp(Message::P_TYPE) != Message::POSTED_TEXT) {
                        printf("Last message is not a chat message\n");
                        return;
                }

                bool result = false;
                cmessage->CanEdit(result);
                if (!result) {
                        printf("Can't edit this message\n");
                        return;
                }

                printf("\nMessage text is: %s\n", (const char*)cmessage->GetStrProp(Message::P_BODY_XML));
                cmessage->Edit(ask("New text:"),false);
        } else {
                printf("Receive some message first\n");
                return;
        }
}




















