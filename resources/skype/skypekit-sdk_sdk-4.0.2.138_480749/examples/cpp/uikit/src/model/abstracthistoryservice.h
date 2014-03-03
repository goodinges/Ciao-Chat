
#ifndef _ABSTRACTHISTORYSERVICE_H
#define _ABSTRACTHISTORYSERVICE_H

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>
#include <Poco/SharedPtr.h>

#include <vector>

#include "historydata.h"

namespace Mvp
{

namespace Model
{

/**
 * @class AbstractHistoryService abstracthistoryservice.h abstracthistoryservice.h
 * @brief Abstract History Service class.
 *
 * This class provides the base interface that needs to be implemented
 * by the History services, such as, and specifically, fetching the
 * history from the user account and signals to propagate any history
 * changes.
 *
 * @note This class is abstract. If you're looking for a concrete one try
 * looking at HistoryService class.
 */
class AbstractHistoryService {
public:
    AbstractHistoryService(){};
    virtual ~AbstractHistoryService(){};

    /**
     * Returns a list of history data.
     *
     * When requested the method will return a vector containing a set of
     * history data representing each real Skype Conversation. This history
     * data only provides some data about the real conversation and is
     * identified by the conversation's identity value.
     *
     * @param hasUnread comes back true if there are unread items in the history.
     * @returns a vector containing history data.
     * @see HistoryData
     */
    virtual std::vector<Poco::SharedPtr<HistoryData> > getHistory(
        bool &hasUnread) = 0;

    /**
     * Mark all history item messages as read.
     */
    virtual void markMessagesRead() = 0;

    /**
     * Signal for history changes notification.
     *
     * This signal is emitted when a new item should be added to the history
     * list.
     */
    Poco::BasicEvent<Poco::EventArgs> historyChanged;
};

};

};
#endif

