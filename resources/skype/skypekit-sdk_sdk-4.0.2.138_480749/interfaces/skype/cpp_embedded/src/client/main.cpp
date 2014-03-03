
#include "main.h"
#include <stdio.h>
#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#endif

using namespace Sid;

String inetAddr 	= "127.0.0.1";
int portNum		= 8963;
bool quitEventLoop	= false;
bool mobileMode         = false;
int numEventThreads     = 1;
int dispatchAllEvents   = 0;
String localname        = "";
const char* log_streams = 0;
bool transport_encryption = true; 

char* apptoken_certificate_buf  = 0; 
CSkype* skype 			= 0;
CommandInvoker* invoker		= 0;

#ifdef SKYPEKIT_SURFACE_RENDERING
#if defined(GL_WINDOW)
glWindow *preview_window=0;
glWindow *receive_window=0;
#endif
void window_run_events();
#endif

void Usage(char * lpMyName)
{
        printf("Usage: %s [CONNECTION][OPTIONS]\n\n"
               "CONNECTION:\n"
               "either tcp\n"
               "\t-i ip addr \t\t- ip address (default %s).\n"
               "\t-p port \t\t- tcp port port (default %u).\n"
               "or local\n"
               "\t-l local_connection_name \n"
               "OPTIONS:\n"
               "\t-t token certificate filename \t- file must contain a valid certificate.\n"
               "\t-r log_file_base \t- record transport streams in log path. \"-l ./transport\" will produce ./transport_log_in.1 and  ./transport_log_out.1\n"
               "\t-m \t\t\t- enable Mobile Mode\n"
               "\t-e numeventthreads\t- set number of threads to deal with events 1 (default) <= numeventthreads <= 4\n"
               "\t-n \t\t\t- disable TLS transport encryption\n"
               "\t-a \t\t\t- all events are dispatched (with creating the object if needed)\n"
               "\n",
               lpMyName, (const char*)inetAddr, portNum);
        fflush(stdout);

        exit(1);
}

void skypeCleanup() 
{
        if (skype) {
                skype->stop();
                delete skype;
        }
}

void cleanup()
{
        skypeCleanup();
        if (apptoken_certificate_buf) delete [] apptoken_certificate_buf;
        if (invoker) delete invoker;
}

void mobileSetup()
{
        // If mobileMode option was enabled, setting up all Mobile Mode related keys  
        String mobileModeEnabled        = String("*Lib/PM/MobileModeEnabled");
        String superNodeKeepalivePeriod = String("*Lib/Connection/SupernodeKeepalivePeriod");
        String statsUpdatePeriod        = String("*Lib/Connection/StatsUpdatePeriod");
        
        if(mobileMode) {
                skype->SetInt(mobileModeEnabled, 1);
                skype->SetInt(superNodeKeepalivePeriod, 600);
                skype->SetInt(statsUpdatePeriod, 600);
        } else { // Otherwise checking and deleting those keys          
                bool isDefined = false;
                
                skype->IsDefined(mobileModeEnabled, isDefined);
                if(isDefined) skype->Delete(mobileModeEnabled);
                        
                skype->IsDefined(superNodeKeepalivePeriod, isDefined);
                if(isDefined) skype->Delete(superNodeKeepalivePeriod);
                        
                skype->IsDefined(statsUpdatePeriod, isDefined);
                if(isDefined) skype->Delete(statsUpdatePeriod);
        }         
}

void skypeConnect()
{
        skypeCleanup();   
                            
        Sid::TransportInterface::Status status; 
        skype = new CSkype(numEventThreads);
        skype->set_transport_encryption(transport_encryption);
	if (dispatchAllEvents) 
		skype->set_dispatch_all();
        if (localname == "")
                status = skype->init(apptoken_certificate_buf, (const char*)inetAddr, portNum, log_streams);
        else
                status = skype->init(apptoken_certificate_buf, (const char*)localname, log_streams);
                   
        if (status != Sid::TransportInterface::OK) {
                printf("\n::: Error connecting to skypekit, enter 'r' to reconnect...\n");
                return;               
        }
        
        skype->start();    
           
        mobileSetup();    

        Sid::String version;
        skype->GetVersionString(version);
        printf("%s\n\n", (const char*) version);
          
        skype->login_default_account();  
}

void sigInt(int /*n*/)
{
        printf("\n::: Exiting...\n");
        fflush(stdout);
        
        cleanup();
        exit(1);
}

#ifndef _WIN32
bool main_select(int runtime_fd, int& polled)
{
#ifndef SKYPEKIT_SURFACE_RENDERING
        if (!runtime_fd) return true;
#endif
        polled = 0;
        struct timeval tv;
        fd_set read_fd;

#ifndef SKYPEKIT_SURFACE_RENDERING
        tv.tv_sec  = 0;
        tv.tv_usec = 200000;
#else
        tv.tv_sec = 0;
        tv.tv_usec = 10000;
#endif
        FD_ZERO(&read_fd);
        FD_SET(0, &read_fd);
        if (runtime_fd) FD_SET(runtime_fd, &read_fd);

        if (select(runtime_fd+1, &read_fd, NULL, NULL, &tv) == -1) return false;
        if (runtime_fd) polled = FD_ISSET(runtime_fd, &read_fd); 
        return FD_ISSET(0, &read_fd);
}
#else
bool main_select(int, int&) {
#ifdef SKYPEKIT_SURFACE_RENDERING
        return ::_kbhit() != 0;
#else
        return true;
#endif
}
#endif

int main(int argc, char * argv[])
{
        int ret = 0;
        char buffer[512];
        unsigned char command;
        unsigned char subCommand;

        // collect parameters
        for (int i = 1; i < argc; i++) {
                if (argv[i][0] == '-' || argv[i][0] == '/') {
                        switch (argv[i][1]) {
                        case 'i': { // internet address?
                                if (i + 1 == argc) Usage(argv[0]);
                                if (argv[i + 1][0] == '-') Usage(argv[0]);
                                inetAddr = String(argv[++i]);
                                break;
                        }
                        case 'p': { // port number
                                if (i + 1 == argc) Usage(argv[0]);
                                if (argv[i + 1][0] == '-') Usage(argv[0]);
                                portNum = atoi(argv[++i]);
                                break;
                        }
                        case 'e': { // num threads for events
                                if (i + 1 == argc) Usage(argv[0]);
                                if (argv[i + 1][0] == '-') Usage(argv[0]);
                                numEventThreads = atoi(argv[++i]);
#ifndef _WIN32
                                if (numEventThreads == 0) break; // We demonstrate signle threading only on linux
#endif
                                if (numEventThreads < 1) numEventThreads = 1;
                                else if (numEventThreads > 4) numEventThreads = 4;
                                break;
                        }
                        case 'l': { // local connection?
                                if (i + 1 == argc) Usage(argv[0]);
                                if (argv[i + 1][0] == '-') Usage(argv[0]);
                                localname = String(argv[++i]);
                                break;
                        }
                        case 'r': { // log transport streams?
                                if (i + 1 == argc) Usage(argv[0]);
                                if (argv[i + 1][0] == '-') Usage(argv[0]);
                                log_streams = argv[++i];
                                break;
                        }
                        case 't': { // token?
                                FILE* f = 0;
                                size_t fsize = 0;
                                if (i + 1 < argc && argv[i + 1][0] != '-' && (f = fopen(argv[++i], "r")) != 0) {
                                        fseek(f, 0, SEEK_END);
                                        fsize = ftell(f);
                                        rewind(f);
                                        apptoken_certificate_buf = new char[fsize + 1];
                                        size_t read = fread(apptoken_certificate_buf, 1, fsize, f);
                                        if (read != fsize) { printf("error reading token cert file\n");}
                                        apptoken_certificate_buf[fsize]=0; //cert buffer should be null terminated
                                        fclose(f);
                                } else
                                        Usage(argv[0]);
                                break;
                        }
                        case 'a': {                 
                        	dispatchAllEvents = 1;
                        	break;
                        }
                        case 'm': { // Mobile Mode                    
                        	mobileMode = true;
                        	break;
                        }
                        case 'n': {
                                transport_encryption = false; 
                                break;
                        }
                        default:
                                Usage(argv[0]);
                                break;
                        }
                } else {
                        Usage(argv[0]);
                        break;
                }
        }

        signal(SIGINT, sigInt);

        skypeConnect();
        
        printf("\n::: Type ? for list of available commands\n");
        fflush(stdout);
        
        invoker = new CommandInvoker();
        int polled = 0;
        int runtime_fd = numEventThreads ? 0 : skype->get_socket_fd();
        while (!quitEventLoop) {
                if (main_select(runtime_fd, polled)) { // can be omitted if numEventThreads > 0 and no SKYPEKIT_SURFACE_RENDERING
                        if (!fgets(buffer, 79, stdin)) { /*error*/ }
                        if (char* ps = strchr(buffer, '\n')) *ps = '\0';

                        if (strlen(buffer) != 2) {
                                if (buffer[0] == '?') {
                                        invoker->executeHelp(skype->SkypekitConnected());
                                        fflush(stdout);
                                } else if (buffer[0] == '%') {
                                        skype->PrintObjectStatistics();                                                                                 
                                } else if (buffer[0] == 'q') {
                                        break;      
                                } else if (buffer[0] == 'r') {
                                        printf("\n::: Reconnecting...\n");
                                        skypeConnect();                                                                           
                                } else {
                                        printf("\n::: Type ? for list of available commands\n");
                                        fflush(stdout);
                                }

                                continue;
                        }

                        command = buffer[strlen(buffer) - 2];
                        subCommand = buffer[strlen(buffer) - 1];
                        
                        if (skype->SkypekitConnected())
                                invoker->executeCommand(command, subCommand);
                        else 
                                printf("\n::: Disconnected from skypekit, enter 'r' to reconnect...\n\n");

                }
                if (numEventThreads == 0) skype->process_events(polled);                                
#ifdef SKYPEKIT_SURFACE_RENDERING
                window_run_events();
#endif
        }
        
        cleanup();      
        return ret;
}

