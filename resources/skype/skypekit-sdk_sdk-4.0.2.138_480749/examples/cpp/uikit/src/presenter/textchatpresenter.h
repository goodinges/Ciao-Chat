#ifndef _TEXTCHATPRESENTER_H
#define _TEXTCHATPRESENTER_H

#include "basechatpresenter.h"
#include "view/abstractchatview.h"
#include "model/abstractchatservice.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class TextChatPresenter textchatpresenter.h textchatpresenter.h
 * @brief Class to present chat information using TextChatService.
 *
 * This class is responsible for connecting the ChatService to the ChatView,
 * manipulating the ChatView (show, hide, etc.) and passing data from one
 * to the other and back.
 *
 * @see AbstractPresenter, AbstractChatService and AbstractChatView.
 */
class TextChatPresenter : public BaseChatPresenter
{
public:
    /**
     * Constructor.
     *
     * @param model AbstractChatService to use for this text chat.
     * @param smsMode true to start in sms mode, false for im mode.
     */
    TextChatPresenter(Mvp::Model::AbstractChatService *model,
        bool smsMode = false);

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    void setView(Mvp::View::AbstractChatView *view);

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    void showView();

    /**
     * Slot to react to incoming messages from the conversation.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message the message information of the incoming message.
     */
    void onIncomingMessage(const void *sender,
        Poco::SharedPtr<Mvp::Model::MessageData> &message);

    /**
     * Slot to react to sms delivered messages from the model.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message the message that was delivered
     */
    void onSmsDelivered(const void *sender, std::string &message);

    /**
     * Slot to react to sms error messages from the model.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param pair the reason and message of the sms failure.
     */
    void onSmsError(const void *sender,
        std::pair<Sms::FAILUREREASON, std::string> &pair);

    /**
     * Slot to react to imFailed messages from the model.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message the text of the message that failed.
     */
    void onIMFailed(const void *sender, std::string &message);

    /**
     * Slot to handle requests to manage the conversation.
     *
     * @param sender a pointer to the object that sent  the signal.
     */
    void onManageConversation(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to handle requests to toggle the mode.
     *
     * @param sender a pointer to the object that sent  the signal.
     */
    void onToggleMode(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to handle requests to leave the chat.
     *
     * @param sender a pointer to the object that sent  the signal.
     */

    void onLeaveChat(const void *sender, Poco::EventArgs &args);
    /**
     * Slot to handle participant list changes.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onParticipantListChanged(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to handle contact changes.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param contact the new contact data to use.
     */
    void onContactChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact);

private:
    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onViewClose(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to postMessage signal from the view.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message a string containing the message posted.
     */
    void onMessagePost(const void *sender, std::string &message);

    /**
     * Slot to react to messagesConsumed signal from the view.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onMessagesConsumed(const void *sender, Poco::EventArgs &args);

    /**
     * Helper method to determine if we should support Sms mode.
     *
     * The method will go through the participant list checking if one of them
     * has the needed data to enable the SMS capability.
     *
     * @param participants the list of participants to be verified.
     *
     * returns true if one of the participants has the cap, or false if don't.
     */
    bool checkSmsSupport(std::vector<Poco::SharedPtr<
            Mvp::Model::ContactData> > &participants);

    bool m_smsMode;
    bool m_viewReady;

};

};

};
#endif

