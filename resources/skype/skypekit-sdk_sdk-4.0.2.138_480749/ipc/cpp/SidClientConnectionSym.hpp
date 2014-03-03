#ifndef SID_CLIENT_CONNECTION_SYMBIAN_HPP
#define SID_CLIENT_CONNECTION_SYMBIAN_HPP

#ifdef _SYMBIAN

#include <e32property.h>

#include "SidConnection.hpp"
#include "SidMsgQueueConnection.hpp"

namespace Sid
{

	class TcpClientConnection : public Connection
	{
	public:
		TcpClientConnection(const char* host, int port, 
			const char* log_path = 0, int retries = 0, int retry_delay = 1, ConnectionStateListener* listener = 0);
                ~TcpClientConnection();
                virtual int recv(uint num_bytes, char* dest, uint non_blocking_ahead);
                virtual int send(uint num_bytes, const char* src, bool immediate_send_hint);
                virtual void terminate();
                void set_non_blocking();
                void wait_input();
                SKYPEKIT_DISABLE_COPY(TcpClientConnection);
	};

	class LocalClientConnection : public MsgQueueConnection
	{
	public:
		LocalClientConnection(TSecureId id, const char *log_path = 0);

		// Parameters as passed in ClientSession
		LocalClientConnection(const char *local, const char *streamlog, int retries, int retry_delay, ConnectionStateListener *pConnStateListener);
		~LocalClientConnection();

	protected:
		virtual void terminate();
		void set_non_blocking() { }
		void wait_input() { }

	private:
		static const TInt KQueueTimeout = 15000000;

		bool	Connect();
		void	Disconnect();

		TInt	CheckMessageQueues() const;

		TUid	m_iSkypeKit;

		RProperty m_rConnectState;

		TMessageQueue m_rRcvQ;
		TMessageQueue m_rSndQ;
	};
} // namespace Sid

#endif // _SYMBIAN

#endif // SID_CLIENT_CONNECTION_SYMBIAN_HPP
