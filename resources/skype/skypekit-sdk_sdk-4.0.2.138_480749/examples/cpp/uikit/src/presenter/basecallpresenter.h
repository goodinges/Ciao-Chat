#ifndef _BASECALLPRESENTER_H
#define _BASECALLPRESENTER_H

#include "abstractpresenter.h"
#include "model/abstractcallservice.h"
#include "view/abstractcallview.h"

#include <Poco/BasicEvent.h>
#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class BaseCallPresenter basecallpresenter.h basecallpresenter.h
 * @brief Base class to define the interface for CallPresenters.
 *
 * CallPresenter is responsible for connecting the CallService to the
 * CallView, manipulating the CallView (show, hide, etc.) and passing
 * data from one to the other and back.
 *
 * This class is a base class that does the connecting of poco signals to slots.
 *
 * @see AbstractPresenter, AbstractCallService and AbstractCallView.
 */
class BaseCallPresenter : public AbstractPresenter<Mvp::View::AbstractCallView>
{
public:
    /**
     * Constructor.
     *
     * Takes the model to use for this presenter.
     *
     * @param model AbstractCallService to use for this call.
     */
    BaseCallPresenter(Mvp::Model::AbstractCallService *model)
        :m_model(model)
    {
        // Connect the model's signals to our slots.
        m_model->liveStatusChanged +=
            Poco::Delegate<BaseCallPresenter, Conversation::LOCAL_LIVESTATUS>
            (this, &BaseCallPresenter::onLiveStatusChanged);

        m_model->participantListChanged +=
            Poco::Delegate<BaseCallPresenter, Poco::EventArgs>
            (this, &BaseCallPresenter::onParticipantListChanged);

        m_model->contactChanged += Poco::Delegate
            <BaseCallPresenter, Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseCallPresenter::onContactChanged);

        m_model->localVideoAvailable +=
            Poco::Delegate<BaseCallPresenter, bool>
            (this, &BaseCallPresenter::onLocalVideoAvailable);

        m_model->remoteVideoAvailable +=
            Poco::Delegate<BaseCallPresenter, bool>
            (this, &BaseCallPresenter::onRemoteVideoAvailable);
    }

    /**
     * Destructor.
     */
    virtual ~BaseCallPresenter()
    {
        // Connect the model's signals to our slots.
        m_model->liveStatusChanged -=
            Poco::Delegate<BaseCallPresenter, Conversation::LOCAL_LIVESTATUS>
            (this, &BaseCallPresenter::onLiveStatusChanged);

        m_model->participantListChanged -=
            Poco::Delegate<BaseCallPresenter, Poco::EventArgs>
            (this, &BaseCallPresenter::onParticipantListChanged);

        m_model->contactChanged -= Poco::Delegate
            <BaseCallPresenter, Poco::SharedPtr<Mvp::Model::ContactData> >
            (this, &BaseCallPresenter::onContactChanged);

        m_model->localVideoAvailable -=
            Poco::Delegate<BaseCallPresenter, bool>
            (this, &BaseCallPresenter::onLocalVideoAvailable);

        m_model->remoteVideoAvailable -=
            Poco::Delegate<BaseCallPresenter, bool>
            (this, &BaseCallPresenter::onRemoteVideoAvailable);
    }

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    virtual void setView(Mvp::View::AbstractCallView *view) = 0;

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    virtual void showView() = 0;

    /**
     * Slot to handle conversation status changed signals.
     *
     * Whenever the conversation's live status changes, this method will be
     * called to handle the change.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param liveStatus new conversation live status.
     */
    virtual void onLiveStatusChanged(const void *sender,
        Conversation::LOCAL_LIVESTATUS &liveStatus) = 0;

    /**
     * Slot to handle participant list changes.
     *
     * @param sender a pointer to the object that sent the signal.
     *
     * @note the new participant list is not passed in the signal, but must
     * be retrieved by the implementation code from the service.
     */
    virtual void onParticipantListChanged(const void *sender,
        Poco::EventArgs &args) = 0;

    /**
     * Slot to handle requests to manage the conversation.
     *
     * @param sender a pointer to the object that sent  the signal.
     */
    virtual void onManageConversation(const void *sender,
        Poco::EventArgs &args) = 0;

    /**
     * Slot to handle contact changes.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param contact the new contact data to use.
     */
    virtual void onContactChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact) = 0;

    /**
     * Signal that the user has requested to manage the conversation.
     */
    Poco::BasicEvent<Poco::EventArgs> manageConversation;

    /**
     * Slot to handle local participant video available signals.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param available whether the video is available or not.
     */
    virtual void onLocalVideoAvailable(const void *sender, bool &available) = 0;

    /**
     * Slot to handle remote participant video available signals.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param available whether the video is available or not.
     */
    virtual void onRemoteVideoAvailable(const void *sender, bool &available) = 0;

    /**
     * Slot to handle DTMF tones.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param name the name of the pressed key.
     */
    virtual void onDtmfTone(const void *sender, std::string &name) = 0;

    /**
     * Signal that the user has requested to view the video.
     */
    Poco::BasicEvent<Poco::EventArgs> showVideo;

protected:
    /**
     * Slot to react to view close signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onViewClose(const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Slot to react to the accepted signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onCallAccepted(const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Slot to react to the rejected signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onCallRejected(const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Slot to react to the ended signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     */
    virtual void onCallEnded(const void *sender, Poco::EventArgs &args) = 0;

    /**
     * Slot to react to the mute signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param muted true to mute the call, false to unmute.
     */
    virtual void onCallMute(const void *sender, bool &muted) = 0;

    /**
     * Slot to react to the start local video signal from the view.
     *
     * @param sender a pointer to the sender of the signal.
     * @param start true to start local video, false to stop it.
     */
    virtual void onStartLocalVideo(const void *sender, bool &start) = 0;

    Mvp::Model::AbstractCallService *m_model;
};

};

};
#endif
