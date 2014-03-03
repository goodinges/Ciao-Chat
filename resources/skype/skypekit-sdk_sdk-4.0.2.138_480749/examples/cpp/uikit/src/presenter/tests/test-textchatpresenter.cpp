#include <memory>
#include "gmock/gmock.h"
#include "model/messagedata.h"
#include "presenter/textchatpresenter.h"

using ::testing::_;
using ::testing::Return;

std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts;
std::vector<Poco::SharedPtr<Mvp::Model::MessageData> > messages;

namespace Mvp
{

namespace Test
{

class MockChatView : public Mvp::View::AbstractChatView
{
public:
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD0(showView, void());
    MOCK_METHOD1(setChatTitle, void(const std::string&));
    MOCK_METHOD1(addMessage, void(Poco::SharedPtr<Mvp::Model::MessageData>&));
    MOCK_METHOD2(reportMessageStatus,
        void(Mvp::View::AbstractChatView::StatusCode, std::string&));
    MOCK_METHOD0(destroyView, void());
    MOCK_METHOD1(setParticipants,
        void(std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >));
    MOCK_METHOD1(updateContact, void(Poco::SharedPtr<Mvp::Model::ContactData>&));
    MOCK_METHOD1(setSmsMode, void(bool));
    MOCK_METHOD1(enableControls, void(bool));
    MOCK_METHOD1(enableModeToggle, void(bool));
    MOCK_METHOD1(showLeaveAction, void(bool));

    MockChatView(){};

    void triggerPostMessage()
    {
        std::string message("Test Message");
        postMessage(this, message);
    }

    void triggerToggleMode()
    {
        Poco::EventArgs args;
        toggleMode(this, args);
    }
};

class MockChatService : public Mvp::Model::AbstractChatService
{
public:
    MOCK_METHOD1(postMessage, void(std::string&));
    MOCK_METHOD1(postSMS, void(std::string&));
    MOCK_METHOD0(chatName, std::string());
    MOCK_METHOD0(isConsumer, bool());
    MOCK_METHOD2(onIncomingMessage, void(const void *sender,
        Poco::SharedPtr<Mvp::Model::MessageData> &));
    MOCK_METHOD2(onMessageError, void(const void *sender, std::string &));
    MOCK_METHOD2(onSpawnedConference, void(const void *sender,
        const Mvp::Model::SkypeConversation::Ref &conference));
    MOCK_METHOD2(onParticipantListChanged, void(const void *sender,
        Poco::EventArgs &));
    MOCK_CONST_METHOD0(conversation, const ConversationRef());
    MOCK_METHOD0(getParticipants,
        std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >());
    MOCK_METHOD1(messageHistory,
            std::vector<Poco::SharedPtr<Mvp::Model::MessageData> >(
                unsigned int));
    MOCK_METHOD0(leaveChat, void());
    MOCK_METHOD0(consumeMessages, void());

    void triggerIncomingMessage()
    {
        Poco::SharedPtr<Mvp::Model::MessageData> data =
            new Mvp::Model::MessageData;
        messageReceived(this, data);
    }

    void triggerSmsDelivered()
    {
        std::string message("Test Message");
        smsDelivered(this, message);
    }

    void triggerSmsError()
    {
        std::pair<Sms::FAILUREREASON, std::string> pair(Sms::MISC_ERROR, "");
        smsError(this, pair);
    }
};

TEST(chatpresenter, test_incomingMessages)
{
    Poco::SharedPtr<MockChatService> m(new MockChatService);

    Mvp::Presenter::TextChatPresenter presenter(m.get());
    Poco::SharedPtr<MockChatView> v(new MockChatView());
    EXPECT_CALL(*v, initialize());

    ON_CALL(*m, getParticipants())
        .WillByDefault(Return(contacts));
    ON_CALL(*m, messageHistory(_))
        .WillByDefault(Return(messages));

    EXPECT_CALL(*m, chatName());
    EXPECT_CALL(*v, setChatTitle(_));
    EXPECT_CALL(*m, messageHistory(_));
    EXPECT_CALL(*m, isConsumer());
    EXPECT_CALL(*v, enableControls(_));
    EXPECT_CALL(*v, showLeaveAction(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();

    EXPECT_CALL(*v, addMessage(_));
    m->triggerIncomingMessage();
}

TEST(chatpresenter, test_postingMessages)
{
    Poco::SharedPtr<MockChatService> m(new MockChatService);

    Mvp::Presenter::TextChatPresenter presenter(m.get());
    Poco::SharedPtr<MockChatView> v(new MockChatView());
    EXPECT_CALL(*v, initialize());

    ON_CALL(*m, getParticipants())
        .WillByDefault(Return(contacts));
    ON_CALL(*m, messageHistory(_))
        .WillByDefault(Return(messages));

    EXPECT_CALL(*m, chatName());
    EXPECT_CALL(*v, setChatTitle(_));
    EXPECT_CALL(*m, messageHistory(_));
    EXPECT_CALL(*m, isConsumer());
    EXPECT_CALL(*v, enableControls(_));
    EXPECT_CALL(*v, showLeaveAction(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();

    EXPECT_CALL(*m, postMessage(_));
    v->triggerPostMessage();
}

TEST(chatpresenter, test_postingsms)
{
    Poco::SharedPtr<MockChatService> m(new MockChatService);

    Mvp::Presenter::TextChatPresenter presenter(m.get());
    Poco::SharedPtr<MockChatView> v(new MockChatView());
    EXPECT_CALL(*v, initialize());

    ON_CALL(*m, getParticipants())
        .WillByDefault(Return(contacts));
    ON_CALL(*m, messageHistory(_))
        .WillByDefault(Return(messages));

    EXPECT_CALL(*m, chatName());
    EXPECT_CALL(*v, setChatTitle(_));
    EXPECT_CALL(*m, messageHistory(_));
    EXPECT_CALL(*m, isConsumer());
    EXPECT_CALL(*v, enableControls(_));
    EXPECT_CALL(*v, showLeaveAction(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();

    EXPECT_CALL(*v, setSmsMode(true));
    v->triggerToggleMode();

    EXPECT_CALL(*m, postSMS(_));
    v->triggerPostMessage();

    EXPECT_CALL(*v, reportMessageStatus(
        Mvp::View::AbstractChatView::SmsDelivered, _));
    m->triggerSmsDelivered();

    EXPECT_CALL(*v, reportMessageStatus(
        Mvp::View::AbstractChatView::MiscError, _));
    m->triggerSmsError();
}

};

};

int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

