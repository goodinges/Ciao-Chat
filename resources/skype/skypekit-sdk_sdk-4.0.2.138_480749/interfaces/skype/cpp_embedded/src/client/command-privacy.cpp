
#include "command-privacy.h"

void CommandPrivacy::executeHelp()
{
        printf("\n[Privacy] - p\n"
               "	pa - change avatar policy\n"
               "	pb - change contact count policy\n"
               "	pc - change chat policy\n"
               "	pi - change skypein policy\n"
               "	po - change skypeout policy\n"
               "	pz - change timezone policy\n"
               "	pv - change voicemail policy\n"
               "	pw - change web presence policy\n"
              );
}

void CommandPrivacy::execute_a() //change avatar policy
{
        Account::Ref account = skype->get_active_account();
        if (account)
                account->SetServersideIntProperty(Account::P_AVATAR_POLICY, ask_yesno("every one can see") ? Account::BUDDIES_OR_AUTHORIZED_CAN_SEE : Account::EVERYONE_CAN_SEE);
}

void CommandPrivacy::execute_b() //change contact count policy
{
        Account::Ref account = skype->get_active_account();
        const char* possibilities[] = { "authorized", "noone", "" };
        int					values[]				= { Account::DISCLOSE_TO_AUTHORIZED, Account::DISCLOSE_TO_NOONE };
        if (account)
                account->SetServersideIntProperty(Account::P_BUDDYCOUNT_POLICY, values[ask_list("show contact count to", possibilities)]);
}

void CommandPrivacy::execute_c() //change chat policy
{
        Account::Ref account = skype->get_active_account();
        if (account)
                account->SetServersideIntProperty(Account::P_CHAT_POLICY, ask_yesno("every one can add") ? Account::EVERYONE_CAN_ADD : Account::BUDDIES_OR_AUTHORIZED_CAN_ADD);
}

void CommandPrivacy::execute_i() //change skypein policy
{
        Account::Ref account = skype->get_active_account();
        const char* possibilities[] = { "all", "disclosed", "buddies", "" };
        int	values[] = { Account::ALL_NUMBERS_CAN_CALL, Account::DISCLOSED_NUMBERS_CAN_CALL, Account::BUDDY_NUMBERS_CAN_CALL };
        if (account)
                account->SetServersideIntProperty(Account::P_PSTN_CALL_POLICY, values[ask_list("who can call", possibilities)]);
}

void CommandPrivacy::execute_o() //change skypeout policy
{
        Account::Ref account = skype->get_active_account();
        const char* possibilities[] = { "buddies", "everyone", ""};
        int	values[] = { Account::PHONENUMBERS_VISIBLE_FOR_BUDDIES, Account::PHONENUMBERS_VISIBLE_FOR_EVERYONE};
        if (account)
                account->SetServersideIntProperty(Account::P_PHONENUMBERS_POLICY, values[ask_list("show number to", possibilities)]);
}

void CommandPrivacy::execute_z() //change timezone policy
{
        Account::Ref account = skype->get_active_account();
        const char* possibilities[] = { "auto", "manual", "undisclosed", "" };
        int	values[] = { Account::TZ_AUTOMATIC, Account::TZ_MANUAL, Account::TZ_UNDISCLOSED };
        if (account)
                account->SetServersideIntProperty(Account::P_TIMEZONE_POLICY, values[ask_list("time zone policy", possibilities)]);
}

void CommandPrivacy::execute_v() //change voicemail policy
{
        Account::Ref account = skype->get_active_account();
        if (account)
                account->SetServersideIntProperty(Account::P_VOICEMAIL_POLICY, ask_yesno("enable voicemail") ? Account::VOICEMAIL_DISABLED : Account::VOICEMAIL_ENABLED);
}

void CommandPrivacy::execute_w() //change web presence policy
{
        Account::Ref account = skype->get_active_account();
        if (account)
                account->SetServersideIntProperty(Account::P_WEBPRESENCE_POLICY,
                                                  ask_yesno("enable web presence") ? Account::WEBPRESENCE_DISABLED : Account::WEBPRESENCE_ENABLED);
}


