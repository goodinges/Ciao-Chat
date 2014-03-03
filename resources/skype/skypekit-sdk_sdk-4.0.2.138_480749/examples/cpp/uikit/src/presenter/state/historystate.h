
#ifndef _HISTORYSTATE_H
#define _HISTORYSTATE_H

#include "state.h"
#include "skype-embedded_2.h"

namespace Mvp
{

namespace Model
{
class AbstractHistoryService;
};


namespace View
{
class AbstractView;
class AbstractHistoryView;
};

namespace Presenter
{
class HistoryPresenter;


/**
 * @class HistoryState historystate.h historystate.h
 * @brief Implementation of a history state needed to own all history mvp objects.
 *
 * This class implements a history state to show the history of conversations
 * and chats. It also creates and owns a HistoryService, a HistoryPresenter,
 * and an AbstractHistoryView.
 *
 * @see State, IdleState, HistoryService, HistoryPresenter, AbstractHistoryView.
 */
class HistoryState: public State
{
public:
    /**
     * Constructor
     *
     * @param context The context this state belongs to.
     */
    HistoryState(Context &context);

    /**
     * Destructor
     */
    ~HistoryState();

    /**
     * Perform any initialization needed.
     */
    void init();

    /**
     * Get the unique id of this state.
     *
     * @return a string that uniquely identifies this state.
     */
    std::string id() const;

private:
    /**
     * Slot to catch the view created by the viewfactory.
     *
     * View creation is asynchronous so this slot is needed to get
     * the pointer to the view after it is requested by
     * viewFactory->requestView(HistoryView).
     *
     * @param sender a pointer to the object that sent the signal.
     * @param pair a pair containing the request id and the view object.
     *
     * @see AbstractViewFactory::requestView
     *
     * @note the view should be of type AbstractHistoryView.
     */
    void catchView(const void *sender,
        std::pair<int, Mvp::View::AbstractView *> &pair);

    /**
     * Model, Presenter and View needed to display and manipulate history.
     */
    Mvp::Model::AbstractHistoryService *m_service;
    Mvp::Presenter::HistoryPresenter *m_presenter;
    Mvp::View::AbstractHistoryView *m_view;
    int m_viewRequestId;
};

};

};
#endif

