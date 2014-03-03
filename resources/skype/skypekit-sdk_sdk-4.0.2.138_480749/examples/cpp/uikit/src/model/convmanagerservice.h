
#ifndef _CONVMANAGERSERVICE_H
#define _CONVMANAGERSERVICE_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>
#include <Poco/SharedPtr.h>

#include <vector>

#include "abstractconvmanagerservice.h"
#include "contactdata.h"

namespace Mvp
{

namespace Model
{

/**
 * @class ConvManagerService convmanagerservice.h convmanagerservice.h
 * @brief  Conversation Manager Service class.
 *
 * This class provides a concrete implementation of AbstractConvManagerService.
 *
 * @see AbstractConvManagerService.
 *
 */
class ConvManagerService: public AbstractConvManagerService {
public:
    enum Mode {
        Call,
        Chat
    };

    /**
     * Constructor
     *
     * @param conversation the conversation this service should manage.
     */
    ConvManagerService(const ConversationRef &conversation, Mode mode);

    /**
     * Destructor.
     */
    ~ConvManagerService();

    /**
     * Get the list of participants.
     *
     * When requested the method will return a vector containing a set of meta
     * contacts representing each real Skype Contact in the conversation. This
     * meta contact only provides some data about the real contact and is
     * identified by the contact's skypename value.
     *
     * @returns vector<ContactData *> A vector containing meta contacts.
     * @see ContactData
     */
    std::vector<Poco::SharedPtr<ContactData> > getParticipants();

    /**
     * Get the list of possible participants.
     *
     * When requested the method will return a vector containing a set of meta
     * contacts representing each real Skype Contact that could join or be
     * invited into the conversation. This meta contact only provides some data
     * about the real contact and is identified by the contact's skypename
     * value.
     *
     * @returns vector<ContactData *> A vector containing meta contacts.
     * @see ContactData
     */
    std::vector<Poco::SharedPtr<ContactData> > getNonParticipants();

    /**
     * Set the participant list for the conversation.
     *
     * This method sets the new participants list to the conversation. If the
     * conversation is not a conference yet, it will create and spawn a new
     * conference and then add the new participants to this conversation.
     *
     * @param participantSkypeNames the participants that should be set to the
     * conversation.
     */
    void setParticipants(
        const std::vector<std::string> &participantSkypeNames);

    /**
     * React to participant list changes in the conversation.
     *
     * When a conversation's participant list changes, it emits a signal and we
     * then respond to it here.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onParticipantListChange(const void *sender, Poco::EventArgs &args);

private:
    ConversationRef m_conversation;
    Mode m_conversationMode;
};

};

};
#endif

