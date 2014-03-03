
#include "SidDebugging.hpp"
#include "SidVideoHostConnection.hpp"

struct OptionsParser {
	OptionsParser() :
		m_IpcPrefix("/tmp/"),
		m_Help(false) {
	}
	void Usage(const char *executable) {
		SID_INFO("usage:");
		SID_INFO("	%s [options]", executable);
		SID_INFO("	options:");
		SID_INFO("		-h - print help");
		SID_INFO("		-s prefix - path prefix for ipc key. default /tmp/");
		SID_INFO("		-r - reconnect after connection lost");
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
				case 'r': {
					m_Reconnect = true;
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
	int m_Reconnect;
};


#ifndef DONT_DEFINE_VIDEOHOST_MAIN
int main(int argc, const char **argv) {
	Sid::SidVideoHostConnection m_IpcConn;
	OptionsParser parser;
	if (parser.ParseOptions(argc, argv) < 0 || parser.m_Help) {
		parser.Usage(argv[0]);
		return -1;
	}

	if (parser.m_Reconnect) {
		while (true) {
			m_IpcConn.StartIpcThread(parser.m_IpcPrefix);
			m_IpcConn.WaitDisconnect();
		}
	} else {
		m_IpcConn.StartIpcThread(parser.m_IpcPrefix);
		m_IpcConn.WaitDisconnect();
	}
}
#endif
