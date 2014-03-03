/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package FX;

/**
 *
 * @author Parvaneh
 */
/**
 * Copyright (c) 2008, 2011 Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 */
import UserInterface.ContactInfoCollectionListModel;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.animation.*;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.effect.DropShadow;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import javafx.util.Duration;
import javax.swing.DefaultListModel;
import org.jivesoftware.smack.*;

public class Gtalk extends Application {

    private DefaultListModel statusesListModel = new DefaultListModel();
    private Connection gmailConnection;
    private UserInterface.ChatForm chatForm;

    public Gtalk(Connection gmailConnection) {
        this.gmailConnection = gmailConnection;
    }

    void setGModel(ContactInfoCollectionListModel contactInfoCollectionListModel) {
        listView.setItems(FXCollections.observableList(contactInfoCollectionListModel.vc));
    }
    private ParallelTransition parallelTransition;
    private ParallelTransition parallelTransition1= new ParallelTransition();
    private SequentialTransition sequentialTransition1;
    private SequentialTransition sequentialTransition;
    final ListView<String> listView = new ListView<>();
    TextField userNameField = new TextField();
    PasswordField passwordField = new PasswordField();
    Button signIn = new Button("Sign In");
    GridPane grid = new GridPane();
    DropShadow shadow = new DropShadow();
    public Label notExist = new Label("    Username does not exists");
    ImageView GtalkImage = new ImageView();
    ImageView GtalkIcon = new ImageView();
    ImageView rangi = new ImageView();
    Image gtalkLogo = new Image("src/fx/Gtak-icon.png");
    Image gtalkBg = new Image("src/fx/gtalk.png");
    Image rang = new Image("src/fx/rang.png");
    VBox box = new VBox();
    final VBox vbox = new VBox();

    @Override
    public void start(Stage stage) throws Exception {
        stage.setTitle("Gtalk");

        Scene scene = new Scene(vbox, 300, 550);
        stage.setScene(scene);
        listView.setItems(FXCollections.observableArrayList(
                "Row 1", "Row 2", "Long Row 3", "Row 4", "Row 5", "Row 6",
                "Row 7", "Row 8", "Row 9", "Row 10", "Row 11", "Row 12", "Row 13",
                "Row 14", "Row 15", "Row 16", "Row 17", "Row 18", "Row 19", "Row 20"));
        listView.getSelectionModel().setSelectionMode(SelectionMode.MULTIPLE);
        rangi.setImage(rang);
        rangi.setOpacity(0);
        GtalkImage.setImage(gtalkLogo);
        GtalkIcon.setImage(gtalkBg);
        listView.setOpacity(0);
        GtalkImage.setTranslateX(50);
        GtalkImage.setTranslateY(180);
        GtalkImage.setScaleX(1.1f);
        GtalkImage.setScaleY(1.1f);
        //grid.setPrefSize(300, 550);
        grid.setTranslateY(-280);
        //grid.setAlignment(Pos.TOP_LEFT);
        grid.setTranslateX(40);
        vbox.getChildren().add(GtalkIcon);

        //box.setPrefSize(400, 600);

        //box.setAlignment(Pos.TOP_LEFT);
        box.setSpacing(0);
        //box.setPadding(new Insets(10, 10, 0, 10));
        //box.setOpacity(0);

        //grid.setPadding(new Insets(10, 10, 5, 70));
        grid.setVgap(10);
        grid.setHgap(10);

        //userName text field
        userNameField.setPromptText("Enter your username");
        userNameField.setPrefColumnCount(10);
        userNameField.getText();
        GridPane.setConstraints(userNameField, 0, 0);
        grid.getChildren().add(userNameField);

        GridPane.setConstraints(notExist, 0, 2);
        //grid.getChildren().add(notExist);

        // password field
        passwordField.setPromptText("Enter your password");
        GridPane.setConstraints(passwordField, 0, 1);
        grid.getChildren().add(passwordField);
        box.getChildren().add(grid);
        signIn.setTranslateX(180);
        signIn.setTranslateY(-135);

        box.getChildren().add(signIn);


        FadeTransition fadeTransition =
                new FadeTransition(Duration.seconds(2.0), GtalkImage);
        fadeTransition.setFromValue(0.1f);
        fadeTransition.setToValue(1.0f);
        fadeTransition.setCycleCount(1);
        FadeTransition fadeTransition1 =
                new FadeTransition(Duration.seconds(2.0), box);
        fadeTransition1.setFromValue(0.0f);
        fadeTransition1.setToValue(1.0f);
        fadeTransition1.setCycleCount(1);


        FadeTransition loginTransition1 =
                new FadeTransition(Duration.seconds(1.0), listView);
        loginTransition1.setFromValue(0.0f);
        loginTransition1.setToValue(1.0f);
        loginTransition1.setCycleCount(1);
        
                FadeTransition loginTransition2 =
                new FadeTransition(Duration.seconds(1.0), rangi);
        loginTransition2.setFromValue(0.0f);
        loginTransition2.setToValue(1.0f);
        loginTransition2.setCycleCount(1);

        TranslateTransition translateTransition =
                new TranslateTransition(Duration.seconds(1), GtalkImage);
        translateTransition.setFromY(180);
        translateTransition.setToY(-10);
        //translateTransition.setFromX(80);
        translateTransition.setToX(-30);
        translateTransition.setCycleCount(1);


        RotateTransition rotateTransition =
                new RotateTransition(Duration.seconds(1), GtalkImage);
        rotateTransition.setByAngle(360f);
        rotateTransition.setCycleCount(1);

        ScaleTransition scaleTransition =
                new ScaleTransition(Duration.seconds(1), GtalkImage);
        scaleTransition.setToX(0.6f);
        scaleTransition.setToY(0.6f);
        scaleTransition.setCycleCount(1);


        sequentialTransition = new SequentialTransition();

        sequentialTransition1 = new SequentialTransition();
        parallelTransition = new ParallelTransition();
        parallelTransition.getChildren().addAll(
                translateTransition,
                rotateTransition,
                scaleTransition);
        stage.show();
        parallelTransition.setCycleCount(1);
        fadeTransition1.play();

        parallelTransition1.getChildren().addAll(loginTransition1,loginTransition2);
        sequentialTransition.getChildren().addAll(fadeTransition,
                parallelTransition, parallelTransition1);



        signIn.addEventHandler(MouseEvent.MOUSE_ENTERED,
                new EventHandler<MouseEvent>() {

                    @Override
                    public void handle(MouseEvent e) {
                        signIn.setEffect(shadow);
                    }
                });
//Removing the shadow when the mouse cursor is off
        signIn.addEventHandler(MouseEvent.MOUSE_EXITED,
                new EventHandler<MouseEvent>() {

                    @Override
                    public void handle(MouseEvent e) {
                        signIn.setEffect(null);
                    }
                });
        signIn.addEventHandler(MouseEvent.MOUSE_CLICKED,
                new EventHandler<MouseEvent>() {

                    @Override
                    public void handle(MouseEvent e) {
                        try {
                            gmailConnection.login(userNameField.getText(), passwordField.getText());
                        } catch (XMPPException ex) {
                            Logger.getLogger(Gtalk.class.getName()).log(Level.SEVERE, null, ex);
                        }
                        setGModel(new ContactInfoCollectionListModel(gmailConnection.getRoster()));
                        gmailConnection.getChatManager().addChatListener(new ChatManagerListener() {

                            @Override
                            public void chatCreated(org.jivesoftware.smack.Chat chat1, boolean bln) {
                                if (!bln) {
                                    final org.jivesoftware.smack.Chat chat = chat1;
                                    new Thread(new Runnable() {

                                        @Override
                                        public void run() {
                                            Platform.runLater(new Runnable() {

                                                @Override
                                                public void run() {
                                                    final FX.ChatBoxFX cf = new FX.ChatBoxFX();
                                                    chat.addMessageListener(new MessageListener() {

                                                        @Override
                                                        public void processMessage(org.jivesoftware.smack.Chat chat, org.jivesoftware.smack.packet.Message msg) {
                                                            cf.showMessage(msg.getBody());
                                                        }
                                                    });
                                                    cf.setChat(chat);
                                                    cf.setGtalk(true);
                                                    //cf.setvisible(true);
                                                    Stage stage = new Stage();
                                                    cf.start(stage);
                                                }
                                            });
                                        }
                                    }).start();


                                }
                            }
                        });



                        vbox.getChildren().remove(0);
                        vbox.getChildren().remove(box);
                        vbox.getChildren().addAll(GtalkImage);
                        sequentialTransition.play();

                        vbox.getChildren().addAll(rangi,listView);
                        vbox.setSpacing(
                                0);

                    }
                });

        listView.addEventHandler(MouseEvent.MOUSE_CLICKED, new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent evt) {
                if (evt.getClickCount() == 2) {
                    final String recep = listView.getSelectionModel().getSelectedItem();
                    
                    System.out.println(recep);

                    new Thread(new Runnable() {

                        @Override
                        public void run() {
                            Platform.runLater(new Runnable() {

                                @Override
                                public void run() {
                                    final FX.ChatBoxFX cf = new FX.ChatBoxFX();
                                    org.jivesoftware.smack.Chat chat = gmailConnection.getChatManager().createChat(recep + "@gmail.com", new MessageListener() {

                                        @Override
                                        public void processMessage(org.jivesoftware.smack.Chat chat, final org.jivesoftware.smack.packet.Message message) {
                                            Platform.runLater(new Runnable() {

                                                @Override
                                                public void run() {
                                                    cf.showMessage(message.getBody());
                                                }
                                            });
                                        }
                                    });
                                    cf.setChat(chat);
                                    cf.setGtalk(true);
                                    //cf.setvisible(true);
                                    Stage stage = new Stage();
                                    cf.start(stage);
                                    System.err.println("1");
                                }
                            });
                        }
                    }).start();





                }
            }
        });
        signIn.setStyle(
                "-fx-font: 13 arial; -fx-base: #ffa500;");

        vbox.getChildren().addAll(box);

    }

    
    public static void main(String[] args) {
        launch(args);
    }
}
