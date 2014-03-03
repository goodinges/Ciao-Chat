
#ifndef CConversation_INCLUDED_HPP
#define CConversation_INCLUDED_HPP

#include "skype-embedded_2.h"
#include "skype-object.h"

class CConversation : public Conversation
{
public:
        typedef DRef<CConversation, Conversation> Ref;
        typedef DRefs<CConversation, Conversation> Refs;
        CConversation(unsigned int oid, SERootObject* root);
        ~CConversation() {}

        void OnChange(int prop);
        virtual void OnParticipantListChange();
private:
        Participant::Refs m_participants;
};

#endif //CConversation_INCLUDED_HPP

