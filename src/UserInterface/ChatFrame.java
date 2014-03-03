/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * ChatFrame.java
 *
 * Created on Nov 8, 2011, 12:14:49 PM
 */
package UserInterface;

import ChatUtil.ChatSession;
import ChatUtil.messageListener;
import com.skype.api.Conversation;
import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.AbstractAction;
import javax.swing.ActionMap;
import javax.swing.InputMap;
import javax.swing.JComponent;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledDocument;
import org.jivesoftware.smack.XMPPException;

/**
 *
 * @author Parvaneh
 */
public class ChatFrame extends javax.swing.JFrame {

    private ChatSession chat;
    private boolean gtalk = false;
    private org.jivesoftware.smack.Chat gchat;
    private Conversation skypeConversation;
    private boolean skypeMode = false;

    public void setGtalk(boolean gtalk) {
        this.gtalk = gtalk;
    }

    /** Creates new form ChatFrame */
    public ChatFrame() {
        initComponents();
        this.addWindowListener(new WindowAdapter() {

            @Override
            public void windowClosing(WindowEvent e) {

                super.windowClosing(e);
            }
        });
        setStyle();
        /*
        new Thread(new Runnable() {

            public void run() {
                ImageIcon ii = new ImageIcon("profilepic.jpg");
                ii.setImage(ii.getImage().getScaledInstance(80, 80, BufferedImage.TYPE_INT_RGB));
                profilePicLabel1.setIcon(ii);

                List<Byte> list = authControl.getProfilePic(chat.);

                byte[] bytes = new byte[list.size()];
                for (int i = 0; i < list.size(); i++) {
                    bytes[i] = list.get(i);
                }
                File im = new File("profilepic.jpg");
                FileOutputStream fos;
                try {
                    fos = new FileOutputStream(im);
                    fos.write(bytes);
                    fos.flush();
                    fos.close();
                    System.out.println(im.getAbsolutePath());
                    try {
                        InputStream in = new ByteArrayInputStream(bytes);
                        BufferedImage image = ImageIO.read(in);
                        chatform.profilepicUpdate(image);
                    } catch (InterruptedException ex) {
                        Logger.getLogger(MainForm.class.getName()).log(Level.SEVERE, null, ex);
                    }
                } catch (IOException ex) {
                    Logger.getLogger(authControl.class.getName()).log(Level.SEVERE, null, ex);
                }




            }
        }).start();
         * 
         */
    }

    private void setStyle() {
        doc = showTextPane.getStyledDocument();
        dateStyle1 = showTextPane.addStyle("date", null);
        StyleConstants.setFontFamily(dateStyle1, "Comic Sans MS");
        StyleConstants.setForeground(dateStyle1, Color.blue);
        StyleConstants.setFontSize(dateStyle1, 12);
        StyleConstants.setBold(dateStyle1, true);
        textStyle1 = showTextPane.addStyle("text", null);
        StyleConstants.setFontFamily(textStyle1, "Comic Sans MS");
        StyleConstants.setForeground(textStyle1, Color.blue);
        StyleConstants.setFontSize(textStyle1, 11);
        dateStyle2 = showTextPane.addStyle("date2", null);
        StyleConstants.setFontFamily(dateStyle2, "Comic Sans MS");
        StyleConstants.setForeground(dateStyle2, Color.orange);
        StyleConstants.setFontSize(dateStyle2, 12);
        StyleConstants.setBold(dateStyle2, true);
        textStyle2 = showTextPane.addStyle("text2", null);
        StyleConstants.setFontFamily(textStyle2, "Comic Sans MS");
        StyleConstants.setForeground(textStyle2, Color.orange);
        StyleConstants.setFontSize(textStyle2, 11);
    }
    StyledDocument doc;
    Style dateStyle1;
    Style textStyle1;
    Style dateStyle2;
    Style textStyle2;

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        mainPanel = new javax.swing.JPanel();
        enterTextPanel = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        enterTextPane = new javax.swing.JTextPane();
        sendButton = new javax.swing.JButton();
        profilePicPanel = new javax.swing.JPanel();
        profilePicLabel = new javax.swing.JLabel();
        contactNameLabel = new javax.swing.JLabel();
        profilePicLabel1 = new javax.swing.JLabel();
        yourNameLabel = new javax.swing.JLabel();
        statusLabel = new javax.swing.JLabel();
        StatusLabel1 = new javax.swing.JLabel();
        yourTextLabel = new javax.swing.JLabel();
        viewHistoryButton = new javax.swing.JButton();
        jScrollPane2 = new javax.swing.JScrollPane();
        showTextPane = new javax.swing.JTextPane();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Chat");

        mainPanel.setBackground(new java.awt.Color(21, 51, 173));

        enterTextPanel.setBackground(new java.awt.Color(255, 255, 255));
        enterTextPanel.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(255, 179, 0), 3));

        jScrollPane1.setBorder(null);

        enterTextPane.setBorder(null);
        enterTextPane.setCaretColor(new java.awt.Color(255, 255, 255));
        enterTextPane.setCursor(new java.awt.Cursor(java.awt.Cursor.TEXT_CURSOR));
        enterTextPane.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                enterTextPaneKeyPressed(evt);
            }
        });
        jScrollPane1.setViewportView(enterTextPane);
        int condition = JComponent.WHEN_FOCUSED;
        InputMap iMap = enterTextPane.getInputMap(condition);
        ActionMap aMap = enterTextPane.getActionMap();

        String enter = "enter";
        iMap.put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), enter);
        aMap.put(enter,new AbstractAction() {

            @Override
            public void actionPerformed(ActionEvent arg0) {
                sendButtonActionPerformed(null);
            }
        });

        sendButton.setBackground(new java.awt.Color(255, 179, 0));
        sendButton.setFont(new java.awt.Font("Arial", 0, 11));
        sendButton.setForeground(new java.awt.Color(21, 51, 173));
        sendButton.setText("Send");
        sendButton.setBorder(new javax.swing.border.SoftBevelBorder(javax.swing.border.BevelBorder.RAISED, null, null, null, java.awt.Color.blue));
        sendButton.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                sendButtonMouseClicked(evt);
            }
        });
        sendButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                sendButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout enterTextPanelLayout = new javax.swing.GroupLayout(enterTextPanel);
        enterTextPanel.setLayout(enterTextPanelLayout);
        enterTextPanelLayout.setHorizontalGroup(
            enterTextPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, enterTextPanelLayout.createSequentialGroup()
                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 328, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(sendButton, javax.swing.GroupLayout.PREFERRED_SIZE, 57, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        enterTextPanelLayout.setVerticalGroup(
            enterTextPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 100, Short.MAX_VALUE)
            .addGroup(enterTextPanelLayout.createSequentialGroup()
                .addGap(11, 11, 11)
                .addComponent(sendButton, javax.swing.GroupLayout.DEFAULT_SIZE, 78, Short.MAX_VALUE)
                .addGap(11, 11, 11))
        );

        profilePicPanel.setBackground(new java.awt.Color(255, 179, 0));

        profilePicLabel.setIcon(new javax.swing.ImageIcon(getClass().getResource("/UserInterface/profilePicSmall-icon.jpg"))); // NOI18N

        contactNameLabel.setFont(new java.awt.Font("Arial", 0, 11));
        contactNameLabel.setForeground(new java.awt.Color(21, 51, 173));
        contactNameLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        contactNameLabel.setText("Contact Name");

        profilePicLabel1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/UserInterface/profilePicSmall-icon.jpg"))); // NOI18N

        yourNameLabel.setFont(new java.awt.Font("Arial", 0, 11));
        yourNameLabel.setForeground(new java.awt.Color(21, 51, 173));
        yourNameLabel.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        yourNameLabel.setText("Your Name");

        statusLabel.setFont(new java.awt.Font("Arial", 0, 11));
        statusLabel.setForeground(new java.awt.Color(21, 51, 173));
        statusLabel.setText("Status");

        StatusLabel1.setFont(new java.awt.Font("Arial", 0, 11));
        StatusLabel1.setForeground(new java.awt.Color(21, 51, 173));
        StatusLabel1.setText("Status");

        javax.swing.GroupLayout profilePicPanelLayout = new javax.swing.GroupLayout(profilePicPanel);
        profilePicPanel.setLayout(profilePicPanelLayout);
        profilePicPanelLayout.setHorizontalGroup(
            profilePicPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, profilePicPanelLayout.createSequentialGroup()
                .addContainerGap(78, Short.MAX_VALUE)
                .addComponent(profilePicLabel)
                .addGap(47, 47, 47))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, profilePicPanelLayout.createSequentialGroup()
                .addGap(51, 51, 51)
                .addComponent(contactNameLabel, javax.swing.GroupLayout.DEFAULT_SIZE, 146, Short.MAX_VALUE)
                .addContainerGap())
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, profilePicPanelLayout.createSequentialGroup()
                .addContainerGap(109, Short.MAX_VALUE)
                .addComponent(statusLabel)
                .addGap(67, 67, 67))
            .addGroup(profilePicPanelLayout.createSequentialGroup()
                .addGap(42, 42, 42)
                .addGroup(profilePicPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(profilePicPanelLayout.createSequentialGroup()
                        .addComponent(StatusLabel1)
                        .addGap(30, 30, 30))
                    .addGroup(profilePicPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(yourNameLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 91, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(profilePicLabel1)))
                .addContainerGap(45, Short.MAX_VALUE))
        );
        profilePicPanelLayout.setVerticalGroup(
            profilePicPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(profilePicPanelLayout.createSequentialGroup()
                .addGap(41, 41, 41)
                .addComponent(profilePicLabel)
                .addGap(28, 28, 28)
                .addComponent(contactNameLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(statusLabel)
                .addGap(40, 40, 40)
                .addComponent(profilePicLabel1)
                .addGap(29, 29, 29)
                .addComponent(yourNameLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(StatusLabel1)
                .addContainerGap(51, Short.MAX_VALUE))
        );

        yourTextLabel.setFont(new java.awt.Font("Arial", 2, 12));
        yourTextLabel.setForeground(new java.awt.Color(255, 179, 0));
        yourTextLabel.setText("Your text :");

        viewHistoryButton.setBackground(new java.awt.Color(255, 179, 0));
        viewHistoryButton.setFont(new java.awt.Font("Comic Sans MS", 0, 11));
        viewHistoryButton.setForeground(new java.awt.Color(21, 51, 173));
        viewHistoryButton.setText("History");
        viewHistoryButton.setBorder(new javax.swing.border.SoftBevelBorder(javax.swing.border.BevelBorder.RAISED, null, null, null, java.awt.Color.black));
        viewHistoryButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                viewHistoryButtonActionPerformed(evt);
            }
        });

        showTextPane.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(255, 179, 0), 3));
        jScrollPane2.setViewportView(showTextPane);

        javax.swing.GroupLayout mainPanelLayout = new javax.swing.GroupLayout(mainPanel);
        mainPanel.setLayout(mainPanelLayout);
        mainPanelLayout.setHorizontalGroup(
            mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(mainPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 411, Short.MAX_VALUE)
                    .addGroup(mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                        .addComponent(viewHistoryButton, javax.swing.GroupLayout.PREFERRED_SIZE, 85, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(yourTextLabel)
                        .addComponent(enterTextPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                .addGap(18, 18, 18)
                .addComponent(profilePicPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        mainPanelLayout.setVerticalGroup(
            mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(mainPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(mainPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(profilePicPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(mainPanelLayout.createSequentialGroup()
                        .addComponent(viewHistoryButton)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 254, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(yourTextLabel)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(enterTextPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(mainPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(mainPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

private void sendButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_sendButtonActionPerformed
    if (gtalk == false && skypeMode == false) {
        chat.send(enterTextPane.getText());
    } else if (gtalk == true) {
        try {
            gchat.sendMessage(enterTextPane.getText());
        } catch (XMPPException ex) {
            Logger.getLogger(ChatFrame.class.getName()).log(Level.SEVERE, null, ex);
        }
    } else if (skypeMode = true) {
        skypeConversation.PostText(enterTextPane.getText(), false);
    }
    try {
        doc.insertString(doc.getLength(), "(" + getSentTextTime() + ") : ", dateStyle1);
    } catch (BadLocationException ex) {
        Logger.getLogger(ChatFrame.class.getName()).log(Level.SEVERE, null, ex);
    }
    try {
        doc.insertString(doc.getLength(), enterTextPane.getText() + "\n", textStyle1);
    } catch (BadLocationException ex) {
        Logger.getLogger(ChatFrame.class.getName()).log(Level.SEVERE, null, ex);
    }
    enterTextPane.setText("");


}//GEN-LAST:event_sendButtonActionPerformed

private void enterTextPaneKeyPressed(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_enterTextPaneKeyPressed
}//GEN-LAST:event_enterTextPaneKeyPressed

private void sendButtonMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_sendButtonMouseClicked
// TODO add your handling code here:
}//GEN-LAST:event_sendButtonMouseClicked

private void viewHistoryButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_viewHistoryButtonActionPerformed
// TODO add your handling code here:
}//GEN-LAST:event_viewHistoryButtonActionPerformed

    private String getSentTextTime() {
        GregorianCalendar now = new GregorianCalendar();//gets the current date and time 
        int year = now.get(Calendar.YEAR);
        int month = now.get(Calendar.MONTH);
        int day = now.get(Calendar.DATE) + 1; //add one because January is integer 0 
        int hour = now.get(Calendar.HOUR_OF_DAY);
        int minute = now.get(Calendar.MINUTE);
        int second = now.get(Calendar.SECOND);

        String output = year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second;
        return output;

    }

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(ChatFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(ChatFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(ChatFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(ChatFrame.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {

                //  yourNameLabel.setText(ChatForm.getUserInfo());
                new ChatFrame().setVisible(true);
            }
        });
    }
    Font timedisplayFont = new Font("Arial", Font.BOLD, 11);
    Font textdisplayFont = new Font("Arial", Font.PLAIN, 11);
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel StatusLabel1;
    private javax.swing.JLabel contactNameLabel;
    private javax.swing.JTextPane enterTextPane;
    private javax.swing.JPanel enterTextPanel;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JPanel mainPanel;
    private javax.swing.JLabel profilePicLabel;
    private javax.swing.JLabel profilePicLabel1;
    private javax.swing.JPanel profilePicPanel;
    private javax.swing.JButton sendButton;
    private javax.swing.JTextPane showTextPane;
    private javax.swing.JLabel statusLabel;
    private javax.swing.JButton viewHistoryButton;
    private javax.swing.JLabel yourNameLabel;
    private javax.swing.JLabel yourTextLabel;
    // End of variables declaration//GEN-END:variables

    public void setChat(ChatSession chat) {
        this.chat = chat;
        chat.setListener(new messageListener() {

            @Override
            public void processMessage(String message) {
                try {
                    doc.insertString(doc.getLength(), "(" + getSentTextTime() + ") : ", dateStyle2);
                } catch (BadLocationException ex) {
                    Logger.getLogger(ChatFrame.class.getName()).log(Level.SEVERE, null, ex);
                }
                try {
                    doc.insertString(doc.getLength(), message + "\n", textStyle2);
                } catch (BadLocationException ex) {
                    Logger.getLogger(ChatFrame.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });
    }

    public void showMessage(String message) {
        if (message != null) {
            try {
                doc.insertString(doc.getLength(), "(" + getSentTextTime() + ") : ", dateStyle2);
            } catch (BadLocationException ex) {
                Logger.getLogger(ChatFrame.class.getName()).log(Level.SEVERE, null, ex);
            }
            try {
                doc.insertString(doc.getLength(), message + "\n", textStyle2);
            } catch (BadLocationException ex) {
                Logger.getLogger(ChatFrame.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    void setChat(org.jivesoftware.smack.Chat chat) {
        gchat = chat;
    }

    public void setSkype(Conversation conversation) {
        skypeConversation = conversation;
    }

    public void setSkypeMode() {
        skypeMode = true;
    }
}
