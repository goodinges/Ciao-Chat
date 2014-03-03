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

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.concurrent.TimeoutException;

import android.net.LocalSocket;
import android.net.LocalSocketAddress;
import android.net.LocalSocketAddress.Namespace;
import com.skype.util.Log;

public class LocalSocketTransport implements Transport {

	// INTERFACE IMPLEMENTATION
	private static final String TAG = "LocalSocketTransport";

	private LocalSocket mSocket = null;
	private boolean mConnected = false;
	private InputStream mInStream = null;
	private OutputStream mOutStream = null;
	private static final String CSOCKET_NAME = "SkypeKit";
	private static final Namespace CSOCKET_NAMESPACE = Namespace.ABSTRACT;

	public LocalSocketTransport()
	{
		super();
		buffer=new byte[BUFFERSIZE];
		bufferidx=0;
		bufferend=0;
	}
	
	@Override
	public boolean connect() throws IOException {
		if (!mConnected) {
			LocalSocketAddress sockAddr = new LocalSocketAddress(CSOCKET_NAME,CSOCKET_NAMESPACE);
			mSocket = new LocalSocket();
			try {
				mSocket.connect(sockAddr);
				// Get the input/output streams
				mInStream = mSocket.getInputStream();
				mOutStream = mSocket.getOutputStream();
				// We successfully connected our socket
				mConnected = true;
			} catch (IOException e) {
				Log.e(getClass().getName() + "#connect", e.getMessage());
			}
		}
		return mConnected;
	}

	@Override
	public void disconnect() throws IOException {
		if (mConnected) {
			try {
				mInStream.close();
				mInStream = null;
				mOutStream.close();
				mOutStream = null;
				mSocket.close();
			} catch (IOException e) {
				Log.e(getClass().getName() + "#disconnect", e.getMessage());
			}
			mConnected = false;
		}
	}

	@Override
	public boolean hasMore() throws IOException {
		return bufferidx != bufferend;
	}

	@Override
	public boolean isConnected() throws IOException {
		// If we think we are connect, do a real check
		if (mConnected) {
			return mSocket.isConnected();
		}
		return mConnected;
	}

	@Override
	public void startLogging(String logFileName)
	{}

	@Override
	public int peek() throws IOException {
		try {
			synchronized(mInStream)
			{
				return(ReadByte(true));
			}
		} catch (InterruptedException e) {
			throw new IOException();
		} catch (TimeoutException e) {
			throw new IOException();
		}
	}

	@Override
	public int read() throws IOException {
		try {
			synchronized(mInStream)
			{
				return(ReadByte(false));
			}
		} catch (InterruptedException e) {
			throw new IOException();
		} catch (TimeoutException e) {
			throw new IOException();
		}
	}

	public int read(int numBytes, byte[] bytes) throws IOException
	{
		int bytescount=0;
		peek(); //fill buffer
		synchronized(mInStream)
		{
			while((bufferidx<bufferend) && (bytescount<=numBytes))
			{
				bytes[bytescount]=buffer[bufferidx];
				bufferidx++;
				bytescount++;
			}
		}
		return bytescount;
	}

	public int read(int numBytes, byte[] bytes, boolean requirenumbytes) throws IOException
	{
		if(!requirenumbytes)
		{
			return read(numBytes,bytes);
		}
		for(int n=0;n<numBytes;n++)
		{
			try {
				synchronized(mInStream)
				{
					bytes[n]=ReadByte(false);
				}
			} catch (InterruptedException e) {
				throw new IOException();
			} catch (TimeoutException e) {
				throw new IOException();
			}
		}
		return numBytes;
	}

	@Override
	public boolean write(byte b) throws IOException {
		if (mConnected) {
			try {
				synchronized(mOutStream)
				{
					mOutStream.write(b);
					Log.d(TAG,"Write: " + Integer.toHexString(b & 0xFF) + " " + (char)b + " " + (int)(b & 0xFF));
				}
			}
			catch (IOException e) {
				Log.e(e.getClass().getName(), e.getMessage());
				throw e;
			}
		}
		return true;
	}

	@Override
	public boolean write(int numBytes, byte[] bytes) throws IOException {
		if (mConnected) {
			try {
				synchronized(mOutStream)
				{
					mOutStream.write(bytes, 0, numBytes);
					for(int i=0;i<numBytes;i++)
					{
						Log.d(TAG,"Write: " + Integer.toHexString(bytes[i] & 0xFF) + " " + (char)bytes[i] + " " + (int)(bytes[i] & 0xFF));
					}
				}
			}
			catch (IOException e) {
				Log.e(e.getClass().getName(), e.getMessage());
				throw e;
			}
		}
		return true;
/*
		if (mConnected) {
			try {
				int msgLen = message.length();
				// We assume an integer is sized 4 bytes
				byte[] size = new byte[4];
				// We put the message size to the correct values
				size[3] = (byte) (msgLen & 0xFF);
				size[2] = (byte) ((msgLen >> 8) & 0xFF);
				size[1] = (byte) ((msgLen >> 16) & 0xFF);
				size[0] = (byte) ((msgLen >> 24) & 0xFF);
				// First we write the size of the message
				mOutStream.write(size);
				// Then we write the message
				mOutStream.write(message.getBytes());
				// Send it right away
				mOutStream.flush();
			} catch (IOException e) {
				Log.e(e.getClass().getName(), e.getMessage());
			}
		}
		*/
	}

	/*
	private boolean hasMore(int wait) {
		boolean dataAvailable = false;
		if (mConnected) {
			try {
				long current = System.currentTimeMillis() + wait;
				do {
					if (mInStream.available() > 0) {
						dataAvailable = true;
					}
				} while (current >= System.currentTimeMillis()
						&& !dataAvailable);
			} catch (IOException e) {
				Log.e(e.getClass().getName(), e.getMessage());
			}
		}
		return dataAvailable;
	}

	/**
	 * Private helper function for waiting for data in the input stream. Done to
	 * prevent pull() from blocking
	 * 
	 * @param wait
	 *            Time to wait for
	 * @throws IOException
	 */
/*	private void waitForAvailable(int wait) throws IOException {
		int timeout = wait;
		long until = System.currentTimeMillis() + timeout;
		while (mInStream.available() == 0) {
			if (System.currentTimeMillis() > until) {
				throw new IOException("input timed out");
			}
			try {
				Thread.sleep(wait / 2);
			} catch (InterruptedException ignore) {
			}
		}
	}
*/
	
	public static final int BUFFERSIZE=1024*64; //bytes
	public static final int STREAMTIMEOUT=1000*60*10; //ms
	
	private static byte[] buffer;
	private int bufferidx;
	private int bufferend;
	
	private byte ReadByte(boolean peek) throws IOException, InterruptedException, TimeoutException
	{
		//Log.d(TAG,"ReadByte entered");
		while(bufferidx==bufferend)
		{
			Log.d(TAG,"Transport buffer reading more...");
			bufferidx=0;
			bufferend=mInStream.read(buffer); // blocks
			if(bufferend==-1)
			{
				Log.e(TAG,"Socket transport is returning -1.");
				throw new IOException("Socket transport stream EOF");
			}
			else
			{
				Log.d(TAG,"Transport buffer read some more...");
			}
		}		
		byte retbyte=buffer[bufferidx];
		if(peek)
		{
			Log.d(TAG,"Peek: " + Integer.toHexString(retbyte & 0xFF) + " " + (char)retbyte + " " + (int)(retbyte & 0xFF));
		}
		else
		{
			Log.d(TAG,"Read: " + Integer.toHexString(retbyte & 0xFF) + " " + (char)retbyte + " " + (int)(retbyte & 0xFF));
			bufferidx++;
		}
		return retbyte;
	}

}
