#ifndef _ABSTRACTCALLVIEW_H
#define _ABSTRACTCALLVIEW_H

#include <string>

#include "abstractview.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

#include "model/contactdata.h"

namespace Mvp
{

namespace View
{

/**
 * @class AbstractCallView abstractcallview.h abstractcallview.h
 * @brief Abstract base class declaring all methods CallViews should implement.
 *
 * This class provides all of the information and interfaces presenters need to
 * work with any CallView object.
 *
 * @see QtCallView
 */
class AbstractCallView : public AbstractView
{
public:

    enum CallStatus
    {
        Starting,
        Ringing,
        Ongoing,
        OnHold,
        Done
    };

    /**
     * Set the title of the call window.
     *
     * @param title the title to show for this call view.
     */
    virtual void setCallTitle(const std::string &title) = 0;

    /**
     * Show the given status code in the user interface.
     *
     * Subclasses should implement this to show the given status code somehow
     * in the interface, within the guidelines of the skypekit certification
     * documents.
     *
     * @param status the status code to show.
     */
    virtual void setCallStatus(CallStatus status) = 0;

    /**
     * Set the participant list.
     *
     * When the call presenter is initialized, it asks the model  for the list
     * of participants, then passes that list to the view via this
     * setParticipants method.
     *
     * @param contacts A vector of contact data to use initially in the view.
     */
    virtual void setParticipants(
            std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts) = 0;

    /**
     * Set local video available status.
     *
     * Once the call is started we need to enable or disable the local video
     * support according to our local capabilities.
     *
     * @param available Set to true if there's video support, otherwise false.
     */
    virtual void setVideoAvailable(bool available) = 0;

    /**
     * Update the given contact's contact data.
     *
     * @param contact the contact's new contact data to use.
     */
    virtual void updateContact(
        Poco::SharedPtr<Mvp::Model::ContactData> &contact) = 0;

    /**
     * Get the local video widget's handle.
     *
     * @returns the window handle of the local video widget.
     */
    virtual uint getLocalVideoWidget() = 0;

    /**
     * Get the remote video widget's handle.
     *
     * @returns the window handle of the remote video widget.
     */
    virtual uint getRemoteVideoWidget() = 0;

    /**
     * Show/Hide the remote video widget.
     *
     * @param show true to show, false to hide the remote video widget.
     */
    virtual void showRemoteVideo(bool show) = 0;

    /**
     * Signal to notify when the call is on hold.
     */
    Poco::BasicEvent<bool> hold;

    /**
     * Signal to notify when the call is muted.
     */
    Poco::BasicEvent<bool> mute;

    /**
     * Signal to notify when the call is ended.
     */
    Poco::BasicEvent<Poco::EventArgs> end;

    /**
     * Signal to notify that the call was accepted.
     */
    Poco::BasicEvent<Poco::EventArgs> accepted;

    /**
     * Signal to notify that the call was rejected.
     */
    Poco::BasicEvent<Poco::EventArgs> rejected;

    /**
     * Signal to notify that the user wants to manage the participants.
     */
    Poco::BasicEvent<Poco::EventArgs> manage;

    /**
     * Signal to notify that the user wants to send his video to the
     * other participant.
     */
    Poco::BasicEvent<bool> startLocalVideo;

    /**
     * Signal to nofify that the user wants to send a DTMF tone.
     */
    Poco::BasicEvent<std::string> dtmfTone;
};

};

};
#endif
