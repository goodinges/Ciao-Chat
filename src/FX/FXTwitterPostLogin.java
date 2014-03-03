package FX;

/**
 * Copyright (c) 2008, 2011 Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 */
/**
 * Copyright (c) 2008, 2011 Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 */
import javafx.animation.FadeTransition;
import javafx.animation.ParallelTransition;
import javafx.animation.SequentialTransition;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.event.EventHandler;
import javafx.animation.TranslateTransition;
import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.util.Duration;
import javax.swing.ImageIcon;

public class FXTwitterPostLogin extends Application {

    Label[] profilePics = new Label[20];
    TextArea selfTwitt = new TextArea();
    TextArea[] twitt = new TextArea[20];
    HBox[] twitts = new HBox[20];
    HBox self = new HBox();
    VBox array = new VBox();
    ProgressIndicator p1 = new ProgressIndicator();
    Image image1 = new Image("src/fx/birdflying1.png");
    Image image2 = new Image("src/fx/birdflying.png");
    Image image3 = new Image("src/fx/twit.png");
    Image image4 = new Image("src/fx/twitee.png");
    ImageView imageView = new ImageView();
    ImageView imageView1 = new ImageView();
    ImageView imageView2 = new ImageView();
    ImageView selfPic = new ImageView();
    private TranslateTransition translateTransition;
    private TranslateTransition translateTransition1;
    SequentialTransition sequentialTransition = new SequentialTransition();
    SequentialTransition sequentialTransition1 = new SequentialTransition();
    ParallelTransition parallelTransition1 = new ParallelTransition();
    VBox vbox = new VBox();

    private void init(Stage primaryStage) {
        ScrollPane scrollPane = new ScrollPane();
        final Group root = new Group();
        Scene scene = new Scene(root, 700, 550);
        scene.setFill(Color.rgb(50, 204, 254));
        // array.setPadding(new Insets(150, 20, 0, 20));
        scrollPane.getStyleClass().add("noborder-scroll-pane");
        scrollPane.setStyle("-fx-background-color:#32ccfe");
        scrollPane.setContent(array);
        scrollPane.setFitToWidth(true);
        // scrollPane.setPrefWidth(390);
        scrollPane.setPrefHeight(350);
        vbox.setAlignment(Pos.TOP_LEFT);
        vbox.setSpacing(100);
        vbox.getChildren().addAll(imageView2, scrollPane);


        for (int i = 0; i < 20; i++) {
            profilePics[i] = new Label("Test");
            twitt[i] = new TextArea("");
            twitts[i] = new HBox();
            twitts[i].setPadding(new Insets(20, 20, 0, 20));
            twitts[i].setSpacing(107);
        }
        for (int i = 0; i < 20; i++) {
            twitt[i].setEditable(false);
            twitt[i].setPrefSize(500, 25);
        }
        for (int j = 0; j < 20; j++) {
            twitts[j].getChildren().addAll(profilePics[j], twitt[j]);
        }
        array.getChildren().add(self);
        for (int z = 0; z < 20; z++) {
            array.getChildren().add(twitts[z]);
        }
        final Label label = new Label("Authorizing...");
        p1.setPrefSize(50, 50);
        p1.setTranslateX(320);
        p1.setTranslateY(250);
        label.setTranslateX(310);
        label.setTranslateY(315);
        label.setFont(new Font("Segoe Print Bold", 12));
        imageView.setImage(image1);
        imageView1.setImage(image2);
        imageView2.setImage(image3);
        selfPic.setImage(image4);
        selfPic.setFitWidth(50);
        selfPic.setFitHeight(50);
        selfPic.setPreserveRatio(true);
        selfTwitt.setText("Twitter....");
        selfTwitt.setEditable(false);
        selfTwitt.setPrefSize(500, 25);
        selfTwitt.setEditable(true);
        self.getChildren().addAll(selfPic, selfTwitt);
        self.setSpacing(100);
        primaryStage.setResizable(false);
        primaryStage.setScene(scene);
        imageView.setTranslateX(800);
        imageView.setTranslateY(50);
        imageView1.setTranslateX(-200);
        imageView1.setTranslateY(50);
        imageView2.setTranslateX(0);
        imageView2.setTranslateY(20);
        imageView2.setOpacity(0);
        root.getChildren().addAll(imageView, imageView1, p1, label, vbox);
        translateTransition = new TranslateTransition(Duration.seconds(4), imageView);
        translateTransition1 = new TranslateTransition(Duration.seconds(4), imageView1);
        translateTransition.setFromX(900);
        translateTransition.setToX(-250);
        translateTransition1.setFromX(-200);
        translateTransition1.setToX(800);
        translateTransition.setCycleCount(1);
        translateTransition1.setCycleCount(1);
        translateTransition.setAutoReverse(true);
        translateTransition1.setAutoReverse(true);
        sequentialTransition.getChildren().addAll(translateTransition, translateTransition1);
        sequentialTransition.setCycleCount(1);
        FadeTransition loginTransition =
                new FadeTransition(Duration.seconds(0.5), imageView2);
        loginTransition.setFromValue(0.0f);
        loginTransition.setToValue(1.0f);
        loginTransition.setCycleCount(1);
        parallelTransition1.getChildren().addAll(loginTransition);
        sequentialTransition1.getChildren().addAll(sequentialTransition, parallelTransition1);





        sequentialTransition.setOnFinished(new EventHandler<ActionEvent>() {

            public void handle(ActionEvent arg0) {
                root.getChildren().remove(0);
            }
        });
    }

    public void play() {
        sequentialTransition1.play();
    }

    @Override
    public void stop() {
        sequentialTransition1.stop();
    }

    public double getSampleWidth() {
        return 400;
    }

    public double getSampleHeight() {
        return 40;
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        init(primaryStage);
        primaryStage.show();
        play();
    }

    public static void main(String[] args) {
        launch(args);
    }

    public void setStatus(String trim) {
        selfTwitt.setText(trim);
    }

    public void seticon(ImageIcon icon) {
        //selfPic.setImage(icon.getImage());
    }
}
