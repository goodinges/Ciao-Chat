# -------------------------------------------------
# SkypeKit video calls with Qt
# -------------------------------------------------

CONFIG -= app_bundle
CONFIG += x86

TARGET = videocalls
TEMPLATE = app
DEFINES += SKYPEKIT_SURFACE_RENDERING
DEFINES += SSL_LIB_CYASSL

win32 {
    DEFINES += _WIN32
}

# Note that this needs to match with transport used in the C++ wrapper lib.
# If you are unsure, check defines in skypekit-cppwrapper_2_lib project.

# In case of Windows, we only have one transport mechanism - shared memory, which will
# be switched on based on _WIN32 define (see /ipc/cpp/VideoBuffers/videotransport.h)
!win32 {
    DEFINES += VIDEO_TRANSPORT_SYSV
    DEFINES += VIDEO_TRANSPORT_POSIX
}

DISTROROOT = ../../../
WINLIBPATH = "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib/"

SOURCES +=  main.cpp \
            mainwindow.cpp \
            QSKClasses.cpp \
            videowidget.cpp

HEADERS += mainwindow.h \
    QSKClasses.h \
    videowidget.h

FORMS += mainwindow.ui

# ---------------------------------------------------------
# Common SkypeKit include directories for all platforms
# ---------------------------------------------------------
INCLUDEPATH += $${DISTROROOT}interfaces/skype/cpp_embedded/src/api \
    $${DISTROROOT}interfaces/skype/cpp_embedded/src/ipc \
    $${DISTROROOT}interfaces/skype/cpp_embedded/src/types \
    $${DISTROROOT}interfaces/skype/cpp_embedded/src/platform/threading \
    $${DISTROROOT}ipc/cpp \
    $${DISTROROOT}ipc/cpp/platform/se \
    $${DISTROROOT}ipc/cpp/ssl/cyassl/include \
    $${DISTROROOT}/ipc/cpp/ssl/cyassl/ctaocrypt/include \
    $${DISTROROOT}/ipc/cpp/ssl/cyassl/include/openssl \
    $${DISTROROOT}ipc/cpp/VideoBuffers

# -----------------------------------------------------------
# Threading include dir. and wrapper library for Linux & Mac
# -----------------------------------------------------------
!win32 { 
    INCLUDEPATH += $${DISTROROOT}interfaces/skype/cpp_embedded/src/platform/threading/pthread

    LIBS += $${DISTROROOT}interfaces/skype/cpp_embedded/build/libskypekit-cppwrapper_2_lib.a
    LIBS += $${DISTROROOT}interfaces/skype/cpp_embedded/build/libskypekit-cyassl_lib.a
}

# ---------------------------------------------------------
# Threading include dir. and wrapper library for Windows
# NB! The WS2_32.Lib is (I think) not included in Express
# versions of the MS Visual C++
# The Advapi32.lib is required by the cyassl lib.
# ---------------------------------------------------------
win32 { 
    INCLUDEPATH += $${DISTROROOT}interfaces/skype/cpp_embedded/src/platform/threading/win

    LIBS += $${WINLIBPATH}WS2_32.Lib
    LIBS += $${WINLIBPATH}Advapi32.lib
    LIBS += $${DISTROROOT}interfaces/skype/cpp_embedded/build/skypekit-cyassl_lib.lib
    LIBS += $${DISTROROOT}interfaces/skype/cpp_embedded/build/skypekit-cppwrapper_2_lib.lib
}
