#ifndef _CONVMANAGERPRESENTER_H
#define _CONVMANAGERPRESENTER_H

#include "baseconvmanagerpresenter.h"
#include "view/abstractconvmanagerview.h"
#include "model/abstractconvmanagerservice.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class ConvManagerPresenter convmanagerpresenter.h convmanagerpresenter.h
 * @brief Class to present conversation management information.
 *
 * This class is responsible for connecting the ConvManagerService to the
 * ConvManagerView, manipulating the ConvManagerView (show, hide, etc.) and
 * passing data from one to the other and back.
 *
 * @see AbstractPresenter, AbstractConvManagerService and
 * AbstractConvManagerView.
 */
class ConvManagerPresenter : public BaseConvManagerPresenter
{
public:
    /**
     * Constructor.
     *
     * @param model AbstractConvManagerService to use.
     */
    ConvManagerPresenter(Mvp::Model::AbstractConvManagerService *model);

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    void setView(Mvp::View::AbstractConvManagerView *view);

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    void showView();

protected:
    /**
     * Slot to react to participants changed signal from the service.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onParticipantsChanged(const void *sender, Poco::EventArgs &);

    /**
     * Slot to react to contact changed signal from the service.
     *
     * @param sender a pointer to the sender of the signal.
     * @param contact the new contact data to use.
     */
    void onContactChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact);

    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onViewClose(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to updateParticipantList signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param participants a vector of skypenames to set as the participants.
     */
    void onUpdateParticipantList(const void *sender,
        const std::vector<std::string> &participants);
};

};

};
#endif

