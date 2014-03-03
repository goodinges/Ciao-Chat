
#ifndef CParticipant_INCLUDED_HPP
#define CParticipant_INCLUDED_HPP

#include "skype-embedded_2.h"
#include "skype-object.h"

class CParticipant : public Participant
{
public:
        typedef DRef<CParticipant,Participant> Ref;
        typedef DRefs<CParticipant,Participant> Refs;
        CParticipant(unsigned int oid, SERootObject* root);
        ~CParticipant() {}

        void OnChange(int prop);
        void OnIncomingDTMF(const DTMF& dtmf);
private:
        Video::Ref m_video;
};

#endif //CParticipant_INCLUDED_HPP

