/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package UserInterface;

import java.awt.CardLayout;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Platform;
import javafx.stage.Stage;
import javax.swing.DefaultListModel;
import javax.swing.JOptionPane;
import org.jivesoftware.smack.Chat;
import org.jivesoftware.smack.ChatManagerListener;
import org.jivesoftware.smack.Connection;
import org.jivesoftware.smack.MessageListener;
import org.jivesoftware.smack.XMPPException;
import org.jivesoftware.smack.packet.Message;

/**
 *
 * @author Parvaneh
 */
public class Gtalk extends javax.swing.JFrame {

    private DefaultListModel statusesListModel = new DefaultListModel();
    private Connection gmailConnection;
    private ChatForm chatForm;

    /**
     * Creates new form Gtalk
     */
    public Gtalk() {
        initComponents();
    }

    Gtalk(Connection gmailConnection) {
        this.gmailConnection = gmailConnection;
        initComponents();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        mainPanel = new javax.swing.JPanel();
        gtalkLoginPanel = new javax.swing.JPanel();
        gtalkpic = new javax.swing.JLabel();
        username = new javax.swing.JLabel();
        usernameField = new javax.swing.JTextField();
        password = new javax.swing.JLabel();
        gtalkLoginButton = new javax.swing.JButton();
        jpasswordField = new javax.swing.JPasswordField();
        GtalkPanel = new javax.swing.JPanel();
        gtalkpic1 = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        jList1 = new javax.swing.JList();
        jLabel1 = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Gtalk");
        setMinimumSize(new java.awt.Dimension(234, 447));
        setResizable(false);

        mainPanel.setLayout(new java.awt.CardLayout());

        gtalkLoginPanel.setBackground(new java.awt.Color(21, 51, 179));
        gtalkLoginPanel.setName("card1"); // NOI18N

        gtalkpic.setIcon(new javax.swing.ImageIcon(getClass().getResource("/UserInterface/google-talk-icon.png"))); // NOI18N

        username.setFont(new java.awt.Font("Tahoma", 1, 11));
        username.setForeground(new java.awt.Color(255, 179, 0));
        username.setText("Username");

        password.setFont(new java.awt.Font("Tahoma", 1, 11));
        password.setForeground(new java.awt.Color(255, 179, 0));
        password.setText("Password");

        gtalkLoginButton.setText("Login");
        gtalkLoginButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                gtalkLoginButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout gtalkLoginPanelLayout = new javax.swing.GroupLayout(gtalkLoginPanel);
        gtalkLoginPanel.setLayout(gtalkLoginPanelLayout);
        gtalkLoginPanelLayout.setHorizontalGroup(
            gtalkLoginPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(gtalkLoginPanelLayout.createSequentialGroup()
                .addGroup(gtalkLoginPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(gtalkLoginPanelLayout.createSequentialGroup()
                        .addGap(79, 79, 79)
                        .addComponent(gtalkLoginButton))
                    .addGroup(gtalkLoginPanelLayout.createSequentialGroup()
                        .addGap(29, 29, 29)
                        .addGroup(gtalkLoginPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(password)
                            .addComponent(username)
                            .addComponent(gtalkpic)
                            .addComponent(usernameField, javax.swing.GroupLayout.DEFAULT_SIZE, 171, Short.MAX_VALUE)
                            .addComponent(jpasswordField))))
                .addContainerGap(34, Short.MAX_VALUE))
        );
        gtalkLoginPanelLayout.setVerticalGroup(
            gtalkLoginPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(gtalkLoginPanelLayout.createSequentialGroup()
                .addGap(25, 25, 25)
                .addComponent(gtalkpic)
                .addGap(39, 39, 39)
                .addComponent(username)
                .addGap(9, 9, 9)
                .addComponent(usernameField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(password)
                .addGap(18, 18, 18)
                .addComponent(jpasswordField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(39, 39, 39)
                .addComponent(gtalkLoginButton)
                .addContainerGap(164, Short.MAX_VALUE))
        );

        mainPanel.add(gtalkLoginPanel, "card1");

        GtalkPanel.setBackground(new java.awt.Color(21, 51, 179));
        GtalkPanel.setName("card2"); // NOI18N

        jScrollPane1.setBackground(new java.awt.Color(21, 51, 179));
        jScrollPane1.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(255, 255, 255), 2));

        jList1.setModel(new javax.swing.AbstractListModel() {
            String[] strings = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
            public int getSize() { return strings.length; }
            public Object getElementAt(int i) { return strings[i]; }
        });
        jList1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jList1MouseClicked(evt);
            }
        });
        jScrollPane1.setViewportView(jList1);

        jLabel1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/UserInterface/google-talk-icon.png"))); // NOI18N

        javax.swing.GroupLayout GtalkPanelLayout = new javax.swing.GroupLayout(GtalkPanel);
        GtalkPanel.setLayout(GtalkPanelLayout);
        GtalkPanelLayout.setHorizontalGroup(
            GtalkPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(GtalkPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(GtalkPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, 214, Short.MAX_VALUE)
                    .addGroup(GtalkPanelLayout.createSequentialGroup()
                        .addGap(16, 16, 16)
                        .addComponent(jLabel1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(gtalkpic1)
                        .addGap(0, 0, Short.MAX_VALUE)))
                .addContainerGap())
        );
        GtalkPanelLayout.setVerticalGroup(
            GtalkPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(GtalkPanelLayout.createSequentialGroup()
                .addGroup(GtalkPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(GtalkPanelLayout.createSequentialGroup()
                        .addGap(57, 57, 57)
                        .addComponent(gtalkpic1))
                    .addGroup(GtalkPanelLayout.createSequentialGroup()
                        .addGap(21, 21, 21)
                        .addComponent(jLabel1)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 48, Short.MAX_VALUE)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 323, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        mainPanel.add(GtalkPanel, "card2");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(mainPanel, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(mainPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    void setGModel(ContactInfoCollectionListModel contactInfoCollectionListModel) {
        jList1.setModel(contactInfoCollectionListModel);
        jList1.updateUI();
    }

    private void gtalkLoginButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_gtalkLoginButtonActionPerformed
        try {
            gmailConnection.login(usernameField.getText(), jpasswordField.getText());
            setGModel(new ContactInfoCollectionListModel(gmailConnection.getRoster()));
            gmailConnection.getChatManager().addChatListener(new ChatManagerListener() {

                public void chatCreated(Chat chat1, boolean bln) {
                    if (!bln) {
                        final Chat chat = chat1;
                        new Thread(new Runnable() {

                            @Override
                            public void run() {
                                Platform.runLater(new Runnable() {

                                    @Override
                                    public void run() {
                                        final FX.ChatBoxFX cf = new FX.ChatBoxFX();
                                        chat.addMessageListener(new MessageListener() {

                                            @Override
                                            public void processMessage(Chat chat, Message msg) {
                                                cf.showMessage(msg.getBody());
                                            }
                                        });
                                        cf.setChat(chat);
                                        cf.setGtalk(true);
                                        //cf.setvisible(true);
                                        Stage stage = new Stage();
                                        cf.start(stage);
                                    }
                                });
                            }
                        }).start();


                    }
                }
            });
            // TODO add your handling code here:
            CardLayout cl = (CardLayout) (mainPanel.getLayout());
            cl.next(mainPanel);
        } catch (XMPPException ex) {
            JOptionPane.showMessageDialog(this, "Login Failed");
            Logger.getLogger(GTalkLogin.class.getName()).log(Level.SEVERE, null, ex);
        }
    }//GEN-LAST:event_gtalkLoginButtonActionPerformed

    private void jList1MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jList1MouseClicked
        if (evt.getClickCount() == 2) {
            int index = jList1.locationToIndex(evt.getPoint());
            final String recep = (String) jList1.getModel().getElementAt(index);
            System.out.println(recep);

            new Thread(new Runnable() {

                @Override
                public void run() {
                    Platform.runLater(new Runnable() {

                        @Override
                        public void run() {
                            final FX.ChatBoxFX cf = new FX.ChatBoxFX();
                            Chat chat = gmailConnection.getChatManager().createChat(recep + "@gmail.com", new MessageListener() {

                                @Override
                                public void processMessage(Chat chat, final Message message) {
                                    Platform.runLater(new Runnable() {

                                        @Override
                                        public void run() {
                                    cf.showMessage(message.getBody());
                                        }
                                    });
                                }
                            });
                            cf.setChat(chat);
                            cf.setGtalk(true);
                            //cf.setvisible(true);
                            Stage stage = new Stage();
                            cf.start(stage);
                            System.err.println("1");
                        }
                    });
                }
            }).start();





        }
    }//GEN-LAST:event_jList1MouseClicked

    void setChatForm(ChatForm aThis) {
        this.chatForm = aThis;
    }

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
            java.util.logging.Logger.getLogger(Gtalk.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Gtalk.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Gtalk.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Gtalk.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /*
         * Create and display the form
         */
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new Gtalk().setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JPanel GtalkPanel;
    private javax.swing.JButton gtalkLoginButton;
    private javax.swing.JPanel gtalkLoginPanel;
    private javax.swing.JLabel gtalkpic;
    private javax.swing.JLabel gtalkpic1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JList jList1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JPasswordField jpasswordField;
    private javax.swing.JPanel mainPanel;
    private javax.swing.JLabel password;
    private javax.swing.JLabel username;
    private javax.swing.JTextField usernameField;
    // End of variables declaration//GEN-END:variables
}
