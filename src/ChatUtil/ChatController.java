/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChatUtil;

import Authentication.authControl;
import FX.ChatBoxFX;
import UserInterface.ChatFrame;
import java.net.Socket;
import java.util.List;
import java.util.Vector;
import javafx.application.Platform;
import javafx.stage.Stage;

/**
 *
 * @author Kamran
 */
public class ChatController {

    public static void newChat(String recep) {
        final ChatSession chat = new ChatSession(recep);
        new Thread(new Runnable() {

            @Override
            public void run() {
                Platform.runLater(new Runnable() {

                    @Override
                    public void run() {

                        //  yourNameLabel.setText(ChatForm.getUserInfo());
                        ChatBoxFX cf = new ChatBoxFX();
                        cf.setChat(chat);
                        Stage stage = new Stage();
                        cf.start(stage);
                        //cf.setvisible(true);

                    }
                });
            }
        }).start();


    }

    public static void newChat(Socket clientSocket) {
        final ChatSession chat = new ChatSession(clientSocket);
        new Thread(new Runnable() {

            @Override
            public void run() {
                Platform.runLater(new Runnable() {

                    @Override
                    public void run() {

                        //  yourNameLabel.setText(ChatForm.getUserInfo());
                        ChatBoxFX cf = new ChatBoxFX();
                        cf.setChat(chat);
                        Stage stage = new Stage();
                        cf.start(stage);
                        //cf.setvisible(true);

                    }
                });
            }
        }).start();
    }

    public static Vector<String> getContacts(String text) {
        List<Object> list = getContactlist(text);
        Vector<String> vc = new Vector<String>();
        for (int i = 0; i < list.size(); i++) {
            vc.add((String) list.get(i));
        }

        return vc;
    }

    public static void addContact(String text) {
        addContact_1(authControl.getId(), text);
    }

    private static boolean addContact_1(java.lang.String userID, java.lang.String friendID) {
        ChatWSclient.ChatWS_Service service = new ChatWSclient.ChatWS_Service();
        ChatWSclient.ChatWS port = service.getChatWSPort();
        return port.addContact(userID, friendID);
    }

    private static java.util.List<java.lang.Object> getContactlist(java.lang.String userID) {
        ChatWSclient.ChatWS_Service service = new ChatWSclient.ChatWS_Service();
        ChatWSclient.ChatWS port = service.getChatWSPort();
        return port.getContactlist(userID);
    }
}
