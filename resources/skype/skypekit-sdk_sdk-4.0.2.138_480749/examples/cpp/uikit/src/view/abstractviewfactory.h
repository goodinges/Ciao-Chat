#ifndef _ABSTRACTVIEWFACTORY_H
#define _ABSTRACTVIEWFACTORY_H

#include "abstractview.h"

#include <Poco/BasicEvent.h>

namespace Mvp
{

namespace View
{

/**
 * @class AbstractViewFactory abstractviewfactory.h abstractviewfactory.h
 * @brief Abstract base class of a factory to create views from.
 *
 * In order to create views without knowledge of the type of the views an
 * abstract factory is needed.  This header can then be included in other
 * places and used to create views.  This class also uses the singleton pattern,
 * so there can only ever be one object of this type per application.
 *
 * The views created by factories are created asynchronously, so interested
 * parties should connect to the viewCreated signal, then call requestView(),
 * passing in the type of view they would like created.
 *
 * @note Subclasses should define and initialize m_instance in their cpp file.
 *
 * @see QtViewFactory
 */
class AbstractViewFactory
{
public:
    /**
     * Singleton accessor.
     *
     * This class implements the singleton pattern, so an accessor
     * is needed.  instance() is that accessor.
     *
     * @return A pointer to the singleton object.
     */
    static AbstractViewFactory *instance(){ return m_instance; }

    /**
     * Enumeration of possible view types.
     */
    enum ViewType
    {
        LoginView,
        ContactListView,
        TextChatView,
        CallView,
        ConvManagerView,
        HistoryView,
        FTView
    };

    /**
     * Request that a view be created of given type.
     *
     * In order for a state to create a view to pass to its presenter object
     * it needs to implement a catchView slot, connect it to the
     * AbstractViewFactory::viewCreated signal, and then call the requestView()
     * method.  This method will create the view, initialize its properties
     * then emit the viewCreated signal so any interested objects can then use
     * the newly created view.
     *
     * @param type The type of view to be created.
     *
     * @returns a unique request id for this request.
     */
    virtual int requestView(ViewType type) = 0;

    /**
     * Signal emitted when a view is created.
     *
     * When the gui thread creates a view it emits this signal to pass the
     * pointer to the view to the object that requested the view.
     */
    Poco::BasicEvent<std::pair<int, AbstractView*> > viewCreated;

protected:
    /**
     * Singleton instance pointer.
     *
     * @note Subclasses should define and initialize this in their cpp file.
     */
    static AbstractViewFactory *m_instance;
};

};

};
#endif
