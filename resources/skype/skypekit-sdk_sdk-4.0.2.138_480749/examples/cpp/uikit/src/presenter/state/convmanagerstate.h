
#ifndef _CONVMANAGERSTATE_H
#define _CONVMANAGERSTATE_H

#include "state.h"
#include "skype-embedded_2.h"
#include "model/convmanagerservice.h"

namespace Mvp
{

namespace Model
{
class AbstractConvManagerService;
};


namespace View
{
class AbstractView;
class AbstractConvManagerView;
};

namespace Presenter
{
class ConvManagerPresenter;


/**
 * @class ConvManagerState convmanagerstate.h convmanagerstate.h
 * @brief Implementation of a convmanager state needed to own all mvp objects
 *  for conversation management.
 *
 * Conversation Management refers to adding contacts to the existing
 * conversation, and removing participants from the conversation. This class
 * implements a convmanager state which owns a ConvManagerService, a
 * ConvManagerPresenter, and a ConvManagerView.
 *
 * @see State, ConvManagerService, ConvManagerPresenter,
 * AbstractConvManagerView.
 */
class ConvManagerState: public State
{
public:
    /**
     * Constructor
     *
     * @param context the context this state belongs to.
     * @param conversation the conversation this state is to manage.
     * @param convMode the mode of the conversation to manage.
     */
    ConvManagerState(Context &context, const ConversationRef &conversation,
        Mvp::Model::ConvManagerService::Mode convMode);

    /**
     * Destructor
     */
    ~ConvManagerState();

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
     * viewFactory->requestView(ConvManagerView).
     *
     * @param sender a pointer to the object that sent the signal.
     * @param pair a pair containing the request id and the view object.
     *
     * @see AbstractViewFactory::requestView
     *
     * @note the view should be of type AbstractConvManagerView.
     */
    void catchView(const void *sender,
        std::pair<int, Mvp::View::AbstractView *> &pair);

    ConversationRef m_conversation;

    /**
     * Model, Presenter and View needed to manage a conversation.
     */
    Mvp::Model::AbstractConvManagerService *m_service;
    Mvp::Presenter::ConvManagerPresenter *m_presenter;
    Mvp::View::AbstractConvManagerView *m_view;

    /**
     * The mode of the conversation we are to manage.
     *
     * When asked to add or remove participants from a conversation, we need
     * to know what type of conversation we are managing, because when adding
     * participants to a call, we need to ring them after adding them to the
     * conversation.  We pass this mode into our ConvManagerService when we
     * create it, and it then acts accordingly.
     *
     * @see Mvp::Model::ConvManagerService::Mode
     */
    Mvp::Model::ConvManagerService::Mode m_convMode;

    int m_viewRequestId;
};

};

};

#endif

