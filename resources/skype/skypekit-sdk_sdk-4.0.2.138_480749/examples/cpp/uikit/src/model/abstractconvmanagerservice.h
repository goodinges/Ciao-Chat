
#ifndef _ABSTRACTCONVMANAGERSERVICE_H
#define _ABSTRACTCONVMANAGERSERVICE_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>
#include <Poco/SharedPtr.h>

#include <vector>

#include "contactdata.h"

namespace Mvp
{

namespace Model
{

/**
 * @class AbstractConvManagerService abstractconvmanagerservice.h
 * abstractconvmanagerservice.h
 * @brief Abstract Conversation Manager Service class.
 *
 * This class provides the base interface that needs to be implemented
 * by the Conversation Manager services, such as, and specifically, getting
 * the conversation's participants, updating the participantlist, getting
 * possible participants and asking skypekit to invite/remove participants
 * based on those chosen by the user in the interface.
 *
 * @note This class is abstract. If you're looking for a concrete one try
 * looking at ConvManagerService class.
 *
 */
class AbstractConvManagerService {
public:
    AbstractConvManagerService(){};
    virtual ~AbstractConvManagerService(){};

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
    virtual std::vector<Poco::SharedPtr<ContactData> > getParticipants() = 0;

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
    virtual std::vector<Poco::SharedPtr<ContactData> > getNonParticipants() = 0;

    /**
     * Add the participant list for the conversation.
     *
     * This method sets the new participants list to the conversation. If the
     * conversation is not a conference yet, it will create and spawn a new
     * conference and then add the new participants to this conversation.
     *
     * @param participantSkypeNames the participants to set on the conversation.
     */
    virtual void setParticipants(
        const std::vector<std::string> &participantSkypeNames) = 0;

    /**
     * React to participant list changes in the conversation.
     *
     * When a conversation's participant list changes, it emits a signal and we
     * then respond to it here.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    virtual void onParticipantListChange(
        const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Signal for contact changes notification.
     *
     * This signal will be emitted whenever a Skype Contact has its properties
     * changed. Thus it acts as a central point for change notifications.
     */
    Poco::BasicEvent<Poco::SharedPtr<ContactData> > contactChanged;

    /**
     * Signal that participants have changed.
     *
     * When the participant list changes, this signal is sent so the view knows
     * it needs to update it's participant and non-participant lists.
     */
    Poco::BasicEvent<Poco::EventArgs> participantsChanged;
};

};

};
#endif

