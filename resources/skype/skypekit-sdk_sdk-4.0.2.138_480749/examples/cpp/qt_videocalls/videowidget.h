
#ifndef _VIDEOWIDGET_H_
#define _VIDEOWIDGET_H_

#include <QWidget>

#include <VideoTransportClient.hpp>
#ifdef WIN32
    #include <WinShm.hpp>
#else
    #include <PosixShm.hpp>
#endif

class VideoWidget : public QWidget
{
public:
    VideoWidget( QWidget *parent = 0 );

    void renderStart();
    void renderStop();
    void closeEvent ( QCloseEvent* event );
    void paintEvent( QPaintEvent* event );
    void timerEvent( QTimerEvent* event );
    int getKey() { return ipc.key(); }

    void setParticipantName (QString newName);

private:
    #ifdef WIN32
        VideoTransportClient <WinShm> ipc;
    #else
        VideoTransportClient <PosixShm> ipc;
    #endif
        int currentwidth;
        int timerId;
        QString participantName;
};

#endif
