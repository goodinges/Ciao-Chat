
#ifndef _HISTORYDATA_H
#define _HISTORYDATA_H

#include <Poco/Data/BLOB.h>

namespace Mvp
{

namespace Model
{

enum HistoryType
{
    IncomingCall = 1,
    OutgoingCall,
    MissedCall,
    FailedCall,
    Chat,
    ConferenceIn,
    ConferenceOut,
    MissedConference,
    VoiceMail,
    OutgoingTransfer,
    IncomingTransfer
};

struct HistoryData
{
    // The type of history item.
    HistoryType type;
    // True if this item has been consumed, false otherwise.
    bool read;
    // The displayname of this history item.
    std::string displayName;
    // The text of this history item (only used for Chat type).
    std::string text;
    // The conversation identity of the history item.
    std::string conversationIdentity;
    // The message Id of the message that this history item is from.
    Poco::Data::BLOB messageId;
    // The timestamp of the message that this history item is from.
    unsigned int timestamp;
};

};

};

#endif


