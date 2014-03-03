#include <memory>
#include "gmock/gmock.h"
#include "model/contactdata.h"
#include "presenter/convmanagerpresenter.h"

using ::testing::_;
using ::testing::Return;

std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts;

namespace Mvp
{

namespace Test
{

class MockConvManagerView : public Mvp::View::AbstractConvManagerView
{
public:
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD0(showView, void());
    MOCK_METHOD0(destroyView, void());
    MOCK_METHOD1(setNonParticipants,
        void(std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >));
    MOCK_METHOD1(setParticipants,
        void(std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >));
    MOCK_METHOD2(setConversationDetails, void(std::string&, std::string&));
    MOCK_METHOD1(updateContact,
            void(const Poco::SharedPtr<Mvp::Model::ContactData> data));

    MockConvManagerView()
    {
    };

    void triggerUpdateParticipantList()
    {
        std::vector<std::string> newparticipants;
        newparticipants.push_back(std::string("fakecontact1"));
        newparticipants.push_back(std::string("fakecontact2"));

        updateParticipantList(this, newparticipants);
    }

};

class MockConvManagerService : public Mvp::Model::AbstractConvManagerService
{
public:
    MOCK_METHOD1(setParticipants, void(const std::vector<std::string>&));
    MOCK_METHOD2(onParticipantListChange, void(const void *sender,
        Poco::EventArgs &));
    MOCK_METHOD0(getNonParticipants, std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >());
    MOCK_METHOD0(getParticipants, std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >());

    MockConvManagerService()
    {
        // Set the default return values for the service.
        ON_CALL(*this, getNonParticipants()).WillByDefault(Return(contacts));
        ON_CALL(*this, getParticipants()).WillByDefault(Return(contacts));
    }

    void triggerContactChanged()
    {
        Poco::SharedPtr<Mvp::Model::ContactData> contact = contacts[0];
        contact->status = Mvp::Model::Away;
        contactChanged(this, contact);
    }

    void triggerParticipantsChanged()
    {
        Poco::EventArgs args;
        participantsChanged(this, args);
    }
};

TEST(convmanagerpresenter, test_contactChanged)
{
    Poco::SharedPtr<MockConvManagerService> m(new MockConvManagerService);

    Mvp::Presenter::ConvManagerPresenter presenter(m.get());
    Poco::SharedPtr<MockConvManagerView> v(new MockConvManagerView());
    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());

    EXPECT_CALL(*m, getNonParticipants());
    EXPECT_CALL(*v, setNonParticipants(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    EXPECT_CALL(*v, showView());
    presenter.showView();

    EXPECT_CALL(*v, updateContact(_));
    m->triggerContactChanged();
}

TEST(convmanagerpresenter, test_participantsChanged)
{
    Poco::SharedPtr<MockConvManagerService> m(new MockConvManagerService);

    Mvp::Presenter::ConvManagerPresenter presenter(m.get());
    Poco::SharedPtr<MockConvManagerView> v(new MockConvManagerView());
    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());

    EXPECT_CALL(*m, getNonParticipants());
    EXPECT_CALL(*v, setNonParticipants(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    EXPECT_CALL(*v, showView());
    presenter.showView();

    EXPECT_CALL(*m, getNonParticipants());
    EXPECT_CALL(*v, setNonParticipants(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    m->triggerParticipantsChanged();
}

TEST(convmanagerpresenter, test_updateParticipants)
{
    Poco::SharedPtr<MockConvManagerService> m(new MockConvManagerService);

    Mvp::Presenter::ConvManagerPresenter presenter(m.get());
    Poco::SharedPtr<MockConvManagerView> v(new MockConvManagerView());
    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());

    EXPECT_CALL(*m, getNonParticipants());
    EXPECT_CALL(*v, setNonParticipants(_));
    EXPECT_CALL(*m, getParticipants());
    EXPECT_CALL(*v, setParticipants(_));
    EXPECT_CALL(*v, showView());
    presenter.showView();

    EXPECT_CALL(*m, setParticipants(_));
    v->triggerUpdateParticipantList();
}

};

};

void initializeData()
{
    Poco::SharedPtr<Mvp::Model::ContactData> contact(new Mvp::Model::ContactData);
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

