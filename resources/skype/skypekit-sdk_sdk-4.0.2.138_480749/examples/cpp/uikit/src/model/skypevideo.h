#ifndef _SKYPEVIDEO_H
#define _SKYPEVIDEO_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

/**
 * @class SkypeVideo skypevideo.h skypevideo.h
 * @brief Implements basic features of the SkypeKit Video class.
 *
 * In order to intercept the video's property change events it's necessary
 * to subclass the Video class and overwrite some basic virtual methods that
 * will be called when a property is changed. The most important virtual method
 * is:
 *
 * void OnChange(int prop): This method will be called whenever a property is
 * changed. Here it's possible to catch all changes related to the video.
 * Status is the only one we are really concerned with here.
 *
 * When a Participant' VideoStatus changes to Available, that means GetVideo
 * will return a valid Video object.  Then that Video object's status can be
 * checked to see if it's Available for streaming yet or not.  If it's not ready
 * for streaming yet, we need to watch OnChange here to know when it becomes
 * available for streaming. See CallService::onVideoStatusChanged for an example
 * of how to catch those events.
 */
class SkypeVideo : public Video
{
public:
    typedef DRef<SkypeVideo, Video> Ref;

    /**
     * Destructor.
     */
    ~SkypeVideo();

    /**
     * Signal that the video status has changed.
     *
     * When the video status changes we need to emit this signal
     * so any interested objects can know the new status. Possible
     * values of Video::STATUS are:
     * Video::NOT_AVAILABLE
     * Video::AVAILABLE
     * Video::STARTING
     * Video::REJECTED
     * Video::RUNNING
     * Video::STOPPING
     * Video::PAUSED
     * Video::NOT_STARTED
     * Video::HINT_IS_VIDEOCALL_RECEIVED
     * Video::UNKNOWN
     * and Video::RENDERING
     * We will emit them all and let the videoservice decide what to do.
     */
    Poco::BasicEvent<Video::STATUS> statusChanged;

protected:
    /**
     * Constructor.
     */
    SkypeVideo(unsigned int oid, SERootObject *root);

    /**
     * Reflects property changes.
     *
     * When a Video's property is changed this method will be called.
     * It's important to know that when this method is called the property
     * itself is already changed inside the Video's class, which means
     * that you have to get the new property value by using the property index
     * passed to this method.
     *
     * @param prop the property index.
     */
    void OnChange(int prop);

private:
    friend class SkypeCore;
};

};

};

#endif

