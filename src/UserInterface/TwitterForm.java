/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */

/*
 * Twitter.java
 *
 * Created on Dec 5, 2011, 6:42:57 PM
 */
package UserInterface;

import Control.twt.TwitterClient;
import chrriis.common.UIUtils;
import chrriis.dj.nativeswing.swtimpl.NativeInterface;
import com.sun.jersey.api.client.Client;
import com.sun.jersey.api.client.UniformInterfaceException;
import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

/**
 *
 * @author Parvaneh
 */
public class TwitterForm extends javax.swing.JFrame {

    /**
     * Creates new form Twitter
     */
    TwitterClient twitterClient;
    public static browserPanel browser;
        
    public static void gotourl(String str){
        browser.webBrowser.navigate(str);
    }
    public TwitterForm() {
        
        initComponents();
        jProgressBar1.setIndeterminate(true);
        browser = new browserPanel("about:blank");
        twitterClient = new TwitterClient();
        
        
        
        browser.setTwitterClient(twitterClient);
        browser.setTwitterForm(this);
        UIUtils.setPreferredLookAndFeel();
        NativeInterface.open();
        SwingUtilities.invokeLater(new Runnable() {

            public void run() {
                JFrame frame = new JFrame();
                frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                frame.getContentPane().add(browser, BorderLayout.CENTER);
                frame.setSize(800, 600);
                frame.setLocationByPlatform(true);
                frame.setVisible(true);
                browser.setJframe(frame);
            }
        });
        jButton1ActionPerformed(null);
        
        //NativeInterface.runEventPump();

    }

    public void settext(String text) {
        TwittTextField.setText(text);
    }

    public void seticon(ImageIcon icon) {
        profilePic.setIcon(icon);
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jSplitPane1 = new javax.swing.JSplitPane();
        mainPanel = new javax.swing.JPanel();
        loginPanel = new javax.swing.JPanel();
        TwittPic = new javax.swing.JLabel();
        jButton1 = new javax.swing.JButton();
        TwitterPanel = new javax.swing.JPanel();
        twitterPic = new javax.swing.JLabel();
        profilePic = new javax.swing.JLabel();
        TwittTextField = new javax.swing.JTextField();
        twittButton = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        jList1 = new javax.swing.JList();
        jProgressBar1 = new javax.swing.JProgressBar();
        jLabel2 = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Twitter");
        setMinimumSize(new java.awt.Dimension(600, 620));
        setName("twitter"); // NOI18N

        mainPanel.setBackground(new java.awt.Color(21, 51, 179));
        mainPanel.setMaximumSize(new java.awt.Dimension(600, 620));
        mainPanel.setMinimumSize(new java.awt.Dimension(600, 620));
        mainPanel.setPreferredSize(new java.awt.Dimension(600, 620));
        mainPanel.setLayout(new java.awt.CardLayout());

        loginPanel.setBackground(new java.awt.Color(21, 51, 179));
        loginPanel.setMaximumSize(new java.awt.Dimension(600, 620));
        loginPanel.setMinimumSize(new java.awt.Dimension(600, 620));
        loginPanel.setName("card1"); // NOI18N
        loginPanel.setPreferredSize(new java.awt.Dimension(600, 620));

        jButton1.setText("jButton1");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout loginPanelLayout = new javax.swing.GroupLayout(loginPanel);
        loginPanel.setLayout(loginPanelLayout);
        loginPanelLayout.setHorizontalGroup(
            loginPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, loginPanelLayout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(loginPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addComponent(TwittPic, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jButton1, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGap(747, 747, 747))
        );
        loginPanelLayout.setVerticalGroup(
            loginPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, loginPanelLayout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(jButton1)
                .addGap(449, 449, 449)
                .addComponent(TwittPic, javax.swing.GroupLayout.PREFERRED_SIZE, 125, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(36, 36, 36))
        );

        mainPanel.add(loginPanel, "card1");

        TwitterPanel.setBackground(new java.awt.Color(21, 51, 179));
        TwitterPanel.setForeground(new java.awt.Color(255, 255, 255));
        TwitterPanel.setMaximumSize(new java.awt.Dimension(600, 620));
        TwitterPanel.setMinimumSize(new java.awt.Dimension(600, 620));
        TwitterPanel.setName("card2"); // NOI18N
        TwitterPanel.setPreferredSize(new java.awt.Dimension(600, 620));

        profilePic.setLabelFor(TwittTextField);
        profilePic.setText("Pic");
        profilePic.setMaximumSize(new java.awt.Dimension(48, 48));
        profilePic.setMinimumSize(new java.awt.Dimension(48, 48));
        profilePic.setPreferredSize(new java.awt.Dimension(48, 48));

        twittButton.setText("Tweet");
        twittButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                twittButtonActionPerformed(evt);
            }
        });

        jScrollPane1.setBackground(new java.awt.Color(21, 51, 179));
        jScrollPane1.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(255, 255, 255), 2));

        jList1.setBackground(new java.awt.Color(21, 51, 179));
        jList1.setForeground(new java.awt.Color(255, 255, 255));
        jList1.setCellRenderer(new twitt2());
        jScrollPane1.setViewportView(jList1);

        jLabel2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/UserInterface/twitterrr.png"))); // NOI18N

        javax.swing.GroupLayout TwitterPanelLayout = new javax.swing.GroupLayout(TwitterPanel);
        TwitterPanel.setLayout(TwitterPanelLayout);
        TwitterPanelLayout.setHorizontalGroup(
            TwitterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(TwitterPanelLayout.createSequentialGroup()
                .addGap(217, 217, 217)
                .addComponent(jProgressBar1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(TwitterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                .addGroup(javax.swing.GroupLayout.Alignment.LEADING, TwitterPanelLayout.createSequentialGroup()
                    .addContainerGap()
                    .addComponent(profilePic, javax.swing.GroupLayout.PREFERRED_SIZE, 64, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                    .addComponent(TwittTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 354, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(twittButton))
                .addGroup(javax.swing.GroupLayout.Alignment.LEADING, TwitterPanelLayout.createSequentialGroup()
                    .addGap(76, 76, 76)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 443, javax.swing.GroupLayout.PREFERRED_SIZE)))
            .addComponent(twitterPic)
            .addComponent(jLabel2)
        );
        TwitterPanelLayout.setVerticalGroup(
            TwitterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(TwitterPanelLayout.createSequentialGroup()
                .addComponent(twitterPic)
                .addGap(0, 0, 0)
                .addComponent(jLabel2)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 40, Short.MAX_VALUE)
                .addGroup(TwitterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(TwittTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(twittButton, javax.swing.GroupLayout.PREFERRED_SIZE, 21, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(profilePic, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 354, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jProgressBar1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(78, 78, 78))
        );

        mainPanel.add(TwitterPanel, "card2");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(mainPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(mainPanel, javax.swing.GroupLayout.DEFAULT_SIZE, 642, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed

        twitterClient.login(this);         CardLayout cl = (CardLayout) (mainPanel.getLayout());         cl.next(mainPanel);  // TODO add your handling code here:}//GEN-LAST:event_jButton1ActionPerformed
    }
        private void twittButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_twittButtonActionPerformed
        String rawStatus =TwittTextField.getText().trim();
        String status = null;
        try {
            status = URLEncoder.encode(rawStatus, "UTF-8");
        } catch (UnsupportedEncodingException ex) {
            Logger.getLogger(TwitterForm.class.getName()).log(Level.SEVERE, null, ex);
        }
        twitterClient.client.makeOAuthRequestUnique();
        try {
            twitterClient.client.updateStatus(String.class, status, null);
        } catch (UniformInterfaceException ex) {
            System.out.println("Exception when calling updateStatus = " + ex.getResponse().getEntity(String.class));
        }        // TODO add your handling code here:
    }//GEN-LAST:event_twittButtonActionPerformed

    

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /*
         * Set the Nimbus look and feel
         */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /*
         * If Nimbus (introduced in Java SE 6) is not available, stay with the
         * default look and feel. For details see
         * http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(TwitterForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(TwitterForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(TwitterForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(TwitterForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /*
         * Create and display the form
         */
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new TwitterForm().setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel TwittPic;
    private javax.swing.JTextField TwittTextField;
    private javax.swing.JPanel TwitterPanel;
    private javax.swing.JButton jButton1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JList jList1;
    private javax.swing.JProgressBar jProgressBar1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JSplitPane jSplitPane1;
    private javax.swing.JPanel loginPanel;
    private javax.swing.JPanel mainPanel;
    private javax.swing.JLabel profilePic;
    private javax.swing.JButton twittButton;
    private javax.swing.JLabel twitterPic;
    // End of variables declaration//GEN-END:variables

    void hideProgressBar() {
        jProgressBar1.setVisible(false);
        jList1.setModel(twitterClient.statusesListModel);
    }
}
