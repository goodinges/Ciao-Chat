
#include "command-contactsearch.h"

void CommandContactSearch::complete_search() {
        current_contactsearch = 0; // invalidate the ref with a null pointer
}

void CommandContactSearch::executeHelp()
{
        printf("\n[CONTACTSEARCH] - s\n" 
               "\tsb - basic search\n" 
               "\tsi - contact search for buddy by skypename\n"
               "\tst - contact search with terms\n");
        fflush(stdout);
}


void CommandContactSearch::execute_b()
{
        if (!(skype->activeAccount && skype->activeAccount->getStatus() == Account::LOGGED_IN)) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }


        if (current_contactsearch) {
                printf("Wait that current search completes before submitting a new one\n");
                fflush(stdout);
                return;
        }

        CContactSearch::Ref search;

        if (!skype->CreateBasicContactSearch(ask("text"), search)) {
                printf("Unable to create contact search\n");
                fflush(stdout);
                return;
        }
        search->set_controller(this);

        bool valid;
        if (!search->IsValid(valid) || !valid || !search->Submit()) {
                printf("Unable to submit contact search\n");
                fflush(stdout);
        } else {
                current_contactsearch = search;
        }
}

void CommandContactSearch::execute_t()
{
        if (!(skype->activeAccount && skype->activeAccount->getStatus() == Account::LOGGED_IN)) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }

        if (current_contactsearch) {
                printf("Wait that current search completes before submitting a new one\n");
                fflush(stdout);
                return;
        }

        CContactSearch::Ref search;
        bool Valid; 
        if (skype->CreateContactSearch(search)) { 
                search->set_controller(this);
                search->AddStrTerm(Contact::P_SKYPENAME, ContactSearch::CONTAINS_WORDS, ask("contains word:"), Valid); 

                if (Valid) { 
                         printf("Starting search..\n"); 
                         search->Submit(); 
                         current_contactsearch = search;
                }
        }
}


void CommandContactSearch::execute_i()
{
        if (!(skype->activeAccount && skype->activeAccount->getStatus() == Account::LOGGED_IN)) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }


        if (current_contactsearch) {
                printf("Wait that current search completes before submitting a new one\n");
                fflush(stdout);
                return;
        }

        CContactSearch::Ref search;
        if (!skype->CreateIdentitySearch(ask("identity"), search)) {
                printf("Unable to create contact search\n");
                fflush(stdout);
                return;
        }
        search->set_controller(this);
        bool valid;
        if (!search->IsValid(valid) || !valid || !search->Submit()) {
                printf("Unable to submit contact search\n");
                fflush(stdout);
        } else {
                current_contactsearch = search;
        }

}

