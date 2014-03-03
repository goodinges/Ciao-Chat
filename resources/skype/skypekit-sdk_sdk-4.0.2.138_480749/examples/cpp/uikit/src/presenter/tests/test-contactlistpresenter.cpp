#include <memory>
#include "gmock/gmock.h"
#include "presenter/contactlistpresenter.h"
#include "skype-embedded_2.h"

using ::testing::_;
using ::testing::Return;

std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts;
Poco::SharedPtr<Mvp::Model::ContactData> accountInfo;

namespace Mvp
{

namespace Test
{

class MockContactListView : public Mvp::View::AbstractContactListView
{
public:
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD0(showView, void());
    MOCK_METHOD1(setContactList, void(std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >));
    MOCK_METHOD1(setAccountInfo, void(const Poco::SharedPtr<Mvp::Model::ContactData>));
    MOCK_METHOD1(updateContact, void(const Poco::SharedPtr<Mvp::Model::ContactData>));
    MOCK_METHOD0(destroyView, void());

    MockContactListView()
    {
    };
};

class MockContactListService : public Mvp::Model::AbstractContactListService
{
public:
    MOCK_METHOD0(getContactList, std::vector<Poco::SharedPtr<Mvp::Model::ContactData> >());
    MOCK_METHOD0(accountInformation, const Poco::SharedPtr<Mvp::Model::ContactData>());

    MockContactListService()
    {
        // Set the default return values for the service.
        ON_CALL(*this, getContactList()).WillByDefault(Return(contacts));
        ON_CALL(*this, accountInformation()).WillByDefault(Return(accountInfo));
    }

    void triggerContactChange()
    {
        Poco::SharedPtr<Mvp::Model::ContactData> contact = contacts[0];
        contact->status = Mvp::Model::Away;
        contactChanged(this, contact);
    }
};

TEST(contactlist, perform_getContactList)
{
    Poco::SharedPtr<MockContactListService> m(new MockContactListService);

    Mvp::Presenter::ContactListPresenter presenter(m.get());
    Poco::SharedPtr<MockContactListView> v(new MockContactListView());
    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());

    EXPECT_CALL(*m, getContactList());
    EXPECT_CALL(*v, setContactList(_));
    EXPECT_CALL(*m, accountInformation());
    EXPECT_CALL(*v, setAccountInfo(_));
    EXPECT_CALL(*v, showView());
    presenter.showView();
}

TEST(contactlist, perform_contactChanged)
{
    Poco::SharedPtr<MockContactListService> m(new MockContactListService);

    Mvp::Presenter::ContactListPresenter presenter(m.get());
    Poco::SharedPtr<MockContactListView> v(new MockContactListView());
    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());

    EXPECT_CALL(*m, getContactList());
    EXPECT_CALL(*v, setContactList(_));
    EXPECT_CALL(*m, accountInformation());
    EXPECT_CALL(*v, setAccountInfo(_));
    EXPECT_CALL(*v, showView());
    presenter.showView();

    EXPECT_CALL(*v, updateContact(_));
    m->triggerContactChange();
}

};

};

void initializeData()
{
    Poco::SharedPtr<Mvp::Model::ContactData> accountInfo(new Mvp::Model::ContactData);
    accountInfo->displayName = "Test account";
    accountInfo->status = Mvp::Model::Available;

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

