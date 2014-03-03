#include "convmanagerpresenter.h"

#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

#include "view/abstractconvmanagerview.h"

namespace Mvp
{

namespace Presenter
{

ConvManagerPresenter::ConvManagerPresenter(
    Mvp::Model::AbstractConvManagerService *model)
    : BaseConvManagerPresenter(model)
{
}

void ConvManagerPresenter::setView(Mvp::View::AbstractConvManagerView *view)
{
    // Give the view to the base class for caching.
    AbstractPresenter<Mvp::View::AbstractConvManagerView>::setView(view);

    // Let the view initialize itself.
    m_view->initialize();

    // Connect to the view's signals.
    m_view->viewClosed += Poco::Delegate<ConvManagerPresenter, Poco::EventArgs>
        (this, &ConvManagerPresenter::onViewClose);

    m_view->updateParticipantList +=
        Poco::Delegate<ConvManagerPresenter, const std::vector<std::string> >
        (this, &ConvManagerPresenter::onUpdateParticipantList);
}

void ConvManagerPresenter::showView()
{
    m_view->setNonParticipants(m_model->getNonParticipants());
    m_view->setParticipants(m_model->getParticipants());

    // Then show it.
    m_view->showView();
}

void ConvManagerPresenter::onViewClose(
    const void *sender, Poco::EventArgs &args)
{
    finished(this, args);
}

void ConvManagerPresenter::onParticipantsChanged(const void *sender,
        Poco::EventArgs &)
{
    // Update the non participant list.
    m_view->setNonParticipants(m_model->getNonParticipants());

    // Update the participant list.
    m_view->setParticipants(m_model->getParticipants());
}

void ConvManagerPresenter::onContactChanged(const void *sender,
        Poco::SharedPtr<Mvp::Model::ContactData> &contact)
{
    m_view->updateContact(contact);
}

void ConvManagerPresenter::onUpdateParticipantList(const void *sender,
        const std::vector<std::string> &participants)
{
    /* Check the participants list (it should have at least 1 participant,
     * otherwise, do nothing.)
     */
    if (participants.size() > 0) {
        printf("Adding new participants.\n");
        m_model->setParticipants(participants);
    }
}

};

};
