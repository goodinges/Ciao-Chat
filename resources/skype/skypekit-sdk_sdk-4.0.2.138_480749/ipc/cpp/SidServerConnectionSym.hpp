#ifndef SID_SidServerConnectionSym_HPP_INCLUDED
#define SID_SidServerConnectionSym_HPP_INCLUDED

#include <e32property.h>

#include "SidListener.hpp"
#include "SidConnection.hpp"
#include "SidMsgQueueConnection.hpp"

namespace Sid {

	class TcpListener : public Listener {
	public:
		TcpListener(int port, int max_parallel_connection, int exit_after_connection, int local_only);
		~TcpListener();
		void process_command(CommandInitiator*);
		bool m_local_only;
	};

	class LocalListener : public Listener {
	public:
		LocalListener(const char* local, int max_parallel_connection, int exit_after_connection);
		~LocalListener();
		void process_command(CommandInitiator*);
		char* m_local_name;

		// Caller takes ownership
		void GiveAwayQueues(RMsgQueue<message_buf> *&pInQueue, RMsgQueue<message_buf> *&pOutQueue);

	private:
		void InitL();
		void WaitOnMsgQueueConnectL();

		RProperty m_cConnectState;
		RProperty m_cQueueState;

		// The connected connection will take ownership of the queues
		RMsgQueue<message_buf>	*m_pInQueue;
		RMsgQueue<message_buf>	*m_pOutQueue;

		void SetUpQueuePropL();
		void SetUpMessageQueuesL();
	};  

	class TcpServerConnection : public Connection, public ServerCommandInterface {
	public:
		TcpServerConnection(int socket, const char* log);
                ~TcpServerConnection();
                virtual int recv(uint num_bytes, char* dest, uint non_blocking_ahead);
                virtual int send(uint num_bytes, const char* src, bool immediate_send_hint);
                virtual void terminate();
		int get_connid() const;
	};

	class LocalServerConnection : public MsgQueueConnection, public ServerCommandInterface
	{
	public:
		LocalServerConnection(int id, const char *log);
		~LocalServerConnection();
		int get_connid() const;
	};

} // namespace Sid

#endif
