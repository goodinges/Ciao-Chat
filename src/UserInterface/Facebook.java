/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package UserInterface;

import Facebook.CiaoFacebook;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Parvaneh
 */
public class Facebook extends javax.swing.JFrame {
    CiaoFacebook myC;
    
    /**
     * Creates new form Facebook
     */
    public Facebook() throws URISyntaxException, IOException {
        initComponents();
        jEditorPane2.setVisible(false);
        myC=new CiaoFacebook();
        myC.Login();
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        mainPanel = new javax.swing.JPanel();
        facebook = new javax.swing.JPanel();
        facebookPic = new javax.swing.JLabel();
        statusTextField = new javax.swing.JTextField();
        jButton1 = new javax.swing.JButton();
        jScrollPane2 = new javax.swing.JScrollPane();
        jEditorPane1 = new javax.swing.JEditorPane();
        jScrollPane3 = new javax.swing.JScrollPane();
        jEditorPane2 = new javax.swing.JEditorPane();
        jButton2 = new javax.swing.JButton();
        facebookLogin = new javax.swing.JPanel();
        facebookIcon = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTextArea1 = new javax.swing.JTextArea();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Facebook");
        setMinimumSize(new java.awt.Dimension(759, 642));
        setResizable(false);

        mainPanel.setBackground(new java.awt.Color(21, 51, 179));
        mainPanel.setMaximumSize(new java.awt.Dimension(759, 642));
        mainPanel.setMinimumSize(new java.awt.Dimension(759, 642));
        mainPanel.setPreferredSize(new java.awt.Dimension(759, 642));
        mainPanel.setLayout(new java.awt.CardLayout());

        facebook.setBackground(new java.awt.Color(21, 51, 179));

        jButton1.setText("Update Feed");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        jScrollPane2.setViewportView(jEditorPane1);

        jEditorPane2.setMinimumSize(new java.awt.Dimension(50, 50));
        jEditorPane2.setPreferredSize(new java.awt.Dimension(50, 50));
        jScrollPane3.setViewportView(jEditorPane2);

        jButton2.setText("Set Status");

        javax.swing.GroupLayout facebookLayout = new javax.swing.GroupLayout(facebook);
        facebook.setLayout(facebookLayout);
        facebookLayout.setHorizontalGroup(
            facebookLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(facebookLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(facebookPic)
                .addGap(18, 18, 18)
                .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 86, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addGroup(facebookLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(facebookLayout.createSequentialGroup()
                        .addComponent(statusTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 402, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jButton2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addComponent(jButton1, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.Alignment.TRAILING))
                .addGap(214, 214, 214))
        );
        facebookLayout.setVerticalGroup(
            facebookLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, facebookLayout.createSequentialGroup()
                .addGap(21, 21, 21)
                .addComponent(facebookPic, javax.swing.GroupLayout.PREFERRED_SIZE, 66, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 17, Short.MAX_VALUE)
                .addGroup(facebookLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(statusTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 407, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jButton1)
                .addGap(96, 96, 96))
            .addGroup(facebookLayout.createSequentialGroup()
                .addGap(71, 71, 71)
                .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 70, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(529, Short.MAX_VALUE))
        );

        mainPanel.add(facebook, "card3");

        facebookLogin.setBackground(new java.awt.Color(21, 51, 179));

        jScrollPane1.setBorder(null);

        jTextArea1.setColumns(20);
        jTextArea1.setRows(5);
        jScrollPane1.setViewportView(jTextArea1);

        javax.swing.GroupLayout facebookLoginLayout = new javax.swing.GroupLayout(facebookLogin);
        facebookLogin.setLayout(facebookLoginLayout);
        facebookLoginLayout.setHorizontalGroup(
            facebookLoginLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(facebookLoginLayout.createSequentialGroup()
                .addGroup(facebookLoginLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(facebookLoginLayout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(facebookIcon))
                    .addGroup(facebookLoginLayout.createSequentialGroup()
                        .addGap(59, 59, 59)
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 646, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(63, Short.MAX_VALUE))
        );
        facebookLoginLayout.setVerticalGroup(
            facebookLoginLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(facebookLoginLayout.createSequentialGroup()
                .addGap(26, 26, 26)
                .addComponent(facebookIcon)
                .addGap(18, 18, 18)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 492, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(134, Short.MAX_VALUE))
        );

        mainPanel.add(facebookLogin, "card2");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(mainPanel, javax.swing.GroupLayout.PREFERRED_SIZE, 768, javax.swing.GroupLayout.PREFERRED_SIZE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(mainPanel, javax.swing.GroupLayout.PREFERRED_SIZE, 670, javax.swing.GroupLayout.PREFERRED_SIZE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
        try {
            // TODO add your handling code here:
            jEditorPane1.setContentType("text/html");
            myC.getMe();
            
            jEditorPane2.setVisible(true);
            jEditorPane2.setContentType("text/html");
            jEditorPane2.setText("<img width=50 height=50 src=" + myC.myPic + ">"  );
            jEditorPane1.setContentType("text/html;charset=UTF-8");
            myC.getHTMLPage();
            jEditorPane1.setPage("file:///C:/Users/Kamran/Documents/NetBeansProjects/Ciao! Chat/Ciao! Chat/result.html");
            



        } catch (FileNotFoundException ex) {
            Logger.getLogger(Facebook.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(Facebook.class.getName()).log(Level.SEVERE, null, ex);
        }
}//GEN-LAST:event_jButton1ActionPerformed

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
            java.util.logging.Logger.getLogger(Facebook.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Facebook.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Facebook.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Facebook.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /*
         * Create and display the form
         */
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                try {
                    new Facebook().setVisible(true);
                } catch (URISyntaxException ex) {
                    Logger.getLogger(Facebook.class.getName()).log(Level.SEVERE, null, ex);
                } catch (IOException ex) {
                    Logger.getLogger(Facebook.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JPanel facebook;
    private javax.swing.JLabel facebookIcon;
    private javax.swing.JPanel facebookLogin;
    private javax.swing.JLabel facebookPic;
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JEditorPane jEditorPane1;
    private javax.swing.JEditorPane jEditorPane2;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JTextArea jTextArea1;
    private javax.swing.JPanel mainPanel;
    private javax.swing.JTextField statusTextField;
    // End of variables declaration//GEN-END:variables
}