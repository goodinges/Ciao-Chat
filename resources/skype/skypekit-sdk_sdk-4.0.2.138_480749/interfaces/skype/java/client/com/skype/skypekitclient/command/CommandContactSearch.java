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
import com.skype.api.ContactSearch;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandContactSearch extends ClientCommandInterface
{
    // private static final String TAG = "CommandContactSearch";

    private SkypekitClient skClient;

    protected CommandContactSearch(SkypekitClient skypekitClient)
    {
        super();
        skClient = skypekitClient;
    }

    @Override
    public String getName()
    {
        return "ContactSearch";
    }

    public void executeHelp()
    {
        skClient.out("\n[CONTACTSEARCH] - s\n"
                + "\tsb - basic search\n"
                + "\tsi - contact search for buddy by skypename\n"
                + "\tst - contact search with terms\n");
    }

    void execute_b() // basic search
    {
        if (skClient.notLoggedIn())
            return;

        ContactSearch search = skClient.skype.CreateBasicContactSearch(Ask.ask("text"));
        if (search == null) {
            skClient.error("Unable to create contact search");
            return;
        }

        if (!search.IsValid()) {
            skClient.error("Unable to submit contact search");
        }
        else {
            search.Submit();
        }

    }

    void execute_t() // contact search with terms
    {
        if (skClient.notLoggedIn())
            return;

        ContactSearch search = skClient.skype.CreateContactSearch();
        boolean valid = search.AddStrTerm(Contact.PROPERTY.skypename.getId(), ContactSearch.CONDITION.CONTAINS_WORDS,
                Ask.ask("contains word:"), false /* TODO: what is add to subs? */);

        if (valid) {
            search.Submit();
        }
    }

    void execute_i() // contact search for buddy by skypename
    {
        if (skClient.notLoggedIn())
            return;

        ContactSearch search = skClient.skype.CreateIdentitySearch(Ask.ask("identity"));
        if (search == null) {
            skClient.error("Unable to create contact search");
            return;
        }

        if (!search.IsValid()) {
            skClient.error("Unable to submit contact search");
        }
        else {
            search.Submit();
        }

    }

}
