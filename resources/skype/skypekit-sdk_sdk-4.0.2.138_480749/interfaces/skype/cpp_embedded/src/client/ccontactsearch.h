
#ifndef CContactSearch_INCLUDED_HPP
#define CContactSearch_INCLUDED_HPP

#include "skype-embedded_2.h"
#include "skype-object.h"

class CommandContactSearch;

class CContactSearch : public ContactSearch
{
public:
        typedef DRef<CContactSearch, ContactSearch> Ref;
        typedef DRefs<CContactSearch, ContactSearch> Refs;

        CContactSearch(unsigned int oid, SERootObject* root);
        ~CContactSearch() {}

        void set_controller(CommandContactSearch* controller) { m_controller = controller; }

        void OnChange(int prop);
        void OnNewResult(const ContactRef& contact, const uint& rankValue);

        CommandContactSearch* m_controller;
};



#endif //CContactSearch_INCLUDED_HPP

