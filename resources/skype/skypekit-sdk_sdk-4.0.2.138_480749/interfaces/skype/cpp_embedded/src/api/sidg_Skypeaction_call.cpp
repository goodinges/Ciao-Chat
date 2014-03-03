#include "SidPlatform.hpp"
#include "SidCommandProcessor.hpp"
#include "skype-embedded_2.h"
#include "sidg_msgs_Skype.hpp"

#ifdef SE_USE_NAMESPACE
namespace Skype {
#endif
bool Skype::GetVersionString(
        Sid::String& version
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\034",MsgconfigGetVersionStringResponse::BEGIN,&version);
        return __status;
}

bool Skype::GetUnixTimestamp(
        uint& timestamp
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(5,"ZR\000\206\001",MsgconfigGetUnixTimestampResponse::BEGIN,&timestamp);
        return __status;
}

bool ContactGroup::GiveDisplayName(
        const Sid::String& name
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\012\001", MsgcontactContactGroupGiveDisplayNameRequest::BEGIN,&object_id,&name);
}

bool ContactGroup::Delete(
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\012\002",MsgcontactContactGroupDeleteRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGroupDeleteResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool ContactGroup::GetConversations(
        ConversationRefs& conversations
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\012\003",MsgcontactContactGroupGetConversationsRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGroupGetConversationsResponse::BEGIN,&conversations);
                return __status;
        }
        return false;
}

bool ContactGroup::CanAddConversation(
        bool& result,// true if AddConversation(contact) works on this group
        const ConversationRef conversation
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\012\004",MsgcontactContactGroupCanAddConversationRequest::BEGIN,&object_id,&conversation)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGroupCanAddConversationResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool ContactGroup::AddConversation(
        const ConversationRef& conversation
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\012\005", MsgcontactContactGroupAddConversationRequest::BEGIN,&object_id,&conversation);
}

bool ContactGroup::CanRemoveConversation(
        bool& result// true if RemoveConversation(contact) works on this group
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\012\006",MsgcontactContactGroupCanRemoveConversationRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGroupCanRemoveConversationResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool ContactGroup::RemoveConversation(
        const ConversationRef& conversation
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\012\007", MsgcontactContactGroupRemoveConversationRequest::BEGIN,&object_id,&conversation);
}

bool ContactGroup::GetContacts(
        ContactRefs& contacts
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\012\010",MsgcontactContactGroupGetContactsRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGroupGetContactsResponse::BEGIN,&contacts);
                return __status;
        }
        return false;
}

bool ContactGroup::CanAddContact(
        bool& result,// true if AddContact(contact) works on this group
        const ContactRef contact
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\012\011",MsgcontactContactGroupCanAddContactRequest::BEGIN,&object_id,&contact)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGroupCanAddContactResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool ContactGroup::AddContact(
        const ContactRef& contact
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\012\012", MsgcontactContactGroupAddContactRequest::BEGIN,&object_id,&contact);
}

bool ContactGroup::CanRemoveContact(
        bool& result// true if RemoveContact(contact) works on this group
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\012\013",MsgcontactContactGroupCanRemoveContactRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGroupCanRemoveContactResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool ContactGroup::RemoveContact(
        const ContactRef& contact
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\012\014", MsgcontactContactGroupRemoveContactRequest::BEGIN,&object_id,&contact);
}

bool Skype::GetHardwiredContactGroup(
        const ContactGroup::TYPE& type,
        ContactGroupRef& contactGroup
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\001",MsgcontactGetHardwiredContactGroupRequest::BEGIN,&type)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactGetHardwiredContactGroupResponse::BEGIN,&contactGroup);
                return __status;
        }
        return false;
}

bool Skype::GetCustomContactGroups(
        ContactGroupRefs& groups
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\002",MsgcontactGetCustomContactGroupsResponse::BEGIN,&groups);
        return __status;
}

bool Skype::CreateCustomContactGroup(
        ContactGroupRef& group
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\003",MsgcontactCreateCustomContactGroupResponse::BEGIN,&group);
        return __status;
}

bool Contact::GetType(
        TYPE& type
)
{
        SERootObject* __session = root;
        int __rsp_type;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\001",MsgcontactContactGetTypeRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGetTypeResponse::BEGIN,&__rsp_type);
                type=static_cast<TYPE>(__rsp_type);
                return __status;
        }
        return false;
}

bool Contact::GetIdentity(
        Sid::String& identity// returns CONTACT_SKYPENAME or CONTACT_PSTNNUMBER value
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\002",MsgcontactContactGetIdentityRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGetIdentityResponse::BEGIN,&identity);
                return __status;
        }
        return false;
}

bool Contact::GetAvatar(
        bool& present,
        Sid::Binary& avatar
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\004",MsgcontactContactGetAvatarRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGetAvatarResponse::BEGIN,&present,&avatar);
                return __status;
        }
        return false;
}

bool Contact::GetVerifiedEmail(
        Sid::String& email
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\003",MsgcontactContactGetVerifiedEmailRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGetVerifiedEmailResponse::BEGIN,&email);
                return __status;
        }
        return false;
}

bool Contact::GetVerifiedCompany(
        Sid::String& company
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\010",MsgcontactContactGetVerifiedCompanyRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGetVerifiedCompanyResponse::BEGIN,&company);
                return __status;
        }
        return false;
}

bool Contact::IsMemberOf(
        const ContactGroupRef& group,
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\006",MsgcontactContactIsMemberOfRequest::BEGIN,&object_id,&group)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactIsMemberOfResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Contact::IsMemberOfHardwiredGroup(
        const ContactGroup::TYPE& groupType,
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\007",MsgcontactContactIsMemberOfHardwiredGroupRequest::BEGIN,&object_id,&groupType)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactIsMemberOfHardwiredGroupResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Contact::SetBlocked(
        const bool& blocked,
        const bool abuse
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\002\026", MsgcontactContactSetBlockedRequest::BEGIN,&object_id,&blocked,&abuse);
}

bool Contact::IgnoreAuthRequest(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\002\025", MsgcontactContactIgnoreAuthRequestRequest::BEGIN,&object_id);
}

bool Contact::GiveDisplayName(
        const Sid::String& name
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\002\012", MsgcontactContactGiveDisplayNameRequest::BEGIN,&object_id,&name);
}

bool Contact::SetBuddyStatus(
        const bool& isMyBuddy,
        const bool syncAuth
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\002\014", MsgcontactContactSetBuddyStatusRequest::BEGIN,&object_id,&isMyBuddy,&syncAuth);
}

bool Contact::SendAuthRequest(
        const Sid::String& message,
        const uint extras_bitmask// bitmask of EXTRA_AUTHREQ_FIELDS
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\002\015", MsgcontactContactSendAuthRequestRequest::BEGIN,&object_id,&message,&extras_bitmask);
}

bool Contact::HasAuthorizedMe(
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\016",MsgcontactContactHasAuthorizedMeRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactHasAuthorizedMeResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Contact::SetPhoneNumber(
        const uint& num,// in range 1..3
        const Sid::String& label,
        const Sid::String& number
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\002\017", MsgcontactContactSetPhoneNumberRequest::BEGIN,&object_id,&num,&label,&number);
}

bool Contact::OpenConversation(
        ConversationRef& conversation
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\021",MsgcontactContactOpenConversationRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactOpenConversationResponse::BEGIN,&conversation);
                return __status;
        }
        return false;
}

bool Contact::HasCapability(
        const CAPABILITY& capability,
        bool& result,
        const bool queryServer// sets CONTACT_REFRESHING if querying from server
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\022",MsgcontactContactHasCapabilityRequest::BEGIN,&object_id,&capability,&queryServer)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactHasCapabilityResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Contact::GetCapabilityStatus(
        const CAPABILITY& capability,
        CAPABILITYSTATUS& status,
        const bool queryServer
)
{
        SERootObject* __session = root;
        int __rsp_status;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\002\023",MsgcontactContactGetCapabilityStatusRequest::BEGIN,&object_id,&capability,&queryServer)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactContactGetCapabilityStatusResponse::BEGIN,&__rsp_status);
                status=static_cast<CAPABILITYSTATUS>(__rsp_status);
                return __status;
        }
        return false;
}

bool Contact::RefreshProfile(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\002\024", MsgcontactContactRefreshProfileRequest::BEGIN,&object_id);
}

bool Skype::GetContactType(
        const Sid::String& identity,
        Contact::TYPE& type
)
{
        SERootObject* __session = this;
        int __rsp_type;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\005",MsgcontactGetContactTypeRequest::BEGIN,&identity)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactGetContactTypeResponse::BEGIN,&__rsp_type);
                type=static_cast<Contact::TYPE>(__rsp_type);
                return __status;
        }
        return false;
}

bool Skype::GetContact(
        const Sid::String& identity,
        ContactRef& contact
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\006",MsgcontactGetContactRequest::BEGIN,&identity)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactGetContactResponse::BEGIN,&contact);
                return __status;
        }
        return false;
}

bool Skype::FindContactByPstnNumber(
        const Sid::String& number,
        bool& found,
        ContactRef& contact,
        uint& foundInKey// type is actually PROPKEY
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\010",MsgcontactFindContactByPstnNumberRequest::BEGIN,&number)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactFindContactByPstnNumberResponse::BEGIN,&found,&contact,&foundInKey);
                return __status;
        }
        return false;
}

bool Skype::GetIdentityType(
        const Sid::String& identity,
        IDENTITYTYPE& type
)
{
        SERootObject* __session = this;
        int __rsp_type;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\023",MsgcontactGetIdentityTypeRequest::BEGIN,&identity)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactGetIdentityTypeResponse::BEGIN,&__rsp_type);
                type=static_cast<IDENTITYTYPE>(__rsp_type);
                return __status;
        }
        return false;
}

bool Skype::IdentitiesMatch(
        const Sid::String& identityA,
        const Sid::String& identityB,
        bool& result
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000X",MsgcontactIdentitiesMatchRequest::BEGIN,&identityA,&identityB)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactIdentitiesMatchResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Skype::NormalizeIdentity(
        const Sid::String& original,
        NORMALIZERESULT& result,
        Sid::String& normalized,
        const bool isNewSkypeName
)
{
        SERootObject* __session = this;
        int __rsp_result;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\011",MsgcontactNormalizeIdentityRequest::BEGIN,&original,&isNewSkypeName)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactNormalizeIdentityResponse::BEGIN,&__rsp_result,&normalized);
                result=static_cast<NORMALIZERESULT>(__rsp_result);
                return __status;
        }
        return false;
}

bool Skype::NormalizePSTNWithCountry(
        const Sid::String& original,
        NORMALIZERESULT& result,
        Sid::String& normalized,
        const uint countryPrefix
)
{
        SERootObject* __session = this;
        int __rsp_result;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\315\001",MsgcontactNormalizePSTNWithCountryRequest::BEGIN,&original,&countryPrefix)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactNormalizePSTNWithCountryResponse::BEGIN,&__rsp_result,&normalized);
                result=static_cast<NORMALIZERESULT>(__rsp_result);
                return __status;
        }
        return false;
}

bool ContactSearch::AddMinAgeTerm(
        const uint& min_age_in_years,
        bool& valid,
        const bool add_to_subs
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\001\001",MsgcontactsearchContactSearchAddMinAgeTermRequest::BEGIN,&object_id,&min_age_in_years,&add_to_subs)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchContactSearchAddMinAgeTermResponse::BEGIN,&valid);
                return __status;
        }
        return false;
}

bool ContactSearch::AddMaxAgeTerm(
        const uint& max_age_in_years,
        bool& valid,
        const bool add_to_subs
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\001\002",MsgcontactsearchContactSearchAddMaxAgeTermRequest::BEGIN,&object_id,&max_age_in_years,&add_to_subs)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchContactSearchAddMaxAgeTermResponse::BEGIN,&valid);
                return __status;
        }
        return false;
}

bool ContactSearch::AddEmailTerm(
        const Sid::String& email,
        bool& valid,
        const bool add_to_subs
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\001\003",MsgcontactsearchContactSearchAddEmailTermRequest::BEGIN,&object_id,&email,&add_to_subs)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchContactSearchAddEmailTermResponse::BEGIN,&valid);
                return __status;
        }
        return false;
}

bool ContactSearch::AddLanguageTerm(
        const Sid::String& language,
        bool& valid,
        const bool add_to_subs
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\001\004",MsgcontactsearchContactSearchAddLanguageTermRequest::BEGIN,&object_id,&language,&add_to_subs)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchContactSearchAddLanguageTermResponse::BEGIN,&valid);
                return __status;
        }
        return false;
}

bool ContactSearch::AddStrTerm(
        const int& prop,// only selected contact properties
        const CONDITION& cond,
        const Sid::String& value,
        bool& valid,
        const bool add_to_subs
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\001\005",MsgcontactsearchContactSearchAddStrTermRequest::BEGIN,&object_id,&prop,&cond,&value,&add_to_subs)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchContactSearchAddStrTermResponse::BEGIN,&valid);
                return __status;
        }
        return false;
}

bool ContactSearch::AddIntTerm(
        const int& prop,// only selected contact properties
        const CONDITION& cond,
        const uint& value,
        bool& valid,
        const bool add_to_subs
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\001\006",MsgcontactsearchContactSearchAddIntTermRequest::BEGIN,&object_id,&prop,&cond,&value,&add_to_subs)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchContactSearchAddIntTermResponse::BEGIN,&valid);
                return __status;
        }
        return false;
}

bool ContactSearch::AddOr(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\001\007", MsgcontactsearchContactSearchAddOrRequest::BEGIN,&object_id);
}

bool ContactSearch::IsValid(
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\001\010",MsgcontactsearchContactSearchIsValidRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchContactSearchIsValidResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool ContactSearch::Submit(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\001\011", MsgcontactsearchContactSearchSubmitRequest::BEGIN,&object_id);
}

bool ContactSearch::Extend(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\001\012", MsgcontactsearchContactSearchExtendRequest::BEGIN,&object_id);
}

bool ContactSearch::Release(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\001\014", MsgcontactsearchContactSearchReleaseRequest::BEGIN,&object_id);
}

bool ContactSearch::GetResults(
        ContactRefs& contacts,
        const uint from,
        const uint count
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\001\013",MsgcontactsearchContactSearchGetResultsRequest::BEGIN,&object_id,&from,&count)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchContactSearchGetResultsResponse::BEGIN,&contacts);
                return __status;
        }
        return false;
}

bool Skype::GetOptimalAgeRanges(
        Sid::List_uint& rangeList
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000M",MsgcontactsearchGetOptimalAgeRangesResponse::BEGIN,&rangeList);
        return __status;
}

bool Skype::CreateContactSearch(
        ContactSearchRef& search
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\012",MsgcontactsearchCreateContactSearchResponse::BEGIN,&search);
        return __status;
}

bool Skype::CreateBasicContactSearch(
        const Sid::String& text,
        ContactSearchRef& search
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\013",MsgcontactsearchCreateBasicContactSearchRequest::BEGIN,&text)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchCreateBasicContactSearchResponse::BEGIN,&search);
                return __status;
        }
        return false;
}

bool Skype::CreateIdentitySearch(
        const Sid::String& identity,
        ContactSearchRef& search
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\014",MsgcontactsearchCreateIdentitySearchRequest::BEGIN,&identity)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgcontactsearchCreateIdentitySearchResponse::BEGIN,&search);
                return __status;
        }
        return false;
}

bool Participant::CanSetRankTo(
        const RANK& rank,
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\023\001",MsgconversationParticipantCanSetRankToRequest::BEGIN,&object_id,&rank)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationParticipantCanSetRankToResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Participant::SetRankTo(
        const RANK& rank
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\023\002", MsgconversationParticipantSetRankToRequest::BEGIN,&object_id,&rank);
}

bool Participant::Ring(
        const Sid::String identityToUse,// ring an alternate identity, such as a PSTN number
        const bool videoCall,// enable video
        const uint nrofRedials,// Unused
        const uint redialPeriod,// Unused
        const bool autoStartVM,// Unused.  On dialog, if falling on VM, greeting and recording will be automatically started
        const Sid::String origin// When call is initiated from web link, must contain the URI that was used
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\023\003", MsgconversationParticipantRingRequest::BEGIN,&object_id,&identityToUse,&videoCall,&nrofRedials,&redialPeriod,&autoStartVM,&origin);
}

bool Participant::RingIt(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\023\010", MsgconversationParticipantRingItRequest::BEGIN,&object_id);
}

bool Participant::SetLiveIdentityToUse(
        const Sid::String identityToUse// Empty string will reset it to default, ie the participant identity
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\023\007", MsgconversationParticipantSetLiveIdentityToUseRequest::BEGIN,&object_id,&identityToUse);
}

bool Participant::GetVideo(
        VideoRef& video
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\023\004",MsgconversationParticipantGetVideoRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationParticipantGetVideoResponse::BEGIN,&video);
                return __status;
        }
        return false;
}

bool Participant::Hangup(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\023\005", MsgconversationParticipantHangupRequest::BEGIN,&object_id);
}

bool Participant::Retire(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\023\006", MsgconversationParticipantRetireRequest::BEGIN,&object_id);
}

bool Conversation::SetOption(
        const int& propKey,// allowed values are conversation 'opt' properties
        const uint& value
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\001", MsgconversationConversationSetOptionRequest::BEGIN,&object_id,&propKey,&value);
}

bool Conversation::SetTopic(
        const Sid::String& topic,
        const bool isXML
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\002", MsgconversationConversationSetTopicRequest::BEGIN,&object_id,&topic,&isXML);
}

bool Conversation::SetGuidelines(
        const Sid::String& guidelines,
        const bool isXML
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\003", MsgconversationConversationSetGuidelinesRequest::BEGIN,&object_id,&guidelines,&isXML);
}

bool Conversation::SetPicture(
        const Sid::Binary& jpeg
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\004", MsgconversationConversationSetPictureRequest::BEGIN,&object_id,&jpeg);
}

bool Conversation::SpawnConference(
        const Sid::List_String& identitiesToAdd,
        ConversationRef& conference// resulting conference
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022\006",MsgconversationConversationSpawnConferenceRequest::BEGIN,&object_id,&identitiesToAdd)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationSpawnConferenceResponse::BEGIN,&conference);
                return __status;
        }
        return false;
}

bool Conversation::AddConsumers(
        const Sid::List_String& identities
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\007", MsgconversationConversationAddConsumersRequest::BEGIN,&object_id,&identities);
}

bool Conversation::Assimilate(
        const ConversationRef& otherConversation,
        ConversationRef& conversation// new live conference
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022\011",MsgconversationConversationAssimilateRequest::BEGIN,&object_id,&otherConversation)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationAssimilateResponse::BEGIN,&conversation);
                return __status;
        }
        return false;
}

bool Conversation::JoinLiveSession(
        const Sid::String accessToken// if starting a live session, allows to set a custom access token
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\012", MsgconversationConversationJoinLiveSessionRequest::BEGIN,&object_id,&accessToken);
}

bool Conversation::RingOthers(
        const Sid::List_String identities,// Participants to ring. An empty list means ringing all participants of at least speaker level
        const bool videoCall,// Indicates that we want to do a video call (video still needs to be separately enabled)
        const Sid::String origin// When call is initiated from web link, must contain the URI that was used
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022$", MsgconversationConversationRingOthersRequest::BEGIN,&object_id,&identities,&videoCall,&origin);
}

bool Conversation::MuteMyMicrophone(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\013", MsgconversationConversationMuteMyMicrophoneRequest::BEGIN,&object_id);
}

bool Conversation::UnmuteMyMicrophone(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\014", MsgconversationConversationUnmuteMyMicrophoneRequest::BEGIN,&object_id);
}

bool Conversation::HoldMyLiveSession(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\015", MsgconversationConversationHoldMyLiveSessionRequest::BEGIN,&object_id);
}

bool Conversation::ResumeMyLiveSession(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\016", MsgconversationConversationResumeMyLiveSessionRequest::BEGIN,&object_id);
}

bool Conversation::LeaveLiveSession(
        const bool postVoiceAutoresponse
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\017", MsgconversationConversationLeaveLiveSessionRequest::BEGIN,&object_id,&postVoiceAutoresponse);
}

bool Conversation::StartVoiceMessage(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022-", MsgconversationConversationStartVoiceMessageRequest::BEGIN,&object_id);
}

bool Conversation::TransferLiveSession(
        const Sid::List_String& identities,
        const Sid::String transferTopic
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022(", MsgconversationConversationTransferLiveSessionRequest::BEGIN,&object_id,&identities,&transferTopic);
}

bool Conversation::CanTransferLiveSession(
        const Sid::String& identity,
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022.",MsgconversationConversationCanTransferLiveSessionRequest::BEGIN,&object_id,&identity)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationCanTransferLiveSessionResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Conversation::SendDTMF(
        const Participant::DTMF& dtmf,
        const uint lengthInMS
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\020", MsgconversationConversationSendDTMFRequest::BEGIN,&object_id,&dtmf,&lengthInMS);
}

bool Conversation::StopSendDTMF(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\060", MsgconversationConversationStopSendDTMFRequest::BEGIN,&object_id);
}

bool Conversation::SetMyTextStatusTo(
        const Participant::TEXT_STATUS& status
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\022", MsgconversationConversationSetMyTextStatusToRequest::BEGIN,&object_id,&status);
}

bool Conversation::PostText(
        const Sid::String& text,
        MessageRef& message,// resulting message if no error
        const bool isXML
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022\023",MsgconversationConversationPostTextRequest::BEGIN,&object_id,&text,&isXML)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationPostTextResponse::BEGIN,&message);
                return __status;
        }
        return false;
}

bool Conversation::PostContacts(
        const ContactRefs& contacts
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\024", MsgconversationConversationPostContactsRequest::BEGIN,&object_id,&contacts);
}

bool Conversation::PostFiles(
        const Sid::List_Filename& paths,
        const Sid::String& body,
        TRANSFER_SENDFILE_ERROR& error_code,
        Sid::Filename& error_file
)
{
        SERootObject* __session = root;
        int __rsp_error_code;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022\025",MsgconversationConversationPostFilesRequest::BEGIN,&object_id,&paths,&body)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationPostFilesResponse::BEGIN,&__rsp_error_code,&error_file);
                error_code=static_cast<TRANSFER_SENDFILE_ERROR>(__rsp_error_code);
                return __status;
        }
        return false;
}

bool Conversation::PostVoiceMessage(
        const VoicemailRef& voicemail,// DEPRECATED, the method uses the conversation active_voicemail
        const Sid::String& body// Alternative text to send to clients not supporting displaying the POSTED_VOICE_MESSAGE message
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\026", MsgconversationConversationPostVoiceMessageRequest::BEGIN,&object_id,&voicemail,&body);
}

bool Conversation::PostSMS(
        const SmsRef& sms,
        const Sid::String body// DEPRECATED, not used
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\027", MsgconversationConversationPostSMSRequest::BEGIN,&object_id,&sms,&body);
}

bool Conversation::GetJoinBlob(
        Sid::String& blob
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022\030",MsgconversationConversationGetJoinBlobRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationGetJoinBlobResponse::BEGIN,&blob);
                return __status;
        }
        return false;
}

bool Conversation::Join(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\031", MsgconversationConversationJoinRequest::BEGIN,&object_id);
}

bool Conversation::EnterPassword(
        const Sid::String& password
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\032", MsgconversationConversationEnterPasswordRequest::BEGIN,&object_id,&password);
}

bool Conversation::SetPassword(
        const Sid::String& password,
        const Sid::String& hint
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\033", MsgconversationConversationSetPasswordRequest::BEGIN,&object_id,&password,&hint);
}

bool Conversation::RetireFrom(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\034", MsgconversationConversationRetireFromRequest::BEGIN,&object_id);
}

bool Conversation::Delete(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022/", MsgconversationConversationDeleteRequest::BEGIN,&object_id);
}

bool Conversation::RenameTo(
        const Sid::String& name// empty name unsets the local name
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\035", MsgconversationConversationRenameToRequest::BEGIN,&object_id,&name);
}

bool Conversation::SetBookmark(
        const bool& bookmark
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\036", MsgconversationConversationSetBookmarkRequest::BEGIN,&object_id,&bookmark);
}

bool Conversation::SetAlertString(
        const Sid::String& alertString
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\037", MsgconversationConversationSetAlertStringRequest::BEGIN,&object_id,&alertString);
}

bool Conversation::RemoveFromInbox(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022 ", MsgconversationConversationRemoveFromInboxRequest::BEGIN,&object_id);
}

bool Conversation::AddToInbox(
        const uint timestamp// 0 means the inbox_timstamp will be set to now
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022!", MsgconversationConversationAddToInboxRequest::BEGIN,&object_id,&timestamp);
}

bool Conversation::SetConsumedHorizon(
        const uint& timestamp,
        const bool also_unconsume// if true, also unconsumes newer messages
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022\042", MsgconversationConversationSetConsumedHorizonRequest::BEGIN,&object_id,&timestamp,&also_unconsume);
}

bool Conversation::MarkUnread(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\022#", MsgconversationConversationMarkUnreadRequest::BEGIN,&object_id);
}

bool Conversation::IsMemberOf(
        const ContactGroupRef& group,
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022%",MsgconversationConversationIsMemberOfRequest::BEGIN,&object_id,&group)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationIsMemberOfResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Conversation::GetParticipants(
        ParticipantRefs& participants,
        const Conversation::PARTICIPANTFILTER filter
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022&",MsgconversationConversationGetParticipantsRequest::BEGIN,&object_id,&filter)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationGetParticipantsResponse::BEGIN,&participants);
                return __status;
        }
        return false;
}

bool Conversation::GetLastMessages(
        MessageRefs& contextMessages,// Already consumed messages, provided for context
        MessageRefs& unconsumedMessages,// Unconsumed messages
        const uint requireTimestamp// If set returns message from at least this timestamp, if not, include last 24 hours messages
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022'",MsgconversationConversationGetLastMessagesRequest::BEGIN,&object_id,&requireTimestamp)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationGetLastMessagesResponse::BEGIN,&contextMessages,&unconsumedMessages);
                return __status;
        }
        return false;
}

bool Conversation::FindMessage(
        const Sid::String& text,
        MessageRef& message,
        const uint fromTimestampUp
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\022)",MsgconversationConversationFindMessageRequest::BEGIN,&object_id,&text,&fromTimestampUp)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationConversationFindMessageResponse::BEGIN,&message);
                return __status;
        }
        return false;
}

bool Skype::CreateConference(
        ConversationRef& conference
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\015",MsgconversationCreateConferenceResponse::BEGIN,&conference);
        return __status;
}

bool Skype::GetConversationByIdentity(
        const Sid::String& convoIdentity,
        ConversationRef& conversation
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\017",MsgconversationGetConversationByIdentityRequest::BEGIN,&convoIdentity)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationGetConversationByIdentityResponse::BEGIN,&conversation);
                return __status;
        }
        return false;
}

bool Skype::GetConversationByParticipants(
        const Sid::List_String& participantIdentities,
        ConversationRef& conversation,
        const bool createIfNonExisting,
        const bool ignoreBookmarkedOrNamed
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\020",MsgconversationGetConversationByParticipantsRequest::BEGIN,&participantIdentities,&createIfNonExisting,&ignoreBookmarkedOrNamed)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationGetConversationByParticipantsResponse::BEGIN,&conversation);
                return __status;
        }
        return false;
}

bool Skype::GetConversationByBlob(
        const Sid::String& joinBlob,
        ConversationRef& conversation,
        const bool alsoJoin
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\021",MsgconversationGetConversationByBlobRequest::BEGIN,&joinBlob,&alsoJoin)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationGetConversationByBlobResponse::BEGIN,&conversation);
                return __status;
        }
        return false;
}

bool Skype::GetConversationList(
        ConversationRefs& conversations,
        const Conversation::LIST_TYPE type
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\022",MsgconversationGetConversationListRequest::BEGIN,&type)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationGetConversationListResponse::BEGIN,&conversations);
                return __status;
        }
        return false;
}

bool Message::CanEdit(
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\011\001",MsgconversationMessageCanEditRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationMessageCanEditResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Message::Edit(
        const Sid::String& newText,
        const bool isXML,
        const bool undo// Reverts the message body to the original version. newText parameter is ignored when this is set
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\011\002", MsgconversationMessageEditRequest::BEGIN,&object_id,&newText,&isXML,&undo);
}

bool Message::GetContacts(
        ContactRefs& contacts
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\011\003",MsgconversationMessageGetContactsRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationMessageGetContactsResponse::BEGIN,&contacts);
                return __status;
        }
        return false;
}

bool Message::GetTransfers(
        TransferRefs& transfers
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\011\004",MsgconversationMessageGetTransfersRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationMessageGetTransfersResponse::BEGIN,&transfers);
                return __status;
        }
        return false;
}

bool Message::GetVoiceMessage(
        VoicemailRef& voicemail
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\011\005",MsgconversationMessageGetVoiceMessageRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationMessageGetVoiceMessageResponse::BEGIN,&voicemail);
                return __status;
        }
        return false;
}

bool Message::GetSMS(
        SmsRef& sms
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\011\006",MsgconversationMessageGetSMSRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationMessageGetSMSResponse::BEGIN,&sms);
                return __status;
        }
        return false;
}

bool Message::DeleteLocally(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\011\010", MsgconversationMessageDeleteLocallyRequest::BEGIN,&object_id);
}

bool Skype::GetMessageByGuid(
        const Sid::Binary& guid,
        MessageRef& message
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\025",MsgconversationGetMessageByGuidRequest::BEGIN,&guid)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationGetMessageByGuidResponse::BEGIN,&message);
                return __status;
        }
        return false;
}

bool Skype::GetMessageListByType(
        const Message::TYPE& type,// Type of messages requested
        const bool& latestPerConvOnly,// Whether to return only the most recent message per conversation
        MessageRefs& messages,
        const uint fromTimestampInc,// Starting timestamp for reqested range, inclusive
        const uint toTimestampExc// Ending timestamp for requested range, exclusive
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\210\001",MsgconversationGetMessageListByTypeRequest::BEGIN,&type,&latestPerConvOnly,&fromTimestampInc,&toTimestampExc)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgconversationGetMessageListByTypeResponse::BEGIN,&messages);
                return __status;
        }
        return false;
}

bool Video::SetScreen(
        const uint& windowh
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\013\001", MsgvideoVideoSetScreenRequest::BEGIN,&object_id,&windowh);
}

bool Video::Start(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\013\002", MsgvideoVideoStartRequest::BEGIN,&object_id);
}

bool Video::Stop(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\013\003", MsgvideoVideoStopRequest::BEGIN,&object_id);
}

bool Video::SubmitCaptureRequest(
        bool& ret,
        uint& requestId
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\013\013",MsgvideoVideoSubmitCaptureRequestRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgvideoVideoSubmitCaptureRequestResponse::BEGIN,&ret,&requestId);
                return __status;
        }
        return false;
}

bool Video::SetScreenCaptureRectangle(
        const int& x0,
        const int& y0,
        const uint& width,
        const uint& height,
        const int monitorNumber,
        const uint windowHandle
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\013\005", MsgvideoVideoSetScreenCaptureRectangleRequest::BEGIN,&object_id,&x0,&y0,&width,&height,&monitorNumber,&windowHandle);
}

bool Video::SetRenderRectangle(
        const int& x0,
        const int& y0,
        const uint& width,
        const uint& height
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\013\006", MsgvideoVideoSetRenderRectangleRequest::BEGIN,&object_id,&x0,&y0,&width,&height);
}

bool Video::SetRemoteRendererId(
        const uint& id
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\013\016", MsgvideoVideoSetRemoteRendererIdRequest::BEGIN,&object_id,&id);
}

bool Video::SelectVideoSource(
        const Video::MEDIATYPE& mediaType,
        const Sid::String webcamName,
        const Sid::String devicePath,
        const bool updateSetup
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\013\007", MsgvideoVideoSelectVideoSourceRequest::BEGIN,&object_id,&mediaType,&webcamName,&devicePath,&updateSetup);
}

bool Video::GetCurrentVideoDevice(
        MEDIATYPE& mediatype,
        Sid::String& deviceName,
        Sid::String& devicePath
)
{
        SERootObject* __session = root;
        int __rsp_mediatype;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\013\012",MsgvideoVideoGetCurrentVideoDeviceRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgvideoVideoGetCurrentVideoDeviceResponse::BEGIN,&__rsp_mediatype,&deviceName,&devicePath);
                mediatype=static_cast<MEDIATYPE>(__rsp_mediatype);
                return __status;
        }
        return false;
}

bool Skype::GetAvailableVideoDevices(
        Sid::List_String& deviceNames,
        Sid::List_String& devicePaths,
        uint& count
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000P",MsgvideoGetAvailableVideoDevicesResponse::BEGIN,&deviceNames,&devicePaths,&count);
        return __status;
}

bool Skype::HasVideoDeviceCapability(
        const Sid::String& deviceName,
        const Sid::String& devicePath,
        const Video::VIDEO_DEVICE_CAPABILITY& cap,
        bool& result
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000!",MsgvideoHasVideoDeviceCapabilityRequest::BEGIN,&deviceName,&devicePath,&cap)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgvideoHasVideoDeviceCapabilityResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Skype::DisplayVideoDeviceTuningDialog(
        const Sid::String& deviceName,
        const Sid::String& devicePath
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000\042", MsgvideoDisplayVideoDeviceTuningDialogRequest::BEGIN,&deviceName,&devicePath);
}

bool Skype::GetPreviewVideo(
        const Video::MEDIATYPE& type,
        VideoRef& video,
        const Sid::String deviceName,// name and path to be used only with media type VIDEO
        const Sid::String devicePath
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000#",MsgvideoGetPreviewVideoRequest::BEGIN,&type,&deviceName,&devicePath)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgvideoGetPreviewVideoResponse::BEGIN,&video);
                return __status;
        }
        return false;
}

bool Skype::VideoCommand(
        const Sid::String& command,
        Sid::String& response
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000;",MsgvideoVideoCommandRequest::BEGIN,&command)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgvideoVideoCommandResponse::BEGIN,&response);
                return __status;
        }
        return false;
}

bool Voicemail::StartRecording(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\007\003", MsgvmVoicemailStartRecordingRequest::BEGIN,&object_id);
}

bool Voicemail::StopRecording(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\007\004", MsgvmVoicemailStopRecordingRequest::BEGIN,&object_id);
}

bool Voicemail::StartPlayback(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\007\005", MsgvmVoicemailStartPlaybackRequest::BEGIN,&object_id);
}

bool Voicemail::StopPlayback(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\007\006", MsgvmVoicemailStopPlaybackRequest::BEGIN,&object_id);
}

bool Voicemail::Delete(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\007\007", MsgvmVoicemailDeleteRequest::BEGIN,&object_id);
}

bool Voicemail::Cancel(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\007\010", MsgvmVoicemailCancelRequest::BEGIN,&object_id);
}

bool Voicemail::CheckPermission(
        bool& result
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\007\015",MsgvmVoicemailCheckPermissionRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgvmVoicemailCheckPermissionResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Skype::GetGreeting(
        const Sid::String& skypeName,
        VoicemailRef& greeting
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000-",MsgvmGetGreetingRequest::BEGIN,&skypeName)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgvmGetGreetingResponse::BEGIN,&greeting);
                return __status;
        }
        return false;
}

bool Skype::PlayStart(
        const uint& soundid,
        const Sid::Binary& sound,
        const bool& loop,
        const bool useCallOutDevice
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000\060", MsgaudioPlayStartRequest::BEGIN,&soundid,&sound,&loop,&useCallOutDevice);
}

bool Skype::PlayStartFromFile(
        const uint& soundid,
        const Sid::Filename& datafile,
        const bool& loop,
        PREPARESOUNDRESULT& result,
        const bool useCallOutDevice
)
{
        SERootObject* __session = this;
        int __rsp_result;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\324\001",MsgaudioPlayStartFromFileRequest::BEGIN,&soundid,&datafile,&loop,&useCallOutDevice)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaudioPlayStartFromFileResponse::BEGIN,&__rsp_result);
                result=static_cast<PREPARESOUNDRESULT>(__rsp_result);
                return __status;
        }
        return false;
}

bool Skype::PlayStop(
        const uint& soundid
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000\061", MsgaudioPlayStopRequest::BEGIN,&soundid);
}

bool Skype::StartRecordingTest(
        const bool recordAndPlaybackData
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000\062", MsgaudioStartRecordingTestRequest::BEGIN,&recordAndPlaybackData);
}

bool Skype::StopRecordingTest(
)
{
        SERootObject* __session = this;
        return __session->call_00(4,"ZR\000\063");
}

bool Skype::GetAvailableOutputDevices(
        Sid::List_String& handleList,
        Sid::List_String& nameList,
        Sid::List_String& productIdList
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\065",MsgaudioGetAvailableOutputDevicesResponse::BEGIN,&handleList,&nameList,&productIdList);
        return __status;
}

bool Skype::GetAvailableRecordingDevices(
        Sid::List_String& handleList,
        Sid::List_String& nameList,
        Sid::List_String& productIdList
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\066",MsgaudioGetAvailableRecordingDevicesResponse::BEGIN,&handleList,&nameList,&productIdList);
        return __status;
}

bool Skype::SelectSoundDevices(
        const Sid::String& callInDevice,
        const Sid::String& callOutDevice,
        const Sid::String& waveOutDevice
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000\067", MsgaudioSelectSoundDevicesRequest::BEGIN,&callInDevice,&callOutDevice,&waveOutDevice);
}

bool Skype::GetAudioDeviceCapabilities(
        const Sid::String& deviceHandle,
        Sid::String& interfaceString,
        uint& capabilities// bit set of AUDIODEVICE_CAPABILITIES
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\070",MsgaudioGetAudioDeviceCapabilitiesRequest::BEGIN,&deviceHandle)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaudioGetAudioDeviceCapabilitiesResponse::BEGIN,&interfaceString,&capabilities);
                return __status;
        }
        return false;
}

bool Skype::GetNrgLevels(
        uint& micLevel,
        uint& speakerLevel
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\071",MsgaudioGetNrgLevelsResponse::BEGIN,&micLevel,&speakerLevel);
        return __status;
}

bool Skype::VoiceCommand(
        const Sid::String& command,
        Sid::String& response
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000:",MsgaudioVoiceCommandRequest::BEGIN,&command)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaudioVoiceCommandResponse::BEGIN,&response);
                return __status;
        }
        return false;
}

bool Skype::GetSpeakerVolume(
        uint& volume
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000<",MsgaudioGetSpeakerVolumeResponse::BEGIN,&volume);
        return __status;
}

bool Skype::SetSpeakerVolume(
        const uint& volume
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000=", MsgaudioSetSpeakerVolumeRequest::BEGIN,&volume);
}

bool Skype::GetMicVolume(
        uint& micVolume
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000>",MsgaudioGetMicVolumeResponse::BEGIN,&micVolume);
        return __status;
}

bool Skype::SetMicVolume(
        const uint& volume
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000?", MsgaudioSetMicVolumeRequest::BEGIN,&volume);
}

bool Skype::IsSpeakerMuted(
        bool& muted
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000@",MsgaudioIsSpeakerMutedResponse::BEGIN,&muted);
        return __status;
}

bool Skype::IsMicrophoneMuted(
        bool& muted
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\101",MsgaudioIsMicrophoneMutedResponse::BEGIN,&muted);
        return __status;
}

bool Skype::MuteSpeakers(
        const bool& mute
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000\102", MsgaudioMuteSpeakersRequest::BEGIN,&mute);
}

bool Skype::MuteMicrophone(
        const bool& mute
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000\103", MsgaudioMuteMicrophoneRequest::BEGIN,&mute);
}

bool Skype::SetOperatingMedia(
        const OPERATING_MEDIA& media,
        const uint& maxUplinkBps,
        const uint& maxDownlinkBps
)
{
        SERootObject* __session = this;
        return __session->call_m0(5,"ZR\000\377\001", MsgbwmSetOperatingMediaRequest::BEGIN,&media,&maxUplinkBps,&maxDownlinkBps);
}

bool Sms::GetTargetStatus(
        const Sid::String& target,
        TARGETSTATUS& status
)
{
        SERootObject* __session = root;
        int __rsp_status;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\014\004",MsgsmsSmsGetTargetStatusRequest::BEGIN,&object_id,&target)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsmsSmsGetTargetStatusResponse::BEGIN,&__rsp_status);
                status=static_cast<TARGETSTATUS>(__rsp_status);
                return __status;
        }
        return false;
}

bool Sms::GetTargetPrice(
        const Sid::String& target,
        uint& price
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\014\015",MsgsmsSmsGetTargetPriceRequest::BEGIN,&object_id,&target)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsmsSmsGetTargetPriceResponse::BEGIN,&price);
                return __status;
        }
        return false;
}

bool Sms::SetTargets(
        const Sid::List_String& numbers,
        bool& success// false if some targets are not valid pstn-numbers
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\014\006",MsgsmsSmsSetTargetsRequest::BEGIN,&object_id,&numbers)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsmsSmsSetTargetsResponse::BEGIN,&success);
                return __status;
        }
        return false;
}

bool Sms::SetBody(
        const Sid::String& text,
        SETBODYRESULT& result,
        Sid::List_String& chunks,
        uint& charsUntilNextChunk
)
{
        SERootObject* __session = root;
        int __rsp_result;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\014\007",MsgsmsSmsSetBodyRequest::BEGIN,&object_id,&text)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsmsSmsSetBodyResponse::BEGIN,&__rsp_result,&chunks,&charsUntilNextChunk);
                result=static_cast<SETBODYRESULT>(__rsp_result);
                return __status;
        }
        return false;
}

bool Sms::GetBodyChunks(
        Sid::List_String& textChunks,
        uint& charsUntilNextChunk
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\014\010",MsgsmsSmsGetBodyChunksRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsmsSmsGetBodyChunksResponse::BEGIN,&textChunks,&charsUntilNextChunk);
                return __status;
        }
        return false;
}

bool Skype::RequestConfirmationCode(
        const Sms::CONFIRM_TYPE& type,
        const Sid::String& number,
        SmsRef& sms
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\035",MsgsmsRequestConfirmationCodeRequest::BEGIN,&type,&number)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsmsRequestConfirmationCodeResponse::BEGIN,&sms);
                return __status;
        }
        return false;
}

bool Skype::SubmitConfirmationCode(
        const Sid::String& number,
        const Sid::String& code,
        SmsRef& sms
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000\036",MsgsmsSubmitConfirmationCodeRequest::BEGIN,&number,&code)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsmsSubmitConfirmationCodeResponse::BEGIN,&sms);
                return __status;
        }
        return false;
}

bool Skype::CreateOutgoingSms(
        SmsRef& sms
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000\106",MsgsmsCreateOutgoingSmsResponse::BEGIN,&sms);
        return __status;
}

bool Transfer::Accept(
        const Sid::Filename& filenameWithPath,
        bool& success
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\006\003",MsgftTransferAcceptRequest::BEGIN,&object_id,&filenameWithPath)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgftTransferAcceptResponse::BEGIN,&success);
                return __status;
        }
        return false;
}

bool Transfer::Pause(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\006\004", MsgftTransferPauseRequest::BEGIN,&object_id);
}

bool Transfer::Resume(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\006\005", MsgftTransferResumeRequest::BEGIN,&object_id);
}

bool Transfer::Cancel(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\006\006", MsgftTransferCancelRequest::BEGIN,&object_id);
}

bool Account::GetStatusWithProgress(
        STATUS& status,
        uint& progress// progress goes from 0..100 and indicates the progress for current status
)
{
        SERootObject* __session = root;
        int __rsp_status;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\005\001",MsgaccountAccountGetStatusWithProgressRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountAccountGetStatusWithProgressResponse::BEGIN,&__rsp_status,&progress);
                status=static_cast<STATUS>(__rsp_status);
                return __status;
        }
        return false;
}

bool Account::Login(
        const Contact::AVAILABILITY setAvailabilityTo// activate a LOGGED_OUT_AND_PWD_SAVED account. *optionally* set availability
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\005", MsgaccountAccountLoginRequest::BEGIN,&object_id,&setAvailabilityTo);
}

bool Account::LoginWithPassword(
        const Sid::String& password,
        const bool savePwd,
        const bool saveDataLocally
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\006", MsgaccountAccountLoginWithPasswordRequest::BEGIN,&object_id,&password,&savePwd,&saveDataLocally);
}

bool Account::Register(
        const Sid::String& password,
        const bool savePwd,
        const bool saveDataLocally,
        const Sid::String email,
        const bool allowSpam
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\007", MsgaccountAccountRegisterRequest::BEGIN,&object_id,&password,&savePwd,&saveDataLocally,&email,&allowSpam);
}

bool Account::Logout(
        const bool clearSavedPwd
)
{
        SERootObject* __session = root;
        __session->__logging_out(moduleID()); /* disable events apart from account changes */
        return __session->call_m0(4,"ZR\005\010", MsgaccountAccountLogoutRequest::BEGIN,&object_id,&clearSavedPwd);
}

bool Account::ChangePassword(
        const Sid::String& oldPassword,
        const Sid::String& newPassword,
        const bool savePwd
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\013", MsgaccountAccountChangePasswordRequest::BEGIN,&object_id,&oldPassword,&newPassword,&savePwd);
}

bool Account::SetPasswordSaved(
        const bool& savePwd
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\031", MsgaccountAccountSetPasswordSavedRequest::BEGIN,&object_id,&savePwd);
}

bool Account::SetServersideIntProperty(
        const int& propKey,
        const uint& value
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\014", MsgaccountAccountSetServersideIntPropertyRequest::BEGIN,&object_id,&propKey,&value);
}

bool Account::SetServersideStrProperty(
        const int& propKey,
        const Sid::String& value
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\015", MsgaccountAccountSetServersideStrPropertyRequest::BEGIN,&object_id,&propKey,&value);
}

bool Account::CancelServerCommit(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\017", MsgaccountAccountCancelServerCommitRequest::BEGIN,&object_id);
}

bool Account::SetIntProperty(
        const int& propKey,
        const uint& value
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\020", MsgaccountAccountSetIntPropertyRequest::BEGIN,&object_id,&propKey,&value);
}

bool Account::SetStrProperty(
        const int& propKey,
        const Sid::String& value
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\021", MsgaccountAccountSetStrPropertyRequest::BEGIN,&object_id,&propKey,&value);
}

bool Account::SetBinProperty(
        const int& propKey,
        const Sid::Binary& value
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\022", MsgaccountAccountSetBinPropertyRequest::BEGIN,&object_id,&propKey,&value);
}

bool Account::SetAvailability(
        const Contact::AVAILABILITY& availability// only subset of all contact availabilities allowed
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\023", MsgaccountAccountSetAvailabilityRequest::BEGIN,&object_id,&availability);
}

bool Account::SetStandby(
        const bool& standby
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\012", MsgaccountAccountSetStandbyRequest::BEGIN,&object_id,&standby);
}

bool Account::GetCapabilityStatus(
        const Contact::CAPABILITY& capability,
        CAPABILITYSTATUS& status,
        uint& expiryTimestamp
)
{
        SERootObject* __session = root;
        int __rsp_status;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\005\025",MsgaccountAccountGetCapabilityStatusRequest::BEGIN,&object_id,&capability)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountAccountGetCapabilityStatusResponse::BEGIN,&__rsp_status,&expiryTimestamp);
                status=static_cast<CAPABILITYSTATUS>(__rsp_status);
                return __status;
        }
        return false;
}

bool Account::GetSkypenameHash(
        Sid::String& skypenameHash
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\005\026",MsgaccountAccountGetSkypenameHashRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountAccountGetSkypenameHashResponse::BEGIN,&skypenameHash);
                return __status;
        }
        return false;
}

bool Account::GetVerifiedEmail(
        Sid::String& email
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\005\002",MsgaccountAccountGetVerifiedEmailRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountAccountGetVerifiedEmailResponse::BEGIN,&email);
                return __status;
        }
        return false;
}

bool Account::GetVerifiedCompany(
        Sid::String& company
)
{
        SERootObject* __session = root;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\005\003",MsgaccountAccountGetVerifiedCompanyRequest::BEGIN,&object_id)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountAccountGetVerifiedCompanyResponse::BEGIN,&company);
                return __status;
        }
        return false;
}

bool Account::Delete(
)
{
        SERootObject* __session = root;
        return __session->call_m0(4,"ZR\005\030", MsgaccountAccountDeleteRequest::BEGIN,&object_id);
}

bool Skype::GetAccount(
        const Sid::String& identity,
        AccountRef& account
)
{
        SERootObject* __session = this;
        static Sid::String last_skypename;
        if (last_skypename != identity) { cleanup(1,&account); last_skypename = identity; }
        __session->__logout_done(); /* enable events again */
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000s",MsgaccountGetAccountRequest::BEGIN,&identity)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountGetAccountResponse::BEGIN,&account);
                return __status;
        }
        return false;
}

bool Skype::GetExistingAccounts(
        Sid::List_String& accountNameList
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000q",MsgaccountGetExistingAccountsResponse::BEGIN,&accountNameList);
        return __status;
}

bool Skype::GetDefaultAccountName(
        Sid::String& account
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(4,"ZR\000r",MsgaccountGetDefaultAccountNameResponse::BEGIN,&account);
        return __status;
}

bool Skype::GetSuggestedSkypename(
        const Sid::String& fullname,
        Sid::String& suggestedName
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000t",MsgaccountGetSuggestedSkypenameRequest::BEGIN,&fullname)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountGetSuggestedSkypenameResponse::BEGIN,&suggestedName);
                return __status;
        }
        return false;
}

bool Skype::ValidateAvatar(
        const Sid::Binary& value,
        VALIDATERESULT& result,
        int& freeBytesLeft
)
{
        SERootObject* __session = this;
        int __rsp_result;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000w",MsgaccountValidateAvatarRequest::BEGIN,&value)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountValidateAvatarResponse::BEGIN,&__rsp_result,&freeBytesLeft);
                result=static_cast<VALIDATERESULT>(__rsp_result);
                return __status;
        }
        return false;
}

bool Skype::ValidateProfileString(
        const int& propKey,
        const Sid::String& strValue,
        VALIDATERESULT& result,
        int& freeBytesLeft,
        const bool forRegistration
)
{
        SERootObject* __session = this;
        int __rsp_result;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000f",MsgaccountValidateProfileStringRequest::BEGIN,&propKey,&strValue,&forRegistration)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountValidateProfileStringResponse::BEGIN,&__rsp_result,&freeBytesLeft);
                result=static_cast<VALIDATERESULT>(__rsp_result);
                return __status;
        }
        return false;
}

bool Skype::ValidatePassword(
        const Sid::String& username,
        const Sid::String& password,
        VALIDATERESULT& result
)
{
        SERootObject* __session = this;
        int __rsp_result;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000G",MsgaccountValidatePasswordRequest::BEGIN,&username,&password)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgaccountValidatePasswordResponse::BEGIN,&__rsp_result);
                result=static_cast<VALIDATERESULT>(__rsp_result);
                return __status;
        }
        return false;
}

bool Skype::GetUsedPort(
        uint& port
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(5,"ZR\000\202\001",MsgconnectionGetUsedPortResponse::BEGIN,&port);
        return __status;
}

bool Skype::GetStr(
        const Sid::String& key,
        Sid::String& value
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000x",MsgsetupGetStrRequest::BEGIN,&key)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsetupGetStrResponse::BEGIN,&value);
                return __status;
        }
        return false;
}

bool Skype::GetInt(
        const Sid::String& key,
        int& value
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000y",MsgsetupGetIntRequest::BEGIN,&key)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsetupGetIntResponse::BEGIN,&value);
                return __status;
        }
        return false;
}

bool Skype::GetBin(
        const Sid::String& key,
        Sid::Binary& value
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000z",MsgsetupGetBinRequest::BEGIN,&key)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsetupGetBinResponse::BEGIN,&value);
                return __status;
        }
        return false;
}

bool Skype::SetStr(
        const Sid::String& key,
        const Sid::String& value
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000{", MsgsetupSetStrRequest::BEGIN,&key,&value);
}

bool Skype::SetInt(
        const Sid::String& key,
        const int& value
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000|", MsgsetupSetIntRequest::BEGIN,&key,&value);
}

bool Skype::SetBin(
        const Sid::String& key,
        const Sid::Binary& value
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000}", MsgsetupSetBinRequest::BEGIN,&key,&value);
}

bool Skype::IsDefined(
        const Sid::String& key,
        bool& value
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,4,"ZR\000~",MsgsetupIsDefinedRequest::BEGIN,&key)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsetupIsDefinedResponse::BEGIN,&value);
                return __status;
        }
        return false;
}

bool Skype::Delete(
        const Sid::String& key
)
{
        SERootObject* __session = this;
        return __session->call_m0(4,"ZR\000\177", MsgsetupDeleteRequest::BEGIN,&key);
}

bool Skype::GetSubKeys(
        const Sid::String& key,
        Sid::List_String& value
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\200\001",MsgsetupGetSubKeysRequest::BEGIN,&key)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgsetupGetSubKeysResponse::BEGIN,&value);
                return __status;
        }
        return false;
}

bool Skype::GetISOLanguageInfo(
        Sid::List_String& languageCodeList,
        Sid::List_String& languageNameList// assumes UI has set correct language
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(5,"ZR\000\317\001",MsgisoGetISOLanguageInfoResponse::BEGIN,&languageCodeList,&languageNameList);
        return __status;
}

bool Skype::GetISOCountryInfo(
        Sid::List_String& countryCodeList,
        Sid::List_String& countryNameList,// assumes UI has set correct language
        Sid::List_uint& countryPrefixList,
        Sid::List_String& countryDialExampleList
)
{
        SERootObject* __session = this;
        bool __status = __session->call_0n(5,"ZR\000\320\001",MsgisoGetISOCountryInfoResponse::BEGIN,&countryCodeList,&countryNameList,&countryPrefixList,&countryDialExampleList);
        return __status;
}

bool Skype::GetISOCountryCodebyPhoneNo(
        const Sid::String& number,
        Sid::String& countryCode
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\323\001",MsgisoGetISOCountryCodebyPhoneNoRequest::BEGIN,&number)) {
                bool __status = __session->rd_parms_lst(__cmdini,MsgisoGetISOCountryCodebyPhoneNoResponse::BEGIN,&countryCode);
                return __status;
        }
        return false;
}

bool Skype::App2AppCreate(
        const Sid::String& appname,
        bool& result
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\327\001",Msgapp2appApp2AppCreateRequest::BEGIN,&appname)) {
                bool __status = __session->rd_parms_lst(__cmdini,Msgapp2appApp2AppCreateResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Skype::App2AppDelete(
        const Sid::String& appname,
        bool& result
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\330\001",Msgapp2appApp2AppDeleteRequest::BEGIN,&appname)) {
                bool __status = __session->rd_parms_lst(__cmdini,Msgapp2appApp2AppDeleteResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Skype::App2AppConnect(
        const Sid::String& appname,
        const Sid::String& skypename,
        bool& result
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\331\001",Msgapp2appApp2AppConnectRequest::BEGIN,&appname,&skypename)) {
                bool __status = __session->rd_parms_lst(__cmdini,Msgapp2appApp2AppConnectResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Skype::App2AppDisconnect(
        const Sid::String& appname,
        const Sid::String& stream,
        bool& result
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\332\001",Msgapp2appApp2AppDisconnectRequest::BEGIN,&appname,&stream)) {
                bool __status = __session->rd_parms_lst(__cmdini,Msgapp2appApp2AppDisconnectResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Skype::App2AppWrite(
        const Sid::String& appname,
        const Sid::String& stream,
        const Sid::Binary& data,
        bool& result
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\333\001",Msgapp2appApp2AppWriteRequest::BEGIN,&appname,&stream,&data)) {
                bool __status = __session->rd_parms_lst(__cmdini,Msgapp2appApp2AppWriteResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Skype::App2AppDatagram(
        const Sid::String& appname,
        const Sid::String& stream,
        const Sid::Binary& data,
        bool& result
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\334\001",Msgapp2appApp2AppDatagramRequest::BEGIN,&appname,&stream,&data)) {
                bool __status = __session->rd_parms_lst(__cmdini,Msgapp2appApp2AppDatagramResponse::BEGIN,&result);
                return __status;
        }
        return false;
}

bool Skype::App2AppRead(
        const Sid::String& appname,
        const Sid::String& stream,
        bool& result,
        Sid::Binary& data
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\335\001",Msgapp2appApp2AppReadRequest::BEGIN,&appname,&stream)) {
                bool __status = __session->rd_parms_lst(__cmdini,Msgapp2appApp2AppReadResponse::BEGIN,&result,&data);
                return __status;
        }
        return false;
}

bool Skype::App2AppGetConnectableUsers(
        const Sid::String& appname,
        bool& result,
        Sid::List_String& users
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\336\001",Msgapp2appApp2AppGetConnectableUsersRequest::BEGIN,&appname)) {
                bool __status = __session->rd_parms_lst(__cmdini,Msgapp2appApp2AppGetConnectableUsersResponse::BEGIN,&result,&users);
                return __status;
        }
        return false;
}

bool Skype::App2AppGetStreamsList(
        const Sid::String& appname,
        const APP2APP_STREAMS& listType,
        bool& result,
        Sid::List_String& streams,
        Sid::List_uint& receivedSizes// For RECEIVED_STREAMS, contains the number of bytes in each stream waiting to be read
)
{
        SERootObject* __session = this;
        Sid::CommandInitiator* __cmdini;
        if (__session->call_mn(__cmdini,5,"ZR\000\337\001",Msgapp2appApp2AppGetStreamsListRequest::BEGIN,&appname,&listType)) {
                bool __status = __session->rd_parms_lst(__cmdini,Msgapp2appApp2AppGetStreamsListResponse::BEGIN,&result,&streams,&receivedSizes);
                return __status;
        }
        return false;
}
#ifdef SE_USE_NAMESPACE
} // Skype
#endif
