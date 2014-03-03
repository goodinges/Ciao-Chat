/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package FX;

/**
 *
 * @author Parvaneh
 */
import Authentication.authControl;
import ChatUtil.ChatController;
import UserInterface.ContactInfoCollectionListModel;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.AbstractMap.SimpleEntry;
import java.util.Map.Entry;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.event.EventType;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.effect.DropShadow;
import javafx.scene.effect.Effect;
import javafx.scene.effect.Glow;
import javafx.scene.effect.SepiaTone;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.*;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.TextAlignment;
import javafx.stage.Stage;
import javax.imageio.ImageIO;

public class LoginForm extends Application {

    ChatForm chatform;
    final Entry<String, Effect>[] effects = new Entry[]{
        new SimpleEntry<String, Effect>("Sepia Tone", new SepiaTone()),
        new SimpleEntry<String, Effect>("Glow", new Glow()),
        new SimpleEntry<String, Effect>("Shadow", new DropShadow())
    };
    final ImageView pic = new ImageView();
    final Label name = new Label();
    final Label binName = new Label();
    final Label description = new Label();
    private int currentIndex = -1;
    final VBox vbox = new VBox();
    public ImageView profileImage = new ImageView();
    public Image profilePic;
    public Label createAccount = new Label("    Get a new Ciao! account");
    public Label ForgotPass = new Label("     Forgot your password?");
    TextField userNameField = new TextField();
    PasswordField passwordField = new PasswordField();
    Button signIn = new Button("Sign In");
    ChoiceBox cb;
    GridPane grid = new GridPane();
    GridPane checkboxgrid = new GridPane();
    GridPane progressbarGrid = new GridPane();
    DropShadow shadow = new DropShadow();
    CheckBox cb1 = new CheckBox("Remember my password");
//A checkbox with a string caption
    CheckBox cb2 = new CheckBox("Sign in automatically");
    ProgressIndicator p1 = new ProgressIndicator();

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) {
        final Stage thisStage = stage;
        stage.setTitle("Ciao! Chat");
        Scene scene = new Scene(new VBox(), 300, 650);
        scene.setFill(Color.rgb(21, 51, 173));

        vbox.setAlignment(Pos.CENTER);
        vbox.setSpacing(15);
        vbox.setPadding(new Insets(90, 10, 0, 10));

        profileImage.setFitWidth(130);
        profileImage.setFitHeight(130);
        profileImage.setPreserveRatio(true);
        profilePic = new Image("src/UserInterface/profilePicLarge-icon.jpg");
        profileImage.setImage(profilePic);
        vbox.getChildren().add(profileImage);

        cb = new ChoiceBox(FXCollections.observableArrayList(
                "Online", "Busy", "Away", "Invisible"));
        //cb.setSkin(null);
        cb.setTooltip(new Tooltip("Change your status "));
        vbox.getChildren().add(cb);


        //hb.setSpacing(10);
        //hb.setAlignment(Pos.CENTER);

        createAccount.setFont(Font.font("Comic Sans MS ", 11));
        createAccount.setTextFill(Color.rgb(255, 179, 0));

        ForgotPass.setFont(Font.font("Comic Sans MS ", 11));
        ForgotPass.setTextFill(Color.rgb(255, 179, 0));

        grid.setPadding(new Insets(10, 10, 5, 70));
        checkboxgrid.setPadding(new Insets(10, 10, 5, 70));
        grid.setVgap(0);
        grid.setHgap(10);
        checkboxgrid.setVgap(5);

//userName text field
        userNameField.setPromptText("Enter your username");
        userNameField.setPrefColumnCount(10);
        userNameField.getText();
        GridPane.setConstraints(userNameField, 0, 0);
        grid.getChildren().add(userNameField);
        //hb.getChildren().add(userNameField);

        GridPane.setConstraints(createAccount, 0, 1);
        grid.getChildren().add(createAccount);


        grid.setVgap(10);

// password field
        passwordField.setPromptText("Enter your password");
        GridPane.setConstraints(passwordField, 0, 2);
        grid.getChildren().add(passwordField);
        //hb.getChildren().add(passwordField);

        GridPane.setConstraints(ForgotPass, 0, 3);
        grid.getChildren().add(ForgotPass);

//signIn button

        //GridPane.setConstraints(signIn, 0, 2);
        //grid.getChildren().add(signIn);

        //hb.getChildren().add(signIn);

        vbox.getChildren().add(grid);

        cb1.setSelected(true);
        cb2.setSelected(true);

        //vbox.getChildren().add(cb1);
        //vbox.getChildren().add(cb2);


        GridPane.setConstraints(cb1, 0, 1);
        checkboxgrid.getChildren().add(cb1);

        GridPane.setConstraints(cb2, 0, 2);
        checkboxgrid.getChildren().add(cb2);

        vbox.getChildren().add(checkboxgrid);
        vbox.getChildren().add(signIn);

        p1.setPrefSize(35, 35);
        p1.setOpacity(0);
        progressbarGrid.setPadding(new Insets(10, 10, 5, 125));
        progressbarGrid.getChildren().add(p1);
        vbox.getChildren().add(progressbarGrid);

        name.setFont(new Font("Verdana Bold", 22));
        binName.setFont(new Font("Arial Italic", 10));
        pic.setFitHeight(150);
        pic.setPreserveRatio(true);
        description.setWrapText(true);
        description.setTextAlignment(TextAlignment.JUSTIFY);


        MenuBar menuBar = new MenuBar();   //Set Color??

        // --- Menu File
        Menu menuFile = new Menu("File");
        MenuItem add = new MenuItem("Shuffle",
                new ImageView(new Image("src/menusample/new.png")));
        add.setOnAction(new EventHandler<ActionEvent>() {

            public void handle(ActionEvent t) {
                vbox.setVisible(true);
            }
        });
        MenuItem clear = new MenuItem("Clear");
        clear.setAccelerator(KeyCombination.keyCombination("Ctrl+X"));
        clear.setOnAction(new EventHandler<ActionEvent>() {

            public void handle(ActionEvent t) {
                vbox.setVisible(false);
            }
        });
        MenuItem exit = new MenuItem("Exit");
        exit.setOnAction(new EventHandler<ActionEvent>() {

            public void handle(ActionEvent t) {
                System.exit(0);
            }
        });


        menuFile.getItems().addAll(add, clear, new SeparatorMenuItem(), exit);
        // --- Menu Edit
        Menu menuEdit = new Menu("Contacts");

        // --- Menu View
        Menu menuView = new Menu("Actions");

        menuBar.getMenus().addAll(menuFile, menuEdit, menuView);
        ((VBox) scene.getRoot()).getChildren().addAll(menuBar, vbox);
        stage.setScene(scene);
        stage.show();

        // --- Creating four check menu items within the start method
        CheckMenuItem titleView = createMenuItem("Title", name);
        CheckMenuItem binNameView = createMenuItem("Binomial name", binName);
        CheckMenuItem picView = createMenuItem("Picture", pic);
        CheckMenuItem descriptionView = createMenuItem("Description", description);
        menuView.getItems().addAll(titleView, binNameView, picView, descriptionView);

        Menu menuEffect = new Menu("Picture Effect");
        final ToggleGroup groupEffect = new ToggleGroup();
        for (Entry<String, Effect> effect : effects) {
            RadioMenuItem itemEffect = new RadioMenuItem(effect.getKey());
            itemEffect.setUserData(effect.getValue());
            itemEffect.setToggleGroup(groupEffect);
            menuEffect.getItems().add(itemEffect);
        }
        final MenuItem noEffects = new MenuItem("No Effects");
        noEffects.setDisable(true);
        noEffects.setOnAction(new EventHandler<ActionEvent>() {

            public void handle(ActionEvent t) {
                pic.setEffect(null);
                groupEffect.getSelectedToggle().setSelected(false);
                noEffects.setDisable(true);
            }
        });

        groupEffect.selectedToggleProperty().addListener(new ChangeListener<Toggle>() {

            public void changed(ObservableValue<? extends Toggle> ov,
                    Toggle old_toggle, Toggle new_toggle) {
                if (groupEffect.getSelectedToggle() != null) {
                    Effect effect =
                            (Effect) groupEffect.getSelectedToggle().getUserData();
                    pic.setEffect(effect);
                    noEffects.setDisable(false);
                } else {
                    noEffects.setDisable(true);
                }
            }
        });
        menuEdit.getItems().addAll(menuEffect, noEffects);

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
        createAccount.setOnMouseEntered(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                createAccount.setScaleX(1.2);
                createAccount.setScaleY(1.2);
            }
        });

        createAccount.setOnMouseExited(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                createAccount.setScaleX(1);
                createAccount.setScaleY(1);
            }
        });
        createAccount.setOnMouseClicked(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent arg0) {
                CreateAccount ca = new CreateAccount();
                Stage stage = new Stage();
                ca.start(stage);
            }
        });
        ForgotPass.setOnMouseEntered(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                ForgotPass.setScaleX(1.2);
                ForgotPass.setScaleY(1.2);
            }
        });

        ForgotPass.setOnMouseExited(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                ForgotPass.setScaleX(1);
                ForgotPass.setScaleY(1);
            }
        });
        ForgotPass.setOnMouseClicked(new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent e) {
                ForgotPassword fg = new ForgotPassword();
                Stage stage = new Stage();
                fg.start(stage);
            }
        });
        signIn.addEventFilter(MouseEvent.MOUSE_CLICKED, new EventHandler<MouseEvent>() {

            public void handle(MouseEvent arg0) {
                p1.setOpacity(1);

                new Thread(new Runnable() {

                    public void run() {
                        Platform.runLater(new Runnable() {

                            @Override
                            public void run() {
                                final String id = userNameField.getText();
                                if (authControl.login(id, passwordField.getText().toCharArray())) {
                                    chatform = new ChatForm(new ContactInfoCollectionListModel(ChatController.getContacts(id)));
                                    String[] infos = new String[10];
                                    infos = authControl.getUserInfo(id, infos);
                                    chatform.setUserInfo(infos);
                                    //dispose();
                                    chatform.start(thisStage);
                                    new Thread(new Runnable() {

                                        public void run() {
                                            java.util.List<Byte> list = authControl.getProfilePic(id);

                                            byte[] bytes = new byte[list.size()];
                                            for (int i = 0; i < list.size(); i++) {
                                                bytes[i] = list.get(i);
                                            }
                                            File im = new File("profilepic.jpg");
                                            FileOutputStream fos;
                                            try {
                                                fos = new FileOutputStream(im);
                                                fos.write(bytes);
                                                fos.flush();
                                                fos.close();
                                                System.out.println(im.getAbsolutePath());

                                                InputStream in = new ByteArrayInputStream(bytes);
                                                BufferedImage image = ImageIO.read(in);
                                                //chatform.profilepicUpdate(image);

                                            } catch (IOException ex) {
                                                Logger.getLogger(authControl.class.getName()).log(Level.SEVERE, null, ex);
                                            }


                                        }
                                    }).start();

                                } else {
                                    //signInFailedLabel.setText("Incorrect username or password");
                                }
                            }
                        });

                    }
                }).start();

            }
        });
//Adding the shadow when the mouse cursor is on
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
        cb1.setStyle(
                "-fx-font-size: 11;");
        cb2.setStyle(
                "-fx-font-size: 11;");
        signIn.setStyle("-fx-font: 13 arial; -fx-base: #ffa500;");
        //  signIn.setStyle(
        //"-fx-font: bold italic 20pt Arial"
        //+ "-fx-effect: dropshadow( one-pass-box , black , 8 , 0.0 , 2 , 0 )");
    }

    private class PageData {

        public String name;
        public String description;
        public String binNames;
        public Image image;

        public PageData(String name, String description, String binNames) {
            this.name = name;
            this.description = description;
            this.binNames = binNames;
            image = new Image(getClass().getResourceAsStream(name + ".jpg"));
        }
    }
    // The createMenuItem method

    private static CheckMenuItem createMenuItem(String title, final Node node) {
        CheckMenuItem cmi = new CheckMenuItem(title);
        cmi.setSelected(true);
        cmi.selectedProperty().addListener(new ChangeListener<Boolean>() {

            public void changed(ObservableValue ov,
                    Boolean old_val, Boolean new_val) {
                node.setVisible(new_val);
            }
        });
        return cmi;
    }
}
