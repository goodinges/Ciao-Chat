#ifndef _BASECONVMANAGERPRESENTER_H
#define _BASECONVMANAGERPRESENTER_H

#include "abstractpresenter.h"
#include "model/abstractconvmanagerservice.h"
#include "model/messagedata.h"
#include "view/abstractconvmanagerview.h"

#include <Poco/BasicEvent.h>
#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class BaseConvManagerPresenter baseconvmanagerpresenter.h
 * baseconvmanagerpresenter.h
 * @brief Base class to define the interface for ConvManagerPresenters.
 *
 * ConvManagerPresenter is responsible for connecting the ConvManagerService to
 * the ConvManagerView, manipulating the ConvManagerView (show, hide, etc.)
 * and passing data from one to the other and back.
 *
 * This class is a base class that does the connecting of poco signals to slots.
 *
 * @see AbstractPresenter, AbstractConvManagerService and
 * AbstractConvManagerView.
 */
class BaseConvManagerPresenter :
    public AbstractPresenter<Mvp::View::AbstractConvManagerView>
{
public:
    /**
     * Constructor.
     *
     * Takes the model to use for this presenter.
     *
     * @param model AbstractConvManagerService to use to manage a conversation.
     */
    BaseConvManagerPresenter(Mvp::Model::AbstractConvManagerService *model)
        :m_model(model)
    {
        // Connect to model signals.
        m_model->participantsChanged +=
            Poco::Delegate<BaseConvManagerPresenter, Poco::EventArgs>
            (this, &BaseConvManagerPresenter::onParticipantsChanged);
        m_model->contactChanged += Poco::Delegate
            <BaseConvManagerPresenter, Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseConvManagerPresenter::onContactChanged);
    }

    /**
     * Destructor.
     */
    virtual ~BaseConvManagerPresenter()
    {
        // Disconnect from model signals.
        m_model->participantsChanged -=
            Poco::Delegate<BaseConvManagerPresenter, Poco::EventArgs>
            (this, &BaseConvManagerPresenter::onParticipantsChanged);
        m_model->contactChanged -= Poco::Delegate
            <BaseConvManagerPresenter, Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseConvManagerPresenter::onContactChanged);
    }

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    virtual void setView(Mvp::View::AbstractConvManagerView *view) = 0;

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
     * Slot to react to participants changed signal from the service.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onParticipantsChanged(const void *sender,
        Poco::EventArgs &) = 0;

    /**
     * Slot to react to contact changed signal from the service.
     *
     * @param sender a pointer to the sender of the signal.
     * @param contact the new contact data to use.
     */
    virtual void onContactChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact) = 0;

    /**
     * Slot to react to updateParticipantList from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param participants a vector of skypenames to set as the participants.
     */
    virtual void onUpdateParticipantList(const void *sender,
        const std::vector<std::string> &participants) = 0;

    Mvp::Model::AbstractConvManagerService *m_model;
};

};

};

#endif

