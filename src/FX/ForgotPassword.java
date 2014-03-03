/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package FX;

/**
 *
 * @author Parvaneh
 */
import javafx.application.Application;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.effect.DropShadow;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.stage.Stage;

public class ForgotPassword extends Application {
    TextField textField = new TextField();

    private HBox hb = new HBox();

    public static void main(String[] args) {
        launch(args);

    }

    @Override
    public void start(Stage stage) {
        final DropShadow shadow = new DropShadow();
        Scene scene = new Scene(new Group());
        // scene.getStylesheets().add("Buttons.css");
        scene.setFill(Color.rgb(21, 51, 173));
        stage.setTitle("Forgot your Password?");
        stage.setWidth(400);
        stage.setHeight(200);
        textField.setMaxWidth(350);
        textField.setPrefWidth(350);

        final Label label = new Label("To get your account details enter your username :");
        label.setFont(new Font("Arial Bold Italic", 14));
        label.setTextFill(Color.rgb(255, 179, 0));

        final Button addButton = new Button("Send Information");
        addButton.setTranslateX(230);
        addButton.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent e) {
            }
        });
        //Adding the shadow when the mouse cursor is on
        addButton.addEventHandler(MouseEvent.MOUSE_ENTERED,
                new EventHandler<MouseEvent>() {

                    @Override
                    public void handle(MouseEvent e) {
                        addButton.setEffect(shadow);
                    }
                });
//Removing the shadow when the mouse cursor is off
        addButton.addEventHandler(MouseEvent.MOUSE_EXITED,
                new EventHandler<MouseEvent>() {

                    @Override
                    public void handle(MouseEvent e) {
                        addButton.setEffect(null);
                    }
                });

        //addButton.setStyle("-fx-font: 13 arial; -fx-base: #ffa500;");
        hb.setAlignment(Pos.CENTER);
        //hb.setPadding(new Insets(0,280,0,0));
        hb.getChildren().addAll(addButton);
        hb.setSpacing(5);

        final VBox vbox = new VBox();
        vbox.setSpacing(25);
        
        vbox.getChildren().addAll(label,textField,addButton);
        vbox.setPadding(new Insets(20,10,10,10));
        addButton.setStyle("-fx-font: 13 arial; -fx-base: #ffa500;");

        ((Group) scene.getRoot()).getChildren().addAll(vbox);

        stage.setScene(scene);
        stage.show();
    }
}
