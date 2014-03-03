
#include "statemachine.h"

#include "loggedoutstate.h"

namespace Mvp
{

namespace Presenter
{

StateMachine::StateMachine()
{
    // Push a new LoggedOut state onto the context to start things going.
    m_context.pushState(new LoggedOutState(m_context));
}

StateMachine::~StateMachine()
{
}

};

};
