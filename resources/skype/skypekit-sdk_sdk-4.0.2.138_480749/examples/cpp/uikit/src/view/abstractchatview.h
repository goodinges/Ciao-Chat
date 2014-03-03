
#ifndef _ABSTRACTCHATVIEW_H
#define _ABSTRACTCHATVIEW_H

#include <string>

#include "abstractview.h"

#include "model/messagedata.h"
#include "model/contactdata.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace View
{

/**
 * @class AbstractChatView abstractchatview.h abstractchatview.h
 * @brief Abstract base class declaring all methods ChatViews should implement.
 *
 * This class provides all of the information and interfaces presenters need to
 * work with any ChatView object.
 *
 * @see QtTextChatView
 */
class AbstractChatView : public AbstractView
{
public:

    enum StatusCode
    {
        IMFailed,
        SmsDelivered,
        MiscError,
        ServerConnectFailed,
        NoSmsCapability,
        InsufficientFunds,
        InvalidConfirmationCode,
        UserBlocked,
        IPBlocked,
        NodeBlocked,
        NoSenderIDCapability,
        GeneralError
    };

    /**
     * Set the title of the chat.
     *
     * @param title the title to show for this chat view.
     */
    virtual void setChatTitle(const std::string &title) = 0;

    /**
     * Add a message to the view.
     *
     * When a new message comes from skype, this method is called to add
     * the message to the chat window in the log area.
     *
     * @param message the message data to show.
     */
    virtual void addMessage(Poco::SharedPtr<Mvp::Model::MessageData> &message) = 0;

    /**
     * Add a message about a status change to the view.
     *
     * @param code the code to display.
     * @param message the message that generated the given status code.
     */
    virtual void reportMessageStatus(StatusCode code, std::string &message) = 0;

    /**
     * Set the participant list.
     *
     * When the chat presenter is initialized, it asks the model  for the list
     * of participants, then passes that list to the view via this
     * setParticipants method. Any time the participant list changes the
     * new list is fetched and passed into this method.
     *
     * @param contacts A vector of contact data to use initially in the view.
     */
    virtual void setParticipants(
            std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts) = 0;

    /**
     * Update the given contact's contact data.
     *
     * @param contact the contact's new contact data to use.
     */
    virtual void updateContact(
        Poco::SharedPtr<Mvp::Model::ContactData> &contact) = 0;

    /**
     * Set the Sms mode.
     *
     * @param smsMode true to use smsMode, false to use IM mode.
     */
    virtual void setSmsMode(bool smsMode) = 0;

    /**
     * Enable or Disable the chat controls.
     *
     * @param enabled true to enable all controls, false to disable.
     */
    virtual void enableControls(bool enabled) = 0;

    /**
    * Enable the sms/im toggle button if sms mode is currently available.
    *
    * @param canSms true if the current conversation allows sending of sms.
    */
    virtual void enableModeToggle(bool canSms) = 0;

    /**
     * Show/hide the leave chat button.
     *
     * @param show true to show the button, false to hide the button.
     */
    virtual void showLeaveAction(bool show) = 0;

    /**
     * Signal to post a message to the chat.
     */
    Poco::BasicEvent<std::string> postMessage;

    /**
     * Signal to notify that the user wants to manage the participants.
     */
    Poco::BasicEvent<Poco::EventArgs> manage;

    /**
     * Signal to notify that the user wants to change modes.
     */
    Poco::BasicEvent<Poco::EventArgs> toggleMode;

    /**
     * Signal to notify that the user wants to leave the current chat.
     */
    Poco::BasicEvent<Poco::EventArgs> leaveChat;

    /**
     * Signal to notify that there're no pending mensages to be displayed and
     * that all of them can be consumed already.
     */
    Poco::BasicEvent<Poco::EventArgs> messagesConsumed;
};

};

};
#endif
