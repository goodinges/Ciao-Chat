/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Skype;

import FX.ChatBoxFX;
import FX.Skype;
import Skype.Util.AppKeyPairMgr;
import Skype.Util.MySession;
import UserInterface.ChatFrame;
import UserInterface.ContactInfoCollectionListModel;
import UserInterface.SkypeFrame;
import com.skype.api.ContactGroup.TYPE;
import com.sun.deploy.uitoolkit.impl.fx.ui.FXAboutDialog;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import javafx.application.Platform;
import javafx.stage.Stage;

/**
 *
 * @author Kamran
 */
public class SkypeController {

    /**
     * Info/Debug console output message prefix/identifier tag. Corresponds to
     * class name.
     *
     * @since 1.0
     */
    public static final String MY_CLASS_TAG = "CiaoChat";
    public static Map<String, ChatBoxFX> chatFrameMap = new HashMap<String, ChatBoxFX>();
    private static AppKeyPairMgr myAppKeyPairMgr = new AppKeyPairMgr();
    private static MySession mySession = new MySession();
    private static String ACCOUNT_NAME;
    private static String ACCOUNT_PWORD;
    private static SkypeController skypeController = new SkypeController();

    public static void startChat(final String msgAuthor) {

        new Thread(new Runnable() {

            @Override
            public void run() {
                Platform.runLater(new Runnable() {

                    @Override
                    public void run() {
                        ChatBoxFX chatframe = SkypeController.chatFrameMap.get(msgAuthor);
                        if (chatframe == null) {
                            chatframe = new ChatBoxFX();
                            Stage stage = new Stage();
                            chatframe.start(stage);
                            //chatframe.setVisible(true);
                            chatframe.setSkypeMode();
                            String[] participants = {msgAuthor};
                            chatframe.setSkype(mySession.mySkype.GetConversationByParticipants(participants, true, true));
                            SkypeController.chatFrameMap.put(msgAuthor, chatframe);
                        }
                    }
                });
            }
        }).start();


        //chatframe.setVisible(true);

    }

    public static boolean Login(Skype fxSkype) {
        SkypeController.fxSkype = fxSkype;
        return Login();
    }
    private static Skype fxSkype;

    public SkypeController() {
    }

    public static void setACCOUNT_NAME(String ACCOUNT_NAME) {
        SkypeController.ACCOUNT_NAME = ACCOUNT_NAME;
    }

    public static void setACCOUNT_PWORD(String ACCOUNT_PWORD) {
        SkypeController.ACCOUNT_PWORD = ACCOUNT_PWORD;
    }

    /**
     * Main loop
     *
     * @param args <ol> <li>name of the target Skype account</li> <li>password
     * for the target Skype account</li> <li>optional pathname of an AppKeyPair
     * PEM file</li> </ol>
     *
     * @since 1.0
     */
    public static void main(String[] args) {
        setACCOUNT_NAME("goodinges");
        setACCOUNT_PWORD("goldbarrow6");
        if (Login()) {
            skypeController.doChat(mySession);
        } else {
            MySession.myConsole.printf("%s: Logging in failed! %n", MY_CLASS_TAG);
        }
        Logout();
    }

    public static void Logout() {
        MySession.myConsole.printf("%s: main - Logging out...%n", MY_CLASS_TAG);
        mySession.mySignInMgr.Logout(MY_CLASS_TAG, mySession);
        MySession.myConsole.printf("%s: Cleaning up...%n", MY_CLASS_TAG);
        if (mySession != null) {
            mySession.doTearDownSession();
        }
        MySession.myConsole.printf("%s: Done!%n", MY_CLASS_TAG);
    }

    public static boolean Login() {
        if (!myAppKeyPairMgr.setAppKeyPairFromFile()) {
            return false;
        }
        MySession.myConsole.printf("%s: main - Creating session - Account = %s%n",
                MY_CLASS_TAG, ACCOUNT_NAME);
        mySession.doCreateSession(MY_CLASS_TAG, ACCOUNT_NAME, myAppKeyPairMgr);
        MySession.myConsole.printf("%s: main - Logging in w/ password %s%n",
                MY_CLASS_TAG, ACCOUNT_PWORD);
        if (mySession.mySignInMgr.Login(MY_CLASS_TAG, mySession, ACCOUNT_PWORD)) {
            //
            // >>>Insert "business logic" here <<<
            //
        } else {
            MySession.myConsole.printf("%s: Account for %s does not exist.%n", MY_CLASS_TAG, mySession.myAccountName);
            return false;
        }
        MySession.myConsole.printf("%s: Logged in! %n", MY_CLASS_TAG);
        Platform.runLater(new Runnable() {

            @Override
            public void run() {
                fxSkype.logedin(new ContactInfoCollectionListModel(mySession.mySkype.GetHardwiredContactGroup(TYPE.SKYPE_BUDDIES).GetContacts()));
            }
        });
//        SkypeFrame skypeFrame = new SkypeFrame();
//        skypeFrame.setVisible(true);
//        skypeFrame.setSkypeModel(new ContactInfoCollectionListModel(mySession.mySkype.GetHardwiredContactGroup(TYPE.SKYPE_BUDDIES).GetContacts()));
        return true;
    }

    void doChat(MySession mySession) {

        MySession.myConsole.printf("%s: Waiting for incoming chat messages...%n%n",
                mySession.myTutorialTag);
        try {
            while (true) {
                int keyboardChar = System.in.read();
                // Some platforms think ENTER is 0x0D; others think it's 0x0A...
                if ((keyboardChar == 0x0D) || (keyboardChar == 0x0A)) {
                    break;
                }
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return;
        }

        // End capturing chat text
        MySession.myConsole.println();
    }
}
