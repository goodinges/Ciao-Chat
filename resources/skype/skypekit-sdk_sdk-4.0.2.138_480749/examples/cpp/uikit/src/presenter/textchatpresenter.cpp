#include "textchatpresenter.h"

#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

#include "view/abstractchatview.h"

namespace Mvp
{

namespace Presenter
{

TextChatPresenter::TextChatPresenter(Mvp::Model::AbstractChatService *model,
    bool smsMode)
    : BaseChatPresenter(model),
    m_smsMode(smsMode),
    m_viewReady(false)
{
}

void TextChatPresenter::setView(Mvp::View::AbstractChatView *view)
{
    // Give the view to the base class for caching.
    AbstractPresenter<Mvp::View::AbstractChatView>::setView(view);

    // Let the view initialize itself.
    m_view->initialize();

    // Connect to the view's signals.
    m_view->postMessage += Poco::Delegate<TextChatPresenter, std::string>
        (this, &TextChatPresenter::onMessagePost);
    m_view->manage += Poco::Delegate<TextChatPresenter, Poco::EventArgs>
        (this, &TextChatPresenter::onManageConversation);
    m_view->toggleMode += Poco::Delegate<TextChatPresenter, Poco::EventArgs>
        (this, &TextChatPresenter::onToggleMode);
    m_view->leaveChat += Poco::Delegate<TextChatPresenter, Poco::EventArgs>
        (this, &TextChatPresenter::onLeaveChat);
    m_view->messagesConsumed += Poco::Delegate<TextChatPresenter, Poco::EventArgs>
        (this, &TextChatPresenter::onMessagesConsumed);

    m_view->viewClosed += Poco::Delegate<TextChatPresenter, Poco::EventArgs>
        (this, &TextChatPresenter::onViewClose);

    // Update the title in case it has changed.
    m_view->setChatTitle(m_model->chatName());

    // Set the new participant list.
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > participants =
        m_model->getParticipants();
    m_view->setParticipants(participants);

    // Check if we are consumer or not.
    bool consumer = m_model->isConsumer();

    // Enable/disable the view controls based on our permissions.
    m_view->enableControls(consumer);

    // Enable/disable sms toggle button based on canSMS value.
    if (consumer)
        m_view->enableModeToggle(checkSmsSupport(participants));

    // Ask the view to show the leave action if it's already a conference chat.
    m_view->showLeaveAction((participants.size() > 1) && consumer);

    // Populate the message history for this conversation.
    std::vector<Poco::SharedPtr<Mvp::Model::MessageData> > messages =
        m_model->messageHistory(0);

    unsigned int size = messages.size();
    for (unsigned int i = 0; i < size; ++i)
        m_view->addMessage(messages[i]);

    m_view->setSmsMode(m_smsMode);

    // The view can start receiving msgs.
    m_viewReady = true;
}

void TextChatPresenter::showView()
{
    // Show it.
    m_view->showView();
}

void TextChatPresenter::onIncomingMessage(const void *sender,
    Poco::SharedPtr<Mvp::Model::MessageData> &message)
{
    if (!m_view || !m_viewReady)
        return;

    m_view->addMessage(message);
}

void TextChatPresenter::onSmsDelivered(const void *sender, std::string &message)
{
    m_view->reportMessageStatus(Mvp::View::AbstractChatView::SmsDelivered,
        message);
}

void TextChatPresenter::onSmsError(const void *sender,
        std::pair<Sms::FAILUREREASON, std::string> &pair)
{
    // The first element of the pair is the failure reason.
    switch (pair.first) {
        case Sms::MISC_ERROR:
            m_view->reportMessageStatus(
                Mvp::View::AbstractChatView::MiscError, pair.second);
            break;
        case Sms::SERVER_CONNECT_FAILED:
            m_view->reportMessageStatus(
                Mvp::View::AbstractChatView::ServerConnectFailed, pair.second);
            break;
        case Sms::NO_SMS_CAPABILITY:
            m_view->reportMessageStatus(
                Mvp::View::AbstractChatView::NoSmsCapability, pair.second);
            break;
        case Sms::INSUFFICIENT_FUNDS:
            m_view->reportMessageStatus(
                Mvp::View::AbstractChatView::InsufficientFunds, pair.second);
            break;
        case Sms::INVALID_CONFIRMATION_CODE:
            m_view->reportMessageStatus(
                Mvp::View::AbstractChatView::InvalidConfirmationCode,
                pair.second);
            break;
        case Sms::USER_BLOCKED:
            m_view->reportMessageStatus(
                Mvp::View::AbstractChatView::UserBlocked, pair.second);
            break;
        case Sms::IP_BLOCKED:
            m_view->reportMessageStatus(
                Mvp::View::AbstractChatView::IPBlocked, pair.second);
            break;
        case Sms::NODE_BLOCKED:
            m_view->reportMessageStatus(
                Mvp::View::AbstractChatView::NodeBlocked, pair.second);
            break;
        case Sms::NO_SENDERID_CAPABILITY:
            m_view->reportMessageStatus(
                Mvp::View::AbstractChatView::NoSenderIDCapability, pair.second);
            break;
    }
}

void TextChatPresenter::onIMFailed(const void *sender, std::string &message)
{
    // Just report the failure.
    m_view->reportMessageStatus(Mvp::View::AbstractChatView::IMFailed, message);
}

void TextChatPresenter::onParticipantListChanged(const void *sender,
          Poco::EventArgs &args)
{
    printf("Changing participant list in TextChatPresenter.\n");

    // Update the title in case it has changed.
    m_view->setChatTitle(m_model->chatName());

    // Set the new participant list.
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > participants =
        m_model->getParticipants();
    m_view->setParticipants(participants);

    // Check if we are consumer or not.
    bool consumer = m_model->isConsumer();

    // Enable/disable the view controls based on our permissions.
    m_view->enableControls(consumer);

    // Enable/disable sms toggle button based on canSMS value.
    if (consumer)
        m_view->enableModeToggle(checkSmsSupport(participants));

    // Ask the view to show the leave button.
    m_view->showLeaveAction(true);
}

void TextChatPresenter::onManageConversation(const void *sender,
    Poco::EventArgs &args)
{
    // Just forward the signal so the chatstate can deal with it.
    manageConversation(this, args);
}

void TextChatPresenter::onToggleMode(const void *sender, Poco::EventArgs &args)
{
    m_smsMode = !m_smsMode;
    printf("togglemode called, new mode %d.\n", m_smsMode);

    m_view->setSmsMode(m_smsMode);
}

void TextChatPresenter::onLeaveChat(const void *sender, Poco::EventArgs &args)
{
    m_model->leaveChat();
}

void TextChatPresenter::onContactChanged(const void *sender,
     Poco::SharedPtr<Mvp::Model::ContactData> &contact)
{
    // Just update the contact on the view.
    m_view->updateContact(contact);
}

void TextChatPresenter::onViewClose(const void *sender, Poco::EventArgs &args)
{
    finished(this, args);
}

void TextChatPresenter::onMessagePost(const void *sender, std::string &message)
{
    // Ask the model to post the message.
    if (m_smsMode)
        m_model->postSMS(message);
    else
        m_model->postMessage(message);
}

void TextChatPresenter::onMessagesConsumed(const void *sender, Poco::EventArgs &args)
{
    m_model->consumeMessages();
}

bool TextChatPresenter::checkSmsSupport(std::vector<Poco::SharedPtr<
        Mvp::Model::ContactData> > &participants)
{
    bool canSMS = false;
    unsigned int size = participants.size();
    for (unsigned int i = 0; i < size; ++i) {
        if (participants[i]->canSMS) {
            canSMS = true;
            break;
        }
    }

    return canSMS;
}

};

};
