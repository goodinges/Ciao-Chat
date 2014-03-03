#include "historypresenter.h"

#include <stdio.h>

#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

#include "model/skypecore.h"
#include "view/abstracthistoryview.h"

namespace Mvp
{

namespace Presenter
{

HistoryPresenter::HistoryPresenter(Mvp::Model::AbstractHistoryService *model)
    : BaseHistoryPresenter(model)
{
}

void HistoryPresenter::setView(Mvp::View::AbstractHistoryView *view)
{
    // Give the view to the base class for caching.
    AbstractPresenter<Mvp::View::AbstractHistoryView>::setView(view);

    // Let the view initialize itself.
    m_view->initialize();

    m_view->viewClosed += Poco::Delegate<HistoryPresenter, Poco::EventArgs>
        (this, &HistoryPresenter::onViewClose);
    m_view->chatConversation +=
        Poco::Delegate<HistoryPresenter, const std::string>
        (this, &HistoryPresenter::onChatConversation);
    m_view->callConversation +=
        Poco::Delegate<HistoryPresenter, const std::string>
        (this, &HistoryPresenter::onCallConversation);
    m_view->markHistoryRead +=
        Poco::Delegate<HistoryPresenter, Poco::EventArgs>
        (this, &HistoryPresenter::onMarkHistoryRead);

    bool hasUnread(false);
    std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> > history =
        m_model->getHistory(hasUnread);
    m_view->setHistory(history, hasUnread);
}

void HistoryPresenter::showView()
{
    // Show it.
    m_view->showView();
}

void HistoryPresenter::onViewClose(const void *sender, Poco::EventArgs &args)
{
    finished(this, args);
}

void HistoryPresenter::onHistoryChanged(const void *sender, Poco::EventArgs &args)
{
    bool hasUnread(false);

    // Get the history list and hasUnread value from the model.
    std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> > history =
        m_model->getHistory(hasUnread);

    // Give the history list to the view.
    m_view->setHistory(history, hasUnread);
}

void HistoryPresenter::onChatConversation(
    const void *sender, const std::string &conversationId)
{
    // Request the chat.
    Mvp::Model::SkypeCore::instance()->requestChatByConversation(conversationId);
}

void HistoryPresenter::onCallConversation(
    const void *sender, const std::string &conversationId)
{
    // Request the call.
    Mvp::Model::SkypeCore::instance()->requestCallByConversation(conversationId);
}

void HistoryPresenter::onMarkHistoryRead(const void *sender,
    Poco::EventArgs &args)
{
    // Ask the model to mark the messages as read.
    m_model->markMessagesRead();
}

};

};

