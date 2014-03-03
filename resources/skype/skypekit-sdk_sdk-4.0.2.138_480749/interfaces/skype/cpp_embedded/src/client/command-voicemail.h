
#ifndef CommandVoiceMail_INCLUDED_HPP
#define CommandVoiceMail_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandVoiceMail : public ClientCommandInterface
{
public:
        CommandVoiceMail() {}
        ~CommandVoiceMail() {}

        void executeHelp();
        String getName() {
                return String("VoiceMail");
        }

        void execute_v(); //start recording new voicemail
        void execute_V(); //end recording and send voicemail
        void execute_w(); //cancel voicemail recording/send
        void execute_p(); //play voicemails
        void execute_s(); //stop voicemail playback

};

#endif //CommandVoiceMail_INCLUDED_HPP

