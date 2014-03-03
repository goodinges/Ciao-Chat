#include "ftservice.h"

#include "skype-embedded_2.h"

#include <Poco/Delegate.h>

using Poco::Delegate;

namespace Mvp
{

namespace Model
{

FTService::FTService(const TransferRef &transfer)
    : m_transfer(transfer)
{
    SkypeTransfer::Ref skypeTransfer(transfer);

    // Connect statusChanged signal.
    skypeTransfer->statusChanged +=
        Poco::Delegate<AbstractFTService, Transfer::STATUS>
        (this, &AbstractFTService::onStatusChanged);

    // Connect progressChanged signal.
    skypeTransfer->progressChanged +=
        Poco::Delegate<AbstractFTService, Poco::EventArgs>
        (this, &AbstractFTService::onProgressChanged);
}

FTService::~FTService()
{
    // Disconnect statusChanged signal.
    m_transfer->statusChanged -=
        Poco::Delegate<AbstractFTService, Transfer::STATUS>
        (this, &AbstractFTService::onStatusChanged);

    // Disconnect progressChanged signal.
    m_transfer->progressChanged -=
        Poco::Delegate<AbstractFTService, Poco::EventArgs>
        (this, &AbstractFTService::onProgressChanged);

    // Leave the transfer.
    m_transfer->Cancel();
}

Transfer::STATUS FTService::status()
{
    // Get the status from the transfer.
    Transfer::STATUS status;
    m_transfer->GetPropStatus(status);

    return status;
}

Transfer::FAILUREREASON FTService:: failureCode()
{
    // Get the failure code fro the transfer.
    Transfer::FAILUREREASON code;
    m_transfer->GetPropFailurereason(code);

    return code;
}

void FTService::acceptFT(const std::string &filePath)
{
    Transfer::STATUS cstatus = status();

    /* Before accepting the ft we need to check if the ft
     * is ready to be accepted.
     */
    if (cstatus == Transfer::NEW ||
        cstatus == Transfer::CONNECTING) {
        bool success = false;
        const Sid::Filename fileName = filePath.c_str();
        m_transfer->Accept(fileName, success);
    }
}

void FTService::rejectFT()
{
    Transfer::STATUS cstatus = status();

    /* Before rejecting the ft we need to check if the ft
     * is ready to be rejected.
     */
    if (cstatus == Transfer::NEW ||
        cstatus == Transfer::CONNECTING) {
        m_transfer->Cancel();
    }
}

void FTService::cancelFT()
{
    Transfer::STATUS cstatus = status();

    /* Before cancelling the ft we need to check if the ft
     * is ready to be finished.
     */
    if (cstatus != Transfer::CANCELLED ||
        cstatus != Transfer::COMPLETED ||
        cstatus != Transfer::FAILED ||
        cstatus != Transfer::CANCELLED_BY_REMOTE) {
        m_transfer->Cancel();
    }
}

void FTService::pauseFT()
{
    Transfer::STATUS cstatus = status();

    /* Before pausing the ft we need to check if the ft
     * is ready to be paused.
     */
    if (cstatus == Transfer::TRANSFERRING ||
        cstatus == Transfer::TRANSFERRING_OVER_RELAY) {
        m_transfer->Pause();
    }
}

void FTService::resumeFT()
{
    Transfer::STATUS cstatus = status();

    /* Before resuming the ft we need to check if the ft
     * is ready to be resumed.
     */
    if (cstatus == Transfer::PAUSED)
        m_transfer->Resume();
}

const std::string FTService::fileSource()
{
    Sid::String source;
    Transfer::TYPE type;

    m_transfer->GetPropType(type);

    if (type == Transfer::INCOMING) {
        m_transfer->GetPropPartnerDispname(source);

        Sid::String file;
        m_transfer->GetPropFilename(file);

        source = source + ":/" + file;
    }
    else {
        m_transfer->GetPropFilepath(source);
    }

    return source.data();
}

const std::string FTService::fileName()
{
    Sid::String source;
    Transfer::TYPE type;

    m_transfer->GetPropType(type);

    if (type == Transfer::INCOMING)
        m_transfer->GetPropFilename(source);

    return source.data();
}

const std::string FTService::fileDestination()
{
    Sid::String dest;
    Transfer::TYPE type;

    m_transfer->GetPropType(type);

    if (type == Transfer::INCOMING) {
        m_transfer->GetPropFilepath(dest);
    }
    else {
        m_transfer->GetPropPartnerDispname(dest);

        Sid::String file;
        m_transfer->GetPropFilename(file);

        dest = dest + ":/" + file;
    }

    return dest.data();
}

std::string FTService::partnerDisplayName()
{
    Sid::String name;
    m_transfer->GetPropPartnerDispname(name);

    return name.data();
}

const std::string FTService::fileSize()
{
    Sid::String size;

    m_transfer->GetPropFilesize(size);

    return size.data();
}

void FTService::onStatusChanged(const void *sender,
        Transfer::STATUS &status)
{
    // Propagate the transfer status to other objects.
    statusChanged(this, status);
}

void FTService::onProgressChanged(const void *sender,
        Poco::EventArgs &args)
{
    /* When the progress of the transfer is updated it's necessary to emit a
     * signal with two basic information: The number of bytes already
     * transferred and the number of bytes being transferred per second.
     */

    // Get bytes transferred.
    Sid::String bytesTransferred;
    m_transfer->GetPropBytestransferred(bytesTransferred);

    // Get bytes per second.
    uint bytesPerSecond;
    m_transfer->GetPropBytespersecond(bytesPerSecond);

    std::pair<std::string, uint> progress;
    progress.first = bytesTransferred.data();
    progress.second = bytesPerSecond;

    // Notify other objects about the progress changes.
    progressChanged(this, progress);
}

Transfer::TYPE FTService::transferDirection()
{
    Transfer::TYPE type;

    m_transfer->GetPropType(type);

    return type;
}

};

};
