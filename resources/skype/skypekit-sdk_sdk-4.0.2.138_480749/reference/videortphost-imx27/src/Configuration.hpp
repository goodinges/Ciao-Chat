/*
 * Copyright (C) 2010 Skype Technologies S.A. Confidential and proprietary
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained
 * in, relating to, or arising from the internet telephony software of Skype
 * Limited (including its affiliates, "Skype"), including without limitation
 * this source code, Skype API and related material of such software
 * proprietary to Skype and/or its licensors ("IP Rights") are and shall
 * remain the exclusive property of Skype and/or its licensors. The recipient
 * hereby acknowledges and agrees that any unauthorized use of the IP Rights
 * is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and ownership
 * thereof.
 */

#ifndef CONFIGURATION_HPP_
#define CONFIGURATION_HPP_

#include <string.h>
#include <stdio.h>

#define CFG_MAX_STR 512
#define NR_ENTRIES 50

struct ConfEntry {
	enum EntryType {
		TYPE_INT = 0,
		TYPE_STR
	};
	void initIntEntry(const char *name_, int val, const char *comm) {
		strncpy(name, name_, sizeof(name));
		type = TYPE_INT;
		intval = val;
		strncpy(comment, comm, sizeof(comment));
	}
	void initStrEntry(const char *name_, const char *val, const char *comm) {
		strncpy(name, name_, sizeof(name));
		type = TYPE_STR;
		strncpy(strval, val, sizeof(strval));
		strncpy(comment, comm, sizeof(comment));
	}
	char name[CFG_MAX_STR];
	EntryType type;
	int intval;
	char strval[CFG_MAX_STR];
	char comment[CFG_MAX_STR];
};

/*! \class Configuration
 *  Convenience class for storage and retrieval of configuration options.
 */
class Configuration {
public:
	static Configuration *GetCfg();

	Configuration();
	~Configuration();

	int Init(const char *filename);
	bool IsInitialized() {return m_InitDone;};
	int GetIntVal(const char *key);
	char *GetStrVal(const char *key);
private:
	void CreateConfigFile(const char *filename);
	int ReadInt(const char *key, int &value);
	int ReadStr(const char *key, char *value);
	int FindKey(const char *key, char *line);
	bool m_InitDone;
	FILE *m_CfgFile;
	char tmpLine[CFG_MAX_STR];
	ConfEntry m_Entries[NR_ENTRIES];
};

#endif /* CONFIGURATION_HPP_ */
