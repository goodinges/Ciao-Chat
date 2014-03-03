/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package FX;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.effect.DropShadow;
import javafx.scene.effect.Reflection;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.stage.Stage;

/**
 *
 * @author Parvaneh
 */
public class AddFriends extends Application {

//    final String[] addfriendsOptions = new String[]{"Find friends on Ciao!", "Invite friends"};
//    final Image[] images = new Image[addfriendsOptions.length];
//    final ImageView[] pics = new ImageView[addfriendsOptions.length];
    VBox vbox = new VBox();
    Image image = new Image("C:/Users/Parvaneh/Desktop/logoo.jpg");
    ImageView imageView = new ImageView();
    TitledPane tps1 = new TitledPane();
    TitledPane tps2 = new TitledPane();
    Label label1 = new Label("Find friends in Ciao! Chat directory");
    Label label2 = new Label("      Enter your friend's name ,Ciao! ID , E-mail address.");
    Label label3 = new Label("Invite your friends to join you on Ciao! Chat");
    Label label4 = new Label("                                    Type E-mail addresses below.");
    Label label5 = new Label("                                 ( separate multiple addresses by ';' ) ");
    public ImageView findfriendImage = new ImageView();
    public ImageView addfriendImage = new ImageView();
    public Image findfriend = new Image("C:/Users/Parvaneh/Documents/Project/trunk/trunk/Ciao! Chat/src/UserInterface/search-icon.png");
    public Image addfriend = new Image("C:/Users/Parvaneh/Desktop/findfriend.png");
    Button find = new Button();
    Button invite = new Button("   Invite   ");
    DropShadow dropshadow = new DropShadow();
    Label label = new Label("Ciao Chat!");
    Text t = new Text();
    Reflection r = new Reflection();
    
    public static void main(String[] args) {
        launch(args);
    }
    
    @Override
    public void start(Stage stage) {
        label.setFont(new Font("Segoe Print Bold", 36));
        label.setTextFill(Color.rgb(255, 179, 0));
        label.setEffect(r);
        //label.setEffect(dropshadow);
        vbox.setPadding(new Insets(160, 150, 200, 170));
        imageView.setImage(image);
        vbox.setAlignment(Pos.TOP_LEFT);
        vbox.getChildren().add(label);
        stage.setTitle("Add Friends");
        Scene scene = new Scene(new Group(), 553, 390);
        scene.setFill(Color.rgb(21, 51, 173));
        
        
        final Accordion accordion = new Accordion();
        //vbox.setAlignment(Pos.CENTER);
        label1.setFont(Font.font("Gabriola", FontWeight.BOLD, 20));
        label2.setFont(Font.font("David", 17));
        label3.setFont(Font.font("Gabriola", FontWeight.BOLD, 20));
        label2.setFont(Font.font("David", 17));
        label2.setFont(Font.font("David", 14));
        label1.setTextFill(Color.rgb(21, 51, 173));
        label2.setTextFill(Color.rgb(0, 0, 0));
        label3.setTextFill(Color.rgb(21, 51, 173));
        label4.setTextFill(Color.rgb(0, 0, 0));
        label5.setTextFill(Color.rgb(255, 179, 0));
        findfriendImage.setImage(findfriend);
        find = new Button("  Find  ", new ImageView(findfriend));
        VBox vbox1 = new VBox();
        vbox1.setStyle(" -fx-fill: red;");
        GridPane grid1 = new GridPane();
        grid1.setVgap(25);
        grid1.setPadding(new Insets(50, 110, 125, 110));
        grid1.add(label1, 0, 0);
        grid1.add(label2, 0, 1);
        grid1.add(new TextField(), 0, 2);
        grid1.add(find, 0, 3);
        tps1.setText("Find friends on Ciao!");
        //vbox1.getChildren().add(grid1);
        //tps1.setGraphic(findfriendImage);
        tps1.setContent(grid1);
        
        GridPane grid2 = new GridPane();
        grid2.setVgap(20);
        grid2.setPadding(new Insets(50, 70, 125, 70));
        grid2.add(label3, 0, 0);
        grid2.add(label4, 0, 1);
        grid2.add(label5, 0, 2);
        grid2.add(new TextField(), 0, 3);
        grid2.add(invite, 0, 4);
        tps2.setText("Invite friends");
        tps2.setContent(grid2);

        //tps[0].setCollapsible(false);


//
//        for (int i = 0; i < addfriendsOptions.length; i++) {
//            images[i] = new Image(getClass().getResourceAsStream(addfriendsOptions[i] + ".jpg"));
//            pics[i] = new ImageView(images[i]);
//            tps[i] = new TitledPane(addfriendsOptions[i], pics[i]);
//        }
        accordion.getPanes().add(tps1);
        accordion.getPanes().add(tps2);
        accordion.setExpandedPane(tps1);
        invite.addEventHandler(MouseEvent.MOUSE_ENTERED,
                new EventHandler<MouseEvent>() {
                    
                    @Override
                    public void handle(MouseEvent e) {
                        invite.setEffect(dropshadow);
                    }
                });
//Removing the shadow when the mouse cursor is off
        invite.addEventHandler(MouseEvent.MOUSE_EXITED,
                new EventHandler<MouseEvent>() {
                    
                    @Override
                    public void handle(MouseEvent e) {
                        invite.setEffect(null);
                    }
                });
        
        find.addEventHandler(MouseEvent.MOUSE_ENTERED,
                new EventHandler<MouseEvent>() {
                    
                    @Override
                    public void handle(MouseEvent e) {
                        find.setEffect(dropshadow);
                    }
                });
//Removing the shadow when the mouse cursor is off
        find.addEventHandler(MouseEvent.MOUSE_EXITED,
                new EventHandler<MouseEvent>() {
                    
                    @Override
                    public void handle(MouseEvent e) {
                        find.setEffect(null);
                    }
                });
        
        addfriendImage.setImage(addfriend);
        //vbox.getChildren().add(addfriendImage);
        //addfriendImage.setOpacity(.5);
        Group root = (Group) scene.getRoot();
        //((VBox) scene.getRoot()).getChildren().add(vbox);
        // vbox.getChildren().add(t);
        root.getChildren().addAll(vbox,accordion);
        //root.getChildren().add(accordion);
        stage.setScene(scene);
        stage.show();
        
    }
}
