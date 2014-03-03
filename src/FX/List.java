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

public class List extends Application {

    public static class Person {

        private final StringProperty name;
        private final StringProperty id;
        private final StringProperty email;

        private Person(String name, String id, String email) {
            this.name = new SimpleStringProperty(name);
            this.id = new SimpleStringProperty(id);
            this.email = new SimpleStringProperty(email);
        }

        public String getName() {
            return name.get();
        }

        public String getID() {
            return id.get();
        }

        public String getEmail() {
            return email.get();
        }
    }
    private TableView<Person> table = new TableView<Person>();
    private final ObservableList<Person> data =
            FXCollections.observableArrayList(
            new Person("Jacob", "Smith", "jacob.smith@example.com"),
            new Person("Isabella", "Johnson", "isabella.johnson@example.com"),
            new Person("Ethan", "Williams", "ethan.williams@example.com"),
            new Person("Emma", "Jones", "emma.jones@example.com"),
            new Person("Michael", "Brown", "michael.brown@example.com"));
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
        stage.setTitle("Search");
        stage.setWidth(378);
        stage.setHeight(485);


        final Label label = new Label("Search Result");
        label.setFont(new Font("Arial Bold Italic", 16));
        label.setTextFill(Color.rgb(255, 179, 0));

        TableColumn nameCol = new TableColumn("Name");
        nameCol.setCellValueFactory(
                new PropertyValueFactory<Person, String>("name"));

        TableColumn idCol = new TableColumn("ID");
        idCol.setCellValueFactory(
                new PropertyValueFactory<Person, String>("id"));

        TableColumn emailCol = new TableColumn("Email");
        emailCol.setMinWidth(200);
        emailCol.setCellValueFactory(
                new PropertyValueFactory<Person, String>("email"));

        table.setItems(data);

        table.getColumns().addAll(nameCol, idCol, emailCol);


        //ButtonBuilder.create().text("Add").id("record-sales").build();
        final Button addButton = new Button("   Add   ");
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
        vbox.setSpacing(10);
        vbox.getChildren().addAll(table);
        vbox.setPadding(new Insets(0, 0, 0, 0));
        
        vbox.getChildren().add(hb);

        ((Group) scene.getRoot()).getChildren().addAll(vbox);

        stage.setScene(scene);
        stage.show();
    }
}
