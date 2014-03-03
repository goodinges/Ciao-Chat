
#include "context.h"

#include "state.h"

#include <algorithm>

namespace Mvp
{

namespace Presenter
{

void Context::pushState(State *state)
{
    // Critical block.
    Poco::Mutex::ScopedLock lock(m_mutex);

    State *current = checkDuplicated(state);
    if (!current) {
        // Push the new state onto the top of the vector.
        m_states.push_back(state);

        // Let it do any initialization needed.
        state->init();
    }
    else {
        current->init();
        delete state;
    }
}

State *Context::popState()
{
    // Critical block.
    Poco::Mutex::ScopedLock lock(m_mutex);

    // Get the return value from the top of the vector.
    State *retval = m_states.back();

    // Pop it off the top.
    m_states.pop_back();

    State *current = m_states.back();

    if (current)
        current->init();

    // And finally return the popped state.
    return retval;
}

State *Context::currentState()
{
    // Critical block.
    Poco::Mutex::ScopedLock lock(m_mutex);

    State *state = 0;
    if (m_states.size() > 0)
        state = m_states.back();

    return state;
}

bool Context::removeState(const State *state)
{
    // Critical block.
    Poco::Mutex::ScopedLock lock(m_mutex);

    bool retval = false;

    // Look for the given state in the state vector.
    std::vector<State *>::iterator result =
        std::find(m_states.begin(), m_states.end(), state);

    /* If the state was found, set retval to True and remove the state from the
     * vector. Once it's removed we need to initialize the current state again,
     * but only if the current state is the one being removed.
     */
    if (result != m_states.end()) {
        bool needToInit = false;

        if (*result == m_states.back())
            needToInit = true;

        retval = true;
        m_states.erase(result);

        if (needToInit && m_states.size() > 0) {
            State *current = m_states.back();

            if (current)
                current->init();
        }
    }

    return retval;
}

State *Context::checkDuplicated(const State *state)
{
    State *ret = 0;

    for (unsigned int i = 0; i < m_states.size(); ++i) {
        if (*(m_states[i]) == *state) {
            ret = m_states[i];
            break;
        }
    }

    return ret;

}

};

};
