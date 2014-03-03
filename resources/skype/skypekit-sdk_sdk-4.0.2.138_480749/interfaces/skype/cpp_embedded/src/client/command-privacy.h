
#ifndef CommandPrivacy_INCLUDED_HPP
#define CommandPrivacy_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandPrivacy : public ClientCommandInterface
{
public:
        CommandPrivacy() {}
        ~CommandPrivacy() {}

        void executeHelp();
        String getName() {
                return String("Privacy");
        }

        void execute_a(); //change avatar policy
        void execute_b(); //change contact count policy
        void execute_c(); //change chat policy
        void execute_i(); //change skypein policy
        void execute_o(); //change skypeout policy
        void execute_z(); //change timezone policy
        void execute_v(); //change voicemail policy
        void execute_w(); //change web presence policy

private:

};

#endif //CommandSms_INCLUDED_HPP

