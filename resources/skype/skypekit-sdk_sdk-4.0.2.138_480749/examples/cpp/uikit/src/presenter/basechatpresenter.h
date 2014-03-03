#ifndef _BASECHATPRESENTER_H
#define _BASECHATPRESENTER_H

#include "abstractpresenter.h"
#include "model/abstractchatservice.h"
#include "model/messagedata.h"
#include "view/abstractchatview.h"

#include <Poco/BasicEvent.h>
#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class BaseChatPresenter basechatpresenter.h basechatpresenter.h
 * @brief Base class to define the interface for ChatPresenters.
 *
 * ChatPresenter is responsible for connecting the ChatService to the
 * ChatView, manipulating the ChatView (show, hide, etc.) and passing
 * data from one to the other and back.
 *
 * This class is a base class that does the connecting of poco signals to slots.
 *
 * @see AbstractPresenter, AbstractChatService and AbstractChatView.
 */
class BaseChatPresenter : public AbstractPresenter<Mvp::View::AbstractChatView>
{
public:
    /**
     * Constructor.
     *
     * Takes the model to use for this presenter.
     *
     * @param model AbstractChatService to use for this text chat.
     */
    BaseChatPresenter(Mvp::Model::AbstractChatService *model)
        :m_model(model)
    {
        // Connect the model's signals to our slots.
        m_model->messageReceived +=
            Poco::Delegate<BaseChatPresenter,
            Poco::SharedPtr<Mvp::Model::MessageData> >
            (this, &BaseChatPresenter::onIncomingMessage);

        m_model->smsDelivered +=
            Poco::Delegate<BaseChatPresenter, std::string>
            (this, &BaseChatPresenter::onSmsDelivered);

        m_model->smsError +=
            Poco::Delegate<BaseChatPresenter,
            std::pair<Sms::FAILUREREASON, std::string> >
            (this, &BaseChatPresenter::onSmsError);

        m_model->imFailed +=
            Poco::Delegate<BaseChatPresenter, std::string>
            (this, &BaseChatPresenter::onIMFailed);

        m_model->participantListChanged +=
            Poco::Delegate<BaseChatPresenter, Poco::EventArgs>
            (this, &BaseChatPresenter::onParticipantListChanged);

        m_model->contactChanged +=
            Poco::Delegate<BaseChatPresenter,
            Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseChatPresenter::onContactChanged);
    }

    /**
     * Destructor.
     */
    virtual ~BaseChatPresenter()
    {
        // Disonnect the model's signals from our slots.
        m_model->messageReceived -=
            Poco::Delegate<BaseChatPresenter,
            Poco::SharedPtr<Mvp::Model::MessageData> >
            (this, &BaseChatPresenter::onIncomingMessage);

        m_model->smsDelivered -=
            Poco::Delegate<BaseChatPresenter, std::string>
            (this, &BaseChatPresenter::onSmsDelivered);

        m_model->smsError -=
            Poco::Delegate<BaseChatPresenter,
            std::pair<Sms::FAILUREREASON, std::string> >
            (this, &BaseChatPresenter::onSmsError);

        m_model->imFailed -=
            Poco::Delegate<BaseChatPresenter, std::string>
            (this, &BaseChatPresenter::onIMFailed);

        m_model->participantListChanged -=
            Poco::Delegate<BaseChatPresenter, Poco::EventArgs>
            (this, &BaseChatPresenter::onParticipantListChanged);

        m_model->contactChanged -=
            Poco::Delegate<BaseChatPresenter,
            Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseChatPresenter::onContactChanged);
    }

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    virtual void setView(Mvp::View::AbstractChatView *view) = 0;

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    virtual void showView() = 0;

    /**
     * Slot to react to incoming messages from the conversation.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message the message information of the incoming message.
     */
    virtual void onIncomingMessage(const void *sender,
        Poco::SharedPtr<Mvp::Model::MessageData> &message) = 0;

    /**
     * Slot to react to sms delivered messages from the model.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message the message that was delivered.
     */
    virtual void onSmsDelivered(const void *sender, std::string &message) = 0;

    /**
     * Slot to react to sms error messages from the model.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param pair the reason and message of the sms failure.
     */
    virtual void onSmsError(const void *sender,
        std::pair<Sms::FAILUREREASON, std::string> &pair) = 0;

    /**
     * Slot to react to imFailed messages from the model.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message the text of the message that failed.
     */
    virtual void onIMFailed(const void *sender, std::string &message) = 0;

    /**
     * Slot to handle requests to manage the conversation.
     *
     * @param sender a pointer to the object that sent  the signal.
     */
    virtual void onManageConversation(const void *sender,
          Poco::EventArgs &args) = 0;

    /**
     * Slot to handle participant list changes.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    virtual void onParticipantListChanged(const void *sender,
          Poco::EventArgs &args) = 0;

    /**
     * Slot to handle contact changes.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param contact the new contact data to use.
     */
    virtual void onContactChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact) = 0;

    /**
     * Signal that the user has requested to manage the conversation.
     */
    Poco::BasicEvent<Poco::EventArgs> manageConversation;

protected:
    /**
     * Slot to react to postMessage signal from the view.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param message a string containing the message posted.
     */
    virtual void onMessagePost(const void *sender, std::string &message) = 0;

    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onViewClose(const void *sender, Poco::EventArgs &args) = 0;

    Mvp::Model::AbstractChatService *m_model;
};

};

};
#endif
