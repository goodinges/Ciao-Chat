
#ifndef CommandContactList_INCLUDED_HPP
#define CommandContactList_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

extern const PMAP CONTACTGROUP_TYPE_MAP[];

class CommandContactList : public ClientCommandInterface
{
public:
        CommandContactList() {}
        ~CommandContactList() {}

        void executeHelp();
        String getName() {
                return String("ContactList");
        }

        void execute_B(); //list all buddies
        void execute_r(); //rename
        void execute_C(); //add and send auth
        void execute_p(); //list contact properties
        void execute_b(); //block
        void execute_u(); //block
        void execute_d(); //delete contact
        void execute_a(); //report abuse
        void execute_g(); //report abuse
        void execute_v(); //get avatar of contact
        void execute_R(); //refresh profile
        void execute_K(); //contact capabilities

private:
        Contact::Ref find_contact(Sid::String& skypename);
};

#endif //CommandContactList_INCLUDED_HPP

