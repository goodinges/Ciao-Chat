/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChatUtil;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

/**
 *
 * @author Kamran
 */
public class testServer {

    public static void main(String[] args) throws IOException, InterruptedException {
        ServerSocket ss = new ServerSocket(444);
        Socket clientSocket = null;
        
        try {
            clientSocket = ss.accept();
        } catch (IOException e) {
            System.err.println("Accept failed.");
            System.exit(1);
        }

        PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
        BufferedReader in = new BufferedReader(
                new InputStreamReader(
                clientSocket.getInputStream()));
        while (true) {
            System.out.println(in.readLine());
            Thread.sleep(1000);
            out.println("ok");
        }
    }
}
