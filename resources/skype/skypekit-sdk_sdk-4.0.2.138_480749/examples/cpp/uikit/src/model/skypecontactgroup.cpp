#include "skypecontactgroup.h"

namespace Mvp
{

namespace Model
{

SkypeContactGroup::SkypeContactGroup(unsigned int oid, SERootObject* root)
    : ContactGroup(oid, root)
{
    printf("New ContactGroup created.\n.");
}

SkypeContactGroup::~SkypeContactGroup()
{
}

void SkypeContactGroup::OnChange(const ContactRef &contact)
{
    Poco::EventArgs args;
    contactGroupChanged(this, args);
}

};

};
