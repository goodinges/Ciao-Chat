
#include "command-setup.h"

void CommandSetup::executeHelp()
{
        printf("\n[SETUP] - e\n"
               "\tef - enable/disable file transfer auto-accept feature\n"
               "\tea - set idle time (in seconds) for away\n"
               "\ten - set idle time (in seconds) for not available\n"
               "\tet - [CALL] set timeout (in seconds) for call forwarding / send to voicemail\n"
               "\tev - [CALL] send call to voicemail after the timeout\n"
               "\tec - [CALL] apply call forwarding rules. Overrides 'ev' command\n"
               "\tes - start sending video automatically when call starts\n"
               "\ted - enable/disable video functionality\n"
			   "\tei - set video device\n"
              );
}

void CommandSetup::execute_f()
{
        int result; 
        skype->GetInt(SETUPKEY_FT_AUTOACCEPT, result);
        if (0 == result) {
                if (!ask_yesno("Autoaccept is disabled, enable?")) {
                        skype->SetInt(SETUPKEY_FT_AUTOACCEPT, 1);
                        skype->SetStr(SETUPKEY_FT_SAVEPATH, 
                                ask("full path to save files (used for AutoAccept feature):")); 
                        printf("enabled\n");       
                }
        } else {
                if (!ask_yesno("Autoaccept is enabled, disable?")) {
                        skype->SetInt(SETUPKEY_FT_AUTOACCEPT, 0);
                        printf("disabled\n");         
                }
        }
}

void CommandSetup::execute_n()
{
        print_int_val(SETUPKEY_IDLE_TIME_FOR_NA, "Current value:");
        skype->SetInt(SETUPKEY_IDLE_TIME_FOR_NA, 
                ask_int("Idle time (in seconds) after which availability is set to NOT_AVAILABLE:"));
        print_int_val(SETUPKEY_IDLE_TIME_FOR_NA, "New idle time for NOT_AVAILABLE:");
}

void CommandSetup::execute_a()
{
        print_int_val(SETUPKEY_IDLE_TIME_FOR_AWAY, "Current value:");
        skype->SetInt(SETUPKEY_IDLE_TIME_FOR_AWAY, 
                ask_int("Idle time (in seconds) after which availability is set to AWAY:"));
        print_int_val(SETUPKEY_IDLE_TIME_FOR_AWAY, "New idle time for AWAY:");               
}

void CommandSetup::execute_t()
{
        print_int_val(SETUPKEY_CALL_NOANSWER_TIMEOUT, "Current value:");
        skype->SetInt(SETUPKEY_CALL_NOANSWER_TIMEOUT, 
                ask_int("Timeout (in seconds) for call forwarding / send to voicemail:"));
        print_int_val(SETUPKEY_CALL_NOANSWER_TIMEOUT, "New call answer timeout:");
}

void CommandSetup::execute_v()
{
        print_int_val(SETUPKEY_CALL_SEND_TO_VM, "Current value:");
        skype->SetInt(SETUPKEY_CALL_SEND_TO_VM, 
                !ask_yesno("Send call to voicemail after the timeout?"));
        print_int_val(SETUPKEY_CALL_SEND_TO_VM, "Call sent to vm after timeout:");
}

void CommandSetup::execute_c()
{
        print_int_val(SETUPKEY_CALL_APPLY_CF, "Current value:");
        skype->SetInt(SETUPKEY_CALL_APPLY_CF, 
                !ask_yesno("Apply call forwarding rules?"));
        print_int_val(SETUPKEY_CALL_APPLY_CF, "Call sent to cf after timeout:");
}

void CommandSetup::execute_s()
{
        print_int_val(SETUPKEY_VIDEO_AUTOSEND, "Current value:");
        skype->SetInt(SETUPKEY_VIDEO_AUTOSEND, 
                !ask_yesno("Start sending video automatically when call starts?"));
        print_int_val(SETUPKEY_VIDEO_AUTOSEND, "Video is started automatically:");
}

void CommandSetup::execute_d()
{
        int result; 
        skype->GetInt(SETUPKEY_VIDEO_DISABLE, result);
        if (0 == result) {
                if (!ask_yesno("Video is enabled, disable?")) {
                        skype->SetInt(SETUPKEY_VIDEO_DISABLE, 1);
                        printf("disabled\n");       
                }
        } else {
                if (!ask_yesno("Video is disabled, enable?")) {
                        skype->SetInt(SETUPKEY_VIDEO_DISABLE, 0);
                        printf("enabled\n");         
                }
        }
}

void CommandSetup::execute_i()
{
	Sid::String currentVideoDeviceName, currentVideoDevicePath;
	Sid::List_String lVideoDeviceNames, lVideoDevicePaths;	
	uint numVideoDevices;
	uint selectVideoDevice;
	
	// get the current video device
	skype->GetStr(SETUPKEY_VIDEO_DEVICE, currentVideoDeviceName);
	skype->GetStr(SETUPKEY_VIDEO_DEVICE_PATH, currentVideoDevicePath);
	printf("\nCurrent video device: %s \t %s \n\n", (const char*)currentVideoDeviceName, (const char*)currentVideoDevicePath);
	
	// display the list of available devices
	skype->GetAvailableVideoDevices(lVideoDeviceNames, lVideoDevicePaths, numVideoDevices);
	for(uint i = 0; i < numVideoDevices; i++) {
		printf("%d. - %s \t %s", i, (const char*)lVideoDeviceNames[i], (const char*)lVideoDevicePaths[i] );	
		printf("\n");
	}
	printf("Select new video device: \n");		
	scanf("%d", &selectVideoDevice);
	
	// set the new selected device
	if (selectVideoDevice >= 0 && selectVideoDevice <= numVideoDevices) {
		skype->SetStr(SETUPKEY_VIDEO_DEVICE, (const char*)lVideoDeviceNames[selectVideoDevice]);
		skype->SetStr(SETUPKEY_VIDEO_DEVICE_PATH, (const char*)lVideoDevicePaths[selectVideoDevice]);
	}
	
	// get the current video device again to show what was set
	skype->GetStr(SETUPKEY_VIDEO_DEVICE, currentVideoDeviceName);
	skype->GetStr(SETUPKEY_VIDEO_DEVICE_PATH, currentVideoDevicePath);
	printf("new video device: %s %s \n", (const char*)currentVideoDeviceName, (const char*)currentVideoDevicePath);	
}

void CommandSetup::print_int_val(const char* key, const char* descr)
{
        int result; 
        skype->GetInt(key, result);        
        printf("\n%s %d\n", descr, result);
}


















