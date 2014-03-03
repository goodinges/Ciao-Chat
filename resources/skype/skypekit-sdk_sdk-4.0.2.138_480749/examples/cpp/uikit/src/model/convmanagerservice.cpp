
#include "convmanagerservice.h"

#include "model/skypecontact.h"
#include "model/skypeparticipant.h"
#include "model/skypecontactgroup.h"
#include "model/skypeconversation.h"
#include "model/skypecore.h"
#include "model/util.h"

namespace Mvp
{

namespace Model
{

ConvManagerService::ConvManagerService(const ConversationRef &conversation,
    Mode mode)
    :m_conversation(conversation),
    m_conversationMode(mode)
{
}

ConvManagerService::~ConvManagerService()
{
}

std::vector<Poco::SharedPtr<ContactData> >
ConvManagerService::getParticipants()
{
    std::vector<Poco::SharedPtr<ContactData> >contacts;

    ParticipantRefs participants;
    m_conversation->GetParticipants(participants,
        Conversation::OTHER_CONSUMERS);

    unsigned int size = participants.size();
    for (unsigned int i = 0; i < size; ++i) {
        // Get the SkypeParticipant.
        SkypeParticipant::Ref participant(participants[i]);

        if (m_conversationMode == Call) {
            // If the participant is live, add it to the list.
            if (participant->isLive()) {
                // Get the contact for this participant.
                SkypeContact::Ref contact =
                    Util::contactFromParticipant(participant);

                // Convert contact in ContactData structure.
                Poco::SharedPtr<ContactData>
                    newData(Util::dataFromContact(contact));

                // Add to the container.
                contacts.push_back(newData);
            }
        }
        else { // m_conversationMode == Chat.
            SkypeContact::Ref contact =
                Util::contactFromParticipant(participant);

            // Convert contact in ContactData structure.
            Poco::SharedPtr<ContactData>
                newData(Util::dataFromContact(contact));

            // Add to the container.
            contacts.push_back(newData);
        }
    }

    return contacts;
}

std::vector<Poco::SharedPtr<ContactData> >
    ConvManagerService::getNonParticipants()
{
    std::vector<Poco::SharedPtr<ContactData> > nonParticipants;

    // Gets the Skype Core instance.
    SkypeCore *skype = SkypeCore::instance();

    /* Once we have decided which group to fetch it's necessary to get a
    * contact group object passing which kind of group you want, in our
    * case ALL_BUDDIES.
    */
    SkypeContactGroup::Ref contactGroup;
    skype->GetHardwiredContactGroup(ContactGroup::ALL_BUDDIES, contactGroup);

    // Now we have a specialized group with a set of contacts.
    ContactRefs list;
    contactGroup->GetContacts(list);

    // We need the participants, so we can filter them out.
    std::vector<Poco::SharedPtr<ContactData> > participants =
        getParticipants();

    // Let's extract some data from the contacts.
    unsigned int size = list.size();
    for (unsigned int i = 0; i < size; ++i) {
        // Now let's see if the contact is a participant already.
        Poco::SharedPtr<ContactData> newData(Util::dataFromContact(list[i]));

        bool isParticipant = false;

        unsigned int pSize = participants.size();
        for (unsigned int j = 0; j < pSize; ++j)
            if (participants[j]->skypeName == newData->skypeName ||
                (m_conversationMode == Call && newData->status == Offline)) {
                isParticipant = true;
                break;
            }

        // Otherwise, add it to the list.
        if (!isParticipant)
            nonParticipants.push_back(newData);
    }

    return nonParticipants;
}

void ConvManagerService::setParticipants(
    const std::vector<std::string> &participantSkypeNames)
{
    // Get the new participants.
    unsigned int newSkypeNamesSize = participantSkypeNames.size();
    std::vector<std::string> newSkypeNames = participantSkypeNames;

    // Get the current participants.
    ParticipantRefs currentSkypeNames;
    m_conversation->GetParticipants(currentSkypeNames,
            Conversation::OTHER_CONSUMERS);
    unsigned int currentSkypeNamesSize = currentSkypeNames.size();


    // Check which participant needs to be added to the conversation.
    SEStringList addSkypeNames;

    for (unsigned int i = 0; i < newSkypeNamesSize; ++i) {
        bool contain = false;

        for (unsigned int j = 0; j < currentSkypeNamesSize; ++j) {
            SEString oid;
            currentSkypeNames[j]->GetPropIdentity(oid);

            if (newSkypeNames[i] == oid.data()) {
                if (m_conversationMode == Call)
                    currentSkypeNames[j]->Ring();
                contain = true;
                break;
            }
        }

        if (!contain)
            addSkypeNames.append(SEString(newSkypeNames[i].c_str()));
    }


    // Check which participant needs to be removed from the conversation.
    ParticipantRefs remSkypeNames;

    for (unsigned int i = 0; i < currentSkypeNamesSize; ++i) {
        bool contain = false;

        for (unsigned int j = 0; j < newSkypeNamesSize; ++j) {
            SEString oid;
            currentSkypeNames[i]->GetPropIdentity(oid);

            if (oid.data() == newSkypeNames[j]) {
                contain = true;
                break;
            }
        }

        if (!contain)
            remSkypeNames.append(currentSkypeNames[i]);
    }

    SkypeConversation::Ref newConversation;

    Conversation::TYPE type;
    m_conversation->GetPropType(type);

    // Pass the skypename string into addConsumers
    if (type == Conversation::DIALOG) {
        if (m_conversation->SpawnConference(addSkypeNames, newConversation))
            printf("spawned conference from conversation\n");
        else
            printf("error spawning conference from conversation\n");
    }
    else {
        // Add new participants to the conference.
        if (m_conversation->AddConsumers(addSkypeNames))
            printf("added participants to conversation\n");
        else
            printf("error adding participants to conversation\n");

        // Remove old participants from the conference.
        unsigned int remSize = remSkypeNames.size();
        for (unsigned int i = 0; i < remSize; ++i) {
            SEString oid;
            remSkypeNames[i]->GetPropIdentity(oid);

            if (m_conversationMode == Chat) {
                if (!remSkypeNames[i]->Retire())
                    printf("error retiring participant form conversation\n");
            }
            else {
                if (!remSkypeNames[i]->Hangup())
                    printf("error removing participant form conversation\n");
            }
        }
    }
}

void ConvManagerService::onParticipantListChange(const void *sender,
        Poco::EventArgs &args)
{
    participantsChanged(this, args);
}

};

};


