#ifndef _SKYPEMESSAGE_H
#define _SKYPEMESSAGE_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

/**
 * @class SkypeMessage skypemessage.h skypemessage.h
 * @brief Implements basic features of the SkypeKit Message class.
 *
 * In order to intercept the message's property change events it's necessary
 * to subclass the Message class and overwrite some basic virtual methods that
 * will be called when a property is changed. The most important virtual method
 * is:
 *
 * void OnChange(int prop): This method will be called whenever a property is
 * changed. Here it's possible to catch all changes related to the message.
 * P_BODY_XML is the only one we are really concerned with here.
 */
class SkypeMessage : public Message
{
public:
    typedef DRef<SkypeMessage, Message> Ref;

    /**
     * Destructor.
     */
    ~SkypeMessage();

    /**
     * Signal that the message has changed.
     *
     * We will emit them all and let the historyservice decide what to do.
     */
    Poco::BasicEvent<const MessageRef> messageChanged;

    /**
     * Signal that the sending status changed.
     */
    Poco::BasicEvent<Message::SENDING_STATUS> statusChanged;

protected:
    /**
     * Constructor.
     */
    SkypeMessage(unsigned int oid, SERootObject *root);

    /**
     * Reflects property changes.
     *
     * When a Message's property is changed this method will be called.
     * It's important to know that when this method is called the property
     * itself is already changed inside the Message's class, which means
     * that you have to get the new property value by using the property index
     * passed to this method.
     *
     * @param prop the property index.
     */
    void OnChange(int prop);

private:
    friend class SkypeCore;
};

};

};

#endif

