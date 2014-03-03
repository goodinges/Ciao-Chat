#ifndef _CALLSERVICE_H_
#define _CALLSERVICE_H_

#include <string>

#include "abstractcallservice.h"
#include "skypecontact.h"
#include "skypeconversation.h"
#include "skypeparticipant.h"
#include "skypevideo.h"

namespace Mvp
{

namespace Model
{

/**
 * @class CallService callservice.h callservice.h
 * @brief Implements a simple Call Service.
 *
 * This class represents the interaction between a call window and the
 * SkypeKit itself.  All incoming calls on the conversation will be emitted
 * from this object, and all outgoing calls will come from the view into
 * this service to be given to the skypekit.
 *
 * @see AbstractCallService
 */
class CallService : public AbstractCallService
{
public:
    /**
     * Constructor
     *
     * Every CallService needs a conversation to do its work, so we take
     * the ConversationRef as a parameter at construction time.
     *
     * @param conversation the conversation to use for this call.
     */
    CallService(const ConversationRef &conversation);

    /**
     * Destructor, leaves the conversation.
     */
    ~CallService();

    /**
     * Returns the conversation call name.
     *
     * @return the unique conversation id or name.
     */
    std::string callName();

    /**
     * Return the current conversation live status.
     *
     * @return the current conversation live status.
     */
    Conversation::LOCAL_LIVESTATUS liveStatus();

    /**
     * Returns the conversation participant list.
     *
     * @returns a list of contacts currently participating in the call.
     */
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >
        getParticipants();

    /**
     * Accept the Incoming Call.
     */
    void acceptCall();

    /**
     * Reject the Incoming Call.
     */
    void rejectCall();

    /**
     * End the ongoing Call.
     */
    void endCall();

    /**
     * Mute the current ongoing call.
     *
     * @param muted the call is going to be muted or unmuted.
     */
    void mute(bool muted = false);

    /**
     * Put the current call on hold.
     *
     * @param held the call is going to be put on hold or resumed.
     */
    void hold(bool held = false);

    /**
     * Start/Stop streaming the local video.
     *
     * Starts the local video object, so the remote user can start viewing
     * our stream.  Also displays the local video in a preview widget.
     *
     * @param start true to start, false to stop.
     */
    void startLocalVideo(bool start = true);

    /**
     * Start/Stop streaming the remote video.
     *
     * Starts the remote video object.  Note the remote video object should
     * already be in AVAILABLE state for this to work.
     *
     * @param start true to start, false to stop.
     */
    void startRemoteVideo(bool start = true);

    /**
     * Set the local video widget.
     *
     * This method tells skypekit the handle of the widget to use for rendering
     * the local preview video. Skypekit handles all rendering, we just need
     * to tell it which widget to draw to.
     *
     * @param widget the widget to use for local video.
     */
    void setLocalVideoWidget(uint &widget);

    /**
     * Set the remote video widget.
     *
     * This method tells skypekit the handle of the widget to use for rendering
     * the remote video.  Skypekit handles all rendering, we just tell it which
     * widget to draw to.
     *
     * @param widget the widget to use for remote video.
     */
    void setRemoteVideoWidget(uint &widget);

    /**
     * Send a DTMF tone to the conversation.
     *
     * @param dtmf a tone to be sent.
     */
    void sendDTMF(Participant::DTMF dtmf);

    /**
     * Slot to handle conversation status changed signal.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param liveStatus the new conversation live status.
     */
    void onLiveStatusChanged(const void *sender,
        Conversation::LOCAL_LIVESTATUS &liveStatus);

    /**
     * Slot to handle participant list changes.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onParticipantListChanged(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to handle participants joining the call.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onParticipantJoinedCall(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to handle participants leaving the call.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onParticipantLeftCall(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to handle when a conference is spawned from the initial conversation.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param conference the new conversation spawned from the old one.
     */
    void onSpawnedConference(const void *sender,
        const SkypeConversation::Ref &conference);

    /**
     * Slot to handle when a contact changes.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param contact the contactref that has changed.
     */
    void onContactChanged(const void *sender,
        ContactRef &contact);

    /**
     * Slot to handle/pass on the video available signal from the participant.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param available true if video is available, false otherwise.
     */
    void onVideoAvailable(const void *sender, bool &available);

    /**
     * Slot to handle video status changes from the video objects.
     *
     * When a video object's status changes it tells us via this slot.  We
     * therefore need to handle all video status changes in this method.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param status the video's new status.
     */
    void onVideoStatusChanged(const void *sender, Video::STATUS &status);

private:

    void connectToVideo(SkypeVideo::Ref &video);
    void disconnectFromVideo(SkypeVideo::Ref &video);

    void connectToParticipant(SkypeParticipant::Ref &participant);
    void disconnectFromParticipant(SkypeParticipant::Ref &participant);

    void connectToConversation(const ConversationRef &conversation);
    void disconnectFromConversation();

    bool m_isLive;
    SkypeVideo::Ref m_localVideo;
    SkypeVideo::Ref m_previewVideo;
    SkypeVideo::Ref m_remoteVideo;
    std::vector<SkypeParticipant::Ref> m_participants;
    SkypeConversation::Ref m_conversation;
};

};

};

#endif
