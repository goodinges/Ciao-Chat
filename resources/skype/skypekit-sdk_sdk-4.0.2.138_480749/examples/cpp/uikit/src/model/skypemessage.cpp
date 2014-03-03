#include "skypemessage.h"

namespace Mvp
{

namespace Model
{

SkypeMessage::SkypeMessage(unsigned int oid, SERootObject* root)
    : Message(oid, root)
{
}

SkypeMessage::~SkypeMessage()
{
}

void SkypeMessage::OnChange(int prop)
{
    /* Here is the place where you can capture all property changes when they
     * happen. It's important to know that this method will receive ALL changes
     * from ALL properties, which means that it's necessary to create a nice and
     * robust filter inside this method to handle each type of property,
     * otherwise it can fail by acting incorrectly or even lose some events.
     *
     * Another interesting thing is that this method does not receive the new
     * value of the changed property. It's necessary to get the value from the
     * Message class itself, because once this method is called the property is
     * already changed in the class.
     */
    if (prop == Message::P_BODY_XML || prop == Message::P_CONSUMPTION_STATUS) {
        MessageRef message = this->ref();
        messageChanged(this, message);
    }
    else if (prop == Message::P_SENDING_STATUS) {

        // Get the sending status.
        Message::SENDING_STATUS status;
        GetPropSendingStatus(status);

        // Then send it on.
        statusChanged(this, status);
    }
}

};

};

