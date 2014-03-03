
#include "command-conversationcall.h"

#ifdef GL_WINDOW
#ifdef _WIN32
#include "winglwindow.h"
#else
#include "glwindow.h"
#endif
#endif

void CommandConversationCall::executeHelp()
{
        printf("\n[CONVERSATION CALL] - C\n"
               "\tCp - place a call (selected conversation)\n"
               "\tCx - place a call (enter skypename)\n"
               "\tCn - place a call (enter number)\n"
               "\tCe - end call\n"
               "\tCa - answer a call\n"
               "\tCf - forward a call\n"
               "\tCm - mute/unmute\n"
               "\tCh - hold/unhold\n"
               "\tCd - send dtmf\n"
               "\tCv - [VIDEO] place a call (video is enabled/started automatically)\n"
               "\tCV - [VIDEO] get devicelist\n"
               "\tCS - [VIDEO] start sending video\n"
               "\tCs - [VIDEO] stop sending video\n"
               "\tCR - [VIDEO] start receiving video\n"
               "\tCr - [VIDEO] stop receiving video\n"
               "\tCw - [VIDEO] get dimensions\n"
               "\tCc - [VIDEO] send custom command\n"
               "\tCC - [VOICE] send custom command\n"
               "\tCD - [VOICE] get devicelist\n"
               "\tCE - [VOICE] select device\n"
              );
}

void CommandConversationCall::execute_a()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                return;
        }

        Conversation::Refs conversations;
        if (!skype->GetConversationList(conversations, Conversation::LIVE_CONVERSATIONS)) {
                printf("Unable to get conversation list\n");
                return;
        }
                
        for (uint i = 0; i < (uint)conversations.size(); i++) {
                Conversation::Ref conversation = conversations[i];
                Conversation::LOCAL_LIVESTATUS conv_live_status;
                conversation->GetPropLocalLivestatus(conv_live_status);

                if (conv_live_status == Conversation::RINGING_FOR_ME) {
                        printf("found conversation %d\n", conversation->getOID());
                        conversation->JoinLiveSession();
                        skype->set_current_conversation(conversation);
                        //create participant objects to receive propchanges 
                        ParticipantRefs participants;
                        conversation->GetParticipants(participants);
                        fetch(participants);
                        for (uint j = 0; j < (uint)participants.size(); j++) {
                                participants[j]->getOID();    
                        }                          
                        return;
                }
        }
}

void CommandConversationCall::execute_p()
{
        ring(false);
}

void CommandConversationCall::execute_v()
{
        ring(true);
}

void CommandConversationCall::execute_V()
{
	Sid::List_String name, path;
	uint count;
	skype->GetAvailableVideoDevices(name,path,count);
	printf("video devices( %u / %u ):\n", count, (uint) name.size());
	for(uint i = 0; i < (uint)name.size(); i++)
		printf("%s \t %s\n",
                      (const char*)name[i],
                      (const char*)path[i] );
	return;
}

void CommandConversationCall::execute_e()
{
        if (!skype->loggedIn()) {
                printf("You are not logged in\n");
                return;
        }

        Conversation::Refs conversations;
        if (!skype->GetConversationList(conversations, Conversation::LIVE_CONVERSATIONS)) {
                printf("Unable to get conversation list\n");
                return;
        }

        for (uint i = 0; i < (uint)conversations.size(); i++) {
                Conversation::Ref conversation = conversations[i];
                printf("found conversation %d\n", conversation->getOID());
                conversation->LeaveLiveSession(false);
        }
}

void CommandConversationCall::execute_f()
{
        printf("TBD\n");
}

void CommandConversationCall::execute_m()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                bool is_muted = false;
                conversation->GetPropLiveIsMuted(is_muted);
                if (!is_muted) {
                        conversation->MuteMyMicrophone();
                } else {
                        conversation->UnmuteMyMicrophone();
                }
        }
}

void CommandConversationCall::execute_h()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                Conversation::LOCAL_LIVESTATUS lstatus;
                conversation->GetPropLocalLivestatus(lstatus);
                if (lstatus != Conversation::ON_HOLD_LOCALLY) {
                        conversation->HoldMyLiveSession();
                } else {
                        conversation->ResumeMyLiveSession();
                }
        }
}

void CommandConversationCall::execute_d()
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                String dtmfstr = ask("Enter dtmf (1,2,3,4,5,6,7,8,9,0,*,#):");
                Participant::DTMF dtmf;
                if (dtmfstr == "*") {
                        dtmf = Participant::DTMF_STAR;
                } else if (dtmfstr == "#") {
                        dtmf = Participant::DTMF_POUND;
                } else {
                        dtmf = (Participant::DTMF)atoi((const char*)dtmfstr);
                }
                conversation->SendDTMF(dtmf);
        }
}

void CommandConversationCall::execute_S()
{
        VideoRef video = get_send_video();
        if (video) {
                VideoRef preview;
                Sid::String name,path;
                Video::MEDIATYPE type=Video::MEDIA_VIDEO;
		video->SelectVideoSource( type, "Display iSight", "0x2624200005ac8508", false );
                video->GetCurrentVideoDevice(type,name,path);
                skype->GetPreviewVideo(type,preview,name,path);
                if (preview)
                {
#ifdef SKYPEKIT_SURFACE_RENDERING
                        if (preview_window)
                        {
                                preview->SetRemoteRendererId(0);
                                delete preview_window;
                                preview_window = NULL;
                        }
#if GL_WINDOW
                        preview_window = new glWindow( "Preview" ); 
#endif

                        if ( preview_window )
                                preview->SetRemoteRendererId(preview_window->getKey());
                        
						preview->Start();
#endif
                }
                video->Start(); 
        }
}

void CommandConversationCall::execute_s()
{
        VideoRef video = get_send_video();
        if (video) {
#ifdef SKYPEKIT_SURFACE_RENDERING
                VideoRef preview;
                Sid::String name,path;
                Video::MEDIATYPE type=Video::MEDIA_VIDEO;
                video->GetCurrentVideoDevice(type,name,path);
                skype->GetPreviewVideo(type,preview,name,path);
                if (preview)
                {
                        preview->SetRemoteRendererId(0);
                        preview->Stop();
                        if (preview_window)
                        {
                                delete preview_window;
                        }
                        preview_window = 0;
                }
#endif
                video->Stop(); 
        }
}

void CommandConversationCall::execute_w()
{
        VideoRef video = get_send_video();
	Sid::String dimensions;
        if (video && video->GetPropDimensions(dimensions)) {
		printf("Send Video Size %s\n", (const char*) dimensions);
        }
        video = get_receive_video();
        if (video && video->GetPropDimensions(dimensions)) {
		printf("Receive Video Size %s\n", (const char*) dimensions);
        }
}


void CommandConversationCall::execute_R()
{
        VideoRef video = get_receive_video();
        if (video) {
#ifdef SKYPEKIT_SURFACE_RENDERING
                if (receive_window)
                {
			video->SetRemoteRendererId(0);
                        delete receive_window;
			receive_window = NULL;
                }

#if GL_WINDOW
		receive_window = new glWindow( "Remote" );
#endif
		if ( receive_window )
			video->SetRemoteRendererId(receive_window->getKey());
#endif
                video->Start(); 
        }
}

void CommandConversationCall::execute_r()
{
        VideoRef video = get_receive_video();
        if (video) {
#ifdef SKYPEKIT_SURFACE_RENDERING
		video->SetRemoteRendererId(0);
                video->Stop();
                if (receive_window)
                {
                        delete receive_window;
                }
                receive_window=0;
#else
                video->Stop();
#endif
        }
}

void CommandConversationCall::execute_c()
{
	String command, response;
        command = ask("enter command:");

	printf("VideoCommand: sending %s\n", (const char*)command);
	skype->VideoCommand(command, response);
	printf("VideoCommand: response: %s\n", (const char*)response);
}

void CommandConversationCall::execute_C()
{
        String command, response;
        command = ask("enter command:");

        printf("VoiceCommand: sending %s\n", (const char*)command);
        skype->VoiceCommand(command, response);
        printf("VoiceCommand: response: %s\n", (const char*)response);
}

void CommandConversationCall::ring(bool enable_video)
{
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) {
                //get participants
                Participant::Refs participants;
                if (!conversation->GetParticipants(participants, Conversation::OTHER_CONSUMERS)) {
                        printf("Unable to get participant list\n");
                        return;
                }
                fetch(participants);
                //ring participants
                for (uint i = 0; i < (uint)participants.size(); i++) {
                        Participant::Ref participant = participants[i];
                        if (participant) {
                                participant->Ring("", enable_video);
                        }
                }
        }
}

VideoRef CommandConversationCall::get_send_video()
{
	VideoRef video(0);
	Conversation::Ref conversation = skype->get_current_conversation();
	if (conversation) {
                Conversation::TYPE ctype;
                conversation->GetPropType(ctype);
                if (ctype != Conversation::DIALOG) {
                        printf("Selected conversation is not 1-1 dialog call\n");
                        return video;      
                }
                        
                //get participants
                Participant::Refs participants;
                if (!conversation->GetParticipants(participants, Conversation::MYSELF)) {
                        printf("Unable to get participant\n");
                        return video;
                }

                //get video
                if (participants.size()) {
                        Participant::Ref participant = participants[0];
                        if (participant && participant->GetVideo(video)) {
                                return video;
                        }
                } else {
                        printf("No participants received\n");
                }
	}
	return video;
}

VideoRef CommandConversationCall::get_receive_video()
{
        VideoRef video(0);
        Conversation::Ref conversation = skype->get_current_conversation();
        if (conversation) { 
                Conversation::TYPE ctype;
                conversation->GetPropType(ctype);
                if (ctype != Conversation::DIALOG) {
                        printf("Selected conversation is not 1-1 dialog call\n");
                        return video;      
                }
                             
                //get participants
                Participant::Refs participants;
                if (!conversation->GetParticipants(participants, Conversation::OTHER_CONSUMERS)) {
                        printf("Unable to get participant\n");
                        return video;
                }

                //get video
                if (participants.size()) {
                        Participant::Ref participant = participants[0];
                        if (participant && participant->GetVideo(video)) {
                                printf("Got video from %s\n", 
                                        (const char*)participant->GetStrProp(Participant::P_IDENTITY));
                                return video;
                        }
                        else 
                        {
                                printf("Unable to get video from participant: %s\n", participant ? (const char*)participant->GetStrProp(Participant::P_IDENTITY) : "unknown");
                        }
                } else {
                        printf("Unable to get participant\n");
                }
        }
        return video;
}

void CommandConversationCall::execute_x()
{
        if (!skype->select_conversation(ask("call to (enter skypename):"))) {
              printf("Unable to get conversation\n"); 
              return;
        }
        ring(true);
}

void CommandConversationCall::execute_n()
{
        Conversation::Ref conversation;
        String target = ask("call to (enter number):");
        skype->GetConversationByIdentity(target, conversation);
        if (conversation) {
                skype->set_current_conversation(conversation);
        } else {
                printf("unable to create conversation from identity %s\n", (const char*)target);
                return;
        }
        ring(false);
}

void CommandConversationCall::execute_D()
{
	Sid::List_String guid, name, productID;
	skype->GetAvailableOutputDevices(guid,name,productID);
	printf("devices:\n");
	for(uint i = 0; i < (uint)guid.size(); i++)
		printf("%s \t %s \t %s\n",
                      (const char*)guid[i],
                      (const char*)name[i],
                      (const char*)productID[i]);
	return;
}

void CommandConversationCall::execute_E()
{
	String in,out,notification;
	in = ask("input device:");
	out = ask("output device:");
	notification = ask("notification device:");
        if(skype->SelectSoundDevices(in,out,notification))
		printf("Setting sound devices successful\n");
	else
		printf("Setting sound devices failed\n");
}

