#ifndef _ABSTRACTPRESENTER_H
#define _ABSTRACTPRESENTER_H

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class AbstractPresenter abstractpresenter.h abstractpresenter.h
 * @brief Defines the abstract interface for presenters.
 *
 * The Presenter's job is to connect models to views and vice versa. It
 * is also responsible for showing the view. This class defines
 * some methods every presenter at least needs to implement, namely setView()
 * and showView().
 */
template <class T>
class AbstractPresenter {
public:
    AbstractPresenter()
        :m_view(0){}

    /**
     * Set the view object that this presenter should use.
     *
     * @param view the view to use.
     */
    virtual void setView(T *view) {
        m_view = view;
    }

    /**
     * Derived classes need to implement showView() to tell the view to show
     * after first setting up anything necessary.
     */
    virtual void showView() = 0;

    /**
     * Signal to tell any interested objects that this presenter is done and
     * should be destroyed.
     *
     * @note This should be connected to the state's stateFinished slot
     * so it can remove itself from the context, etc.
     */
    Poco::BasicEvent<Poco::EventArgs> finished;

protected:
    /**
     * The view, each presenter has one, so we cache it here in the base class.
     */
    T *m_view;
};

};

};
#endif
