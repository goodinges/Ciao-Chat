
// Inclusion of ssl.h here is for "fixing" a naming conflict
// between constants in the ssl lib and windows libraries.
// Removing this include will cause your prjocet build to fail
// with syntax errors in ssl.h on windows.
// The video frames are -not- actually encrypted.
#ifdef WIN32
    #include <ssl.h>
#endif

#include "videowidget.h"
#include "QSKClasses.h"

#include <QPainter>

#define SID_MAKEFOURCC(ch0, ch1, ch2, ch3) \
        ( (unsigned long)(unsigned char)(ch0)         | ( (unsigned long)(unsigned char)(ch1) << 8 ) | \
        ( (unsigned long)(unsigned char)(ch2) << 16 ) | ( (unsigned long)(unsigned char)(ch3) << 24 ) )


VideoWidget::VideoWidget( QWidget *parent ) : QWidget( parent )
{
    // Here we will inform the runtime that we are capable of drawing
    // RGB32 and RGB24 bitmaps. The runtime will pick the most suitable
    // of those. Format of the frames we receive back will be included in the
    // bufferstruct's colorspace and bitcount fields

    unsigned int list[2];
    list[0] = SID_MAKEFOURCC('B','I','3','2');
    list[1] = SID_MAKEFOURCC('B','I','2','4');
    ipc.setPreferences( 2, list );
    currentwidth = 0;
};

void VideoWidget::setParticipantName (QString newName)
{
    participantName = newName;
    this->setWindowTitle(participantName);
};

// Starts polling the transport mechanism for new frames.
void VideoWidget::renderStart()
{    
    timerId = startTimer(10);
};

// Stops the timer.
void VideoWidget::renderStop()
{
    killTimer(timerId);
};

// Handling widget close event
void VideoWidget::closeEvent ( QCloseEvent* )
{
    renderStop();
};

void VideoWidget::timerEvent( QTimerEvent * )
{
    // Every now and then, let's check if there is a new frame available.
    // ipc.getNewFrame only checks for a new frame.
    VideoTransportBase::bufferstruct *buffer = ipc.getNewFrame();
    if ( !buffer ) return;

    // Frame size has changed?
    if ( currentwidth != buffer->width )
    {
        resize( buffer->width, buffer->height );
        currentwidth = buffer->width;

        QString newCaption = participantName + " ";
        newCaption.append(QString::number(buffer->width));
        newCaption.append("x");
        newCaption.append(QString::number(buffer->height));
        setWindowTitle(newCaption);
    }
    // Update will trigger paintEvent
    update();
};

// paintEvent will be called from the Qt main thread.
void VideoWidget::paintEvent( QPaintEvent* )
{
    // Retrieving current state of the frame buffer.
    VideoTransportBase::bufferstruct *buffer = ipc.getFrame();
    if ( !buffer ) return;

    // Now we need to check the actual bitmap format of the frame
    QImage::Format format;

    if      ( buffer->fourcc == SID_MAKEFOURCC('B','I','3','2') ) format = QImage::Format_RGB32;
    else if ( buffer->fourcc == SID_MAKEFOURCC('B','I','2','4') ) format = QImage::Format_RGB888;
    else if ( buffer->fourcc == 0  )
    {
        // FOURCC == 0 is also a valid value, in which case the actual format can be
        // either RGB32, RGB24 or RGB 16. Between those we can choose based on buffer's bitcount

        if      ( buffer->bitsperpixel == 32 ) format = QImage::Format_RGB32;
        else if ( buffer->bitsperpixel == 24 ) format = QImage::Format_RGB888;
        else if ( buffer->bitsperpixel == 16 ) format = QImage::Format_RGB16;
    };

    // Creating image and drawing it..
    QImage img( (const uchar *)ipc.bufferData(buffer), buffer->width, buffer->height, format );
    QPainter painter( this );
    painter.drawImage( rect(), img );
};

