
#include "cconversation.h"
#include "cmessage.h"

CConversation::CConversation(unsigned int oid, SERootObject* rootobj)
                : Conversation(oid, rootobj)
{      
    GetParticipants(m_participants, ALL);
    fetch(m_participants);
}

void CConversation::OnChange(int prop)
{        
        String value = GetProp(prop);
        List_String dbg = getPropDebug(prop, value);
        
        Sid::String displ_name = GetStrProp(Conversation::P_DISPLAYNAME);
        fprintf(stdout,"CONVERSATION.%s:%s = %s\n", 
                (const char*)displ_name, 
                (const char*)dbg[1], 
                (const char*)dbg[2]);
                
        fflush(stdout);
}

void CConversation::OnParticipantListChange()
{
    GetParticipants(m_participants, ALL);
    fetch(m_participants);
}

