#include <gmock/gmock.h>

#include "model/loginservice.h"
#include "model/contactlistservice.h"
#include "model/skypecore.h"
#include "model/skypeaccount.h"
#include "presenter/basecontactlistpresenter.h"

#include <iostream>
#include <fstream>

using ::testing::_;

char *username;
char *password;

namespace Mvp
{

namespace Test
{

class MockContactListPresenter : public Mvp::Presenter::BaseContactListPresenter
{
public:
    MockContactListPresenter(Mvp::Model::AbstractContactListService *model)
        :Mvp::Presenter::BaseContactListPresenter(model){};
    MOCK_METHOD1(setView, void(Mvp::View::AbstractContactListView*));
    MOCK_METHOD0(showView, void());
    MOCK_METHOD2(onViewClose, void(const void *sender, Poco::EventArgs &));
    MOCK_METHOD2(onContactChanged, void(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact));
    MOCK_METHOD2(onAccountChanged, void(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &accountData));
    MOCK_METHOD2(onContactGroupChanged, void(const void *sender, Poco::EventArgs &));
    MOCK_METHOD2(onChatContact, void(const void *sender, const std::string &skypeName));
    MOCK_METHOD2(onSmsContact, void(const void *sender, const std::string &skypeName));
    MOCK_METHOD2(onCallContact, void(const void *sender, const std::string &skypeName));
};

Mvp::Model::LoginService *loginService;
Mvp::Model::ContactListService *svc;

TEST(dummy, compiler_check)
{
    EXPECT_EQ(1, 1);
    EXPECT_EQ(0, 0);
    EXPECT_EQ(true, true);
}

TEST(getContactList, get_contact_list)
{
    EXPECT_FALSE(loginService->currentlyLoggedIn());

    loginService->login(username, password);
    // TODO: find a better way to wait for login.
    while (!loginService->currentlyLoggedIn())
        sleep(1); // wait for login

    EXPECT_TRUE(loginService->currentlyLoggedIn());

    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts =
        svc->getContactList();
    // Check that the contactlist size is at least 1.
    EXPECT_GE(contacts.size(), 1);

    const Poco::SharedPtr<Mvp::Model::ContactData> account =
        svc->accountInformation();
    EXPECT_EQ(strcmp(account->skypeName.data(), username), 0);
}

TEST(accountChanged, fullname_change)
{
    EXPECT_TRUE(loginService->currentlyLoggedIn());

    // Get the account ref.
    Mvp::Model::SkypeCore *skype = Mvp::Model::SkypeCore::instance();

    // To get the account info we need to get the account ref first.
    Mvp::Model::SkypeAccount::Ref accountRef;

    if (skype->GetAccount(skype->currentIdentity(), accountRef)) {
        MockContactListPresenter presenter(svc);

        SEString oldFullName;
        accountRef->GetPropFullname(oldFullName);

        EXPECT_CALL(presenter, onAccountChanged(_, _)).Times(2);
        accountRef->SetStrProperty(Account::P_FULLNAME, "testing fullname");

        accountRef->SetStrProperty(Account::P_FULLNAME, oldFullName);

        sleep(3);
    }
}

TEST(accountChanged, avatar_change)
{
    EXPECT_TRUE(loginService->currentlyLoggedIn());

    // Get the account ref.
    Mvp::Model::SkypeCore *skype = Mvp::Model::SkypeCore::instance();

    // To get the account info we need to get the account ref first.
    Mvp::Model::SkypeAccount::Ref accountRef;

    if (skype->GetAccount(skype->currentIdentity(), accountRef)) {
        MockContactListPresenter presenter(svc);

        std::ifstream avatarFile;
        avatarFile.open("/usr/share/skype/avatars/Skype.png",
            std::ios::in | std::ios::binary);
        if (avatarFile.is_open()) {

            avatarFile.seekg(0, std::ios::end);
            int length = avatarFile.tellg();
            avatarFile.seekg(0, std::ios::beg);

            unsigned char *avatarData = new unsigned char [length];

            avatarFile.read((char*)avatarData, length);
            avatarFile.close();

            SEBinary oldAvatar;
            accountRef->GetPropAvatarImage(oldAvatar);

            SEBinary avatar(avatarData, length);
            delete [] avatarData;

            EXPECT_CALL(presenter, onAccountChanged(_, _)).Times(2);
            accountRef->SetBinProperty(Account::P_AVATAR_IMAGE, avatar);

            accountRef->SetBinProperty(Account::P_AVATAR_IMAGE, oldAvatar);
            sleep(3);
        }
    }
}

};
};

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage: loginmodel-test skypename password [timeout]\n");
        printf("\tskypename:\ta valid skype name.\n");
        printf("\tpassword:\tvalid skype password\n");
        return 1;
    } else {
        username = argv[1];
        password = argv[2];
        Mvp::Test::svc = new Mvp::Model::ContactListService;
        Mvp::Test::loginService = new Mvp::Model::LoginService;
        testing::InitGoogleMock(&argc, argv);
        return RUN_ALL_TESTS();
    }
}
