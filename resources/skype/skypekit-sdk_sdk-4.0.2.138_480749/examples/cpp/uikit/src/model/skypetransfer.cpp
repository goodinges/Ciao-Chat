#include "skypetransfer.h"

namespace Mvp
{

namespace Model
{

SkypeTransfer::SkypeTransfer(unsigned int oid, SERootObject *root)
    :Transfer(oid, root)
{
}

SkypeTransfer::~SkypeTransfer()
{
    printf("transfer dtor\n");
}

void SkypeTransfer::OnChange(int prop)
{
    /* Here is the place where you can capture all property changes when they
     * happen. It's important to know that this method will receive ALL changes
     * from ALL properties, which means that it's necessary to create a nice and
     * robust filter inside this method to handle each type of property,
     * otherwise it can fail by acting incorrectly or even lose some events.
     *
     * Another interesting thing is that this method does not receive the new
     * value of the changed property. It's necessary to get the value from the
     * Transfer class itself, because once this method is called the
     * property is already changed in the class.
     */

    /* For our Transfer class we will watch for the status changes and for the
     * bytes transferred in the session. The Status property will give us the
     * current status of the transfer, allowing us to control it properly
     * depending on the status. The bytes transferred property will give us a
     * way to know how many bytes was already sent to the other peer, allowing
     * us to calculate the session's progress. There're other properties that
     * can be used to watch the session progress, such as bytes per second
     * (P_BYTESPERSECOND) and ETA (P_FINISHTIME).
     */

    if (prop == Transfer::P_STATUS) {
        printf("Transfer property %d changed\n.", prop);

        // Get the live status value and emit a signal.
        Transfer::STATUS status;
        GetPropStatus(status);

        statusChanged(this, status);
    }
    else if (prop == Transfer::P_BYTESTRANSFERRED) {
        printf("Transfer property %d changed\n.", prop);

        // Notify about bytes transferred.
        Poco::EventArgs args;
        progressChanged(this, args);
    }
}

};

};
