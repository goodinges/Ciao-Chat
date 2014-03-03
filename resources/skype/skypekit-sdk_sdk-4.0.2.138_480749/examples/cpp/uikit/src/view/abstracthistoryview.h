
#ifndef _ABSTRACTHISTORYVIEW_H
#define _ABSTRACTHISTORYVIEW_H

#include "abstractview.h"

#include <Poco/BasicEvent.h>
#include <Poco/SharedPtr.h>

#include <string>

#include "model/historydata.h"

namespace Mvp
{

namespace View
{

/**
 * @class AbstractHistoryView abstracthistoryview.h
 * abstracthistoryview.h
 * @brief Abstract base class declaring all methods that HistoryViews should
 * implement.
 *
 * This class provides all of the information and interfaces presenters need to
 * work with any HistoryView object.  This includes methods to
 * initially set the history.  It also includes signals for calling
 * and starting a chat based an item in the history.
 *
 * @see QtHistoryView
 */
class AbstractHistoryView : public AbstractView
{
public:
    /**
     * Destructor.
     */
    virtual ~AbstractHistoryView(){};

    /**
     * Set the history for initial display.
     *
     * When the history presenter is initialized, it asks the model
     * for the list of history data, then passes that list to the view via this
     * setHistory method.
     *
     * @note Subclasses need to implement this, and are free to cache any
     * data passed in for future use.
     * @note If any previous data was cached it should be cleared at this point.
     *
     * @param history A vector of history data to use initially in the view.
     * @param unreadItems true if there are unread items in history, false
     * otherwise.
     */
    virtual void setHistory(
        std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> > history,
        bool unreadItems) = 0;

    /**
     * A signal to call a given conversation.  The conversation identifier is
     * passed in the signal.
     */
    Poco::BasicEvent<const std::string> callConversation;

    /**
     * A signal to start a chat with a given conversation.  The conversation
     * identifier is passed in the signal.
     */
    Poco::BasicEvent<const std::string> chatConversation;

    /**
     * A signal to mark all the history as read.
     */
    Poco::BasicEvent<Poco::EventArgs> markHistoryRead;
};

};

};
#endif
