/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package FX;

import ChatUtil.ChatController;
import ChatUtil.ChatListener;
import UserInterface.ContactInfoCollectionListModel;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.event.EventType;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.*;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;
import org.jivesoftware.smack.Connection;
import org.jivesoftware.smack.XMPPConnection;
import org.jivesoftware.smack.XMPPException;

/**
 *
 * @author Tannaz
 */
public class ChatForm extends Application {

    private static ContactInfoCollectionListModel listmodel;
    private Connection gmailConnection;

    public static ContactInfoCollectionListModel getListmodel() {
        return listmodel;
    }

    public static void setListmodel(ContactInfoCollectionListModel listmodel) {
        ChatForm.listmodel = listmodel;
    }
    final VBox vbox = new VBox();
    public ImageView profileImage = new ImageView();
    public Image profilePic;
    public ImageView addFriend = new ImageView();
    public Image addFriendIcon;
    public ImageView twitter = new ImageView();
    public Image twitterImage = new Image("src/fx/twitterIcon.png");
    public ImageView skype = new ImageView();
    public Image skypeImage = new Image("src/fx/skypeIcon.png");
    public ImageView Gtalk = new ImageView();
    public Image gtalkImage = new Image("src/fx/GtalkBlueIcon.png");
    public ImageView facebook = new ImageView();
    public Image facebookImage = new Image("src/fx/facebook-icon.png");
    public ImageView friendfeed = new ImageView();
    public Image friendfeedImage = new Image("src/fx/FriendfeedIcon.png");
    public String Name = "FirstName LastName ";
    public Label nameL = new Label(Name);
    final ListView<String> listView = new ListView<String>();

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }

    public ChatForm() {
        new ChatListener().start();
    }
    Image img;

    public ChatForm(ContactInfoCollectionListModel listmodel) {
        ChatForm.listmodel = listmodel;
        new ChatListener().start();
    }

    public void rersizeImage() {
//        BufferedImage resizedImage = new BufferedImage(80, 80, BufferedImage.TYPE_INT_RGB);
//        Graphics2D g = resizedImage.createGraphics();
//        g.drawImage(img, 0, 0, 80, 80, null);
//        g.dispose();
//        jLabel2.setIcon(new ImageIcon(resizedImage.getScaledInstance(80, 80, 0)));
    }

    @Override
    public void start(Stage stage) {
        // --- main screen ---


        stage.setTitle("Ciao! Chat");
        Scene scene = new Scene(vbox, 300, 650);
        scene.setFill(Color.rgb(21, 51, 173));
        stage.setScene(scene);
        stage.show();

        // --- Adding menu ---

        MenuBar menuBar = new MenuBar();
        // --- File Menu ---
        Menu menuFile = new Menu("File");

        Menu changeStatus = new Menu("Change Status");

        MenuItem OnlineI = new MenuItem("Online");
        MenuItem BusyI = new MenuItem("Busy");
        MenuItem AwayI = new MenuItem("Away");
        MenuItem InvisibleI = new MenuItem("Invisible");

        changeStatus.getItems().addAll(OnlineI, BusyI, AwayI, InvisibleI);

        MenuItem settingsMenu = new MenuItem("Settings");

        MenuItem signOutMenu = new MenuItem("Sign Out");

        menuFile.getItems().addAll(changeStatus, settingsMenu, signOutMenu);

        // --- Contacts menu ---
        Menu menuContacts = new Menu("Contacts");

        // --- Actions Menu ---

        Menu menuActions = new Menu("Actions");

        // --- Help ---

        Menu menuHelp = new Menu("Help");
        MenuItem ciaoI = new MenuItem("Ciao!Help");
        MenuItem aboutI = new MenuItem("About");
        // --------------
        menuBar.getMenus().addAll(menuFile, menuContacts, menuActions, menuHelp);
        vbox.getChildren().addAll(menuBar);


        // --- Top Menu ---

        facebook.setImage(facebookImage);
        twitter.setImage(twitterImage);
        skype.setImage(skypeImage);
        Gtalk.setImage(gtalkImage);
        friendfeed.setImage(friendfeedImage);
        profileImage.setFitWidth(80);
        profileImage.setFitHeight(80);
        profileImage.setPreserveRatio(true);
        profilePic = new Image("src/UserInterface/profilePicLarge-icon.jpg");
        addFriendIcon = new Image("src/fx/addfriend-icon.jpg");
        addFriend.setImage(addFriendIcon);
        profileImage.setImage(profilePic);
        listView.setItems(FXCollections.observableArrayList(
                "Row 1", "Row 2", "Long Row 3", "Row 4", "Row 5", "Row 6",
                "Row 7", "Row 8", "Row 9", "Row 10", "Row 11", "Row 12", "Row 13",
                "Row 14", "Row 15", "Row 16", "Row 17", "Row 18", "Row 19", "Row 20"));
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
                                    ChatController.newChat(recep);

                                }
                            });
                        }
                    }).start();





                }
            }
        });


        HBox hbox = new HBox();
        HBox hbox1 = new HBox();
        VBox vbox1 = new VBox();
        HBox icons = new HBox();
        icons.setTranslateX(0);
        icons.setTranslateY(-80);
        icons.setSpacing(20);
        icons.setPadding(new Insets(0, 10, 0, 10));
        icons.getChildren().addAll(facebook, friendfeed, skype, Gtalk, twitter);

        final Label addfriend = new Label("\n" + "Add friends");
        addfriend.setTextFill(Color.rgb(255, 179, 0));
        //nameL.setFont(Font.font("Gabriola", FontWeight.BOLD, 16));
        nameL.setTextFill(Color.rgb(255, 179, 0));
        ChoiceBox cb = new ChoiceBox(FXCollections.observableArrayList(
                "Online", "Busy", "Away", "Invisible"));
        TextField text = new TextField();
        text.setPromptText("Status... ");
        text.setPrefColumnCount(50);
        text.getText();
        text.setMaxWidth(500);

        Rectangle rec = new Rectangle(290, 100, Color.WHITE);
        Rectangle rec1 = new Rectangle(290, 430, Color.WHITE);
        Label addIcon = new Label("Add Friends");
        rec.setTranslateX(5);
        rec.setTranslateY(10);
        rec1.setTranslateX(5);
        rec1.setTranslateY(-20);
//        listView.setTranslateX(10);
//        listView.setTranslateY(-500);
        //rec.toBack();


        TextField findFtxt = new TextField();
        findFtxt.setPromptText("Find someone in my contact list ");
        findFtxt.setPrefColumnCount(50);
        findFtxt.getText();
        findFtxt.setMaxWidth(280);

        VBox vbox2 = new VBox();
        HBox hbox2 = new HBox();
        VBox vbox3 = new VBox();
        vbox3.setMinHeight(430);
        vbox3.setPrefSize(290, 430);
        vbox3.setMaxSize(290, 430);
        vbox3.setStyle("-fx-background-color:#ffffff");
        vbox3.setTranslateX(5);
        vbox3.setTranslateY(-50);
        hbox2.getChildren().addAll(addFriend, addfriend);
        hbox2.setSpacing(5);
        listView.setPrefWidth(280);
        listView.setMaxWidth(280);
        listView.setPrefHeight(360);
        listView.setMaxHeight(360);
        vbox2.setSpacing(15);
        vbox2.getChildren().addAll(hbox2, listView);
        vbox2.setTranslateX(5);
        vbox2.setTranslateY(0);
        vbox3.getChildren().add(vbox2);
        vbox3.setPadding(new Insets(5, 0, 0, 0));



        hbox.setPadding(new Insets(20, 10, 10, 10));
        hbox.setSpacing(10);
        vbox1.setSpacing(35);


        hbox1.getChildren().addAll(nameL, cb);
        vbox1.getChildren().addAll(hbox1, text);
        hbox.getChildren().addAll(profileImage, vbox1);
        hbox.setTranslateY(-100);
        vbox.getChildren().addAll(rec, hbox, icons, vbox3);

        final ContextMenu cm = new ContextMenu();
        MenuItem cmItem1 = new MenuItem("Change profile image");
        cmItem1.setOnAction(new EventHandler<ActionEvent>() {

            public void handle(ActionEvent e) {
                Clipboard clipboard = Clipboard.getSystemClipboard();
                ClipboardContent content = new ClipboardContent();
                content.putImage(profileImage.getImage());
                clipboard.setContent(content);
            }
        });

        cm.getItems().add(cmItem1);
        profileImage.addEventHandler(MouseEvent.MOUSE_CLICKED,
                new EventHandler<MouseEvent>() {

                    @Override
                    public void handle(MouseEvent e) {
                        if (e.getButton() == MouseButton.SECONDARY) {
                            cm.show(profileImage, e.getScreenX(), e.getScreenY());
                        }
                    }
                });
        addfriend.setOnMouseEntered(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                addfriend.setScaleX(1.2);
                addfriend.setScaleY(1.2);
            }
        });

        addfriend.setOnMouseExited(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                addfriend.setScaleX(1);
                addfriend.setScaleY(1);
            }
        });
        addfriend.setOnMouseClicked(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                AddFriends af = new AddFriends();
                Stage stage = new Stage();
                af.start(stage);
            }
        });
        facebook.setOnMouseEntered(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                facebook.setScaleX(1.2);
                facebook.setScaleY(1.2);
            }
        });

        facebook.setOnMouseExited(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                facebook.setScaleX(1);
                facebook.setScaleY(1);
            }
        });
        facebook.setOnMouseClicked(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent arg0) {
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        Platform.runLater(new Runnable() {

                            @Override
                            public void run() {
                                Facebook f = new Facebook();
                                Stage stage = new Stage();
                                try {
                                    f.start(stage);
                                } catch (Exception ex) {
                                    Logger.getLogger(ChatForm.class.getName()).log(Level.SEVERE, null, ex);
                                }
                            }
                        });
                    }
                }).start();
//                try {
//                    new UserInterface.Facebook().setVisible(true);
//                } catch (URISyntaxException ex) {
//                    Logger.getLogger(ChatForm.class.getName()).log(Level.SEVERE, null, ex);
//                } catch (IOException ex) {
//                    Logger.getLogger(ChatForm.class.getName()).log(Level.SEVERE, null, ex);
//                }

            }
        });
        twitter.setOnMouseEntered(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                twitter.setScaleX(1.2);
                twitter.setScaleY(1.2);
            }
        });

        twitter.setOnMouseExited(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                twitter.setScaleX(1);
                twitter.setScaleY(1);
            }
        });
        twitter.setOnMouseClicked(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent arg0) {
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        Platform.runLater(new Runnable() {

                            @Override
                            public void run() {
                                Twitter twitter = new Twitter();
                                Stage stage = new Stage();
                                twitter.start(stage);
                            }
                        });
                    }
                }).start();
            }
        });
        skype.setOnMouseEntered(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                skype.setScaleX(1.2);
                skype.setScaleY(1.2);
            }
        });

        skype.setOnMouseExited(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                skype.setScaleX(1);
                skype.setScaleY(1);
            }
        });
        skype.setOnMouseClicked(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent arg0) {
                new Thread(new Runnable() {

                    public void run() {
                        try {
                            Runtime.getRuntime().exec("resources\\skype\\windows-x86-skypekit-novideo_4.0.2.1195_710188\\bin\\windows-x86\\windows-x86-skypekit-novideo.exe");
                        } catch (IOException ex) {
                            Logger.getLogger(ChatForm.class.getName()).log(Level.SEVERE, null, ex);
                        }
                    }
                }).start();
                Platform.runLater(new Runnable() {

                    @Override
                    public void run() {
                        Skype fxSkype = new Skype();
                        Stage s = new Stage();
                        try {
                            fxSkype.start(s);
                        } catch (Exception ex) {
                            Logger.getLogger(ChatForm.class.getName()).log(Level.SEVERE, null, ex);
                        }
                    }
                });
            }
        });
        Gtalk.setOnMouseEntered(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                Gtalk.setScaleX(1.2);
                Gtalk.setScaleY(1.2);
            }
        });

        Gtalk.setOnMouseExited(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                Gtalk.setScaleX(1);
                Gtalk.setScaleY(1);
            }
        });
        Gtalk.setOnMouseClicked(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent arg0) {
                gmailConnection = new XMPPConnection("google.com");
                try {
                    gmailConnection.connect();
                } catch (XMPPException ex) {
                    Logger.getLogger(ChatForm.class.getName()).log(Level.SEVERE, null, ex);
                }
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        Platform.runLater(new Runnable() {

                            @Override
                            public void run() {
                                FX.Gtalk gl = new FX.Gtalk(gmailConnection);
                                Stage stage = new Stage();
                                try {
                                    gl.start(stage);
                                } catch (Exception ex) {
                                    Logger.getLogger(ChatForm.class.getName()).log(Level.SEVERE, null, ex);
                                }

                            }
                        });
                    }
                }).start();

            }
        });
        friendfeed.setOnMouseEntered(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                friendfeed.setScaleX(1.2);
                friendfeed.setScaleY(1.2);
            }
        });

        friendfeed.setOnMouseExited(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                friendfeed.setScaleX(1);
                friendfeed.setScaleY(1);
            }
        });

        
        listView.setItems(FXCollections.observableList(listmodel.vc));
    }

    public void setUserInfo(String[] infos) {
        nameL.setText(infos[0] + " " + infos[1]);
    }

    public String getUserInfo() {
        return nameL.getText();
    }

    void setGModel(ContactInfoCollectionListModel contactInfoCollectionListModel) {
    }

    public void profilepicUpdate(Image img) throws InterruptedException {
        this.img = img;
        rersizeImage();
    }
}
