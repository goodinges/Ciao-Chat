/*
 * To change this template, choose Tools | Templates and open the template in
 * the editor.
 */
package Control.twt;

import FX.FXTwitterPostLogin;
import FX.Twitter;
import Twitter2.twitteroauth.twitterresponse.StatusType;
import Twitter2.twitteroauth.twitterresponse.Statuses;
import UserInterface.TwitterForm;
import com.sun.jersey.api.client.Client;
import com.sun.jersey.api.client.UniformInterfaceException;
import com.sun.jersey.api.client.WebResource;
import com.sun.jersey.api.representation.Form;
import com.sun.jersey.oauth.client.OAuthClientFilter;
import com.sun.jersey.oauth.signature.OAuthParameters;
import com.sun.jersey.oauth.signature.OAuthSecrets;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.SwingUtilities;
import javax.ws.rs.core.MultivaluedMap;

/**
 *
 * @author Kamran
 */
public class TwitterClient {
public DefaultListModel statusesListModel = new DefaultListModel();
    Twitter form;
    private Form requestTokenResponse;
    private String oauth_verifier;

    public void enterPinContinue() throws MalformedURLException  {
        System.out.println(requestTokenResponse.getFirst("oauth_token"));
        try {
            new FXTwitterPostLogin().start(form.getStage());
        } catch (Exception ex) {
            Logger.getLogger(TwitterClient.class.getName()).log(Level.SEVERE, null, ex);
        }
        client.login3(requestTokenResponse, oauth_verifier);
        client.initOAuth();


        //Call getUserTimeline, get a list of statuses, pass the most recent
        //status as a StatusType object, and display the text of that object
        //in the JTextField
        Twitter2.twitteroauth.twitterresponse.Statuses statuses = client.getUserTimeline(Twitter2.twitteroauth.twitterresponse.Statuses.class, null, null, null, "1");
        Twitter2.twitteroauth.twitterresponse.StatusType st = statuses.getStatus().get(0);
        FXTwitterPostLogin fxtpl = new FXTwitterPostLogin();
        fxtpl.setStatus(st.getText().trim());
        try {
            fxtpl.start(form.getStage());
        } catch (Exception ex) {
            Logger.getLogger(TwitterClient.class.getName()).log(Level.SEVERE, null, ex);
        }


        //Get a UserType object from the StatusType object, get the URL of that
        //user's icon, and display that icon in the JLabel
        Twitter2.twitteroauth.twitterresponse.UserType user = st.getUser();
        String iconSrc = user.getProfileImageUrl();
        URL iconUrl = new URL(iconSrc);
        ImageIcon icon = new ImageIcon(iconUrl, user.getScreenName());
        fxtpl.seticon(icon);
    }

    public void login(Twitter aThis) {
        try {
            Timer t = new Timer("Twitter Updater`", false);
        t.scheduleAtFixedRate(new TimerTask() {

            @Override
            public void run() {
                System.out.println("Timer Task is running");
                try {
                    client.initOAuth();
                    Statuses response = client.getFriendsTimeline(Statuses.class, null, null, null, "10");
                    // Clear the list model so it does not replicate the contents from the last run
                    statusesListModel.clear();
                    // Create a Status Type object for every status in the Status list, and add an element
                    // to the list model for every status type object
                    for (final StatusType st : response.getStatus()) {
                        SwingUtilities.invokeLater(new Runnable() {

                            @Override
                            public void run() {
                                statusesListModel.addElement(st);
                            }
                            
                        });
                    }
                } catch (UniformInterfaceException ex) {
                    System.out.println("Exception when calling getFriendsTimeline = " + ex.getResponse().getEntity(String.class));
                }
            }
        }, 30000, 10000);
            form = aThis;
            initUserInfo();

        } catch (IOException ex) {
            Logger.getLogger(TwitterCl.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public TwitterClient() {
    }

    private void initUserInfo() throws MalformedURLException, IOException {


        //Create an instance of the internal service class
        client = new TwitterCl("xml");


        //Log in, get tokens, and append the tokens to the consumer and secret
        //keys
        requestTokenResponse = client.login();
        //oauth_verifier = client.login2(requestTokenResponse);
        Twitter.gotourl("http://twitter.com/oauth/authorize?oauth_token=" + requestTokenResponse.getFirst("oauth_token"));
        
    }
    public TwitterCl client;

    public void enterPinContinue(String pin) {
        oauth_verifier = pin;
        try {
            enterPinContinue();
        } catch (MalformedURLException ex) {
            Logger.getLogger(TwitterClient.class.getName()).log(Level.SEVERE, null, ex);
        } catch (UniformInterfaceException ex) {
            Logger.getLogger(TwitterClient.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }

    public static class TwitterCl {

        private WebResource webResource;
        private Client client;
        private static final String BASE_URI = "http://twitter.com";

        public TwitterCl(String format) {
            com.sun.jersey.api.client.config.ClientConfig config = new com.sun.jersey.api.client.config.DefaultClientConfig();
            client = Client.create(config);
            String resourcePath = java.text.MessageFormat.format("statuses", new Object[]{format});
            webResource = client.resource(BASE_URI).path(resourcePath);
        }
        private static final String OAUTH_BASE_URL = "http://twitter.com/oauth";
        /**
         * Please, specify the consumer_key string obtained from service API
         * pages
         */
        private static final String CONSUMER_KEY = "SZbi7rCzpHHDbDi6o0mvw";
        /**
         * Please, specify the consumer_secret string obtained from service API
         * pages
         */
        private static final String CONSUMER_SECRET = "USDCp4vIdnujwEdvY2SnrT4jHaTFIelevjylxd5s";
        private OAuthParameters oauth_params;
        private OAuthSecrets oauth_secrets;
        private OAuthClientFilter oauth_filter;
        private String oauth_access_token;
        private String oauth_access_token_secret;

        public String login2(Form requestTokenResponse) throws IOException, UniformInterfaceException {
            String oauth_verifier = authorizeConsumer(requestTokenResponse);
            return oauth_verifier;

        }

        public void login3(Form requestTokenResponse, String oauth_verifier)  {
            Form accessTokenResponse = getOAuthAccessToken(requestTokenResponse, oauth_verifier);
            oauth_access_token_secret = accessTokenResponse.getFirst("oauth_token_secret");
            oauth_access_token = accessTokenResponse.getFirst("oauth_token");
        }

        public void setResourcePath(String format) {
            String resourcePath = java.text.MessageFormat.format("statuses/user_timeline.{0}", new Object[]{format});
            webResource = client.resource(BASE_URI).path(resourcePath);
        }

        /**
         * @param responseType Class representing the response
         * @param since query parameter
         * @param since_id query parameter
         * @param page query parameter
         * @param count query parameter
         * @return response object (instance of responseType class)
         */
        public <T> T getUserTimeline(Class<T> responseType, String since, String since_id, String page, String count) throws UniformInterfaceException {
            String[] queryParamNames = new String[]{"since", "since_id", "page", "count"};
            String[] queryParamValues = new String[]{since, since_id, page, count};
            return webResource.path("user_timeline.xml").queryParams(getQueryOrFormParams(queryParamNames, queryParamValues)).accept(javax.ws.rs.core.MediaType.TEXT_XML).get(responseType);
        }
        public <T> T getFriendsTimeline(Class<T> responseType, String since, String since_id, String page, String count) throws UniformInterfaceException {
            String[] queryParamNames = new String[]{"since", "since_id", "page", "count"};
            String[] queryParamValues = new String[]{since, since_id, page, count};
            return webResource.path("friends_timeline.xml").queryParams(getQueryOrFormParams(queryParamNames, queryParamValues)).accept(javax.ws.rs.core.MediaType.TEXT_XML).get(responseType);
        }

        public <T> T updateStatus(Class<T> responseType, String status, String in_reply_to_status_id) throws UniformInterfaceException {
            String[] formParamNames = new String[]{"status", "in_reply_to_status_id"};
            String[] formParamValues = new String[]{status, in_reply_to_status_id};
            return webResource.path("update.xml").type(javax.ws.rs.core.MediaType.APPLICATION_FORM_URLENCODED).post(responseType, getQueryOrFormParams(formParamNames, formParamValues));
        }

        private MultivaluedMap getQueryOrFormParams(String[] paramNames, String[] paramValues) {
            MultivaluedMap<String, String> qParams = new com.sun.jersey.api.representation.Form();
            for (int i = 0; i < paramNames.length; i++) {
                if (paramValues[i] != null) {
                    qParams.add(paramNames[i], paramValues[i]);
                }
            }
            return qParams;
        }

        public void close() {
            client.destroy();
        }

        /**
         * You need to call this method at the beginning to authorize the application to work with user data.
         * The method obtains the OAuth access token string, that is appended to each API request later.
         */
        public Form login() throws IOException, UniformInterfaceException {
            Form requestTokenResponse = getOAuthRequestToken();
            return requestTokenResponse;
        }

        private Form getOAuthRequestToken(){
            WebResource resource = client.resource(OAUTH_BASE_URL).path("request_token");
            oauth_params = new OAuthParameters().consumerKey(CONSUMER_KEY).signatureMethod(com.sun.jersey.oauth.signature.HMAC_SHA1.NAME).version("1.0").nonce().timestamp();
            oauth_secrets = new OAuthSecrets().consumerSecret(CONSUMER_SECRET);
            oauth_filter = new OAuthClientFilter(client.getProviders(), oauth_params, oauth_secrets);
            resource.addFilter(oauth_filter);
            return resource.get(Form.class);
        }

        private Form getOAuthAccessToken(Form requestTokenResponse, String oauth_verifier) {
            WebResource resource = client.resource(OAUTH_BASE_URL).path("access_token");
            oauth_params.token(requestTokenResponse.getFirst("oauth_token")).signatureMethod(com.sun.jersey.oauth.signature.HMAC_SHA1.NAME).version("1.0").nonce().timestamp().verifier(oauth_verifier);
            oauth_secrets.tokenSecret(requestTokenResponse.getFirst("oauth_token_secret"));
            resource.addFilter(oauth_filter);
            return resource.get(Form.class);
        }

        /**
         * The method sets the OAuth parameters for webResource.
         * The method needs to be called after login() method, or when the webResource path is changed
         */
        public void initOAuth() {
            oauth_params = new OAuthParameters().consumerKey(CONSUMER_KEY).token(oauth_access_token).signatureMethod(com.sun.jersey.oauth.signature.HMAC_SHA1.NAME).version("1.0").nonce().timestamp();
            oauth_secrets = new OAuthSecrets().consumerSecret(CONSUMER_SECRET).tokenSecret(oauth_access_token_secret);
            oauth_filter = new OAuthClientFilter(client.getProviders(), oauth_params, oauth_secrets);
            webResource.addFilter(oauth_filter);
        }

        /**
         * The method increases OAuth nonce and timestamp parameters to make each request unique.
         * The method should be called when repetitive requests are sent to service API provider:
         * <pre>
         * client.initOauth();
         * client.getXXX(...);
         * client.makeOAuthRequestUnique();
         * client.getYYY(...);
         * client.makeOAuthRequestUnique();
         * client.getZZZ(...);
         * </pre>
         */
        public void makeOAuthRequestUnique() {
            if (oauth_params != null) {
                oauth_params.nonce().timestamp();
            }
        }

        private java.lang.String authorizeConsumer(Form requestTokenResponse) throws IOException {
//            try {
//                java.awt.Desktop.getDesktop().browse(new java.net.URI("http://twitter.com/oauth/authorize?oauth_token=" + requestTokenResponse.getFirst("oauth_token")));
//            } catch (java.net.URISyntaxException ex) {
//                ex.printStackTrace();
//            }
            java.io.BufferedReader br = null;
            String oauth_verifier = null;
            try {
                br = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
                System.out.print("Type oauth_verifier string (taken from callback page url):");
                oauth_verifier = br.readLine();
            } finally {
                br.close();
            }
            return oauth_verifier;
        }
    }
}
