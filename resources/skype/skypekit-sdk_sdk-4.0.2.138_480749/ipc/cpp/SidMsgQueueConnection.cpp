#ifdef _SYMBIAN

#include "SidMsgQueueConnection.hpp"
#include "SidTransportLog.hpp"

#ifndef min
template<typename A, typename B> inline A min(const A &a, const B &b) { return (a<b)?a:b; }
#endif

namespace Sid
{
	PeriodicCallbackThread::PeriodicCallbackThread(CallbackFn fn, void *pData, uint iInterval)
		:m_bShutdown(false)
		,m_pFunc(fn)
		,m_pData(pData)
		,m_iInterval(iInterval)
	{
	}

	void PeriodicCallbackThread::StopThread()
	{
		m_bShutdown = true;
		Stop();
	}

	void PeriodicCallbackThread::Run()
	{
		while (!m_bShutdown)
		{
			sleep(m_iInterval);
			if (!m_pFunc(m_pData)) break;
		}
	}

	MsgQueueConnection::MsgQueueConnection(const char *log_path)
		:Connection(log_path)
		,m_pInQueue(NULL)
		,m_pOutQueue(NULL)
		,m_pPeriodicCallbackThread(NULL)
		,m_iReadIndex(0)
	{
        m_socket = -1;
		m_cBuffer.len = 0;
	}

	MsgQueueConnection::~MsgQueueConnection()
	{
		terminate();

		if (m_pPeriodicCallbackThread)
		{
			m_pPeriodicCallbackThread->StopThread();
			delete m_pPeriodicCallbackThread;
		}
	}

	Connection::Status MsgQueueConnection::bl_read_bytes(Sid::CommandInitiator *current_command_processor, uint num_bytes, char *dest)
	{
		if (num_bytes == 0)
			return OK;

		int read = -1;

		if (dest != 0)
		{
			while ((read = recv(num_bytes, dest, 0)) == 0) {}

			if (m_log) m_log->read(current_command_processor,read,dest);
		}
		else
		{
			char chunk[256];
			while (num_bytes > 0)
			{
				while ((read = recv((num_bytes) > 256 ? 256 : num_bytes, chunk, 0)) == 0) {}
				if (read < 0) break;

				if (m_log) m_log->read(current_command_processor,read,chunk);

				num_bytes -= read;
			}
			num_bytes = read; // So that the return value would be OK if everything's fine
		}

		if (read < 0)
		{
			terminate();
			return ERR_READ;
		}

		return (read == num_bytes) ? OK : ERR_READ;
	}

	bool MsgQueueConnection::CheckConnection(void *pData)
	{
		MsgQueueConnection *pThis = static_cast<MsgQueueConnection*>(pData);

		if (!pThis->is_connected())
			return false;

		if (pThis->m_pInQueue != NULL && pThis->m_pOutQueue != NULL)
		{
			THandleInfo infoIn;
			THandleInfo infoOut;

			pThis->m_pInQueue->HandleInfo(&infoIn);
			pThis->m_pOutQueue->HandleInfo(&infoOut);

			TInt n = infoIn.iNumProcesses;

			if (pThis->m_sConnectionCheck.m_iNumProcesses == 0 && n < 2) // Initialize the count
			{
				pThis->m_sConnectionCheck.m_iNumProcesses = n;
				pThis->m_sConnectionCheck.m_iTime = time(0);
			}
			else
			{
				if (n == 2 && pThis->m_sConnectionCheck.m_iNumProcesses < 2) // Connection established
				{
					pThis->m_sConnectionCheck.m_iNumProcesses = n;
					pThis->m_sConnectionCheck.m_bConnected = true;
					pThis->m_pPeriodicCallbackThread->SetInterval(5000); // Once it's known that connection is established, we can make this check less often
				}
				else if (pThis->m_sConnectionCheck.m_iNumProcesses > n || n > 2) // Disconnected
				{
					pThis->OnDisconnected();
					pThis->m_sConnectionCheck.m_bConnected = false;
					return false;
				}
			}
		}
		
		// Check if the connection timed out
        /*if (pThis->m_sConnectionCheck.m_bConnected == false)
		{
			time_t currTime = time(0);
			if (currTime - pThis->m_sConnectionCheck.m_iTime >= pThis->s_iTimeOut)
			{
				pThis->OnDisconnected();
				return false;
			}
        }*/

		return true;
	}

	void MsgQueueConnection::Initialize(TMessageQueue *pInQueue, TMessageQueue *pOutQueue)
	{
		m_pInQueue = pInQueue;
		m_pOutQueue = pOutQueue;

        m_pPeriodicCallbackThread = new PeriodicCallbackThread(&CheckConnection, this, 100);
        m_pPeriodicCallbackThread->Start();
	}

	MsgQueueConnection::EReceiveStatus MsgQueueConnection::Receive()
	{
		TRequestStatus dataStatus;
		GetInQueue().NotifyDataAvailable(dataStatus);

		TRequestStatus timerStatus;
		RTimer timer;
		timer.CreateLocal();
		timer.After(timerStatus, 1000000);

		User::WaitForRequest(dataStatus, timerStatus);

		if (dataStatus == KRequestPending)
		{
			GetInQueue().CancelDataAvailable();
            User::WaitForRequest(dataStatus);
			timer.Close();
			return RS_TIME_OUT;
		}
		else
		{
			timer.Cancel();
            User::WaitForRequest(timerStatus);
            timer.Close();
		}

		if (KErrNone == GetInQueue().Receive(*(reinterpret_cast<message_buf*>(&m_cBuffer))))
		{
			return RS_OK;
		}
		else return RS_ERROR;
	}

	bool MsgQueueConnection::ReceiveMessage()
	{
		//GetInQueue().ReceiveBlocking(*(reinterpret_cast<message_buf*>(&m_cBuffer)));
		EReceiveStatus status;
		while ((status = Receive()) == RS_TIME_OUT)
		{
			if (!is_connected()) return false; // The MsgQueueNotifier may have called OnDisconnected
		}

		if (status == RS_ERROR) return false;

		//STTester_PROCESS_BUFFER();

		if (m_cBuffer.len==KMaxQueueSlotSize)  ///check for multipart msg
		{
			message_buf tmpBuf;
			bool done = false;
			while (!done)		
			{				
				//Call non-blocking version of Receive(), returns KErrUnderFlow if no msg in queue
				if (KErrNone==(GetInQueue().Receive(tmpBuf))) 
				{
					int nBytes = min(tmpBuf.len, KMaxQueueSlotSize);

					Mem::Copy(m_cBuffer.msg+m_cBuffer.len, tmpBuf.buf, nBytes);
					m_cBuffer.len += nBytes;

					if (nBytes<KMaxQueueSlotSize)
						done = true;
				}
				else break;
			} 
		}

		return true;
	}

	void MsgQueueConnection::OnDisconnected()
	{
		terminate();
	}

	/*static inline void LOG(const char *message)
	{
		(void)message;
	}*/
#define LOG(message) do {} while (0)

	int MsgQueueConnection::recv(uint num_bytes, char *dest, uint /*non_blocking_read_ahead*/)
	{
		if (!is_connected())
		{
			LOG("SKYPE ERROR - LocalServerConnection::recv(), not connected");
			return -1;
		}

		uint nBytesRead = 0;

		while (nBytesRead < num_bytes)
		{
			if (m_iReadIndex >= m_cBuffer.len)
			{
				if (ReceiveMessage() == false) return -1;
				m_iReadIndex = 0;
			}

			uint n = min((uint)m_cBuffer.len - m_iReadIndex, num_bytes-nBytesRead);
			Mem::Copy(dest, m_cBuffer.msg+m_iReadIndex, n);

			m_iReadIndex += n;
			nBytesRead += n;
		}

		return nBytesRead;
	}

	int MsgQueueConnection::send(uint num_bytes, const char *src, bool immediate_send_hint)
	{
		if (!is_connected())
		{
			LOG("SKYPE ERROR - LocalServerConnection::send(), not connected");
			return -1;
		}

		// message is copied because STTester modifies the buffer and needs a non-const
		//SEString msg = message;
		//STTester_ON_OUTGOING_EVENT(msg);

		message_buf msg_buf;

		//loop through msg 
		//
		int tot = num_bytes;
		int offset = 0;

		do
		{
			//send msg to queue in chunks
			msg_buf.len = min(tot, KMaxQueueSlotSize);
			Mem::Copy(msg_buf.buf, (const char*)src+offset, msg_buf.len);
			TInt err = GetOutQueue().Send( msg_buf );
			LOG("TRANSPORT: MsgQueueConnection::send - Buffer sent, err: " + SEString::from(err));

			// If we hit exactly the maximum slot size, skypekit expects more data and will "hang".
			// To prevent this we send a terminating message with length 0. (AAAXL-115).
			if ( tot == KMaxQueueSlotSize )
			{
				message_buf emptyBuf;
				emptyBuf.len = 0;
				TInt err = GetOutQueue().Send( emptyBuf );
				LOG("TRANSPORT: MsgQueueConnection::send - Empty buffer sent, err: " + SEString::from(err));
			}

			tot -= msg_buf.len;
			offset += msg_buf.len;
		}
		while (tot>0);

#if 0
		if (debug)
			LOG("TRANSPORT: Wrote message \"" + msg + "\"");
#endif

		return offset;
	}

	void MsgQueueConnection::terminate()
	{
		m_socket = -1;
	}

} // namespace Sid

#endif // _SYMBIAN
