/**
 * Copyright (C) 2010, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

package com.skype.skypekitclient.command;



import com.skype.api.Conversation;
import com.skype.api.Conversation.LIST_TYPE;
import com.skype.api.Conversation.PARTICIPANTFILTER;
import com.skype.api.Participant;
import com.skype.api.Skype;
import com.skype.api.Video;
import com.skype.skypekitclient.SkypekitClient;
import com.skype.skypekitclient.utils.Ask;

/**
 *
 */
public class CommandConversationCall extends ClientCommandInterface {
    //    private static final String TAG = "CommandConversationCall";

    private SkypekitClient skClient;

    protected CommandConversationCall(SkypekitClient skypekitClient) {
        super();
        skClient = skypekitClient;
    }

    @Override
    public String getName() { return "ConversationCall"; }

    public void executeHelp()
    {
        skClient.out(
                "\n[CONVERSATION CALL] - C\n" +
                "\tCp - place a call (selected conversation)\n" +
                "\tCx - place a call (enter skypename)\n" +
                "\tCe - end call\n" +
                "\tCa - answer a call\n" +
                "\tCf - forward a call\n" +
                "\tCm - mute/unmute\n" +
                "\tCh - hold/unhold\n" +
                "\tCd - send dtmf\n" +
                "\tCv - [VIDEO] place a call (video is enabled/started automatically)\n" +
                "\tCS - [VIDEO] start sending video\n" +
                "\tCs - [VIDEO] stop sending video\n" +
                "\tCR - [VIDEO] start receiving video\n" +
                "\tCV - [VIDEO] get video device list\n" +
                "\tCr - [VIDEO] stop receiving video\n" +
                "\tCc - [VIDEO] send custom command\n" +
                "\tCC - [VOICE] send custom command\n" +
                "\tCD - [VOICE] get devicelist\n" +
                "\tCE - [VOICE] select device\n"
        );
    }


    public void execute_a()	// answer a call
    {
        if (skClient.notLoggedIn()) {
            System.out.println("You are not logged in.");
            return;
        }

        Conversation[] conversations = skClient.skype.GetConversationList(LIST_TYPE.LIVE_CONVERSATIONS);
        if (conversations == null) {
            skClient.error("Unable to get conversation list");
            return;
        }
        
        for (int i = 0; i < (int)conversations.length; i++) {
            Conversation conversation = conversations[i];
 
            skClient.currentConversation = conversation;
            if (skClient.currentConversation != null) {
                Conversation.LOCAL_LIVESTATUS lstatus = Conversation.LOCAL_LIVESTATUS.get(
                            skClient.currentConversation.GetIntProperty(Conversation.PROPERTY.local_livestatus));

                if (lstatus == Conversation.LOCAL_LIVESTATUS.RINGING_FOR_ME) {
                
                    System.out.printf("found conversation %d\n", conversation.getOid());
                    
                    skClient.currentConversation.JoinLiveSession("");
                    
                    // create participant objects to receive propchanges
                    Participant[] participants = skClient.currentConversation.GetParticipants(PARTICIPANTFILTER.ALL);
                    for (int k = 0; k < participants.length; k++) {
                        participants[k].getOid();
                    }                                      
                    return;
                }
            }
        }
    }

    public void execute_p()	// place a call (selected conversation)
    {
        ring(false);
    }

    public void execute_v()	// [VIDEO] place a call (video is enabled/started automatically)
    {
        ring(true);
    }

    public void execute_e()	// end call
    {
        if (skClient.notLoggedIn())
            return;

        Conversation[] conversations = skClient.skype.GetConversationList(LIST_TYPE.LIVE_CONVERSATIONS);
        if (conversations == null) {
            skClient.error("Unable to get conversation list");
            return;
        }

        if (conversations.length == 0) {
            skClient.out("0 conversations");
            return;
        }

        for (int i = 0; i < conversations.length; i++) {
            skClient.out("found conversation" + conversations[i].getOid());
            conversations[i].LeaveLiveSession(false);
        }
    }

    public void execute_f()	// forward a call
    {
        skClient.error("TBD");
    }

    public void execute_m()	// mute/unmute
    {
        if (skClient.currentConversation != null) {
            if (skClient.currentConversation.GetBooleanProperty(Conversation.PROPERTY.live_is_muted) == false) {
                skClient.currentConversation.MuteMyMicrophone();
            } else {
                skClient.currentConversation.UnmuteMyMicrophone();
            }
        }
    }

    public void execute_h()	// hold/unhold
    {
        if (skClient.currentConversation != null) {
            Conversation.LOCAL_LIVESTATUS lstatus = 
                Conversation.LOCAL_LIVESTATUS.get(
                        skClient.currentConversation.GetIntProperty(Conversation.PROPERTY.local_livestatus));

            if (lstatus != Conversation.LOCAL_LIVESTATUS.ON_HOLD_LOCALLY) {
                skClient.currentConversation.HoldMyLiveSession();
            } else {
                skClient.currentConversation.ResumeMyLiveSession();
            }
        }
    }

    public void execute_d()	// send dtmf
    {
        if (skClient.currentConversation != null) {
            String dtmfstr = Ask.ask("Enter dtmf (1,2,3,4,5,6,7,8,9,0,*,#):");
            Participant.DTMF dtmf;
            String star = "*";
            String pound = "#";
            if (dtmfstr.equals(star)) {
                dtmf = Participant.DTMF.DTMF_STAR;
            } else if (dtmfstr.equals(pound)) {
                dtmf = Participant.DTMF.DTMF_POUND;
            } else {
            	int dtn = Integer.valueOf(dtmfstr);
                dtmf = Participant.DTMF.get(dtn);
            }
            skClient.currentConversation.SendDTMF(dtmf, 260); //TODO: using 260 <used for lengthInMS in CommandConversationConference> next make it a const
        }
    }

    public void execute_S()	// [VIDEO] start sending video
    {
        Video video = get_send_video();
        if (video != null) {
            Video.GetCurrentVideoDeviceResult videoDevice = video.GetCurrentVideoDevice();
            Video preview = skClient.skype.GetPreviewVideo(videoDevice.mediatype, videoDevice.deviceName, videoDevice.devicePath);

            if (preview != null) {
                preview.Start();
            }
            video.Start(); 
        }
    }

    public void execute_s()	// [VIDEO] stop sending video
    {
        Video video = get_send_video();
        if (video != null) {
            video.Stop(); 
        }
    }

    public void execute_R()	// [VIDEO] start receiving video
    {
        Video video = get_receive_video();
        if (video != null) {
            video.Start(); 
        }
    }

    public void execute_r() // [VIDEO] stop receiving video
    {
        Video video = get_receive_video();
        if (video != null) {
            video.Stop();
        }
    }

    public void execute_c()	// [VIDEO] send custom command
    {
        String command, response;
        command = Ask.ask("enter command:");

        skClient.out("VideoCommand: sending " + command);
        response = skClient.skype.VideoCommand(command);
        skClient.out("VideoCommand: response: " + response);
    }

    public void execute_C()	// [VIDEO] send custom command
    {
        String command, response;
        command = Ask.ask("enter command:");

        skClient.out("VoiceCommand: sending " + command);
        response = skClient.skype.VoiceCommand(command);
        skClient.out("VoiceCommand: response: " + response);
    }

    private void ring(boolean enable_video)
    {
        if (skClient.currentConversation != null) {
            //get participants
            Participant[] participants = skClient.currentConversation.GetParticipants(Conversation.PARTICIPANTFILTER.OTHER_CONSUMERS);
            if (participants == null) {
                skClient.error("Unable to get participant list");
                return;
            }
            if (participants.length == 0) {
                skClient.out("No participants");
                return;
            }

            //ring participants
            for (int i = 0; i < participants.length; i++) {
                participants[i].Ring("", enable_video, 0, 0, false, "");
            }
        }
        else
            skClient.out("You are not logged in or there is no selected conversation (use cc)");
    }

    private Video get_send_video()
    {
        Video video = null;
        if (skClient.currentConversation != null) {
            Conversation.TYPE ctype = Conversation.TYPE.get(skClient.currentConversation.GetIntProperty(Conversation.PROPERTY.type));
            if (ctype != Conversation.TYPE.DIALOG) {
                skClient.error("Selected conversation is not 1-1 dialog call");
                return video;      
            }

            //get participants
            Participant[] participants = skClient.currentConversation.GetParticipants(Conversation.PARTICIPANTFILTER.MYSELF);
            if (participants == null) {
                skClient.error("Unable to get participant");
                return video;
            }

            //get video
            if (participants.length > 0) {
                Participant participant = participants[0];
                video = participant.GetVideo();
                if (video != null) {
                    return video;
                }
            } else {
                skClient.out("No participants received");
            }
        }
        return video;
    }

    private Video get_receive_video()
    {
        Video video = null;
        if (skClient.currentConversation != null) {
            Conversation.TYPE ctype = Conversation.TYPE.get(skClient.currentConversation.GetIntProperty(Conversation.PROPERTY.type));
            if (ctype != Conversation.TYPE.DIALOG) {
                skClient.error("Selected conversation is not 1-1 dialog call");
                return video;      
            }

            //get participants
            Participant[] participants = skClient.currentConversation.GetParticipants(Conversation.PARTICIPANTFILTER.OTHER_CONSUMERS);
            if (participants == null) {
                skClient.error("Unable to get participant");
                return video;
            }

            //get video
            if (participants.length > 0) {
                Participant participant = participants[0];
                video = participant.GetVideo();
                if (video != null) {
                    skClient.out("Got video from " + participant.GetStrProperty(Participant.PROPERTY.identity));
                    return video;
                } else {
                    skClient.error("Unable to get video from participant: " + participant.GetStrProperty(Participant.PROPERTY.identity));
                }
            } else {
                skClient.out("No participants received");
            }
        }
        return video;
    }

    public void execute_x()	// place a call (enter skypename)
    {
        if (skClient.notLoggedIn())
            return;

        String[] names = {Ask.ask("call to (enter skypename):")};
        if ( names.length == 0) {
            skClient.error("No participant entered, unable to get conversation");
            return;
        }

        Conversation conversation = skClient.skype.GetConversationByParticipants(names, true, false);
        if (conversation == null) {
            System.err.println("Error: Unable to get conversation");
            return;
        }

        skClient.currentConversation = conversation;     
        Participant[] participants;
        participants = skClient.currentConversation.GetParticipants(PARTICIPANTFILTER.ALL);
        for (int i = 0; i < participants.length; i++) 
            participants[i].getOid();    

        ring(true);
    }

    public void execute_V()	// [VIDEO] get video device list
    {
        Skype.GetAvailableVideoDevicesResult videoDevices =	skClient.skype.GetAvailableVideoDevices();
        skClient.out("video devices:");
        for(int i = 0; i < videoDevices.count; i++)
            skClient.out(videoDevices.deviceNames[i] + " \t " + 
                    videoDevices.devicePaths[i]);
        return;
    }

    public void execute_D()	// [VOICE] get devicelist
    {
        Skype.GetAvailableOutputDevicesResult outputDevices = skClient.skype.GetAvailableOutputDevices();
        skClient.out("devices:");
        for(int i = 0; i < outputDevices.handleList.length; i++)
            skClient.out(outputDevices.handleList[i] + " \t " + 
                    outputDevices.nameList[i] + " \t " +
                    outputDevices.productIdList[i]);
        return;
    }

    public void execute_E()	// [VOICE] select device
    {
        String in,out,notification;
        in = Ask.ask("input device:");
        out = Ask.ask("output device:");
        notification = Ask.ask("notification device:");
        skClient.skype.SelectSoundDevices(in,out,notification);
        //			skClient.out("Setting sound devices successful");
        //		else
        //			skClient.error("Setting sound devices failed");
    }

}
