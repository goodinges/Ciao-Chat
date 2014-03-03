
#ifndef _CONTACTDATA_H
#define _CONTACTDATA_H

#include <Poco/Data/BLOB.h>

namespace Mvp
{

namespace Model
{

enum ContactAvailability
{
    Available = 1,
    Away,
    Busy,
    SkypeOut,
    Offline
};

struct ContactData
{
    // The contact's skypename.
    std::string skypeName;
    // The contact's displayname.
    std::string displayName;
    // The contact's availability as a string.
    std::string availability;
    // The contact's availability as a ContactAvailability enumeration.
    ContactAvailability status;
    // The contact's avatar image in a Binary BLOB.
    Poco::Data::BLOB avatar;
    // True if the contact can receive instant messages.
    bool canIM;
    // True if the contact is PSTN type, or has a mobile number set.
    bool canSMS;
    // True if the contact can receive calls.
    bool canCall;
    // True if the contact can transfer files.
    bool canFT;
};

};

};
#endif
