/**
 * Copyright (C) 2010, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

package com.skype.ipc;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.Principal;
import java.security.PrivateKey;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.LinkedList;
import java.lang.reflect.Method;

import javax.net.ssl.KeyManager;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLEngine;
import javax.net.ssl.SSLEngineResult;
import javax.net.ssl.SSLEngineResult.HandshakeStatus;
import javax.net.ssl.SSLEngineResult.Status;
import javax.net.ssl.SSLException;
import javax.net.ssl.SSLParameters;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509ExtendedKeyManager;
import javax.net.ssl.X509TrustManager;

import com.skype.util.Log;

/**
 * @author kcjones
 *
 */
public class TLSServerTransport implements Transport
{
	private static final String TAG = "TLSServerTransport";
	private static int MAXBUF;
	
	private boolean connected = false;
	private Transport transport = null;
	private SSLEngine engine = null;
	private SSLContext context = null;
    private FileOutputStream logFileOut = null;
    private FileOutputStream logFileIn = null;
    private boolean isLogging = false;

	private X509ExtendedKeyManager keyManager = null;
	private TrustManager trustManager = null;

	private byte decrypted_bytes[] = new byte[]{};
	private LinkedList<byte[]> decrypted_chunks = new LinkedList<byte[]>();
    private ByteBuffer raw_source = null;

    public TLSServerTransport(Transport t, X509Certificate cert, PrivateKey privKey)
    {
        transport = t;

        trustManager = new dummyTrustManager();
        keyManager = new dummyKeyManager(cert,privKey);
        decrypted_chunks.clear();
    }
	
	protected void finalize() throws Throwable
	{
	    super.finalize();
	    if (isLogging) {
	        logFileOut.flush();
	        logFileOut.close();
	        logFileIn.flush();
	        logFileIn.close();
	    }
	}

	@Override
	public void startLogging(String logFileName)
	{
	    assert(!isLogging);
	    
        if (logFileName != null && ! logFileName.isEmpty()) {
            try {
                logFileOut = new FileOutputStream(new File(logFileName + ".out.bin"), false);
                logFileIn  = new FileOutputStream(new File(logFileName + ".in.bin"), false);
                isLogging = true;
                Log.d(TAG, "Transport logging started");
            }
            catch (FileNotFoundException e) {
                Log.e(TAG, "Unable to open transport log files using:" + logFileName);
            }
        }
	}
	
	void do_handshake() throws SSLException, IOException 
	{
	        
	        Log.d(TAG, "do_handshake()");
	        SSLEngineResult engineResult = null;
	        HandshakeStatus result =  HandshakeStatus.NEED_UNWRAP;
	        while(true) {
	            switch(result) {
	            case FINISHED:
	                Log.d(TAG, result.toString());
	                return;
	            case NEED_TASK:
	                Log.d(TAG, result.toString());
	                Runnable task;
	                while((task=engine.getDelegatedTask()) != null) {
	                    task.run();
	                }
	                break;
	            case NEED_UNWRAP: {

	                byte [] msg = new byte [16384];
	                int idx = 0;
	                Log.d(TAG, result.toString());

	                if (decrypted_bytes.length == 0) {
	                    try {
	                        transport.peek();
	                        while(transport.hasMore() ) {
	                            msg[idx++] = (byte) transport.read();
	                        }
	                    }
	                    catch (IOException e) {
	                        Log.e(TAG, "Read exception:" + e.getMessage() + ":" + e.getCause());
	                        throw e;
	                    }
	                }
	                else {
	                    idx = decrypted_bytes.length;
	                    System.arraycopy(decrypted_bytes, 0, msg, 0, idx);
	                    decrypted_bytes = new byte[]{};
	                }

	                ByteBuffer src0 = ByteBuffer.wrap(msg);
	                ByteBuffer dst0 = ByteBuffer.allocate(16384);
	                engineResult = engine.unwrap(src0, dst0);
	                result = engineResult.getHandshakeStatus();
	                int bytesConsumed = engineResult.bytesConsumed();

	                int sum = 0; //check if we had trailing zeroes
	                while(bytesConsumed < idx)
	                    sum |= msg[bytesConsumed++];

	                int bytesLeftOver = idx - engineResult.bytesConsumed();
	                if (sum != 0 && bytesLeftOver > 0 ) { //more than one message, keep a copy
	                    decrypted_bytes = new byte[bytesLeftOver];
	                    System.arraycopy(msg, engineResult.bytesConsumed(), 
	                            decrypted_bytes, 0, bytesLeftOver);
	                }
	                break;
	            }
	            case NEED_WRAP: {
	                Log.d(TAG, result.toString());
	                ByteBuffer EMPTY = ByteBuffer.allocate(0);
	                ByteBuffer dst1 = ByteBuffer.allocate(65536);
	                engineResult = engine.wrap(EMPTY, dst1);
	                byte [] bytes2send = new byte[engineResult.bytesProduced()];
	                dst1.rewind();
	                dst1.get(bytes2send);
	                transport.write(bytes2send.length , bytes2send);
	                break;
	            }
	            case NOT_HANDSHAKING:
	                Log.d(TAG, "discarding " + decrypted_bytes.length + " bytes");
	                if (decrypted_bytes.length > 0) {
	                    ByteBuffer s = ByteBuffer.wrap(decrypted_bytes);
	                    Log.hexdump(s.array());
	                    decrypted_bytes = new byte[] {};
	                }
	                Log.d(TAG, result.toString());
	                return;
	            }
	            result = engine.getHandshakeStatus();

        }
	}

	void checkConnected() throws IOException
	{
		if (!connected) 
			throw new IOException("TLSServerTransport not connected!");
	}

	@Override
	public boolean connect() throws IOException
	{
		Log.d(TAG, "connect()");

		if (!transport.connect()) 
			return false;

		try {
			context = SSLContext.getInstance("TLS");
		} catch (NoSuchAlgorithmException e) {
			throw new IOException("TLSServerTransport:IOException:" + e.getMessage() );
		}
		KeyManager [] km = { keyManager };
		TrustManager [] tm = { trustManager };
		try {
			context.init(km, tm, null);
		} catch (KeyManagementException e) {
			throw new IOException("TLSServerTransport:KeyManagementException:" + e.getMessage() );
		}
		engine = context.createSSLEngine();
		engine.setUseClientMode(false);
		engine.setNeedClientAuth(false);
		engine.setWantClientAuth(false);

		try {
			@SuppressWarnings("rawtypes")
            Class c = Class.forName("javax.net.ssl.SSLEngine");
			for (Method m : c.getDeclaredMethods() ) {
				if (m.getName() == "getSSLParameters") { //this is not supported on Android < 2.3
					SSLParameters params = engine.getSSLParameters();
					params.setCipherSuites(new String [] { 
						"TLS_RSA_WITH_AES_128_CBC_SHA"
					});
					engine.setSSLParameters(params);
					}
				}
			}
		catch (ClassNotFoundException e) { //shouldnt ever happen
		}

		try {
			do_handshake();
		} 
		catch (SSLException e) {
		    throw new IOException("TLSServerTransport:handshake_failed:" + e.getMessage(), e);
		}
		catch (IOException e) {
		    throw new IOException("TLSServerTransport:handshake_failed:" + e.getMessage(), e);
		}
		connected = true;

		X509Certificate cert = keyManager.getCertificateChain("dummyServer")[0];
		byte[] certBytes = null;
		try {
			certBytes = cert.getEncoded();
		} catch (CertificateEncodingException e) {
			e.printStackTrace();
		}
		String lenStr = String.format("%08X", certBytes.length);
		byte [] lenBytes = lenStr.getBytes();
		write(lenBytes.length,lenBytes);
		write(certBytes.length,certBytes);

		return true;
	}

	@Override
	public void disconnect() throws IOException
	{
		checkConnected();
		
		transport.disconnect();
	}

	@Override
	public boolean hasMore() throws IOException 
	{
		checkConnected();
		Log.d(TAG, "hasMore() bytes:" + decrypted_bytes.length + " chunks:" + decrypted_chunks.size());
		
		return (decrypted_bytes.length > 0) || decrypted_chunks.size() > 0 || transport.hasMore();
	}

	@Override
	public boolean isConnected() throws IOException
	{
		return transport.isConnected();
	}

	@Override
	public int peek() throws IOException
	{
//	    Log.d(TAG, "TLSServerTransport::peek() called");
	    checkConnected();

	    if (decrypted_chunks.isEmpty()) {
	        readChunks();
	    }
	    if (decrypted_chunks.isEmpty())
	    {
	        Log.d(TAG, "TLS Peek - nothing to peek at...");
	        return 0;
	    }
//	    Log.d(TAG, "TLS Peek: " + Integer.toHexString(decrypted_chunks.getFirst()[0] & 0xFF) +
//	            " " + (char)decrypted_chunks.getFirst()[0] +
//	            " " + (int)(decrypted_chunks.getFirst()[0] & 0xFF));
	    return decrypted_chunks.getFirst()[0];
	}

	@Override
	public int read() throws IOException
	{
		checkConnected();
		byte b[] = new byte[]{'0'};
		read(1,b);
//		Log.d(TAG, "returning byte:" + b);
		return b[0];
	}

	@Override
	public int read(int numBytes, byte[] bytes) throws IOException
	{
		checkConnected();

		int num_waits = 0;
		final int MAXWAITS = 10;
		final int WAITMILIS = 50; // how long should we wait?
		
		while (decrypted_chunks.isEmpty())
		{
		    readChunks();

		    if (decrypted_chunks.isEmpty()) {
		        if (++num_waits > MAXWAITS)
		            throw new IOException("timeout waiting for transport input");

		        try {
		            Log.d(TAG, "Waiting for data on transport take:" + num_waits);
		            wait(WAITMILIS);
		        }
		        catch (InterruptedException e) {}
		    }
		}

		byte [] chunk = decrypted_chunks.removeFirst();
//		Log.d(TAG, "chunk being read from is:");
//		Log.hexdump(chunk);

		if (chunk.length == numBytes) {	
		    // copy decrypted bytes into caller's byte array
		    System.arraycopy(chunk, 0, bytes, 0, chunk.length);
		    return chunk.length;
		}
		else if (chunk.length < numBytes) {
		    throw new IOException("Input stream parsing error. underflow?");
		}
		else {
		    // unfortunately, our readers do not request the input in chunks
		    // instead they read 1 byte increments which means we have to pop
		    // the top chunk of input, peel one byte off, and push the remains back onto the list.
		    // sub-optimal but works for now.
		    System.arraycopy(chunk, 0, bytes, 0, numBytes);
		    byte [] chunkless = new byte[chunk.length - numBytes];
		    System.arraycopy(chunk, numBytes, chunkless, 0, chunk.length-numBytes);
		    decrypted_chunks.addFirst(chunkless);
		    return numBytes;
		}
	}

	@Override
	public int read(int numBytes, byte[] bytes, boolean needNumBytes) throws IOException {
		Log.d(TAG, "TLSServerTransport::read(3) called");
		int bytesRead = read(numBytes, bytes);
		if ((bytesRead != numBytes) && (needNumBytes))
			throw new IOException("TLSServerTransport::read() - read less than required number of bytes");
		return bytesRead;
	}

	/**
	 * drain all input from transport stream and decrypt using engine.
	 * 
	 * @param raw_bytes - buffer to accept unwrapped bytes read from socket
	 * @throws IOException - when read buffer is too small, or other socket read exceptions thrown
	 */
	protected void readChunks() throws IOException
	{

	        boolean hadLeftovers = false;

	        // allocate byte buffers to match session parameters exactly
	        if (raw_source == null) {
	            SSLSession s = engine.getSession();
	            MAXBUF = s.getPacketBufferSize();
	            Log.d(TAG, "SSLSession packet size:" + MAXBUF);
	            raw_source = ByteBuffer.allocate(MAXBUF);
	        }

	        int pos = raw_source.position();
	        if (pos > 0) {
	            Log.d(TAG, "UNDERFLOW -contining from position:" + pos);
	            hadLeftovers = true;
	        }

	        transport.peek();
	        while( transport.hasMore() &&
	                raw_source.position() < MAXBUF ) {
	            raw_source.put((byte)transport.read());
	        }

	        if (pos == raw_source.position()) {
	            Log.d(TAG, "Nothing read.");
	            return;
	        }
	        Log.d(TAG, "readChunks() done reading at position:" + raw_source.position());

	        raw_source.flip();

	        ByteBuffer dst = ByteBuffer.allocate(MAXBUF); 
	        boolean unwrapped;
	        do {
	            if (hadLeftovers) {
	                dumpSourceBuffer("Before unwrap()");
	            }

	            SSLEngineResult engineResult = engine.unwrap(raw_source, dst);

	            if (hadLeftovers) {
	                dumpSourceBuffer("After  unwrap()");
	            }

	            SSLEngineResult.Status unwrap_status = engineResult.getStatus();
	            Log.d(TAG, "unwrap status:" + unwrap_status);

	            unwrapped = (unwrap_status == SSLEngineResult.Status.OK);
	            dst.rewind();
	            if (unwrapped)
	            {
	                int l = engineResult.bytesProduced();
	                Log.d(TAG, "decrypted length:" + l);
	                byte [] chunk = new byte[l];
	                dst.get(chunk, 0, l);

	                Log.d(TAG, "\nchunk read is :");
	                Log.hexdump(chunk);
	                if (isLogging)
	                    logFileIn.write(chunk);

	                decrypted_chunks.addLast(chunk);
	            }
	            else if (unwrap_status != Status.BUFFER_UNDERFLOW) {
	                throw new IOException("Unrecoverable TLS error: " + unwrap_status);
	            }
	            else {
	                Log.d(TAG, "UNDERFLOW? reseting raw_source for next time.");
	            }
	            dst.rewind();
	        } while (unwrapped && raw_source.hasRemaining());

	    Log.d(TAG, "readChunks() done, hasRemaining:" + raw_source.hasRemaining());

	    if (raw_source.hasRemaining()) {
	        Log.e(TAG, "UNDERFLOW? bytes left over");
	        raw_source.compact();
	    }
	    else {
	        raw_source.clear();
	    }
	}
	
	private void dumpSourceBuffer(String tag)
	{
	    int pos = raw_source.position();
	    int lim =raw_source.limit();
        Log.e(TAG, "---" + tag + " position:" + pos + " limit:" +  lim);
        
        if (raw_source.hasArray() && pos < lim) {
            byte [] src = raw_source.array();
            byte [] section = new byte[lim - pos];
            System.arraycopy(src, pos, section, 0, lim - pos);
            Log.hexdump(section);
        }
	}

	@Override
	public boolean write(byte b) throws IOException
	{
		Log.e(TAG, "FAIL! write(" + b + ")");
		throw new IOException("Invalid use of TLS transport");
	}

	@Override
	public boolean write(int numBytes, byte[] bytes)
	throws IOException
	{
//	    synchronized (sslEngineLock) {

	        // Log.d(TAG, "write(" + numBytes +"):");
	        // Log.hexdump(bytes);

	        checkConnected();

	        if (isLogging) {
	            logFileOut.write(bytes, 0, numBytes);
	        }

	        int dstBytes = 65536;
	        ByteBuffer src = ByteBuffer.wrap(bytes,0,numBytes);
	        ByteBuffer dst = ByteBuffer.allocate(dstBytes);
	        SSLEngineResult engineResult = engine.wrap(src, dst);
	        byte [] bytes2send = new byte[engineResult.bytesProduced()];
	        dst.rewind();
	        dst.get(bytes2send);
	        transport.write(bytes2send.length , bytes2send);

//	    }
	    
	    return false;
	}

	public class dummyKeyManager extends X509ExtendedKeyManager {
		X509Certificate cert;
		PrivateKey privKey;

		public dummyKeyManager(X509Certificate _cert,PrivateKey _privKey) {
			privKey = _privKey;
			cert = _cert;
		}

		@Override
		public String chooseClientAlias(String[] arg0, Principal[] arg1, Socket arg2) {
			Log.d(TAG, "chooseClientAlias");
			return null;
		}

		public String chooseEngineServerAlias(String keyType, Principal[] issuers, SSLEngine engine) {
			Log.d(TAG, "chooseEngineServerAlias " + keyType);
			if (keyType == "RSA") {
				return "dummyServer";
			}
			return null;
		}

		@Override
		public String chooseServerAlias(String keyType, Principal[] issuers, Socket arg2) {
			Log.d(TAG, "chooseServerAlias");
			return null;
		}

		@Override
		public X509Certificate[] getCertificateChain(String arg0) {
			Log.d(TAG, "getCertificateChain");
			return new X509Certificate[] { cert };
		}

		@Override
		public String[] getClientAliases(String arg0, Principal[] arg1) {
			Log.d(TAG, "getClientAliases");
			return null;
		}

		@Override
		public PrivateKey getPrivateKey(String arg0) {
			Log.d(TAG, "getPrivateKey");
			return privKey;
		}

		@Override
		public String[] getServerAliases(String arg0, Principal[] arg1) {
			Log.d(TAG, "getServerAliases");
			return null;
		}
	}

	/* 
	 * This class should never be called
	 * */
	public class dummyTrustManager implements X509TrustManager {
		@Override
		public void checkClientTrusted(X509Certificate[] arg0, String arg1)
		throws CertificateException {
			throw new CertificateException("dummyTrustManager:checkClientTrusted called");
		}

		@Override
		public void checkServerTrusted(X509Certificate[] arg0, String arg1)
		throws CertificateException {
			throw new CertificateException("dummyTrustManagercheckServerTrusted called");
		}

		@Override
		public X509Certificate[] getAcceptedIssuers() {
			Log.d(TAG, "dummyTrustManager:getAcceptedIssuers");
			return null;
		}
	}


}
