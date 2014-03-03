/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Facebook;

import chrriis.common.UIUtils;
import chrriis.dj.nativeswing.swtimpl.NativeInterface;
import com.restfb.BinaryAttachment;
import com.restfb.Connection;
import com.restfb.DefaultFacebookClient;
import com.restfb.FacebookClient;
import com.restfb.Parameter;
import com.restfb.json.JsonObject;
import com.restfb.types.FacebookType;
import com.restfb.types.Page;
import com.restfb.types.Post;
import com.restfb.types.User;
import java.awt.BorderLayout;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.util.List;
import java.util.Scanner;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

/**
 *
 * @author M0C
 */
public class CiaoFacebook 
{
   
    Scanner input;
    FacebookClient facebookClient ;
    FacebookClient publicOnlyFacebookClient = new DefaultFacebookClient();
    
    public String accessToken;
    public String myName;
    public String myID;
    public String myPic;
    
    int myFriendsCount;
    User[] friends;
    Post[] feeds;
    
    public static browserPanelFB browser;
    
    public void Login() throws URISyntaxException, IOException
    {
        String theURL=new String();
        //theURL="http://ciao.m0c.ir";
        theURL="https://www.facebook.com/dialog/oauth?client_id=250542098342156&redirect_uri=https://www.facebook.com/connect/login_success.html&scope=read_stream,user_interests,friends_interests,user_likes,friends_likes,user_location,friends_location,user_online_presence,friends_online_presence,user_photos,friends_photos,user_relationships,friends_relationships,user_status,friends_status,read_friendlists,read_requests,read_stream,xmpp_login,create_event,manage_friendlists,manage_notifications,publish_stream,offline_access&response_type=token";
        //theURL="http://ciao.m0c.ir/fb.htm";
        //URI temp=new URI(theURL);
        browser = new browserPanelFB(theURL);
        browser.setFacebook(this);
        UIUtils.setPreferredLookAndFeel();
        NativeInterface.open();
        SwingUtilities.invokeLater(new Runnable() {
        
            
            public void run() {
                JFrame frame = new JFrame();
                frame.setTitle("Loading....");
                frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                frame.getContentPane().add(browser, BorderLayout.CENTER);
                frame.setSize(1100, 700);
                frame.setLocationByPlatform(true);
                frame.setVisible(true);
                browser.setJframe(frame);
            }
        });    
        //Desktop.getDesktop().browse(temp);
        
        
        
    }
    public void done() throws MalformedURLException, IOException
    {
        //.
        Start();
        
        
    }
    public String[][] getMyFriendsStatus()
    {
        if (friends==null)
            getMyFriends();

        String [][] myReturnObj=new String[myFriendsCount][2];
        
        String sequenceOfFriendIDs=new String();
        sequenceOfFriendIDs="";
        for(int i=0;i<myFriendsCount;i++)
        {
            sequenceOfFriendIDs=sequenceOfFriendIDs + "uid=" + friends[i].getId() + " or ";
        }
        sequenceOfFriendIDs=sequenceOfFriendIDs.substring(0, sequenceOfFriendIDs.length()-4);
        String query = "SELECT status_id FROM status WHERE " + sequenceOfFriendIDs ;
        List<JsonObject> result = facebookClient.executeQuery(query, JsonObject.class);
        
        String statusID=new String();
        JsonObject status;
        String fromWho=new String();
        String[] parts;
        
        for(int i=0;i<10;i++)
        {


            
            statusID =result.get(i).getString("status_id");
            status = facebookClient.fetchObject(statusID, JsonObject.class);
        
            fromWho=status.getString("from");
            parts=fromWho.split("\"");
            
            myReturnObj[i][0]=parts[7];
            myReturnObj[i][1]=status.getString("message");
        }
        return myReturnObj;
 
        //https://graph.facebook.com/1206320099/feed?access_token=AAADj3eDF4QwBAEwym03EcwbdwVOWd05lxZChbSbDUYkmjrT8CtZBLwp9VsOeiblZBxzEPyfEAH3UOyWz6zDDu4WvA0PYS0ZD
    }
    public String getFriendID(String FriendName)
    {
        String friendID=new String();
                
        if (friends==null)
            getMyFriends();

        for(int i=0;i<friends.length;i++)
        {
            if(friends[i].getName().equals(FriendName))
            {
                friendID=friends[i].getId().trim();
            }
        }
        return friendID;
    }
    public String getFriendPic(String FriendName)
    {

        return "https://graph.facebook.com/" + getFriendID(FriendName) + "/picture?access_token=" + accessToken;


    }
    
    public void Start() throws MalformedURLException, IOException
    {
       //accessToken=new String();

       

        //URL codePage = new URL("http://ciao.m0c.ir/code.txt");
        //BufferedReader in = new BufferedReader(
        //new InputStreamReader(
        //codePage.openStream()));

        //String inputLine;


        //accessToken=in.readLine();
        
        //in.close();
       
       


        //System.out.println(accessToken);

        facebookClient = new DefaultFacebookClient(accessToken);
        //System.out.println(getMyName());


    }
    
    public long publishStatus(String myStatus)           //Returns message ID, if message ID=0 then the status has not been updated
    {
        FacebookType publishMessageResponse =
        facebookClient.publish("me/feed", FacebookType.class,
        Parameter.with("message", myStatus));

        return Long.parseLong(publishMessageResponse.getId());

    }
    
    public void getMe()
    {
        User user = facebookClient.fetchObject("me", User.class);
        myName= user.getName();
        myID=user.getId();
        myPic="https://graph.facebook.com/" + myID + "/picture?access_token=" + accessToken;
    }



    public void getMyFriends()
    {
        Connection<User> myFriends = facebookClient.fetchConnection("me/friends", User.class);
        myFriendsCount=myFriends.getData().size();
        friends=new User[myFriendsCount];
        for (int i=0;i<myFriends.getData().size();i++)
        {
            friends[i]=new User();
            friends[i]=myFriends.getData().get(i);
        }

    }

    public void getMyFeed()
    {
        Connection<Post> myFeed = facebookClient.fetchConnection("me/feed", Post.class);
        
        feeds=new Post[myFeed.getData().size()];
        for (int i=0;i<myFeed.getData().size();i++)
        {
            feeds[i]=new Post();
            feeds[i]=myFeed.getData().get(i);
        }
        

    }
    
    
    public int getMyFriendsCount()
    {
        return myFriendsCount;
    }

    public Page getPage(String PageName)
    {
        Page myPage = facebookClient.fetchObject(PageName, Page.class);
        return myPage;
    }
    
    public long publishPhoto()      //Not completely implemented yet!
    {
        FacebookType publishPhotoResponse = facebookClient.publish("me/photos", FacebookType.class,
        BinaryAttachment.with("cat.png", getClass().getResourceAsStream("/cat.png")),
        Parameter.with("message", "Test cat"));

        return  Long.parseLong(publishPhotoResponse.getId());

    }
    
    public void getHTMLPage() throws FileNotFoundException, IOException
    {
        String[][] stats;
        stats=getMyFriendsStatus();
        
        String resultHTML=new String();
        
        
        String header=new String();
        String eachStat=new String();
        String allStats=new String();
        String footer=new String();
        
        input=new Scanner(new File("header.txt"));
        while(input.hasNextLine())
        {
                
            header=header + input.nextLine() + "\n";
        }     
        
        input=new Scanner(new File("eachStat.txt"));
        while(input.hasNextLine())
        {
                
            eachStat=eachStat + input.nextLine() + "\n";
        }         
        
        input=new Scanner(new File("footer.txt"));
        while(input.hasNextLine())
        {
                
            footer=footer + input.nextLine() + "\n";
        }            
        
        allStats="";
        resultHTML=header;
        String temp=new String();
        
        for(int i=0;i<10;i++)
        {
            temp=eachStat;
            temp=temp.replaceAll("#userName", stats[i][0]);
            temp=temp.replaceAll("#userStatus", stats[i][1]);
            temp=temp.replaceAll("src=statuses.htm", "src=" + getFriendPic(stats[i][0]));
            allStats=allStats + temp;
        }
        resultHTML=resultHTML+allStats + footer;
        File toDel=new File("result.html");
        toDel.delete();
        
        FileOutputStream file=new FileOutputStream("result.html",true);
        String crlf=Character.toString((char)13)+ Character.toString((char)10);
        file.write(resultHTML.getBytes()); 
        file.close();
    }
}