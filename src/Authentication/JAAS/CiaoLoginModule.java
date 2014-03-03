/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Authentication.JAAS;

import java.io.IOException;
import java.util.AbstractList;
import java.util.List;
import java.util.Map;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
import javax.security.auth.Subject;
import javax.security.auth.callback.Callback;
import javax.security.auth.callback.CallbackHandler;
import javax.security.auth.callback.NameCallback;
import javax.security.auth.callback.PasswordCallback;
import javax.security.auth.callback.UnsupportedCallbackException;
import javax.security.auth.login.LoginException;
import javax.security.auth.spi.LoginModule;

/**
 *
 * @author Kamran
 */
public class CiaoLoginModule implements LoginModule {

    private Subject subject;
    private CallbackHandler callbackHandler;
    // the authentication status
    private boolean succeeded = false;
    private boolean commitSucceeded = false;
    // username and password
    private String username;
    private UsernamePrincipal userPrincipal;

    public void initialize(Subject subject, CallbackHandler callbackHandler, Map<String, ?> sharedState, Map<String, ?> options) {
        this.subject = subject;
        this.callbackHandler = callbackHandler;
    }

    public boolean login() throws LoginException {

            /*
            Properties properties = null;
            properties = new Properties();
            properties.put("java.naming.factory.initial",
            "com.sun.jndi.cosnaming.CNCtxFactory");
            properties.put("java.naming.provider.url", "localhost:1099");
            Properties props = new Properties();
            try {
                props.load(new FileInputStream("jndi.properties"));
            } catch (IOException ex) {
                Logger.getLogger(CiaoLoginModule.class.getName()).log(Level.SEVERE, null, ex);
            }
             * 
             */
         
        
        NameCallback nameCallback = new NameCallback("Username");
        PasswordCallback passwordCallback = new PasswordCallback("Password", false);
        Callback[] callbacks = {nameCallback, passwordCallback};
        try {
            //prompting for a user name and password
            callbackHandler.handle(callbacks);
        } catch (IOException ex) {
            Logger.getLogger(CiaoLoginModule.class.getName()).log(Level.SEVERE, null, ex);
        } catch (UnsupportedCallbackException ex) {
            Logger.getLogger(CiaoLoginModule.class.getName()).log(Level.SEVERE, null, ex);
        }
        //verify the password against a password database.
        //TODO @ Tannaz : check user and pass against mysql DB
        //temp & test code:
        List<Integer> ls = new Vector<Integer> ();
        char[] tey = passwordCallback.getPassword();
        for(int i =0;i<tey.length;i++){
            ls.add(Integer.valueOf(tey[i]));
        }
        if (athenticate(nameCallback.getName(), ls)) {
            passwordCallback.clearPassword();
            username = nameCallback.getName();
            succeeded = true;
            return true;
        } else {
            passwordCallback.clearPassword();
            succeeded = false;
            return false;
        }
    }

    public boolean commit() throws LoginException {
        if (succeeded == false) {
            return false;
        } else {
            userPrincipal = new UsernamePrincipal(username);
            if (!subject.getPrincipals().contains(userPrincipal)) {
                subject.getPrincipals().add(userPrincipal);
            }

            username = null;

            commitSucceeded = true;
            return true;
        }
    }

    public boolean abort() throws LoginException {
        if (succeeded == false) {
            return false;
        } else if (succeeded == true && commitSucceeded == false) {
            // login succeeded but overall authentication failed
            succeeded = false;
            username = null;
            userPrincipal = null;
        } else {
            // overall authentication succeeded and commit succeeded,
            // but someone else's commit failed
            logout();
        }
        return true;
    }

    public boolean logout() throws LoginException {
        subject.getPrincipals().remove(userPrincipal);
        succeeded = false;
        succeeded = commitSucceeded;
        username = null;

        userPrincipal = null;
        return true;
    }

    private static boolean athenticate(java.lang.String username, java.util.List<java.lang.Integer> pass) {
        AuthenticationWS.AuthenticationWS_Service service = new AuthenticationWS.AuthenticationWS_Service();
        AuthenticationWS.AuthenticationWS port = service.getAuthenticationWSPort();
        return port.athenticate(username, pass);
    }

    
}
