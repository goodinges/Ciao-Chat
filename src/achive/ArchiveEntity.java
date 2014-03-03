/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package achive;

/**
 *
 * @author M0C
 */
public class ArchiveEntity 
{
    public String chatDate;
    public String chatTime;
    public String hostChatter;
    public String guestChatter;
    public String text;
    
    public long date2Number()
    {
        String temp=new String();
        temp=chatDate.replace("/", "");
        return Long.parseLong(temp);
    }
}
