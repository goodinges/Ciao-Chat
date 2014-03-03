#ifndef _ABSTRACTVIEW_H
#define _ABSTRACTVIEW_H

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace View
{

/**
 * @class AbstractView abstractview.h abstractview.h
 * @brief Abstract base class defining interfaces all views should implement.
 *
 * A generic view class is used to define what methods all views should
 * implement.  Each view needs an initialize() and showView() method.  They also
 * each have a viewClosed signal to tell the presenter the user has closed the
 * view.
 *
 * @see AbstractLoginView, AbstractContactListView.
 */
class AbstractView
{
public:
    /**
     * Destructor.
     */
    virtual ~AbstractView() {}

    /**
     * This method is called on new views to get them set up to display.
     */
    virtual void initialize() = 0;

    /**
     * This method is called by the presenter (in showView(), which is called
     * by each state object) to ask the view to display itself.
     */
    virtual void showView() = 0;

    /**
     * Destroy the view object.
     *
     * In some gui toolkits, widgets must be deleted in the same thread as they are created.
     * (Qt is one such toolkit).  So we provide a destroyView() method that can be called to
     * to set up the widget for destruction in the gui thread.
     *
     * @see QtLoginView::destroyView()
     */
    virtual void destroyView() = 0;

    /**
     * A signal to tell the presenter when the view has been closed by the
     * user.
     */
    Poco::BasicEvent<Poco::EventArgs> viewClosed;
};

};

};
#endif
