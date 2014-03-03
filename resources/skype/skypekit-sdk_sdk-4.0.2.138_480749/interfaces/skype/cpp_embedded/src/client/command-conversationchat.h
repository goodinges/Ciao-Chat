
#ifndef CommandConversationChat_INCLUDED_HPP
#define CommandConversationChat_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandConversationChat : public ClientCommandInterface
{
public:
        CommandConversationChat() {}
        ~CommandConversationChat() {}

        void executeHelp();
        String getName() {
                return String("ConversationChat");
        }

        void execute_s();
        void execute_e();

private:

};

#endif //CommandConversationChat_INCLUDED_HPP

