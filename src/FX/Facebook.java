/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package FX;

/**
 * Copyright (c) 2008, 2011 Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 */
import java.util.Random;
import javafx.animation.*;
import javafx.scene.PerspectiveCamera;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.HPos;
import javafx.geometry.Orientation;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.Separator;
import javafx.scene.effect.DropShadow;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.RectangleBuilder;
import javafx.scene.transform.Rotate;
import javafx.util.Duration;


public class Facebook extends Application {

    Separator separator1 = new Separator();
    private Timeline animation;
    VBox vbox = new VBox();
    private static final Random RANDOM = new Random();
    private static final Interpolator INTERPOLATOR = Interpolator.SPLINE(0.295, 0.800, 0.305, 1.000);
    SequentialTransition sequentialTransition = new SequentialTransition();
    ParallelTransition parallelTransition = new ParallelTransition();

    private void init(Stage primaryStage) {
        final Group root = new Group();
        primaryStage.setResizable(false);
        Scene scene = new Scene(root, 700, 700, true);
        separator1.setOrientation(Orientation.HORIZONTAL);
        separator1.setPrefWidth(700);
        separator1.setHalignment(HPos.LEFT);
        separator1.setTranslateX(0);
        separator1.setTranslateY(90);

        MenuBar menuBar = new MenuBar();

        // --- Menu File
        Menu menuFile = new Menu("File");

        // --- Menu Edit
        Menu menuEdit = new Menu("Edit");

        // --- Menu View
        Menu menuView = new Menu("View");

        menuBar.getMenus().addAll(menuFile, menuEdit, menuView);
        final ImageView image6 = new ImageView();
        Image facebookImage6 = new Image("src/fx/facebookIcon.png");
        image6.setImage(facebookImage6);
        image6.setTranslateX(10);
        image6.setTranslateY(20);
        image6.setOpacity(0);
        separator1.setOpacity(0);
        //root.getTransforms().addAll(
        //      new Translate(400 / 2, 150 / 2),
        //    new Rotate(180, Rotate.X_AXIS));
        //root.getChildren().add(menuBar);
        scene.setFill(Color.rgb(21, 51, 173));
        primaryStage.setScene(scene);
        primaryStage.getScene().setCamera(new PerspectiveCamera());
        root.getChildren().add(create3dContent());
        root.getChildren().addAll(image6, separator1);
        animation.setOnFinished(new EventHandler<ActionEvent>() {

            public void handle(ActionEvent arg0) {
                root.getChildren().remove(0);
            }
        });
        FadeTransition loginTransition1 =
                new FadeTransition(Duration.seconds(4.0), separator1);
        loginTransition1.setFromValue(0.0f);
        loginTransition1.setToValue(1.0f);
        loginTransition1.setCycleCount(1);
        FadeTransition loginTransition =
                new FadeTransition(Duration.seconds(4.0), image6);
        loginTransition.setFromValue(0.0f);
        loginTransition.setToValue(1.0f);
        loginTransition.setCycleCount(1);
        parallelTransition.getChildren().addAll(loginTransition, loginTransition1);
        parallelTransition.setCycleCount(1);
        sequentialTransition.getChildren().addAll(animation, parallelTransition);
        separator1.setStyle(" -fx-background-color: #ffffff;-fx-background-radius: 3;");


        //parallelTransition.play();

    }

    public Node create3dContent() {
        final ImageView image1 = new ImageView();
        final ImageView image2 = new ImageView();
        final ImageView image3 = new ImageView();
        final ImageView image4 = new ImageView();
        final ImageView image5 = new ImageView();

        Image facebookImage1 = new Image("src/fx/facebook1.png");
        Image facebookImage2 = new Image("src/fx/facebook2.png");
        Image facebookImage3 = new Image("src/fx/facebook3.png");
        Image facebookImage4 = new Image("src/fx/facebook4.png");
        Image facebookImage5 = new Image("src/fx/facebook5.png");

        image1.setImage(facebookImage1);
        image2.setImage(facebookImage2);
        image3.setImage(facebookImage3);
        image4.setImage(facebookImage4);
        image5.setImage(facebookImage5);
        image1.setTranslateX(300);
        image1.setTranslateY(300);
        image2.setTranslateX(250);
        image2.setTranslateY(250);
        image3.setTranslateX(350);
        image3.setTranslateY(350);
        image4.setTranslateX(300);
        image4.setTranslateY(300);
        image5.setTranslateX(200);
        image5.setTranslateY(200);

        DropShadow effect = new DropShadow();
        effect.setRadius(0);
        effect.setOffsetY(1);
        effect.setColor(Color.WHITE);
        image1.setEffect(effect);
        image2.setEffect(effect);
        image3.setEffect(effect);
        image4.setEffect(effect);
        image5.setEffect(effect);




        animation = new Timeline();
        animation.getKeyFrames().addAll(
                new KeyFrame(Duration.seconds(3), new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                // we are done remove us from scene
                vbox.getChildren().remove(image1);

            }
        },
                new KeyValue(image1.translateXProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image1.translateYProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image1.opacityProperty(), 0f)),
                new KeyFrame(Duration.seconds(4), new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                // we are done remove us from scene
                vbox.getChildren().remove(image2);

            }
        },
                new KeyValue(image2.translateXProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image2.translateYProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image2.opacityProperty(), 0f)),
                new KeyFrame(Duration.seconds(4), new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                // we are done remove us from scene
                vbox.getChildren().remove(image3);

            }
        },
                new KeyValue(image3.translateXProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image3.translateYProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image3.opacityProperty(), 0f)),
                new KeyFrame(Duration.seconds(4), new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                // we are done remove us from scene
                vbox.getChildren().remove(image4);

            }
        },
                new KeyValue(image4.translateXProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image4.translateYProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image4.opacityProperty(), 0f)),
                new KeyFrame(Duration.seconds(3), new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent event) {
                // we are done remove us from scene
                vbox.getChildren().remove(image5);

            }
        },
                new KeyValue(image5.translateXProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image5.translateYProperty(), getRandom(0.0f, 500), INTERPOLATOR),
                new KeyValue(image5.opacityProperty(), 0f)));
        animation.setCycleCount(1);

        return new Group(image1, image2, image3, image4, image5);
    }

    public void play() {
        sequentialTransition.play();
    }

    @Override
    public void stop() {

        sequentialTransition.pause();

    }

    private static float getRandom(double min, double max) {
        return (float) (RANDOM.nextFloat() * (max - min) + min);
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
}
