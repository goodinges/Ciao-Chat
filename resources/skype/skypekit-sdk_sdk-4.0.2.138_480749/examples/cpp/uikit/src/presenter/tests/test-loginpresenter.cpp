#include <memory>
#include "gmock/gmock.h"
#include "presenter/loginpresenter.h"
//#include "skype-embedded_2.h"
//#include <stdio.h>

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;


namespace Mvp
{

namespace Test
{

class MockLoginService;
class FakeLoginService;

MockLoginService *svc;

class MockLoginView : public Mvp::View::AbstractLoginView
{
public:
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD0(username, std::string());
    MOCK_METHOD0(password, std::string());
    MOCK_METHOD0(showView, void());
    MOCK_METHOD1(showStatus,
            void(Mvp::View::AbstractLoginView::LoginStatus c));
    MOCK_METHOD1(setLoginError,
            void(Mvp::View::AbstractLoginView::LoginError c));
    MOCK_METHOD0(hideView, void());
    MOCK_METHOD0(destroyView, void());

    void fireOnLogin()
    {
        Poco::EventArgs args;
        logInEvent(this, args);
    }

    void fireOnLogout()
    {
        Poco::EventArgs args;
        logOutEvent(this, args);
    }

    MockLoginView(){};
};

class MockLoginService : public Mvp::Model::AbstractLoginService
{
public:
    MOCK_METHOD2(login, bool(std::string username, std::string password));
    MOCK_METHOD0(logout, bool());
    MOCK_METHOD2(statusChanged, void(const void*, Account::STATUS&));
    MOCK_METHOD0(currentlyLoggedIn, bool());
    MOCK_METHOD0(logoutReason, Account::LOGOUTREASON());

    void DelegateToFake();
};

bool fakelogin(std::string username, std::string password)
{
    Poco::EventArgs args;

    if (username == std::string("username")) {
        if (password == std::string("password"))
            svc->loggedIn(svc, args);
        else
            svc->loggedOut(svc, args);
    }

    // Return true if username is "username" otherwise return false.
    return username == std::string("username");
}

bool fakelogout()
{
    Poco::EventArgs args;

    svc->loggedOut(svc, args);
    return true;
}

void MockLoginService::DelegateToFake()
{
    ON_CALL(*this, login(_, _)).WillByDefault(Invoke(&fakelogin));
    ON_CALL(*this, logout()).WillByDefault(Invoke(&fakelogout));
}

TEST(login, perform_login)
{
    Mvp::Presenter::LoginPresenter presenter(svc);
    std::auto_ptr<MockLoginView> v(new MockLoginView());

    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());
    // Give "username" as username two times, then empty username.
    ON_CALL(*v, username())
        .WillByDefault(Return(std::string("username")));

    ON_CALL(*v, password())
        .WillByDefault(Return(std::string("password")));


    EXPECT_CALL(*svc, login(_, _));
    EXPECT_CALL(*v, username());
    EXPECT_CALL(*v, password());
    EXPECT_CALL(*v, showStatus(Mvp::View::AbstractLoginView::LoggingIn));
    EXPECT_CALL(*v, showStatus(Mvp::View::AbstractLoginView::LoggedIn));
    v->fireOnLogin();
}

TEST(login, perform_logout)
{
    Mvp::Presenter::LoginPresenter presenter(svc);
    std::auto_ptr<MockLoginView> v(new MockLoginView());
    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());

    ON_CALL(*svc, logoutReason())
        .WillByDefault(Return(Account::LOGOUT_CALLED));

    EXPECT_CALL(*svc, logout());
    EXPECT_CALL(*svc, logoutReason());
    EXPECT_CALL(*v, showStatus(Mvp::View::AbstractLoginView::LoggedOut));
    v->fireOnLogout();
}

TEST(login, attempt_login_badpassword)
{
    Mvp::Presenter::LoginPresenter presenter(svc);
    std::auto_ptr<MockLoginView> v(new MockLoginView());
    // Give "username" as username.
    ON_CALL(*v, username())
        .WillByDefault(Return(std::string("username")));

    ON_CALL(*v, password())
        .WillByDefault(Return(std::string("badpassword")));

    ON_CALL(*svc, logoutReason())
        .WillByDefault(Return(Account::INCORRECT_PASSWORD));

    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());

    EXPECT_CALL(*svc, login(_, _));
    EXPECT_CALL(*v, username());
    EXPECT_CALL(*v, password());
    EXPECT_CALL(*v, showStatus(Mvp::View::AbstractLoginView::LoggingIn));
    EXPECT_CALL(*v, showStatus(Mvp::View::AbstractLoginView::LoggedOut));
    EXPECT_CALL(*svc, logoutReason());
    EXPECT_CALL(*v, setLoginError(
                Mvp::View::AbstractLoginView::InvalidCredentials));
    v->fireOnLogin();
}

TEST(login, attempt_login_nopassword)
{
    Mvp::Presenter::LoginPresenter presenter(svc);
    std::auto_ptr<MockLoginView> v(new MockLoginView());
    // Give "username" as username.
    ON_CALL(*v, username())
        .WillByDefault(Return(std::string("username")));

    ON_CALL(*v, password())
        .WillByDefault(Return(std::string("")));

    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());

    EXPECT_CALL(*v, username());
    EXPECT_CALL(*v, password());
    EXPECT_CALL(*v, showStatus(
                Mvp::View::AbstractLoginView::InputCredentials));
    v->fireOnLogin();
}

TEST(login, attempt_login_nousername)
{
    Mvp::Presenter::LoginPresenter presenter(svc);
    std::auto_ptr<MockLoginView> v(new MockLoginView());
    // Give "username" as username.
    ON_CALL(*v, username())
        .WillByDefault(Return(std::string("")));

    ON_CALL(*v, password())
        .WillByDefault(Return(std::string("password")));

    EXPECT_CALL(*v, initialize());
    presenter.setView(v.get());

    EXPECT_CALL(*v, username());
    EXPECT_CALL(*v, password());
    EXPECT_CALL(*v, showStatus(
                Mvp::View::AbstractLoginView::InputCredentials));
    v->fireOnLogin();
}

};

};

int main(int argc, char** argv)
{
    Mvp::Test::svc = new Mvp::Test::MockLoginService;
    Mvp::Test::svc->DelegateToFake();
    testing::InitGoogleMock(&argc, argv);

    int retval = RUN_ALL_TESTS();

    delete Mvp::Test::svc;
    return retval;
}

