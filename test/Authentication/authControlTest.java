/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Authentication;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Parvaneh
 */
public class authControlTest {
    
    public authControlTest() {
    }

    @BeforeClass
    public static void setUpClass() throws Exception {
    }

    @AfterClass
    public static void tearDownClass() throws Exception {
    }

    /**
     * Test of login method, of class authControl.
     */
    @Test
    public void testLogin() {
        System.out.println("authControlTest: testLogin ");
        String username = "goodinges";
        char[] password = {'g','o','o','d','i','n','g','e','s'};
        boolean expResult = true;
        boolean result = authControl.login(username, password);
        assertEquals(expResult, result);
        
    }

    /**
     * Test of signUp method, of class authControl.
     */
    @Test
    public void testSignUp() {
        char[] password = {'p','e','d','r','a','m'};
        assertEquals(true,authControl.signUp("alij", password, 1, "tey", "kam", "booqi", false, null, null, null, "g@g.com"));
        assertEquals(false,authControl.signUp("ali", password, 1, "tey", "kam", "booqij", false, null, null, null, "g@g.com"));

        // TODO review the generated test code and remove the default call to fail
    }

    /**
     * Test of usernameTaken method, of class authControl.
     */
    @Test
    public void testUsernameTaken() throws Exception {
        System.out.println("usernameTakenTest : testUsernameTaken ");

        assertEquals(true, authControl.usernameTaken("p.rahimi"));
        assertEquals(true, authControl.usernameTaken("goodinges"));
        assertEquals(false, authControl.usernameTaken("taqi"));
        // TODO review the generated test code and remove the default call to fail.
    }

    /**
     * Test of forgetCred method, of class authControl.
     */
    @Test
    public void testForgetCred() throws Exception {
        System.out.println("forgetCredTest : testForgetCred");
       
        authControl.forgetCred("p.rahimi");
        // TODO review the generated test code and remove the default call to fail.
    }

    /**
     * Test of getUserInfo method, of class authControl.
     */
    @Test
    public void testGetUserInfo() {
        System.out.println("getUserInfoTest : testGetUserInfo");
        String username = "p.rahimi";
        String[] infos = new String[10];
        String[] expResult = new String[10];
        expResult[0] = "parvaneh";
        expResult[1] = "rahimi";
        String[] result = authControl.getUserInfo(username, infos);
        assertEquals(expResult, result);
        expResult[0] = "Kamran";
        expResult[1] = "Nobahat";
        assertEquals(expResult, authControl.getUserInfo("goodinges", infos));
        // TODO review the generated test code and remove the default call to fail.
    }
}
