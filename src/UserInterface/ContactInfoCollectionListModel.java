/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package UserInterface;

import com.skype.api.Contact;
import java.util.Iterator;
import java.util.Vector;
import javax.swing.*;
import javax.swing.event.*;
import org.jivesoftware.smack.Roster;
import org.jivesoftware.smack.RosterEntry;

/**
 *
 * @author Parvaneh
 */
public class ContactInfoCollectionListModel implements ListModel {

    public Vector<String> vc;

    public ContactInfoCollectionListModel(Vector<String> vc) {
        if (vc == null) {
            vc = new Vector<String>();
        } else {
            this.vc = vc;
        }
    }

    public ContactInfoCollectionListModel(Roster roster) {
        String gmail = "@gmail.com";
        vc = new Vector<String>();
        Iterator<RosterEntry> it = roster.getEntries().iterator();
        while (it.hasNext()) {
            String str[] = it.next().getUser().split(gmail);
            vc.add(str[0]);
        }
    }

    public ContactInfoCollectionListModel(Contact[] contacts) {
        vc = new Vector<String>();
        for(int i=0;i<contacts.length;i++){
            vc.add(contacts[i].GetIdentity());
        }
    }
    
    
    public Object getElementAt(int index) {
        return (vc.elementAt(index));
    }

    public int getSize() {
        int size = vc.size();
        //System.out.println(size);
        return (size);
    }

    public void addElement(String str) {
        vc.add(str);
    }

    public void addListDataListener(ListDataListener l) {
    }

    public void removeListDataListener(ListDataListener l) {
    }
}
