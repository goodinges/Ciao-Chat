/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package Authentication.JAAS;

import java.io.IOException;
import javax.security.auth.callback.Callback;
import javax.security.auth.callback.CallbackHandler;
import javax.security.auth.callback.NameCallback;
import javax.security.auth.callback.PasswordCallback;
import javax.security.auth.callback.TextOutputCallback;
import javax.security.auth.callback.UnsupportedCallbackException;

/**
 *
 * @author Kamran
 */
public class myCallBackHandler implements CallbackHandler{
    private final String username;
    private final char[] password;

    public myCallBackHandler(String username, char[] password) {
        this.username = username;
        this.password = password;
    }

    public void handle(Callback[] callbacks) throws IOException, UnsupportedCallbackException {
        for (int i = 0; i < callbacks.length; i++) {
    if (callbacks[i] instanceof TextOutputCallback) {



    } else if (callbacks[i] instanceof NameCallback) {
        ((NameCallback)callbacks[i]).setName(username);

    } else if (callbacks[i] instanceof PasswordCallback) {
        ((PasswordCallback)callbacks[i]).setPassword(password);
    } else {
        throw new UnsupportedCallbackException
         (callbacks[i], "Unrecognized Callback");
    }
  }
    }

}
