
#include "command-contactlist.h"

void CommandContactList::executeHelp()
{
        printf("\n[CONTACTLIST] - l\n"
               "\tlB - list all buddies\n"
               "\tlg - list contacts in contact groups\n"
               "\tlC - add contact and send auth request\n"
               "\tlr - rename a contact\n"
               "\tlp - list contact properties\n"
               "\tlb - block a contact\n"
               "\tlu - block a contact\n"
               "\tld - delete a	contact from list\n"
               "\tla - report abuse\n"
               "\tlv - store contact avatar\n"
               "\tlR - refresh profile\n"
               "\tlK - show contact capabilities \n"
              );
        fflush(stdout);
}

void CommandContactList::execute_p()
{
        Sid::String skypename = ask("get properties from (skypename) : ");
        CContact::Ref contact = find_contact(skypename);
        if (!contact) {
        	printf("Unable to get contact\n");
                return;        
        }  
        printf("contact oid %d\n", contact->getOID());
         
        List_uint l;
        List_String n;
        l.append(Contact::P_TYPE );
        n.append("type\t\t\t");        
        l.append(Contact::P_SKYPENAME );
        n.append("skypename\t\t");
        l.append(Contact::P_PSTNNUMBER );
        n.append("pstnnumber\t\t");
        l.append(Contact::P_FULLNAME );
        n.append("fullname\t\t");
        l.append(Contact::P_BIRTHDAY );
        n.append("birthday\t\t");
        l.append(Contact::P_GENDER );
        n.append("gender\t\t\t");
        l.append(Contact::P_LANGUAGES );
        n.append("languages\t\t");
        l.append(Contact::P_COUNTRY );
        n.append("country\t\t\t");
        l.append(Contact::P_PROVINCE );
        n.append("province\t\t");
        l.append(Contact::P_CITY );
        n.append("city\t\t\t");
        l.append(Contact::P_PHONE_HOME );
        n.append("phone_home\t\t");
        l.append(Contact::P_PHONE_OFFICE );
        n.append("phone_office\t\t");
        l.append(Contact::P_PHONE_MOBILE );
        n.append("phone_mobile\t\t");
        l.append(Contact::P_EMAILS );
        n.append("emails\t\t\t");
        l.append(Contact::P_HOMEPAGE );
        n.append("homepage\t\t");
        l.append(Contact::P_ABOUT );
        n.append("about\t\t\t");
        l.append(Contact::P_AVATAR_IMAGE );
        n.append("avatar_image\t\t");
        l.append(Contact::P_MOOD_TEXT );
        n.append("mood_text\t\t");
        l.append(Contact::P_RICH_MOOD_TEXT );
        n.append("rich_mood_text\t\t");
        l.append(Contact::P_TIMEZONE );
        n.append("timezone\t\t");
        l.append(Contact::P_CAPABILITIES );
        n.append("capabilities\t\t");
        l.append(Contact::P_PROFILE_TIMESTAMP );
        n.append("profile_timestamp\t");
        l.append(Contact::P_NROF_AUTHED_BUDDIES );
        n.append("nrof_authed_buddies\t");
        l.append(Contact::P_IPCOUNTRY );
        n.append("ipcountry\t\t");
        l.append(Contact::P_AVATAR_TIMESTAMP );
        n.append("avatar_timestamp\t");
        l.append(Contact::P_MOOD_TIMESTAMP );
        n.append("mood_timestamp\t\t");
        l.append(Contact::P_RECEIVED_AUTHREQUEST );
        n.append("received_authrequest\t");
        l.append(Contact::P_AUTHREQ_TIMESTAMP );
        n.append("authreq_timestamp\t");
        l.append(Contact::P_LASTONLINE_TIMESTAMP );
        n.append("lastonline_timestamp\t");
        l.append(Contact::P_AVAILABILITY );
        n.append("availability\t\t");
        l.append(Contact::P_DISPLAYNAME );
        n.append("displayname\t\t");
        l.append(Contact::P_REFRESHING );
        n.append("refreshing\t\t");
        l.append(Contact::P_GIVEN_AUTHLEVEL );
        n.append("given_authlevel\t\t");
        l.append(Contact::P_GIVEN_DISPLAYNAME );
        n.append("given_displayname\t");
        l.append(Contact::P_ASSIGNED_COMMENT );
        n.append("assigned_comment\t");
        l.append(Contact::P_LASTUSED_TIMESTAMP );
        n.append("lastused_timestamp\t");
        l.append(Contact::P_AUTHREQUEST_COUNT );
        n.append("authrequest_count\t");
        l.append(Contact::P_ASSIGNED_PHONE1 );
        n.append("assigned_phone1\t\t");
        l.append(Contact::P_ASSIGNED_PHONE1_LABEL );
        n.append("assigned_phone1_label\t");
        l.append(Contact::P_ASSIGNED_PHONE2 );
        n.append("assigned_phone2\t\t");
        l.append(Contact::P_ASSIGNED_PHONE2_LABEL );
        n.append("assigned_phone2_label\t");
        l.append(Contact::P_ASSIGNED_PHONE3 );
        n.append("assigned_phone3\t\t");
        l.append(Contact::P_ASSIGNED_PHONE3_LABEL );
        n.append("assigned_phone3_label\t");
        
        contact->FetchProps(l);
        Dict_int d = contact->GetProps(l);
        
        for (uint i = 0; i < (uint)l.size(); i++) {
                String value = d.value(l[i]);
                List_String dbg = contact->getPropDebug( l[i], value );
                
                printf("	%s%s\n",(const char*)n[i], (const char*)dbg[2]);
        }
}

void CommandContactList::execute_C()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                return;
        }

        CContact::Ref contact;
        if (!skype->GetContact(ask("enter skypename:"), contact)) {
            printf("Cannot get contact\n");
            return;
        }
        if (!contact->SetBuddyStatus(true, true)) {
            printf("Cannot set buddy status\n");
            return;
        }
        if (!contact->SendAuthRequest(ask("enter auth request string:")))
            printf("Cannot send authorization request\n");
}

void CommandContactList::execute_d()
{
        Sid::String skypename = ask("delete contact name: ");
        CContact::Ref buddy = find_contact(skypename);
        if (buddy) {
                ContactGroup::Ref cg;
                bool yes = false;
                skype->GetHardwiredContactGroup(ContactGroup::ALL_BUDDIES,cg);
                if (!cg->CanRemoveContact(yes) || !yes 
                                || !cg->RemoveContact(buddy)) {
                        printf("Can't remove\n");
                        fflush(stdout);
                }
        }
}

Contact::Ref CommandContactList::find_contact(Sid::String& skypename)
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                return Contact::Ref(0);
        }
        
        ContactGroup::Ref cg;
        Contact::Refs contacts;
        if (!skype->GetHardwiredContactGroup(ContactGroup::ALL_KNOWN_CONTACTS, cg) || !cg->GetContacts(contacts)) {
                printf("Unable to get contact list\n");
                return Contact::Ref(0);
        }
        
        Sid::String identity; 
        for (uint i = 0; i < (uint)contacts.size(); i++) {
                contacts[i]->GetIdentity(identity);
                if (contacts[i] && identity == skypename) {
                        return contacts[i];
                }
        }
        
        return Contact::Ref(0);
}

void CommandContactList::execute_b()
{
        Sid::String skypename = ask("block name: ");
        CContact::Ref buddy = find_contact(skypename);
        if (buddy && !buddy->SetBlocked(true))
                printf("Can't block %s in buddies\n", (const char*)buddy->GetStrProp(Contact::P_DISPLAYNAME));
        fflush(stdout);
}

void CommandContactList::execute_u()
{
        Sid::String skypename = ask("unblock name: ");
        CContact::Ref buddy = find_contact(skypename);
        if (buddy && !buddy->SetBlocked(false))
                printf("Can't unblock %s in buddies\n", (const char*)buddy->GetStrProp(Contact::P_DISPLAYNAME));
        fflush(stdout);
}

void CommandContactList::execute_a()
{
        Sid::String skypename = ask("report abuse from: ");
        CContact::Ref buddy = find_contact(skypename);        
        if (buddy && !buddy->SetBlocked(true,true))
                printf("Can't mark %s in buddies as abusing\n", (const char*)buddy->GetStrProp(Contact::P_DISPLAYNAME));
        fflush(stdout);
}

void CommandContactList::execute_B()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }

        ContactGroup::Ref cg;
        Contact::Refs buddies;
        if (!skype->GetHardwiredContactGroup(ContactGroup::ALL_BUDDIES,cg) || !cg->GetContacts(buddies)) {
                printf("Unable to get buddies list\n");
                fflush(stdout);
                return;
        }
        printf("buddies = \n");
        
        List_uint props;
        props.append(Contact::P_DISPLAYNAME);
        props.append(Contact::P_AVAILABILITY);
        skype->FetchMultipleObjectsProps(Contact::MODULE_ID, buddies, props);        
        
        for (uint i = 0; i < (uint)buddies.size(); i++) {
                CContact::Ref buddy = buddies[i];
                Dict_int d = buddy->GetProps(props);
                
                String value = d.value(Contact::P_AVAILABILITY);
                List_String dbg = buddy->getPropDebug(Contact::P_AVAILABILITY, value);
                
                String dname = d.value(Contact::P_DISPLAYNAME);
                printf("%s (%s) [%s]\n",
                       (const char*)dname,
                       (const char*)buddy->skypeName,
                       (const char*)dbg[2]);
        }
        fflush(stdout);
}

void CommandContactList::execute_r()
{
        Sid::String skypename = ask("rename old name: ");
        CContact::Ref buddy = find_contact(skypename);
        if (buddy) {
                buddy->GiveDisplayName(ask("to newname: "));
        }
}

void CommandContactList::execute_g()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                fflush(stdout);
                return;
        }
        
        int n = 0;
        while (CONTACTGROUP_TYPE_MAP[n].prop) {
                printf("%d - %s\n", n, CONTACTGROUP_TYPE_MAP[n].prop);
                n++;
        }
        
        char* txt = new char[sizeof("select contact group (enter 1-%d): ")];
        sprintf(txt,"select contact group (enter 1-%d): ", n);
        int cnum = ask_int(txt);
        delete [] txt;
        
        if (cnum >= n) {
                return;
        }
        
        ContactGroup::Ref cg;
        Contact::Refs contacts;
        if (!skype->GetHardwiredContactGroup((ContactGroup::TYPE)CONTACTGROUP_TYPE_MAP[cnum].key, cg) || !cg->GetContacts(contacts)) {
                printf("Unable to get contact list\n");
                return;
        }
        
        printf("contacts = \n");
        if (!contacts.size()) {
                printf("none\n");
                return;
        }
    
        String identity; 
        for (uint i = 0; i < (uint)contacts.size(); i++) {
                CContact::Ref contact = contacts[i];
                contact->GetIdentity(identity);
                printf("%d - %s\n", i+1, (const char*)identity); 
        }       
}


void CommandContactList::execute_v()
{
        Sid::String skypename = ask("skypename: ");
        CContact::Ref buddy = find_contact(skypename);
        if (buddy) {
              Sid::Binary avatar;
              bool present;
              if (buddy->GetAvatar(present, avatar) && present) {
                      Sid::Binary propavatar;
                      buddy->GetPropAvatarImage(propavatar);
                      printf("%s avatar size = %d and same as property %d\n", 
                        (const char*)skypename, (int) avatar.size(), avatar == propavatar); 
                     
              } else {
                     Sid::Binary propavatar;
                     buddy->GetPropAvatarImage(propavatar);
                     printf("%s has no avatar using default of size %d, prop = %d\n", 
                        (const char*)skypename, (int) avatar.size(), (int) propavatar.size()); 
              }
        }
}

void CommandContactList::execute_R() 
{
        Sid::String skypename = ask("skypename: ");
        CContact::Ref contact = find_contact(skypename);
        if (contact) {
                contact->RefreshProfile();        
        }
}

void CommandContactList::execute_K()
{
        Sid::String skypename = ask("get capabilities from (skypename) : ");
        CContact::Ref contact = find_contact(skypename);
        if (!contact) {
                printf("Unable to get contact\n");
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
                "FIRST_EXPIRY_WARNING"  /** <30 days*/,
                "SECOND_EXPIRY_WARNING" /** <7 days*/,
                "FINAL_EXPIRY_WARNING"  /** <1 day*/
        };
        
        Contact::CAPABILITYSTATUS s;
        uint expiry_timestamp = 0;
        for (int c = 0; c <= Contact::CAPABILITY_PUBLIC_CONTACT; c++) {
                if (!contact->GetCapabilityStatus((Contact::CAPABILITY) c, s, expiry_timestamp == 1)) {
                        printf("error when getting capability\n");
                        fflush(stdout);
                        return;
                }
                if (s > Contact::CAPABILITY_EXISTS)
                        printf("capability %s status is %s (expires on %d)\n", capabilities[c], capability_status[s], expiry_timestamp);
                else
                        printf("capability %s status is %s\n", capabilities[c], capability_status[s]);
                
                fflush(stdout);
        }
}







