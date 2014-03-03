#include <stdio.h>
#include "sidg_multirtpif_server.hpp"
#include "SidMultiRTPInterface.hpp"
#include "SidProtocolEnums.hpp"
#include "sidg_multirtpif_cb_client.hpp"
#include "SidDebugging.hpp"

struct OptionsParser {
	OptionsParser() :
		m_IpcPrefix(IPC_PREFIX_DEFAULT),
		m_Help(false) {
	}
	void Usage(const char *executable) {
		SID_INFO("usage:");
		SID_INFO("	%s [options]", executable);
		SID_INFO("	options:");
		SID_INFO("		-h - print help");
		SID_INFO("		-s prefix - path prefix for ipc key. default /tmp/");
	}
	int ParseOptions(int argc, const char **argv) {
		for (int i = 1; i < argc; i++) {
			const char c = argv[i][0];
			if (c == '-' || c == '/') {
				switch (argv[i][1]) {
				case 'h': {
					m_Help = true;
					break;
				}
				case 's': {
					if (i + 1 >= argc)
						return -1;
					m_IpcPrefix = argv[++i];
					break;
				}
				default: {
					return -1;
					break;
				}
				}
			} else {
				return -1;
			}
		}

		return 0;
	}
	const char *m_IpcPrefix;
	bool m_Help;
};


int main(int argc, const char **argv) {
	OptionsParser parser;
	if (parser.ParseOptions(argc, argv) < 0 || parser.m_Help) {
		parser.Usage(argv[0]);
		return -1;
	}

	Sid::SkypeMultiRTPInterfaceServer *multirtpif_server = new Sid::SkypeMultiRTPInterfaceServer();
	Sid::SkypeMultiRTPCallbackInterfaceClient *multirtpif_cb_client = new Sid::SkypeMultiRTPCallbackInterfaceClient();

	SkypeMultiRTPInterface* multirtpif = SkypeMultiRTPInterfaceGet(multirtpif_cb_client);
	multirtpif_server->set_if(multirtpif);

	Sid::String fromskypekitkey;
	Sid::String toskypekitkey;

	fromskypekitkey.Format( "%smultirtp_from_skypekit_key", parser.m_IpcPrefix);
	toskypekitkey.Format( "%smultirtp_to_skypekit_key", parser.m_IpcPrefix);

	multirtpif_server->Connect((const char* ) fromskypekitkey, 0);
	multirtpif_cb_client->Connect((const char* ) toskypekitkey, 500);

	Sid::Protocol::Status status;
	do {
		status =multirtpif_server->ProcessCommands();
	} while (status == Sid::Protocol::OK);

	SkypeMultiRTPInterfaceRelease(multirtpif);
	multirtpif_server->Disconnect();
	multirtpif_cb_client->Disconnect();

	delete multirtpif_server;
	delete multirtpif_cb_client;

	printf("MultiRTPServerTransport disconnected, exiting from multirtptesthost\n");
}
