
#ifndef CommandGreeting_INCLUDED_HPP
#define CommandGreeting_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandGreeting : public ClientCommandInterface
{
public:
        CommandGreeting() {}
        ~CommandGreeting() {}

        void executeHelp();
        String getName() {
                return String("Greeting");
        }

        void execute_p(); //play
        void execute_r(); //record
        void execute_s(); //stop record
        void execute_z(); //reset

private:

};

#endif //CommandGreeting_INCLUDED_HPP

