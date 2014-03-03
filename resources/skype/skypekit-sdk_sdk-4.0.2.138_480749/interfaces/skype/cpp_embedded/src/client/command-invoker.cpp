
#include "command-invoker.h"

CommandInvoker::CommandInvoker()
{
        ptrDictionary.insert("a", new CommandAccount());
        ptrDictionary.insert("c", new CommandConversation());
        ptrDictionary.insert("C", new CommandConversationCall());
        ptrDictionary.insert("s", new CommandContactSearch());
        ptrDictionary.insert("l", new CommandContactList());
        ptrDictionary.insert("f", new CommandFileTransfer());
        ptrDictionary.insert("M", new CommandConversationConference());
        ptrDictionary.insert("V", new CommandVoiceMail());
        ptrDictionary.insert("S", new CommandSms());
        ptrDictionary.insert("G", new CommandGreeting());
        ptrDictionary.insert("p", new CommandPrivacy());
        ptrDictionary.insert("h", new CommandConversationChat());
        ptrDictionary.insert("e", new CommandSetup());
        ptrDictionary.insert("2", new CommandApp2app());
}

CommandInvoker::~CommandInvoker()
{
        //cleanup
        List_String keys = ptrDictionary.keys();
        for (uint i = 0; i < (uint)keys.size(); i++) {
                delete (ClientCommandInterface*)ptrDictionary.value(keys[i]);              
        }        
}

void CommandInvoker::executeCommand(unsigned char cmd, unsigned char subCmd)
{
        if (cmd == '?' && subCmd == '?') {
                executeFullHelp();
                return;
        }

        char res[2];
        res[0] = cmd;
        res[1] = '\0';
        
        ClientCommandInterface* command = (ClientCommandInterface*)ptrDictionary.value(res);
        if (command == 0) {
                printf("unused command\n");
                return;
        }

        switch (subCmd) {
        case 'a' :
                command->execute_a();
                break;
        case 'b' :
                command->execute_b();
                break;
        case 'c' :
                command->execute_c();
                break;
        case 'd' :
                command->execute_d();
                break;
        case 'e' :
                command->execute_e();
                break;
        case 'f' :
                command->execute_f();
                break;
        case 'g' :
                command->execute_g();
                break;
        case 'h' :
                command->execute_h();
                break;
        case 'i' :
                command->execute_i();
                break;
        case 'j' :
                command->execute_j();
                break;
        case 'k' :
                command->execute_k();
                break;
        case 'l' :
                command->execute_l();
                break;
        case 'm' :
                command->execute_m();
                break;
        case 'n' :
                command->execute_n();
                break;
        case 'o' :
                command->execute_o();
                break;
        case 'p' :
                command->execute_p();
                break;
        case 'q' :
                command->execute_q();
                break;
        case 'r' :
                command->execute_r();
                break;
        case 's' :
                command->execute_s();
                break;
        case 't' :
                command->execute_t();
                break;
        case 'u' :
                command->execute_u();
                break;
        case 'v' :
                command->execute_v();
                break;
        case 'w' :
                command->execute_w();
                break;
        case 'x' :
                command->execute_x();
                break;
        case 'y' :
                command->execute_y();
                break;
        case 'z' :
                command->execute_z();
                break;
        case 'A' :
                command->execute_A();
                break;
        case 'B' :
                command->execute_B();
                break;
        case 'C' :
                command->execute_C();
                break;
        case 'D' :
                command->execute_D();
                break;
        case 'E' :
                command->execute_E();
                break;
        case 'F' :
                command->execute_F();
                break;
        case 'G' :
                command->execute_G();
                break;
        case 'H' :
                command->execute_H();
                break;
        case 'I' :
                command->execute_I();
                break;
        case 'J' :
                command->execute_J();
                break;
        case 'K' :
                command->execute_K();
                break;
        case 'L' :
                command->execute_L();
                break;
        case 'M' :
                command->execute_M();
                break;
        case 'N' :
                command->execute_N();
                break;
        case 'O' :
                command->execute_O();
                break;
        case 'P' :
                command->execute_P();
                break;
        case 'Q' :
                command->execute_Q();
                break;
        case 'R' :
                command->execute_R();
                break;
        case 'S' :
                command->execute_S();
                break;
        case 'T' :
                command->execute_T();
                break;
        case 'U' :
                command->execute_U();
                break;
        case 'V' :
                command->execute_V();
                break;
        case 'W' :
                command->execute_W();
                break;
        case 'X' :
                command->execute_X();
                break;
        case 'Y' :
                command->execute_Y();
                break;
        case 'Z' :
                command->execute_Z();
                break;
        case '1' :
                command->execute_1();
                break;
        case '2' :
                command->execute_2();
                break;
        case '3' :
                command->execute_3();
                break;
        case '4' :
                command->execute_4();
                break;
        case '5' :
                command->execute_5();
                break;
        case '6' :
                command->execute_6();
                break;
        case '7' :
                command->execute_7();
                break;
        case '8' :
                command->execute_8();
                break;
        case '9' :
                command->execute_9();
                break;
        case '0' :
                command->execute_0();
                break;
        case '?' :
                command->executeHelp(); //module help
                break;
        default:
                break;
        }
}

void CommandInvoker::executeFullHelp()
{
        executeHelp();
        List_String keys = ptrDictionary.keys();
        for (uint i = 0; i < (uint)keys.size(); i++) {
                ((ClientCommandInterface*)ptrDictionary.value(keys[i]))->executeHelp();
        }
}

void CommandInvoker::executeHelp(bool skypecommands)
{
        printf("\n\t? - show help\n" \
                "\tr - reconnect to skypekit\n" \
                "\t%% - show created objects\n" \
                "\tq - quit\n");
                
        if (skypecommands) {        
                printf("\n[HELP] - ?\n\t?? - full help\n");
                List_String keys = ptrDictionary.keys();
                for (uint i = 0; i < (uint)keys.size(); i++) {
                        String key = keys[i];
                        printf("\t%s? - %s help\n", (const char*)key,
                                (const char*)((ClientCommandInterface*)ptrDictionary.value(key))->getName());                
                }
        }
}

ClientCommandInterface* CommandInvoker::getCommand(String key)
{
        return (ClientCommandInterface*)ptrDictionary.value(key);
}

