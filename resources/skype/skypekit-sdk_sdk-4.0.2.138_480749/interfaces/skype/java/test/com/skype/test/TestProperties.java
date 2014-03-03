package com.skype.test;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;


public class TestProperties
{
	public String	pemFileName;
	public String	inetAddr;
	public int	  portNum;
	public int	  runtimePause;
	public String	runtime;
	public String	user1;
	public String	password1;
	public String	user2;
	public String	password2;
	public String	invaliduser;

	public TestProperties() throws IOException {
		ClassLoader loader = this.getClass().getClassLoader();
		InputStream in = loader.getResourceAsStream("skypekit-test.properties");
		Properties props = new Properties();
		try {
			props.load(in);
		}
		catch (IOException e) {
			System.out.print("unable to read test configuartion properties");
			throw e;
		}

		pemFileName = props.getProperty("pemFileName").toString();
		inetAddr = props.getProperty("ipaddr", "127.0.0.1").toString();
		portNum = Integer.parseInt(props.getProperty("port", "8963"));
		
		runtimePause = Integer.parseInt(props.getProperty("pause", "1000"));
		runtime = props.getProperty("runtime").toString();
		
		user1 = props.getProperty("user1", "bernalkc.t1");
		password1 = props.getProperty("password1", "foobar0");
		user2 = props.getProperty("user2", "bernalkc.t2");
		password2 = props.getProperty("password2", "foobar0");
		invaliduser = props.getProperty("invalid_username", "---");
	}
}
