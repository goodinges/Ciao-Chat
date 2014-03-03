#ifndef _SKYPESMS_H
#define _SKYPESMS_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

/**
 * @class SkypeSms skypesms.h skypesms.h
 * @brief Implements basic features of the SkypeKit Sms class.
 *
 * In order to intercept the sms's property change events it's necessary
 * to subclass the Sms class and overwrite some basic virtual methods that
 * will be called when a property is changed. The most important virtual method
 * is:
 *
 * void OnChange(int prop): This method will be called whenever a property is
 * changed. Here it's possible to catch all changes related to the sms.
 * Status is the only one we are really concerned with here.
 */
class SkypeSms : public Sms
{
public:
    typedef DRef<SkypeSms, Sms> Ref;

    /**
     * Destructor.
     */
    ~SkypeSms();

    /**
     * Signal that the sms status has changed.
     */
    Poco::BasicEvent<Sms::STATUS> statusChanged;

protected:
    /**
     * Constructor.
     */
    SkypeSms(unsigned int oid, SERootObject *root);

    /**
     * Reflects property changes.
     *
     * When a Sms's property is changed this method will be called.
     * It's important to know that when this method is called the property
     * itself is already changed inside the Sms's class, which means
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

