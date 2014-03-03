
#ifdef _WIN32
	const char* keyFileName = "..\\keypair\\keypairfilename";
#else
	const char* keyFileName = "../keypair/keypairfilename";
#endif

char* keyBuf	= 0; 
uint  keyLen	= 0;

bool getKeyPair ()

{
	FILE* f = 0;
	size_t fsize = 0;

	f = fopen(keyFileName, "r");

	if (f != 0)
	{
		fseek(f, 0, SEEK_END);
		fsize = ftell(f);
		rewind(f);
		keyLen = fsize + 1;
		keyBuf = new char[keyLen];
		size_t read = fread(keyBuf,1,fsize,f);
		if (read != fsize) 
		{ 
			printf("Error reading %s\n", keyFileName);
			return false;
		};
		keyBuf[fsize]=0; //cert should be null terminated
		fclose(f);
		return true;		
	};
	
	printf("Error opening app token file: %s\n", keyFileName);
	return false;
};