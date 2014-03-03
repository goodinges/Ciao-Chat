
#ifndef CommandSms_INCLUDED_HPP
#define CommandSms_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandSms : public ClientCommandInterface
{
public:
        CommandSms() {}
        ~CommandSms() {}

        void executeHelp();
        String getName() {
                return String("Sms");
        }

        void execute_s(); //send
        void execute_p(); //price
        void execute_c(); //char number
        void execute_m(); //message count

        Sms::Ref create_sms();
private:

};

#endif //CommandSms_INCLUDED_HPP

