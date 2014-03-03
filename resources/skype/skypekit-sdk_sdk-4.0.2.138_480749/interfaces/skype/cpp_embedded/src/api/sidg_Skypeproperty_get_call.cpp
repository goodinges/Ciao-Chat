#include "SidPlatform.hpp"
#include "SidCommandProcessor.hpp"
#include "skype-embedded_2.h"
#include "sidg_msgs_Skype.hpp"
#include "SidField.hpp"

#ifdef SE_USE_NAMESPACE
namespace Skype {
#endif
        bool ContactGroup::GetPropType(ContactGroup::TYPE& type) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 10, getOID(), 155,"ZG\233\001]\012",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        type = *static_cast<ContactGroup::TYPE*>(__session->get_cached_prop(10, getOID(), 155, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool ContactGroup::GetPropCustomGroupId(uint& custom_group_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 10, getOID(), 154,"ZG\232\001]\012",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        custom_group_id = *static_cast<uint*>(__session->get_cached_prop(10, getOID(), 154, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool ContactGroup::GetPropGivenDisplayname(Sid::String& given_displayname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 10, getOID(), 151,"ZG\227\001]\012",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        given_displayname = *static_cast<Sid::String*>(__session->get_cached_prop(10, getOID(), 151, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool ContactGroup::GetPropNrofcontacts(uint& nrofcontacts) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 10, getOID(), 152,"ZG\230\001]\012",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        nrofcontacts = *static_cast<uint*>(__session->get_cached_prop(10, getOID(), 152, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool ContactGroup::GetPropNrofcontactsOnline(uint& nrofcontacts_online) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 10, getOID(), 153,"ZG\231\001]\012",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        nrofcontacts_online = *static_cast<uint*>(__session->get_cached_prop(10, getOID(), 153, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropType(Contact::TYPE& type) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 202,"ZG\312\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        type = *static_cast<Contact::TYPE*>(__session->get_cached_prop(2, getOID(), 202, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropSkypename(Sid::String& skypename) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 4,"ZG\004]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        skypename = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 4, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropPstnnumber(Sid::String& pstnnumber) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 6,"ZG\006]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        pstnnumber = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 6, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropFullname(Sid::String& fullname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 5,"ZG\005]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        fullname = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 5, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropBirthday(uint& birthday) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 7,"ZG\007]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        birthday = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 7, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropGender(uint& gender) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 8,"ZG\010]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        gender = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 8, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropLanguages(Sid::String& languages) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 9,"ZG\011]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        languages = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 9, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropCountry(Sid::String& country) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 10,"ZG\012]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        country = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 10, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropProvince(Sid::String& province) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 11,"ZG\013]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        province = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 11, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropCity(Sid::String& city) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 12,"ZG\014]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        city = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 12, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropPhoneHome(Sid::String& phone_home) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 13,"ZG\015]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        phone_home = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 13, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropPhoneOffice(Sid::String& phone_office) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 14,"ZG\016]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        phone_office = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 14, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropPhoneMobile(Sid::String& phone_mobile) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 15,"ZG\017]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        phone_mobile = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 15, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropEmails(Sid::String& emails) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 16,"ZG\020]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        emails = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 16, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropHomepage(Sid::String& homepage) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 17,"ZG\021]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        homepage = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 17, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAbout(Sid::String& about) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 18,"ZG\022]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        about = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 18, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAvatarImage(Sid::Binary& avatar_image) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 37,"ZG%]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        avatar_image = *static_cast<Sid::Binary*>(__session->get_cached_prop(2, getOID(), 37, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropMoodText(Sid::String& mood_text) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 26,"ZG\032]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        mood_text = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 26, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropRichMoodText(Sid::String& rich_mood_text) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 205,"ZG\315\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        rich_mood_text = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 205, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropTimezone(uint& time_zone) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 27,"ZG\033]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        time_zone = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 27, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropCapabilities(Sid::Binary& capabilities) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 36,"ZG$]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        capabilities = *static_cast<Sid::Binary*>(__session->get_cached_prop(2, getOID(), 36, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropProfileTimestamp(uint& profile_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 19,"ZG\023]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        profile_timestamp = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 19, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropNrofAuthedBuddies(uint& nrof_authed_buddies) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 28,"ZG\034]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        nrof_authed_buddies = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 28, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropIpcountry(Sid::String& ipcountry) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 29,"ZG\035]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        ipcountry = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 29, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAvatarTimestamp(uint& avatar_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 182,"ZG\266\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        avatar_timestamp = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 182, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropMoodTimestamp(uint& mood_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 183,"ZG\267\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        mood_timestamp = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 183, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropReceivedAuthrequest(Sid::String& received_authrequest) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 20,"ZG\024]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        received_authrequest = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 20, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAuthreqTimestamp(uint& authreq_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 25,"ZG\031]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        authreq_timestamp = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 25, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropLastonlineTimestamp(uint& lastonline_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 35,"ZG#]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        lastonline_timestamp = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 35, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAvailability(Contact::AVAILABILITY& availability) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 34,"ZG\042]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        availability = *static_cast<Contact::AVAILABILITY*>(__session->get_cached_prop(2, getOID(), 34, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropDisplayname(Sid::String& displayname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 21,"ZG\025]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        displayname = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 21, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropRefreshing(bool& refreshing) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 22,"ZG\026]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        refreshing = *static_cast<bool*>(__session->get_cached_prop(2, getOID(), 22, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropGivenAuthlevel(Contact::AUTHLEVEL& given_authlevel) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 23,"ZG\027]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        given_authlevel = *static_cast<Contact::AUTHLEVEL*>(__session->get_cached_prop(2, getOID(), 23, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropGivenDisplayname(Sid::String& given_displayname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 33,"ZG!]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        given_displayname = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 33, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAssignedComment(Sid::String& assigned_comment) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 180,"ZG\264\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        assigned_comment = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 180, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropLastusedTimestamp(uint& lastused_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 39,"ZG']\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        lastused_timestamp = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 39, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAuthrequestCount(uint& authrequest_count) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 41,"ZG)]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        authrequest_count = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 41, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAssignedPhone1(Sid::String& assigned_phone1) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 184,"ZG\270\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        assigned_phone1 = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 184, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAssignedPhone1Label(Sid::String& assigned_phone1_label) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 185,"ZG\271\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        assigned_phone1_label = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 185, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAssignedPhone2(Sid::String& assigned_phone2) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 186,"ZG\272\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        assigned_phone2 = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 186, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAssignedPhone2Label(Sid::String& assigned_phone2_label) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 187,"ZG\273\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        assigned_phone2_label = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 187, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAssignedPhone3(Sid::String& assigned_phone3) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 188,"ZG\274\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        assigned_phone3 = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 188, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropAssignedPhone3Label(Sid::String& assigned_phone3_label) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 189,"ZG\275\001]\002",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        assigned_phone3_label = *static_cast<Sid::String*>(__session->get_cached_prop(2, getOID(), 189, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Contact::GetPropPopularityOrd(uint& popularity_ord) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 2, getOID(), 42,"ZG*]\002",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        popularity_ord = *static_cast<uint*>(__session->get_cached_prop(2, getOID(), 42, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool ContactSearch::GetPropContactSearchStatus(ContactSearch::STATUS& contact_search_status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 1, getOID(), 200,"ZG\310\001]\001",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        contact_search_status = *static_cast<ContactSearch::STATUS*>(__session->get_cached_prop(1, getOID(), 200, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropConvoId(ConversationRef& convo_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 930,"ZG\242\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        convo_id = *static_cast<ConversationRef*>(__session->get_cached_prop(19, getOID(), 930, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropIdentity(Sid::String& identity) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 931,"ZG\243\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        identity = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 931, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropRank(Participant::RANK& rank) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 932,"ZG\244\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        rank = *static_cast<Participant::RANK*>(__session->get_cached_prop(19, getOID(), 932, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropRequestedRank(Participant::RANK& requested_rank) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 933,"ZG\245\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        requested_rank = *static_cast<Participant::RANK*>(__session->get_cached_prop(19, getOID(), 933, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropTextStatus(Participant::TEXT_STATUS& text_status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 934,"ZG\246\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        text_status = *static_cast<Participant::TEXT_STATUS*>(__session->get_cached_prop(19, getOID(), 934, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropVoiceStatus(Participant::VOICE_STATUS& voice_status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 935,"ZG\247\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        voice_status = *static_cast<Participant::VOICE_STATUS*>(__session->get_cached_prop(19, getOID(), 935, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropVideoStatus(Participant::VIDEO_STATUS& video_status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 936,"ZG\250\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        video_status = *static_cast<Participant::VIDEO_STATUS*>(__session->get_cached_prop(19, getOID(), 936, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropLiveIdentity(Sid::String& live_identity) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 943,"ZG\257\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        live_identity = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 943, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropLivePriceForMe(Sid::String& live_price_for_me) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 938,"ZG\252\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        live_price_for_me = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 938, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropLiveFwdIdentities(Sid::String& live_fwd_identities) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 948,"ZG\264\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        live_fwd_identities = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 948, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropLiveStartTimestamp(uint& live_start_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 939,"ZG\253\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        live_start_timestamp = *static_cast<uint*>(__session->get_cached_prop(19, getOID(), 939, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropSoundLevel(uint& sound_level) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 941,"ZG\255\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        sound_level = *static_cast<uint*>(__session->get_cached_prop(19, getOID(), 941, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropDebuginfo(Sid::String& debuginfo) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 942,"ZG\256\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        debuginfo = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 942, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropLastVoiceError(Sid::String& last_voice_error) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 947,"ZG\263\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        last_voice_error = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 947, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropQualityProblems(Sid::String& quality_problems) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 949,"ZG\265\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        quality_problems = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 949, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropLiveType(uint& live_type) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 950,"ZG\266\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        live_type = *static_cast<uint*>(__session->get_cached_prop(19, getOID(), 950, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropLiveCountry(Sid::String& live_country) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 951,"ZG\267\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        live_country = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 951, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropTransferredBy(Sid::String& transferred_by) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 952,"ZG\270\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        transferred_by = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 952, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropTransferredTo(Sid::String& transferred_to) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 953,"ZG\271\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        transferred_to = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 953, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropAdder(Sid::String& adder) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 954,"ZG\272\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        adder = *static_cast<Sid::String*>(__session->get_cached_prop(19, getOID(), 954, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Participant::GetPropLastLeavereason(LEAVE_REASON& last_leavereason) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 19, getOID(), 955,"ZG\273\007]\023",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        last_leavereason = *static_cast<LEAVE_REASON*>(__session->get_cached_prop(19, getOID(), 955, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropIdentity(Sid::String& identity) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 972,"ZG\314\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        identity = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 972, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropType(Conversation::TYPE& type) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 902,"ZG\206\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        type = *static_cast<Conversation::TYPE*>(__session->get_cached_prop(18, getOID(), 902, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropLiveHost(Sid::String& live_host) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 918,"ZG\226\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        live_host = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 918, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropLiveStartTimestamp(uint& live_start_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 974,"ZG\316\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        live_start_timestamp = *static_cast<uint*>(__session->get_cached_prop(18, getOID(), 974, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropLiveIsMuted(bool& live_is_muted) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 996,"ZG\344\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        live_is_muted = *static_cast<bool*>(__session->get_cached_prop(18, getOID(), 996, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropAlertString(Sid::String& alert_string) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 920,"ZG\230\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        alert_string = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 920, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropIsBookmarked(bool& is_bookmarked) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 921,"ZG\231\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        is_bookmarked = *static_cast<bool*>(__session->get_cached_prop(18, getOID(), 921, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropGivenDisplayname(Sid::String& given_displayname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 925,"ZG\235\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        given_displayname = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 925, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropDisplayname(Sid::String& displayname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 924,"ZG\234\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        displayname = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 924, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropLocalLivestatus(Conversation::LOCAL_LIVESTATUS& local_livestatus) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 927,"ZG\237\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        local_livestatus = *static_cast<Conversation::LOCAL_LIVESTATUS*>(__session->get_cached_prop(18, getOID(), 927, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropInboxTimestamp(uint& inbox_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 928,"ZG\240\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        inbox_timestamp = *static_cast<uint*>(__session->get_cached_prop(18, getOID(), 928, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropInboxMessageId(MessageRef& inbox_message_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 973,"ZG\315\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        inbox_message_id = *static_cast<MessageRef*>(__session->get_cached_prop(18, getOID(), 973, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropUnconsumedSuppressedMessages(uint& unconsumed_suppressed_messages) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 975,"ZG\317\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        unconsumed_suppressed_messages = *static_cast<uint*>(__session->get_cached_prop(18, getOID(), 975, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropUnconsumedNormalMessages(uint& unconsumed_normal_messages) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 976,"ZG\320\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        unconsumed_normal_messages = *static_cast<uint*>(__session->get_cached_prop(18, getOID(), 976, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropUnconsumedElevatedMessages(uint& unconsumed_elevated_messages) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 977,"ZG\321\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        unconsumed_elevated_messages = *static_cast<uint*>(__session->get_cached_prop(18, getOID(), 977, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropUnconsumedMessagesVoice(bool& unconsumed_messages_voice) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 970,"ZG\312\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        unconsumed_messages_voice = *static_cast<bool*>(__session->get_cached_prop(18, getOID(), 970, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropActiveVmId(VoicemailRef& active_vm_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 971,"ZG\313\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        active_vm_id = *static_cast<VoicemailRef*>(__session->get_cached_prop(18, getOID(), 971, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropConsumptionHorizon(uint& consumption_horizon) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 979,"ZG\323\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        consumption_horizon = *static_cast<uint*>(__session->get_cached_prop(18, getOID(), 979, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropLastActivityTimestamp(uint& last_activity_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 981,"ZG\325\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        last_activity_timestamp = *static_cast<uint*>(__session->get_cached_prop(18, getOID(), 981, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropSpawnedFromConvoId(ConversationRef& spawned_from_convo_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 915,"ZG\223\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        spawned_from_convo_id = *static_cast<ConversationRef*>(__session->get_cached_prop(18, getOID(), 915, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropCreator(Sid::String& creator) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 903,"ZG\207\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        creator = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 903, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropCreationTimestamp(uint& creation_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 904,"ZG\210\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        creation_timestamp = *static_cast<uint*>(__session->get_cached_prop(18, getOID(), 904, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropMyStatus(Conversation::MY_STATUS& my_status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 919,"ZG\227\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        my_status = *static_cast<Conversation::MY_STATUS*>(__session->get_cached_prop(18, getOID(), 919, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropOptJoiningEnabled(bool& opt_joining_enabled) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 922,"ZG\232\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        opt_joining_enabled = *static_cast<bool*>(__session->get_cached_prop(18, getOID(), 922, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropOptEntryLevelRank(Participant::RANK& opt_entry_level_rank) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 906,"ZG\212\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        opt_entry_level_rank = *static_cast<Participant::RANK*>(__session->get_cached_prop(18, getOID(), 906, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropOptDiscloseHistory(bool& opt_disclose_history) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 907,"ZG\213\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        opt_disclose_history = *static_cast<bool*>(__session->get_cached_prop(18, getOID(), 907, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropOptAdminOnlyActivities(Conversation::ALLOWED_ACTIVITY& opt_admin_only_activities) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 909,"ZG\215\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        opt_admin_only_activities = *static_cast<Conversation::ALLOWED_ACTIVITY*>(__session->get_cached_prop(18, getOID(), 909, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropPasswordhint(Sid::String& passwordhint) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 980,"ZG\324\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        passwordhint = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 980, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropMetaName(Sid::String& meta_name) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 910,"ZG\216\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        meta_name = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 910, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropMetaTopic(Sid::String& meta_topic) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 911,"ZG\217\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        meta_topic = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 911, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropMetaGuidelines(Sid::String& meta_guidelines) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 913,"ZG\221\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        meta_guidelines = *static_cast<Sid::String*>(__session->get_cached_prop(18, getOID(), 913, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Conversation::GetPropMetaPicture(Sid::Binary& meta_picture) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 18, getOID(), 914,"ZG\222\007]\022",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        meta_picture = *static_cast<Sid::Binary*>(__session->get_cached_prop(18, getOID(), 914, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropConvoId(ConversationRef& convo_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 960,"ZG\300\007]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        convo_id = *static_cast<ConversationRef*>(__session->get_cached_prop(9, getOID(), 960, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropConvoGuid(Sid::String& convo_guid) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 120,"ZGx]\011",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        convo_guid = *static_cast<Sid::String*>(__session->get_cached_prop(9, getOID(), 120, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropAuthor(Sid::String& author) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 122,"ZGz]\011",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        author = *static_cast<Sid::String*>(__session->get_cached_prop(9, getOID(), 122, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropAuthorDisplayname(Sid::String& author_displayname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 123,"ZG{]\011",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        author_displayname = *static_cast<Sid::String*>(__session->get_cached_prop(9, getOID(), 123, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropGuid(Sid::Binary& guid) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 792,"ZG\230\006]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        guid = *static_cast<Sid::Binary*>(__session->get_cached_prop(9, getOID(), 792, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropOriginallyMeantFor(Sid::String& originally_meant_for) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 790,"ZG\226\006]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        originally_meant_for = *static_cast<Sid::String*>(__session->get_cached_prop(9, getOID(), 790, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropTimestamp(uint& timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 121,"ZGy]\011",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        timestamp = *static_cast<uint*>(__session->get_cached_prop(9, getOID(), 121, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropType(Message::TYPE& type) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 961,"ZG\301\007]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        type = *static_cast<Message::TYPE*>(__session->get_cached_prop(9, getOID(), 961, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropSendingStatus(Message::SENDING_STATUS& sending_status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 962,"ZG\302\007]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        sending_status = *static_cast<Message::SENDING_STATUS*>(__session->get_cached_prop(9, getOID(), 962, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropConsumptionStatus(Message::CONSUMPTION_STATUS& consumption_status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 968,"ZG\310\007]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        consumption_status = *static_cast<Message::CONSUMPTION_STATUS*>(__session->get_cached_prop(9, getOID(), 968, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropEditedBy(Sid::String& edited_by) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 222,"ZG\336\001]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        edited_by = *static_cast<Sid::String*>(__session->get_cached_prop(9, getOID(), 222, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropEditTimestamp(uint& edit_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 223,"ZG\337\001]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        edit_timestamp = *static_cast<uint*>(__session->get_cached_prop(9, getOID(), 223, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropParamKey(uint& param_key) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 963,"ZG\303\007]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        param_key = *static_cast<uint*>(__session->get_cached_prop(9, getOID(), 963, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropParamValue(uint& param_value) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 964,"ZG\304\007]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        param_value = *static_cast<uint*>(__session->get_cached_prop(9, getOID(), 964, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropBodyXml(Sid::String& body_xml) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 127,"ZG\177]\011",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        body_xml = *static_cast<Sid::String*>(__session->get_cached_prop(9, getOID(), 127, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropIdentities(Sid::String& identities) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 125,"ZG}]\011",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        identities = *static_cast<Sid::String*>(__session->get_cached_prop(9, getOID(), 125, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropReason(Sid::String& reason) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 966,"ZG\306\007]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        reason = *static_cast<Sid::String*>(__session->get_cached_prop(9, getOID(), 966, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropLeavereason(LEAVE_REASON& leavereason) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 126,"ZG~]\011",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        leavereason = *static_cast<LEAVE_REASON*>(__session->get_cached_prop(9, getOID(), 126, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Message::GetPropParticipantCount(uint& participant_count) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 9, getOID(), 982,"ZG\326\007]\011",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        participant_count = *static_cast<uint*>(__session->get_cached_prop(9, getOID(), 982, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Video::GetPropStatus(Video::STATUS& status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 11, getOID(), 130,"ZG\202\001]\013",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        status = *static_cast<Video::STATUS*>(__session->get_cached_prop(11, getOID(), 130, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Video::GetPropError(Sid::String& error) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 11, getOID(), 131,"ZG\203\001]\013",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        error = *static_cast<Sid::String*>(__session->get_cached_prop(11, getOID(), 131, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Video::GetPropDebuginfo(Sid::String& debuginfo) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 11, getOID(), 132,"ZG\204\001]\013",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        debuginfo = *static_cast<Sid::String*>(__session->get_cached_prop(11, getOID(), 132, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Video::GetPropDimensions(Sid::String& dimensions) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 11, getOID(), 133,"ZG\205\001]\013",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        dimensions = *static_cast<Sid::String*>(__session->get_cached_prop(11, getOID(), 133, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Video::GetPropMediaType(Video::MEDIATYPE& media_type) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 11, getOID(), 134,"ZG\206\001]\013",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        media_type = *static_cast<Video::MEDIATYPE*>(__session->get_cached_prop(11, getOID(), 134, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Video::GetPropConvoId(uint& convo_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 11, getOID(), 1104,"ZG\320\010]\013",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        convo_id = *static_cast<uint*>(__session->get_cached_prop(11, getOID(), 1104, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Video::GetPropDevicePath(Sid::String& device_path) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 11, getOID(), 1105,"ZG\321\010]\013",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        device_path = *static_cast<Sid::String*>(__session->get_cached_prop(11, getOID(), 1105, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropType(Voicemail::TYPE& type) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 100,"ZGd]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        type = *static_cast<Voicemail::TYPE*>(__session->get_cached_prop(7, getOID(), 100, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropPartnerHandle(Sid::String& partner_handle) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 101,"ZGe]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        partner_handle = *static_cast<Sid::String*>(__session->get_cached_prop(7, getOID(), 101, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropPartnerDispname(Sid::String& partner_dispname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 102,"ZGf]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        partner_dispname = *static_cast<Sid::String*>(__session->get_cached_prop(7, getOID(), 102, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropStatus(Voicemail::STATUS& status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 103,"ZGg]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        status = *static_cast<Voicemail::STATUS*>(__session->get_cached_prop(7, getOID(), 103, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropFailurereason(Voicemail::FAILUREREASON& failurereason) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 104,"ZGh]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        failurereason = *static_cast<Voicemail::FAILUREREASON*>(__session->get_cached_prop(7, getOID(), 104, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropSubject(Sid::String& subject) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 105,"ZGi]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        subject = *static_cast<Sid::String*>(__session->get_cached_prop(7, getOID(), 105, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropTimestamp(uint& timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 106,"ZGj]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        timestamp = *static_cast<uint*>(__session->get_cached_prop(7, getOID(), 106, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropDuration(uint& duration) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 107,"ZGk]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        duration = *static_cast<uint*>(__session->get_cached_prop(7, getOID(), 107, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropAllowedDuration(uint& allowed_duration) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 108,"ZGl]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        allowed_duration = *static_cast<uint*>(__session->get_cached_prop(7, getOID(), 108, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropPlaybackProgress(uint& playback_progress) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 109,"ZGm]\007",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        playback_progress = *static_cast<uint*>(__session->get_cached_prop(7, getOID(), 109, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropConvoId(ConversationRef& convo_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 830,"ZG\276\006]\007",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        convo_id = *static_cast<ConversationRef*>(__session->get_cached_prop(7, getOID(), 830, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Voicemail::GetPropChatmsgGuid(Sid::Binary& chatmsg_guid) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 7, getOID(), 831,"ZG\277\006]\007",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        chatmsg_guid = *static_cast<Sid::Binary*>(__session->get_cached_prop(7, getOID(), 831, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropType(Sms::TYPE& type) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 190,"ZG\276\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        type = *static_cast<Sms::TYPE*>(__session->get_cached_prop(12, getOID(), 190, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropStatus(Sms::STATUS& status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 191,"ZG\277\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        status = *static_cast<Sms::STATUS*>(__session->get_cached_prop(12, getOID(), 191, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropFailurereason(Sms::FAILUREREASON& failurereason) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 192,"ZG\300\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        failurereason = *static_cast<Sms::FAILUREREASON*>(__session->get_cached_prop(12, getOID(), 192, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropIsFailedUnseen(bool& is_failed_unseen) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 48,"ZG0]\014",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        is_failed_unseen = *static_cast<bool*>(__session->get_cached_prop(12, getOID(), 48, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropTimestamp(uint& timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 198,"ZG\306\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        timestamp = *static_cast<uint*>(__session->get_cached_prop(12, getOID(), 198, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropPrice(uint& price) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 193,"ZG\301\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        price = *static_cast<uint*>(__session->get_cached_prop(12, getOID(), 193, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropPricePrecision(uint& price_precision) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 49,"ZG1]\014",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        price_precision = *static_cast<uint*>(__session->get_cached_prop(12, getOID(), 49, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropPriceCurrency(Sid::String& price_currency) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 194,"ZG\302\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        price_currency = *static_cast<Sid::String*>(__session->get_cached_prop(12, getOID(), 194, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropReplyToNumber(Sid::String& reply_to_number) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 199,"ZG\307\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        reply_to_number = *static_cast<Sid::String*>(__session->get_cached_prop(12, getOID(), 199, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropTargetNumbers(Sid::String& target_numbers) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 195,"ZG\303\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        target_numbers = *static_cast<Sid::String*>(__session->get_cached_prop(12, getOID(), 195, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropTargetStatuses(Sid::Binary& target_statuses) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 196,"ZG\304\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        target_statuses = *static_cast<Sid::Binary*>(__session->get_cached_prop(12, getOID(), 196, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropBody(Sid::String& body) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 197,"ZG\305\001]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        body = *static_cast<Sid::String*>(__session->get_cached_prop(12, getOID(), 197, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Sms::GetPropChatmsgId(MessageRef& chatmsg_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 12, getOID(), 840,"ZG\310\006]\014",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        chatmsg_id = *static_cast<MessageRef*>(__session->get_cached_prop(12, getOID(), 840, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropType(Transfer::TYPE& type) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 80,"ZGP]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        type = *static_cast<Transfer::TYPE*>(__session->get_cached_prop(6, getOID(), 80, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropPartnerHandle(Sid::String& partner_handle) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 81,"ZGQ]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        partner_handle = *static_cast<Sid::String*>(__session->get_cached_prop(6, getOID(), 81, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropPartnerDispname(Sid::String& partner_dispname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 82,"ZGR]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        partner_dispname = *static_cast<Sid::String*>(__session->get_cached_prop(6, getOID(), 82, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropStatus(Transfer::STATUS& status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 83,"ZGS]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        status = *static_cast<Transfer::STATUS*>(__session->get_cached_prop(6, getOID(), 83, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropFailurereason(Transfer::FAILUREREASON& failurereason) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 84,"ZGT]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        failurereason = *static_cast<Transfer::FAILUREREASON*>(__session->get_cached_prop(6, getOID(), 84, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropStarttime(uint& starttime) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 85,"ZGU]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        starttime = *static_cast<uint*>(__session->get_cached_prop(6, getOID(), 85, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropFinishtime(uint& finishtime) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 86,"ZGV]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        finishtime = *static_cast<uint*>(__session->get_cached_prop(6, getOID(), 86, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropFilepath(Sid::String& filepath) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 87,"ZGW]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        filepath = *static_cast<Sid::String*>(__session->get_cached_prop(6, getOID(), 87, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropFilename(Sid::String& filename) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 88,"ZGX]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        filename = *static_cast<Sid::String*>(__session->get_cached_prop(6, getOID(), 88, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropFilesize(Sid::String& filesize) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 89,"ZGY]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        filesize = *static_cast<Sid::String*>(__session->get_cached_prop(6, getOID(), 89, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropBytestransferred(Sid::String& bytestransferred) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 90,"ZGZ]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        bytestransferred = *static_cast<Sid::String*>(__session->get_cached_prop(6, getOID(), 90, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropBytespersecond(uint& bytespersecond) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 91,"ZG[]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        bytespersecond = *static_cast<uint*>(__session->get_cached_prop(6, getOID(), 91, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropChatmsgGuid(Sid::Binary& chatmsg_guid) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 92,"ZG\134]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        chatmsg_guid = *static_cast<Sid::Binary*>(__session->get_cached_prop(6, getOID(), 92, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropChatmsgIndex(uint& chatmsg_index) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 93,"ZG]]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        chatmsg_index = *static_cast<uint*>(__session->get_cached_prop(6, getOID(), 93, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Transfer::GetPropConvoId(ConversationRef& convo_id) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 6, getOID(), 98,"ZGb]\006",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        convo_id = *static_cast<ConversationRef*>(__session->get_cached_prop(6, getOID(), 98, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropStatus(Account::STATUS& status) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 70,"ZGF]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        status = *static_cast<Account::STATUS*>(__session->get_cached_prop(5, getOID(), 70, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropPwdchangestatus(Account::PWDCHANGESTATUS& pwdchangestatus) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 71,"ZGG]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        pwdchangestatus = *static_cast<Account::PWDCHANGESTATUS*>(__session->get_cached_prop(5, getOID(), 71, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropLogoutreason(Account::LOGOUTREASON& logoutreason) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 73,"ZGI]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        logoutreason = *static_cast<Account::LOGOUTREASON*>(__session->get_cached_prop(5, getOID(), 73, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropCommitstatus(Account::COMMITSTATUS& commitstatus) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 78,"ZGN]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        commitstatus = *static_cast<Account::COMMITSTATUS*>(__session->get_cached_prop(5, getOID(), 78, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropSuggestedSkypename(Sid::String& suggested_skypename) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 72,"ZGH]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        suggested_skypename = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 72, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropSkypeoutBalanceCurrency(Sid::String& skypeout_balance_currency) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 74,"ZGJ]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        skypeout_balance_currency = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 74, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropSkypeoutBalance(uint& skypeout_balance) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 75,"ZGK]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        skypeout_balance = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 75, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropSkypeoutPrecision(uint& skypeout_precision) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 804,"ZG\244\006]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        skypeout_precision = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 804, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropSkypeinNumbers(Sid::String& skypein_numbers) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 76,"ZGL]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        skypein_numbers = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 76, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropCblsyncstatus(Account::CBLSYNCSTATUS& cblsyncstatus) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 79,"ZGO]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        cblsyncstatus = *static_cast<Account::CBLSYNCSTATUS*>(__session->get_cached_prop(5, getOID(), 79, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropOfflineCallforward(Sid::String& offline_callforward) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 77,"ZGM]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        offline_callforward = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 77, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropChatPolicy(Account::CHATPOLICY& chat_policy) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 160,"ZG\240\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        chat_policy = *static_cast<Account::CHATPOLICY*>(__session->get_cached_prop(5, getOID(), 160, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropSkypeCallPolicy(Account::SKYPECALLPOLICY& skype_call_policy) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 161,"ZG\241\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        skype_call_policy = *static_cast<Account::SKYPECALLPOLICY*>(__session->get_cached_prop(5, getOID(), 161, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropPstnCallPolicy(Account::PSTNCALLPOLICY& pstn_call_policy) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 162,"ZG\242\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        pstn_call_policy = *static_cast<Account::PSTNCALLPOLICY*>(__session->get_cached_prop(5, getOID(), 162, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropAvatarPolicy(Account::AVATARPOLICY& avatar_policy) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 163,"ZG\243\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        avatar_policy = *static_cast<Account::AVATARPOLICY*>(__session->get_cached_prop(5, getOID(), 163, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropBuddycountPolicy(Account::BUDDYCOUNTPOLICY& buddycount_policy) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 164,"ZG\244\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        buddycount_policy = *static_cast<Account::BUDDYCOUNTPOLICY*>(__session->get_cached_prop(5, getOID(), 164, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropTimezonePolicy(Account::TIMEZONEPOLICY& timezone_policy) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 165,"ZG\245\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        timezone_policy = *static_cast<Account::TIMEZONEPOLICY*>(__session->get_cached_prop(5, getOID(), 165, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropWebpresencePolicy(Account::WEBPRESENCEPOLICY& webpresence_policy) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 166,"ZG\246\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        webpresence_policy = *static_cast<Account::WEBPRESENCEPOLICY*>(__session->get_cached_prop(5, getOID(), 166, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropPhonenumbersPolicy(Account::PHONENUMBERSPOLICY& phonenumbers_policy) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 168,"ZG\250\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        phonenumbers_policy = *static_cast<Account::PHONENUMBERSPOLICY*>(__session->get_cached_prop(5, getOID(), 168, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropVoicemailPolicy(Account::VOICEMAILPOLICY& voicemail_policy) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 169,"ZG\251\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        voicemail_policy = *static_cast<Account::VOICEMAILPOLICY*>(__session->get_cached_prop(5, getOID(), 169, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropPartnerOptedout(Sid::String& partner_optedout) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 773,"ZG\205\006]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        partner_optedout = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 773, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropServiceProviderInfo(Sid::String& service_provider_info) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 800,"ZG\240\006]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        service_provider_info = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 800, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropRegistrationTimestamp(uint& registration_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 801,"ZG\241\006]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        registration_timestamp = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 801, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropNrOfOtherInstances(uint& nr_of_other_instances) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 802,"ZG\242\006]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        nr_of_other_instances = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 802, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropSkypename(Sid::String& skypename) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 4,"ZG\004]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        skypename = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 4, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropFullname(Sid::String& fullname) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 5,"ZG\005]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        fullname = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 5, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropBirthday(uint& birthday) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 7,"ZG\007]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        birthday = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 7, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropGender(uint& gender) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 8,"ZG\010]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        gender = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 8, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropLanguages(Sid::String& languages) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 9,"ZG\011]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        languages = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 9, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropCountry(Sid::String& country) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 10,"ZG\012]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        country = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 10, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropProvince(Sid::String& province) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 11,"ZG\013]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        province = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 11, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropCity(Sid::String& city) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 12,"ZG\014]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        city = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 12, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropPhoneHome(Sid::String& phone_home) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 13,"ZG\015]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        phone_home = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 13, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropPhoneOffice(Sid::String& phone_office) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 14,"ZG\016]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        phone_office = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 14, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropPhoneMobile(Sid::String& phone_mobile) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 15,"ZG\017]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        phone_mobile = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 15, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropEmails(Sid::String& emails) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 16,"ZG\020]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        emails = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 16, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropHomepage(Sid::String& homepage) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 17,"ZG\021]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        homepage = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 17, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropAbout(Sid::String& about) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 18,"ZG\022]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        about = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 18, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropProfileTimestamp(uint& profile_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 19,"ZG\023]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        profile_timestamp = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 19, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropMoodText(Sid::String& mood_text) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 26,"ZG\032]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        mood_text = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 26, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropTimezone(uint& time_zone) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 27,"ZG\033]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        time_zone = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 27, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropNrofAuthedBuddies(uint& nrof_authed_buddies) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 28,"ZG\034]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        nrof_authed_buddies = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 28, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropAvailability(Contact::AVAILABILITY& availability) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 34,"ZG\042]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        availability = *static_cast<Contact::AVAILABILITY*>(__session->get_cached_prop(5, getOID(), 34, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropAvatarImage(Sid::Binary& avatar_image) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 37,"ZG%]\005",5)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        avatar_image = *static_cast<Sid::Binary*>(__session->get_cached_prop(5, getOID(), 37, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropAvatarTimestamp(uint& avatar_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 182,"ZG\266\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        avatar_timestamp = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 182, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropMoodTimestamp(uint& mood_timestamp) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 183,"ZG\267\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        mood_timestamp = *static_cast<uint*>(__session->get_cached_prop(5, getOID(), 183, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
        bool Account::GetPropRichMoodText(Sid::String& rich_mood_text) {
                SEClientSession* __session = root;
                Sid::CommandInitiator* __cmdini = __session->get_current_thread();
                uchar kind;
                Sid::Protocol::Status get_status;
                if ((get_status =__session->get_property(__cmdini, 5, getOID(), 205,"ZG\315\001]\005",6)) == Sid::Protocol::OK) {
                        Sid::Mutex::ScopedLock sl(__session->get_cache_lock());
                        rich_mood_text = *static_cast<Sid::String*>(__session->get_cached_prop(5, getOID(), 205, kind));
                }
                return (get_status == Sid::Protocol::OK);
        }
        
#ifdef SE_USE_NAMESPACE
} // Skype
#endif
