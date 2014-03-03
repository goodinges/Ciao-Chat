/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChatUtil;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Platform;

/**
 *
 * @author Kamran
 */
public final class ChatSession {

    private InetSocketAddress address;
    Socket socket = null;
    PrintWriter out = null;
    private String contact;

    public String getContact() {
        return contact;
    }

    public void setContact(String contact) {
        this.contact = contact;
    }
    private String me;

    public String getMe() {
        return me;
    }

    public void setMe(String me) {
        this.me = me;
    }
    BufferedReader in = null;
    private messageListener listener;

    public ChatSession(String recep) {
        this.address = getAddress(recep);
        connect(address);
    }

    ChatSession(Socket clientSocket) {
        socket = clientSocket;
        try {
            setInOut();
        } catch (IOException ex) {
            Logger.getLogger(ChatSession.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public InetSocketAddress getAddress(String recep) {
        InetSocketAddress in = new InetSocketAddress(getIP(recep), getPort(recep));
        System.out.println(getIP(recep));
        return in;
    }

    private void connect(InetSocketAddress address) {
        try {
            socket = new Socket(address.getAddress(), address.getPort());
            setInOut();
        } catch (IOException ex) {
            Logger.getLogger(ChatSession.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    public void setInOut() throws IOException {
        out = new PrintWriter(socket.getOutputStream(), true);
        in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    }

    private void disconnect() {
        try {
            socket.close();
            in.close();
            out.close();
        } catch (IOException ex) {
            Logger.getLogger(ChatSession.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void send(String text) {
        out.println(text);
    }

    public void setListener(messageListener messageListener) {
        listener = messageListener;
        listener.setIN(in);
        new Thread(listener).start();
    }

    private static ChatWSclient.InetSocketAddress requestChat(java.lang.String id) {
        ChatWSclient.ChatWS_Service service = new ChatWSclient.ChatWS_Service();
        ChatWSclient.ChatWS port = service.getChatWSPort();
        return port.requestChat(id);
    }

    private static String getIP(java.lang.String id) {
        ChatWSclient.ChatWS_Service service = new ChatWSclient.ChatWS_Service();
        ChatWSclient.ChatWS port = service.getChatWSPort();
        return port.getIP(id);
    }

    private static int getPort(java.lang.String id) {
        ChatWSclient.ChatWS_Service service = new ChatWSclient.ChatWS_Service();
        ChatWSclient.ChatWS port = service.getChatWSPort();
        return port.getPort(id);
    }
}
