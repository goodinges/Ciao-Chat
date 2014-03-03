#ifndef _SKYPECONVERSATION_H
#define _SKYPECONVERSATION_H

#include "skype-embedded_2.h"

#include "messagedata.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>
#include <Poco/SharedPtr.h>

namespace Mvp
{

namespace Model
{

/**
 * @class SkypeConversation skypeconversation.h skypeconversation.h
 * @brief Implements basic features of the SkypeKit Conversation class.
 *
 * In order to intercept the conversation's incoming messages we need to subclass
 * the Conversation class and overwrite OnMessage that is called when messages
 * come in.
 *
 * @see TextChatService, AbstractChatService
 */
class SkypeConversation : public Conversation
{
public:
    typedef DRef<SkypeConversation, Conversation> Ref;

    /**
     * Destructor.
     */
    virtual ~SkypeConversation();

    /**
     * Accessor for m_active.
     *
     * A Conversation is "active" if it has a ChatState already.  SkypeCore
     * uses this attribute to decide whether or not to emit newIncomingChat.
     * newIncomingChat triggers a new ChatState and chat window creation.
     *
     * @see SkypeConversation::OnMessage
     *
     * @returns the value of m_active.
     */
    bool isActive();

    /**
     * Accessor to set m_active.
     *
     * @param value to set m_active to.
     */
    void setActive(bool value);

    /**
     * Signal that a message was received.
     *
     * When a message is received OnMessage copies the important data to
     * an object of type MessageData, and sends it on this signal.
     *
     * @see SkypeConversation::OnMessage
     */
    Poco::BasicEvent<Poco::SharedPtr<MessageData> > messageReceived;

    /**
     * Signal that the live status has changed.
     */
    Poco::BasicEvent<Conversation::LOCAL_LIVESTATUS> liveStatusChanged;

    /**
     * Signal that a new incoming call has arrived to this conversation.
     */
    Poco::BasicEvent<ConversationRef> incomingCall;

    /**
     * Signal that the participant list has changed.
     */
    Poco::BasicEvent<Poco::EventArgs> participantListChanged;

    /**
     * Signal that the conversation has spawned a conference.
     */
    Poco::BasicEvent<const SkypeConversation::Ref> spawnedConference;

    /**
     * Signal that a new history item message has appeared.
     */
    Poco::BasicEvent<const MessageRef> newHistoryItem;

protected:
    /**
     * Constructor
     */
    SkypeConversation(unsigned int oid, SERootObject *root);

    /**
     * Override the OnChange method to handle conversation change notifications.
     *
     * @param prop the property that changed.
     */
    void OnChange(int prop);

    /**
     * Override the OnMessage method to handle incoming messages.
     *
     * When a message is received this method copies the important data to a
     * MessageData structure, and emits messageReceived.
     *
     * @see SkypeConversation::messageReceived
     *
     * @param message the message coming in.
     */
    void OnMessage(const MessageRef &message);

    /**
     * Override the OnParticipantListChange method to handle changes.
     *
     * When the participant list changes we need to send a signal so the
     * services connected to us can react accordingly.
     */
    void OnParticipantListChange();

    /**
     * Override the OnSpawnConference method to handle conference spawning.
     *
     * When a Conversation becomes a Conference (because other contacts are
     * added to the conversation), this method will be called.  We will use
     * this method to send a signal that a Conference was spawned from this
     * Conversation.
     *
     * @param conference the conference that has been spawned.
     */
    void OnSpawnConference(const ConversationRef &conference);

private:
    friend class SkypeCore;

    bool m_active;
};

};

};

#endif
