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
public class ConnectionTest
{
	private static TestProperties	properties;

	private Skype	              skype;
	private Transport	          transport;

	/**
	 * @throws java.lang.Exception
	 */
	@BeforeClass
	public static void setUpBeforeClass() throws Exception
	{
		properties = new TestProperties();

		String runtime = properties.runtime;

		Runtime.getRuntime().exec(runtime);
		// note the crude pause in constructor used to make sure runtime is
		// alive for test
	}

	/**
	 * @throws java.lang.Exception
	 */
	@Before
	public void setUp() throws Exception
	{
		Thread.sleep(properties.runtimePause);
		skype = new Skype();
	}

	/**
	 * Test method for {@link com.skype.ipc.TLSServerTransport#connect()}.
	 * 
	 * @throws IOException
	 * @throws InvalidKeySpecException
	 */
	@Test
	public void testConnect() throws IOException, InvalidKeySpecException
	{
		PemReader donkey = new PemReader(properties.pemFileName);
		X509Certificate c = donkey.getCertificate();
		assertNotNull(c);
		PrivateKey p = donkey.getKey();
		assertNotNull(p);
		Transport t = new TCPSocketTransport(properties.inetAddr, properties.portNum);
		assertNotNull(t);

		transport = new TLSServerTransport(t, c, p);
		assertNotNull(transport);

		assertTrue( ! transport.isConnected());
		skype.Init(transport);
		assertTrue(transport.isConnected());

		String v = skype.GetVersionString();
		assertTrue(v.length() > 0);

		skype.Close();
		assertTrue( ! transport.isConnected());
	}

}
