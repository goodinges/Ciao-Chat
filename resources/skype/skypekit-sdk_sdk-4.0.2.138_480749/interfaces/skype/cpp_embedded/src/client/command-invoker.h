
#ifndef CommandInvoker_INCLUDED_HPP
#define CommandInvoker_INCLUDED_HPP

#include "command-account.h"
#include "command-conversation.h"
#include "command-conversationcall.h"
#include "command-conversationconference.h"
#include "command-contactsearch.h"
#include "command-contactlist.h"
#include "command-filetransfer.h"
#include "command-voicemail.h"
#include "command-sms.h"
#include "command-greeting.h"
#include "command-privacy.h"
#include "command-conversationchat.h"
#include "command-setup.h"
#include "command-app2app.h"

class CommandInvoker
{
public:
        CommandInvoker();
        ~CommandInvoker();

        void executeCommand(unsigned char command, unsigned char subCommand);
        ClientCommandInterface* getCommand(String key);
        void executeHelp(bool skypecommands = true);
        void executeFullHelp();

private:
        Sid::Dict_ptr_str ptrDictionary;

};

#endif //CommandInvoker_INCLUDED_HPP

