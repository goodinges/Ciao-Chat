
#include "state.h"

#include <stdio.h>

namespace Mvp
{

namespace Presenter
{

State::State(Context &context)
    : m_parentContext(context)
{
}

State::~State()
{
    /* The state is being destroyed,
     * so first remove ourself from the parent context.
     */
    m_parentContext.removeState(this);
}

void State::stateFinished(const void *sender, Poco::EventArgs &)
{
    printf("stateFinished called\n");

    /* Delete ourself, so the destructor will remove us
     * from the parent context object.
     */
    delete this;
}

bool State::operator==(const State &state) const
{
    return (id() == state.id());
}

};

};
