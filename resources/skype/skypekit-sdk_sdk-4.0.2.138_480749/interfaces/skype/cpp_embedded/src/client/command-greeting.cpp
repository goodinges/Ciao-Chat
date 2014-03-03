
#include "command-greeting.h"

void CommandGreeting::executeHelp()
{
        printf("\n[Greeting] - G\n"
               "\tp - play\n"
               "\tr - start record\n"
               "\tr - stop record\n"
               "\tz - reset\n"
              );
}

void CommandGreeting::execute_p()
{
        Voicemail::Ref greeting;
        if (!skype->GetGreeting(ask("skypename: "),greeting)) {
                printf("no greeting message");
                return;
        }
        greeting->StartPlayback();
}

void CommandGreeting::execute_r()
{
        Account::Ref account = skype->get_active_account();
        CVoicemail::Ref greeting;
        if (account) {
                if (!skype->GetGreeting(account->GetStrProp(Account::P_SKYPENAME), greeting) || !greeting) {
                        printf("no greeting message");
                        return;
                }
                
                greeting->StartRecording();
        }
}

void CommandGreeting::execute_s()
{
        Account::Ref account = skype->get_active_account();
        CVoicemail::Ref greeting;
        if (account) {
                if (!skype->GetGreeting(account->GetStrProp(Account::P_SKYPENAME), greeting) || !greeting) {
                        printf("no greeting message");
                        return;
                }
                
                Voicemail::STATUS status; 
                greeting->GetPropStatus(status); 
                if (status == Voicemail::RECORDING) {
                        greeting->StopRecording();
                } else {
                        printf("not recording\n");       
                }
        }
}

void CommandGreeting::execute_z()
{
        Account::Ref account = skype->get_active_account();
        Voicemail::Ref greeting;
        if (account) {
                if (!skype->GetGreeting(account->GetStrProp(Account::P_SKYPENAME), greeting) || !greeting) {
                        printf("no greeting message");
                        return;
                }
                greeting->Delete();
        }
}

