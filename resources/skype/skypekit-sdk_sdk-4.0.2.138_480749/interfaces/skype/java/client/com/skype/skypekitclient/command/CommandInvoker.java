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


import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.command.ClientCommandInterface.NoCommandException;

public class CommandInvoker {
//    private static final String TAG = "CommandInvoker";

    private SkypekitClient skClient;

    private static Map <String, ClientCommandInterface> commandMap;

	public CommandInvoker(SkypekitClient skypekitClient) {
		skClient = skypekitClient;

		commandMap = new HashMap<String, ClientCommandInterface>();
    	commandMap.put("a", new CommandAccount(skClient));
    	commandMap.put("c", new CommandConversation(skClient));
    	commandMap.put("C", new CommandConversationCall(skClient));
    	commandMap.put("s", new CommandContactSearch(skClient));
    	commandMap.put("l", new CommandContactList(skClient));
    	commandMap.put("f", new CommandFileTransfer(skClient));
    	commandMap.put("M", new CommandConversationConference(skClient));
    	commandMap.put("V", new CommandVoiceMail(skClient));
    	commandMap.put("S", new CommandSms(skClient));
    	commandMap.put("G", new CommandGreeting(skClient));
    	commandMap.put("p", new CommandPrivacy(skClient));
    	commandMap.put("h", new CommandConversationChat(skClient));
    	commandMap.put("e", new CommandSetup(skClient));
    	commandMap.put("2", new CommandApp2App(skClient));
    }

	public void executeCommand( Character cmd, Character subCmd) {

		if (cmd == '?' && subCmd == '?') {
            executeFullHelp();
            return;
		}

		ClientCommandInterface command = (ClientCommandInterface)commandMap.get(cmd.toString());
		if (command == null) {
		    skClient.error("invalid command");
			return;
		}

		try {
		switch (subCmd.charValue()) {
        case 'a' :
        	command.execute_a();
        	break;
        case 'b' :
        	command.execute_b();
        	break;
        case 'c' :
            command.execute_c();
            break;
        case 'd' :
            command.execute_d();
            break;
        case 'e' :
        	command.execute_e();
        	break;
        case 'f' :
        	command.execute_f();
        	break;
        case 'g' :
        	command.execute_g();
        	break;
        case 'h' :
        	command.execute_h();
        	break;
        case 'i' :
        	command.execute_i();
        	break;
        case 'j' :
        	command.execute_j();
        	break;
        case 'k' :
        	command.execute_k();
        	break;
        case 'l' :
        	command.execute_l();
        	break;
        case 'm' :
        	command.execute_m();
        	break;
        case 'n' :
        	command.execute_n();
        	break;
        case 'o' :
        	command.execute_o();
        	break;
        case 'p' :
        	command.execute_p();
        	break;
        case 'q' :
        	command.execute_q();
        	break;
        case 'r' :
        	command.execute_r();
        	break;
        case 's' :
        	command.execute_s();
        	break;
        case 't' :
        	command.execute_t();
        	break;
        case 'u' :
        	command.execute_u();
        	break;
        case 'v' :
        	command.execute_v();
        	break;
        case 'w' :
        	command.execute_w();
        	break;
        case 'x' :
        	command.execute_x();
        	break;
        case 'y' :
        	command.execute_y();
        	break;
        case 'z' :
        	command.execute_z();
        	break;
        case 'A' :
        	command.execute_A();
        	break;
        case 'B' :
        	command.execute_B();
        	break;
        case 'C' :
        	command.execute_C();
        	break;
        case 'D' :
        	command.execute_D();
        	break;
        case 'E' :
        	command.execute_E();
        	break;
        case 'F' :
        	command.execute_F();
        	break;
        case 'G' :
        	command.execute_G();
        	break;
        case 'H' :
        	command.execute_H();
        	break;
        case 'I' :
        	command.execute_I();
        	break;
        case 'J' :
        	command.execute_J();
        	break;
        case 'K' :
        	command.execute_K();
        	break;
        case 'L' :
        	command.execute_L();
        	break;
        case 'M' :
        	command.execute_M();
        	break;
        case 'N' :
        	command.execute_N();
        	break;
        case 'O' :
        	command.execute_O();
        	break;
        case 'P' :
        	command.execute_P();
        	break;
        case 'Q' :
        	command.execute_Q();
        	break;
        case 'R' :
        	command.execute_R();
        	break;
        case 'S' :
        	command.execute_S();
        	break;
        case 'T' :
        	command.execute_T();
        	break;
        case 'U' :
        	command.execute_U();
        	break;
        case 'V' :
        	command.execute_V();
        	break;
        case 'W' :
        	command.execute_W();
        	break;
        case 'X' :
        	command.execute_X();
        	break;
        case 'Y' :
        	command.execute_Y();
        	break;
        case 'Z' :
        	command.execute_Z();
        	break;
        case '1' :
        	command.execute_1();
        	break;
        case '2' :
        	command.execute_2();
        	break;
        case '3' :
        	command.execute_3();
        	break;
        case '4' :
        	command.execute_4();
        	break;
        case '5' :
        	command.execute_5();
        	break;
        case '6' :
        	command.execute_6();
        	break;
        case '7' :
        	command.execute_7();
        	break;
        case '8' :
        	command.execute_8();
        	break;
        case '9' :
        	command.execute_9();
        	break;
        case '0' :
        	command.execute_0();
        	break;
        case '?' :
        	command.executeHelp(); //module help
        	break;
        default:
            skClient.error("invalid command");
        	break;
        }
		}
		catch (NoCommandException e) {
		    skClient.error("invalid command");
		}
	}

	public void executeFullHelp()
	{
		String key;

		executeHelp(false);

		Set<String> keySet = commandMap.keySet();
		Iterator<String> it = keySet.iterator();
		while ( it.hasNext()) {
			key = it.next();
			commandMap.get(key).executeHelp();
		} 
	}

	public void executeHelp(boolean skypecommands)
	{
		String key;
        skClient.out(
        	"\n\t? - show help\n" +
            "\tr - reconnect to skypekit\n" +
            "\t%% - show created objects\n" +
            "\tq - quit");
                
        if (skypecommands) {        
            skClient.out("\n[HELP] - ?\n\t?? - full help");
            
            Set<String> keySet = commandMap.keySet();
            Iterator<String> it = keySet.iterator();
            while ( it.hasNext()) {
            	key = it.next();
            	skClient.out("\t" + key + "? - " + (commandMap.get(key)).getName() + " help");
            } 
        }
	}
	
	public	ClientCommandInterface getCommand(String key)
	{
	    return (ClientCommandInterface)commandMap.get(key);
	}
}
