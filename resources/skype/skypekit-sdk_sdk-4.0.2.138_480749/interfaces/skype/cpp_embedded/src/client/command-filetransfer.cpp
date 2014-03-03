
#include "command-filetransfer.h"

void CommandFileTransfer::executeHelp()
{
        printf("\n[FILE TRANSFER] - c\n"
               "\tfs - send a file\n"
               "\tfr - receive a file\n"
               "\tfc - cancel\n"
               "\tfp - pause or resume a transfer\n");
}

void CommandFileTransfer::execute_s()
{
        Conversation::Ref conversation = skype->get_current_conversation();

        if (conversation) {
                List_Filename files;
                String fnms;
                for (Filename fn = ask("filename: "); fn != ""; fn = ask("filename: ")) {
                        files.append(fn);
                        fnms += fn;
                }
                TRANSFER_SENDFILE_ERROR err;
                Filename fnerr;
                if (!conversation->PostFiles(files, fnms, err, fnerr) || err)
                        printf("failed to post %s (from %s) because %d\n", 
                                (const char*)fnerr, 
                                (const char*)fnms, err);
                else
                        printf("posting %s\n", (const char*)fnerr);
        }
}

void pauseresume(const Message::Refs& messages)
{
        for (unsigned int m = 0; m < (uint)messages.size(); m++) {
                Message::Ref msg = messages[m];
                if (msg->GetUintProp(Message::P_TYPE) == Message::POSTED_FILES) {
                        Transfer::Refs transfers;
                        if (msg->GetTransfers(transfers)) {
                                for (unsigned int i = 0; i < (uint)transfers.size(); i++) {
                                        Transfer::Ref transfer = transfers[i];
                                        if (transfer->GetUintProp(Transfer::P_STATUS) == Transfer::PAUSED) {
                                                transfer->Resume();
                                        } else {
                                                transfer->Pause();
                                        }
                                }
                        }
                }
        }
}

void cancel(const Message::Refs& messages)
{
        for (unsigned int m = 0; m < (uint)messages.size(); m++) {
                Message::Ref msg = messages[m];
                if (msg->GetUintProp(Message::P_TYPE) == Message::POSTED_FILES) {
                        Transfer::Refs transfers;
                        if (msg->GetTransfers(transfers)) {
                                for (unsigned int i = 0; i < (uint)transfers.size(); i++) {
                                        Transfer::Ref transfer = transfers[i];
                                        transfer->Cancel();
                                }
                        }
                }
        }
}

void receive(const Message::Refs& messages)
{
        for (unsigned int m = 0; m < (uint)messages.size(); m++) {
                Message::Ref msg = messages[m];
                if (msg->GetUintProp(Message::P_TYPE) == Message::POSTED_FILES) {
                        Transfer::Refs transfers;
                        if (msg->GetTransfers(transfers)) {
                                for (unsigned int i = 0; i < (uint)transfers.size(); i++) {
                                        Transfer::Ref transfer = transfers[i];
                                        Transfer::TYPE type; 
                                        transfer->GetPropType(type);
                                        bool result;
                                        if (type == Transfer::INCOMING && transfer->GetUintProp(Transfer::P_STATUS) < Transfer::TRANSFERRING) {
                                                transfer->Accept(ask("save incoming file to (path + filename): "), result);   
                                                if (!result) {
                                                        printf("failed to accept tranfer\n");       
                                                }   
                                        }
                                }
                        }
                }
        }
}

void CommandFileTransfer::execute_p()
{
        Conversation::Ref conversation = skype->get_current_conversation();

        if (conversation) {

                Message::Refs oldmsg;
                Message::Refs newmsg;
                if (!conversation->GetLastMessages(oldmsg,newmsg)) {
                        printf("no transfers are available in current conversation\n");
                        return;
                }

                pauseresume(oldmsg);
                pauseresume(newmsg);
        }
}

void CommandFileTransfer::execute_r() // receive
{
        Conversation::Ref conversation = skype->get_current_conversation();

        if (conversation) {

                Message::Refs oldmsg;
                Message::Refs newmsg;
                if (!conversation->GetLastMessages(oldmsg,newmsg)) {
                        printf("no transfers are available in current conversation\n");
                        return;
                }

                receive(oldmsg);
                receive(newmsg);
        }
}

void CommandFileTransfer::execute_c() // cancel
{
        Conversation::Ref conversation = skype->get_current_conversation();

        if (conversation) {

                Message::Refs oldmsg;
                Message::Refs newmsg;
                if (!conversation->GetLastMessages(oldmsg,newmsg)) {
                        printf("no transfers are available in current conversation\n");
                        return;
                }

                cancel(oldmsg);
                cancel(newmsg);
        }
}

