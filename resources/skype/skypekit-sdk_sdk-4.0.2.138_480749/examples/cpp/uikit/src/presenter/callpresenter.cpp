#include "callpresenter.h"

#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>
#include <Poco/SharedPtr.h>

#include "view/abstractcallview.h"
#include "model/contactdata.h"

namespace Mvp
{

namespace Presenter
{

CallPresenter::CallPresenter(Mvp::Model::AbstractCallService *model)
    : BaseCallPresenter(model)
{
}

void CallPresenter::setView(Mvp::View::AbstractCallView *view)
{
    // Give the view to the base class for caching.
    AbstractPresenter<Mvp::View::AbstractCallView>::setView(view);

    // Let the view initialize itself.
    m_view->initialize();

    m_view->accepted += Poco::Delegate<CallPresenter, Poco::EventArgs>
        (this, &CallPresenter::onCallAccepted);
    m_view->rejected += Poco::Delegate<CallPresenter, Poco::EventArgs>
        (this, &CallPresenter::onCallRejected);
    m_view->end += Poco::Delegate<CallPresenter, Poco::EventArgs>
        (this, &CallPresenter::onCallEnded);
    m_view->mute += Poco::Delegate<CallPresenter, bool>
        (this, &CallPresenter::onCallMute);
    m_view->hold += Poco::Delegate<CallPresenter, bool>
        (this, &CallPresenter::onCallHold);
    m_view->manage += Poco::Delegate<CallPresenter, Poco::EventArgs>
        (this, &CallPresenter::onManageConversation);
    m_view->startLocalVideo += Poco::Delegate<CallPresenter, bool>
        (this, &CallPresenter::onStartLocalVideo);
    m_view->dtmfTone += Poco::Delegate<CallPresenter, std::string>
        (this, &CallPresenter::onDtmfTone);

    m_view->viewClosed += Poco::Delegate<CallPresenter, Poco::EventArgs>
        (this, &CallPresenter::onViewClose);

    // Give the call view a title.
    m_view->setCallTitle(m_model->callName());

    // Set the conversation participants.
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts;
    contacts = m_model->getParticipants();
    m_view->setParticipants(contacts);

    // Set the initial live status just in case we missed some signal.
    Conversation::LOCAL_LIVESTATUS status = m_model->liveStatus();
    onLiveStatusChanged(this, status);
}

void CallPresenter::showView()
{
    // Then show it.
    m_view->showView();
}

void CallPresenter::onViewClose(const void *sender, Poco::EventArgs &args)
{
    Conversation::LOCAL_LIVESTATUS liveStatus = m_model->liveStatus();

    /* If the user has closed the view and before leaving the call we need to
     * terminate the current ongoing call.
     */
    if (liveStatus != Conversation::NONE &&
        liveStatus != Conversation::RECENTLY_LIVE) {
        onCallEnded(this, args);
    }

    finished(this, args);
}

void CallPresenter::onCallAccepted(const void *sender, Poco::EventArgs &args)
{
    m_model->acceptCall();
}

void CallPresenter::onCallRejected(const void *sender, Poco::EventArgs &args)
{
    m_model->rejectCall();
}

void CallPresenter::onCallEnded(const void *sender, Poco::EventArgs &args)
{
    m_model->endCall();
}

void CallPresenter::onCallMute(const void *sender, bool &muted)
{
    m_model->mute(muted);
}

void CallPresenter::onCallHold(const void *sender, bool &held)
{
    m_model->hold(held);
}

void CallPresenter::onLiveStatusChanged(const void *sender,
        Conversation::LOCAL_LIVESTATUS &liveStatus)
{
    printf("%s: liveStatusChanged: %d\n", m_model->callName().data(),
            liveStatus);

    switch (liveStatus) {
        case Conversation::STARTING:
            m_view->setCallStatus(Mvp::View::AbstractCallView::Starting);
            break;
        case Conversation::RINGING_FOR_ME:
            m_view->setCallStatus(Mvp::View::AbstractCallView::Ringing);
            break;
        case Conversation::IM_LIVE:
            m_view->setCallStatus(Mvp::View::AbstractCallView::Ongoing);
            break;
        case Conversation::ON_HOLD_LOCALLY:
        case Conversation::ON_HOLD_REMOTELY:
            m_view->setCallStatus(Mvp::View::AbstractCallView::OnHold);
            break;
        case Conversation::RECENTLY_LIVE:
            m_view->setCallStatus(Mvp::View::AbstractCallView::Done);
		default:
            break;
    }
}

void CallPresenter::onLocalVideoAvailable(const void *sender, bool &available)
{
    printf("CallPresenter::onLocalVideoAvailable called %d.\n", available);
    m_view->setVideoAvailable(available);
}

void CallPresenter::onRemoteVideoAvailable(const void *sender, bool &available)
{
    printf("CallPresenter::onRemoteVideoAvailable called %d.\n", available);

    if (available) {
        // Get the remote video widget from the view.
        uint widget = m_view->getRemoteVideoWidget();
        // Set the remote video widget to use in the api.
        m_model->setRemoteVideoWidget(widget);
    }

    // As soon as remote video is available, start it.
    m_view->showRemoteVideo(available);
    m_model->startRemoteVideo(available);
}

void CallPresenter::onParticipantListChanged(const void *sender,
         Poco::EventArgs &args)
{
    // Give the call view a title.
    m_view->setCallTitle(m_model->callName());

    // Set the conversation participants.
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts;
    contacts = m_model->getParticipants();

    if (contacts.size() > 2)
        m_view->setVideoAvailable(false);

    m_view->setParticipants(contacts);
}

void CallPresenter::onContactChanged(const void *sender,
    Poco::SharedPtr<Mvp::Model::ContactData> &contact)
{
    m_view->updateContact(contact);
}

void CallPresenter::onManageConversation(const void *sender,
                          Poco::EventArgs &args)
{
    manageConversation(this, args);
}

void CallPresenter::onStartLocalVideo(const void *sender, bool &start)
{
    printf("onStartLocalVideo called %d.\n", start);

    if (start) {
        // Get the local video preview widget from the view.
        uint widget = m_view->getLocalVideoWidget();
        // Set the local video widget to use in the api.
        m_model->setLocalVideoWidget(widget);
    }

    // Start local video.
    m_model->startLocalVideo(start);
}

void CallPresenter::onDtmfTone(const void *sender, std::string &name)
{
    // Here we have to map our dialpad keys to their respective DTMF values.
    Participant::DTMF tone;

    if (name == "0")
        tone = Participant::DTMF_0;
    else if (name == "1")
        tone = Participant::DTMF_1;
    else if (name == "2")
        tone = Participant::DTMF_2;
    else if (name == "3")
        tone = Participant::DTMF_3;
    else if (name == "4")
        tone = Participant::DTMF_4;
    else if (name == "5")
        tone = Participant::DTMF_5;
    else if (name == "6")
        tone = Participant::DTMF_6;
    else if (name == "7")
        tone = Participant::DTMF_7;
    else if (name == "8")
        tone = Participant::DTMF_8;
    else if (name == "9")
        tone = Participant::DTMF_9;
    else if (name == "*")
        tone = Participant::DTMF_STAR;
    else if (name == "#")
        tone = Participant::DTMF_POUND;

    // Ask the model to send the tone.
    m_model->sendDTMF(tone);
}

};

};
