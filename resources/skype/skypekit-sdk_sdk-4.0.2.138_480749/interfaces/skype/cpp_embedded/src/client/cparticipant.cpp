
#include "cparticipant.h"

CParticipant::CParticipant(unsigned int oid, SERootObject* rootobj)
                : Participant(oid, rootobj)
{
//         fprintf(stdout,"New participant oid %d\n", getOID());
}

void CParticipant::OnChange(int prop)
{
        String value = GetProp(prop);
        List_String dbg = getPropDebug(prop, value);
        String identity = GetStrProp(Participant::P_IDENTITY);
        fprintf(stdout,"PARTICIPANT.%s:%s = %s\n", 
                (const char*)identity, 
                (const char*)dbg[1], 
                (const char*)dbg[2]);
        if (prop == P_VIDEO_STATUS && value.toInt() == VIDEO_AVAILABLE)
        {
            if (GetVideo(m_video))
                m_video.fetch();
        }
}

void CParticipant::OnIncomingDTMF(const DTMF& dtmf)
{
        String identity = GetStrProp(Participant::P_IDENTITY);
	    fprintf(stdout,"PARTICIPANT.%s.OnIncomingDTMF dtmf = %d\n", 
               (const char*)identity, (int)dtmf);
}

