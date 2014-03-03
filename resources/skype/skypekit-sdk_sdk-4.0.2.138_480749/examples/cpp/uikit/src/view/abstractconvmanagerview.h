
#ifndef _ABSTRACTCONVMANAGERVIEW_H
#define _ABSTRACTCONVMANAGERVIEW_H

#include "abstractview.h"

#include <Poco/BasicEvent.h>
#include <Poco/SharedPtr.h>

#include <vector>
#include <string>

#include "model/contactdata.h"

namespace Mvp
{

namespace View
{

/**
 * @class AbstractConvManagerView abstractconvmanagerview.h
 * abstractconvmanagerview.h
 * @brief Abstract base class declaring all methods that ConvManagerViews should
 * implement.
 *
 * This class provides all of the information and interfaces presenters need to
 * work with any ConvManagerView object.  This includes methods to
 * initially set the existing contacts and possible contacts to add.  It also
 * has signals for requesting to set the participant list for example when
 * "Ok" is pressed.
 *
 * @see QtConvManagerView
 */
class AbstractConvManagerView : public AbstractView
{
public:
    /**
     * Destructor.
     */
    virtual ~AbstractConvManagerView(){};

     /**
     * Set the non-participant list for initial display.
     *
     * When the conversation manager presenter is initialized, it asks the
     * model for the list of participants and non-participants, then passes
     * that the non-participants list to the view via this setNonParticipants
     * method.
     *
     * @note Subclasses need to implement this, and are free to cache any
     * data passed in for future use.
     * @note If any previous data was cached it should be cleared at this
     * point.
     *
     * @param nonParticipants a vector of contact data to use initially in the
     * nonParticipants view.
     */
    virtual void setNonParticipants(std::vector<Poco::SharedPtr<
            Mvp::Model::ContactData> > nonParticipants) = 0;

    /**
     * Set the participant list for initial display.
     *
     * When the conversation manager presenter is initialized, it asks the
     * model for the list of participants and non-participants, then passes
     * that the non-participants list to the view via this setParticipants
     * method.
     *
     * @note Subclasses need to implement this, and are free to cache any
     * data passed in for future use.
     * @note If any previous data was cached it should be cleared at this
     * point.
     *
     * @param participants a vector of contact data to use initially in the
     * participants view.
     */
    virtual void setParticipants(std::vector<Poco::SharedPtr<
            Mvp::Model::ContactData> > participants) = 0;

    /**
     * Set the conversation details for this view.
     *
     * The ConvManagerView can handle conversation management for chats and
     * calls, so this method is used to customize the view to describe the
     * conversation it is managing.  A conversation name and type are given
     * and can be used in the view.
     *
     * @param title the title of the conversation.
     * @param type the type of conversation (e.g. "Chat", "Call", etc.).
     */
    virtual void setConversationDetails(std::string &title,
            std::string &type) = 0;

    /**
     * Update a given contact's details (displayname, availability, etc.).
     *
     * Whenever a contact's features change, this method will be called to
     * update the contact's display information.  Any previous information
     * for this contact can be safely forgotten.
     *
     * @param data The new data to use for the given contact.
     *
     * @note The contact's skypeName should never change, so this can be used
     * to safely know which contact to update.
     */
    virtual void updateContact(
            const Poco::SharedPtr<Mvp::Model::ContactData> data) = 0;

    /**
     * A signal to update the participants in the conversation to the list
     * of skypeNames given.
     */
    Poco::BasicEvent<const std::vector<std::string> > updateParticipantList;
};

};

};

#endif

