#include <memory>
#include "gmock/gmock.h"
#include "presenter/callpresenter.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts;

namespace Mvp
{

namespace Test
{

class MockCallView : public Mvp::View::AbstractCallView
{
public:
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD0(showView, void());
    MOCK_METHOD1(setCallTitle, void(const std::string&));
    MOCK_METHOD1(setCallStatus, void(CallStatus));
    MOCK_METHOD1(setParticipants,
            void(std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >));
    MOCK_METHOD1(updateContact, void(Poco::SharedPtr<Mvp::Model::ContactData>&));
    MOCK_METHOD1(setVideoAvailable, void(bool));
    MOCK_METHOD0(destroyView, void());
    MOCK_METHOD0(getLocalVideoWidget, uint());
    MOCK_METHOD0(getRemoteVideoWidget, uint());
    MOCK_METHOD1(showRemoteVideo, void(bool));

    MockCallView()
        : m_answer(true)
    {
        ON_CALL(*this, setCallStatus(_))
            .WillByDefault(Invoke(this, &MockCallView::acceptSetCallStatus));
    };

    void setDefaultAction(bool answer = true)
    {
        m_answer = answer;
    }

    void fireHold(bool held = true)
    {
        hold(this, held);
    }

    void fireMute(bool muted = true)
    {
        mute(this, muted);
    }

    void fireDtmf()
    {
        std::string tone;

        for (unsigned int i = 0; i < 11; ++i, tone = i)
            dtmfTone(this, tone);
    }
private:
    void acceptSetCallStatus(CallStatus status)
    {
        Poco::EventArgs args;

        switch(status) {
            case AbstractCallView::Ringing:
                if (m_answer)
                    accepted(this, args);
                else
                    rejected(this, args);
				
                break;
			default: break;
        }
    }
	
    bool m_answer;
};

class MockCallService : public Mvp::Model::AbstractCallService
{
public:
    MOCK_METHOD0(callName, std::string());
    MOCK_METHOD0(liveStatus, Conversation::LOCAL_LIVESTATUS());
    MOCK_METHOD0(getParticipants,
            std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >());
    MOCK_METHOD0(acceptCall, void());
    MOCK_METHOD0(rejectCall, void());
    MOCK_METHOD0(endCall, void());
    MOCK_METHOD1(mute, void(bool));
    MOCK_METHOD1(hold, void(bool));
    MOCK_METHOD2(onLiveStatusChanged, void(const void *,
                Conversation::LOCAL_LIVESTATUS&));
    MOCK_METHOD2(onParticipantListChanged, void(const void *,
                Poco::EventArgs&));
    MOCK_METHOD2(onSpawnedConference, void(const void *sender,
        const Mvp::Model::SkypeConversation::Ref &conference));
    MOCK_METHOD2(onParticipantJoinedCall, void(const void *, Poco::EventArgs&));
    MOCK_METHOD2(onParticipantLeftCall, void(const void *, Poco::EventArgs&));
    MOCK_METHOD1(startLocalVideo, void(bool));
    MOCK_METHOD1(startRemoteVideo, void(bool));
    MOCK_METHOD1(setLocalVideoWidget, void(uint&));
    MOCK_METHOD1(setRemoteVideoWidget, void(uint&));
    MOCK_METHOD1(sendDTMF, void(Participant::DTMF));

    MockCallService()
    {
        ON_CALL(*this, acceptCall())
            .WillByDefault(Invoke(this, &MockCallService::fakeAccept));

        ON_CALL(*this, rejectCall())
            .WillByDefault(Invoke(this, &MockCallService::fakeReject));

        ON_CALL(*this, hold(_))
            .WillByDefault(Invoke(this, &MockCallService::fakeHold));
    };

private:
    void fakeAccept()
    {
        Conversation::LOCAL_LIVESTATUS status = Conversation::IM_LIVE;

        onLiveStatusChanged(this, status);
    }

    void fakeReject()
    {
        Conversation::LOCAL_LIVESTATUS status = Conversation::NONE;

        onLiveStatusChanged(this, status);
    }

    void fakeHold(bool held)
    {
        Conversation::LOCAL_LIVESTATUS status;

        status = (held ? Conversation::ON_HOLD_LOCALLY : Conversation::IM_LIVE);

        onLiveStatusChanged(this, status);
    }
};

TEST(callpresenter, test_acceptIncomingCall)
{
    Poco::SharedPtr<MockCallService> m(new MockCallService);

    Mvp::Presenter::CallPresenter presenter(m.get());
    Poco::SharedPtr<MockCallView> v(new MockCallView());
    EXPECT_CALL(*v, initialize());

    ON_CALL(*m, getParticipants())
        .WillByDefault(Return(contacts));
    ON_CALL(*m, liveStatus())
        .WillByDefault(Return(Conversation::RINGING_FOR_ME));

    EXPECT_CALL(*m, callName()).WillRepeatedly(Return(std::string("fakecall")));
    EXPECT_CALL(*v, setCallTitle(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    EXPECT_CALL(*m, liveStatus());
    EXPECT_CALL(*v, setCallStatus(Mvp::View::AbstractCallView::Ringing));


    // Answering call.
    EXPECT_CALL(*m, acceptCall());

    Conversation::LOCAL_LIVESTATUS status = Conversation::IM_LIVE;
    EXPECT_CALL(*m, onLiveStatusChanged(_, status));

    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();

    // Holding call.
    EXPECT_CALL(*m, hold(true));

    status = Conversation::ON_HOLD_LOCALLY;
    EXPECT_CALL(*m, onLiveStatusChanged(_, status));

    v->fireHold(true);

    // Resuming call.
    EXPECT_CALL(*m, hold(false));

    status = Conversation::IM_LIVE;
    EXPECT_CALL(*m, onLiveStatusChanged(_, status));

    v->fireHold(false);

    // Muting call.
    EXPECT_CALL(*m, mute(true));
    v->fireMute(true);

    // Unmuting call.
    EXPECT_CALL(*m, mute(false));
    v->fireMute(false);

    // Send DTMFs.
    EXPECT_CALL(*m, sendDTMF(_))
        .Times(11);
    v->fireDtmf();
}

TEST(callpresenter, test_rejectIncomingCall)
{
    Poco::SharedPtr<MockCallService> m(new MockCallService);

    Mvp::Presenter::CallPresenter presenter(m.get());
    Poco::SharedPtr<MockCallView> v(new MockCallView());
    v->setDefaultAction(false);
    EXPECT_CALL(*v, initialize());

    ON_CALL(*m, getParticipants())
        .WillByDefault(Return(contacts));
    ON_CALL(*m, liveStatus())
        .WillByDefault(Return(Conversation::RINGING_FOR_ME));

    EXPECT_CALL(*m, callName()).WillRepeatedly(Return(std::string("fakecall")));
    EXPECT_CALL(*v, setCallTitle(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    EXPECT_CALL(*m, liveStatus());
    EXPECT_CALL(*v, setCallStatus(Mvp::View::AbstractCallView::Ringing));


    // Declining call.
    EXPECT_CALL(*m, rejectCall());

    Conversation::LOCAL_LIVESTATUS status = Conversation::NONE;
    EXPECT_CALL(*m, onLiveStatusChanged(_, status));

    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();
}

TEST(callpresenter, test_outgoingCall)
{
    Poco::SharedPtr<MockCallService> m(new MockCallService);

    Mvp::Presenter::CallPresenter presenter(m.get());
    Poco::SharedPtr<MockCallView> v(new MockCallView());
    EXPECT_CALL(*v, initialize());

    ON_CALL(*m, getParticipants())
        .WillByDefault(Return(contacts));
    ON_CALL(*m, liveStatus())
        .WillByDefault(Return(Conversation::STARTING));

    // Requesting a new call.
    EXPECT_CALL(*m, callName()).WillRepeatedly(Return(std::string("fakecall")));
    EXPECT_CALL(*v, setCallTitle(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    EXPECT_CALL(*m, liveStatus());
    EXPECT_CALL(*v, setCallStatus(Mvp::View::AbstractCallView::Starting));

    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();
}
};

};

void initializeData()
{
    Poco::SharedPtr<Mvp::Model::ContactData>
        contact(new Mvp::Model::ContactData);

    contact->displayName = "Test DisplayName";
    contact->skypeName = "Foo";
    contact->status = Mvp::Model::Available;

    contacts.push_back(contact);
}

int main(int argc, char** argv)
{
    initializeData();

    testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}

