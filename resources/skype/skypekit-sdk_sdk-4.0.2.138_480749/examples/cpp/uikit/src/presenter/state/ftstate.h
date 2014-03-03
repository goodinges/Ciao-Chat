#ifndef _FTSTATE_H
#define _FTSTATE_H

#include "state.h"
#include "skype-embedded_2.h"

namespace Mvp
{

namespace Model
{
class AbstractFTService;
};


namespace View
{
class AbstractView;
class AbstractFTView;
};

namespace Presenter
{
class FTPresenter;

/**
 * @class FTState ftstate.h ftstate.h
 * @brief Implementation of a File Transfer state needed to own all mvp objects
 * for a file transfer session.
 *
 * This class implements a file transfer state which owns a FTService, a
 * FTPresenter, and an AbstractFTView.
 *
 * @see State, FTService, FTPresenter, AbstractFTView.
 */
class FTState: public State
{
public:
    /**
     * Constructor
     *
     * @param context the context this state belongs to.
     * @param transfer the tranfer object that will be managed.
     */
    FTState(Context &context, const TransferRef &transfer);

    /**
     * Destructor
     */
    ~FTState();

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
     * viewFactory->requestView(FTView).
     *
     * @param sender a pointer to the object that sent the signal.
     * @param pair a pair containing the request id and the view object.
     *
     * @see AbstractViewFactory::requestView
     *
     * @note the view should be of type AbstractFTView.
     */
    void catchView(const void *sender,
        std::pair<int, Mvp::View::AbstractView *> &pair);

    /**
     * A reference to the transfer going to be managed.
     */
    TransferRef m_transfer;

    /**
     * Model, Presenter and View needed to manage a file transfer.
     */
    Mvp::Model::AbstractFTService *m_service;
    Mvp::Presenter::FTPresenter *m_presenter;
    Mvp::View::AbstractFTView *m_view;
    int m_viewRequestId;
};

};

};
#endif

