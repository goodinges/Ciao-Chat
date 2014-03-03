
#include "csms.h"

CSms::CSms(unsigned int oid, SERootObject* rootobj)
                : Sms(oid, rootobj)
{
        fprintf(stdout,"New sms oid %d\n", getOID());
}

void CSms::OnChange(int prop)
{
        String value = GetProp(prop);
        List_String dbg = getPropDebug(prop, value);
        fprintf(stdout,"SMS.:%s = %s\n", 
                (const char*)dbg[1], 
                (const char*)dbg[2]);
}

