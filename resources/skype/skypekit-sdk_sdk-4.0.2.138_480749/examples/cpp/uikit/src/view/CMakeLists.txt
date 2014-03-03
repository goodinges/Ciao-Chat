# Create a separated library for Views.
project(qtview)

include_directories(${PROJECT_BINARY_DIR})

SET(view_HDRS
    qtloginview.h
    qtcontactlistview.h
    qtconvmanagerview.h
    qtdialpadwidget.h
    qtftview.h
    qtviewfactory.h
    qthistorymodel.h
    qthistoryview.h
    qtimagecache.h
    qtcontactlistwidget.h
    qtcontactlistmodel.h
    qtcontactlistdelegate.h
    qtcallview.h
    qttextchatview.h
    qtpstnnumber.h)

SET(view_SRCS qtloginview.cpp
    qtcontactlistwidget.cpp
    qtcontactlistmodel.cpp
    qtcontactlistdelegate.cpp
    qtcontactlistview.cpp
    qtconvmanagerview.cpp
    qtdialpadwidget.cpp
    qthistorymodel.cpp
    qthistoryview.cpp
    qtftview.cpp
    qtimagecache.cpp
    qttextchatview.cpp
    qtcallview.cpp
    qtviewfactory.cpp
    qtpstnnumber.cpp)

SET(view_UI
    qt-login.ui
    qt-contactlist.ui
    qt-convmanager.ui
    qt-dialpad.ui
    qt-history.ui
    qt-textchat.ui
    qt-call.ui
    qt-ft.ui
    qt-pstnnumber.ui)

qt4_add_resources(view_RC_SRCS skypeicons.qrc)

QT4_WRAP_UI(UI_SRCS ${view_UI})
QT4_WRAP_CPP(MOC_SRCS ${view_HDRS})

add_library(Mvp-view
            ${view_SRCS}
            ${UI_SRCS}
            ${MOC_SRCS}
            ${view_RC_SRCS})

