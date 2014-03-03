#ifndef _BASELOGINPRESENTER_H
#define _BASELOGINPRESENTER_H

#include "abstractpresenter.h"
#include "model/abstractloginservice.h"

#include <Poco/BasicEvent.h>
#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace View
{
class AbstractLoginView;
};

namespace Presenter
{

/**
 * @class BaseLoginPresenter baseloginpresenter.h baseloginpresenter.h
 * @brief Base class to define the interface for LoginPresenters.
 *
 * LoginPresenter is responsible for connecting the LoginService to the
 * LoginView, manipulating the LoginView (show, hide, etc.) and passing
 * data from one to the other and back.
 *
 * This class is a base class that does the connecting of poco signals to
 * slots.
 *
 * @see AbstractPresenter, AbstractLoginService and AbstractLoginView.
 */
class BaseLoginPresenter : public AbstractPresenter<
                           Mvp::View::AbstractLoginView>
{
public:
    /**
     * Constructor.
     *
     * Takes the model to use for this presenter.
     *
     * @param model AbstractLoginService to use for logging into Skype network.
     */
    BaseLoginPresenter(Mvp::Model::AbstractLoginService *model)
        :m_model(model)
    {
        // Connect the model's signals to our slots.
        m_model->loggedIn += Poco::Delegate<BaseLoginPresenter,
            Poco::EventArgs> (this, &BaseLoginPresenter::loggedIn);
        m_model->loggedOut += Poco::Delegate<BaseLoginPresenter,
            Poco::EventArgs> (this, &BaseLoginPresenter::loggedOut);
    }

    virtual ~BaseLoginPresenter()
    {
        // Connect the model's signals to our slots.
        m_model->loggedIn -= Poco::Delegate<BaseLoginPresenter,
            Poco::EventArgs> (this, &BaseLoginPresenter::loggedIn);
        m_model->loggedOut -= Poco::Delegate<BaseLoginPresenter,
            Poco::EventArgs> (this, &BaseLoginPresenter::loggedOut);
    }

    /**
     * Set the view the presenter should use.
     *
     * @param view the view this presenter should use and respond to.
     */
    virtual void setView(Mvp::View::AbstractLoginView *view) = 0;

    /**
     * Show the view.
     *
     * The state that owns this presenter will ask the presenter to show
     * the view when it becomes the current state.
     */
    virtual void showView() = 0;

    /**
     * A signal to say that the state login succeeded.
     *
     * This signal is sent when loggedIn() is called, which is connected
     * to the LoginService's LoggedIn signal.
     */
    Poco::BasicEvent<Poco::EventArgs> loginSucceeded;

protected:
    /**
     * Slot to react to view close.
     */
    virtual void onViewClose(const void *sender, Poco::EventArgs &) = 0;

    /**
     * Slot to react to login requests from the view.
     */
    virtual void onLogIn(const void* sender, Poco::EventArgs &) = 0;

    /**
     * Slot to react to logout requests from the view.
     */
    virtual void onLogOut(const void* sender, Poco::EventArgs &) = 0;

    /**
     * Slot to react to login event from the LoginService.
     */
    virtual void loggedIn(const void* sender, Poco::EventArgs &) = 0;

    /**
     * Slot to react to logout event from the LoginService.
     */
    virtual void loggedOut(const void* sender, Poco::EventArgs &) = 0;

    Mvp::Model::AbstractLoginService *m_model;
};

};

};
#endif
