#include "skypeconversation.h"

#include "util.h"

namespace Mvp
{

namespace Model
{

SkypeConversation::SkypeConversation(unsigned int oid, SERootObject *root)
    :Conversation(oid, root),
    m_active(false)
{
}

SkypeConversation::~SkypeConversation()
{
}

void SkypeConversation::OnChange(int prop)
{
    /* Here is the place where you can capture all property changes when they
     * happen. It's important to know that this method will receive ALL changes
     * from ALL properties, which means that it's necessary to create a nice and
     * robust filter inside this method to handle each type of property,
     * otherwise it can fail by acting incorrectly or even lose some events.
     *
     * Another interesting thing is that this method does not receive the new
     * value of the changed property. It's necessary to get the value from the
     * Conversation class itself, because once this method is called the
     * property is already changed in the class.
     */

    /* For our Conversation class we will only watch for local live status
     * changes, that will give us the current status of the call.
     */
    if (prop == Conversation::P_LOCAL_LIVESTATUS) {
        printf("Conversation property %d changed\n.", prop);

        // Get the live status value and emit a signal.
        Conversation::LOCAL_LIVESTATUS liveStatus;
        GetPropLocalLivestatus(liveStatus);

        liveStatusChanged(this, liveStatus);

        if (liveStatus == Conversation::RINGING_FOR_ME) {
            printf("Emitting incomingCall signal\n");
            ConversationRef convRef = this->ref();
            incomingCall(this, convRef);
        }
    }
}

void SkypeConversation::OnParticipantListChange()
{
    printf("SkypeConversation::OnParticipantListChange called.\n");

    Poco::EventArgs args;
    participantListChanged(this, args);
}

void SkypeConversation::OnSpawnConference(const ConversationRef &conference)
{
    spawnedConference(this, conference);
}

void SkypeConversation::OnMessage(const MessageRef &message)
{
    // Get the message type.
    int messageType = message->GetProp(Message::P_TYPE).toInt();

    Poco::SharedPtr<MessageData> data;

    switch (messageType) {
        case Message::POSTED_EMOTE:
        case Message::POSTED_TEXT:
        case Message::POSTED_SMS:
            data = Util::dataFromMessage(message);
            messageReceived(this, data);
            newHistoryItem(this, message);
            break;
        case Message::STARTED_LIVESESSION:
            newHistoryItem(this, message);
            break;
    }
}

bool SkypeConversation::isActive()
{
    return m_active;
}

void SkypeConversation::setActive(bool value)
{
    m_active = value;
}

};

};
