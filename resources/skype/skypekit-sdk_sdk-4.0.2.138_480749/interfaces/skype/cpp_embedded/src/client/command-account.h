
#ifndef CommandAccount_INCLUDED_HPP
#define CommandAccount_INCLUDED_HPP

#include "main.h"
#include "command-interface.h"

class CommandAccount : public ClientCommandInterface
{
public:
        CommandAccount() {
                availability=0;
        }
        ~CommandAccount() {}

        void executeHelp();
        String getName() {
                return String("Account");
        }

        void execute_L(); // login (with password)
        void execute_I(); // login (without password)
        void execute_D(); // login (with default account)
        void execute_R(); // register
        void execute_O(); // logout (don't save password
        void execute_P(); // change password (don't save password
        void execute_p(); // change presence
        void execute_K(); // show own capabilities
        void execute_n(); // change full name
        void execute_m(); // change mood
        void execute_e(); // change email
        void execute_h(); // change home phone
        void execute_o(); // change office phone
        void execute_c(); // change cell phone
        void execute_b(); // change birthday
        void execute_g(); // change gender
        void execute_l(); // change language
        void execute_C(); // change country
        void execute_r(); // change region
        void execute_t(); // change city
        void execute_w(); // change homepage
        void execute_a(); // change about
        void execute_z(); // change timezone
        void execute_F(); // set call forwarding
        void execute_A(); // view auth requests
        void execute_E(); // view existing accounts
        void execute_V(); // validate skypename
        void execute_1(); // set avatar image

        static const Contact::AVAILABILITY availabilities[];
        static const char* availabilities_as_text[];
        int availability;
};

#endif //CommandAccount_INCLUDED_HPP

