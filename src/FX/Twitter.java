package FX;

import Control.twt.TwitterClient;
import javafx.animation.FadeTransition;
import javafx.animation.RotateTransition;
import javafx.animation.SequentialTransition;
import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.geometry.VPos;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Hyperlink;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.Region;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.web.PopupFeatures;
import javafx.scene.web.WebEngine;
import javafx.scene.web.WebView;
import javafx.stage.Stage;
import javafx.util.Callback;
import javafx.util.Duration;
import javax.swing.ImageIcon;
import org.w3c.dom.Document;


public class Twitter extends Application {

    public TwitterClient twitterClient = new TwitterClient();
    SequentialTransition sequentialTransition = new SequentialTransition();
    static Browser browser;
    VBox box = new VBox();
    VBox vbox1 = new VBox();
    private Scene scene;
    Image bird = new Image("src/fx/twitter.png");
    ImageView image = new ImageView();

    public static void gotourl(String str) {
        browser.webEngine.load(str);
    }
    private Stage stage;

//    private void twittButtonActionPerformed(java.awt.event.ActionEvent evt) {
//        String rawStatus = TwittTextField.getText().trim();
//        String status = null;
//        try {
//            status = URLEncoder.encode(rawStatus, "UTF-8");
//        } catch (UnsupportedEncodingException ex) {
//            Logger.getLogger(TwitterForm.class.getName()).log(Level.SEVERE, null, ex);
//        }
//        twitterClient.client.makeOAuthRequestUnique();
//        try {
//            twitterClient.client.updateStatus(String.class, status, null);
//        } catch (UniformInterfaceException ex) {
//            System.out.println("Exception when calling updateStatus = " + ex.getResponse().getEntity(String.class));
//        }        // TODO add your handling code here:
//    }
//    void hideProgressBar() {
//        jProgressBar1.setVisible(false);
//        jList1.setModel(twitterClient.statusesListModel);
//    }
//    public void settext(String text) {
//        TwittTextField.setText(text);
//    }
//    public void seticon(ImageIcon icon) {
//        profilePic.setIcon(icon);
//    }
    @Override
    public void start(Stage stage) {
        this.stage = stage;
        browser = new Browser(twitterClient);
        twitterClient.login(this);

        // create scene
        stage.setTitle("Web View");
        scene = new Scene(box, 700, 550, Color.WHITE);
        stage.setScene(scene);
        scene.getStylesheets().add("webviewsample/BrowserToolbar.css");
        image.setImage(bird);


        vbox1.setAlignment(Pos.TOP_LEFT);
        vbox1.setSpacing(10);
        vbox1.setPadding(new Insets(0, 0, 0, 0));
        vbox1.setOpacity(0);
        box.getChildren().add(image);

        FadeTransition loginTransition =
                new FadeTransition(Duration.seconds(0.5), vbox1);
        loginTransition.setFromValue(0.0f);
        loginTransition.setToValue(1.0f);
        loginTransition.setCycleCount(1);
        FadeTransition loginTransition1 =
                new FadeTransition(Duration.seconds(1.0), image);
        loginTransition1.setFromValue(1.0f);
        loginTransition1.setToValue(0.0f);
        loginTransition1.setCycleCount(1);

        RotateTransition rotateTransition =
                new RotateTransition(Duration.seconds(3), image);
        rotateTransition.setByAngle(360f);
        rotateTransition.setCycleCount(1);


        // show stage

        stage.show();

        sequentialTransition.getChildren().addAll(rotateTransition, loginTransition1,
                loginTransition);
        sequentialTransition.setCycleCount(1);

        sequentialTransition.play();

        vbox1.getChildren().addAll(browser);


        box.getChildren().add(vbox1);


        sequentialTransition.setOnFinished(new EventHandler<ActionEvent>() {

            public void handle(ActionEvent arg0) {
                box.getChildren().remove(0);
            }
        });
    }

    public static void main(String[] args) {
        launch(args);
    }

    public void settext(String trim) {
        throw new UnsupportedOperationException("Not yet implemented");
    }

    public void seticon(ImageIcon icon) {
        throw new UnsupportedOperationException("Not yet implemented");
    }

    public Stage getStage() {
        return stage;
    }
}
class Browser extends Region {

    private HBox toolBar = new HBox();
    private static String[] imageFiles = new String[]{
        "product.png",
        "blog.png",
        "forum.png",
        "partners.png"
    };
    private static String[] captions = new String[]{
        "Products",
        "Blogs",
        "Forums",
        "Partners"
    };
    private static String[] urls = new String[]{
        "http://www.oracle.com/products/index.html",
        "http://blogs.oracle.com/",
        "http://forums.oracle.com/forums/main.jspa",
        "http://www.oracle.com/partners/index.html"
    };
    final ImageView selectedImage = new ImageView();
    final Hyperlink[] hpls = new Hyperlink[captions.length];
    final Image[] images = new Image[imageFiles.length];
    final WebView browser = new WebView();
    final WebEngine webEngine = browser.getEngine();
    final Button hideAll = new Button("Hide All");
    final Button showAll = new Button("ShowAll");
    final WebView smallView = new WebView();
    TwitterClient twitterClient;
    public Browser(final TwitterClient twitterClient) {
        this.twitterClient = twitterClient;
        //apply the styles
        getStyleClass().add("browser");
        webEngine.documentProperty().addListener(new ChangeListener<Document>() {
            
            @Override
            public void changed(ObservableValue<? extends Document> observable, Document oldDoc, Document newDoc) {
                if (oldDoc != null){
                    if (oldDoc.getBaseURI().startsWith("https://twitter.com/oauth/authorize?")){
                        
                    }
                }
                if (newDoc != null) {
                    if ("https://twitter.com/oauth/authorize".equals(newDoc.getBaseURI())) {
                        String pin = newDoc.getElementsByTagName("code").item(0).getTextContent();
                        System.out.println(pin);
                        twitterClient.enterPinContinue(pin);
                    }
                }
                //System.out.println(newDoc.getElementsByTagName("CODE").item(0).getNodeValue());
            }
        });
        // load the home page        

        for (int i = 0; i < captions.length; i++) {
            final Hyperlink hpl = hpls[i] = new Hyperlink(captions[i]);

            final Image image = images[i] =
                    new Image(getClass().getResourceAsStream(imageFiles[i]));
            hpl.setGraphic(new ImageView(image));
            final String url = urls[i];

            hpl.setOnAction(new EventHandler<ActionEvent>() {

                @Override
                public void handle(ActionEvent e) {
                    webEngine.load(url);
                    if (hpl.getText().equals("Forums")) {
                        toolBar.getChildren().removeAll(showAll, hideAll);
                        toolBar.getChildren().addAll(showAll, hideAll);
                    } else {
                        toolBar.getChildren().removeAll(showAll, hideAll);
                    }
                }
            });

        }

        // create the toolbar


        hideAll.setOnAction(new EventHandler() {

            @Override
            public void handle(Event t) {
                webEngine.executeScript("hideAll()");
            }
        });


        showAll.setOnAction(new EventHandler() {

            @Override
            public void handle(Event t) {
                webEngine.executeScript("showAll()");
            }
        });

        smallView.setPrefSize(120, 80);
        webEngine.setCreatePopupHandler(
                new Callback<PopupFeatures, WebEngine>() {

                    @Override
                    public WebEngine call(PopupFeatures config) {
                        smallView.setFontScale(0.8);
                        if (!toolBar.getChildren().contains(smallView)) {
                            toolBar.getChildren().add(smallView);
                        }
                        return smallView.getEngine();
                    }
                });

        //add components
        // getChildren().add(toolBar);
        getChildren().add(browser);


    }


    private Node createSpacer() {
        Region spacer = new Region();
        HBox.setHgrow(spacer, Priority.ALWAYS);
        return spacer;
    }

    @Override
    protected void layoutChildren() {
        double w = getWidth();
        double h = getHeight();
        double tbHeight = toolBar.prefHeight(w);
        layoutInArea(browser, 0, 0, w, h - tbHeight, 0, HPos.CENTER, VPos.CENTER);
        layoutInArea(toolBar, 0, h - tbHeight, w, tbHeight, 0, HPos.CENTER, VPos.CENTER);
    }

    @Override
    protected double computePrefWidth(double height) {
        return 800;
    }

    @Override
    protected double computePrefHeight(double width) {
        return 600;
    }
}
