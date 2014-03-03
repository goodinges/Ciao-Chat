
#ifndef CommandConversationCall_INCLUDED_HPP
#define CommandConversationCall_INCLUDED_HPP

#include "command-interface.h"
#include "main.h"

class CommandConversationCall : public ClientCommandInterface
{
public:
        CommandConversationCall() {}
        ~CommandConversationCall() {}

        void executeHelp();
        String getName() {
                return String("ConversationCall");
        }

        void execute_a(); //answer a call
        void execute_p(); //place a call
        void execute_e(); //end call
        void execute_f(); //forward call
        void execute_m(); //mute/unmute
        void execute_h(); //hold/unhold
        void execute_d(); //dtmf
        void execute_v(); //place a call (enable video)
        void execute_V(); //get devicelist
        void execute_S(); //start send video
        void execute_s(); //stop send video
        void execute_R(); //start receive video
        void execute_r(); //stop receive video
        void execute_x(); //place a call (skypename)
        void execute_n(); //place a call (number)
        void execute_c(); //send custom command to videoengine
        void execute_C(); //send custom command to voiceengine
        void execute_w(); //get video dimensions
        
        void execute_D(); //get devicelist
        void execute_E(); //set device

        void ring(bool enable_video = false); 
        VideoRef get_send_video(); 
        VideoRef get_receive_video(); 
private:

};

#endif //CommandConversationCall_INCLUDED_HPP

