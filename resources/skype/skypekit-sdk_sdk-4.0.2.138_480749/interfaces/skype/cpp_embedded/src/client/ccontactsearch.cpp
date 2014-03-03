
#include "ccontactsearch.h"
#include "command-contactsearch.h"

CContactSearch::CContactSearch(unsigned int oid, SERootObject* rootobj)
                : ContactSearch(oid, rootobj)
{
        fprintf(stdout,"New contact search oid %d\n", getOID());
        fflush(stdout);
}

void CContactSearch::OnChange(int prop)
{
        if (prop == P_CONTACT_SEARCH_STATUS) {
                STATUS contact_search_status;
                GetPropContactSearchStatus(contact_search_status);
                if (contact_search_status == FINISHED || contact_search_status == FAILED)
                        m_controller->complete_search();
        }

        String value = GetProp(prop);
        List_String dbg = getPropDebug(prop, value);
        fprintf(stdout,"CONTACTSEARCH.%d:%s = %s\n", getOID(), 
                (const char*)dbg[1], 
                (const char*)dbg[2]);
        fflush(stdout);
}

void CContactSearch::OnNewResult(const ContactRef& contact, const uint& rankValue)
{
        Sid::String identity; 
        contact->GetIdentity(identity);
        printf("ContactSearch.%d.OnNewResult (%u): %s\n", getOID(), rankValue, (const char*)identity); 
}

