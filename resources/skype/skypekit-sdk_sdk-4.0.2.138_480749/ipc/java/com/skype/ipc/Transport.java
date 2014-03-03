package com.skype.ipc;

import java.io.IOException;

public interface Transport
{
    /**
     * brief Connects to skypekit. When this function returns successfully, you
     * can start sending and receiving messages.
     */
    public boolean connect() throws IOException;

    /**
     * Disconnects from skypekit
     **/
    public void disconnect() throws IOException;

    public boolean hasMore() throws IOException;

    /** \brief Read one byte. */
    public int read() throws IOException;

    /** \brief Check value of next byte, but do not remove from stream. */
    public int peek() throws IOException;

    /** \brief Read as many bytes as possible, but not more than num_bytes. */
    public int read(int num_bytes, byte[] bytes) throws IOException;

    /**
     * \brief Read number of bytes indicated. Block until all bytes collected.
     * \returns number bytes read
     */
    public int read(int num_bytes, byte[] bytes, boolean require_num_bytes) throws IOException;

    /** \brief Write one byte. */
    public boolean write(byte b) throws IOException;

    /** \brief Write buffer to stream. */
    public boolean write(int num_bytes, byte[] bytes) throws IOException;

    /** \brief Check if the connection to SkypeKit is still there. */
    public boolean isConnected() throws IOException;

    /** \brief start transport logging to two files based on logFileName path. */
    public void startLogging(String logFileName);
}
