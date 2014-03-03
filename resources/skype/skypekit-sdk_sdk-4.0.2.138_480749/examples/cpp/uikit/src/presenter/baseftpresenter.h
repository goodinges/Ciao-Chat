#ifndef _BASEFTPRESENTER_H
#define _BASEFTPRESENTER_H

#include "abstractpresenter.h"
#include "model/abstractftservice.h"
#include "view/abstractftview.h"

#include <Poco/BasicEvent.h>
#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class BaseFTPresenter baseftpresenter.h baseftpresenter.h
 * @brief Base class to define the interface for FTPresenters.
 *
 * FTPresenter is responsible for connecting the FTService to the
 * FTView, manipulating the FTView (show, hide, etc.) and passing
 * data from one to the other and back.
 *
 * This class is a base class that does the connecting of poco signals to slots.
 *
 * @see AbstractPresenter, AbstractFTService and AbstractFTView.
 */
class BaseFTPresenter : public AbstractPresenter<Mvp::View::AbstractFTView>
{
public:
    /**
     * Constructor.
     *
     * Takes the model to use for this presenter.
     *
     * @param model AbstractFTService to use for this file transfer.
     */
    BaseFTPresenter(Mvp::Model::AbstractFTService *model)
        :m_model(model)
    {
        // Connect the model's signals to our slots.
        m_model->statusChanged +=
            Poco::Delegate<BaseFTPresenter, Transfer::STATUS>
            (this, &BaseFTPresenter::onStatusChanged);

        m_model->progressChanged +=
            Poco::Delegate<BaseFTPresenter, const std::pair<const std::string,
            uint> > (this, &BaseFTPresenter::onProgressChanged);
    }

    /**
     * Destructor.
     */
    virtual ~BaseFTPresenter()
    {
        // Disconnect the model's signals to our slots.
        m_model->statusChanged -=
            Poco::Delegate<BaseFTPresenter, Transfer::STATUS>
            (this, &BaseFTPresenter::onStatusChanged);

        m_model->progressChanged -=
            Poco::Delegate<BaseFTPresenter, const std::pair<const std::string,
            uint> > (this, &BaseFTPresenter::onProgressChanged);
    }

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    virtual void setView(Mvp::View::AbstractFTView *view) = 0;

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    virtual void showView() = 0;

    /**
     * Slot to handle transfer status changed.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param status new transfer status.
     */
    virtual void onStatusChanged(const void *sender,
        Transfer::STATUS &status) = 0;

    /**
     * Slot to handle transfer progress changed.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param progress a pair containing the bytes already tranferred and the
     * speed of the transfer.
     */
    virtual void onProgressChanged(const void *sender,
            const std::pair<const std::string, uint> &progress) = 0;
protected:
    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onViewClose(const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Slot to react to the accepted signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param fileName a place where the file will be saved.
     */
    virtual void onFTAccepted(const void *sender,
            const std::string &fileName) = 0;

    /**
     * Slot to react to the rejected signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onFTRejected(const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Slot to react to the cancel signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onFTCancelled(const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Slot to react to the pause signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onFTPaused(const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Slot to react to the resume signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onFTResumed(const void *sender, Poco::EventArgs &args) = 0;

    Mvp::Model::AbstractFTService *m_model;
};

};

};
#endif
