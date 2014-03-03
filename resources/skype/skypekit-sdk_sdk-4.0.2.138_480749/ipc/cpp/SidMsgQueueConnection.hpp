#ifndef SID_MSG_QUEUE_CONNECTION_HPP
#define SID_MSG_QUEUE_CONNECTION_HPP

#ifdef _SYMBIAN

#include <assert.h>
#include <e32base.h>
#include <e32msgqueue.h>

#include "SidConnection.hpp"

namespace Sid
{
	enum TQueueConnectState {EQDisconnected=0, EQConnected=1};

	enum TMyPropertyKeys {ESkypeKitMessageQStateInt, ESkypeKitConnectStateInt};

	const TInt KMaxQueueSlotSize = 224;

	typedef struct SEMsgBuf {
		int len;
		char buf[KMaxQueueSlotSize];
	} message_buf;

	class PeriodicCallbackThread : public Thread
	{
	public:
		typedef bool (*CallbackFn)(void *pData); // return true, if thread should continue calling

		PeriodicCallbackThread(CallbackFn fn, void *pData, uint iInterval); // iInterval is in ms

		inline void SetInterval(uint iInterval) { m_iInterval = iInterval; }
		inline uint GetInterval() const { return m_iInterval; }

		void StopThread();
		virtual void Run();

	private:
		bool m_bShutdown;
		CallbackFn m_pFunc;
		void *m_pData;
		uint m_iInterval;
	};

	class MsgQueueConnection : public Sid::Connection
	{
	public:
		MsgQueueConnection(const char *log_path = 0);
		~MsgQueueConnection();

		virtual Connection::Status bl_read_bytes(Sid::CommandInitiator *current_command_processor, uint num_bytes, char *dest);

	protected:
		// Calls OnDisconnected if connection disconnected
		// returns true, if connection is OK
		static bool CheckConnection(void *pThis);

		typedef RMsgQueue<message_buf> TMessageQueue;

		void Initialize(TMessageQueue *pInQueue, TMessageQueue *pOutQueue);

		inline TMessageQueue &GetInQueue() { assert(m_pInQueue != NULL); return *m_pInQueue; }
		inline TMessageQueue &GetOutQueue() { assert(m_pOutQueue != NULL); return *m_pOutQueue; }

		void OnDisconnected(); // Called by CheckConnection
		virtual int recv(uint num_bytes, char *dest, uint non_blocking_read_ahead);
		virtual int send(uint num_bytes, const char *src, bool immediate_send_hint);
		virtual void terminate();

	private:
		enum EReceiveStatus
		{
			RS_OK,
			RS_TIME_OUT,
			RS_ERROR
		};

		EReceiveStatus Receive();
		bool ReceiveMessage();

		// Derived class should initialize these variables using Initialize
		TMessageQueue	*m_pInQueue;
		TMessageQueue	*m_pOutQueue;

		PeriodicCallbackThread *m_pPeriodicCallbackThread;

		static const time_t s_iTimeOut = 10; // In seconds

		struct SConnectionCheck
		{
			SConnectionCheck()
				:m_iNumProcesses(0)
				,m_bConnected(false)
				,m_iTime(0)
			{
			}

			TInt m_iNumProcesses;
			bool m_bConnected;
			time_t m_iTime;
		};

		SConnectionCheck m_sConnectionCheck;

		TInt m_iReadIndex;

		static const TInt KMaxMsgSize = 28672;

		struct msg_blob
		{
			int len;
			char msg[KMaxMsgSize];
		};

		msg_blob m_cBuffer;
	};

} // namespace Sid

#endif // _SYMBIAN

#endif // SID_MSG_QUEUE_CONNECTION_HPP
