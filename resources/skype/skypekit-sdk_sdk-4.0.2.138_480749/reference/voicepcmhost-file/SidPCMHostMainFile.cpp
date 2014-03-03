#define SID_LOG_MASK 0xff

#include <stdio.h>
#include "sidg_pcmif_server.hpp"
#include "SidPCMInterface.hpp"
#include "SidProtocolEnums.hpp"
#include "sidg_pcmif_cb_client.hpp"
#include "SidDebugging.hpp"

struct OptionsParser {
	OptionsParser() :
		m_IpcPrefix(IPC_PREFIX_DEFAULT),
		m_InFile(0),
		m_OutFile(0),
		m_Loop(0),
		m_Help(false) {
	}
	void Usage(const char *executable) {
		SID_INFO("usage:");
		SID_INFO("	%s [options]", executable);
		SID_INFO("	options:");
		SID_INFO("		-h         - print help");
		SID_INFO("		-s prefix  - path prefix for ipc key. default /tmp/");
		SID_INFO("		-l         - loop the input file");
		SID_INFO("		-i infile  - input file name");
		SID_INFO("		-o outfile - output file name");
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
				case 'l': {
					m_Loop = 1;
					break;
				}
				case 'i': {
					if (i + 1 >= argc)
						return -1;
					m_InFile = argv[++i];
					break;
				}
				case 'o': {
					if (i + 1 >= argc)
						return -1;
					m_OutFile = argv[++i];
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
	const char *m_InFile;
	const char *m_OutFile;
	int m_Loop;
	bool m_Help;
};


int main(int argc, const char **argv) {
	OptionsParser parser;
	if (parser.ParseOptions(argc, argv) < 0 || parser.m_Help) {
		parser.Usage(argv[0]);
		return -1;
	}

	Sid::SkypePCMInterfaceServer *pcmif_server = new Sid::SkypePCMInterfaceServer();
	Sid::SkypePCMCallbackInterfaceClient *pcmif_cb_client = new Sid::SkypePCMCallbackInterfaceClient();

	SkypePCMInterface* pcmif = SkypePCMInterfaceGet(pcmif_cb_client);
	pcmif_server->set_if(pcmif);


	Sid::String fromskypekitkey;
	Sid::String toskypekitkey;

	fromskypekitkey.Format( "%spcm_from_skypekit_key", parser.m_IpcPrefix);
	toskypekitkey.Format( "%spcm_to_skypekit_key", parser.m_IpcPrefix);

	pcmif_server->Connect(fromskypekitkey.data(), 0);
	pcmif_cb_client->Connect(toskypekitkey.data(), 500);

	if(parser.m_OutFile)
	{
	  Sid::String cmd;
	  Sid::String response;
	  cmd.Format("OUT:%s",parser.m_OutFile);
	  pcmif->CustomCommand(cmd, response);
	}
	if(parser.m_InFile)
	{
	  Sid::String cmd;
	  Sid::String response;
	  cmd.Format("IN:%s",parser.m_InFile);
	  pcmif->CustomCommand(cmd,response);
	}
	if(parser.m_Loop)
	{
	  Sid::String cmd = "LOOP:1";
	  Sid::String response;
	  pcmif->CustomCommand("LOOP:1", response);
	}
	
	Sid::Protocol::Status status;
	do {
		status =pcmif_server->ProcessCommands();
	} while (status == Sid::Protocol::OK);

	SkypePCMInterfaceRelease(pcmif);
	pcmif_server->Disconnect();
	pcmif_cb_client->Disconnect();

	delete pcmif_server;
	delete pcmif_cb_client;

	printf("PCMServerTransport disconnected, exiting from pcmtesthost\n");
}
