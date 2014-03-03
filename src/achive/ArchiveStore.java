/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package achive;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;


/**
 *
 * @author M0C
 */
public class ArchiveStore
{
    private String desPath;
    private String hostChatter;
    private String guestChatter;
    private String crlf=new String();
    private FileOutputStream file;

    
    public ArchiveStore(String desinatin_Path,String host_Chatter,String guest_Chatter){
        desPath=new String();
        hostChatter=new String();
        guestChatter=new String();
        crlf=Character.toString((char)13)+ Character.toString((char)10);
        
        set_Destination_Path(desinatin_Path);
        set_Host_Chatter(host_Chatter);
        set_Guest_Chatter(guest_Chatter);
        
        //Bebine age folder be esme host chatter vojod nadare foldersho doros kone
        String chatterPath=new String();
        chatterPath=this.desPath + "\\" + hostChatter;
        
        File archDir=new File(chatterPath);
        if (!archDir.exists())
        {
            archDir.mkdir();
        }
        
        //File ba formate zir ijad kone
        //guestChatter-20101112.dat
        
        String todayDateTime=new String();
        String fileName=new String();
        
        DateFormat dateFormat = new SimpleDateFormat("yyyy_MM_dd-HH_mm_ss");
        Date date = new Date();
        todayDateTime= dateFormat.format(date);
        
        fileName=chatterPath + "\\" + guestChatter + "-" + todayDateTime + ".archive";
        
        

        try{
        file=new FileOutputStream(fileName,true);
        }
        catch(FileNotFoundException ex)
        {
            System.out.println("File not found.");
        }
        catch(SecurityException ex)
        {
            System.out.println("You dont have write access on this file.");
        }  
                
    }

    public void end_Of_Session() throws IOException
    {
        file.close();
    }
    public void host_Sends_Msg(String message) throws IOException
    {
        msg_Sent(hostChatter,message);
    }
    public void guest_Sends_Msg(String message) throws IOException
    {
       msg_Sent(guestChatter,message);
    }
    private void msg_Sent(String Sender,String message) throws IOException
    {
        String output=new String();
        output=Sender + " (" + getTime() + ") : " + message;
        output=output+crlf;
        file.write(output.getBytes()); 
    }

    private void set_Destination_Path(String desPath)
    {
        this.desPath=desPath; 
    }
    private void set_Host_Chatter(String hostChatter)
    {
        this.hostChatter=hostChatter;
    }
    private void set_Guest_Chatter(String guestChatter)
    {
        this.guestChatter=guestChatter;
    }
    
    private String getTime() {
        DateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
        Date date = new Date();
        return dateFormat.format(date);
    }

}
