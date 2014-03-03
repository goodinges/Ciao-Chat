/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package FX;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.scene.web.HTMLEditor;
import javafx.stage.Stage;
 
public class Message extends Application {
    
    @Override
    public void start(Stage stage) {
        stage.setTitle("Message Composing");
        stage.setWidth(500);
        stage.setHeight(500);
        Scene scene = new Scene(new Group());
    
        final VBox root = new VBox();        
        root.setPadding(new Insets(8, 8, 8, 8));
        root.setSpacing(5);
        root.setAlignment(Pos.BOTTOM_LEFT);
        
        final GridPane grid = new GridPane();
        grid.setVgap(5);
        grid.setHgap(10);
              
        final ChoiceBox sendTo = 
            new ChoiceBox(FXCollections.observableArrayList(
                "To:", "Cc:", "Bcc:")
        );
        
        sendTo.setPrefWidth(100);                
        GridPane.setConstraints(sendTo, 0, 0);
        grid.getChildren().add(sendTo);
        
        final TextField tbTo = new TextField();
        tbTo.setPrefWidth(400);
        GridPane.setConstraints(tbTo, 1, 0);
        grid.getChildren().add(tbTo);
        
        final Label subjectLabel = new Label("Subject:");
        GridPane.setConstraints(subjectLabel, 0, 1);
        grid.getChildren().add(subjectLabel);        
        
        final TextField tbSubject = new TextField();
        tbTo.setPrefWidth(400);
        GridPane.setConstraints(tbSubject, 1, 1);
        grid.getChildren().add(tbSubject);
        
        root.getChildren().add(grid);
        
        final HTMLEditor htmlEditor = new HTMLEditor();
        htmlEditor.setPrefHeight(370);
 
        root.getChildren().addAll(htmlEditor, new Button("Send"));        
      
        final Label htmlLabel = new Label();
        htmlLabel.setWrapText(true);
                      
        scene.setRoot(root);
        stage.setScene(scene);
        stage.show();
    }
 
    public static void main(String[] args) {
        launch(args);
    }
}