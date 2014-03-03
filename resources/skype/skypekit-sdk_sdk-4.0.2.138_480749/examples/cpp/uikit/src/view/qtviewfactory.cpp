
#include "qtviewfactory.h"

#include "qtcallview.h"
#include "qtcontactlistview.h"
#include "qtconvmanagerview.h"
#include "qthistoryview.h"
#include "qtloginview.h"
#include "qttextchatview.h"
#include "qtftview.h"

#include <QDebug>
#include <QMetaType>

namespace Mvp
{

namespace View
{

// Declare AbstractViewFactory::m_instance so it exists.
AbstractViewFactory *AbstractViewFactory::m_instance = 0;

AbstractViewFactory *QtViewFactory::instance()
{
    // if m_instance hasn't been allocated, allocate it of our type.
    if (!m_instance)
        m_instance = new QtViewFactory;

    return m_instance;
}

QtViewFactory::~QtViewFactory()
{
}

int QtViewFactory::requestView(AbstractViewFactory::ViewType type)
{
    int id = ++m_lastRequestId;
    /* Here we use QMetaObject::invokeMethod to ask the Qt GUI thread to
     * create the view in its event loop.
     */
    qDebug() << "view requested of type " << type
        << QMetaObject::invokeMethod(this, "createView", Qt::QueuedConnection,
        Q_ARG(AbstractViewFactory::ViewType, type), Q_ARG(int, id));

    return id;
}

void QtViewFactory::createView(AbstractViewFactory::ViewType type, int requestId)
{
    // Create a NULL view.
    AbstractView *view = 0;

    switch (type)
    {
        case LoginView:
            view = new QtLoginView;
            break;
        case ContactListView:
            view = new QtContactListView;
            break;
        case TextChatView:
            view = new QtTextChatView;
            break;
        case CallView:
            view = new QtCallView;
            break;
        case ConvManagerView:
            view = new QtConvManagerView;
            break;
        case HistoryView:
            view = new QtHistoryView;
            break;
        case FTView:
            view = new QtFTView;
            break;
    }

    // If the view is created, emit the viewCreated signal.
    if (view) {
        std::pair<int, AbstractView*> pair(requestId, view);
        viewCreated(this, pair);
    }
}

QtViewFactory::QtViewFactory()
    :m_lastRequestId(0)
{
    // Register our meta type so our slot can work.
    qRegisterMetaType<AbstractViewFactory::ViewType>("AbstractViewFactory::ViewType");
}

};

};
