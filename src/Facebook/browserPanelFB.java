/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Facebook;

/**
 *
 * @author M0C
 */

 import chrriis.dj.nativeswing.swtimpl.components.JWebBrowser;
import chrriis.dj.nativeswing.swtimpl.components.WebBrowserAdapter;
import chrriis.dj.nativeswing.swtimpl.components.WebBrowserNavigationEvent;
import java.awt.BorderLayout;
import java.io.IOException;
import java.net.MalformedURLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFrame;
import javax.swing.JPanel;

/**
 *
 * @author Kamran
 */
public class browserPanelFB extends JPanel {
    public JWebBrowser webBrowser;
    private JFrame jframe;
    CiaoFacebook myObj;

    browserPanelFB(String string) {
        super(new BorderLayout());
        webBrowser = new JWebBrowser();
        webBrowser.navigate(string);
        webBrowser.setBarsVisible(false);
        add(webBrowser, BorderLayout.CENTER);
        webBrowser.addWebBrowserListener(new WebBrowserAdapter() {
        


            @Override
            public void locationChanged(WebBrowserNavigationEvent e) {
                super.locationChanged(e);
                System.out.println(e.getNewResourceLocation());
                String[] splitted;
                splitted=e.getNewResourceLocation().split("#");
                jframe.setTitle(e.getWebBrowser().getPageTitle());
                if("https://www.facebook.com/connect/login_success.html".equals(splitted[0])){
                    e.consume();
                    String[] splitted2;
                    String[] splitted3;
                    String accessToken=new String();
                    
                    splitted2=splitted[1].split("=");
                    splitted3=splitted2[1].split("&");
                    accessToken=splitted3[0];
                    myObj.accessToken=accessToken;
                    try {
                        myObj.done();
                        //System.out.println(content);
                    } catch (MalformedURLException ex) {
                        Logger.getLogger(browserPanelFB.class.getName()).log(Level.SEVERE, null, ex);
                    } catch (IOException ex) {
                        Logger.getLogger(browserPanelFB.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    jframe.dispose();
                }
                
                //e.getWebBrowser().getHTMLContent();
            }
        });
    }



    void setJframe(JFrame frame) {
        this.jframe = frame;
    }
    public void setTitle(String title)
    {
        jframe.setTitle(title);
    }
    void setFacebook(CiaoFacebook aThis) {
        
        myObj=aThis;
        
        
        
    }
}   

