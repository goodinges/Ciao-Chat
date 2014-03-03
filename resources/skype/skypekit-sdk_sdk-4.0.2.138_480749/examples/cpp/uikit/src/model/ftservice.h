#ifndef _FTSERVICE_H_
#define _FTSERVICE_H_

#include <string>

#include "abstractftservice.h"
#include "skypetransfer.h"

namespace Mvp
{

namespace Model
{

/**
 * @class FTService ftservice.h ftservice.h
 * @brief Implements a simple FT Service.
 *
 * This class represents the interaction between a ft window and the
 * SkypeKit itself. All events from the FT session will be managed by
 * this class.
 *
 * @see AbstractFTService
 */
class FTService : public AbstractFTService
{
public:
    /**
     * Constructor
     *
     * Every FTService needs a transfer object to do its work, so we take
     * the TransferRef as a parameter at construction time.
     *
     * @param transfer the transfer object to use for this ft.
     */
    FTService(const TransferRef &transfer);

    /**
     * Destructor, cancels the transfer.
     */
    ~FTService();

    /**
     * Returns the partner display name.
     *
     * @return the display name.
     */
    std::string partnerDisplayName();

    /**
     * Slot to handle transfer status changed.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param status new transfer status.
     */
     void onStatusChanged(const void *sender, Transfer::STATUS &status);

    /**
     * Slot to handle transfer progress changes.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onProgressChanged(const void *sender, Poco::EventArgs &args);

    /**
     * Return the current transfer status.
     *
     * @return the current transfer status.
     */
     Transfer::STATUS status();

    /**
     * Return the current failure reason.
     *
     * @return the current failure code.
     */
    Transfer::FAILUREREASON failureCode();

    /**
     * Accept the Incoming File Transfer.
     *
     * When a FT session is offered the peer has the possibility to accept that
     * transfer by calling this method.
     *
     * @param filePath the local target file path.
     */
    void acceptFT(const std::string &filePath);

    /**
     * Reject the Incoming File Transfer.
     *
     * When a FT session is offered the peer has the possibility to reject that
     * transfer by calling this method.
     */
    void rejectFT();

    /**
     * Cancel the ongoing File Transfer.
     *
     * The FT session can be cancelled by both peers at anytime, by calling
     * this method.
     */
    void cancelFT();

    /**
     * Pause the ongoing File Transfer.
     *
     * Once the FT session is transferring data it's possible to pause the
     * whole session and resume it later from the stopped point.
     */
    void pauseFT();

    /**
     * Resume the paused File Transfer.
     *
     * Once the FT session is paused it's possible to resume the session,
     * continue transferring the missing bytes of the session.
     */
    void resumeFT();

     /**
      * Return the session's file size.
      *
      * Return the size of the file being transferred/negotiated.
      *
      * @return the size of the file.
      */
    const std::string fileSize();

    /**
     * Filename of the incoming file.
     *
     * @return the filename of the incoming file.
     */
    const std::string fileName();

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
    const std::string fileSource();

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
    const std::string fileDestination();

    /**
     * Return the session's direction.
     *
     * The session direction says if it's an INCOMING or OUTGOING transfer.
     *
     * @return the direction of the session.
     */
    Transfer::TYPE transferDirection();

private:

    SkypeTransfer::Ref m_transfer;
};

};

};

#endif
