
#include "command-account.h"

void CommandAccount::executeHelp()
{
        printf("\n[ACCOUNT] - a\n" \
               "\taL - login (with password)\n"
               "\taI - login (without password)\n"
               "\taD - login (with default account)\n"
               "\taR - register\n"
               "\taO - logout\n"
               "\taK - show own capabilities \n"
               "\tap - change presence \n"
               "\taP - change password (don't save password)\n"
               "\tan - change name\n"
               "\tam - change mood\n"
               "\tae - change email\n"
               "\tah - change home phone\n"
               "\tao - change office phone\n"
               "\tac - change cell phone\n"
               "\tab - change birthday\n"
               "\tag - change gender\n"
               "\tal - change language\n"
               "\taC - change country\n"
               "\tar - change region\n"
               "\tat - change city\n"
               "\taw - change homepage\n"
               "\taa - change about\n"
               "\taz - change timezone\n"
               "\taF - set call forwarding\n"
               "\taA - view authorization requests\n"
               "\taE - view existing accounts\n"
               "\taV - validate skypename\n"
               "\ta1 - set avatar image\n"
              );
        fflush(stdout);
}

const Contact::AVAILABILITY CommandAccount::availabilities[] = { Contact::INVISIBLE, Contact::DO_NOT_DISTURB, Contact::ONLINE, Contact::OFFLINE, Contact::AWAY };
const char* CommandAccount::availabilities_as_text[] = { "INVISIBLE", "DO NOT DISTURB", "ONLINE", "OFFLINE", "AWAY", "" };

void CommandAccount::execute_p()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }

        availability = ask_list("set presence to", availabilities_as_text);
        if (!skype->activeAccount->SetAvailability(availabilities[availability])) {
                printf("Unable to SetAvailability to %s\n",availabilities_as_text[availability]);
                fflush(stdout);
        } else {
                printf("Changed Availability to %s\n",availabilities_as_text[availability]);
                fflush(stdout);
        }
}

void CommandAccount::execute_L()
{
        if (!skype->loggedOut()) {
                printf("You have to log out first\n");
                fflush(stdout);
                return;
        }

        if (!skype->GetAccount(ask("enter user name:"), skype->activeAccount)) {
                printf("Unable to get account\n");
                fflush(stdout);
                return;
        }

        if (!skype->activeAccount->LoginWithPassword(ask("enter password:"), (ask_yesno("save password") == 0) )) {
                printf("Unable to login\n");
                fflush(stdout);
        }
}

void CommandAccount::execute_D() // login with default account
{
        if (!skype->loggedOut()) {
                printf("You have to log out first\n");
                fflush(stdout);
                return;
        }

        String accname;
        if (!skype->GetDefaultAccountName(accname) || !skype->GetAccount(accname, skype->activeAccount)) {
                printf("Unable to get account\n");
                fflush(stdout);
                return;
        }
        
        if (skype->activeAccount->getStatus() != Account::LOGGED_OUT_AND_PWD_SAVED)
        {
                printf("Password was not saved for given account: unable to login, use aL"); 
                return;
        }        
        
        if (!skype->activeAccount->Login(Contact::ONLINE)) {
                printf("Unable to login\n");
                fflush(stdout);
        }
}

void CommandAccount::execute_R() // register
{
        if (!skype->loggedOut()) {
                printf("You have to log out first\n");
                fflush(stdout);
                return;
        }

        if (!skype->GetAccount(ask("enter user name:"), skype->activeAccount)) {
                printf("Unable to get account\n");
                fflush(stdout);
                return;
        }

        if (!skype->activeAccount->Register(ask("password:"), (ask_yesno("save password") == 0), true,ask("email:"),false)) {
                printf("Unable to register\n");
                fflush(stdout);
        }
}


void CommandAccount::execute_I()
{
        if (!skype->loggedOut()) {
                printf("You have to log out first\n");
                fflush(stdout);
                return;
        }

        if (!skype->GetAccount(ask("enter user name:"), skype->activeAccount)) {
                printf("Unable to get account\n");
                fflush(stdout);
                return;
        }
        
        if (skype->activeAccount->getStatus() != Account::LOGGED_OUT_AND_PWD_SAVED)
        {
                printf("Password was not saved for given account: unable to login, use aL"); 
                return;
        }
        
        if (!skype->activeAccount->Login(Contact::ONLINE)) {
                printf("Unable to login\n");
                fflush(stdout);
        }
}

void CommandAccount::execute_O()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }

        if (!skype->activeAccount->Logout((ask_yesno("clear password") == 0))) {
                printf("Unable to logout\n");
                fflush(stdout);
        }
}

void CommandAccount::execute_P()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        String oldp = ask("enter old password:");
        String newp = ask("enter new password:");
        String newp2 = ask("enter new password:");
        if (newp == newp2)
                skype->activeAccount->ChangePassword(oldp,newp,false);
}

void CommandAccount::execute_K()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }

        const char* capabilities[] = {
                "VOICEMAIL",
                "SKYPEOUT",
                "SKYPEIN",
                "CAN_BE_SENT_VM",
                "CALL_FORWARD",
                "VIDEO",
                "TEXT",
                "SERVICE_PROVIDER",
                "LARGE_CONFERENCE",
                "COMMERCIAL_CONTACT",
                "PSTN_TRANSFER",
                "TEXT_EVER",
                "VOICE_EVER",
                "MOBILE_DEVICE",
                "PUBLIC_CONTACT"
        };
        const char* capability_status[] = {
                "NOT AVAILABLE",
                "MIXED",
                "EXISTS",
                "FIRST_EXPIRY_WARNING"	/** <30 days*/,
                "SECOND_EXPIRY_WARNING"	/** <7 days*/,
                "FINAL_EXPIRY_WARNING"	/** <1 day*/
        };
        
        Account::CAPABILITYSTATUS s;
        uint expiry_timestamp;
        for (int c = 0; c <= Contact::CAPABILITY_PUBLIC_CONTACT; c++) {
                if (!skype->activeAccount->GetCapabilityStatus((Contact::CAPABILITY) c, s, expiry_timestamp)) {
                        printf("error when getting capability\n");
                        fflush(stdout);
                        return;
                }
                if (s > Account::CAPABILITY_EXISTS)
                        printf("capability %s status is %s (expires on %d)\n", capabilities[c], capability_status[s], expiry_timestamp);
                else
                        printf("capability %s status is %s\n", capabilities[c], capability_status[s]);
                
                fflush(stdout);
        }
}

void CommandAccount::execute_n() // change name
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_FULLNAME, ask("full name: "));
}

void CommandAccount::execute_m() // change mood
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_MOOD_TEXT, ask("mood: "));
}

void CommandAccount::execute_e() // change email
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_EMAILS, ask("email: "));
}

void CommandAccount::execute_h() // change home phone
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_PHONE_HOME, ask("phone at home: "));
}

void CommandAccount::execute_o() // change office phone
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_PHONE_OFFICE, ask("phone at office: "));
}

void CommandAccount::execute_c() // change cell phone
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_PHONE_MOBILE, ask("cell phone: "));
}

void CommandAccount::execute_b() // change birthday
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        String bday = ask("year (YYYY) : ") + ask("month (MM): ") + ask("day (DD): ");        
        skype->activeAccount->SetIntProperty(Account::P_BIRTHDAY, atoi((const char*)bday));
}

void CommandAccount::execute_g() // change gender
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        const char* possibilities[] = { "male", "female", "" };
        skype->activeAccount->SetIntProperty(Account::P_GENDER, ask_list("gender",possibilities)+1);
}

void CommandAccount::execute_l() // change language
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_LANGUAGES, ask("languages: "));
}

void CommandAccount::execute_C() // change country
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_COUNTRY, ask("country: "));
}

void CommandAccount::execute_r() // change region
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_PROVINCE, ask("region: "));
}

void CommandAccount::execute_t() // change city
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_CITY, ask("city: "));
}

void CommandAccount::execute_w() // change homepage
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_HOMEPAGE, ask("homepage: "));
}

void CommandAccount::execute_a() // change about
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetStrProperty(Account::P_ABOUT, ask("about: "));
}

void CommandAccount::execute_z() // change timezone
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetIntProperty(Account::P_TIMEZONE, ask_int("timezone: "));
}

void CommandAccount::execute_F() // set call forwarding
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        skype->activeAccount->SetServersideStrProperty(Account::P_OFFLINE_CALLFORWARD,	ask("forward to: (beginSecond, endSecond, identity)"));
}

void CommandAccount::execute_A() // view auth requests
{
        ContactGroup::Ref cg;
        Contact::Refs authrequests;
        if (!skype->GetHardwiredContactGroup(ContactGroup::CONTACTS_WAITING_MY_AUTHORIZATION, cg) || !cg->GetContacts(authrequests)) {
                printf("Unable to get auth requests list\n");
                fflush(stdout);
                return;
        }

        if (!authrequests.size()) {
                printf("Auth requests list is empty\n");
                fflush(stdout);
                return;
        }

        for (uint i = 0; i < (uint)authrequests.size(); i++) {
                CContact::Ref contact = authrequests[i];
                
                char* txt = new char[sizeof("New authorization request from %s, authorize?") + contact->skypeName.size()];
                sprintf(txt,"New authorization request from %s, authorize?", (const char*)contact->skypeName);
        
                const char* possibilities[] = { "ignore", "add", "block", ""}; 
               	switch (ask_list(txt, possibilities)) {
                case 0:
                        contact->IgnoreAuthRequest();
                        break;
                case 1:
                        contact->SetBuddyStatus(true, true); 
                        break;
                case 2:
                	contact->SetBlocked(true, false);
                	break;        
                default:
                        break;
                }
                                
                delete [] txt;                 
        }
}

void CommandAccount::execute_E() // view existing accounts 
{
    Sid::List_String names;

    if (skype->GetExistingAccounts(names)) {

        if (names.size() != 0)
            printf("Existing accounts:\n");
        else
            printf("No existing accounts\n");

        for (unsigned int i = 0; i < (uint)names.size(); i++)
            printf("%d %s\n", i+1, (const char*)names[i]);

    }
}

void CommandAccount::execute_V()
{
    int free;
    Skype::VALIDATERESULT r;
    const char *validateresults[] = {
        "NOT_VALIDATED",
        "VALIDATED_OK", 
        "TOO_SHORT", 
        "TOO_LONG", 
        "CONTAINS_INVALID_CHAR", 
        "CONTAINS_SPACE",
        "SAME_AS_USERNAME", 
        "INVALID_FORMAT", 
        "CONTAINS_INVALID_WORD", 
        "TOO_SIMPLE", 
        "STARTS_WITH_INVALID_CHAR"
    };
    skype->ValidateProfileString(Contact::P_SKYPENAME, ask("Enter skypename to validate:"), r, free, (ask_yesno("Validate for registration") == 0)); 
    if (r > Skype::STARTS_WITH_INVALID_CHAR)
        printf("result: %d free bytes: %d\n", r, free);
    else
        printf("result: %s free bytes: %d\n", validateresults[r], free);
    fflush(stdout);
}

void CommandAccount::execute_1()
{
    FILE* f = 0;
    size_t fsize = 0;
    f = fopen(ask("Enter file name:"), "rb");
    SEBinary Img;
    if (f != 0)
    {
        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        rewind(f);
        Img.reserve(fsize);
        size_t read = fread(Img, 1, fsize, f);
	if (read != fsize) { printf("error reading file\n");}
        fclose(f);
    } else {
        printf("Cannot open file\n");
        return;
    }
    int bytesLeft;
    Skype::VALIDATERESULT result;
    if (skype->ValidateAvatar(Img, result, bytesLeft) && result == Skype::VALIDATED_OK)
    {
        if (skype->activeAccount->SetBinProperty(Account::P_AVATAR_IMAGE, Img))
        {
            printf("Avatar is set successfully\n");
            return;
        } else {
            printf("Avatar is not set\n");
            return;
        }
    }
    printf("Avatar validation failure\n");
}

