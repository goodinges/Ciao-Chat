#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Poco/EventArgs.h>

#include "model/loginservice.h"
#include "presenter/baseloginpresenter.h"

using ::testing::AtLeast;
using ::testing::_;

char *username;
char *password;
const char *badpassword = "badpassword";
int timeout = 5;
Mvp::Model::LoginService *svc;


namespace Mvp
{

namespace Test
{

class MockLoginPresenter : public Mvp::Presenter::BaseLoginPresenter
{
public:
    MockLoginPresenter(Mvp::Model::AbstractLoginService *model) :Mvp::Presenter::BaseLoginPresenter(model){};
    MOCK_METHOD1(setView, void(Mvp::View::AbstractLoginView*));
    MOCK_METHOD0(showView, void());
    MOCK_METHOD2(onLogIn, void(const void *sender, Poco::EventArgs &));
    MOCK_METHOD2(onLogOut, void(const void *sender, Poco::EventArgs &));
    MOCK_METHOD2(onViewClose, void(const void *sender, Poco::EventArgs &));
    MOCK_METHOD2(loggedIn, void(const void *sender, Poco::EventArgs &));
    MOCK_METHOD2(loggedOut, void(const void *sender, Poco::EventArgs &));
};

TEST(dummy, compiler_check) {
    EXPECT_EQ(1, 1);
    EXPECT_EQ(0, 0);
    EXPECT_EQ(true, true);
}


TEST(login, perform_login) {
    MockLoginPresenter presenter(svc);
    EXPECT_CALL(presenter, loggedIn(_, _))
        .Times(AtLeast(1));

    EXPECT_TRUE(svc->login(username, password));
    while (!svc->currentlyLoggedIn())
        sleep(1);
}

TEST(login, perform_logout) {
    MockLoginPresenter presenter(svc);

    // We expect to get loggedOut() signal when we log out.
    EXPECT_CALL(presenter, loggedOut(_, _));
    EXPECT_TRUE(svc->logout());

    // Wait for the logout to happen.
    while (svc->currentlyLoggedIn())
        sleep(timeout);
}

TEST(login, attempt_login) {
    MockLoginPresenter presenter(svc);
    /* We expect to get loggedOut() signal when trying to log in with
     *  a bad password.
     */
    EXPECT_CALL(presenter, loggedOut(_, _)).Times(AtLeast(1));
    EXPECT_TRUE(svc->login(username, badpassword));

    // Can't sleep until currentlyLoggedIn because we passed bad credentials.
    sleep(timeout);
}

};

};

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage: loginmodel-test skypename password [timeout]\n");
        printf("\tskypename:\ta valid skype name.\n");
        printf("\tpassword:\tvalid skype password\n");
        printf("\ttimeout:\toptional timeout in seconds to wait for failed login"\
        " test, 5 if none entered.\n");
        return 1;
    } else {
        username = argv[1];
        password = argv[2];

        if (argc > 3) {
            // Get the timeout parameter
            timeout = atoi(argv[3]);
        }

        svc = new Mvp::Model::LoginService();
        testing::InitGoogleMock(&argc, argv);
        return RUN_ALL_TESTS();
    }
}

