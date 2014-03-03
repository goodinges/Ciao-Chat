#include "callservice.h"

#include "skypecore.h"
#include "util.h"

#include "skype-embedded_2.h"

#include <Poco/Delegate.h>

using Poco::Delegate;

namespace Mvp
{

namespace Model
{

CallService::CallService(const ConversationRef &conversation)
    : m_localVideo(0),
    m_conversation(conversation)
{
	m_previewVideo = 0;
	m_remoteVideo = 0;
    // Check if this conversation is already live somehow.
    SEString convOid;
    SEString liveOid = SkypeCore::instance()->findLiveSession();

    m_conversation->GetPropIdentity(convOid);

    if (convOid == liveOid) {
        printf("Alright, we are live! %s\n", convOid.data());
        m_isLive = true;
    }

    connectToConversation(m_conversation);
}

CallService::~CallService()
{
    unsigned int size = m_participants.size();
    for (unsigned int i = 0; i < size; ++i)
        disconnectFromParticipant(m_participants[i]);

    m_participants.clear();

    if (m_localVideo)
        disconnectFromVideo(m_localVideo);

    if (m_remoteVideo)
        disconnectFromVideo(m_remoteVideo);

    disconnectFromConversation();

    // Leave the conversation.
    if (m_isLive) {
        // In case the conversation still alive it leaves the session.
        m_conversation->LeaveLiveSession();

        // Mark the service as dead.
        m_isLive = false;
    }
}

void CallService::connectToConversation(const ConversationRef &conversation)
{
    SkypeConversation::Ref skypeConversation(conversation);

    // Connect liveStatusChanged signal.
    skypeConversation->liveStatusChanged +=
        Poco::Delegate<AbstractCallService, Conversation::LOCAL_LIVESTATUS>
        (this, &AbstractCallService::onLiveStatusChanged);

    // Connect participantListChanged signal.
    skypeConversation->participantListChanged +=
        Poco::Delegate<AbstractCallService, Poco::EventArgs>
        (this, &AbstractCallService::onParticipantListChanged);

    // Connect spawnedConference signal.
    skypeConversation->spawnedConference +=
        Poco::Delegate<AbstractCallService, const SkypeConversation::Ref>
        (this, &AbstractCallService::onSpawnedConference);
}

void CallService::disconnectFromConversation()
{
    // Disconnect liveStatusChanged signal.
    m_conversation->liveStatusChanged -=
        Poco::Delegate<AbstractCallService, Conversation::LOCAL_LIVESTATUS>
        (this, &AbstractCallService::onLiveStatusChanged);

    // Disconnect participantListChanged signal.
    m_conversation->participantListChanged -=
        Poco::Delegate<AbstractCallService, Poco::EventArgs>
        (this, &AbstractCallService::onParticipantListChanged);

    // Disconnect spawnedConference signal.
    m_conversation->spawnedConference -=
        Poco::Delegate<AbstractCallService, const SkypeConversation::Ref>
        (this, &AbstractCallService::onSpawnedConference);
}

void CallService::connectToParticipant(SkypeParticipant::Ref &participant)
{
    // Get the ContactRef for the participant.
    SkypeContact::Ref contact = Util::contactFromParticipant(participant);

    // Connect to the contactChanged signal.
    contact->contactChanged +=
        Poco::Delegate<CallService, ContactRef>
        (this, &CallService::onContactChanged);

    // Connect to the joinedCall signal.
    participant->joinedCall +=
        Poco::Delegate<CallService, Poco::EventArgs>
        (this, &AbstractCallService::onParticipantJoinedCall);

    // Connect to the leftCall signal.
    participant->leftCall +=
        Poco::Delegate<CallService, Poco::EventArgs>
        (this, &AbstractCallService::onParticipantLeftCall);

    // Connect to videoAvailable signal.
    participant->videoAvailable +=
        Poco::Delegate<CallService, bool>
        (this, &CallService::onVideoAvailable);
}

void CallService::disconnectFromParticipant(SkypeParticipant::Ref &participant)
{
    // Get the ContactRef for the participant.
    SkypeContact::Ref contact = Util::contactFromParticipant(participant);

    if (contact.present()) {
        // Disconnect from the contactChanged signal.
        contact->contactChanged -=
            Poco::Delegate<CallService, ContactRef>
            (this, &CallService::onContactChanged);
    }

    // Disconnect from the joinedCall signal.
    participant->joinedCall -=
        Poco::Delegate<CallService, Poco::EventArgs>
        (this, &AbstractCallService::onParticipantJoinedCall);

    // Disconnect to the leftCall signal.
    participant->leftCall -=
        Poco::Delegate<CallService, Poco::EventArgs>
        (this, &AbstractCallService::onParticipantLeftCall);

    // Connect to videoAvailable signal.
    participant->videoAvailable -=
        Poco::Delegate<CallService, bool>
        (this, &CallService::onVideoAvailable);
}

void CallService::connectToVideo(SkypeVideo::Ref &video)
{
    video->statusChanged +=
        Poco::Delegate<CallService, Video::STATUS>
        (this, &CallService::onVideoStatusChanged);
}

void CallService::disconnectFromVideo(SkypeVideo::Ref &video)
{
    video->statusChanged -=
        Poco::Delegate<CallService, Video::STATUS>
        (this, &CallService::onVideoStatusChanged);
}

std::string CallService::callName()
{
    // Get the Display name of the conversation.
    SEString name;
    m_conversation->GetPropDisplayname(name);

    return std::string(name.data());
}

Conversation::LOCAL_LIVESTATUS CallService::liveStatus()
{
    // Get the live status from the conversation.
    Conversation::LOCAL_LIVESTATUS status;
    m_conversation->GetPropLocalLivestatus(status);

    return status;
}

std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >
    CallService::getParticipants()
{
    // Create the contact's container.
    std::vector<Poco::SharedPtr<ContactData> > contacts;

    // Gets the list of participants.
    ParticipantRefs callerList;
    m_conversation->GetParticipants(callerList, Conversation::CONSUMERS);

    unsigned int size = m_participants.size();
    for (unsigned int i = 0; i < size; ++i)
        disconnectFromParticipant(m_participants[i]);

    m_participants.clear();

    size = callerList.size();
    for (unsigned int i = 0; i < size; ++i) {
        // Get the SkypeParticipant.
        SkypeParticipant::Ref participant(callerList[i]);

        // Connect to the participant.
        connectToParticipant(participant);

        // Add the participant to the cache.
        m_participants.push_back(participant);

        // If the participant is live, add it to the list.
        Participant::VOICE_STATUS status;
        participant->GetPropVoiceStatus(status);
        if (participant->isLive()) {
            // Get the contact for this participant.
            SkypeContact::Ref contact =
                Util::contactFromParticipant(callerList[i]);

            // Convert contact in ContactData structure.
            Poco::SharedPtr<ContactData> newData(Util::dataFromContact(
                contact));

            // Add to the container.
            contacts.push_back(newData);
        }
    }

    return contacts;
}

void CallService::acceptCall()
{
    Conversation::LOCAL_LIVESTATUS status = liveStatus();

    /* Before accepting the call we need to check if the call
     * is ready to be accepted.
     */
    if (status == Conversation::RINGING_FOR_ME ||
        status == Conversation::STARTING) {
        m_conversation->JoinLiveSession();
    }
}

void CallService::rejectCall()
{
    Conversation::LOCAL_LIVESTATUS status = liveStatus();

    /* Before rejecting the call we need to check if the call
     * is ready to be rejected.
     */
    if (status == Conversation::RINGING_FOR_ME ||
        status == Conversation::STARTING) {
        m_conversation->LeaveLiveSession();
    }
}

void CallService::endCall()
{
    Conversation::LOCAL_LIVESTATUS status = liveStatus();

    /* Before ending the call we need to check if the call
     * is ready to be finished.
     */
    if (status == Conversation::IM_LIVE ||
        status == Conversation::STARTING ||
        status == Conversation::ON_HOLD_LOCALLY ||
        status == Conversation::ON_HOLD_REMOTELY) {
        m_conversation->LeaveLiveSession();
    }
}

void CallService::mute(bool muted)
{
    bool isMuted = false;

    m_conversation->GetPropLiveIsMuted(isMuted);

    Conversation::LOCAL_LIVESTATUS status = liveStatus();

    /* Before muting the call we need to check if the call
     * is ready muted.
     */
    if (status == Conversation::IM_LIVE &&
        isMuted != muted) {
        if (muted)
            m_conversation->MuteMyMicrophone();
        else
            m_conversation->UnmuteMyMicrophone();
    }
}

void CallService::startLocalVideo(bool start)
{
    Video::STATUS status;
    m_localVideo->GetPropStatus(status);

    // First check if the video has already started.
    if (start && (status != Video::STARTING && status != Video::RUNNING)) {
        printf("Starting local video.\n");

        // Start local video so the other end can see us.
        m_localVideo->Start();

        // Start our preview video so we can see ourself.
        if (m_previewVideo)
            m_previewVideo->Start();
    }
    // Stop video if asked to stop.
    else if (!start) {
        printf("Stopping local video.\n");

        // Stop sending video.
        m_localVideo->Stop();

        // Stop the local preview also.
        if (m_previewVideo)
            m_previewVideo->Stop();
    }
}

void CallService::startRemoteVideo(bool start)
{
    Video::STATUS status;
    m_remoteVideo->GetPropStatus(status);

    // First check if the video has already started.
    if (start && (status != Video::STARTING && status != Video::RUNNING)) {
        printf("Starting remote video.\n");

        m_remoteVideo->Start();
    }
    // Stop video if asked to stop.
    else if (!start) {
        printf("Stopping remote video.\n");

        m_remoteVideo->Stop();
    }
}

void CallService::hold(bool held)
{
    Conversation::LOCAL_LIVESTATUS status = liveStatus();

    /* Before holding the call we need to check if the call
     * is not on this state already.
     */
    if ((status == Conversation::IM_LIVE ||
        status == Conversation::STARTING) &&
        held == true) {
        m_conversation->HoldMyLiveSession();
    }
    else if (status == Conversation::ON_HOLD_LOCALLY &&
             held == false) {
        m_conversation->ResumeMyLiveSession();
    }
}

void CallService::onLiveStatusChanged(const void *sender,
        Conversation::LOCAL_LIVESTATUS &liveStatus)
{
    switch (liveStatus) {
        case Conversation::STARTING:
        case Conversation::RINGING_FOR_ME:
        case Conversation::IM_LIVE:
        case Conversation::ON_HOLD_LOCALLY:
        case Conversation::ON_HOLD_REMOTELY:
            m_isLive = true;
            break;
        case Conversation::NONE:
        case Conversation::RECENTLY_LIVE:
            m_isLive = false;
		default:
            break;
    }

    // Propagate the conversation live status to other objects.
    liveStatusChanged(this, liveStatus);
}

void CallService::onVideoAvailable(const void *sender, bool &available)
{
    /* This method is called when a video object becomes available for a
     * participant.  It will be either the local participant (ourself) or the
     * other participant.  When this is called we are able to get the video
     * object from the particiant.  We cache these in m_localVideo and
     * m_remoteVideo for use later on.  We also cache m_previewVideo which is
     * how local video device to widget preview video can be started
     * and stopped. It's important to note that this is not the same as the
     * local video object which controls starting and stopping sending video
     * to the other participant.
     */

    // We need to know the participant that sent the signal.
    SkypeParticipant *participant = const_cast<SkypeParticipant *>(
            reinterpret_cast<const SkypeParticipant *>(sender));

    // If it's our participant, the video is local.
    if (participant->isLocalParticipant()) {
        // Get the local video if we don't already have it.
        if (!m_localVideo) {
            participant->GetVideo(m_localVideo);

            // And connect to it so we'll know when it changes status.
            connectToVideo(m_localVideo);
        }

        // Get it's initial status.
        Video::STATUS status;
        m_localVideo->GetPropStatus(status);

        // If it's already available, emit localVideoAvailable signal.
        if (status == Video::AVAILABLE)
            localVideoAvailable(this, available);
    }
    // Otherwise it's the remote participant's video.
    else {
        // Get the remote video if we don't already have it.
        if (!m_remoteVideo) {
            participant->GetVideo(m_remoteVideo);

            // And connect to it so we'll know when it changes status.
            connectToVideo(m_remoteVideo);
        }

        // Get initial video status.
        Video::STATUS status;
        m_remoteVideo->GetPropStatus(status);

        // If it's already available, emit remoteVideoAvailable signal.
        if (status == Video::AVAILABLE)
            remoteVideoAvailable(this, available);
    }
}

void CallService::onVideoStatusChanged(const void *sender, Video::STATUS &status)
{
    /* This method is called whenever a video object's status has changed, so
     * we need to see which video sent the signal and emit localVideoAvailable
     * or remoteVideoAvailable signals as necessary.
     */

    // First get the video object that sent the signal.
    SkypeVideo *video = const_cast<SkypeVideo *>(
        reinterpret_cast<const SkypeVideo *>(sender));

    bool availability = true;

    // Make a pointer to the signal to emit.
    Poco::BasicEvent<bool>* videoAvailable = 0;

    // Set up the signal pointer based on if the video is the local video or not.
    if (video->getOID() == m_localVideo->getOID())
        videoAvailable = &localVideoAvailable;
    else
        videoAvailable = &remoteVideoAvailable;

    // Check the video status passed in and emit the signal.
    switch (status) {
        case Video::AVAILABLE:
        case Video::RUNNING:
            // We initialized availability to true, so just emit.
            videoAvailable->notify(this, availability);
            break;
        case Video::NOT_AVAILABLE:
        case Video::NOT_STARTED:
        case Video::REJECTED:
            // Switch availability to false, then emit the signal.
            availability = false;
            videoAvailable->notify(this, availability);
		default:
            break;
    }
}

void CallService::setLocalVideoWidget(uint &widget)
{
    SkypeCore *skype = SkypeCore::instance();

    /* At this point we need to get the preview video device from skype so we
     * will be able to start and show the preview in our local video widget.
     *
     * An interesting thing about the SkypeKit API's video objects is that
     * local video has two separate video objects.  One comes from the
     * participant and is used to start and stop streaming the local video to
     * the remote participant.  This should not be used to try to render local
     * video locally however.  The second Video object is used for rendering
     * local video in a as a preview of what the other person will see.  This
     * preview Video object is gotten from the Skype class (SkypeCore in our
     * case) via the GetPreviewVideo method.
     */
    if (!m_previewVideo) {
        SEStringList deviceNames;
        SEStringList deviceIds;
        uint deviceCount;
        skype->GetAvailableVideoDevices(deviceNames, deviceIds, deviceCount);

        // Assume deviceNames[0] and deviceIds[0] are valid.
        skype->GetPreviewVideo(Video::MEDIA_VIDEO, m_previewVideo,
            deviceNames[0], deviceIds[0]);
    }

    /* Here we set the screen to use for the preview video.  Note we are not
     * setting m_localVideo's screen, as that Video object is just to control
     * our sending of video to the other participant.  m_previewVideo is
     * the Video object responsible for rendering what's coming from our camera
     * locally.
     */
    if (m_previewVideo) {
        printf("Setting the local screen to use for preview video.%d.\n", widget);
        m_previewVideo->SetScreen(widget);
        m_previewVideo->SetRenderRectangle(0, 0, 160, 120);
    }
}

void CallService::setRemoteVideoWidget(uint &widget)
{
    // Here we tell SkypeKit the widget to use when rendering the remote video.
    if (m_remoteVideo) {
        printf("Setting the remote screen to use for video.\n");
        m_remoteVideo->SetScreen(widget);
        m_remoteVideo->SetRenderRectangle(0, 0, 160, 120);
    }
}

void CallService::sendDTMF(Participant::DTMF dtmf)
{
    // Send the tone to the conversation.
    m_conversation->SendDTMF(dtmf);
}

void CallService::onParticipantListChanged(const void *sender,
       Poco::EventArgs &args)
{
    printf("CallService::onParticipantListChanged called.\n");

    // Check if we are the live conversation.
    Conversation::LOCAL_LIVESTATUS status = liveStatus();

    // If so, check if any of the participants are not online.
    if (status == Conversation::IM_LIVE) {
        // First get the list of participants.
        ParticipantRefs callerList;
        m_conversation->GetParticipants(callerList,
            Conversation::OTHER_CONSUMERS);
        printf(" got %zu participants.\n", callerList.size());

        // Then iterate over them
        unsigned int size = callerList.size();
        for (unsigned int i = 0; i < size; ++i) {
            // Ring the not live participants.
            Participant::VOICE_STATUS voiceStatus;
            callerList[i]->GetPropVoiceStatus(voiceStatus);

            printf(" got a participant with voice status %d.\n", voiceStatus);

            if (voiceStatus == Participant::VOICE_AVAILABLE ||
                voiceStatus == Participant::VOICE_UNKNOWN)
                callerList[i]->Ring();
        }
    }

    // Send the signal on.
    participantListChanged(this, args);
}

void CallService::onSpawnedConference(const void *sender,
        const SkypeConversation::Ref &conference)
{
    // Disconnect from old conversation.
    disconnectFromConversation();

    m_conversation = conference;

    // Connect to new conversation.
    connectToConversation(m_conversation);

    // Signal to other objects that the conversation has changed.
    conversationChanged(this, m_conversation);

    /* We need to notify the other objects about the current live status, since
     * it can be changed during the conference spawning and they would miss it.
     */
    Conversation::LOCAL_LIVESTATUS status = liveStatus();
    liveStatusChanged(this, status);

    Poco::EventArgs args;
    participantListChanged(this, args);
}

void CallService::onParticipantJoinedCall(const void *sender,
        Poco::EventArgs &args)
{
    participantListChanged(this, args);
}

void CallService::onParticipantLeftCall(const void *sender,
        Poco::EventArgs &args)
{
    participantListChanged(this, args);
}

void CallService::onContactChanged(const void *sender,
        ContactRef &contact)
{
    Poco::SharedPtr<Mvp::Model::ContactData> data =
        Util::dataFromContact(contact);
    contactChanged(this, data);
}

};

};
