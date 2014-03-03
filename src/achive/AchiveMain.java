/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package achive;

import java.io.IOException;

/**
 *
 * @author M0C
 */
public class AchiveMain {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws IOException {
        // TODO code application logic here
        ArchiverViewer myF=new ArchiverViewer();
        myF.set_Host_Chatter("host");
        myF.setVisible(true);
        
    }


}
