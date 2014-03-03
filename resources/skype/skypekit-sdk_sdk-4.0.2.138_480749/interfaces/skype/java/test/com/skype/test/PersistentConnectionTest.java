/**
 * 
 */
package com.skype.test;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.security.PrivateKey;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import com.skype.api.Skype;
import com.skype.ipc.TCPSocketTransport;
import com.skype.ipc.TLSServerTransport;
import com.skype.ipc.Transport;
import com.skype.util.PemReader;

/**
 * @author kcjones
 * 
 */
public class PersistentConnectionTest
{
    private static TestProperties properties;

    private Skype              skype;
    private Transport          transport;
    private X509Certificate    c;
    private PrivateKey         p;

    /**
     * @throws java.lang.Exception
     */
    @BeforeClass
    public static void setUpBeforeClass() throws Exception
    {
        properties = new TestProperties();
        String runtime = properties.runtime + " -m";

        Runtime.getRuntime().exec(runtime);
    }

    /**
     * @throws java.lang.Exception
     */
    @Before
    public void setUp() throws Exception
    {
        Thread.sleep(properties.runtimePause);
        skype = new Skype();

        PemReader donkey = new PemReader(properties.pemFileName);
        c = donkey.getCertificate();
        assertNotNull(c);
        p = donkey.getKey();
        assertNotNull(p);
    }
    
    private void simpleConnect() throws IOException, InvalidKeySpecException
    {
        TCPSocketTransport t = new TCPSocketTransport(properties.inetAddr, properties.portNum);
        assertNotNull(t);

        transport = new TLSServerTransport(t, c, p);
        assertNotNull(transport);
//        assertTrue(transport.isConnected());
        
        skype.Init(transport);

        String v = skype.GetVersionString();
        assertTrue(v.length() > 0);

        skype.Close();
        assertTrue(!t.isConnected());
    }

    @Test
    public void testConnectOnce() throws IOException, InvalidKeySpecException
    {
        simpleConnect();
    }

    @Ignore("pending fix for SKYPEKIT-343") @Test
    public void testConnectMultiple() throws IOException, InvalidKeySpecException
    {
        for (int i = 0; i < 10; i++) {
            simpleConnect();
        }
    }

}
