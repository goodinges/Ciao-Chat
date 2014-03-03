package com.skype.skypekitclient.utils;

import java.util.Scanner;

public class Ask
{
    private static Scanner scan;

    public static String ask(String question)
    {
        scan = new Scanner(System.in);

        System.out.println("\n::: " + question);
        return scan.nextLine();
    }

    public static int ask_list(String question, String[] possibleResponses)
    {
        assert(possibleResponses.length > 0);
        String queryString = question + " (";
        for (int poss = 0, last = possibleResponses.length; poss < last; poss++) {
            queryString += poss + "=" + possibleResponses[poss];
            queryString += (poss == last - 1) ? " )" : ", ";
        }

        int response = ask_int(queryString);
        if (response < 1 || response > possibleResponses.length)
            return 0;
        else
            return response;
    }

    // Ask Yes/No question: if response in no returns false (No), otherwise
    // returns true (Yes)
    public static boolean ask_yesno(String question)
    {
        String ynResponse = ask(question + " (Y/n)? ");

        String[] no = { "n", "no", "non", "ei", "Nein", "" };
        for (int how_to_say_no = 0; !no[how_to_say_no].equals(""); how_to_say_no++)
            if (ynResponse.equalsIgnoreCase(no[how_to_say_no]))
                return false; // No

        return true; // Yes
    }

    public static int ask_int(String question)
    {
        int v = -1;
        try {
            v = Integer.parseInt(ask(question));
        }
        catch (NumberFormatException e) {
            System.err.println("Invalid integer response.");
        }
        return v;
    }
}
