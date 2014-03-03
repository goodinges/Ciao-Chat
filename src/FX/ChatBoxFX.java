package FX;

import ChatUtil.ChatSession;
import ChatUtil.messageListener;
import com.skype.api.Conversation;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.effect.DropShadow;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.Clipboard;
import javafx.scene.input.ClipboardContent;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.web.HTMLEditor;
import javafx.scene.web.WebEngine;
import javafx.scene.web.WebView;
import javafx.stage.Stage;
import org.jivesoftware.smack.XMPPException;

public class ChatBoxFX extends Application {
    private String chatString = new String();
    private ChatSession chat;
    private boolean gtalk = false;
    private org.jivesoftware.smack.Chat gchat;
    private Conversation skypeConversation;
    private boolean skypeMode = false;

    public void setGtalk(boolean gtalk) {
        this.gtalk = gtalk;
    }
    VBox vbox = new VBox();
    VBox vbox1 = new VBox();
    HBox hbox = new HBox();
    Image image = new Image("src/UserInterface/profilePicLarge-icon.jpg");
    Image history = new Image("resources/pics/history.png");
    ImageView iv1 = new ImageView();
    ImageView iv2 = new ImageView();
    /*
     * HBox toolBar = new HBox(); private static String[] imageFiles = new
     * String[]{ "product.png", "blog.png", "forum.png", "partners.png" };
     * private static String[] captions = new String[]{ "Products", "Blogs",
     * "Forums", "Partners" };
     */
    Button historyButton = new Button("  history  ", new ImageView(history));
    ToolBar toolBar = new ToolBar(
            historyButton //            new Button("Open"),
            //            new Button("Save"),
            //            //new Separator(true),
            //            new Button("Clean"),
            //            new Button("Compile"),
            //            new Button("Run"),
            //            //new Separator(true),
            //            new Button("Debug"),
            //            new Button("Profile"));
            );

    public static void main(String[] args) {

        launch(args);
    }
//    private void setStyle() {
//        doc = .getStyledDocument();
//        dateStyle1 = showTextPane.addStyle("date", null);
//        StyleConstants.setFontFamily(dateStyle1, "Comic Sans MS");
//        StyleConstants.setForeground(dateStyle1, java.awt.Color.blue);
//        StyleConstants.setFontSize(dateStyle1, 12);
//        StyleConstants.setBold(dateStyle1, true);
//        textStyle1 = showTextPane.addStyle("text", null);
//        StyleConstants.setFontFamily(textStyle1, "Comic Sans MS");
//        StyleConstants.setForeground(textStyle1, java.awt.Color.blue);
//        StyleConstants.setFontSize(textStyle1, 11);
//        dateStyle2 = showTextPane.addStyle("date2", null);
//        StyleConstants.setFontFamily(dateStyle2, "Comic Sans MS");
//        StyleConstants.setForeground(dateStyle2, java.awt.Color.orange);
//        StyleConstants.setFontSize(dateStyle2, 12);
//        StyleConstants.setBold(dateStyle2, true);
//        textStyle2 = showTextPane.addStyle("text2", null);
//        StyleConstants.setFontFamily(textStyle2, "Comic Sans MS");
//        StyleConstants.setForeground(textStyle2, java.awt.Color.orange);
//        StyleConstants.setFontSize(textStyle2, 11);
//    }
//    StyledDocument doc;
//    Style dateStyle1;
//    Style textStyle1;
//    Style dateStyle2;
//    Style textStyle2;

    
        final WebView browser = new WebView();
        final WebEngine webEngine = browser.getEngine();
        final HTMLEditor htmlEditor = new HTMLEditor();
    
    @Override
    public void start(Stage stage) {

//        setStyle();
        toolBar.setStyle(" -fx-base: #ffa500;");
        historyButton.setTextFill(Color.rgb(21, 51, 173));
        final ContextMenu cm = new ContextMenu();
        MenuItem cmItem1 = new MenuItem("Change profile image");
//        cmItem1.setOnAction(new EventHandler<ActionEvent>() {
//
//            public void handle(ActionEvent e) {
//                Clipboard clipboard = Clipboard.getSystemClipboard();
//                ClipboardContent content = new ClipboardContent();
//                content.putImage(iv2.getImage());
//                clipboard.setContent(content);
//            }
//        });

        cm.getItems().add(cmItem1);

//        iv2.addEventHandler(MouseEvent.MOUSE_CLICKED,
//                new EventHandler<MouseEvent>() {
//
//                    @Override
//                    public void handle(MouseEvent e) {
//                        if (e.getButton() == MouseButton.SECONDARY) {
//                            cm.show(iv2, e.getScreenX(), e.getScreenY());
//                        }
//                    }
//                });
        hbox.setSpacing(20);
        iv1.setImage(image);
        iv2.setImage(image);
        iv1.setFitWidth(100);
        iv2.setFitWidth(100);
        iv1.setPreserveRatio(true);
        iv2.setPreserveRatio(true);
        vbox1.setSpacing(120);
        vbox1.setPadding(new Insets(50, 5, 5, 0));
        //vbox1.getChildren().addAll(iv1, iv2);
        stage.setTitle("Chat");
        Scene scene = new Scene(new VBox(), 440, 460);
        scene.setFill(Color.rgb(21, 51, 173));

        MenuBar menuBar = new MenuBar();

        // --- Menu File
        Menu menuFile = new Menu("File");

        // --- Menu Edit
        Menu menuEdit = new Menu("Edit");

        // --- Menu View
        Menu menuView = new Menu("View");

        menuBar.getMenus().addAll(menuFile, menuEdit, menuView);


        vbox.getChildren().addAll(menuBar);

        /*
         * for (int i = 0; i < captions.length; i++) { final Hyperlink hpl =
         * hpls[i] = new Hyperlink(captions[i]); final Image image = images[i] =
         * new Image(getClass().getResourceAsStream(imageFiles[i]));
         * hpl.setGraphic(new ImageView(image)); final String url = urls[i];
         *
         * // create the toolbar toolBar = new HBox();
         * toolBar.getStyleClass().add("browser-toolbar");
         * toolBar.getChildren().addAll(hpls);
         *
         */

        //add components
        vbox.getChildren().add(toolBar);

        VBox root = new VBox();
        root.setPadding(new Insets(8, 5, 8, 8));
        //root.setSpacing(5);
        root.setAlignment(Pos.TOP_LEFT);


        ScrollPane scrollPane = new ScrollPane();


        DropShadow effect = new DropShadow();
        effect.setRadius(10);
        effect.setOffsetY(3);
        effect.setColor(Color.BLACK);
        browser.setEffect(effect);


        scrollPane.getStyleClass().add("noborder-scroll-pane");
        scrollPane.setStyle("-fx-background-color: white");
        scrollPane.setContent(browser);
        scrollPane.setFitToWidth(false);
        scrollPane.setPrefWidth(390);
        scrollPane.setPrefHeight(240);

        htmlEditor.setPrefWidth(150);
        htmlEditor.setMaxWidth(140);
        htmlEditor.setPrefHeight(200);
        htmlEditor.setHtmlText("");


        Button showHTMLButton = new Button("Send");

        //root.setAlignment(Pos.CENTER);
        showHTMLButton.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent arg0) {
                chatString = chatString + htmlEditor.getHtmlText();
                webEngine.loadContent(chatString);
                String message = htmlEditor.getHtmlText();
                    message = message.substring(79);
                    message = message.substring(0, message.indexOf("</div>"));
                if (gtalk == false && skypeMode == false) {
                    
                    System.out.println("message to send: "+ htmlEditor.getHtmlText());
                    chat.send(message);
                } else if (gtalk == true) {
                    try {
                        gchat.sendMessage(message);
                    } catch (XMPPException ex) {
                        Logger.getLogger(ChatBoxFX.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    
                } else if (skypeMode = true) {
                    skypeConversation.PostText(message, false);
                }
                
                htmlEditor.setHtmlText("");
//                try {
//                    doc.insertString(doc.getLength(), "(" + getSentTextTime() + ") : ", dateStyle1);
//                } catch (BadLocationException ex) {
//                    Logger.getLogger(ChatBoxFX.class.getName()).log(Level.SEVERE, null, ex);
//                }
//                try {
//                    doc.insertString(doc.getLength(), htmlEditor.getHtmlText() + "\n", textStyle1);
//                } catch (BadLocationException ex) {
//                    Logger.getLogger(ChatBoxFX.class.getName()).log(Level.SEVERE, null, ex);
//                }
                
            }
        });

        root.getChildren().addAll(scrollPane, htmlEditor, showHTMLButton);
        hbox.getChildren().addAll(root/*
                 * ,vbox1
                 */);
        vbox.getChildren().add(hbox);

        scene.setRoot(vbox);
        stage.setScene(scene);
        stage.show();
    }

    private String getSentTextTime() {
        GregorianCalendar now = new GregorianCalendar();//gets the current date and time 
        int year = now.get(Calendar.YEAR);
        int month = now.get(Calendar.MONTH);
        int day = now.get(Calendar.DATE) + 1; //add one because January is integer 0 
        int hour = now.get(Calendar.HOUR_OF_DAY);
        int minute = now.get(Calendar.MINUTE);
        int second = now.get(Calendar.SECOND);

        String output = year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second;
        return output;

    }

    public void setChat(ChatSession chat) {
        this.chat = chat;
        chat.setListener(new messageListener() {

            @Override
            public void processMessage(String message) {
                chatString = chatString + "<html><div><font color=\"66CC33\">"+message+"</font></div></html>\n";
                System.out.println(chatString);
                
                webEngine.loadContent(chatString);
                
//                try {
//                    doc.insertString(doc.getLength(), "(" + getSentTextTime() + ") : ", dateStyle2);
//                
//                    doc.insertString(doc.getLength(), message + "\n", textStyle2);
//                } catch (BadLocationException ex) {
//                    Logger.getLogger(ChatBoxFX.class.getName()).log(Level.SEVERE, null, ex);
//                }
                
            }
        });
    }

    public void showMessage(String message) {
        if (message != null) {
            chatString = chatString + "<html><div><font color=\"66CC33\">"+message+"</font></div></html>\n";
            //htmlEditor.setHtmlText(message);
            System.out.println(chatString);
                webEngine.loadContent(chatString);
            
        }
    }

    public void setChat(org.jivesoftware.smack.Chat chat) {
        gchat = chat;
    }

    public void setSkype(Conversation conversation) {
        skypeConversation = conversation;
    }

    public void setSkypeMode() {
        skypeMode = true;
    }
}
