#ifndef _FTPRESENTER_H
#define _FTPRESENTER_H

#include "baseftpresenter.h"
#include "view/abstractftview.h"
#include "model/abstractftservice.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class FTPresenter ftpresenter.h ftpresenter.h
 * @brief Class to present File Transfer information via FTView using FTService.
 *
 * This class is responsible for connecting the FTService to the FTView,
 * manipulating the FTView (show, hide, etc.) and passing data from one
 * to the other and back.
 *
 * @see AbstractPresenter, AbstractFTService and AbstractFTView.
 */
class FTPresenter : public BaseFTPresenter
{
public:
    /**
     * Constructor.
     *
     * @param model AbstractFTService to use for this file transfer.
     */
    FTPresenter(Mvp::Model::AbstractFTService *model);

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    void setView(Mvp::View::AbstractFTView *view);

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    void showView();

protected:
    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onViewClose(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to the accepted signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param fileName a place where the file will be saved.
     */
    void onFTAccepted(const void *sender, const std::string &fileName);

    /**
     * Slot to react to the rejected  signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onFTRejected(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to the cancelled signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
     void onFTCancelled(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to the paused signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
     void onFTPaused(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to the resumed signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
     void onFTResumed(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to handle transfer status changed.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param status the new file transfer status.
     */
     void onStatusChanged(const void *sender,
        Transfer::STATUS &status);

    /**
     * Slot to handle transfer progress changed.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param progress a pair containing the bytes already tranferred and the
     * speed of the transfer.
     */
    void onProgressChanged(const void *sender,
        const std::pair<const std::string, uint> &progress);

private:
    void setViewError();
};

};

};
#endif

