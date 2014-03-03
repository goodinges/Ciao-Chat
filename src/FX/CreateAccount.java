package FX;


import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.stage.Stage;
import javafx.util.Callback;

public class CreateAccount extends Application {

    HBox hbox1 = new HBox();
    HBox hbox2 = new HBox();
    HBox hbox3 = new HBox();
    HBox hbox4 = new HBox();
    HBox hbox5 = new HBox();
    HBox hbox6 = new HBox();
    HBox hbox7 = new HBox();
     HBox hbox8 = new HBox();
      HBox hbox9 = new HBox();
    VBox vbox1 = new VBox();
    VBox vbox2 = new VBox();
    Label id = new Label("*Ciao! ID :                                ");
    Label password = new Label("*Password :                             ");
    Label confirmPassword = new Label("*Confirm Password :               ");
    Label name = new Label("*Name :     ");
    Label gender = new Label("*Gender :    ");
    Label email = new Label("*Email :       ");
    Label birthday = new Label(" Birthday :  ");
    TextField ID = new TextField();
    TextField passwordTextField = new TextField();
    TextField confirmPasswordField = new TextField();
    TextField choicebox = new TextField();
    TextField firstname = new TextField();
    TextField birthday1 = new TextField();
    TextField birthday2 = new TextField();
    TextField birthday3 = new TextField();
    TextField lastname = new TextField();
    TextField emailTextField = new TextField();
    TextArea textArea1 = new TextArea("\n"+"\n"+"\n"+"\n"+"\n"+"    Account Information");
    TextArea textArea2 = new TextArea("\n"+"\n"+"\n"+"\n"+"\n"+"  Personal Information");
    ChoiceBox cb = new ChoiceBox(FXCollections.observableArrayList(
            "What was the name of your best childhood friend?", "What is your favorite food?, What is your pet's name?", "What is the name of your favorite book?", " Where did you spend your honeymoon?"));
    Button check = new Button("Check Availability");
    Button createAccount = new Button("Create Account");
    ProgressIndicator p1 = new ProgressIndicator();
    ToggleGroup tg = new ToggleGroup();
    RadioButton female= new RadioButton("female");
    RadioButton male = new RadioButton("male");
    

    @Override
    public void start(Stage stage) {

        VBox box = new VBox();
        Scene scene = new Scene(box, 800, 600);
        vbox1.setStyle("-fx-background-color:#ffffff");
        vbox2.setStyle("-fx-background-color:#ffffff");
        scene.setFill(Color.rgb(255,179, 0));
        choicebox.setMaxWidth(320);
        emailTextField.setMaxWidth(310);
        emailTextField.setPrefWidth(310);
        passwordTextField.setTooltip(new Tooltip("At least 6 characters "));
        birthday1.setPrefWidth(50);
        birthday1.setTooltip(new Tooltip("Day"));
        birthday2.setTooltip(new Tooltip("Month"));
        birthday3.setTooltip(new Tooltip("Year"));
        firstname.setTooltip(new Tooltip("First name"));
        lastname.setTooltip(new Tooltip("last name"));
        birthday2.setPrefWidth(50);
        birthday3.setPrefWidth(50);
        createAccount.setTranslateX(687);
        createAccount.setTranslateY(5);
        p1.setTranslateX(720);
        p1.setTranslateY(0);
        p1.setPrefSize(30, 30);
        textArea1.setStyle("-fx-font: 13 arial; -fx-base: #ffb300;-fx-background-color:#1533ad");
        textArea2.setStyle("-fx-font: 13 arial; -fx-base: #ffb300;-fx-background-color:#1533ad");
        hbox1.getChildren().addAll(id,ID,check);
        hbox2.getChildren().addAll(password,passwordTextField);
        hbox3.getChildren().addAll(confirmPassword,confirmPasswordField);
        vbox1.getChildren().addAll(hbox1,hbox2,hbox3,cb,choicebox);
        hbox4.getChildren().addAll(name,firstname,lastname);
        hbox5.getChildren().addAll(gender,female,male);
        hbox6.getChildren().addAll(birthday,birthday1,birthday2,birthday3);
        hbox7.getChildren().addAll(email,emailTextField);
        vbox2.getChildren().addAll(hbox4,hbox5,hbox6,hbox7);
        hbox8.getChildren().addAll(textArea1,vbox1);
        hbox9.getChildren().addAll(textArea2,vbox2);
        box.getChildren().addAll(hbox8,hbox9,createAccount,p1);
        hbox1.setSpacing(20);
        hbox2.setSpacing(20);
        hbox3.setSpacing(20);
        hbox4.setSpacing(20);
        hbox5.setSpacing(20);
        hbox6.setSpacing(20);
        hbox7.setSpacing(20);
vbox1.setSpacing(10);
vbox2.setSpacing(10);
        
        hbox8.setTranslateX(10);
        hbox8.setTranslateY(20);
            hbox9.setTranslateX(10);
        hbox9.setTranslateY(20);
        hbox8.setSpacing(30);
        hbox9.setSpacing(30);
        hbox8.setMaxSize(780,240);
       // hbox8.setMinWidth(780);
        hbox8.setPrefSize(780,240);
        hbox8.setPadding(new Insets(15,20,15,20));
        hbox9.setPadding(new Insets(15,20,15,20));
        hbox8.setStyle("-fx-background-color:#ffffff");
        hbox9.setPrefSize(780,240);
        hbox9.setMaxSize(780,240);
        hbox9.setStyle("-fx-background-color:#ffffff");
        box.setSpacing(20);
        textArea1.setMaxSize(150,200);
        textArea2.setMaxSize(150,200);
        textArea1.setPrefSize(150,200);
        textArea2.setPrefSize(150,200);
        textArea1.setEditable(false);
        textArea1.setEditable(false);
        
//        p1.setPrefWidth(150);
//        //p1.setPrefHeight(15);
////        p1.setLayoutX(d);
////        p1.setLayoutY(d);
//        p1.setProgress(1);
        female.setToggleGroup(tg);
        male.setToggleGroup(tg);

        stage.setScene(scene);
        stage.setTitle("Create Account");


        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}