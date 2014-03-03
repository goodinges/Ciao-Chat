
#ifndef CommandConversation_INCLUDED_HPP
#define CommandConversation_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandConversation : public ClientCommandInterface
{
public:
        CommandConversation() {}
        ~CommandConversation() {}

        void executeHelp();
        String getName() {
                return String("Conversation");
        }

        void execute_c(); //set current conversation
        void execute_u(); //open conversation by skypename
        void execute_t(); //set topic of current conversation
        void execute_p(); //set picture of current conversation
        void execute_B(); //bookmark current conversation
        void execute_b(); //unbookmark current conversation
        void execute_g(); // set guidelines
        void execute_n(); //set display name
        void execute_l(); //retire from current conversation
        void execute_o(); //set rank
        void execute_w(); //set text status write
        void execute_W(); //set text status write as angry
        void execute_r(); //set text status reading
        void execute_a(); //set alert string
        void execute_f(); //find message with text
        void execute_m(); //list messages
        void execute_x(); //set current conversation (filtered) 
        void execute_P(); //print message properties        


private:

};

#endif //CommandConversation_INCLUDED_HPP

