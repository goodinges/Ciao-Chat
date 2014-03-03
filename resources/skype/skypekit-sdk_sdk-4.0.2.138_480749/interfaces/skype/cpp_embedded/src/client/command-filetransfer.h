
#ifndef CommandFileTransfer_INCLUDED_HPP
#define CommandFileTransfer_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandFileTransfer : public ClientCommandInterface
{
public:
        CommandFileTransfer() {}
        ~CommandFileTransfer() {}

        void executeHelp();
        String getName() {
                return String("FileTransfer");
        }

        void execute_s(); //send
        void execute_r(); //receive
        void execute_p(); //pause resume
        void execute_c(); //cancel

private:
};

#endif //CommandFileTransfer_INCLUDED_HPP

