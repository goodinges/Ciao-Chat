package com.skype.test;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.security.PrivateKey;
import java.security.cert.X509Certificate;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import com.skype.api.Account;
import com.skype.api.Skype;
import com.skype.ipc.TCPSocketTransport;
import com.skype.ipc.TLSServerTransport;
import com.skype.ipc.Transport;
import com.skype.util.PemReader;


public class LoginTest implements Callback
{
	static private Skype	      skype;
	static private Transport	  transport	= null;
	private static TestProperties	properties;

	private CountDownLatch	      latch;
	private LoginListener	      listener;

	/**
	 * @throws java.lang.Exception
	 */
	@BeforeClass
	public static void setUp() throws Exception
	{
		properties = new TestProperties();

		String runtime = properties.runtime;

		System.out.println("starting runtime");
		Runtime.getRuntime().exec(runtime);

		// note the crude pause to make sure runtime is alive for test
		System.out.println("Pause for runtime to start.");
		Thread.sleep(properties.runtimePause);
		
		System.out.println("Connecting to runtime.");
		skype = new Skype();
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
		System.out.println("Connected.");
	}

	@AfterClass
	public static void tearDown() throws IOException
	{
		skype.Close();
	}

	@Before
	public void registerListener() throws IOException
	{
		Assert.assertTrue(transport.isConnected());
		resetLatch();
		
		listener = new LoginListener(this);
		skype.RegisterListener(Account.moduleID(), listener);

		listener.start();
	}

	@After
	public void uregisterListener()
	{
		skype.UnRegisterListener(Account.moduleID(), listener);
		listener.interrupt();
	}

	public void completed()
	{
		System.out.println("completed");
		latch.countDown();
	}
	
	private void resetLatch()
	{
		latch = new CountDownLatch(1);
	}
	
	private void waitForLatch() throws InterruptedException
	{
		latch.await(3, TimeUnit.SECONDS);
	}

	@Test
	public void testSimpleLogin() throws InterruptedException, IOException
	{
		// Log in
		System.out.println("testSimpleLogin");
		Account account = skype.GetAccount(properties.user1);
		assertNotNull(account);
		account.LoginWithPassword(properties.password1, false, false);
		waitForLatch();
		Assert.assertEquals(Account.STATUS.LOGGED_IN, listener.getStatus());

		// Log out
		resetLatch();
		account.Logout(true);
		waitForLatch();
		Assert.assertEquals(Account.STATUS.LOGGED_OUT, listener.getStatus());
	}

	@Test
	public void testFailedLogin() throws InterruptedException, IOException
	{
		// Log in with bad password
		System.out.println("testFailedLogin");

		Account account = skype.GetAccount(properties.user1);
		account.LoginWithPassword(properties.password1 + "has_no_clothes", false, false);
		waitForLatch();
		Assert.assertEquals(Account.STATUS.LOGGED_OUT, listener.getStatus());
		Assert.assertEquals(Account.LOGOUTREASON.INCORRECT_PASSWORD, listener.getReason());

		// Log in and attempt another login
		resetLatch();
		account.LoginWithPassword(properties.password1, false, false);
		waitForLatch();
		Assert.assertEquals(Account.STATUS.LOGGED_IN, listener.getStatus());

		resetLatch();
		Account second = skype.GetAccount(properties.user2);
		second.LoginWithPassword(properties.password2, false, false);
		waitForLatch();
		Assert.assertEquals(Account.STATUS.LOGGED_OUT, listener.getStatus());
		
		// Log out
		resetLatch();
		account.Logout(true);
		waitForLatch();
		Assert.assertEquals(Account.STATUS.LOGGED_OUT, listener.getStatus());
	}
}
