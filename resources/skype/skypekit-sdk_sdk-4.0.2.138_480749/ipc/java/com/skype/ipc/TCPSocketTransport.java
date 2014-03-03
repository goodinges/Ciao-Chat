package com.skype.ipc;

import java.io.EOFException;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

import com.skype.util.Log;

public class TCPSocketTransport implements Transport {

	// INTERFACE IMPLEMENTATION
	private static final String TAG = "TCPSocketTransport";

	public TCPSocketTransport(String IpAddress, int Port) {
		super();
		this.mIpAddress = IpAddress;
		this.mPort = Port;
		buffer=new byte[BUFFERSIZE];
		bufferidx=0;
		bufferend=0;
		try {
			mInetAddress = InetAddress.getByName(mIpAddress);
			mSocketAddress = new InetSocketAddress(mInetAddress, mPort);
		} catch (java.net.UnknownHostException e) {
			Log.d(TAG,"UnknownHost: "+ e.getMessage());
		}
	}

	private String mIpAddress = null;
	private int mPort = 0;
	private boolean mConnected = false;
	private Socket mSocket;
	private InetAddress mInetAddress;
	private SocketAddress mSocketAddress;
	private static int mSocketTimeout = 1 * 1000;  // x * 1 sec(in millis)
	private boolean isLogging = false;
    private FileOutputStream logFileOut = null;
    private FileOutputStream logFileIn = null;

	public boolean connect() throws IOException {
		Log.d(TAG, "TCPSocketTransport::connect() called");
		if (!mConnected) {
			try {
                mSocket = new Socket(); // create unconnected socket
				mSocket.connect(mSocketAddress, mSocketTimeout);

				Log.d(TAG,"Socket created:"+mSocket);
				// Get the input/output streams
				ins = mSocket.getInputStream();
				outs = mSocket.getOutputStream();

				// We successfully connected our socket
				mConnected = true;
			} catch (java.net.UnknownHostException e) {
				Log.e(TAG,"UnknownHost: "+ e.getMessage());
			} catch (java.io.IOException e) {
				Log.e(TAG,"IO: "+e.getMessage());
			}
		}
		return mConnected;	
	}

	public void disconnect() throws IOException {
		if (mConnected) {
			try {
				ins.close();
				ins = null;
				outs.close();
				outs = null;
				mSocket.close();
			} catch (IOException e) {
				Log.e(e.getClass().getName(), e.getMessage());
			}
			mConnected = false;
		}	
	}

	public boolean isConnected() throws IOException {
		return mConnected;
    }

	@Override
    public void startLogging(String logFileName)
    {
        assert( ! isLogging);
        
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

    public int peek() throws IOException
    {
        // Log.d(TAG, "TCPSocketTransport::peek() called");
        synchronized (ins) {
            return (ReadByte(true));
        }
    }

	public int read() throws IOException
	{
	    synchronized(ins)
	    {
	        return(ReadByte(false));
	    }
	}

	public int read(int numBytes, byte[] bytes) throws IOException
	{
	    synchronized(ins)
	    {
	        for(int n=0;n<numBytes;n++)
	        {
	            bytes[n]=ReadByte(false);
	        }
	    }
		return numBytes;
		/*
		if (mConnected) {
			try {
				instream.read(bytes, 0, numBytes);
			}
			catch (IOException e) {
				Log.e(e.getClass().getName(), e.getMessage());
			}
		}
		return true;
		 */
	}

	public int read(int numBytes, byte[] bytes, boolean requirenumbytes) throws IOException
	{
		if(!requirenumbytes)
		{
			return read(numBytes,bytes);
		}
		for(int n=0;n<numBytes;n++)
		{
		    synchronized(ins)
		    {
		        bytes[n]=ReadByte(false);
		    }
		}
		//Log.d(TAG, "TCPSocketTransport - bytes read from chunk are:");
		//dumpBytes(bytes,numBytes);
		return numBytes;
	}

	public boolean write(byte b) throws IOException {
		if (mConnected) {
			try {
				synchronized(outs)
				{
					outs.write(b);
					//Log.d(TAG,"Write: " + Integer.toHexString(b & 0xFF) + " " + (char)b + " " + (int)(b & 0xFF));
					if (isLogging) {
					    logFileOut.write(b);
					}
				}
			}
			catch (EOFException e) {
			    Log.e(TAG, e.getMessage());
			    throw e;
			}
			catch (IOException e) {
				Log.e(TAG, e.getMessage());
				throw e;
			}
		}
		return true;
	}

	public boolean write(int numBytes, byte[] bytes) throws IOException
	{
		if (mConnected) {
			try {
				synchronized(outs)
				{
					outs.write(bytes, 0, numBytes);
//					for(int i=0;i<numBytes;i++)
//					{
//						Log.d(TAG,"Write: " + Integer.toHexString(bytes[i] & 0xFF) + " " + (char)bytes[i] + " " + (int)(bytes[i] & 0xFF));
//					}
					if (isLogging) {
					    logFileOut.write(bytes,0, numBytes);
					}
				}
			}
			catch (IOException e) {
				Log.e(e.getClass().getName(), e.getMessage());
				throw e;
			}
		}
		return true;
	}

	public InputStream GetInputStream()
	{
		return ins;
	}

	public OutputStream GetOutputStream()
	{
		return outs;
	}

	//// CLASS INTERNALS

	public static final int BUFFERSIZE=1024*64; //bytes
	public static final int STREAMTIMEOUT=1000*60*10; //ms

	private InputStream ins;
	private OutputStream outs;
	private byte[] buffer;
	private int bufferidx;
	private int bufferend;

	private byte ReadByte(boolean peek) throws IOException 
	{
		//Log.d(TAG,"ReadByte entered");
		while(bufferidx==bufferend)
		{
			//Log.d(TAG, "Transport buffer reading more...");
			bufferidx=0;
			bufferend=ins.read(buffer); // blocks
			
			if(bufferend==-1)
			{
				Log.e(TAG,"Socket transport is returning -1.");
				mConnected = false;
				throw new EOFException("Socket transport stream EOF");
			}
			else
			{
				//Log.d(TAG, "Transport buffer read some more...");
			}
		}		
		byte retbyte=buffer[bufferidx];
		if(peek)
		{
//			Log.d(TAG,"Peek: " + Integer.toHexString(retbyte & 0xFF) + " " + (char)retbyte + " " + (int)(retbyte & 0xFF));
		}
		else
		{
//			Log.d(TAG,"Read: " + Integer.toHexString(retbyte & 0xFF) + " " + (char)retbyte + " " + (int)(retbyte & 0xFF));
            if (isLogging) {
                logFileIn.write(retbyte);
            }
            
			bufferidx++;
		}
		return retbyte;
	}

	@Override
	public boolean hasMore() throws IOException {
		return !(bufferidx==bufferend);
	}
}
