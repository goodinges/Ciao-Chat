#include "skype-embedded_2.h"
Skype::Skype(int num_threads) : SERootObject(num_threads, SkypeFields) {}
Skype::~Skype() {}

const unsigned int Skype::moduleID() const 
{
        return 0;
}


/** class ContactGroup **********************/

ContactGroup::ContactGroup(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
ContactGroup::~ContactGroup() {  }
void ContactGroup::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgcontactContactGroup*>(cache);  }
unsigned int ContactGroup::moduleID() const 
{
        return MODULE_ID;
}
String ContactGroup::getNamespace() const 
{
        return __EA(CONTACTGROUP);
}

extern const PMAP CONTACTGROUP_PROPERTY_MAP[] = {
        {__EA(TYPE), ContactGroup::P_TYPE},
        {__EA(CUSTOM_GROUP_ID), ContactGroup::P_CUSTOM_GROUP_ID},
        {__EA(GIVEN_DISPLAYNAME), ContactGroup::P_GIVEN_DISPLAYNAME},
        {__EA(NROFCONTACTS), ContactGroup::P_NROFCONTACTS},
        {__EA(NROFCONTACTS_ONLINE), ContactGroup::P_NROFCONTACTS_ONLINE},
        {0, 0}
};

extern const PMAP CONTACTGROUP_TYPE_MAP[] = {
        {__EA(ALL_KNOWN_CONTACTS), ContactGroup::ALL_KNOWN_CONTACTS},
        {__EA(ALL_BUDDIES), ContactGroup::ALL_BUDDIES},
        {__EA(SKYPE_BUDDIES), ContactGroup::SKYPE_BUDDIES},
        {__EA(SKYPEOUT_BUDDIES), ContactGroup::SKYPEOUT_BUDDIES},
        {__EA(ONLINE_BUDDIES), ContactGroup::ONLINE_BUDDIES},
        {__EA(UNKNOWN_OR_PENDINGAUTH_BUDDIES), ContactGroup::UNKNOWN_OR_PENDINGAUTH_BUDDIES},
        {__EA(RECENTLY_CONTACTED_CONTACTS), ContactGroup::RECENTLY_CONTACTED_CONTACTS},
        {__EA(CONTACTS_WAITING_MY_AUTHORIZATION), ContactGroup::CONTACTS_WAITING_MY_AUTHORIZATION},
        {__EA(CONTACTS_AUTHORIZED_BY_ME), ContactGroup::CONTACTS_AUTHORIZED_BY_ME},
        {__EA(CONTACTS_BLOCKED_BY_ME), ContactGroup::CONTACTS_BLOCKED_BY_ME},
        {__EA(UNGROUPED_BUDDIES), ContactGroup::UNGROUPED_BUDDIES},
        {__EA(CUSTOM_GROUP), ContactGroup::CUSTOM_GROUP},
        {__EA(PROPOSED_SHARED_GROUP), ContactGroup::PROPOSED_SHARED_GROUP},
        {__EA(SHARED_GROUP), ContactGroup::SHARED_GROUP},
        {__EA(EXTERNAL_CONTACTS), ContactGroup::EXTERNAL_CONTACTS},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const ContactGroup::TYPE& v)
        {
                return enumtostring("ContactGroup::TYPE",CONTACTGROUP_TYPE_MAP,v);
        }
}

void ContactGroup::OnChangeConversation(
        __se_unused const ConversationRef& /*conversation*/
) {}

void ContactGroup::OnChange(
        __se_unused const ContactRef& /*contact*/
) {}

extern const EMAP CONTACTGROUP_EMAP[] = {
        {ContactGroup::P_TYPE, CONTACTGROUP_TYPE_MAP},
        {0, 0}
};
const EMAP* ContactGroup::getEMap() const 
{
        return CONTACTGROUP_EMAP;
}
const PMAP* ContactGroup::getPMap() const 
{
        return CONTACTGROUP_PROPERTY_MAP;
}


/** class Contact **********************/

Contact::Contact(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
Contact::~Contact() {  }
void Contact::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgcontactContact*>(cache);  }
unsigned int Contact::moduleID() const 
{
        return MODULE_ID;
}
String Contact::getNamespace() const 
{
        return __EA(CONTACT);
}

extern const PMAP CONTACT_PROPERTY_MAP[] = {
        {__EA(TYPE), Contact::P_TYPE},
        {__EA(SKYPENAME), Contact::P_SKYPENAME},
        {__EA(PSTNNUMBER), Contact::P_PSTNNUMBER},
        {__EA(FULLNAME), Contact::P_FULLNAME},
        {__EA(BIRTHDAY), Contact::P_BIRTHDAY},
        {__EA(GENDER), Contact::P_GENDER},
        {__EA(LANGUAGES), Contact::P_LANGUAGES},
        {__EA(COUNTRY), Contact::P_COUNTRY},
        {__EA(PROVINCE), Contact::P_PROVINCE},
        {__EA(CITY), Contact::P_CITY},
        {__EA(PHONE_HOME), Contact::P_PHONE_HOME},
        {__EA(PHONE_OFFICE), Contact::P_PHONE_OFFICE},
        {__EA(PHONE_MOBILE), Contact::P_PHONE_MOBILE},
        {__EA(EMAILS), Contact::P_EMAILS},
        {__EA(HOMEPAGE), Contact::P_HOMEPAGE},
        {__EA(ABOUT), Contact::P_ABOUT},
        {__EA(AVATAR_IMAGE), Contact::P_AVATAR_IMAGE},
        {__EA(MOOD_TEXT), Contact::P_MOOD_TEXT},
        {__EA(RICH_MOOD_TEXT), Contact::P_RICH_MOOD_TEXT},
        {__EA(TIMEZONE), Contact::P_TIMEZONE},
        {__EA(CAPABILITIES), Contact::P_CAPABILITIES},
        {__EA(PROFILE_TIMESTAMP), Contact::P_PROFILE_TIMESTAMP},
        {__EA(NROF_AUTHED_BUDDIES), Contact::P_NROF_AUTHED_BUDDIES},
        {__EA(IPCOUNTRY), Contact::P_IPCOUNTRY},
        {__EA(AVATAR_TIMESTAMP), Contact::P_AVATAR_TIMESTAMP},
        {__EA(MOOD_TIMESTAMP), Contact::P_MOOD_TIMESTAMP},
        {__EA(RECEIVED_AUTHREQUEST), Contact::P_RECEIVED_AUTHREQUEST},
        {__EA(AUTHREQ_TIMESTAMP), Contact::P_AUTHREQ_TIMESTAMP},
        {__EA(LASTONLINE_TIMESTAMP), Contact::P_LASTONLINE_TIMESTAMP},
        {__EA(AVAILABILITY), Contact::P_AVAILABILITY},
        {__EA(DISPLAYNAME), Contact::P_DISPLAYNAME},
        {__EA(REFRESHING), Contact::P_REFRESHING},
        {__EA(GIVEN_AUTHLEVEL), Contact::P_GIVEN_AUTHLEVEL},
        {__EA(GIVEN_DISPLAYNAME), Contact::P_GIVEN_DISPLAYNAME},
        {__EA(ASSIGNED_COMMENT), Contact::P_ASSIGNED_COMMENT},
        {__EA(LASTUSED_TIMESTAMP), Contact::P_LASTUSED_TIMESTAMP},
        {__EA(AUTHREQUEST_COUNT), Contact::P_AUTHREQUEST_COUNT},
        {__EA(ASSIGNED_PHONE1), Contact::P_ASSIGNED_PHONE1},
        {__EA(ASSIGNED_PHONE1_LABEL), Contact::P_ASSIGNED_PHONE1_LABEL},
        {__EA(ASSIGNED_PHONE2), Contact::P_ASSIGNED_PHONE2},
        {__EA(ASSIGNED_PHONE2_LABEL), Contact::P_ASSIGNED_PHONE2_LABEL},
        {__EA(ASSIGNED_PHONE3), Contact::P_ASSIGNED_PHONE3},
        {__EA(ASSIGNED_PHONE3_LABEL), Contact::P_ASSIGNED_PHONE3_LABEL},
        {__EA(POPULARITY_ORD), Contact::P_POPULARITY_ORD},
        {0, 0}
};

extern const PMAP CONTACT_TYPE_MAP[] = {
        {__EA(UNRECOGNIZED), Contact::UNRECOGNIZED},
        {__EA(SKYPE), Contact::SKYPE},
        {__EA(PSTN), Contact::PSTN},
        {__EA(EMERGENCY_PSTN), Contact::EMERGENCY_PSTN},
        {__EA(FREE_PSTN), Contact::FREE_PSTN},
        {__EA(UNDISCLOSED_PSTN), Contact::UNDISCLOSED_PSTN},
        {__EA(EXTERNAL), Contact::EXTERNAL},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Contact::TYPE& v)
        {
                return enumtostring("Contact::TYPE",CONTACT_TYPE_MAP,v);
        }
}

extern const PMAP CONTACT_AUTHLEVEL_MAP[] = {
        {__EA(NONE), Contact::NONE},
        {__EA(AUTHORIZED_BY_ME), Contact::AUTHORIZED_BY_ME},
        {__EA(BLOCKED_BY_ME), Contact::BLOCKED_BY_ME},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Contact::AUTHLEVEL& v)
        {
                return enumtostring("Contact::AUTHLEVEL",CONTACT_AUTHLEVEL_MAP,v);
        }
}

extern const PMAP CONTACT_AVAILABILITY_MAP[] = {
        {__EA(UNKNOWN), Contact::UNKNOWN},
        {__EA(PENDINGAUTH), Contact::PENDINGAUTH},
        {__EA(BLOCKED), Contact::BLOCKED},
        {__EA(BLOCKED_SKYPEOUT), Contact::BLOCKED_SKYPEOUT},
        {__EA(SKYPEOUT), Contact::SKYPEOUT},
        {__EA(OFFLINE), Contact::OFFLINE},
        {__EA(OFFLINE_BUT_VM_ABLE), Contact::OFFLINE_BUT_VM_ABLE},
        {__EA(OFFLINE_BUT_CF_ABLE), Contact::OFFLINE_BUT_CF_ABLE},
        {__EA(ONLINE), Contact::ONLINE},
        {__EA(AWAY), Contact::AWAY},
        {__EA(NOT_AVAILABLE), Contact::NOT_AVAILABLE},
        {__EA(DO_NOT_DISTURB), Contact::DO_NOT_DISTURB},
        {__EA(SKYPE_ME), Contact::SKYPE_ME},
        {__EA(INVISIBLE), Contact::INVISIBLE},
        {__EA(CONNECTING), Contact::CONNECTING},
        {__EA(ONLINE_FROM_MOBILE), Contact::ONLINE_FROM_MOBILE},
        {__EA(AWAY_FROM_MOBILE), Contact::AWAY_FROM_MOBILE},
        {__EA(NOT_AVAILABLE_FROM_MOBILE), Contact::NOT_AVAILABLE_FROM_MOBILE},
        {__EA(DO_NOT_DISTURB_FROM_MOBILE), Contact::DO_NOT_DISTURB_FROM_MOBILE},
        {__EA(SKYPE_ME_FROM_MOBILE), Contact::SKYPE_ME_FROM_MOBILE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Contact::AVAILABILITY& v)
        {
                return enumtostring("Contact::AVAILABILITY",CONTACT_AVAILABILITY_MAP,v);
        }
}

extern const PMAP CONTACT_EXTRA_AUTHREQ_FIELDS_MAP[] = {
        {__EA(SEND_VERIFIED_EMAIL), Contact::SEND_VERIFIED_EMAIL},
        {__EA(SEND_VERIFIED_COMPANY), Contact::SEND_VERIFIED_COMPANY},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Contact::EXTRA_AUTHREQ_FIELDS& v)
        {
                return enumtostring("Contact::EXTRA_AUTHREQ_FIELDS",CONTACT_EXTRA_AUTHREQ_FIELDS_MAP,v);
        }
}

extern const PMAP CONTACT_CAPABILITY_MAP[] = {
        {__EA(CAPABILITY_VOICEMAIL), Contact::CAPABILITY_VOICEMAIL},
        {__EA(CAPABILITY_SKYPEOUT), Contact::CAPABILITY_SKYPEOUT},
        {__EA(CAPABILITY_SKYPEIN), Contact::CAPABILITY_SKYPEIN},
        {__EA(CAPABILITY_CAN_BE_SENT_VM), Contact::CAPABILITY_CAN_BE_SENT_VM},
        {__EA(CAPABILITY_CALL_FORWARD), Contact::CAPABILITY_CALL_FORWARD},
        {__EA(CAPABILITY_VIDEO), Contact::CAPABILITY_VIDEO},
        {__EA(CAPABILITY_TEXT), Contact::CAPABILITY_TEXT},
        {__EA(CAPABILITY_SERVICE_PROVIDER), Contact::CAPABILITY_SERVICE_PROVIDER},
        {__EA(CAPABILITY_LARGE_CONFERENCE), Contact::CAPABILITY_LARGE_CONFERENCE},
        {__EA(CAPABILITY_COMMERCIAL_CONTACT), Contact::CAPABILITY_COMMERCIAL_CONTACT},
        {__EA(CAPABILITY_PSTN_TRANSFER), Contact::CAPABILITY_PSTN_TRANSFER},
        {__EA(CAPABILITY_TEXT_EVER), Contact::CAPABILITY_TEXT_EVER},
        {__EA(CAPABILITY_VOICE_EVER), Contact::CAPABILITY_VOICE_EVER},
        {__EA(CAPABILITY_MOBILE_DEVICE), Contact::CAPABILITY_MOBILE_DEVICE},
        {__EA(CAPABILITY_PUBLIC_CONTACT), Contact::CAPABILITY_PUBLIC_CONTACT},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Contact::CAPABILITY& v)
        {
                return enumtostring("Contact::CAPABILITY",CONTACT_CAPABILITY_MAP,v);
        }
}

extern const PMAP CONTACT_CAPABILITYSTATUS_MAP[] = {
        {__EA(NO_CAPABILITY), Contact::NO_CAPABILITY},
        {__EA(CAPABILITY_MIXED), Contact::CAPABILITY_MIXED},
        {__EA(CAPABILITY_EXISTS), Contact::CAPABILITY_EXISTS},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Contact::CAPABILITYSTATUS& v)
        {
                return enumtostring("Contact::CAPABILITYSTATUS",CONTACT_CAPABILITYSTATUS_MAP,v);
        }
}

extern const EMAP CONTACT_EMAP[] = {
        {Contact::P_TYPE, CONTACT_TYPE_MAP},
        {Contact::P_AVAILABILITY, CONTACT_AVAILABILITY_MAP},
        {Contact::P_GIVEN_AUTHLEVEL, CONTACT_AUTHLEVEL_MAP},
        {0, 0}
};
const EMAP* Contact::getEMap() const 
{
        return CONTACT_EMAP;
}
const PMAP* Contact::getPMap() const 
{
        return CONTACT_PROPERTY_MAP;
}


/** class ContactSearch **********************/

ContactSearch::ContactSearch(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
ContactSearch::~ContactSearch() {  }
void ContactSearch::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgcontactsearchContactSearch*>(cache);  }
unsigned int ContactSearch::moduleID() const 
{
        return MODULE_ID;
}
String ContactSearch::getNamespace() const 
{
        return __EA(CONTACTSEARCH);
}

extern const PMAP CONTACTSEARCH_PROPERTY_MAP[] = {
        {__EA(CONTACT_SEARCH_STATUS), ContactSearch::P_CONTACT_SEARCH_STATUS},
        {0, 0}
};

extern const PMAP CONTACTSEARCH_STATUS_MAP[] = {
        {__EA(CONSTRUCTION), ContactSearch::CONSTRUCTION},
        {__EA(PENDING), ContactSearch::PENDING},
        {__EA(EXTENDABLE), ContactSearch::EXTENDABLE},
        {__EA(FINISHED), ContactSearch::FINISHED},
        {__EA(FAILED), ContactSearch::FAILED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const ContactSearch::STATUS& v)
        {
                return enumtostring("ContactSearch::STATUS",CONTACTSEARCH_STATUS_MAP,v);
        }
}

extern const PMAP CONTACTSEARCH_CONDITION_MAP[] = {
        {__EA(EQ), ContactSearch::EQ},
        {__EA(GT), ContactSearch::GT},
        {__EA(GE), ContactSearch::GE},
        {__EA(LT), ContactSearch::LT},
        {__EA(LE), ContactSearch::LE},
        {__EA(PREFIX_EQ), ContactSearch::PREFIX_EQ},
        {__EA(PREFIX_GE), ContactSearch::PREFIX_GE},
        {__EA(PREFIX_LE), ContactSearch::PREFIX_LE},
        {__EA(CONTAINS_WORDS), ContactSearch::CONTAINS_WORDS},
        {__EA(CONTAINS_WORD_PREFIXES), ContactSearch::CONTAINS_WORD_PREFIXES},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const ContactSearch::CONDITION& v)
        {
                return enumtostring("ContactSearch::CONDITION",CONTACTSEARCH_CONDITION_MAP,v);
        }
}

void ContactSearch::OnNewResult(
        __se_unused const ContactRef& /*contact*/,
        __se_unused const uint& /*rankValue*/
) {}

extern const EMAP CONTACTSEARCH_EMAP[] = {
        {ContactSearch::P_CONTACT_SEARCH_STATUS, CONTACTSEARCH_STATUS_MAP},
        {0, 0}
};
const EMAP* ContactSearch::getEMap() const 
{
        return CONTACTSEARCH_EMAP;
}
const PMAP* ContactSearch::getPMap() const 
{
        return CONTACTSEARCH_PROPERTY_MAP;
}

extern const PMAP TRANSFER_SENDFILE_ERROR_MAP[] = {
        {__EA(TRANSFER_OPEN_SUCCESS), TRANSFER_OPEN_SUCCESS},
        {__EA(TRANSFER_BAD_FILENAME), TRANSFER_BAD_FILENAME},
        {__EA(TRANSFER_OPEN_FAILED), TRANSFER_OPEN_FAILED},
        {__EA(TRANSFER_TOO_MANY_PARALLEL), TRANSFER_TOO_MANY_PARALLEL},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const TRANSFER_SENDFILE_ERROR& v)
        {
                return enumtostring("TRANSFER_SENDFILE_ERROR",TRANSFER_SENDFILE_ERROR_MAP,v);
        }
}

extern const PMAP LEAVE_REASON_MAP[] = {
        {__EA(LEAVE_REASON_NONE), LEAVE_REASON_NONE},
        {__EA(RETIRED_USER_INCAPABLE), RETIRED_USER_INCAPABLE},
        {__EA(RETIRED_ADDER_MUST_BE_FRIEND), RETIRED_ADDER_MUST_BE_FRIEND},
        {__EA(RETIRED_ADDER_MUST_BE_AUTHORIZED), RETIRED_ADDER_MUST_BE_AUTHORIZED},
        {__EA(RETIRED_DECLINE_ADD), RETIRED_DECLINE_ADD},
        {__EA(RETIRED_UNSUBSCRIBE), RETIRED_UNSUBSCRIBE},
        {__EA(LIVE_NO_ANSWER), LIVE_NO_ANSWER},
        {__EA(LIVE_MANUAL), LIVE_MANUAL},
        {__EA(LIVE_BUSY), LIVE_BUSY},
        {__EA(LIVE_CONNECTION_DROPPED), LIVE_CONNECTION_DROPPED},
        {__EA(LIVE_NO_SKYPEOUT_SUBSCRIPTION), LIVE_NO_SKYPEOUT_SUBSCRIPTION},
        {__EA(LIVE_INSUFFICIENT_FUNDS), LIVE_INSUFFICIENT_FUNDS},
        {__EA(LIVE_INTERNET_CONNECTION_LOST), LIVE_INTERNET_CONNECTION_LOST},
        {__EA(LIVE_SKYPEOUT_ACCOUNT_BLOCKED), LIVE_SKYPEOUT_ACCOUNT_BLOCKED},
        {__EA(LIVE_PSTN_COULD_NOT_CONNECT_TO_SKYPE_PROXY), LIVE_PSTN_COULD_NOT_CONNECT_TO_SKYPE_PROXY},
        {__EA(LIVE_PSTN_INVALID_NUMBER), LIVE_PSTN_INVALID_NUMBER},
        {__EA(LIVE_PSTN_NUMBER_FORBIDDEN), LIVE_PSTN_NUMBER_FORBIDDEN},
        {__EA(LIVE_PSTN_CALL_TIMED_OUT), LIVE_PSTN_CALL_TIMED_OUT},
        {__EA(LIVE_PSTN_BUSY), LIVE_PSTN_BUSY},
        {__EA(LIVE_PSTN_CALL_TERMINATED), LIVE_PSTN_CALL_TERMINATED},
        {__EA(LIVE_PSTN_NETWORK_ERROR), LIVE_PSTN_NETWORK_ERROR},
        {__EA(LIVE_NUMBER_UNAVAILABLE), LIVE_NUMBER_UNAVAILABLE},
        {__EA(LIVE_PSTN_CALL_REJECTED), LIVE_PSTN_CALL_REJECTED},
        {__EA(LIVE_PSTN_MISC_ERROR), LIVE_PSTN_MISC_ERROR},
        {__EA(LIVE_INTERNAL_ERROR), LIVE_INTERNAL_ERROR},
        {__EA(LIVE_UNABLE_TO_CONNECT), LIVE_UNABLE_TO_CONNECT},
        {__EA(LIVE_RECORDING_FAILED), LIVE_RECORDING_FAILED},
        {__EA(LIVE_PLAYBACK_ERROR), LIVE_PLAYBACK_ERROR},
        {__EA(LIVE_LEGACY_ERROR), LIVE_LEGACY_ERROR},
        {__EA(LIVE_BLOCKED_BY_PRIVACY_SETTINGS), LIVE_BLOCKED_BY_PRIVACY_SETTINGS},
        {__EA(LIVE_ERROR), LIVE_ERROR},
        {__EA(LIVE_TRANSFER_FAILED), LIVE_TRANSFER_FAILED},
        {__EA(LIVE_TRANSFER_INSUFFICIENT_FUNDS), LIVE_TRANSFER_INSUFFICIENT_FUNDS},
        {__EA(LIVE_BLOCKED_BY_US), LIVE_BLOCKED_BY_US},
        {__EA(LIVE_EMERGENCY_CALL_DENIED), LIVE_EMERGENCY_CALL_DENIED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const LEAVE_REASON& v)
        {
                return enumtostring("LEAVE_REASON",LEAVE_REASON_MAP,v);
        }
}


/** class Participant **********************/

Participant::Participant(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
Participant::~Participant() {  }
void Participant::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgconversationParticipant*>(cache);  }
unsigned int Participant::moduleID() const 
{
        return MODULE_ID;
}
String Participant::getNamespace() const 
{
        return __EA(PARTICIPANT);
}

extern const PMAP PARTICIPANT_PROPERTY_MAP[] = {
        {__EA(CONVO_ID), Participant::P_CONVO_ID},
        {__EA(IDENTITY), Participant::P_IDENTITY},
        {__EA(RANK), Participant::P_RANK},
        {__EA(REQUESTED_RANK), Participant::P_REQUESTED_RANK},
        {__EA(TEXT_STATUS), Participant::P_TEXT_STATUS},
        {__EA(VOICE_STATUS), Participant::P_VOICE_STATUS},
        {__EA(VIDEO_STATUS), Participant::P_VIDEO_STATUS},
        {__EA(LIVE_IDENTITY), Participant::P_LIVE_IDENTITY},
        {__EA(LIVE_PRICE_FOR_ME), Participant::P_LIVE_PRICE_FOR_ME},
        {__EA(LIVE_FWD_IDENTITIES), Participant::P_LIVE_FWD_IDENTITIES},
        {__EA(LIVE_START_TIMESTAMP), Participant::P_LIVE_START_TIMESTAMP},
        {__EA(SOUND_LEVEL), Participant::P_SOUND_LEVEL},
        {__EA(DEBUGINFO), Participant::P_DEBUGINFO},
        {__EA(LAST_VOICE_ERROR), Participant::P_LAST_VOICE_ERROR},
        {__EA(QUALITY_PROBLEMS), Participant::P_QUALITY_PROBLEMS},
        {__EA(LIVE_TYPE), Participant::P_LIVE_TYPE},
        {__EA(LIVE_COUNTRY), Participant::P_LIVE_COUNTRY},
        {__EA(TRANSFERRED_BY), Participant::P_TRANSFERRED_BY},
        {__EA(TRANSFERRED_TO), Participant::P_TRANSFERRED_TO},
        {__EA(ADDER), Participant::P_ADDER},
        {__EA(LAST_LEAVEREASON), Participant::P_LAST_LEAVEREASON},
        {0, 0}
};

extern const PMAP PARTICIPANT_RANK_MAP[] = {
        {__EA(CREATOR), Participant::CREATOR},
        {__EA(ADMIN), Participant::ADMIN},
        {__EA(SPEAKER), Participant::SPEAKER},
        {__EA(WRITER), Participant::WRITER},
        {__EA(SPECTATOR), Participant::SPECTATOR},
        {__EA(APPLICANT), Participant::APPLICANT},
        {__EA(RETIRED), Participant::RETIRED},
        {__EA(OUTLAW), Participant::OUTLAW},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Participant::RANK& v)
        {
                return enumtostring("Participant::RANK",PARTICIPANT_RANK_MAP,v);
        }
}

extern const PMAP PARTICIPANT_TEXT_STATUS_MAP[] = {
        {__EA(TEXT_UNKNOWN), Participant::TEXT_UNKNOWN},
        {__EA(TEXT_NA), Participant::TEXT_NA},
        {__EA(READING), Participant::READING},
        {__EA(WRITING), Participant::WRITING},
        {__EA(WRITING_AS_ANGRY), Participant::WRITING_AS_ANGRY},
        {__EA(WRITING_AS_CAT), Participant::WRITING_AS_CAT},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Participant::TEXT_STATUS& v)
        {
                return enumtostring("Participant::TEXT_STATUS",PARTICIPANT_TEXT_STATUS_MAP,v);
        }
}

extern const PMAP PARTICIPANT_VOICE_STATUS_MAP[] = {
        {__EA(VOICE_UNKNOWN), Participant::VOICE_UNKNOWN},
        {__EA(VOICE_NA), Participant::VOICE_NA},
        {__EA(VOICE_AVAILABLE), Participant::VOICE_AVAILABLE},
        {__EA(VOICE_CONNECTING), Participant::VOICE_CONNECTING},
        {__EA(RINGING), Participant::RINGING},
        {__EA(EARLY_MEDIA), Participant::EARLY_MEDIA},
        {__EA(LISTENING), Participant::LISTENING},
        {__EA(SPEAKING), Participant::SPEAKING},
        {__EA(VOICE_ON_HOLD), Participant::VOICE_ON_HOLD},
        {__EA(VOICE_STOPPED), Participant::VOICE_STOPPED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Participant::VOICE_STATUS& v)
        {
                return enumtostring("Participant::VOICE_STATUS",PARTICIPANT_VOICE_STATUS_MAP,v);
        }
}

extern const PMAP PARTICIPANT_VIDEO_STATUS_MAP[] = {
        {__EA(VIDEO_UNKNOWN), Participant::VIDEO_UNKNOWN},
        {__EA(VIDEO_NA), Participant::VIDEO_NA},
        {__EA(VIDEO_AVAILABLE), Participant::VIDEO_AVAILABLE},
        {__EA(VIDEO_CONNECTING), Participant::VIDEO_CONNECTING},
        {__EA(STREAMING), Participant::STREAMING},
        {__EA(VIDEO_ON_HOLD), Participant::VIDEO_ON_HOLD},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Participant::VIDEO_STATUS& v)
        {
                return enumtostring("Participant::VIDEO_STATUS",PARTICIPANT_VIDEO_STATUS_MAP,v);
        }
}

extern const PMAP PARTICIPANT_DTMF_MAP[] = {
        {__EA(DTMF_0), Participant::DTMF_0},
        {__EA(DTMF_1), Participant::DTMF_1},
        {__EA(DTMF_2), Participant::DTMF_2},
        {__EA(DTMF_3), Participant::DTMF_3},
        {__EA(DTMF_4), Participant::DTMF_4},
        {__EA(DTMF_5), Participant::DTMF_5},
        {__EA(DTMF_6), Participant::DTMF_6},
        {__EA(DTMF_7), Participant::DTMF_7},
        {__EA(DTMF_8), Participant::DTMF_8},
        {__EA(DTMF_9), Participant::DTMF_9},
        {__EA(DTMF_STAR), Participant::DTMF_STAR},
        {__EA(DTMF_POUND), Participant::DTMF_POUND},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Participant::DTMF& v)
        {
                return enumtostring("Participant::DTMF",PARTICIPANT_DTMF_MAP,v);
        }
}

void Participant::OnIncomingDTMF(
        __se_unused const DTMF& /*dtmf*/
) {}

extern const EMAP PARTICIPANT_EMAP[] = {
        {Participant::P_RANK, PARTICIPANT_RANK_MAP},
        {Participant::P_REQUESTED_RANK, PARTICIPANT_RANK_MAP},
        {Participant::P_TEXT_STATUS, PARTICIPANT_TEXT_STATUS_MAP},
        {Participant::P_VOICE_STATUS, PARTICIPANT_VOICE_STATUS_MAP},
        {Participant::P_VIDEO_STATUS, PARTICIPANT_VIDEO_STATUS_MAP},
        {0, 0}
};
const EMAP* Participant::getEMap() const 
{
        return PARTICIPANT_EMAP;
}
const PMAP* Participant::getPMap() const 
{
        return PARTICIPANT_PROPERTY_MAP;
}


/** class Conversation **********************/

Conversation::Conversation(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
Conversation::~Conversation() {  }
void Conversation::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgconversationConversation*>(cache);  }
unsigned int Conversation::moduleID() const 
{
        return MODULE_ID;
}
String Conversation::getNamespace() const 
{
        return __EA(CONVERSATION);
}

extern const PMAP CONVERSATION_PROPERTY_MAP[] = {
        {__EA(IDENTITY), Conversation::P_IDENTITY},
        {__EA(TYPE), Conversation::P_TYPE},
        {__EA(LIVE_HOST), Conversation::P_LIVE_HOST},
        {__EA(LIVE_START_TIMESTAMP), Conversation::P_LIVE_START_TIMESTAMP},
        {__EA(LIVE_IS_MUTED), Conversation::P_LIVE_IS_MUTED},
        {__EA(ALERT_STRING), Conversation::P_ALERT_STRING},
        {__EA(IS_BOOKMARKED), Conversation::P_IS_BOOKMARKED},
        {__EA(GIVEN_DISPLAYNAME), Conversation::P_GIVEN_DISPLAYNAME},
        {__EA(DISPLAYNAME), Conversation::P_DISPLAYNAME},
        {__EA(LOCAL_LIVESTATUS), Conversation::P_LOCAL_LIVESTATUS},
        {__EA(INBOX_TIMESTAMP), Conversation::P_INBOX_TIMESTAMP},
        {__EA(INBOX_MESSAGE_ID), Conversation::P_INBOX_MESSAGE_ID},
        {__EA(UNCONSUMED_SUPPRESSED_MESSAGES), Conversation::P_UNCONSUMED_SUPPRESSED_MESSAGES},
        {__EA(UNCONSUMED_NORMAL_MESSAGES), Conversation::P_UNCONSUMED_NORMAL_MESSAGES},
        {__EA(UNCONSUMED_ELEVATED_MESSAGES), Conversation::P_UNCONSUMED_ELEVATED_MESSAGES},
        {__EA(UNCONSUMED_MESSAGES_VOICE), Conversation::P_UNCONSUMED_MESSAGES_VOICE},
        {__EA(ACTIVE_VM_ID), Conversation::P_ACTIVE_VM_ID},
        {__EA(CONSUMPTION_HORIZON), Conversation::P_CONSUMPTION_HORIZON},
        {__EA(LAST_ACTIVITY_TIMESTAMP), Conversation::P_LAST_ACTIVITY_TIMESTAMP},
        {__EA(SPAWNED_FROM_CONVO_ID), Conversation::P_SPAWNED_FROM_CONVO_ID},
        {__EA(CREATOR), Conversation::P_CREATOR},
        {__EA(CREATION_TIMESTAMP), Conversation::P_CREATION_TIMESTAMP},
        {__EA(MY_STATUS), Conversation::P_MY_STATUS},
        {__EA(OPT_JOINING_ENABLED), Conversation::P_OPT_JOINING_ENABLED},
        {__EA(OPT_ENTRY_LEVEL_RANK), Conversation::P_OPT_ENTRY_LEVEL_RANK},
        {__EA(OPT_DISCLOSE_HISTORY), Conversation::P_OPT_DISCLOSE_HISTORY},
        {__EA(OPT_ADMIN_ONLY_ACTIVITIES), Conversation::P_OPT_ADMIN_ONLY_ACTIVITIES},
        {__EA(PASSWORDHINT), Conversation::P_PASSWORDHINT},
        {__EA(META_NAME), Conversation::P_META_NAME},
        {__EA(META_TOPIC), Conversation::P_META_TOPIC},
        {__EA(META_GUIDELINES), Conversation::P_META_GUIDELINES},
        {__EA(META_PICTURE), Conversation::P_META_PICTURE},
        {0, 0}
};

extern const PMAP CONVERSATION_TYPE_MAP[] = {
        {__EA(DIALOG), Conversation::DIALOG},
        {__EA(CONFERENCE), Conversation::CONFERENCE},
        {__EA(TERMINATED_CONFERENCE), Conversation::TERMINATED_CONFERENCE},
        {__EA(LEGACY_VOICE_CONFERENCE), Conversation::LEGACY_VOICE_CONFERENCE},
        {__EA(LEGACY_SHAREDGROUP), Conversation::LEGACY_SHAREDGROUP},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Conversation::TYPE& v)
        {
                return enumtostring("Conversation::TYPE",CONVERSATION_TYPE_MAP,v);
        }
}

extern const PMAP CONVERSATION_MY_STATUS_MAP[] = {
        {__EA(CONNECTING), Conversation::CONNECTING},
        {__EA(RETRY_CONNECTING), Conversation::RETRY_CONNECTING},
        {__EA(DOWNLOADING_MESSAGES), Conversation::DOWNLOADING_MESSAGES},
        {__EA(QUEUED_TO_ENTER), Conversation::QUEUED_TO_ENTER},
        {__EA(APPLICANT), Conversation::APPLICANT},
        {__EA(APPLICATION_DENIED), Conversation::APPLICATION_DENIED},
        {__EA(INVALID_ACCESS_TOKEN), Conversation::INVALID_ACCESS_TOKEN},
        {__EA(CONSUMER), Conversation::CONSUMER},
        {__EA(RETIRED_FORCEFULLY), Conversation::RETIRED_FORCEFULLY},
        {__EA(RETIRED_VOLUNTARILY), Conversation::RETIRED_VOLUNTARILY},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Conversation::MY_STATUS& v)
        {
                return enumtostring("Conversation::MY_STATUS",CONVERSATION_MY_STATUS_MAP,v);
        }
}

extern const PMAP CONVERSATION_LOCAL_LIVESTATUS_MAP[] = {
        {__EA(NONE), Conversation::NONE},
        {__EA(STARTING), Conversation::STARTING},
        {__EA(RINGING_FOR_ME), Conversation::RINGING_FOR_ME},
        {__EA(IM_LIVE), Conversation::IM_LIVE},
        {__EA(ON_HOLD_LOCALLY), Conversation::ON_HOLD_LOCALLY},
        {__EA(ON_HOLD_REMOTELY), Conversation::ON_HOLD_REMOTELY},
        {__EA(OTHERS_ARE_LIVE), Conversation::OTHERS_ARE_LIVE},
        {__EA(OTHERS_ARE_LIVE_FULL), Conversation::OTHERS_ARE_LIVE_FULL},
        {__EA(PLAYING_VOICE_MESSAGE), Conversation::PLAYING_VOICE_MESSAGE},
        {__EA(RECORDING_VOICE_MESSAGE), Conversation::RECORDING_VOICE_MESSAGE},
        {__EA(RECENTLY_LIVE), Conversation::RECENTLY_LIVE},
        {__EA(TRANSFERRING), Conversation::TRANSFERRING},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Conversation::LOCAL_LIVESTATUS& v)
        {
                return enumtostring("Conversation::LOCAL_LIVESTATUS",CONVERSATION_LOCAL_LIVESTATUS_MAP,v);
        }
}

extern const PMAP CONVERSATION_ALLOWED_ACTIVITY_MAP[] = {
        {__EA(SET_META), Conversation::SET_META},
        {__EA(ADD_CONSUMERS), Conversation::ADD_CONSUMERS},
        {__EA(SPEAK), Conversation::SPEAK},
        {__EA(SPEAK_AND_WRITE), Conversation::SPEAK_AND_WRITE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Conversation::ALLOWED_ACTIVITY& v)
        {
                return enumtostring("Conversation::ALLOWED_ACTIVITY",CONVERSATION_ALLOWED_ACTIVITY_MAP,v);
        }
}

extern const PMAP CONVERSATION_PARTICIPANTFILTER_MAP[] = {
        {__EA(ALL), Conversation::ALL},
        {__EA(CONSUMERS), Conversation::CONSUMERS},
        {__EA(APPLICANTS), Conversation::APPLICANTS},
        {__EA(CONSUMERS_AND_APPLICANTS), Conversation::CONSUMERS_AND_APPLICANTS},
        {__EA(MYSELF), Conversation::MYSELF},
        {__EA(OTHER_CONSUMERS), Conversation::OTHER_CONSUMERS},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Conversation::PARTICIPANTFILTER& v)
        {
                return enumtostring("Conversation::PARTICIPANTFILTER",CONVERSATION_PARTICIPANTFILTER_MAP,v);
        }
}

extern const PMAP CONVERSATION_LIST_TYPE_MAP[] = {
        {__EA(ALL_CONVERSATIONS), Conversation::ALL_CONVERSATIONS},
        {__EA(INBOX_CONVERSATIONS), Conversation::INBOX_CONVERSATIONS},
        {__EA(BOOKMARKED_CONVERSATIONS), Conversation::BOOKMARKED_CONVERSATIONS},
        {__EA(LIVE_CONVERSATIONS), Conversation::LIVE_CONVERSATIONS},
        {__EA(REALLY_ALL_CONVERSATIONS), Conversation::REALLY_ALL_CONVERSATIONS},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Conversation::LIST_TYPE& v)
        {
                return enumtostring("Conversation::LIST_TYPE",CONVERSATION_LIST_TYPE_MAP,v);
        }
}

void Conversation::OnParticipantListChange(
) {}

void Conversation::OnMessage(
        __se_unused const MessageRef& /*message*/
) {}

void Conversation::OnSpawnConference(
        __se_unused const ConversationRef& /*spawned*/
) {}

extern const EMAP CONVERSATION_EMAP[] = {
        {Conversation::P_TYPE, CONVERSATION_TYPE_MAP},
        {Conversation::P_LOCAL_LIVESTATUS, CONVERSATION_LOCAL_LIVESTATUS_MAP},
        {Conversation::P_MY_STATUS, CONVERSATION_MY_STATUS_MAP},
        {Conversation::P_OPT_ENTRY_LEVEL_RANK, PARTICIPANT_RANK_MAP},
        {Conversation::P_OPT_ADMIN_ONLY_ACTIVITIES, CONVERSATION_ALLOWED_ACTIVITY_MAP},
        {0, 0}
};
const EMAP* Conversation::getEMap() const 
{
        return CONVERSATION_EMAP;
}
const PMAP* Conversation::getPMap() const 
{
        return CONVERSATION_PROPERTY_MAP;
}


/** class Message **********************/

Message::Message(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
Message::~Message() {  }
void Message::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgconversationMessage*>(cache);  }
unsigned int Message::moduleID() const 
{
        return MODULE_ID;
}
String Message::getNamespace() const 
{
        return __EA(MESSAGE);
}

extern const PMAP MESSAGE_PROPERTY_MAP[] = {
        {__EA(CONVO_ID), Message::P_CONVO_ID},
        {__EA(CONVO_GUID), Message::P_CONVO_GUID},
        {__EA(AUTHOR), Message::P_AUTHOR},
        {__EA(AUTHOR_DISPLAYNAME), Message::P_AUTHOR_DISPLAYNAME},
        {__EA(GUID), Message::P_GUID},
        {__EA(ORIGINALLY_MEANT_FOR), Message::P_ORIGINALLY_MEANT_FOR},
        {__EA(TIMESTAMP), Message::P_TIMESTAMP},
        {__EA(TYPE), Message::P_TYPE},
        {__EA(SENDING_STATUS), Message::P_SENDING_STATUS},
        {__EA(CONSUMPTION_STATUS), Message::P_CONSUMPTION_STATUS},
        {__EA(EDITED_BY), Message::P_EDITED_BY},
        {__EA(EDIT_TIMESTAMP), Message::P_EDIT_TIMESTAMP},
        {__EA(PARAM_KEY), Message::P_PARAM_KEY},
        {__EA(PARAM_VALUE), Message::P_PARAM_VALUE},
        {__EA(BODY_XML), Message::P_BODY_XML},
        {__EA(IDENTITIES), Message::P_IDENTITIES},
        {__EA(REASON), Message::P_REASON},
        {__EA(LEAVEREASON), Message::P_LEAVEREASON},
        {__EA(PARTICIPANT_COUNT), Message::P_PARTICIPANT_COUNT},
        {0, 0}
};

extern const PMAP MESSAGE_TYPE_MAP[] = {
        {__EA(SET_METADATA), Message::SET_METADATA},
        {__EA(SPAWNED_CONFERENCE), Message::SPAWNED_CONFERENCE},
        {__EA(ADDED_CONSUMERS), Message::ADDED_CONSUMERS},
        {__EA(ADDED_APPLICANTS), Message::ADDED_APPLICANTS},
        {__EA(RETIRED_OTHERS), Message::RETIRED_OTHERS},
        {__EA(RETIRED), Message::RETIRED},
        {__EA(SET_RANK), Message::SET_RANK},
        {__EA(STARTED_LIVESESSION), Message::STARTED_LIVESESSION},
        {__EA(ENDED_LIVESESSION), Message::ENDED_LIVESESSION},
        {__EA(REQUESTED_AUTH), Message::REQUESTED_AUTH},
        {__EA(GRANTED_AUTH), Message::GRANTED_AUTH},
        {__EA(BLOCKED), Message::BLOCKED},
        {__EA(POSTED_TEXT), Message::POSTED_TEXT},
        {__EA(POSTED_EMOTE), Message::POSTED_EMOTE},
        {__EA(POSTED_CONTACTS), Message::POSTED_CONTACTS},
        {__EA(POSTED_SMS), Message::POSTED_SMS},
        {__EA(POSTED_ALERT), Message::POSTED_ALERT},
        {__EA(POSTED_VOICE_MESSAGE), Message::POSTED_VOICE_MESSAGE},
        {__EA(POSTED_FILES), Message::POSTED_FILES},
        {__EA(POSTED_INVOICE), Message::POSTED_INVOICE},
        {__EA(HAS_BIRTHDAY), Message::HAS_BIRTHDAY},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Message::TYPE& v)
        {
                return enumtostring("Message::TYPE",MESSAGE_TYPE_MAP,v);
        }
}

extern const PMAP MESSAGE_SENDING_STATUS_MAP[] = {
        {__EA(SENDING), Message::SENDING},
        {__EA(SENT), Message::SENT},
        {__EA(FAILED_TO_SEND), Message::FAILED_TO_SEND},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Message::SENDING_STATUS& v)
        {
                return enumtostring("Message::SENDING_STATUS",MESSAGE_SENDING_STATUS_MAP,v);
        }
}

extern const PMAP MESSAGE_CONSUMPTION_STATUS_MAP[] = {
        {__EA(CONSUMED), Message::CONSUMED},
        {__EA(UNCONSUMED_SUPPRESSED), Message::UNCONSUMED_SUPPRESSED},
        {__EA(UNCONSUMED_NORMAL), Message::UNCONSUMED_NORMAL},
        {__EA(UNCONSUMED_ELEVATED), Message::UNCONSUMED_ELEVATED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Message::CONSUMPTION_STATUS& v)
        {
                return enumtostring("Message::CONSUMPTION_STATUS",MESSAGE_CONSUMPTION_STATUS_MAP,v);
        }
}

extern const PMAP MESSAGE_SET_METADATA_KEY_MAP[] = {
        {__EA(SET_META_NAME), Message::SET_META_NAME},
        {__EA(SET_META_TOPIC), Message::SET_META_TOPIC},
        {__EA(SET_META_GUIDELINES), Message::SET_META_GUIDELINES},
        {__EA(SET_META_PICTURE), Message::SET_META_PICTURE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Message::SET_METADATA_KEY& v)
        {
                return enumtostring("Message::SET_METADATA_KEY",MESSAGE_SET_METADATA_KEY_MAP,v);
        }
}

extern const PMAP MESSAGE_LEAVEREASON_MAP[] = {
        {__EA(USER_INCAPABLE), Message::USER_INCAPABLE},
        {__EA(ADDER_MUST_BE_FRIEND), Message::ADDER_MUST_BE_FRIEND},
        {__EA(ADDER_MUST_BE_AUTHORIZED), Message::ADDER_MUST_BE_AUTHORIZED},
        {__EA(DECLINE_ADD), Message::DECLINE_ADD},
        {__EA(UNSUBSCRIBE), Message::UNSUBSCRIBE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Message::LEAVEREASON& v)
        {
                return enumtostring("Message::LEAVEREASON",MESSAGE_LEAVEREASON_MAP,v);
        }
}

extern const EMAP MESSAGE_EMAP[] = {
        {Message::P_TYPE, MESSAGE_TYPE_MAP},
        {Message::P_SENDING_STATUS, MESSAGE_SENDING_STATUS_MAP},
        {Message::P_CONSUMPTION_STATUS, MESSAGE_CONSUMPTION_STATUS_MAP},
        {0, 0}
};
const EMAP* Message::getEMap() const 
{
        return MESSAGE_EMAP;
}
const PMAP* Message::getPMap() const 
{
        return MESSAGE_PROPERTY_MAP;
}


/** class Video **********************/

Video::Video(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
Video::~Video() {  }
void Video::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgvideoVideo*>(cache);  }
unsigned int Video::moduleID() const 
{
        return MODULE_ID;
}
String Video::getNamespace() const 
{
        return __EA(VIDEO);
}

extern const PMAP VIDEO_PROPERTY_MAP[] = {
        {__EA(STATUS), Video::P_STATUS},
        {__EA(ERROR), Video::P_ERROR},
        {__EA(DEBUGINFO), Video::P_DEBUGINFO},
        {__EA(DIMENSIONS), Video::P_DIMENSIONS},
        {__EA(MEDIA_TYPE), Video::P_MEDIA_TYPE},
        {__EA(CONVO_ID), Video::P_CONVO_ID},
        {__EA(DEVICE_PATH), Video::P_DEVICE_PATH},
        {0, 0}
};

extern const PMAP VIDEO_STATUS_MAP[] = {
        {__EA(NOT_AVAILABLE), Video::NOT_AVAILABLE},
        {__EA(AVAILABLE), Video::AVAILABLE},
        {__EA(STARTING), Video::STARTING},
        {__EA(REJECTED), Video::REJECTED},
        {__EA(RUNNING), Video::RUNNING},
        {__EA(STOPPING), Video::STOPPING},
        {__EA(PAUSED), Video::PAUSED},
        {__EA(NOT_STARTED), Video::NOT_STARTED},
        {__EA(HINT_IS_VIDEOCALL_RECEIVED), Video::HINT_IS_VIDEOCALL_RECEIVED},
        {__EA(UNKNOWN), Video::UNKNOWN},
        {__EA(RENDERING), Video::RENDERING},
        {__EA(CHECKING_SUBSCRIPTION), Video::CHECKING_SUBSCRIPTION},
        {__EA(SWITCHING_DEVICE), Video::SWITCHING_DEVICE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Video::STATUS& v)
        {
                return enumtostring("Video::STATUS",VIDEO_STATUS_MAP,v);
        }
}

extern const PMAP VIDEO_MEDIATYPE_MAP[] = {
        {__EA(MEDIA_SCREENSHARING), Video::MEDIA_SCREENSHARING},
        {__EA(MEDIA_VIDEO), Video::MEDIA_VIDEO},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Video::MEDIATYPE& v)
        {
                return enumtostring("Video::MEDIATYPE",VIDEO_MEDIATYPE_MAP,v);
        }
}

extern const PMAP VIDEO_VIDEO_DEVICE_CAPABILITY_MAP[] = {
        {__EA(VIDEOCAP_HQ_CAPABLE), Video::VIDEOCAP_HQ_CAPABLE},
        {__EA(VIDEOCAP_HQ_CERTIFIED), Video::VIDEOCAP_HQ_CERTIFIED},
        {__EA(VIDEOCAP_REQ_DRIVERUPDATE), Video::VIDEOCAP_REQ_DRIVERUPDATE},
        {__EA(VIDEOCAP_USB_HIGHSPEED), Video::VIDEOCAP_USB_HIGHSPEED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Video::VIDEO_DEVICE_CAPABILITY& v)
        {
                return enumtostring("Video::VIDEO_DEVICE_CAPABILITY",VIDEO_VIDEO_DEVICE_CAPABILITY_MAP,v);
        }
}

void Video::OnCaptureRequestCompleted(
        __se_unused const uint& /*requestId*/,
        __se_unused const bool& /*isSuccessful*/,
        __se_unused const Sid::Binary& /*image*/,
        __se_unused const int& /*width*/,
        __se_unused const int& /*height*/
) {}

extern const EMAP VIDEO_EMAP[] = {
        {Video::P_STATUS, VIDEO_STATUS_MAP},
        {Video::P_MEDIA_TYPE, VIDEO_MEDIATYPE_MAP},
        {0, 0}
};
const EMAP* Video::getEMap() const 
{
        return VIDEO_EMAP;
}
const PMAP* Video::getPMap() const 
{
        return VIDEO_PROPERTY_MAP;
}


/** class Voicemail **********************/

Voicemail::Voicemail(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
Voicemail::~Voicemail() {  }
void Voicemail::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgvmVoicemail*>(cache);  }
unsigned int Voicemail::moduleID() const 
{
        return MODULE_ID;
}
String Voicemail::getNamespace() const 
{
        return __EA(VOICEMAIL);
}

extern const PMAP VOICEMAIL_PROPERTY_MAP[] = {
        {__EA(TYPE), Voicemail::P_TYPE},
        {__EA(PARTNER_HANDLE), Voicemail::P_PARTNER_HANDLE},
        {__EA(PARTNER_DISPNAME), Voicemail::P_PARTNER_DISPNAME},
        {__EA(STATUS), Voicemail::P_STATUS},
        {__EA(FAILUREREASON), Voicemail::P_FAILUREREASON},
        {__EA(SUBJECT), Voicemail::P_SUBJECT},
        {__EA(TIMESTAMP), Voicemail::P_TIMESTAMP},
        {__EA(DURATION), Voicemail::P_DURATION},
        {__EA(ALLOWED_DURATION), Voicemail::P_ALLOWED_DURATION},
        {__EA(PLAYBACK_PROGRESS), Voicemail::P_PLAYBACK_PROGRESS},
        {__EA(CONVO_ID), Voicemail::P_CONVO_ID},
        {__EA(CHATMSG_GUID), Voicemail::P_CHATMSG_GUID},
        {0, 0}
};

extern const PMAP VOICEMAIL_TYPE_MAP[] = {
        {__EA(INCOMING), Voicemail::INCOMING},
        {__EA(DEFAULT_GREETING), Voicemail::DEFAULT_GREETING},
        {__EA(CUSTOM_GREETING), Voicemail::CUSTOM_GREETING},
        {__EA(OUTGOING), Voicemail::OUTGOING},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Voicemail::TYPE& v)
        {
                return enumtostring("Voicemail::TYPE",VOICEMAIL_TYPE_MAP,v);
        }
}

extern const PMAP VOICEMAIL_STATUS_MAP[] = {
        {__EA(NOTDOWNLOADED), Voicemail::NOTDOWNLOADED},
        {__EA(DOWNLOADING), Voicemail::DOWNLOADING},
        {__EA(UNPLAYED), Voicemail::UNPLAYED},
        {__EA(BUFFERING), Voicemail::BUFFERING},
        {__EA(PLAYING), Voicemail::PLAYING},
        {__EA(PLAYED), Voicemail::PLAYED},
        {__EA(BLANK), Voicemail::BLANK},
        {__EA(RECORDING), Voicemail::RECORDING},
        {__EA(RECORDED), Voicemail::RECORDED},
        {__EA(UPLOADING), Voicemail::UPLOADING},
        {__EA(UPLOADED), Voicemail::UPLOADED},
        {__EA(DELETING), Voicemail::DELETING},
        {__EA(FAILED), Voicemail::FAILED},
        {__EA(DELETING_FAILED), Voicemail::DELETING_FAILED},
        {__EA(CHECKING), Voicemail::CHECKING},
        {__EA(CANCELLED), Voicemail::CANCELLED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Voicemail::STATUS& v)
        {
                return enumtostring("Voicemail::STATUS",VOICEMAIL_STATUS_MAP,v);
        }
}

extern const PMAP VOICEMAIL_FAILUREREASON_MAP[] = {
        {__EA(MISC_ERROR), Voicemail::MISC_ERROR},
        {__EA(CONNECT_ERROR), Voicemail::CONNECT_ERROR},
        {__EA(NO_VOICEMAIL_CAPABILITY), Voicemail::NO_VOICEMAIL_CAPABILITY},
        {__EA(NO_SUCH_VOICEMAIL), Voicemail::NO_SUCH_VOICEMAIL},
        {__EA(FILE_READ_ERROR), Voicemail::FILE_READ_ERROR},
        {__EA(FILE_WRITE_ERROR), Voicemail::FILE_WRITE_ERROR},
        {__EA(RECORDING_ERROR), Voicemail::RECORDING_ERROR},
        {__EA(PLAYBACK_ERROR), Voicemail::PLAYBACK_ERROR},
        {__EA(NO_PERMISSION), Voicemail::NO_PERMISSION},
        {__EA(RECEIVER_DISABLED_VOICEMAIL), Voicemail::RECEIVER_DISABLED_VOICEMAIL},
        {__EA(SENDER_NOT_AUTHORIZED), Voicemail::SENDER_NOT_AUTHORIZED},
        {__EA(SENDER_BLOCKED), Voicemail::SENDER_BLOCKED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Voicemail::FAILUREREASON& v)
        {
                return enumtostring("Voicemail::FAILUREREASON",VOICEMAIL_FAILUREREASON_MAP,v);
        }
}

extern const EMAP VOICEMAIL_EMAP[] = {
        {Voicemail::P_TYPE, VOICEMAIL_TYPE_MAP},
        {Voicemail::P_STATUS, VOICEMAIL_STATUS_MAP},
        {Voicemail::P_FAILUREREASON, VOICEMAIL_FAILUREREASON_MAP},
        {0, 0}
};
const EMAP* Voicemail::getEMap() const 
{
        return VOICEMAIL_EMAP;
}
const PMAP* Voicemail::getPMap() const 
{
        return VOICEMAIL_PROPERTY_MAP;
}


/** class Sms **********************/

Sms::Sms(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
Sms::~Sms() {  }
void Sms::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgsmsSms*>(cache);  }
unsigned int Sms::moduleID() const 
{
        return MODULE_ID;
}
String Sms::getNamespace() const 
{
        return __EA(SMS);
}

extern const PMAP SMS_PROPERTY_MAP[] = {
        {__EA(TYPE), Sms::P_TYPE},
        {__EA(STATUS), Sms::P_STATUS},
        {__EA(FAILUREREASON), Sms::P_FAILUREREASON},
        {__EA(IS_FAILED_UNSEEN), Sms::P_IS_FAILED_UNSEEN},
        {__EA(TIMESTAMP), Sms::P_TIMESTAMP},
        {__EA(PRICE), Sms::P_PRICE},
        {__EA(PRICE_PRECISION), Sms::P_PRICE_PRECISION},
        {__EA(PRICE_CURRENCY), Sms::P_PRICE_CURRENCY},
        {__EA(REPLY_TO_NUMBER), Sms::P_REPLY_TO_NUMBER},
        {__EA(TARGET_NUMBERS), Sms::P_TARGET_NUMBERS},
        {__EA(TARGET_STATUSES), Sms::P_TARGET_STATUSES},
        {__EA(BODY), Sms::P_BODY},
        {__EA(CHATMSG_ID), Sms::P_CHATMSG_ID},
        {0, 0}
};

extern const PMAP SMS_TYPE_MAP[] = {
        {__EA(OUTGOING), Sms::OUTGOING},
        {__EA(CONFIRMATION_CODE_REQUEST), Sms::CONFIRMATION_CODE_REQUEST},
        {__EA(CONFIRMATION_CODE_SUBMIT), Sms::CONFIRMATION_CODE_SUBMIT},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Sms::TYPE& v)
        {
                return enumtostring("Sms::TYPE",SMS_TYPE_MAP,v);
        }
}

extern const PMAP SMS_STATUS_MAP[] = {
        {__EA(COMPOSING), Sms::COMPOSING},
        {__EA(SENDING_TO_SERVER), Sms::SENDING_TO_SERVER},
        {__EA(SENT_TO_SERVER), Sms::SENT_TO_SERVER},
        {__EA(DELIVERED), Sms::DELIVERED},
        {__EA(SOME_TARGETS_FAILED), Sms::SOME_TARGETS_FAILED},
        {__EA(FAILED), Sms::FAILED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Sms::STATUS& v)
        {
                return enumtostring("Sms::STATUS",SMS_STATUS_MAP,v);
        }
}

extern const PMAP SMS_FAILUREREASON_MAP[] = {
        {__EA(MISC_ERROR), Sms::MISC_ERROR},
        {__EA(SERVER_CONNECT_FAILED), Sms::SERVER_CONNECT_FAILED},
        {__EA(NO_SMS_CAPABILITY), Sms::NO_SMS_CAPABILITY},
        {__EA(INSUFFICIENT_FUNDS), Sms::INSUFFICIENT_FUNDS},
        {__EA(INVALID_CONFIRMATION_CODE), Sms::INVALID_CONFIRMATION_CODE},
        {__EA(USER_BLOCKED), Sms::USER_BLOCKED},
        {__EA(IP_BLOCKED), Sms::IP_BLOCKED},
        {__EA(NODE_BLOCKED), Sms::NODE_BLOCKED},
        {__EA(NO_SENDERID_CAPABILITY), Sms::NO_SENDERID_CAPABILITY},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Sms::FAILUREREASON& v)
        {
                return enumtostring("Sms::FAILUREREASON",SMS_FAILUREREASON_MAP,v);
        }
}

extern const PMAP SMS_TARGETSTATUS_MAP[] = {
        {__EA(TARGET_ANALYZING), Sms::TARGET_ANALYZING},
        {__EA(TARGET_UNDEFINED), Sms::TARGET_UNDEFINED},
        {__EA(TARGET_ACCEPTABLE), Sms::TARGET_ACCEPTABLE},
        {__EA(TARGET_NOT_ROUTABLE), Sms::TARGET_NOT_ROUTABLE},
        {__EA(TARGET_DELIVERY_PENDING), Sms::TARGET_DELIVERY_PENDING},
        {__EA(TARGET_DELIVERY_SUCCESSFUL), Sms::TARGET_DELIVERY_SUCCESSFUL},
        {__EA(TARGET_DELIVERY_FAILED), Sms::TARGET_DELIVERY_FAILED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Sms::TARGETSTATUS& v)
        {
                return enumtostring("Sms::TARGETSTATUS",SMS_TARGETSTATUS_MAP,v);
        }
}

extern const PMAP SMS_SETBODYRESULT_MAP[] = {
        {__EA(BODY_INVALID), Sms::BODY_INVALID},
        {__EA(BODY_TRUNCATED), Sms::BODY_TRUNCATED},
        {__EA(BODY_OK), Sms::BODY_OK},
        {__EA(BODY_LASTCHAR_IGNORED), Sms::BODY_LASTCHAR_IGNORED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Sms::SETBODYRESULT& v)
        {
                return enumtostring("Sms::SETBODYRESULT",SMS_SETBODYRESULT_MAP,v);
        }
}

extern const PMAP SMS_CONFIRM_TYPE_MAP[] = {
        {__EA(ID_SMS), Sms::ID_SMS},
        {__EA(ID_MOBILE), Sms::ID_MOBILE},
        {__EA(ID_SKYPEIN), Sms::ID_SKYPEIN},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Sms::CONFIRM_TYPE& v)
        {
                return enumtostring("Sms::CONFIRM_TYPE",SMS_CONFIRM_TYPE_MAP,v);
        }
}

extern const EMAP SMS_EMAP[] = {
        {Sms::P_TYPE, SMS_TYPE_MAP},
        {Sms::P_STATUS, SMS_STATUS_MAP},
        {Sms::P_FAILUREREASON, SMS_FAILUREREASON_MAP},
        {0, 0}
};
const EMAP* Sms::getEMap() const 
{
        return SMS_EMAP;
}
const PMAP* Sms::getPMap() const 
{
        return SMS_PROPERTY_MAP;
}


/** class Transfer **********************/

Transfer::Transfer(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
Transfer::~Transfer() {  }
void Transfer::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgftTransfer*>(cache);  }
unsigned int Transfer::moduleID() const 
{
        return MODULE_ID;
}
String Transfer::getNamespace() const 
{
        return __EA(TRANSFER);
}

extern const PMAP TRANSFER_PROPERTY_MAP[] = {
        {__EA(TYPE), Transfer::P_TYPE},
        {__EA(PARTNER_HANDLE), Transfer::P_PARTNER_HANDLE},
        {__EA(PARTNER_DISPNAME), Transfer::P_PARTNER_DISPNAME},
        {__EA(STATUS), Transfer::P_STATUS},
        {__EA(FAILUREREASON), Transfer::P_FAILUREREASON},
        {__EA(STARTTIME), Transfer::P_STARTTIME},
        {__EA(FINISHTIME), Transfer::P_FINISHTIME},
        {__EA(FILEPATH), Transfer::P_FILEPATH},
        {__EA(FILENAME), Transfer::P_FILENAME},
        {__EA(FILESIZE), Transfer::P_FILESIZE},
        {__EA(BYTESTRANSFERRED), Transfer::P_BYTESTRANSFERRED},
        {__EA(BYTESPERSECOND), Transfer::P_BYTESPERSECOND},
        {__EA(CHATMSG_GUID), Transfer::P_CHATMSG_GUID},
        {__EA(CHATMSG_INDEX), Transfer::P_CHATMSG_INDEX},
        {__EA(CONVO_ID), Transfer::P_CONVO_ID},
        {0, 0}
};

extern const PMAP TRANSFER_TYPE_MAP[] = {
        {__EA(INCOMING), Transfer::INCOMING},
        {__EA(OUTGOING), Transfer::OUTGOING},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Transfer::TYPE& v)
        {
                return enumtostring("Transfer::TYPE",TRANSFER_TYPE_MAP,v);
        }
}

extern const PMAP TRANSFER_STATUS_MAP[] = {
        {__EA(NEW), Transfer::NEW},
        {__EA(CONNECTING), Transfer::CONNECTING},
        {__EA(WAITING_FOR_ACCEPT), Transfer::WAITING_FOR_ACCEPT},
        {__EA(TRANSFERRING), Transfer::TRANSFERRING},
        {__EA(TRANSFERRING_OVER_RELAY), Transfer::TRANSFERRING_OVER_RELAY},
        {__EA(PAUSED), Transfer::PAUSED},
        {__EA(REMOTELY_PAUSED), Transfer::REMOTELY_PAUSED},
        {__EA(CANCELLED), Transfer::CANCELLED},
        {__EA(COMPLETED), Transfer::COMPLETED},
        {__EA(FAILED), Transfer::FAILED},
        {__EA(PLACEHOLDER), Transfer::PLACEHOLDER},
        {__EA(OFFER_FROM_OTHER_INSTANCE), Transfer::OFFER_FROM_OTHER_INSTANCE},
        {__EA(CANCELLED_BY_REMOTE), Transfer::CANCELLED_BY_REMOTE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Transfer::STATUS& v)
        {
                return enumtostring("Transfer::STATUS",TRANSFER_STATUS_MAP,v);
        }
}

extern const PMAP TRANSFER_FAILUREREASON_MAP[] = {
        {__EA(SENDER_NOT_AUTHORISED), Transfer::SENDER_NOT_AUTHORISED},
        {__EA(REMOTELY_CANCELLED), Transfer::REMOTELY_CANCELLED},
        {__EA(FAILED_READ), Transfer::FAILED_READ},
        {__EA(FAILED_REMOTE_READ), Transfer::FAILED_REMOTE_READ},
        {__EA(FAILED_WRITE), Transfer::FAILED_WRITE},
        {__EA(FAILED_REMOTE_WRITE), Transfer::FAILED_REMOTE_WRITE},
        {__EA(REMOTE_DOES_NOT_SUPPORT_FT), Transfer::REMOTE_DOES_NOT_SUPPORT_FT},
        {__EA(REMOTE_OFFLINE_FOR_TOO_LONG), Transfer::REMOTE_OFFLINE_FOR_TOO_LONG},
        {__EA(TOO_MANY_PARALLEL), Transfer::TOO_MANY_PARALLEL},
        {__EA(PLACEHOLDER_TIMEOUT), Transfer::PLACEHOLDER_TIMEOUT},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Transfer::FAILUREREASON& v)
        {
                return enumtostring("Transfer::FAILUREREASON",TRANSFER_FAILUREREASON_MAP,v);
        }
}

extern const EMAP TRANSFER_EMAP[] = {
        {Transfer::P_TYPE, TRANSFER_TYPE_MAP},
        {Transfer::P_STATUS, TRANSFER_STATUS_MAP},
        {Transfer::P_FAILUREREASON, TRANSFER_FAILUREREASON_MAP},
        {0, 0}
};
const EMAP* Transfer::getEMap() const 
{
        return TRANSFER_EMAP;
}
const PMAP* Transfer::getPMap() const 
{
        return TRANSFER_PROPERTY_MAP;
}


/** class Account **********************/

Account::Account(unsigned int oid, SERootObject* root_obj) :SEObject(oid, root_obj) {}
Account::~Account() {  }
void Account::__clear_cache(void* cache) { if (cache) delete reinterpret_cast<MsgaccountAccount*>(cache);  }
unsigned int Account::moduleID() const 
{
        return MODULE_ID;
}
String Account::getNamespace() const 
{
        return __EA(ACCOUNT);
}

extern const PMAP ACCOUNT_PROPERTY_MAP[] = {
        {__EA(STATUS), Account::P_STATUS},
        {__EA(PWDCHANGESTATUS), Account::P_PWDCHANGESTATUS},
        {__EA(LOGOUTREASON), Account::P_LOGOUTREASON},
        {__EA(COMMITSTATUS), Account::P_COMMITSTATUS},
        {__EA(SUGGESTED_SKYPENAME), Account::P_SUGGESTED_SKYPENAME},
        {__EA(SKYPEOUT_BALANCE_CURRENCY), Account::P_SKYPEOUT_BALANCE_CURRENCY},
        {__EA(SKYPEOUT_BALANCE), Account::P_SKYPEOUT_BALANCE},
        {__EA(SKYPEOUT_PRECISION), Account::P_SKYPEOUT_PRECISION},
        {__EA(SKYPEIN_NUMBERS), Account::P_SKYPEIN_NUMBERS},
        {__EA(CBLSYNCSTATUS), Account::P_CBLSYNCSTATUS},
        {__EA(OFFLINE_CALLFORWARD), Account::P_OFFLINE_CALLFORWARD},
        {__EA(CHAT_POLICY), Account::P_CHAT_POLICY},
        {__EA(SKYPE_CALL_POLICY), Account::P_SKYPE_CALL_POLICY},
        {__EA(PSTN_CALL_POLICY), Account::P_PSTN_CALL_POLICY},
        {__EA(AVATAR_POLICY), Account::P_AVATAR_POLICY},
        {__EA(BUDDYCOUNT_POLICY), Account::P_BUDDYCOUNT_POLICY},
        {__EA(TIMEZONE_POLICY), Account::P_TIMEZONE_POLICY},
        {__EA(WEBPRESENCE_POLICY), Account::P_WEBPRESENCE_POLICY},
        {__EA(PHONENUMBERS_POLICY), Account::P_PHONENUMBERS_POLICY},
        {__EA(VOICEMAIL_POLICY), Account::P_VOICEMAIL_POLICY},
        {__EA(PARTNER_OPTEDOUT), Account::P_PARTNER_OPTEDOUT},
        {__EA(SERVICE_PROVIDER_INFO), Account::P_SERVICE_PROVIDER_INFO},
        {__EA(REGISTRATION_TIMESTAMP), Account::P_REGISTRATION_TIMESTAMP},
        {__EA(NR_OF_OTHER_INSTANCES), Account::P_NR_OF_OTHER_INSTANCES},
        {__EA(SKYPENAME), Account::P_SKYPENAME},
        {__EA(FULLNAME), Account::P_FULLNAME},
        {__EA(BIRTHDAY), Account::P_BIRTHDAY},
        {__EA(GENDER), Account::P_GENDER},
        {__EA(LANGUAGES), Account::P_LANGUAGES},
        {__EA(COUNTRY), Account::P_COUNTRY},
        {__EA(PROVINCE), Account::P_PROVINCE},
        {__EA(CITY), Account::P_CITY},
        {__EA(PHONE_HOME), Account::P_PHONE_HOME},
        {__EA(PHONE_OFFICE), Account::P_PHONE_OFFICE},
        {__EA(PHONE_MOBILE), Account::P_PHONE_MOBILE},
        {__EA(EMAILS), Account::P_EMAILS},
        {__EA(HOMEPAGE), Account::P_HOMEPAGE},
        {__EA(ABOUT), Account::P_ABOUT},
        {__EA(PROFILE_TIMESTAMP), Account::P_PROFILE_TIMESTAMP},
        {__EA(MOOD_TEXT), Account::P_MOOD_TEXT},
        {__EA(TIMEZONE), Account::P_TIMEZONE},
        {__EA(NROF_AUTHED_BUDDIES), Account::P_NROF_AUTHED_BUDDIES},
        {__EA(AVAILABILITY), Account::P_AVAILABILITY},
        {__EA(AVATAR_IMAGE), Account::P_AVATAR_IMAGE},
        {__EA(AVATAR_TIMESTAMP), Account::P_AVATAR_TIMESTAMP},
        {__EA(MOOD_TIMESTAMP), Account::P_MOOD_TIMESTAMP},
        {__EA(RICH_MOOD_TEXT), Account::P_RICH_MOOD_TEXT},
        {0, 0}
};

extern const PMAP ACCOUNT_STATUS_MAP[] = {
        {__EA(LOGGED_OUT), Account::LOGGED_OUT},
        {__EA(LOGGED_OUT_AND_PWD_SAVED), Account::LOGGED_OUT_AND_PWD_SAVED},
        {__EA(CONNECTING_TO_P2P), Account::CONNECTING_TO_P2P},
        {__EA(CONNECTING_TO_SERVER), Account::CONNECTING_TO_SERVER},
        {__EA(LOGGING_IN), Account::LOGGING_IN},
        {__EA(INITIALIZING), Account::INITIALIZING},
        {__EA(LOGGED_IN), Account::LOGGED_IN},
        {__EA(LOGGING_OUT), Account::LOGGING_OUT},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::STATUS& v)
        {
                return enumtostring("Account::STATUS",ACCOUNT_STATUS_MAP,v);
        }
}

extern const PMAP ACCOUNT_CBLSYNCSTATUS_MAP[] = {
        {__EA(CBL_INITIALIZING), Account::CBL_INITIALIZING},
        {__EA(CBL_INITIAL_SYNC_PENDING), Account::CBL_INITIAL_SYNC_PENDING},
        {__EA(CBL_SYNC_PENDING), Account::CBL_SYNC_PENDING},
        {__EA(CBL_SYNC_IN_PROGRESS), Account::CBL_SYNC_IN_PROGRESS},
        {__EA(CBL_IN_SYNC), Account::CBL_IN_SYNC},
        {__EA(CBL_SYNC_FAILED), Account::CBL_SYNC_FAILED},
        {__EA(CBL_REMOTE_SYNC_PENDING), Account::CBL_REMOTE_SYNC_PENDING},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::CBLSYNCSTATUS& v)
        {
                return enumtostring("Account::CBLSYNCSTATUS",ACCOUNT_CBLSYNCSTATUS_MAP,v);
        }
}

extern const PMAP ACCOUNT_LOGOUTREASON_MAP[] = {
        {__EA(LOGOUT_CALLED), Account::LOGOUT_CALLED},
        {__EA(HTTPS_PROXY_AUTH_FAILED), Account::HTTPS_PROXY_AUTH_FAILED},
        {__EA(SOCKS_PROXY_AUTH_FAILED), Account::SOCKS_PROXY_AUTH_FAILED},
        {__EA(P2P_CONNECT_FAILED), Account::P2P_CONNECT_FAILED},
        {__EA(SERVER_CONNECT_FAILED), Account::SERVER_CONNECT_FAILED},
        {__EA(SERVER_OVERLOADED), Account::SERVER_OVERLOADED},
        {__EA(DB_IN_USE), Account::DB_IN_USE},
        {__EA(INVALID_SKYPENAME), Account::INVALID_SKYPENAME},
        {__EA(INVALID_EMAIL), Account::INVALID_EMAIL},
        {__EA(UNACCEPTABLE_PASSWORD), Account::UNACCEPTABLE_PASSWORD},
        {__EA(SKYPENAME_TAKEN), Account::SKYPENAME_TAKEN},
        {__EA(REJECTED_AS_UNDERAGE), Account::REJECTED_AS_UNDERAGE},
        {__EA(NO_SUCH_IDENTITY), Account::NO_SUCH_IDENTITY},
        {__EA(INCORRECT_PASSWORD), Account::INCORRECT_PASSWORD},
        {__EA(TOO_MANY_LOGIN_ATTEMPTS), Account::TOO_MANY_LOGIN_ATTEMPTS},
        {__EA(PASSWORD_HAS_CHANGED), Account::PASSWORD_HAS_CHANGED},
        {__EA(PERIODIC_UIC_UPDATE_FAILED), Account::PERIODIC_UIC_UPDATE_FAILED},
        {__EA(DB_DISK_FULL), Account::DB_DISK_FULL},
        {__EA(DB_IO_ERROR), Account::DB_IO_ERROR},
        {__EA(DB_CORRUPT), Account::DB_CORRUPT},
        {__EA(DB_FAILURE), Account::DB_FAILURE},
        {__EA(INVALID_APP_ID), Account::INVALID_APP_ID},
        {__EA(APP_ID_FAILURE), Account::APP_ID_FAILURE},
        {__EA(UNSUPPORTED_VERSION), Account::UNSUPPORTED_VERSION},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::LOGOUTREASON& v)
        {
                return enumtostring("Account::LOGOUTREASON",ACCOUNT_LOGOUTREASON_MAP,v);
        }
}

extern const PMAP ACCOUNT_PWDCHANGESTATUS_MAP[] = {
        {__EA(PWD_OK), Account::PWD_OK},
        {__EA(PWD_CHANGING), Account::PWD_CHANGING},
        {__EA(PWD_INVALID_OLD_PASSWORD), Account::PWD_INVALID_OLD_PASSWORD},
        {__EA(PWD_SERVER_CONNECT_FAILED), Account::PWD_SERVER_CONNECT_FAILED},
        {__EA(PWD_OK_BUT_CHANGE_SUGGESTED), Account::PWD_OK_BUT_CHANGE_SUGGESTED},
        {__EA(PWD_MUST_DIFFER_FROM_OLD), Account::PWD_MUST_DIFFER_FROM_OLD},
        {__EA(PWD_INVALID_NEW_PWD), Account::PWD_INVALID_NEW_PWD},
        {__EA(PWD_MUST_LOG_IN_TO_CHANGE), Account::PWD_MUST_LOG_IN_TO_CHANGE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::PWDCHANGESTATUS& v)
        {
                return enumtostring("Account::PWDCHANGESTATUS",ACCOUNT_PWDCHANGESTATUS_MAP,v);
        }
}

extern const PMAP ACCOUNT_COMMITSTATUS_MAP[] = {
        {__EA(COMMITTED), Account::COMMITTED},
        {__EA(COMMITTING_TO_SERVER), Account::COMMITTING_TO_SERVER},
        {__EA(COMMIT_FAILED), Account::COMMIT_FAILED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::COMMITSTATUS& v)
        {
                return enumtostring("Account::COMMITSTATUS",ACCOUNT_COMMITSTATUS_MAP,v);
        }
}

extern const PMAP ACCOUNT_CHATPOLICY_MAP[] = {
        {__EA(EVERYONE_CAN_ADD), Account::EVERYONE_CAN_ADD},
        {__EA(BUDDIES_OR_AUTHORIZED_CAN_ADD), Account::BUDDIES_OR_AUTHORIZED_CAN_ADD},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::CHATPOLICY& v)
        {
                return enumtostring("Account::CHATPOLICY",ACCOUNT_CHATPOLICY_MAP,v);
        }
}

extern const PMAP ACCOUNT_SKYPECALLPOLICY_MAP[] = {
        {__EA(EVERYONE_CAN_CALL), Account::EVERYONE_CAN_CALL},
        {__EA(BUDDIES_OR_AUTHORIZED_CAN_CALL), Account::BUDDIES_OR_AUTHORIZED_CAN_CALL},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::SKYPECALLPOLICY& v)
        {
                return enumtostring("Account::SKYPECALLPOLICY",ACCOUNT_SKYPECALLPOLICY_MAP,v);
        }
}

extern const PMAP ACCOUNT_PSTNCALLPOLICY_MAP[] = {
        {__EA(ALL_NUMBERS_CAN_CALL), Account::ALL_NUMBERS_CAN_CALL},
        {__EA(DISCLOSED_NUMBERS_CAN_CALL), Account::DISCLOSED_NUMBERS_CAN_CALL},
        {__EA(BUDDY_NUMBERS_CAN_CALL), Account::BUDDY_NUMBERS_CAN_CALL},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::PSTNCALLPOLICY& v)
        {
                return enumtostring("Account::PSTNCALLPOLICY",ACCOUNT_PSTNCALLPOLICY_MAP,v);
        }
}

extern const PMAP ACCOUNT_AVATARPOLICY_MAP[] = {
        {__EA(BUDDIES_OR_AUTHORIZED_CAN_SEE), Account::BUDDIES_OR_AUTHORIZED_CAN_SEE},
        {__EA(EVERYONE_CAN_SEE), Account::EVERYONE_CAN_SEE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::AVATARPOLICY& v)
        {
                return enumtostring("Account::AVATARPOLICY",ACCOUNT_AVATARPOLICY_MAP,v);
        }
}

extern const PMAP ACCOUNT_BUDDYCOUNTPOLICY_MAP[] = {
        {__EA(DISCLOSE_TO_AUTHORIZED), Account::DISCLOSE_TO_AUTHORIZED},
        {__EA(DISCLOSE_TO_NOONE), Account::DISCLOSE_TO_NOONE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::BUDDYCOUNTPOLICY& v)
        {
                return enumtostring("Account::BUDDYCOUNTPOLICY",ACCOUNT_BUDDYCOUNTPOLICY_MAP,v);
        }
}

extern const PMAP ACCOUNT_TIMEZONEPOLICY_MAP[] = {
        {__EA(TZ_AUTOMATIC), Account::TZ_AUTOMATIC},
        {__EA(TZ_MANUAL), Account::TZ_MANUAL},
        {__EA(TZ_UNDISCLOSED), Account::TZ_UNDISCLOSED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::TIMEZONEPOLICY& v)
        {
                return enumtostring("Account::TIMEZONEPOLICY",ACCOUNT_TIMEZONEPOLICY_MAP,v);
        }
}

extern const PMAP ACCOUNT_WEBPRESENCEPOLICY_MAP[] = {
        {__EA(WEBPRESENCE_DISABLED), Account::WEBPRESENCE_DISABLED},
        {__EA(WEBPRESENCE_ENABLED), Account::WEBPRESENCE_ENABLED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::WEBPRESENCEPOLICY& v)
        {
                return enumtostring("Account::WEBPRESENCEPOLICY",ACCOUNT_WEBPRESENCEPOLICY_MAP,v);
        }
}

extern const PMAP ACCOUNT_PHONENUMBERSPOLICY_MAP[] = {
        {__EA(PHONENUMBERS_VISIBLE_FOR_BUDDIES), Account::PHONENUMBERS_VISIBLE_FOR_BUDDIES},
        {__EA(PHONENUMBERS_VISIBLE_FOR_EVERYONE), Account::PHONENUMBERS_VISIBLE_FOR_EVERYONE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::PHONENUMBERSPOLICY& v)
        {
                return enumtostring("Account::PHONENUMBERSPOLICY",ACCOUNT_PHONENUMBERSPOLICY_MAP,v);
        }
}

extern const PMAP ACCOUNT_VOICEMAILPOLICY_MAP[] = {
        {__EA(VOICEMAIL_ENABLED), Account::VOICEMAIL_ENABLED},
        {__EA(VOICEMAIL_DISABLED), Account::VOICEMAIL_DISABLED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::VOICEMAILPOLICY& v)
        {
                return enumtostring("Account::VOICEMAILPOLICY",ACCOUNT_VOICEMAILPOLICY_MAP,v);
        }
}

extern const PMAP ACCOUNT_CAPABILITYSTATUS_MAP[] = {
        {__EA(NO_CAPABILITY), Account::NO_CAPABILITY},
        {__EA(CAPABILITY_EXISTS), Account::CAPABILITY_EXISTS},
        {__EA(FIRST_EXPIRY_WARNING), Account::FIRST_EXPIRY_WARNING},
        {__EA(SECOND_EXPIRY_WARNING), Account::SECOND_EXPIRY_WARNING},
        {__EA(FINAL_EXPIRY_WARNING), Account::FINAL_EXPIRY_WARNING},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Account::CAPABILITYSTATUS& v)
        {
                return enumtostring("Account::CAPABILITYSTATUS",ACCOUNT_CAPABILITYSTATUS_MAP,v);
        }
}

extern const EMAP ACCOUNT_EMAP[] = {
        {Account::P_STATUS, ACCOUNT_STATUS_MAP},
        {Account::P_PWDCHANGESTATUS, ACCOUNT_PWDCHANGESTATUS_MAP},
        {Account::P_LOGOUTREASON, ACCOUNT_LOGOUTREASON_MAP},
        {Account::P_COMMITSTATUS, ACCOUNT_COMMITSTATUS_MAP},
        {Account::P_CBLSYNCSTATUS, ACCOUNT_CBLSYNCSTATUS_MAP},
        {Account::P_CHAT_POLICY, ACCOUNT_CHATPOLICY_MAP},
        {Account::P_SKYPE_CALL_POLICY, ACCOUNT_SKYPECALLPOLICY_MAP},
        {Account::P_PSTN_CALL_POLICY, ACCOUNT_PSTNCALLPOLICY_MAP},
        {Account::P_AVATAR_POLICY, ACCOUNT_AVATARPOLICY_MAP},
        {Account::P_BUDDYCOUNT_POLICY, ACCOUNT_BUDDYCOUNTPOLICY_MAP},
        {Account::P_TIMEZONE_POLICY, ACCOUNT_TIMEZONEPOLICY_MAP},
        {Account::P_WEBPRESENCE_POLICY, ACCOUNT_WEBPRESENCEPOLICY_MAP},
        {Account::P_PHONENUMBERS_POLICY, ACCOUNT_PHONENUMBERSPOLICY_MAP},
        {Account::P_VOICEMAIL_POLICY, ACCOUNT_VOICEMAILPOLICY_MAP},
        {Account::P_AVAILABILITY, CONTACT_AVAILABILITY_MAP},
        {0, 0}
};
const EMAP* Account::getEMap() const 
{
        return ACCOUNT_EMAP;
}
const PMAP* Account::getPMap() const 
{
        return ACCOUNT_PROPERTY_MAP;
}

void Skype::OnNewCustomContactGroup(
        __se_unused const ContactGroupRef& /*group*/
) {}

extern const PMAP SKYPE_IDENTITYTYPE_MAP[] = {
        {__EA(UNRECOGNIZED), Skype::UNRECOGNIZED},
        {__EA(SKYPE), Skype::SKYPE},
        {__EA(SKYPE_MYSELF), Skype::SKYPE_MYSELF},
        {__EA(SKYPE_UNDISCLOSED), Skype::SKYPE_UNDISCLOSED},
        {__EA(PSTN), Skype::PSTN},
        {__EA(PSTN_EMERGENCY), Skype::PSTN_EMERGENCY},
        {__EA(PSTN_FREE), Skype::PSTN_FREE},
        {__EA(PSTN_UNDISCLOSED), Skype::PSTN_UNDISCLOSED},
        {__EA(CONFERENCE), Skype::CONFERENCE},
        {__EA(EXTERNAL), Skype::EXTERNAL},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Skype::IDENTITYTYPE& v)
        {
                return enumtostring("Skype::IDENTITYTYPE",SKYPE_IDENTITYTYPE_MAP,v);
        }
}

extern const PMAP SKYPE_NORMALIZERESULT_MAP[] = {
        {__EA(IDENTITY_OK), Skype::IDENTITY_OK},
        {__EA(IDENTITY_EMPTY), Skype::IDENTITY_EMPTY},
        {__EA(IDENTITY_TOO_LONG), Skype::IDENTITY_TOO_LONG},
        {__EA(IDENTITY_CONTAINS_INVALID_CHAR), Skype::IDENTITY_CONTAINS_INVALID_CHAR},
        {__EA(PSTN_NUMBER_TOO_SHORT), Skype::PSTN_NUMBER_TOO_SHORT},
        {__EA(PSTN_NUMBER_HAS_INVALID_PREFIX), Skype::PSTN_NUMBER_HAS_INVALID_PREFIX},
        {__EA(SKYPENAME_STARTS_WITH_NONALPHA), Skype::SKYPENAME_STARTS_WITH_NONALPHA},
        {__EA(SKYPENAME_SHORTER_THAN_6_CHARS), Skype::SKYPENAME_SHORTER_THAN_6_CHARS},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Skype::NORMALIZERESULT& v)
        {
                return enumtostring("Skype::NORMALIZERESULT",SKYPE_NORMALIZERESULT_MAP,v);
        }
}

void Skype::OnContactOnlineAppearance(
        __se_unused const ContactRef& /*contact*/
) {}

void Skype::OnContactGoneOffline(
        __se_unused const ContactRef& /*contact*/
) {}

void Skype::OnConversationListChange(
        __se_unused const ConversationRef& /*conversation*/,
        __se_unused const Conversation::LIST_TYPE& /*type*/,
        __se_unused const bool& /*added*/
) {}

void Skype::OnMessage(
        __se_unused const MessageRef& /*message*/,
        __se_unused const bool& /*changesInboxTimestamp*/,
        __se_unused const MessageRef& /*supersedesHistoryMessage*/,
        __se_unused const ConversationRef& /*conversation*/
) {}

void Skype::OnAvailableVideoDeviceListChange(
) {}

void Skype::OnH264Activated(
) {}

extern const PMAP SKYPE_PREPARESOUNDRESULT_MAP[] = {
        {__EA(PREPARESOUND_SUCCESS), Skype::PREPARESOUND_SUCCESS},
        {__EA(PREPARESOUND_MISC_ERROR), Skype::PREPARESOUND_MISC_ERROR},
        {__EA(PREPARESOUND_FILE_NOT_FOUND), Skype::PREPARESOUND_FILE_NOT_FOUND},
        {__EA(PREPARESOUND_FILE_TOO_BIG), Skype::PREPARESOUND_FILE_TOO_BIG},
        {__EA(PREPARESOUND_FILE_READ_ERROR), Skype::PREPARESOUND_FILE_READ_ERROR},
        {__EA(PREPARESOUND_UNSUPPORTED_FILE_FORMAT), Skype::PREPARESOUND_UNSUPPORTED_FILE_FORMAT},
        {__EA(PREPARESOUND_PLAYBACK_NOT_SUPPORTED), Skype::PREPARESOUND_PLAYBACK_NOT_SUPPORTED},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Skype::PREPARESOUNDRESULT& v)
        {
                return enumtostring("Skype::PREPARESOUNDRESULT",SKYPE_PREPARESOUNDRESULT_MAP,v);
        }
}

extern const PMAP SKYPE_AUDIODEVICE_CAPABILITIES_MAP[] = {
        {__EA(HAS_VIDEO_CAPTURE), Skype::HAS_VIDEO_CAPTURE},
        {__EA(HAS_USB_INTERFACE), Skype::HAS_USB_INTERFACE},
        {__EA(POSSIBLY_HEADSET), Skype::POSSIBLY_HEADSET},
        {__EA(HAS_AUDIO_CAPTURE), Skype::HAS_AUDIO_CAPTURE},
        {__EA(HAS_AUDIO_RENDERING), Skype::HAS_AUDIO_RENDERING},
        {__EA(HAS_LOWBANDWIDTH_CAPTURE), Skype::HAS_LOWBANDWIDTH_CAPTURE},
        {__EA(IS_WEBCAM), Skype::IS_WEBCAM},
        {__EA(IS_HEADSET), Skype::IS_HEADSET},
        {__EA(POSSIBLY_WEBCAM), Skype::POSSIBLY_WEBCAM},
        {__EA(HAS_VIDEO_RENDERING), Skype::HAS_VIDEO_RENDERING},
        {__EA(HAS_BLUETOOTH_INTERFACE), Skype::HAS_BLUETOOTH_INTERFACE},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Skype::AUDIODEVICE_CAPABILITIES& v)
        {
                return enumtostring("Skype::AUDIODEVICE_CAPABILITIES",SKYPE_AUDIODEVICE_CAPABILITIES_MAP,v);
        }
}

void Skype::OnAvailableDeviceListChange(
) {}

void Skype::OnNrgLevelsChange(
) {}

extern const PMAP SKYPE_OPERATING_MEDIA_MAP[] = {
        {__EA(OM_UNKNOWN), Skype::OM_UNKNOWN},
        {__EA(OM_FREE), Skype::OM_FREE},
        {__EA(OM_FREE_WIRELESS), Skype::OM_FREE_WIRELESS},
        {__EA(OM_3G), Skype::OM_3G},
        {__EA(OM_4G), Skype::OM_4G},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Skype::OPERATING_MEDIA& v)
        {
                return enumtostring("Skype::OPERATING_MEDIA",SKYPE_OPERATING_MEDIA_MAP,v);
        }
}

extern const PMAP SKYPE_VALIDATERESULT_MAP[] = {
        {__EA(NOT_VALIDATED), Skype::NOT_VALIDATED},
        {__EA(VALIDATED_OK), Skype::VALIDATED_OK},
        {__EA(TOO_SHORT), Skype::TOO_SHORT},
        {__EA(TOO_LONG), Skype::TOO_LONG},
        {__EA(CONTAINS_INVALID_CHAR), Skype::CONTAINS_INVALID_CHAR},
        {__EA(CONTAINS_SPACE), Skype::CONTAINS_SPACE},
        {__EA(SAME_AS_USERNAME), Skype::SAME_AS_USERNAME},
        {__EA(INVALID_FORMAT), Skype::INVALID_FORMAT},
        {__EA(CONTAINS_INVALID_WORD), Skype::CONTAINS_INVALID_WORD},
        {__EA(TOO_SIMPLE), Skype::TOO_SIMPLE},
        {__EA(STARTS_WITH_INVALID_CHAR), Skype::STARTS_WITH_INVALID_CHAR},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Skype::VALIDATERESULT& v)
        {
                return enumtostring("Skype::VALIDATERESULT",SKYPE_VALIDATERESULT_MAP,v);
        }
}

extern const PMAP SKYPE_PROXYTYPE_MAP[] = {
        {__EA(HTTPS_PROXY), Skype::HTTPS_PROXY},
        {__EA(SOCKS_PROXY), Skype::SOCKS_PROXY},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Skype::PROXYTYPE& v)
        {
                return enumtostring("Skype::PROXYTYPE",SKYPE_PROXYTYPE_MAP,v);
        }
}

void Skype::OnProxyAuthFailure(
        __se_unused const PROXYTYPE& /*type*/
) {}

extern const PMAP SKYPE_APP2APP_STREAMS_MAP[] = {
        {__EA(ALL_STREAMS), Skype::ALL_STREAMS},
        {__EA(SENDING_STREAMS), Skype::SENDING_STREAMS},
        {__EA(RECEIVED_STREAMS), Skype::RECEIVED_STREAMS},
        {0, 0}
};

namespace Sid {
        template<> Sid::String tostring(const Skype::APP2APP_STREAMS& v)
        {
                return enumtostring("Skype::APP2APP_STREAMS",SKYPE_APP2APP_STREAMS_MAP,v);
        }
}

void Skype::OnApp2AppDatagram(
        __se_unused const Sid::String& /*appname*/,
        __se_unused const Sid::String& /*stream*/,
        __se_unused const Sid::Binary& /*data*/
) {}

void Skype::OnApp2AppStreamListChange(
        __se_unused const Sid::String& /*appname*/,
        __se_unused const APP2APP_STREAMS& /*listType*/,
        __se_unused const Sid::List_String& /*streams*/,
        __se_unused const Sid::List_uint& /*receivedSizes*/
) {}


ContactGroup * Skype::newContactGroup(int oid)
{
        return new ContactGroup(oid, this);
}

Contact * Skype::newContact(int oid)
{
        return new Contact(oid, this);
}

ContactSearch * Skype::newContactSearch(int oid)
{
        return new ContactSearch(oid, this);
}

Participant * Skype::newParticipant(int oid)
{
        return new Participant(oid, this);
}

Conversation * Skype::newConversation(int oid)
{
        return new Conversation(oid, this);
}

Message * Skype::newMessage(int oid)
{
        return new Message(oid, this);
}

Video * Skype::newVideo(int oid)
{
        return new Video(oid, this);
}

Voicemail * Skype::newVoicemail(int oid)
{
        return new Voicemail(oid, this);
}

Sms * Skype::newSms(int oid)
{
        return new Sms(oid, this);
}

Transfer * Skype::newTransfer(int oid)
{
        return new Transfer(oid, this);
}

Account * Skype::newAccount(int oid)
{
        return new Account(oid, this);
}

SEObject* Skype::factory(const uint& oid, const uint& modid){
        SEObject* o = 0;
        switch (modid) {
        case ContactGroup::MODULE_ID:
                o = newContactGroup(oid);
                break;
        case Contact::MODULE_ID:
                o = newContact(oid);
                break;
        case ContactSearch::MODULE_ID:
                o = newContactSearch(oid);
                break;
        case Participant::MODULE_ID:
                o = newParticipant(oid);
                break;
        case Conversation::MODULE_ID:
                o = newConversation(oid);
                break;
        case Message::MODULE_ID:
                o = newMessage(oid);
                break;
        case Video::MODULE_ID:
                o = newVideo(oid);
                break;
        case Voicemail::MODULE_ID:
                o = newVoicemail(oid);
                break;
        case Sms::MODULE_ID:
                o = newSms(oid);
                break;
        case Transfer::MODULE_ID:
                o = newTransfer(oid);
                break;
        case Account::MODULE_ID:
                o = newAccount(oid);
                break;
        default:
                break;
        }
        return o;
}

Sid::List_uint* Skype::prop_cached_list(void* propstruct, const uint& moduleid) 
{
        switch (moduleid) {
        case ContactGroup::MODULE_ID:
                return &reinterpret_cast<MsgcontactContactGroup*>(propstruct)->cachedProps;
        case Contact::MODULE_ID:
                return &reinterpret_cast<MsgcontactContact*>(propstruct)->cachedProps;
        case ContactSearch::MODULE_ID:
                return &reinterpret_cast<MsgcontactsearchContactSearch*>(propstruct)->cachedProps;
        case Participant::MODULE_ID:
                return &reinterpret_cast<MsgconversationParticipant*>(propstruct)->cachedProps;
        case Conversation::MODULE_ID:
                return &reinterpret_cast<MsgconversationConversation*>(propstruct)->cachedProps;
        case Message::MODULE_ID:
                return &reinterpret_cast<MsgconversationMessage*>(propstruct)->cachedProps;
        case Video::MODULE_ID:
                return &reinterpret_cast<MsgvideoVideo*>(propstruct)->cachedProps;
        case Voicemail::MODULE_ID:
                return &reinterpret_cast<MsgvmVoicemail*>(propstruct)->cachedProps;
        case Sms::MODULE_ID:
                return &reinterpret_cast<MsgsmsSms*>(propstruct)->cachedProps;
        case Transfer::MODULE_ID:
                return &reinterpret_cast<MsgftTransfer*>(propstruct)->cachedProps;
        case Account::MODULE_ID:
                return &reinterpret_cast<MsgaccountAccount*>(propstruct)->cachedProps;
        default:
                break;
        }
        return 0;
}

void* Skype::prop_factory(const uint& moduleid) 
{
        switch (moduleid) {
        case ContactGroup::MODULE_ID:
                return new MsgcontactContactGroup();
        case Contact::MODULE_ID:
                return new MsgcontactContact();
        case ContactSearch::MODULE_ID:
                return new MsgcontactsearchContactSearch();
        case Participant::MODULE_ID:
                return new MsgconversationParticipant();
        case Conversation::MODULE_ID:
                return new MsgconversationConversation();
        case Message::MODULE_ID:
                return new MsgconversationMessage();
        case Video::MODULE_ID:
                return new MsgvideoVideo();
        case Voicemail::MODULE_ID:
                return new MsgvmVoicemail();
        case Sms::MODULE_ID:
                return new MsgsmsSms();
        case Transfer::MODULE_ID:
                return new MsgftTransfer();
        case Account::MODULE_ID:
                return new MsgaccountAccount();
        default:
                break;
        }
        return 0;
}
