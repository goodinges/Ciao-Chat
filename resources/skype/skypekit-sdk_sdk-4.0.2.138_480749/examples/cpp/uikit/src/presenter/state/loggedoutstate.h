
#ifndef _LOGGEDOUTSTATE_H
#define _LOGGEDOUTSTATE_H

#include "state.h"

#include <Poco/EventArgs.h>

namespace Mvp
{

namespace View
{
class AbstractView;
class AbstractLoginView;
};

namespace Model
{
class AbstractLoginService;
};

namespace Presenter
{

class LoginPresenter;

/**
 * @class LoggedOutState loggedoutstate.h loggedoutstate.h
 * @brief Implementation of initial application logged out state.
 *
 * This subclass of State implements the initial logged out state of the
 * application.  Its primary purpose is to manage the login process and
 * create an idlestate on successful login. It also creates and owns a
 * LoginPresenter, a LoginService, and an AbstractLoginView.
 *
 * @see State, LoginService, LoginPresenter and AbstractLoginView.
 */
class LoggedOutState: public State
{
public:
    /**
     * Contstuctor.
     *
     * @param context The parent context of this state.
     */
    LoggedOutState(Context &context);

    /**
     * Destructor.
     */
    ~LoggedOutState();

    /**
     * Perform initialization.
     */
    void init();

    /**
     * Get the unique id of this state.
     *
     * @return a string that uniquely identifies this state.
     */
    std::string id() const;

private:
    /**
     * Slot to catch the view created by the viewfactory.
     *
     * View creation is asynchronous so this slot is needed to get
     * the pointer to the view after it is requested by
     * viewFactory->requestView(LoginView).
     *
     * @param sender a pointer to the object that sent the signal.
     * @param pair a pair containing the request id and the view object.
     *
     * @see AbstractViewFactory::requestView
     *
     * @note The view object's type needs to be AbstractLoginView.
     */
    void catchView(const void *sender,
        std::pair<int, Mvp::View::AbstractView *>&pair);

    /**
     * Slot to react to login success.
     *
     * This slot responds to the successful login signal from the presenter.
     * It creates a new IdleState object and pushes it to the context.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onLogInSuccess(const void *sender, Poco::EventArgs &);

    /**
     * This slot is called when we are logged out of the Skype Network.
     *
     * When we are logged out we need to remove all the other states besides
     * ourself.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onLogOutSuccess(const void *sender, Poco::EventArgs &);

    /**
     * This slot is called when the idlestate closes.
     *
     * When the idle state is finished, we need to log out. This slot does
     * simply just that.
     *
     * @param sender a pointer to the object that sent the signal.
     */
    void onIdleStateFinished(const void *sender, Poco::EventArgs &);

    /**
     * Destroy all other states in the State Machine.
     *
     * This method will look for all states different from the LoggedoutState,
     * remove them from the State Machine Context and then delete the
     * instances.
     */
    void cleanupStates();

    /**
     * Model, Presenter and View needed to perform this state's job.
     */
    Mvp::Model::AbstractLoginService *m_model;
    LoginPresenter *m_presenter;
    Mvp::View::AbstractLoginView *m_view;
    int m_viewRequestId;
};

};

};
#endif
