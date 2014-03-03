#ifndef _HISTORYPRESENTER_H
#define _HISTORYPRESENTER_H

#include "basehistorypresenter.h"
#include "view/abstracthistoryview.h"
#include "model/abstracthistoryservice.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class HistoryPresenter historypresenter.h historypresenter.h
 * @brief Class to present history information using a HistoryService.
 *
 * This class is responsible for connecting the HistoryService to the
 * HistoryView, manipulating the HistoryView (show, hide, etc.) and passing
 * data from one to the other and back.
 *
 * @see AbstractPresenter, AbstractHistoryService and AbstractHistoryView.
 */
class HistoryPresenter : public BaseHistoryPresenter
{
public:
    /**
     * Constructor.
     *
     * @param model AbstractHistoryService to use for this history.
     */
    HistoryPresenter(Mvp::Model::AbstractHistoryService *model);

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    void setView(Mvp::View::AbstractHistoryView *view);

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    void showView();

    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onViewClose(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to new history changed signals from the model.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onHistoryChanged(const void *sender, Poco::EventArgs &args);

private:
    /**
     * Slot to react to requests to begin a chat with a given conversation.
     *
     * When a user double clicks a chat item in the history, chatConversation
     * is sent from the view. This slot handles that signal.
     *
     * @param sender a pointer to the sender of the signal.
     * @param conversationId the id of the conversation to begin the chat with.
     */
    void onChatConversation(const void *sender,
        const std::string &conversationId);

    /**
     * Slot to react to requests to begin a call with a given conversation.
     *
     * When a user double clicks a call item in the history, callConversation
     * is sent from the view.  This slot handles that signal.
     *
     * @param sender a pointer to the sender of the signal.
     * @param conversationId the id of the conversation to call.
     */
    void onCallConversation(const void *sender,
        const std::string &conversationId);

    /**
     * Slot to react to requests to mark history as read.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onMarkHistoryRead(const void *sender, Poco::EventArgs &args);
};

};

};

#endif

