
#ifndef CommandContactSearch_INCLUDED_HPP
#define CommandContactSearch_INCLUDED_HPP

#include "main.h"
#include "command-interface.h"

class CommandContactSearch : public ClientCommandInterface
{
public:
        CommandContactSearch() {}
        ~CommandContactSearch() {}

        void executeHelp();
        String getName() {
                return String("ContactSearch");
        }

        void execute_b(); //basic search
        void execute_i(); //contact search for buddy by skypename
        void execute_t(); //contact search with term

        void complete_search();

        CContactSearch::Ref current_contactsearch;
};

#endif //CommandContactSearch_INCLUDED_HPP

