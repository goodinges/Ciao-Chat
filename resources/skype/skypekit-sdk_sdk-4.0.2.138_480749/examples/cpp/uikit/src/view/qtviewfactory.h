#ifndef _QTVIEWFACTORY_H
#define _QTVIEWFACTORY_H

#include "abstractviewfactory.h"

#include <QObject>

namespace Mvp
{

namespace View
{

/**
 * @class QtViewFactory qtviewfactory.h qtviewfactory.h
 * @brief This factory creates Qt views as requested.
 *
 * This class is a view factory that creates views using the Qt GUI toolkit.
 *
 * @note Applications using this factory should call QtViewFactory::instance()
 *   at least once to initialize this as the ViewFactory for states to use to
 *   create views.
 *
 * @see AbstractViewFactory
 */
class QtViewFactory: public QObject, public AbstractViewFactory
{
Q_OBJECT
public:
    /**
     * Create AbstractViewFactory::m_instance as an instance of QtViewFactory.
     *
     * This method returns the existing AbstractViewFactory::m_instance if it
     * has been allocated, otherwise it allocates a new QtViewFactory and
     * returns.
     *
     * @returns A pointer to the instance of AbstractViewFactory.
     */
    static AbstractViewFactory *instance();

    /**
     * Destructor
     */
    ~QtViewFactory();

    /**
     * Request a view of the given type.
     *
     * @param type The type of view requested.
     *
     * @returns a unique id for the request.
     */
    int requestView(AbstractViewFactory::ViewType type);

private slots:
    /**
     * Slot to actually create the Qt view.
     *
     * This method creates the view that has been requested, and emits
     * the AbstractViewFactory::viewCreated signal with a pointer to
     * the new view.
     *
     * @note This slot is needed so the Qt views will be created on the Qt
     * GUI thread.
     *
     * @param type The type of view requested.
     * @param requestId the id of the request.
     */
    void createView(AbstractViewFactory::ViewType type, int requestId);

private:
    Q_DISABLE_COPY(QtViewFactory)

    /**
     * This is a singleton, so its constructor is private.
     */
    QtViewFactory();

    int m_lastRequestId;
};

};

};
#endif
