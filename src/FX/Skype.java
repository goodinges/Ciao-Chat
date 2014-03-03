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
import Skype.SkypeController;
import UserInterface.ContactInfoCollectionListModel;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.animation.*;
import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.event.EventType;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.effect.DropShadow;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.stage.PopupWindow;
import javafx.stage.Stage;
import javafx.stage.Window;
import javafx.util.Duration;
import javax.swing.JOptionPane;

public class Skype extends Application {

    private ParallelTransition parallelTransition;
    private SequentialTransition sequentialTransition1;
    private SequentialTransition sequentialTransition;
    final ListView<String> listView = new ListView<String>();
    TextField userNameField = new TextField();
    PasswordField passwordField = new PasswordField();
    Button signIn = new Button("Sign In");
    Button signOut = new Button("Sign Out");
    GridPane grid = new GridPane();
    DropShadow shadow = new DropShadow();
    public Label notExist = new Label("    Username does not exists");
    ImageView SkypeImage = new ImageView();
    ImageView SkypeIcon = new ImageView();
    ImageView SkypeRain = new ImageView();
    Image skypeLogo = new Image("src/fx/skype.png");
    Image skypebg = new Image("src/fx/skypebg.png");
    Image skypeRainbow = new Image("src/fx/skypeRainbow.png");
    final Label name = new Label("salam");
    VBox box = new VBox();
    final VBox vbox = new VBox();

    @Override
    public void start(final Stage stage) throws Exception {
        stage.setTitle("Skype");

        Scene scene = new Scene(vbox, 320, 550);
        stage.setScene(scene);
        listView.setItems(FXCollections.observableArrayList(
                "Row 1", "Row 2", "Long Row 3", "Row 4", "Row 5", "Row 6",
                "Row 7", "Row 8", "Row 9", "Row 10", "Row 11", "Row 12", "Row 13",
                "Row 14", "Row 15", "Row 16", "Row 17", "Row 18", "Row 19", "Row 20"));
        listView.getSelectionModel().setSelectionMode(SelectionMode.MULTIPLE);
        SkypeImage.setImage(skypeLogo);
        SkypeIcon.setImage(skypebg);
        SkypeRain.setImage(skypeRainbow);
        listView.setOpacity(0);
        SkypeImage.setTranslateX(50);
        SkypeImage.setTranslateY(180);
        SkypeImage.setScaleX(1.1f);
        SkypeImage.setScaleY(1.1f);
        vbox.getChildren().add(SkypeIcon);


        box.setAlignment(Pos.CENTER);
        box.setSpacing(15);
        box.setPadding(new Insets(10, 10, 0, 10));
        box.setOpacity(0);

        grid.setPadding(new Insets(10, 10, 5, 70));
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

        box.getChildren().addAll(signIn);


        FadeTransition fadeTransition =
                new FadeTransition(Duration.seconds(2.0), SkypeImage);
        fadeTransition.setFromValue(0.1f);
        fadeTransition.setToValue(1.0f);
        fadeTransition.setCycleCount(1);

        //fadeTransition.setDuration(Duration.seconds(3));

        FadeTransition loginTransition =
                new FadeTransition(Duration.seconds(1.0), box);
        loginTransition.setFromValue(0.0f);
        loginTransition.setToValue(1.0f);
        loginTransition.setCycleCount(1);

        FadeTransition loginTransition1 =
                new FadeTransition(Duration.seconds(1.0), listView);
        loginTransition1.setFromValue(0.0f);
        loginTransition1.setToValue(1.0f);
        loginTransition1.setCycleCount(1);

        TranslateTransition translateTransition =
                new TranslateTransition(Duration.seconds(1), SkypeImage);
        translateTransition.setFromY(180);
        translateTransition.setToY(-30);
        //  translateTransition.setFromX(80);
        //translateTransition.setToX(0);
        translateTransition.setCycleCount(1);


        RotateTransition rotateTransition =
                new RotateTransition(Duration.seconds(1), SkypeImage);
        rotateTransition.setByAngle(360f);
        rotateTransition.setCycleCount(1);

        ScaleTransition scaleTransition =
                new ScaleTransition(Duration.seconds(1), SkypeImage);
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


        sequentialTransition.getChildren().addAll(fadeTransition,
                parallelTransition, loginTransition1);


        loginTransition.play();


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
        listView.addEventHandler(MouseEvent.MOUSE_CLICKED, new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent evt) {
                if (evt.getClickCount() == 2) {
                    String msgAuthor = listView.getSelectionModel().getSelectedItem();
                    //my code
                    SkypeController.startChat(msgAuthor);
                }
            }
        });
        final Skype fxSkype = this;
        signIn.addEventHandler(MouseEvent.MOUSE_CLICKED,
                new EventHandler<MouseEvent>() {

                    @Override
                    public void handle(MouseEvent e) {
                        SkypeController.setACCOUNT_NAME(userNameField.getText());
                        SkypeController.setACCOUNT_PWORD(passwordField.getText());
                        if (!SkypeController.Login(fxSkype)) {
                            String message = "Login failed!";
                            System.out.println(message);
                        } else {
                            vbox.getChildren().remove(0);
                            vbox.getChildren().remove(box);
                            vbox.setSpacing(0);
                            vbox.getChildren().addAll(SkypeRain, SkypeImage);
                            sequentialTransition.play();
                            vbox.getChildren().addAll(listView);
                            vbox.setSpacing(0);
                        }


                    }
                });
        signOut.setStyle("-fx-font: 13 arial; -fx-base: #ffa500;");
        signOut.addEventHandler(MouseEvent.MOUSE_CLICKED, new EventHandler<MouseEvent>() {

            @Override
            public void handle(MouseEvent arg0) {
                SkypeController.Logout();
                try {
                    fxSkype.stop();
                    stage.close();
                } catch (Exception ex) {
                    Logger.getLogger(Skype.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });

        signIn.setStyle("-fx-font: 13 arial; -fx-base: #ffa500;");

        vbox.getChildren().add(box);

    }

    public static void main(String[] args) {
        launch(args);
    }

    public void logedin(ContactInfoCollectionListModel contactInfoCollectionListModel) {
        listView.setItems(FXCollections.observableList(contactInfoCollectionListModel.vc));
////        jList1.updateUI();

    }
//    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {
//        this.dispose();
//        SkypeController.Logout();        // TODO add your handling code here:
//    }
}
