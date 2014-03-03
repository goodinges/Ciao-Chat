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

import com.skype.api.Contact;
import com.skype.api.ContactGroup;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandContactList extends ClientCommandInterface
{
    // private static final String TAG = "CommandContactList";

    private SkypekitClient skClient;

    protected CommandContactList(SkypekitClient skypekitClient)
    {
        super();
        skClient = skypekitClient;
    }

    @Override
    public String getName()
    {
        return "ContactList";
    }

    public void executeHelp()
    {
        skClient.out("\n[CONTACTLIST] - l\n"
                + "\tlB - list all buddies\n"
                + "\tlg - list contacts in contact groups\n"
                + "\tlr - rename a contact\n"
                + "\tlp - list contact properties\n"
                + "\tlb - block a contact\n"
                + "\tlu - unblock a contact\n"
                + "\tld - delete a	contact from list\n"
                + "\tla - report abuse\n"
                + "\tlv - store contact avatar\n"
                + "\tlR - refresh profile\n");
    }

    public void execute_p() // list contact properties
    {
        String skypename = Ask.ask("get properties from (skypename) : ");
        Contact contact = find_contact(skypename);
        if (contact == null) {
            skClient.error("Unable to get contact");
            return;
        }
        skClient.out("contact oid " + contact.getOid());

        skClient.out("\t" + "type\t\t\t" + contact.GetIntProperty(Contact.PROPERTY.type));
        skClient.out("\t" + "skypename\t\t" + contact.GetStrProperty(Contact.PROPERTY.skypename));
        skClient.out("\t" + "pstnnumber\t\t" + contact.GetStrProperty(Contact.PROPERTY.pstnnumber));
        skClient.out("\t" + "fullname\t\t" + contact.GetStrProperty(Contact.PROPERTY.fullname));
        skClient.out("\t" + "birthday\t\t" + contact.GetIntProperty(Contact.PROPERTY.birthday));
        skClient.out("\t" + "gender\t\t\t" + contact.GetIntProperty(Contact.PROPERTY.gender));
        skClient.out("\t" + "languages\t\t" + contact.GetStrProperty(Contact.PROPERTY.languages));
        skClient.out("\t" + "country\t\t\t" + contact.GetStrProperty(Contact.PROPERTY.country));
        skClient.out("\t" + "province\t\t" + contact.GetStrProperty(Contact.PROPERTY.province));
        skClient.out("\t" + "city\t\t\t" + contact.GetStrProperty(Contact.PROPERTY.city));
        skClient.out("\t" + "phone_home\t\t" + contact.GetStrProperty(Contact.PROPERTY.phone_home));
        skClient.out("\t" + "phone_office\t\t" + contact.GetStrProperty(Contact.PROPERTY.phone_office));
        skClient.out("\t" + "phone_mobile\t\t" + contact.GetStrProperty(Contact.PROPERTY.phone_mobile));
        skClient.out("\t" + "emails\t\t\t" + contact.GetStrProperty(Contact.PROPERTY.emails));
        skClient.out("\t" + "homepage\t\t" + contact.GetStrProperty(Contact.PROPERTY.homepage));
        skClient.out("\t" + "about\t\t\t" + contact.GetStrProperty(Contact.PROPERTY.about));
        skClient.out("\t" + "avatar_image\t\t" + contact.GetBinProperty(Contact.PROPERTY.avatar_image));
        skClient.out("\t" + "mood_text\t\t" + contact.GetStrProperty(Contact.PROPERTY.mood_text));
        skClient.out("\t" + "rich_mood_text\t\t" + contact.GetStrProperty(Contact.PROPERTY.rich_mood_text));
        skClient.out("\t" + "timezone\t\t" + contact.GetIntProperty(Contact.PROPERTY.timezone));
        skClient.out("\t" + "capabilities\t\t" + contact.GetBinProperty(Contact.PROPERTY.capabilities));
        skClient.out("\t" + "profile_timestamp\t" + contact.GetIntProperty(Contact.PROPERTY.profile_timestamp));
        skClient.out("\t" + "nrof_authed_buddies\t"
                + contact.GetIntProperty(Contact.PROPERTY.nrof_authed_buddies));
        skClient.out("\t" + "ipcountry\t\t" + contact.GetStrProperty(Contact.PROPERTY.ipcountry));
        skClient.out("\t" + "avatar_timestamp\t" + contact.GetIntProperty(Contact.PROPERTY.avatar_timestamp));
        skClient.out("\t" + "mood_timestamp\t\t" + contact.GetIntProperty(Contact.PROPERTY.mood_timestamp));
        skClient.out("\t" + "received_authrequest\t"
                + contact.GetStrProperty(Contact.PROPERTY.received_authrequest));
        skClient.out("\t" + "authreq_timestamp\t" + contact.GetIntProperty(Contact.PROPERTY.authreq_timestamp));
        skClient.out("\t" + "lastonline_timestamp\t"
                + contact.GetIntProperty(Contact.PROPERTY.lastonline_timestamp));
        skClient.out("\t" + "availability\t\t" + contact.GetIntProperty(Contact.PROPERTY.availability));
        skClient.out("\t" + "displayname\t\t" + contact.GetStrProperty(Contact.PROPERTY.displayname));
        skClient.out("\t" + "refreshing\t\t" + contact.GetIntProperty(Contact.PROPERTY.refreshing));
        skClient.out("\t" + "given_authlevel\t\t" + contact.GetIntProperty(Contact.PROPERTY.given_authlevel));
        skClient.out("\t" + "given_displayname\t" + contact.GetStrProperty(Contact.PROPERTY.given_displayname));
        skClient.out("\t" + "assigned_comment\t" + contact.GetStrProperty(Contact.PROPERTY.assigned_comment));
        skClient.out("\t" + "lastused_timestamp\t" + contact.GetIntProperty(Contact.PROPERTY.lastused_timestamp));
        skClient.out("\t" + "authrequest_count\t" + contact.GetIntProperty(Contact.PROPERTY.authrequest_count));
        skClient.out("\t" + "assigned_phone1\t\t" + contact.GetStrProperty(Contact.PROPERTY.assigned_phone1));
        skClient.out("\t" + "assigned_phone1_label\t"
                + contact.GetStrProperty(Contact.PROPERTY.assigned_phone1_label));
        skClient.out("\t" + "assigned_phone2\t\t" + contact.GetStrProperty(Contact.PROPERTY.assigned_phone2));
        skClient.out("\t" + "assigned_phone2_label\t"
                + contact.GetStrProperty(Contact.PROPERTY.assigned_phone2_label));
        skClient.out("\t" + "assigned_phone3\t\t" + contact.GetStrProperty(Contact.PROPERTY.assigned_phone3));
        skClient.out("\t" + "assigned_phone3_label\t"
                + contact.GetStrProperty(Contact.PROPERTY.assigned_phone3_label));
    }

    public void execute_d() // delete a contact from list
    {
        String skypename = Ask.ask("delete contact name: ");
        Contact buddy = find_contact(skypename);
        if (buddy != null) {
            ContactGroup cg = skClient.skype.GetHardwiredContactGroup(ContactGroup.TYPE.ALL_BUDDIES);
            if (cg != null)
                cg.RemoveContact(buddy);
        }
    }

    public Contact find_contact(String skypename)
    {
        if (skClient.notLoggedIn())
            return null;

        ContactGroup cg = skClient.skype.GetHardwiredContactGroup(ContactGroup.TYPE.ALL_KNOWN_CONTACTS);
        Contact[] contacts;

        if ((cg == null) || (null == (contacts = cg.GetContacts()))) {
            skClient.error("Unable to get contact list");
            return null;
        }

        for (int i = 0; i < contacts.length; i++) {
            if ((contacts[i] != null) && (skypename.equals(contacts[i].GetIdentity()))) {
                return contacts[i];
            }
        }

        return null; // No contact with matching skypename
    }

    public void execute_b() // block a contact
    {
        String skypename = Ask.ask("block name: ");
        Contact buddy = find_contact(skypename);
        if (buddy != null) {
            buddy.SetBlocked(true, false);
        }
    }

    public void execute_u() // unblock a contact
    {
        String skypename = Ask.ask("unblock name: ");
        Contact buddy = find_contact(skypename);
        if (buddy != null) {
            buddy.SetBlocked(false, false);
        }
    }

    public void execute_a() // report abuse
    {
        String skypename = Ask.ask("report abuse from: ");
        Contact buddy = find_contact(skypename);
        if (buddy != null) {
            buddy.SetBlocked(true, true);
        }
    }

    public void execute_B() // list all buddies
    {
        if (skClient.notLoggedIn())
            return;

        ContactGroup cg = skClient.skype.GetHardwiredContactGroup(ContactGroup.TYPE.ALL_BUDDIES);
        Contact[] buddies;
        if ((cg == null) || (null == (buddies = cg.GetContacts()))) {
            skClient.error("Unable to get buddies list");
            return;
        }

        skClient.out("buddies = ");

        for (int i = 0; i < buddies.length; i++) {
            Contact buddy = buddies[i];

            skClient.out(buddy.GetStrProperty(Contact.PROPERTY.displayname) + " (" + buddy.GetIdentity() + ") ["
                    + buddy.GetIntProperty(Contact.PROPERTY.availability) + "]");
        }
    }

    public void execute_r() // rename a contact
    {
        String skypename = Ask.ask("rename old name: ");
        Contact buddy = find_contact(skypename);
        if (buddy != null) {
            buddy.GiveDisplayName(Ask.ask("to newname: "));
        }
    }

    public void execute_g() // list contacts in contact groups
    {
        if (skClient.notLoggedIn())
            return;

        ContactGroup.TYPE[] cgTypes = ContactGroup.TYPE.values();
        for (int n = 0; n < cgTypes.length; n++) {
            skClient.out((n + 1) + " - " + cgTypes[n].toString());
        }

        int cnum = Ask.ask_int("select contact group (enter 1-" + cgTypes.length + "): ");

        if ((cnum == 0) || (cnum >= cgTypes.length)) {
            return;
        }

        ContactGroup cg = skClient.skype.GetHardwiredContactGroup(cgTypes[cnum - 1]);

        Contact[] contacts;
        if ((cg == null) || (null == (contacts = cg.GetContacts()))) {
            skClient.error("Unable to get contact list");
            return;
        }

        skClient.out("contacts = ");
        if (contacts.length == 0) {
            skClient.out("none");
            return;
        }

        String identity;
        for (int i = 0; i < contacts.length; i++) {
            Contact contact = contacts[i];
            identity = contact.GetIdentity();
            skClient.out((i + 1) + " - " + identity);
        }
    }

    public void execute_v() // store contact avatar
    {
        String skypename = Ask.ask("skypename: ");
        Contact buddy = find_contact(skypename);
        if (buddy != null) {
            Contact.GetAvatarResult avatar = buddy.GetAvatar();
            if ((avatar != null) && avatar.present) {
                byte[] propavatar = buddy.GetBinProperty(Contact.PROPERTY.avatar_image);
                if (propavatar != null)
                    skClient.out(skypename + " avatar size = " + avatar.avatar.length + " and same as property "
                            + avatar.avatar.equals(propavatar));

            }
            else if (avatar != null) {
                byte[] propavatar = buddy.GetBinProperty(Contact.PROPERTY.avatar_image);
                if (propavatar != null)
                    skClient.out(skypename + " has no avatar using default of size " + avatar.avatar.length
                            + " prop = " + propavatar.length);
            }
            else {
                skClient.error("Failed to get avatar for " + skypename);
            }
        }
    }

    public void execute_R() // refresh profile
    {
        String skypename = Ask.ask("skypename: ");
        Contact contact = find_contact(skypename);
        if (contact != null) {
            contact.RefreshProfile();
        }
    }

}
