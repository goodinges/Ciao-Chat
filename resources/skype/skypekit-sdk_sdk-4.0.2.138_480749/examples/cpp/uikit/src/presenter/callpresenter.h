#ifndef _CALLPRESENTER_H
#define _CALLPRESENTER_H

#include "basecallpresenter.h"
#include "view/abstractcallview.h"
#include "model/abstractcallservice.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class CallPresenter callpresenter.h callpresenter.h
 * @brief Class to present call information via CallView using CallService.
 *
 * This class is responsible for connecting the CallService to the CallView,
 * manipulating the CallView (show, hide, etc.) and passing data from one
 * to the other and back.
 *
 * @see AbstractPresenter, AbstractCallService and AbstractCallView.
 *
 */
class CallPresenter : public BaseCallPresenter
{
public:
    /**
     * Constructor.
     *
     * @param model AbstractCallService to use for this call.
     */
    CallPresenter(Mvp::Model::AbstractCallService *model);

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    void setView(Mvp::View::AbstractCallView *view);

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    void showView();

protected:
    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onViewClose(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to the accepted signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onCallAccepted(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to the rejected  signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onCallRejected(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to the ended signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    void onCallEnded(const void *sender, Poco::EventArgs &args);

    /**
     * Slot to react to the mute  signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param muted true to mute, false to unmute.
     */
    void onCallMute(const void *sender, bool &muted);

    /**
     * Slot to react to the hold signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param held true to hold the call, false to resume.
     */
    void onCallHold(const void *sender, bool &held);

    /**
     * Slot to handle conversation status changed signal.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param liveStatus new conversation live status.
     */
    void onLiveStatusChanged(const void *sender,
        Conversation::LOCAL_LIVESTATUS &liveStatus);

    /**
     * Slot to handle contact changes.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param contact the new contact data to use.
     */
    void onContactChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact);

    /**
     * Slot to handle participant list changes.
     *
     * @param sender a pointer to the object that sent the signal.
     *
     * @note the new participant list is not passed in the signal, but must
     * be retrieved by the implementation code from the service.
     */
    void onParticipantListChanged(const void *sender,
        Poco::EventArgs &args);

    /**
     * Slot to handle local participant video available signals.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param available whether the video is available or not.
     */
    void onLocalVideoAvailable(const void *sender, bool &available);

    /**
     * Slot to handle remote participant video available signals.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param available whether the video is available or not.
     */
    void onRemoteVideoAvailable(const void *sender, bool &available);

    /**
     * Slot to handle DTMF tones.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param name the name of the pressed key.
     */
    void onDtmfTone(const void *sender, std::string &name);

    /**
     * Slot to handle requests to manage the conversation.
     *
     * @param sender a pointer to the object that sent  the signal.
     */
    void onManageConversation(const void *sender,
        Poco::EventArgs &args);

    /**
     * Slot to react to the start local video signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param start true to start local video, false to stop it.
     */
    void onStartLocalVideo(const void *sender, bool &start);
};

};

};
#endif

