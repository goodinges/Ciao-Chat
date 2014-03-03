
#ifndef CommandConversationConference_INCLUDED_HPP
#define CommandConversationConference_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandConversationConference : public ClientCommandInterface
{
public:
        CommandConversationConference() {}
        ~CommandConversationConference() {}

        void executeHelp();
        String getName() {
                return String("ConversationConference");
        }

        void execute_s(); //spawn a conf
        void execute_p(); //add a participant
        void execute_m(); //mute/unmute
        void execute_h(); //hold/unhold
        void execute_d(); //dtmf
        void execute_j(); //join live convesation        

private:

};

#endif //CommandConversationConference_INCLUDED_HPP

