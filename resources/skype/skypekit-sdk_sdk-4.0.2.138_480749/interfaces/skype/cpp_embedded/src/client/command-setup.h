
#ifndef CommandSetup_INCLUDED_HPP
#define CommandSetup_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandSetup : public ClientCommandInterface
{
public:
        CommandSetup() {}
        ~CommandSetup() {}

        void executeHelp();
        String getName() {
                return String("Setup");
        }

        void execute_f();
        void execute_a();
        void execute_n();
        void execute_t();
        void execute_v();
        void execute_c();    
        void execute_s();    
        void execute_d();  
		void execute_i();
        
        void print_int_val(const char* key, const char* descr);
private:

};

#endif //CommandSetup_INCLUDED_HPP

