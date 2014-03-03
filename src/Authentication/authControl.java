/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Authentication;

import Authentication.JAAS.myCallBackHandler;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.security.auth.login.LoginContext;
import javax.security.auth.login.LoginException;

/**
 *
 * @author Kamran
 */
public class authControl {

    private static LoginContext lc;
    private static String id;

    /**
     * Get the value of id
     *
     * @return the value of id
     */
    public static String getId() {
        return id;
    }

    public static boolean login(String username, char[] password) {

        try {
            lc = new LoginContext("Sample", new myCallBackHandler(username, password));
        } catch (LoginException ex) {
            Logger.getLogger(authControl.class.getName()).log(Level.SEVERE, null, ex);
        }
        try {
            lc.login();

        } catch (LoginException ex) {
            Logger.getLogger(authControl.class.getName()).log(Level.SEVERE, null, ex);
            return false;
        }
        id = username;
        try {
            sendIP(InetAddress.getLocalHost().toString().split("/")[1], username);
        } catch (UnknownHostException ex) {
            Logger.getLogger(authControl.class.getName()).log(Level.SEVERE, null, ex);
        }
        sendPort(444, username);
        
        return true;
    }

    public static boolean signUp(String username, char[] password, int selectedQuestion, String answer, String firstName, String lastName, boolean gender, String BDday, String BDmonth, String BDyear, String email) {
        return signup(username, String.valueOf(password), selectedQuestion, answer, firstName, lastName, gender, BDday, email);
    }

    public static boolean usernameTaken(String username) {
        return userNameTaken(username);
    }

    public static void forgetCred(String username) {
        forgetCred_1(username);

    }

    public static String[] getUserInfo(String username, String[] infos) {
        List<String> list = getUserInfo_1(username);
        String[] str = new String[2];
        str[0] = list.get(0);
        str[1] = list.get(1);
        str[1] = list.get(1);
        return str;
    }

    private static boolean athenticate(java.lang.String username, java.util.List<java.lang.Integer> pass) {
        AuthenticationWS.AuthenticationWS_Service service = new AuthenticationWS.AuthenticationWS_Service();
        AuthenticationWS.AuthenticationWS port = service.getAuthenticationWSPort();
        return port.athenticate(username, pass);
    }

    private static boolean forgetCred_1(java.lang.String username) {
        AuthenticationWS.AuthenticationWS_Service service = new AuthenticationWS.AuthenticationWS_Service();
        AuthenticationWS.AuthenticationWS port = service.getAuthenticationWSPort();
        return port.forgetCred(username);
    }

    private static java.util.List<java.lang.String> getUserInfo_1(java.lang.String username) {
        AuthenticationWS.AuthenticationWS_Service service = new AuthenticationWS.AuthenticationWS_Service();
        AuthenticationWS.AuthenticationWS port = service.getAuthenticationWSPort();
        return port.getUserInfo(username);
    }

    private static boolean signup(java.lang.String id, java.lang.String pass, int question, java.lang.String answer, java.lang.String firstName, java.lang.String lastName, boolean gender, java.lang.String birthday, java.lang.String email) {
        AuthenticationWS.AuthenticationWS_Service service = new AuthenticationWS.AuthenticationWS_Service();
        AuthenticationWS.AuthenticationWS port = service.getAuthenticationWSPort();
        return port.signup(id, pass, question, answer, firstName, lastName, gender, birthday, email);
    }

    private static boolean userNameTaken(java.lang.String username) {
        AuthenticationWS.AuthenticationWS_Service service = new AuthenticationWS.AuthenticationWS_Service();
        AuthenticationWS.AuthenticationWS port = service.getAuthenticationWSPort();
        return port.userNameTaken(username);
    }

    private static void sendIP(java.lang.String ip, java.lang.String id) {
        ChatWSclient.ChatWS_Service service = new ChatWSclient.ChatWS_Service();
        ChatWSclient.ChatWS port = service.getChatWSPort();
        port.sendIP(ip, id);
    }

    private static void sendPort(int portt, java.lang.String id) {
        ChatWSclient.ChatWS_Service service = new ChatWSclient.ChatWS_Service();
        ChatWSclient.ChatWS port = service.getChatWSPort();
        port.sendPort(portt, id);
    }

    private static java.util.List<java.lang.Byte> getProfilePicture(java.lang.String userName) {
        ChatWSclient.ChatWS_Service service = new ChatWSclient.ChatWS_Service();
        ChatWSclient.ChatWS port = service.getChatWSPort();
        return port.getProfilePicture(userName);
    }

    public static List<Byte> getProfilePic(String id) {
        return getProfilePicture(id);
    }

    

    
}
