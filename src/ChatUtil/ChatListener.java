/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChatUtil;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Kamran
 */
public class ChatListener extends Thread {
    private boolean accepting = true;

    @Override
    public void run() {
        try {
            ServerSocket ss = new ServerSocket(444);
            Socket clientSocket = null;

            try {
                while (accepting) {
                    clientSocket = ss.accept();
                    newChat(clientSocket);
                }
            } catch (IOException e) {
                System.err.println("Accept failed.");
                System.exit(1);
            }
            /*
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(
            new InputStreamReader(
            clientSocket.getInputStream()));*/
        } catch (IOException ex) {
            Logger.getLogger(ChatListener.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    private void newChat(Socket clientSocket) {
        ChatController.newChat(clientSocket);
    }
}
