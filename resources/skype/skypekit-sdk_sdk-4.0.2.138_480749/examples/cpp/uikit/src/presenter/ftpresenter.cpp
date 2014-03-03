#include "ftpresenter.h"

#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>
#include <Poco/SharedPtr.h>

#include "view/abstractftview.h"

namespace Mvp
{

namespace Presenter
{

FTPresenter::FTPresenter(Mvp::Model::AbstractFTService *model)
    : BaseFTPresenter(model)
{
}

void FTPresenter::setView(Mvp::View::AbstractFTView *view)
{
    // Give the view to the base class for caching.
    AbstractPresenter<Mvp::View::AbstractFTView>::setView(view);

    // Let the view initialize itself.
    m_view->initialize();

    // Connect all view signals to our slots.
    m_view->accepted += Poco::Delegate<FTPresenter, const std::string>
        (this, &FTPresenter::onFTAccepted);
    m_view->rejected += Poco::Delegate<FTPresenter, Poco::EventArgs>
        (this, &FTPresenter::onFTRejected);
    m_view->cancel += Poco::Delegate<FTPresenter, Poco::EventArgs>
        (this, &FTPresenter::onFTCancelled);
    m_view->pause += Poco::Delegate<FTPresenter, Poco::EventArgs>
        (this, &FTPresenter::onFTPaused);
    m_view->resume += Poco::Delegate<FTPresenter, Poco::EventArgs>
        (this, &FTPresenter::onFTResumed);

    m_view->viewClosed += Poco::Delegate<FTPresenter, Poco::EventArgs>
        (this, &FTPresenter::onViewClose);

    /* The first thing we need to do is to check the transfer direction and
     * tell the view which one it's going to handle.
     */
    Mvp::View::AbstractFTView::FTDirection direction;
    Transfer::TYPE type = m_model->transferDirection();

    if (type == Transfer::INCOMING) {
        direction = Mvp::View::AbstractFTView::Incoming;
        m_view->setFTSourceFileName(m_model->fileName());
    }
    else {
        direction = Mvp::View::AbstractFTView::Outgoing;

        // If it's an Outgoing session lets set the file destination already.
        m_view->setDestinationDesc(m_model->fileDestination().c_str());
    }

    // Set the destination to the view.
    m_view->setDirection(direction);

    // Set the file source on the view.
    m_view->setSourceDesc(m_model->fileSource().c_str());

    // Inform the view the size of the file being negotiated.
    m_view->setFileSize(m_model->fileSize());

    // Set View title.
    m_view->setFTTitle(m_model->partnerDisplayName());

    // Set the initial  status just in case we missed some signal.
    Transfer::STATUS status = m_model->status();
    onStatusChanged(this, status);
}

void FTPresenter::showView()
{
    // Then show it.
    m_view->showView();
}

void FTPresenter::onViewClose(const void *sender, Poco::EventArgs &args)
{
    Transfer::STATUS status = m_model->status();

    /* The user has closed the view, so before leaving the file transfer we
     * need to terminate it.
     */
    if (status != Transfer::CANCELLED &&
        status != Transfer::CANCELLED_BY_REMOTE &&
        status != Transfer::FAILED) {
        onFTCancelled(this, args);
    }

    finished(this, args);
}

void FTPresenter::onFTAccepted(const void *sender, const std::string &fileName)
{
    // Ask the model to accept the transfer.
    m_model->acceptFT(fileName);
}

void FTPresenter::onFTRejected(const void *sender, Poco::EventArgs &args)
{
    // Ask the model to reject the transfer.
    m_model->rejectFT();
}

void FTPresenter::onFTCancelled(const void *sender, Poco::EventArgs &args)
{
    // Ask the model to cancel the transfer.
    m_model->cancelFT();
}

void FTPresenter::onFTPaused(const void *sender, Poco::EventArgs &args)
{
    // Ask the model to pause the transfer.
    m_model->pauseFT();
}

void FTPresenter::onFTResumed(const void *sender, Poco::EventArgs &args)
{
    // Ask the model to resume the transfer.
    m_model->resumeFT();
}

void FTPresenter::setViewError()
{
    /* Whenever an error occurs we need to get the error code and set the a
     * proper state or error on the view, then the view can report the error
     * to the user.
     */
    Transfer::FAILUREREASON code = m_model->failureCode();

    switch (code) {
        case Transfer::SENDER_NOT_AUTHORISED:
        case Transfer::REMOTELY_CANCELLED:
            m_view->setFTError(Mvp::View::AbstractFTView::Cancelled);
            break;
        case Transfer::FAILED_READ:
        case Transfer::FAILED_REMOTE_READ:
            m_view->setFTError(Mvp::View::AbstractFTView::FailedOnRead);
            break;
        case Transfer::FAILED_WRITE:
        case Transfer::FAILED_REMOTE_WRITE:
            m_view->setFTError(Mvp::View::AbstractFTView::FailedOnWrite);
            break;
        case Transfer::REMOTE_DOES_NOT_SUPPORT_FT:
            m_view->setFTError(Mvp::View::AbstractFTView::FTNotSupported);
            break;
        case Transfer::PLACEHOLDER_TIMEOUT:
            m_view->setFTError(Mvp::View::AbstractFTView::Timeout);
		default:
            break;
    }
}

void FTPresenter::onStatusChanged(const void *sender,
        Transfer::STATUS &status)
{
    printf("FTPresenter::onStatusChanged: %d\n", status);

    /* Check if the view was already created. In some cases the first status
     * changed signal is emitted too fast before the view creation.
     */
    if (!m_view)
        return;

    // Handle status changes.
    switch (status) {
        case Transfer::NEW:
            m_view->setFTStatus(Mvp::View::AbstractFTView::Connecting);
            break;
        case Transfer::WAITING_FOR_ACCEPT:
            m_view->setFTStatus(Mvp::View::AbstractFTView::WaitingForAccept);
            break;
        case Transfer::TRANSFERRING:
        case Transfer::TRANSFERRING_OVER_RELAY: {
            Transfer::TYPE type = m_model->transferDirection();

            if (type == Transfer::INCOMING)
                m_view->setDestinationDesc(m_model->fileDestination().c_str());

            m_view->setFTStatus(Mvp::View::AbstractFTView::Transferring);
            break;
        }
        case Transfer::PAUSED:
        case Transfer::REMOTELY_PAUSED:
            m_view->setFTStatus(Mvp::View::AbstractFTView::Paused);
            break;
        case Transfer::FAILED:
            m_view->setFTStatus(Mvp::View::AbstractFTView::Failed);
            setViewError();
            break;
        case Transfer::CANCELLED:
        case Transfer::CANCELLED_BY_REMOTE:
            m_view->setFTStatus(Mvp::View::AbstractFTView::Failed);
            m_view->setFTError(Mvp::View::AbstractFTView::Cancelled);
            break;
        case Transfer::COMPLETED:
            m_view->setFTStatus(Mvp::View::AbstractFTView::Done);
            break;
		default: break;
    }
}

void FTPresenter::onProgressChanged(const void *sender,
        const std::pair<const std::string, uint> &progress)
{
    // Update the progress on the view.
    if (m_view)
        m_view->updateTransferData(progress.first, progress.second);
}

};

};
