/**
 * Copyright (C) 2010, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

package com.skype.skypekitclient.command;

import com.skype.api.Account;
import com.skype.api.Contact;
import com.skype.api.Account.GetCapabilityStatusResult;
import com.skype.api.Contact.AVAILABILITY;
import com.skype.api.ContactGroup;
import com.skype.api.Skype;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandAccount extends ClientCommandInterface
{
    // private static final String TAG = "CommandAccount";

    private SkypekitClient skClient;

    protected CommandAccount(SkypekitClient skypekitClient)
    {
        super();
        skClient = skypekitClient;
    }

    @Override
    public String getName()
    {
        return "Account";
    }

    @Override
    public void executeHelp()
    {
        skClient.out("\n[ACCOUNT] - a\n" +
                "\taL - login (with password)\n" +
                "\taI - login (without password)\n" +
                "\taD - login (with default account)\n" +
                "\taR - register\n" +
                "\taO - logout\n" +
                "\taK - show own capabilities \n" +
                "\tap - change presence \n" +
                "\taP - change password (don't save password)\n" +
                "\tan - change name\n" +
                "\tam - change mood\n" +
                "\tae - change email\n" +
                "\tah - change home phone\n" +
                "\tao - change office phone\n" +
                "\tac - change cell phone\n" +
                "\tab - change birthday\n" +
                "\tag - change gender\n" +
                "\tal - change language\n" +
                "\taC - change country\n" +
                "\tar - change region\n" +
                "\tat - change city\n" +
                "\taw - change homepage\n" +
                "\taa - change about\n" +
                "\taz - change timezone\n" +
                "\taF - set call forwarding\n" +
                "\taA - view authorization requests\n" +
                "\taE - view existing accounts\n" +
                "\taM - view account profile\n" +
                "\taV - validate skypename\n"
                );
    }

    public AVAILABILITY availabilities[]         = { AVAILABILITY.INVISIBLE, AVAILABILITY.DO_NOT_DISTURB,
            AVAILABILITY.ONLINE, AVAILABILITY.OFFLINE, AVAILABILITY.AWAY };
    public String       availabilities_as_text[] = { "INVISIBLE", "DO NOT DISTURB", "ONLINE", "OFFLINE", "AWAY" };

    @Override
    public void execute_p() // change presence
    {
        if (skClient.notLoggedIn())
            return;

        int availability = Ask.ask_list("set presence to", availabilities_as_text);
        skClient.account.SetAvailability(availabilities[availability]);

        skClient.out("Changed Availability to " + availabilities_as_text[availability]);
    }

    @Override
    public void execute_L() // Login (with password)
    {
        if (skClient.isLoggedIn()) {
            skClient.error("You have to log out first");
            return;
        }

        skClient.account = skClient.skype.GetAccount(Ask.ask("enter user name:"));
        if (skClient.account == null) {
            skClient.error("Unable to get account\n");
            return;
        }

        skClient.account.LoginWithPassword(
                Ask.ask("enter password:"),
                Ask.ask_yesno("save password"), true);
    }

    @Override
    public void execute_D() // login with default account
    {
        if (skClient.isLoggedIn()) {
            skClient.error("You have to log out first\n");
            return;
        }

        // Get most recently used account that has pwd saved, empty string if
        // none
        String accname = skClient.skype.GetDefaultAccountName();
        if (accname.length() == 0) {
            skClient.error("No default account name with saved password, use aL command to login manually");
        }
        else if (null != (skClient.account = skClient.skype.GetAccount(accname))) {
            skClient.error("Unable to get default account, use aL command to login manually");
        }
        else {
            skClient.out("Logging in with default account: " + accname);
            skClient.account.Login(Contact.AVAILABILITY.ONLINE);
        }
    }

    @Override
    public void execute_R() // register new Skype account
    {
        if (skClient.isLoggedIn()) {
            skClient.error("You have to log out first\n");
            return;
        }

        skClient.account = skClient.skype.GetAccount(Ask.ask("enter user name:"));

        skClient.account.Register(
                Ask.ask("password:"),
                Ask.ask_yesno("save password"),
                true,
                Ask.ask("email:"),
                false);
    }

    @Override
    public void execute_I() // login (without password)
    {
        if (skClient.isLoggedIn()) {
            skClient.error("You have to log out first\n");
            return;
        }

        skClient.account = skClient.skype.GetAccount(Ask.ask("enter user name:"));

        if (skClient.account.GetStatusWithProgress().status != Account.STATUS.LOGGED_OUT_AND_PWD_SAVED) {
            skClient.error("Password was not saved for given account: unable to login, use aL");
            return;
        }

        skClient.out("Logging in ");
        skClient.account.Login(Contact.AVAILABILITY.ONLINE);
    }

    @Override
    public void execute_O()
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.Logout(Ask.ask_yesno("clear password"));
    }

    @Override
    public void execute_P()
    {
        if (skClient.notLoggedIn())
            return;

        String oldp = Ask.ask("enter old password:");
        String newp = Ask.ask("enter new password:");
        String newp2 = Ask.ask("re-enter new password:");
        if (newp.equals(newp2))
            skClient.account.ChangePassword(oldp, newp, false);
    }

    @Override
    public void execute_K()
    {
        if (skClient.notLoggedIn())
            return;

        String capabilities[] = {
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
        String capability_status[] = {
                "NOT AVAILABLE",
                "MIXED",
                "EXISTS",
                "FIRST_EXPIRY_WARNING"/** <30 days */
                ,
                "SECOND_EXPIRY_WARNING"/** <7 days */
                ,
                "FINAL_EXPIRY_WARNING" /** <1 day */
        };

        for (Contact.CAPABILITY c : Contact.CAPABILITY.values()) {
            GetCapabilityStatusResult result = skClient.account.GetCapabilityStatus(c);
            if (result.status.equals(Account.CAPABILITYSTATUS.CAPABILITY_EXISTS))
                skClient.out("Capability " + capabilities[c.getId()] +
                        " status is " + capability_status[result.status.getId()] +
                        " (expires on " + result.expiryTimestamp + ")");
            else
                skClient.out("Capability " + capabilities[c.getId()] +
                        " status is " + capability_status[result.status.getId()]);
        }
    }

    @Override
    public void execute_n() // change name
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.fullname.getId(), Ask.ask("full name: "));
    }

    @Override
    public void execute_m() // change mood
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.mood_text.getId(), Ask.ask("mood: "));
    }

    @Override
    public void execute_e() // change email
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.emails.getId(), Ask.ask("email: "));
    }

    @Override
    public void execute_h() // change home phone
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.phone_home.getId(), Ask.ask("phone at home: "));
    }

    @Override
    public void execute_o() // change office phone
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.phone_office.getId(), Ask.ask("phone at office: "));
    }

    @Override
    public void execute_c() // change cell phone
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.phone_mobile.getId(), Ask.ask("cell phone: "));
    }

    @Override
    public void execute_b() // change birthday
    {
        if (skClient.notLoggedIn())
            return;

        String bday = Ask.ask("year (YYYY) : ") + Ask.ask("month (MM): ") + Ask.ask("day (DD): ");
        skClient.account.SetIntProperty(Account.PROPERTY.birthday.getId(), Integer.valueOf(bday));
    }

    @Override
    public void execute_g() // change gender
    {
        if (skClient.notLoggedIn())
            return;

        String possibilities[] = { "male", "female" };
        skClient.account.SetIntProperty(Account.PROPERTY.gender.getId(), Ask.ask_list("gender", possibilities) + 1);
    }

    @Override
    public void execute_l() // change language
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.languages.getId(), Ask.ask("languages: "));
    }

    @Override
    public void execute_C() // change country
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.country.getId(), Ask.ask("country: "));
    }

    @Override
    public void execute_r() // change region
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.province.getId(), Ask.ask("region: "));
    }

    @Override
    public void execute_t() // change city
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.city.getId(), Ask.ask("city: "));
    }

    @Override
    public void execute_w() // change homepage
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.homepage.getId(), Ask.ask("homepage: "));
    }

    @Override
    public void execute_a() // change about
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.about.getId(), Ask.ask("about: "));
    }

    @Override
    public void execute_z() // change timezone
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetStrProperty(Account.PROPERTY.timezone.getId(), Ask.ask("timezone: "));
    }

    @Override
    public void execute_F() // set call forwarding
    {
        if (skClient.notLoggedIn())
            return;

        skClient.account.SetServersideStrProperty(Account.PROPERTY.offline_callforward.getId(),
                Ask.ask("forward to: (beginSecond, endSecond, identity)"));
    }

    @Override
    public void execute_A() // view auth requests
    {
        ContactGroup cg;
        Contact[] authrequests;

        cg = skClient.skype.GetHardwiredContactGroup(ContactGroup.TYPE.CONTACTS_WAITING_MY_AUTHORIZATION);

        if ((cg == null) || (null == (authrequests = cg.GetContacts()))) {
            skClient.error("Unable to get auth requests list\n");
            return;
        }

        if (authrequests.length == 0) {
            skClient.out("Auth requests list is empty\n");
            return;
        }

        for (int i = 0; i < authrequests.length; i++) {
            Contact contact = authrequests[i];

            String txt = new String("New authorization request from " + contact.GetIdentity() + " authorize?");
            String possibilities[] = { "ignore", "add", "block" };
            switch (Ask.ask_list(txt, possibilities)) {
            case 0:
                contact.IgnoreAuthRequest();
                break;
            case 1:
                contact.SetBuddyStatus(true, true);
                break;
            case 2:
                contact.SetBlocked(true, false);
                break;
            default:
                break;
            }
        }
    }

    @Override
    public void execute_E() // view existing accounts
    {
        String[] names = skClient.skype.GetExistingAccounts();

        if (names.length > 0)
            skClient.out("Existing accounts:\n");
        else
            skClient.out("No existing accounts\n");

        for (int i = 0; i < names.length; i++)
            skClient.out((i + 1) + names[i]);
    }

    public void execute_M() // view account profile
    {
        if (skClient.notLoggedIn())
            return;

        skClient.out("\t" + "skypename\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.skypename));
        skClient.out("\t" + "fullname\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.fullname));
        skClient.out("\t" + "birthday\t\t" + skClient.account.GetIntProperty(Account.PROPERTY.birthday));
        skClient.out("\t" + "gender\t\t\t" + skClient.account.GetIntProperty(Account.PROPERTY.gender));
        skClient.out("\t" + "languages\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.languages));
        skClient.out("\t" + "country\t\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.country));
        skClient.out("\t" + "province\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.province));
        skClient.out("\t" + "city\t\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.city));
        skClient.out("\t" + "phone_home\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.phone_home));
        skClient.out("\t" + "phone_office\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.phone_office));
        skClient.out("\t" + "phone_mobile\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.phone_mobile));
        skClient.out("\t" + "emails\t\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.emails));
        skClient.out("\t" + "homepage\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.homepage));
        skClient.out("\t" + "about\t\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.about));
        skClient.out("\t" + "avatar_image\t\t" + skClient.account.GetBinProperty(Account.PROPERTY.avatar_image));
        skClient.out("\t" + "mood_text\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.mood_text));
        skClient.out("\t" + "rich_mood_text\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.rich_mood_text));
        skClient.out("\t" + "timezone\t\t" + skClient.account.GetIntProperty(Account.PROPERTY.timezone));
        skClient.out("\t" + "profile_timestamp\t" + skClient.account.GetIntProperty(Account.PROPERTY.profile_timestamp));
        skClient.out("\t" + "nrof_authed_buddies\t"
                + skClient.account.GetIntProperty(Account.PROPERTY.nrof_authed_buddies));
        skClient.out("\t" + "ipcountry\t\t" + skClient.account.GetStrProperty(Account.PROPERTY.country));
        skClient.out("\t" + "avatar_timestamp\t" + skClient.account.GetIntProperty(Account.PROPERTY.avatar_timestamp));
        skClient.out("\t" + "mood_timestamp\t\t" + skClient.account.GetIntProperty(Account.PROPERTY.mood_timestamp));
        skClient.out("\t" + "availability\t\t" + skClient.account.GetIntProperty(Account.PROPERTY.availability));
    }

    public void execute_V()
    {
        Skype.ValidateProfileStringResult r;
        String validateresults[] = {
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
        r = skClient.skype.ValidateProfileString(Contact.PROPERTY.skypename.getId(),
                Ask.ask("Enter skypename to validate:"),
                Ask.ask_yesno("Validate for registration"));

        if (r.result.getId() > Skype.VALIDATERESULT.STARTS_WITH_INVALID_CHAR.getId())
            skClient.out("result: " + r.result + " free bytes: " + r.freeBytesLeft);
        else
            skClient.out("result: " + validateresults[r.result.getId()] + " free bytes: " + r.freeBytesLeft);
    }

}
