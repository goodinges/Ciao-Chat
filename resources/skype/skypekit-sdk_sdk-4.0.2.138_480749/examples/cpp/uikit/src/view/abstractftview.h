#ifndef _ABSTRACTFTVIEW_H
#define _ABSTRACTFTVIEW_H

#include <string>

#include "abstractview.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace View
{

/**
 * @class AbstractFTView abstractftview.h abstractftview.h
 * @brief Abstract base class declaring all methods FTViews should implement.
 *
 * This class provides all of the information and interfaces presenters need to
 * work with any FTView object.
 *
 * @see QtFTView
 */
class AbstractFTView : public AbstractView
{
public:

    enum FTStatus
    {
        Connecting,
        WaitingForAccept,
        Transferring,
        Paused,
        Failed,
        Done
    };

    enum FTError
    {
        Cancelled,
        FTNotSupported,
        Timeout,
        FailedOnRead,
        FailedOnWrite
    };

    enum FTDirection
    {
        Incoming = 0,
        Outgoing
    };

    /**
     * Set the title of the file transfer window.
     *
     * @param title the title to show for this file transfer view.
     */
    virtual void setFTTitle(const std::string &title) = 0;

    /**
     * Show the given status code in the user interface.
     *
     * Subclasses should implement this to show the given status code somehow
     * in the interface, within the guidelines of the skypekit certification
     * documents.
     *
     * @param status The status code to show.
     */
    virtual void setFTStatus(FTStatus status) = 0;

    /**
     * Give the source filename.
     *
     * @param filename the original filename of the transfer.
     */
    virtual void setFTSourceFileName(const std::string &fileName) = 0;

    /**
     * Show the given error code in the user interface.
     *
     * Subclasses should implement this to show the given error code somehow
     * in the interface, within the guidelines of the skypekit certification
     * documents.
     *
     * @param error The error code to show.
     */
    virtual void setFTError(FTError error) = 0;

    /**
     * Set the file source description.
     *
     * @param source the description of the file being sent.
     */
    virtual void setSourceDesc(const std::string &source) = 0;

    /**
     * Set the file destination description.
     *
     * @param dest the description of the file being sent.
     */
    virtual void setDestinationDesc(const std::string &dest) = 0;

    /**
     * Set the file size.
     *
     * When a new session is initiated it's necessary to know what is the size
     * of the file being negotiated before starting the transference. The size
     * format is 'bytes'.
     *
     * @param size The number of bytes contained in the file.
     */
    virtual void setFileSize(const std::string &size) = 0;

    /**
     * Set the transfer's direction.
     *
     * As the view needs to handle both transfer directions (Incoming and
     * Outgoing) this method will set a flag saying which kind of transfer the
     * view is going to manage.
     *
     * @param direction the transfer direction going to be managed.
     */
    virtual void setDirection(const FTDirection &direction) = 0;

    /**
     * Updates the transfer progress.
     *
     * Whenever the transfer progress is changed this method will be called to
     * update the view.
     *
     * @param bytesTrans the number of bytes already transferred.
     * @param bytesPS the number of bytes being transferred per second (speed).
     */
    virtual void updateTransferData(const std::string &bytesTrans,
            uint bytesPS) = 0;

    /**
     * Signal to notify when the ft is paused.
     */
    Poco::BasicEvent<Poco::EventArgs> pause;

    /**
     * Signal to notify when the ft is resumed.
     */
    Poco::BasicEvent<Poco::EventArgs> resume;

    /**
     * Signal to notify when the ft is cancelled.
     */
    Poco::BasicEvent<Poco::EventArgs> cancel;

    /**
     * Signal to notify that the ft was accepted.
     */
    Poco::BasicEvent<const std::string> accepted;

    /**
     * Signal to notify that the ft was rejected.
     */
    Poco::BasicEvent<Poco::EventArgs> rejected;
};

};

};
#endif
