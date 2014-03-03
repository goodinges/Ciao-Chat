#ifndef _ABSTRACTFTSERVICE_H
#define _ABSTRACTFTSERVICE_H

#include <string>

#include "skypetransfer.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

/**
 * @class AbstractFTService abstractftservice.h abstractftservice.h
 * @brief Abstract FT Service class.
 *
 * This class provides the base interface that needs to be implemented
 * by the file transfer services, such as, and specifically, getting the ft
 * status, accept, cancel and pause/resume.
 *
 * @note This class is abstract. If you're looking for a concrete one
 * try looking at the FTService class.
 *
 */
class AbstractFTService
{
public:
    /**
     * Constructor
     */
    AbstractFTService() {}

    /**
     * Virtual destructor
     */
    virtual ~AbstractFTService() {}

    /**
     * Returns the partner display name.
     *
     * @return the display name.
     */
    virtual std::string partnerDisplayName() = 0;

    /**
     * Slot to handle transfer status changed.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param status new transfer status.
     */
    virtual void onStatusChanged(const void *sender,
            Transfer::STATUS &status) = 0;

    /**
     * Slot to handle transfer progress changes.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    virtual void onProgressChanged(const void *sender,
            Poco::EventArgs &args) = 0;

    /**
     * Return the current transfer status.
     *
     * @return the current transfer status.
     */
    virtual Transfer::STATUS status() = 0;

    /**
     * Return the current failure reason.
     *
     * @return the current failure code.
     */
    virtual Transfer::FAILUREREASON failureCode() = 0;

    /**
     * Accept the Incoming File Transfer.
     *
     * When a FT session is offered the peer has the possibility to accept that
     * transfer by calling this method.
     *
     * @param filePath the local target file path.
     */
    virtual void acceptFT(const std::string &filePath) = 0;

    /**
     * Reject the Incoming File Transfer.
     *
     * When a FT session is offered the peer has the possibility to reject that
     * transfer by calling this method.
     */
    virtual void rejectFT() = 0;

    /**
     * Cancel the ongoing File Transfer.
     *
     * The FT session can be cancelled by both peers at anytime, by calling
     * this method.
     */
    virtual void cancelFT() = 0;

    /**
     * Pause the ongoing File Transfer.
     *
     * Once the FT session is transferring data it's possible to pause the
     * whole session and resume it later from the stopped point.
     */
    virtual void pauseFT() = 0;

    /**
     * Resume the paused File Transfer.
     *
     * Once the FT session is paused it's possible to resume the session,
     * continue transferring the missing bytes of the session.
     */
    virtual void resumeFT() = 0;

    /**
     * Return the session's file size.
     *
     * Return the size of the file being transferred/negotiated.
     *
     * @return the size of the file.
     */
    virtual const std::string fileSize()  = 0;

    /**
     * Filename of the incoming file.
     *
     * @return the filename of the incoming file.
     */
    virtual const std::string fileName() = 0;

    /**
     * Return the file source.
     *
     * The file source is from where the offered file is being loaded to be sent
     * to the other peer. For Incoming FT sessions the source will be the
     * partner offering the file + the file name. For Outgoing FT sessions the
     * source will be the path of the file stored on the local peer.
     *
     * @return the source of the file.
     */
    virtual const std::string fileSource() = 0;

    /**
     * Return the file destination.
     *
     * The file destination is to where the offered file will be sent or stored.
     * For Incoming FT session the destination will be the local path where the
     * file is going to be stored. For Outgoing FT sessions the destination will
     * be the partner that will receive the file + the file name.
     *
     * @return the destination of the file.
     */
    virtual const std::string fileDestination() = 0;

    /**
     * Return the session's direction.
     *
     * The session direction says if it's an INCOMING or OUTGOING transfer.
     *
     * @return the direction of the session.
     */
    virtual Transfer::TYPE transferDirection() = 0;

    /**
     * Signal that the status has changed.
     */
    Poco::BasicEvent<Transfer::STATUS> statusChanged;

    /**
     * Signal that the progress has changed.
     */
    Poco::BasicEvent<const std::pair<const std::string, uint> > progressChanged;
};

};

};

#endif
