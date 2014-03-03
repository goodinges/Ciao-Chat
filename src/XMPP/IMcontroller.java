/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package XMPP;

import org.jivesoftware.smack.Chat;
import org.jivesoftware.smack.ChatManager;
import org.jivesoftware.smack.Connection;
import org.jivesoftware.smack.MessageListener;
import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.XMPPException;
import org.jivesoftware.smack.packet.Message;

/**
 *
 * @author Kamran
 */
public class IMcontroller {

    final String XMPPserver = "google.com";
    Connection conn;

    boolean connect() throws XMPPException {
        conn = new XMPPConnection(XMPPserver);
        conn.connect();
        return true;
    }

    boolean login(String JID, String JPASS) throws XMPPException {
        conn.login(JID, JPASS);
        return true;

    }

    boolean disconnect() {
        conn.disconnect();
        return true;
    }

    Chat newChat(String respondent) {
        ChatManager chatmanager = conn.getChatManager();
        Chat newChat = chatmanager.createChat(respondent, new MessageListener() {

            public void processMessage(Chat chat, Message message) {
                System.out.println("Received message: " + message);
            }
        });
        return newChat;
    }
    boolean sendMessage(Chat chat,String message) throws XMPPException{
        chat.sendMessage(message);
        return true;
    }
}
