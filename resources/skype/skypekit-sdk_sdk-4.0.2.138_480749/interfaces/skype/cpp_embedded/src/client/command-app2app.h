
#ifndef CommandApp2app_INCLUDED_HPP
#define CommandApp2app_INCLUDED_HPP

#include "main.h"
#include "command-interface.h"

class CommandApp2app: public ClientCommandInterface
{
public:
        CommandApp2app() {}
        ~CommandApp2app() {}

        String getName() { 
            return String("App2app");
        }
        void executeHelp();

        void execute_C(); // create
        void execute_D(); // delete
        void execute_c(); // connect
        void execute_d(); // disconnect
        void execute_w(); // write
        void execute_m(); // write max data
        void execute_M(); // write max+1 data
        void execute_g(); // datagram
        void execute_x(); // max datagram
        void execute_X(); // max+1 datagram
        void execute_r(); // read
        void execute_u(); // get connectable users
        void execute_L(); // get stream list
private:
        void writeTrashData(unsigned int size);
        void writeTrashDatagram(unsigned int size);

private:

        enum Limits {
            MAX_DATAGRAM_SIZE = 1400,
            MAX_WRITE_SIZE = 65535
        };

        static const char* stream_types_as_text[];
};

#endif //CommandApp2app_INCLUDED_HPP

