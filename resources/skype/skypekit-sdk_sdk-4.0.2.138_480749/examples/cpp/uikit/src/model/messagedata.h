
#ifndef _MESSAGEDATA_H
#define _MESSAGEDATA_H

#include <string>

namespace Mvp
{

namespace Model
{

struct MessageData
{
    // The skype name of the author of the message.
    std::string author;
    // The display name of the author of the message.
    std::string authorDisplayname;
    // The text of the body of the message.
    std::string body;
    // Whether or not the message is an emote.
    bool isEmote;
    // The timestamp of the message.
    unsigned int timestamp;
};

};

};
#endif
