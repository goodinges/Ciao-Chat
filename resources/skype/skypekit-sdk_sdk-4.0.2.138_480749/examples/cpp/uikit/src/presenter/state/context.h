
#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <vector>

#include <Poco/Mutex.h>

namespace Mvp
{

namespace Presenter
{

class State;

/**
 * @class Context context.h context.h
 * @brief Class that represents an application context.
 *
 * This class is responsible for maintaining the list of states
 * and knowing which state is current.  Each state is stored in a vector
 * of states, and new states are pushed onto the top of the vector. As
 * states destruct, they remove themselves from the vector with the
 * removeState() method.
 *
 * @see StateMachine, State.
 */
class Context
{
public:
    /**
     * Switch to the given state and push it onto the stack.
     *
     * @param state new state to push and switch to.
     */
    void pushState(State *state);

    /**
     * Get the current state object from the top of the stack.
     *
     * @return  a pointer to the top state object in the stack.
     */
    State *currentState();

    /**
     * Remove the current state and return its object,
     * switching to the previous state.
     *
     * @return a pointer to the state we just popped off the top of the stack.
     */
    State *popState();

    /**
     * Remove the given state from the stack.
     *
     * @param state the state object to remove from the states stack.
     * @return True if successfully removed, False otherwise.
     */
    bool removeState(const State *state);

private:
    /**
     * Helper method to check for duplicate states.
     *
     * Before a new state is pushed to the vector, it is checked to see if it's
     * already in there. This method does the checking.
     *
     * @returns the state that is already in the vector or null.
     */
    State *checkDuplicated(const State *state);

    /**
     * Vector of states that have been added to the stack.
     */
    std::vector<State *> m_states;

    Poco::Mutex m_mutex;
};

};

};
#endif

