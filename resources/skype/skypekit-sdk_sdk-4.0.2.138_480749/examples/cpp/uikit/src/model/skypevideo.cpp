#include "skypevideo.h"

namespace Mvp
{

namespace Model
{

SkypeVideo::SkypeVideo(unsigned int oid, SERootObject* root)
    : Video(oid, root)
{
    printf("New Video created.\n.");
}

SkypeVideo::~SkypeVideo()
{
}

void SkypeVideo::OnChange(int prop)
{
    /* Here is the place where you can capture all property changes when they
     * happen. It's important to know that this method will receive ALL changes
     * from ALL properties, which means that it's necessary to create a nice and
     * robust filter inside this method to handle each type of property,
     * otherwise it can fail by acting incorrectly or even lose some events.
     *
     * Another interesting thing is that this method does not receive the new
     * value of the changed property. It's necessary to get the value from the
     * Video class itself, because once this method is called the property is
     * already changed in the class.
     */

    /* For our Video class we will ony watch for the following
     * property:
     *
     * - Video::STATUS  : Changes when the video status has changed.
     */

    /* We watch for STATUS changes here, so we can send a signal when
     * this video has become available, stopped, paused or become no longer
     * available.
     */
    if (prop == Video::P_STATUS) {
        // Get the video status.
        Video::STATUS status;
        GetPropStatus(status);

        // Send the current status to any interested parties.
        statusChanged(this, status);
    }
}

};

};

