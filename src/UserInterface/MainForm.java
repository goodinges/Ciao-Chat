/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * MainForm.java
 *
 * Created on Oct 23, 2011, 12:17:25 PM
 */
package UserInterface;

import Authentication.authControl;
import ChatUtil.ChatController;
import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;

/**
 *
 * @author Parvaneh
 */
public class MainForm extends javax.swing.JFrame {

    /** Creates new form MainForm */
    public MainForm() {
        initComponents();
        jProgressBar1.setVisible(false);
        connectingLabel.setVisible(false);
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        buttonGroup1 = new javax.swing.ButtonGroup();
        buttonGroup2 = new javax.swing.ButtonGroup();
        jPanel1 = new javax.swing.JPanel();
        jPanel2 = new javax.swing.JPanel();
        statusBComboox = new javax.swing.JComboBox();
        idTextField = new javax.swing.JTextField();
        createAccountLabel = new javax.swing.JLabel();
        passwordField = new javax.swing.JPasswordField();
        forgotPassLabel = new javax.swing.JLabel();
        rememberPassCheckBox = new javax.swing.JCheckBox();
        signInautoCheckBox = new javax.swing.JCheckBox();
        SgnInButton = new javax.swing.JButton();
        jProgressBar1 = new javax.swing.JProgressBar();
        connectingLabel = new javax.swing.JLabel();
        jLabel1 = new javax.swing.JLabel();
        signInFailedLabel = new javax.swing.JLabel();
        MenuBar = new javax.swing.JMenuBar();
        File = new javax.swing.JMenu();
        changeStatusMenu = new javax.swing.JMenu();
        jRadioButtonMenuItem2 = new javax.swing.JRadioButtonMenuItem();
        jRadioButtonMenuItem3 = new javax.swing.JRadioButtonMenuItem();
        jRadioButtonMenuItem4 = new javax.swing.JRadioButtonMenuItem();
        jRadioButtonMenuItem1 = new javax.swing.JRadioButtonMenuItem();
        settingsMenuItem = new javax.swing.JMenuItem();
        signInMenuItem = new javax.swing.JMenuItem();
        Contacts = new javax.swing.JMenu();
        Actions = new javax.swing.JMenu();
        Help = new javax.swing.JMenu();
        helpMenuItem = new javax.swing.JMenuItem();
        aboutMneuItem = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Ciao! Chat");

        jPanel1.setBackground(new java.awt.Color(21, 51, 173));

        jPanel2.setBackground(new java.awt.Color(21, 51, 173));

        statusBComboox.setFont(new java.awt.Font("Calibri", 0, 12));
        statusBComboox.setForeground(new java.awt.Color(21, 51, 173));
        statusBComboox.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Online", "Busy", "Away", "Invisible" }));
        statusBComboox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                statusBCombooxActionPerformed(evt);
            }
        });

        idTextField.setFont(new java.awt.Font("Calibri", 0, 12));
        idTextField.setBorder(javax.swing.BorderFactory.createMatteBorder(1, 1, 1, 1, new java.awt.Color(255, 255, 255)));
        idTextField.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                idTextFieldMouseClicked(evt);
            }
        });
        idTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                idTextFieldActionPerformed(evt);
            }
        });

        createAccountLabel.setFont(new java.awt.Font("Arial", 1, 12));
        createAccountLabel.setForeground(new java.awt.Color(255, 0, 0));
        createAccountLabel.setText("Get a new Ciao! account");
        createAccountLabel.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                createAccountLabelMouseClicked(evt);
            }
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                createAccountLabelMouseEntered(evt);
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                createAccountLabelMouseExited(evt);
            }
        });

        passwordField.setBorder(javax.swing.BorderFactory.createMatteBorder(1, 1, 1, 1, new java.awt.Color(255, 255, 255)));
        passwordField.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                passwordFieldMouseClicked(evt);
            }
        });
        passwordField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                passwordFieldActionPerformed(evt);
            }
        });

        forgotPassLabel.setFont(new java.awt.Font("Arial", 1, 12));
        forgotPassLabel.setForeground(new java.awt.Color(255, 0, 0));
        forgotPassLabel.setText("Forgot your password?");
        forgotPassLabel.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                forgotPassLabelMouseClicked(evt);
            }
            public void mouseEntered(java.awt.event.MouseEvent evt) {
                forgotPassLabelMouseEntered(evt);
            }
            public void mouseExited(java.awt.event.MouseEvent evt) {
                forgotPassLabelMouseExited(evt);
            }
        });

        rememberPassCheckBox.setBackground(new java.awt.Color(21, 51, 173));
        rememberPassCheckBox.setFont(new java.awt.Font("Calibri", 0, 12));
        rememberPassCheckBox.setForeground(new java.awt.Color(255, 179, 0));
        rememberPassCheckBox.setText("Remember my password");
        rememberPassCheckBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rememberPassCheckBoxActionPerformed(evt);
            }
        });

        signInautoCheckBox.setBackground(new java.awt.Color(21, 51, 173));
        signInautoCheckBox.setFont(new java.awt.Font("Calibri", 0, 12));
        signInautoCheckBox.setForeground(new java.awt.Color(255, 179, 0));
        signInautoCheckBox.setText("Sign in automatically");
        signInautoCheckBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                signInautoCheckBoxActionPerformed(evt);
            }
        });

        SgnInButton.setBackground(new java.awt.Color(255, 255, 255));
        SgnInButton.setFont(new java.awt.Font("Calibri", 1, 12));
        SgnInButton.setForeground(new java.awt.Color(21, 51, 173));
        SgnInButton.setText("Sign in");
        SgnInButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                SgnInButtonActionPerformed(evt);
            }
        });

        jProgressBar1.setBackground(new java.awt.Color(255, 179, 0));
        jProgressBar1.setForeground(new java.awt.Color(21, 51, 173));

        connectingLabel.setFont(new java.awt.Font("Tahoma", 1, 12));
        connectingLabel.setForeground(new java.awt.Color(255, 179, 0));
        connectingLabel.setText("Connecting");

        jLabel1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/UserInterface/profilePicLarge-icon.jpg"))); // NOI18N

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(79, 79, 79)
                        .addComponent(jLabel1))
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addGap(57, 57, 57)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                                .addComponent(jProgressBar1, javax.swing.GroupLayout.PREFERRED_SIZE, 129, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(SgnInButton))
                            .addGroup(jPanel2Layout.createSequentialGroup()
                                .addGap(30, 30, 30)
                                .addComponent(connectingLabel)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 100, javax.swing.GroupLayout.PREFERRED_SIZE)))))
                .addContainerGap(47, Short.MAX_VALUE))
            .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(jPanel2Layout.createSequentialGroup()
                    .addGap(36, 36, 36)
                    .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(signInautoCheckBox, javax.swing.GroupLayout.PREFERRED_SIZE, 150, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(idTextField, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, 224, Short.MAX_VALUE)
                            .addComponent(passwordField, javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(createAccountLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 145, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addComponent(rememberPassCheckBox)
                        .addComponent(forgotPassLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 133, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGroup(jPanel2Layout.createSequentialGroup()
                            .addGap(66, 66, 66)
                            .addComponent(statusBComboox, javax.swing.GroupLayout.PREFERRED_SIZE, 76, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addContainerGap(44, Short.MAX_VALUE)))
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(40, 40, 40)
                .addComponent(jLabel1)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 310, Short.MAX_VALUE)
                .addComponent(connectingLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(SgnInButton)
                    .addComponent(jProgressBar1, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(143, 143, 143))
            .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(jPanel2Layout.createSequentialGroup()
                    .addGap(200, 200, 200)
                    .addComponent(statusBComboox, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGap(32, 32, 32)
                    .addComponent(idTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 27, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                    .addComponent(createAccountLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 14, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGap(36, 36, 36)
                    .addComponent(passwordField, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGap(7, 7, 7)
                    .addComponent(forgotPassLabel)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                    .addComponent(rememberPassCheckBox)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                    .addComponent(signInautoCheckBox)
                    .addContainerGap(225, Short.MAX_VALUE)))
        );

        signInFailedLabel.setFont(new java.awt.Font("Arial", 1, 12)); // NOI18N
        signInFailedLabel.setForeground(new java.awt.Color(255, 179, 0));

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(signInFailedLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 261, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(23, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(23, 23, 23)
                .addComponent(signInFailedLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        MenuBar.setBackground(new java.awt.Color(255, 179, 0));

        File.setBackground(new java.awt.Color(255, 179, 0));
        File.setForeground(new java.awt.Color(21, 51, 173));
        File.setText("File");
        File.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                FileActionPerformed(evt);
            }
        });

        changeStatusMenu.setBackground(new java.awt.Color(255, 179, 0));
        changeStatusMenu.setForeground(new java.awt.Color(21, 51, 173));
        changeStatusMenu.setText("Change Status");

        jRadioButtonMenuItem2.setBackground(new java.awt.Color(255, 179, 0));
        buttonGroup1.add(jRadioButtonMenuItem2);
        jRadioButtonMenuItem2.setForeground(new java.awt.Color(21, 51, 173));
        jRadioButtonMenuItem2.setSelected(true);
        jRadioButtonMenuItem2.setText("Online");
        changeStatusMenu.add(jRadioButtonMenuItem2);

        jRadioButtonMenuItem3.setBackground(new java.awt.Color(255, 179, 0));
        buttonGroup1.add(jRadioButtonMenuItem3);
        jRadioButtonMenuItem3.setForeground(new java.awt.Color(21, 51, 173));
        jRadioButtonMenuItem3.setText("Busy");
        changeStatusMenu.add(jRadioButtonMenuItem3);

        jRadioButtonMenuItem4.setBackground(new java.awt.Color(255, 179, 0));
        buttonGroup1.add(jRadioButtonMenuItem4);
        jRadioButtonMenuItem4.setForeground(new java.awt.Color(21, 51, 173));
        jRadioButtonMenuItem4.setText("Away");
        changeStatusMenu.add(jRadioButtonMenuItem4);

        jRadioButtonMenuItem1.setBackground(new java.awt.Color(255, 179, 0));
        buttonGroup1.add(jRadioButtonMenuItem1);
        jRadioButtonMenuItem1.setForeground(new java.awt.Color(21, 51, 173));
        jRadioButtonMenuItem1.setText("Invisible");
        changeStatusMenu.add(jRadioButtonMenuItem1);

        File.add(changeStatusMenu);

        settingsMenuItem.setBackground(new java.awt.Color(255, 179, 0));
        settingsMenuItem.setForeground(new java.awt.Color(21, 51, 173));
        settingsMenuItem.setText("Settings");
        File.add(settingsMenuItem);

        signInMenuItem.setBackground(new java.awt.Color(255, 179, 0));
        signInMenuItem.setForeground(new java.awt.Color(21, 51, 173));
        signInMenuItem.setText("Sign In");
        signInMenuItem.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                signInMenuItemMouseClicked(evt);
            }
        });
        signInMenuItem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                signInMenuItemActionPerformed(evt);
            }
        });
        File.add(signInMenuItem);

        MenuBar.add(File);

        Contacts.setBackground(new java.awt.Color(255, 179, 0));
        Contacts.setForeground(new java.awt.Color(21, 51, 173));
        Contacts.setText("Contacts");
        MenuBar.add(Contacts);

        Actions.setBackground(new java.awt.Color(255, 179, 0));
        Actions.setForeground(new java.awt.Color(21, 51, 173));
        Actions.setText("Actions");
        MenuBar.add(Actions);

        Help.setBackground(new java.awt.Color(255, 179, 0));
        Help.setForeground(new java.awt.Color(21, 51, 173));
        Help.setText("Help");

        helpMenuItem.setBackground(new java.awt.Color(255, 179, 0));
        helpMenuItem.setForeground(new java.awt.Color(21, 51, 173));
        helpMenuItem.setText("Ciao! Help");
        Help.add(helpMenuItem);

        aboutMneuItem.setBackground(new java.awt.Color(255, 179, 0));
        aboutMneuItem.setForeground(new java.awt.Color(21, 51, 173));
        aboutMneuItem.setText("About");
        Help.add(aboutMneuItem);

        MenuBar.add(Help);

        setJMenuBar(MenuBar);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, 632, javax.swing.GroupLayout.PREFERRED_SIZE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

private void statusBCombooxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_statusBCombooxActionPerformed
    // TODO add your handling code here:
}//GEN-LAST:event_statusBCombooxActionPerformed

private void idTextFieldMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_idTextFieldMouseClicked
    //jTextField1.setText("");
}//GEN-LAST:event_idTextFieldMouseClicked

private void idTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_idTextFieldActionPerformed
    // TODO add your handling code here:
}//GEN-LAST:event_idTextFieldActionPerformed

private void createAccountLabelMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_createAccountLabelMouseClicked
    CreateAccountForm createAccountForm = new CreateAccountForm();
    createAccountForm.setVisible(true);
}//GEN-LAST:event_createAccountLabelMouseClicked

private void createAccountLabelMouseEntered(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_createAccountLabelMouseEntered
    createAccountLabel.setForeground(new Color(255, 179, 0));
}//GEN-LAST:event_createAccountLabelMouseEntered

private void createAccountLabelMouseExited(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_createAccountLabelMouseExited
    createAccountLabel.setForeground(new Color(250, 0, 0));
}//GEN-LAST:event_createAccountLabelMouseExited

private void passwordFieldMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_passwordFieldMouseClicked
    //jPasswordField1.setText("");       
}//GEN-LAST:event_passwordFieldMouseClicked

private void forgotPassLabelMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_forgotPassLabelMouseClicked
    ForgotPassword forgotPassword = new ForgotPassword();
    forgotPassword.setVisible(true);
}//GEN-LAST:event_forgotPassLabelMouseClicked

private void forgotPassLabelMouseEntered(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_forgotPassLabelMouseEntered
    forgotPassLabel.setForeground(new Color(255, 179, 0));
}//GEN-LAST:event_forgotPassLabelMouseEntered

private void forgotPassLabelMouseExited(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_forgotPassLabelMouseExited
    forgotPassLabel.setForeground(new Color(250, 0, 0));
}//GEN-LAST:event_forgotPassLabelMouseExited

private void rememberPassCheckBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rememberPassCheckBoxActionPerformed
    // TODO add your handling code here:
}//GEN-LAST:event_rememberPassCheckBoxActionPerformed

private void signInautoCheckBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_signInautoCheckBoxActionPerformed
    // TODO add your handling code here:
}//GEN-LAST:event_signInautoCheckBoxActionPerformed
    ChatForm chatform;
private void SgnInButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_SgnInButtonActionPerformed
    jProgressBar1.setIndeterminate(true);
    jProgressBar1.setVisible(true);
    connectingLabel.setVisible(true);

    new Thread(new Runnable() {

        public void run() {
            final String id = idTextField.getText();
            if (authControl.login(id, passwordField.getPassword())) {
                chatform = new ChatForm(new ContactInfoCollectionListModel(ChatController.getContacts(id)));
                String[] infos = new String[10];
                infos = authControl.getUserInfo(id, infos);
                chatform.setUserInfo(infos);
                dispose();
                chatform.setVisible(true);
                new Thread(new Runnable() {

                    public void run() {
                        List<Byte> list = authControl.getProfilePic(id);

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

            } else {
                signInFailedLabel.setText("Incorrect username or password");
                jProgressBar1.setIndeterminate(false);
                jProgressBar1.setVisible(false);
                connectingLabel.setVisible(false);
            }
        }
    }).start();

}//GEN-LAST:event_SgnInButtonActionPerformed

private void passwordFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_passwordFieldActionPerformed
// TODO add your handling code here:
}//GEN-LAST:event_passwordFieldActionPerformed

private void signInMenuItemMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_signInMenuItemMouseClicked
}//GEN-LAST:event_signInMenuItemMouseClicked

private void FileActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_FileActionPerformed
// TODO add your handling code here:
}//GEN-LAST:event_FileActionPerformed

private void signInMenuItemActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_signInMenuItemActionPerformed
    SgnInButtonActionPerformed(evt);
}//GEN-LAST:event_signInMenuItemActionPerformed

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
            java.util.logging.Logger.getLogger(MainForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(MainForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(MainForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(MainForm.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new MainForm().setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenu Actions;
    private javax.swing.JMenu Contacts;
    private javax.swing.JMenu File;
    private javax.swing.JMenu Help;
    private javax.swing.JMenuBar MenuBar;
    private javax.swing.JButton SgnInButton;
    private javax.swing.JMenuItem aboutMneuItem;
    private javax.swing.ButtonGroup buttonGroup1;
    private javax.swing.ButtonGroup buttonGroup2;
    private javax.swing.JMenu changeStatusMenu;
    private javax.swing.JLabel connectingLabel;
    private javax.swing.JLabel createAccountLabel;
    private javax.swing.JLabel forgotPassLabel;
    private javax.swing.JMenuItem helpMenuItem;
    private javax.swing.JTextField idTextField;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JProgressBar jProgressBar1;
    private javax.swing.JRadioButtonMenuItem jRadioButtonMenuItem1;
    private javax.swing.JRadioButtonMenuItem jRadioButtonMenuItem2;
    private javax.swing.JRadioButtonMenuItem jRadioButtonMenuItem3;
    private javax.swing.JRadioButtonMenuItem jRadioButtonMenuItem4;
    private javax.swing.JPasswordField passwordField;
    private javax.swing.JCheckBox rememberPassCheckBox;
    private javax.swing.JMenuItem settingsMenuItem;
    private javax.swing.JLabel signInFailedLabel;
    private javax.swing.JMenuItem signInMenuItem;
    private javax.swing.JCheckBox signInautoCheckBox;
    private javax.swing.JComboBox statusBComboox;
    // End of variables declaration//GEN-END:variables
}