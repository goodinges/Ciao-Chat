/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ChatUtil;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Platform;
import javafx.concurrent.Task;

/**
 *
 * @author Kamran
 */
public abstract class messageListener implements Runnable {

    private BufferedReader in;
    private boolean listening = true;

    abstract public void processMessage(String message);

    public void setIN(BufferedReader in) {
        this.in = in;
    }

    public void setListening(boolean listening) {
        this.listening = listening;
    }

    @Override
    public void run() {
        Task task = new Task<Void>() {

            @Override
            public Void call() {
                try {
                    while (listening) {
                        System.out.println("sd");
                        final String message =in.readLine();
                        System.out.println("received: " + message);
                        Platform.runLater(new Runnable() {

                            @Override
                            public void run() {

                                processMessage(message);
                            }
                        });
                    }
                } catch (IOException ex) {
                    Logger.getLogger(messageListener.class.getName()).log(Level.SEVERE, null, ex);
                }
                return null;
            }
        };
        new Thread(task).start();



    }
}
