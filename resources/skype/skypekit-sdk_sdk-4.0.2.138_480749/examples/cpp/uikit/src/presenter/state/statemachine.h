
#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H

#include "context.h"

namespace Mvp
{

namespace Presenter
{

/**
 * @class StateMachine statemachine.h statemachine.h
 * @brief Keeps track of the current application state.
 *
 * This StateMachine class implements a state machine pattern.  A state machine
 * is used to ensure the application is always in only one state.  States in our
 * case are loggedout, idle, call, chat, etc.
 *
 * Each state object owns its own views and initializes/shows/hides them as
 * needed. Only one object of this type should be needed for an application.
 *
 * @see State, Context.
 */
class StateMachine
{
public:
    /**
     * Constructor
     */
    StateMachine();

    /**
     * Destructor
     */
    ~StateMachine();

private:
    /**
     * The state machine needs one context object which holds all the states.
     */
    Context m_context;
};

};

};
#endif

