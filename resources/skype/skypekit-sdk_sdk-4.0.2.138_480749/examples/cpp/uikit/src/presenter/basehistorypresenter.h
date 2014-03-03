#ifndef _BASEHISTORYPRESENTER_H
#define _BASEHISTORYPRESENTER_H

#include "abstractpresenter.h"
#include "model/abstracthistoryservice.h"
#include "model/historydata.h"
#include "view/abstracthistoryview.h"

#include <Poco/BasicEvent.h>
#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class BaseHistoryPresenter basehistorypresenter.h basehistorypresenter.h
 * @brief Base class to define the interface for HistoryPresenters.
 *
 * HistoryPresenter is responsible for connecting the HistoryService to the
 * HistoryView, manipulating the HistoryView (show, hide, etc.) and passing
 * data from one to the other and back.
 *
 * This class is a base class that does the connecting of poco signals to slots.
 *
 * @see AbstractPresenter, AbstractHistoryService and AbstractHistoryView.
 */
class BaseHistoryPresenter : public
    AbstractPresenter<Mvp::View::AbstractHistoryView>
{
public:
    /**
     * Constructor.
     *
     * Takes the model to use for this presenter.
     *
     * @param model AbstractHistoryService to use for this text history.
     */
    BaseHistoryPresenter(Mvp::Model::AbstractHistoryService *model)
        :m_model(model)
    {
        // Connect the model's signals to our slots.
        m_model->historyChanged +=
            Poco::Delegate<BaseHistoryPresenter, Poco::EventArgs >
            (this, &BaseHistoryPresenter::onHistoryChanged);
    }

    /**
     * Destructor.
     */
    virtual ~BaseHistoryPresenter()
    {
        // Disonnect the model's signals from our slots.
        m_model->historyChanged -=
            Poco::Delegate<BaseHistoryPresenter, Poco::EventArgs>
            (this, &BaseHistoryPresenter::onHistoryChanged);
    }

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    virtual void setView(Mvp::View::AbstractHistoryView *view) = 0;

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    virtual void showView() = 0;

protected:
    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onViewClose(const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Slot to react to new history changed signals from the model.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onHistoryChanged(const void *sender, Poco::EventArgs &args) = 0;

    Mvp::Model::AbstractHistoryService *m_model;
};

};

};

#endif

