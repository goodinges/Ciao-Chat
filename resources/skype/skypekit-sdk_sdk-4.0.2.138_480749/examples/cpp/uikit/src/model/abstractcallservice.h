#ifndef _ABSTRACTCALLSERVICE_H
#define _ABSTRACTCALLSERVICE_H

#include <string>

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

#include "skypeconversation.h"
#include "contactdata.h"

namespace Mvp
{

namespace Model
{

/**
 * @class AbstractCallService abstractcallservice.h abstractcallservice.h
 * @brief Abstract Call Service class.
 *
 * This class provides the base interface that needs to be implemented
 * by the call service, such as, and specifically, getting the call name,
 * the participant list and the current call status.
 *
 * @note This class is abstract. If you're looking for a concrete one
 * try looking at the CallService class.
 *
 */
class AbstractCallService
{
public:
    /**
     * Constructor
     */
    AbstractCallService() {}

    /**
     * Virtual destructor
     */
    virtual ~AbstractCallService() {}

    /**
     * Returns the conversation call name.
     *
     * @return the unique conversation id or name.
     */
    virtual std::string callName() = 0;

    /**
     * Slot to handle the conversation status changed signal.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param liveStatus the new conversation live status.
     */
    virtual void onLiveStatusChanged(const void *sender,
            Conversation::LOCAL_LIVESTATUS &liveStatus) = 0;

    /**
     * Slot to handle participant list changes.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    virtual void onParticipantListChanged(const void *sender,
                                          Poco::EventArgs &args) = 0;

    /**
     * Slot to handle participants joining the call.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    virtual void onParticipantJoinedCall(const void *sender,
        Poco::EventArgs &args) = 0;

    /**
     * Slot to handle participants leaving the call.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    virtual void onParticipantLeftCall(const void *sender,
        Poco::EventArgs &args) = 0;

    /**
     * Slot to handle when a conference is spawned from the initial conversation.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param conference the new conversation spawned from the old one.
     */
    virtual void onSpawnedConference(const void *sender,
        const SkypeConversation::Ref &conference) = 0;

    /**
     * Return the current conversation live status.
     *
     * @return the current conversation live status.
     */
    virtual Conversation::LOCAL_LIVESTATUS liveStatus() = 0;

    /**
     * Returns the conversation participants list.
     *
     * @returns a list of contacts currently participating in the call.
     */
    virtual std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >
        getParticipants() = 0;

    /**
     * Accept the Incoming Call.
     */
    virtual void acceptCall() = 0;

    /**
     * Reject the Incoming Call.
     */
    virtual void rejectCall() = 0;

    /**
     * End the ongoing Call.
     */
    virtual void endCall() = 0;

    /**
     * Mute the current ongoing call.
     *
     * @param muted the call is going to be muted or unmuted.
     */
    virtual void mute(bool muted = false) = 0;

    /**
     * Put the current call on hold.
     *
     * @param held the call is going to be put on hold or resumed.
     */
    virtual void hold(bool held = false) = 0;

    /**
     * Start/Stop streaming the local video.
     *
     * @param start true to start, false to stop.
     */
    virtual void startLocalVideo(bool start = true) = 0;

    /**
     * Start/Stop streaming the remote video.
     *
     * @param start true to start, false to stop.
     */
    virtual void startRemoteVideo(bool start = true) = 0;

    /**
     * Set the local video widget.
     *
     * @param widget the widget to use for local video.
     */
    virtual void setLocalVideoWidget(uint &widget) = 0;

    /**
     * Set the remote video widget.
     *
     * @param widget the widget to use for remote video.
     */
    virtual void setRemoteVideoWidget(uint &widget) = 0;

    /**
     * Send a DTMF tone to the conversation.
     *
     * @param dtmf a tone to be sent.
     */
    virtual void sendDTMF(Participant::DTMF dtmf) = 0;

    /**
     * Signal that the live status has changed.
     */
    Poco::BasicEvent<Conversation::LOCAL_LIVESTATUS> liveStatusChanged;

    /**
     * Signal that the participant list has changed.
     */
    Poco::BasicEvent<Poco::EventArgs> participantListChanged;

    /**
     * Signal for contact changes notification.
     *
     * This signal will be emitted whenever a Skype Contact has its properties
     * changed. Thus it acts as a central point for change notifications.
     *
     * @note: The new contact data is passed in the signal.
     * @note: Only the participants of the call are watched for changes here.
     */
    Poco::BasicEvent<Poco::SharedPtr<ContactData> > contactChanged;

    /**
     * Signal that the current conversation object has changed.
     *
     * As the Conversation object can change during the Call Service lifetime,
     * it's necessary to have an signal responsible to propagate the new object
     * to other classes.
     *
     * @note: The new conversation is passed in the signal.
     */
    Poco::BasicEvent<const ConversationRef> conversationChanged;

    /**
     * Signal that local video is available.
     *
     * When the local video is available, this signal must be emitted
     * so the user will have a chance to start sending his local video to the
     * other participant.
     *
     * @note: The parameter is true when video is newly available, false
     * when it's no longer available.
     */
    Poco::BasicEvent<bool> localVideoAvailable;

    /**
     * Signal that remote video is available.
     *
     * When the remote video is available, this signal must be emitted
     * so the user will have a chance to start viewing the remote participant's
     * video.
     *
     * @note: The parameter is true when video is newly available, false
     * when it's no longer available.
     */
    Poco::BasicEvent<bool> remoteVideoAvailable;
};

};

};
#endif
