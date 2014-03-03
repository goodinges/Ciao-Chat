/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package XMPP;

import java.util.logging.Level;
import java.util.logging.Logger;
import org.jivesoftware.smack.Chat;
import org.jivesoftware.smack.ChatManager;
import org.jivesoftware.smack.ChatManagerListener;
import org.jivesoftware.smack.Connection;
import org.jivesoftware.smack.MessageListener;
import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.XMPPException;
import org.jivesoftware.smack.packet.Message;
import org.jivesoftware.smack.packet.Presence;

/**
 *
 * @author Kamran
 */
public class Test {

    public static void main(String[] args) throws XMPPException {
        Connection connection = new XMPPConnection("google.com");
        connection.connect();
        connection.login("goodinges@gmail.com", "goldbarrowg");

        Chat chat = connection.getChatManager().createChat("jino.rahimi@gmail.com", new MessageListener() {

            public void processMessage(Chat chat, Message message) {
                System.out.println("Received message: " + message.getBody());
            }
        });
        //System.out.println(chat.getThreadID());
        System.out.println(connection.getRoster().getEntryCount());
        chat.sendMessage("salam! man netbeansam! kamran neveshtatam  :)");
//
//        Presence presence = new Presence(Presence.Type.available);
//        presence.setStatus("Gone fishing");
//// Send the packet (assume we have a Connection instance called "con").
//        connection.sendPacket(presence);
        try {
            Thread.sleep(60000);
        } catch (InterruptedException ex) {
            Logger.getLogger(Test.class.getName()).log(Level.SEVERE, null, ex);
        }
        connection.disconnect();
    }
}
