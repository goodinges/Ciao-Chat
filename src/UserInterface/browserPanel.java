/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package UserInterface;

import Control.twt.TwitterClient;
import chrriis.dj.nativeswing.swtimpl.components.JWebBrowser;
import chrriis.dj.nativeswing.swtimpl.components.WebBrowserAdapter;
import chrriis.dj.nativeswing.swtimpl.components.WebBrowserNavigationEvent;
import com.sun.jersey.api.client.UniformInterfaceException;
import java.awt.BorderLayout;
import java.net.MalformedURLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;
import javax.swing.JPanel;

/**
 *
 * @author Kamran
 */
public class browserPanel extends JPanel {
public JWebBrowser webBrowser;
    private TwitterClient twitterClient;
    private TwitterForm twitterForm;
    private JFrame jframe;
    public browserPanel() {
        super(new BorderLayout());
        webBrowser = new JWebBrowser();
        webBrowser.navigate("http://www.google.com");
        webBrowser.setBarsVisible(false);
        add(webBrowser, BorderLayout.CENTER);
        webBrowser.addWebBrowserListener(new WebBrowserAdapter() {

            @Override
            public void locationChanged(WebBrowserNavigationEvent e) {
                System.out.println(e.getNewResourceLocation());
            }
        });
    }

    public browserPanel(String string) {
        super(new BorderLayout());
        webBrowser = new JWebBrowser();
        webBrowser.navigate(string);
        webBrowser.setBarsVisible(false);
        add(webBrowser, BorderLayout.CENTER);
        webBrowser.addWebBrowserListener(new WebBrowserAdapter() {

            @Override
            public void locationChanging(WebBrowserNavigationEvent e) {
                super.locationChanging(e);
                if("https://twitter.com/oauth/authorize".equals(e.getNewResourceLocation())){
                    jframe.setVisible(false);
                    twitterForm.setVisible(true);
                }
            }

            @Override
            public void locationChanged(WebBrowserNavigationEvent e) {
                super.locationChanged(e);
                System.out.println(e.getNewResourceLocation());
                if("https://twitter.com/oauth/authorize".equals(e.getNewResourceLocation())){
                    e.consume();
                    String content = e.getWebBrowser().getHTMLContent();
                    //System.out.println(content);
                    int index = content.indexOf("<CODE>")+6;
                    
                    try {
                        twitterClient.enterPinContinue(content.substring(index,index+7)); 
                    } catch (UniformInterfaceException ex) {
                        Logger.getLogger(browserPanel.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    twitterForm.hideProgressBar();
                }
                
                //e.getWebBrowser().getHTMLContent();
            }
        });
    }

    public void setTwitterClient(TwitterClient twitterClient) {
        this.twitterClient = twitterClient;
    }

    public void setTwitterForm(TwitterForm aThis) {
        this.twitterForm = aThis;
    }

    public void setJframe(JFrame frame) {
        this.jframe = frame;
    }
}