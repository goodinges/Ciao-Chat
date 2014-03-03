
#ifndef _STATE_H
#define _STATE_H

#include "context.h"

#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Presenter
{

/**
 * @class State state.h state.h
 * @brief Defines the abstract common interface of any state object.
 *
 * States are responsible for representing a state in the application.  Each
 * state should own its own view, presenter and model objects.  It needs to
 * implement the init() function as described.  This class only provides the
 * base interface that every state should implement and a destructor that
 * removes ourselves from the context.
 *
 * @see StateMachine, Context.
 */
class State
{
public:
    /**
     * Constructor
     *
     * @param context This is the parent context to which this state belongs.
     */
    State(Context &context);

    /**
     * Destructor
     *
     * The destructor is implemented to remove ourself from the parent context.
     */
    virtual ~State();

    /**
     * Perform any initialization needed for the state.
     *
     * When the statemachine switches states, this method is called on the
     * new state automatically, so it should show its view and perform any
     * other initialization as needed to enter the state.
     *
     * @note Each state must implement this method.
     */
    virtual void init() = 0;

    /**
     * A slot to respond to finished signal of the state's presenter.
     *
     * @param sender A pointer to the object that sent the signal.
     */
    void stateFinished(const void *sender, Poco::EventArgs &);

    /**
     * Get the unique id of this state.
     *
     * @return a string that uniquely identifies this state.
     */
    virtual std::string id() const = 0;

    /**
     * Equality operator for state objects.
     *
     * @returns true if the states are the same, false otherwise.
     */
    bool operator==(const State &state) const;

protected:

    /**
     * We keep a reference to our parent context so we can remove
     * ourself at destruction time.
     */
    Context &m_parentContext;
};

};

};
#endif

