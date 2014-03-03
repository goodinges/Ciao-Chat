#ifndef _SKYPETRANSFER_H
#define _SKYPETRANSFER_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>
#include <Poco/SharedPtr.h>

namespace Mvp
{

namespace Model
{

/**
 * @class SkypeTransfer skypetransfer.h skypetransfer.h
 * @brief Implements basic features of the SkypeKit Transfer class.
 *
 * In order to intercept the transfer's properties changes we need to subclass
 * the Transfer class and overwrite the OnChange method that is called when
 * a property is updated.
 *
 * @see FTService, AbstractFTService
 */
class SkypeTransfer : public Transfer
{
public:
    typedef DRef<SkypeTransfer, Transfer> Ref;

    /**
     * Constructor
     */
    SkypeTransfer(unsigned int oid, SERootObject *root);

    /**
     * Signal that the live status has changed.
     */
    Poco::BasicEvent<Transfer::STATUS> statusChanged;

    /**
      * Signal that the progress has changed.
      */
    Poco::BasicEvent<Poco::EventArgs> progressChanged;

protected:
    /**
     * Destructor.
     */
    virtual ~SkypeTransfer();

    /**
     * Override the OnChange method to handle transfer change notifications.
     *
     * @param prop the property that changed.
     */
    void OnChange(int prop);
};

};

};

#endif
