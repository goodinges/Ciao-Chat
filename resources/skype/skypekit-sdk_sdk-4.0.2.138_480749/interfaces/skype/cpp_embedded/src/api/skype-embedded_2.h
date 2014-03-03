#include "SidPlatform.hpp"

#ifndef Skype_EMBEDDED_H_INCLUDED
#define Skype_EMBEDDED_H_INCLUDED

#include "sidg_Skyperefs.hpp"
#include "sidg_msgs_Skype.hpp"

#define __EA(a) #a

#ifdef SE_USE_NAMESPACE
namespace Skype
{
#endif

class Skype;
class ContactGroup;
class Contact;
class ContactSearch;
class Participant;
class Conversation;
class Message;
class Video;
class Voicemail;
class Sms;
class Transfer;
class Account;

/*! \class ContactGroup skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  Collects and manages Contacts related by type, status, or some other arbitrary criteria. @brief SkypeKit recognizes
 *  two distinct ContactGroup flavors - "hardwired" and "custom". SkypeKit both defines the criteria for and dynamically
 *  manages all "hardwired" ContactGroups. Individual users explicitly create and manage all "custom" ContactGroups.  @n
 *  @n "Hardwired" groups are primarily organizational tools, for example, they enable you to display a list of all
 *  Contacts awaiting authorization by you. "Custom" groups are also organizational tools, for example, they enable you
 *  to display a list of all Contacts in a particular geographical area or belonging to a particular professional
 *  association, social clubs, and so forth. Primarily, though, "custom" groups are functional tools that enable you to
 *  establish conference calls, group chats, and so forth. @n  @n "Hardwired" ContactGroups are defined for and
 *  available to all users. SkypeKit determines membership in a particular "hardwired" group dynamically whenever a user
 *  invokes Skype.GetHardwiredContactGroup for that group. Subsequent changes to a Contact's status might result in its
 *  being added to (for example, the Contact is now authorized) or removed from (for example, the Contact is now removed
 *  or blocked) one or more "hardwired" groups. @n  @n SkypeKit fires OnChange events for all affected ContractGroup
 *  instances. Essentially all ContactGroup methods related to explicitly adding and removing members and conversations
 *  from the group return false, and CanAdd and CanRemove additionally return a false result. @n  @n "Custom"
 *  ContactGroups can be defined by a particular Skype user through the UI. Your UI should implement Creation, deletion
 *  and filtering contact list by custom contact groups, as well as adding and removing contacts in those groups. @n  @n
 *  A Contact can belong to multiple non-mutually exclusive "hardwired" groups at the same time, for example, an
 *  authorized contact is typically in your "buddy" group, but a Contact cannot belong to CONTACTS_AUTHORIZED_BY_ME if
 *  they are awaiting authorization. Similarly, a Contact can belong to multiple "custom" groups and mutual exclusivity
 *  is typically not an issue. @n 
 */
class ContactGroup : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        ContactGroup(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~ContactGroup();
        typedef ContactGroupRef Ref;
        typedef ContactGroupRefs Refs;
        enum { MODULE_ID = 10 };
        ContactGroupRef ref() { 
                return ContactGroupRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the ContactGroup class */
        enum PROPERTY {
                P_TYPE = 155                    /*!< ContactGroup::TYPE, type: TYPE*/,
                P_CUSTOM_GROUP_ID = 154         /*!< unique 32-bit ID for custom groups, type: uint*/,
                P_GIVEN_DISPLAYNAME = 151       /*!< change via ContactGroup::GiveDisplayname(), type: Sid::String*/,
                P_NROFCONTACTS                  /*!< Number of contacts in the group. NB! The value of this property does not get updated until 5 seconds after account login. During these initial 5 seconds, the value of this property remains 0. The reason for this 5 second delay is to reduce the flurry of property update traffic that occurs during the CBL synchronization phase, following successful login. Note that if you absolutely need to have this value immediately after login, you can still get it by retrieving the contact list with ContactGroup::GetContacts method and examining its size.  @n , type: uint*/,
                P_NROFCONTACTS_ONLINE           /*!< number of contacts online in the group, type: uint*/
        };
        
        /** The list of all possible ContactGroup types. A value of this type can be passed to Skype class GetHardwiredContactGroup to retrieve the relevant ContactGroup object. @n  */
        enum TYPE {
                ALL_KNOWN_CONTACTS = 1        /*!< The superset of all "hardwired" contact groups. @n */,
                ALL_BUDDIES                   /*!< The set of all authorized contacts, that is, contacts that were last the target of Contact::SetBuddyStatus(false) plus all SkypeOut contacts. @n */,
                SKYPE_BUDDIES                 /*!< The set of all authorized Skype contacts (Contact:_SKYPENAME is non-null).  @n Note that this excludes Skype contacts that have either never been the target of Contact::SetBuddyStatus(true) or were last the target of Contactact::SetBuddyStatus(false). @n */,
                SKYPEOUT_BUDDIES              /*!< The set of all SkypeOut contacts (Contact:_PSTNNUMBER is non-null). PSTN contacts can be added to the contact list by retrieving a new contact object with Skype::GetContact, passing in the phone number as string, and then either using Contact::SetBuddyStatus(true) or adding the contact to the SKYPEOUT_BUDDIES group with ContactGroup::AddContact. @n */,
                ONLINE_BUDDIES                /*!< The subset of ALL_BUDDIES that are currently online, including those currently marked as DO_NOT_DISTURBED and AWAY. @n */,
                UNKNOWN_OR_PENDINGAUTH_BUDDIES/*!< The set of all contacts whose Contact:_TYPE reflects UNRECOGNIZED OR have not authorized the local user yet. @n */,
                RECENTLY_CONTACTED_CONTACTS   /*!< This filter returns top 10 most recently contacted contacts, based on Contact::P_LASTUSED_TIMESTAMP property values. This is not configurable. Note that the P_LASTUSED_TIMESTAMP property does not propagate between different Skype instances - thus this filter only works in context of the local database. Recent contacts that were in touch with the user on some other Skype installation will not show up in this filter. @n */,
                CONTACTS_WAITING_MY_AUTHORIZATION/*!< Contacts to whose authorization request the user has not responded yet. The UI should enable the user to accept, decline the authorization request and in case of decline, optionally block further incoming communication from the contact. See: Contact::SetBuddyStatus, Contact::SetBlocked and Contact::IgnoreAuthRequest for more information. @n */,
                CONTACTS_AUTHORIZED_BY_ME     /*!< All contacts authorized by the user. @n */,
                CONTACTS_BLOCKED_BY_ME        /*!< Group of contacts the user has blocked from further incoming communications. If the UI enables contact blocking, it should also provide interface for the user to unblock the blocked contacts. Note that a contact can simultaneously be in both CONTACTS_BLOCKED_BY_ME and CONTACTS_AUTHORIZED_BY_ME groups. @n */,
                UNGROUPED_BUDDIES             /*!< The set of all "buddies" that are not also a member of a custom group. @n */,
                CUSTOM_GROUP                  /*!< A custom group defined by user. @n */,
                PROPOSED_SHARED_GROUP         /*!< The shared contact group functionality is no longer supported. This contact group type can be ignored. @n */,
                SHARED_GROUP                  /*!< The shared contact group functionality is no longer supported. This contact group type can be ignored. @n */,
                EXTERNAL_CONTACTS             /*!< The set of all contacts that were originally imported from an external address book. @n */
        };
        
        /** Setter for ContactGroup class GIVEN_DISPLAYNAME property. @n  */
        bool GiveDisplayName(
                const Sid::String& name
        );
        
        /** Removes the contact group. This is synced across instances logged in with the same account - which can take several minutes for the sync to happen. @n  */
        bool Delete(
                bool& result
        );
        
        /** Returns list of conversations in the ContactGroup. @n  */
        bool GetConversations(
                ConversationRefs& conversations
        );
        
        /** Checks if the current user can add given conversation to the ContactGroup. Returns false for most of the hardwired contact groups for example. @n  */
        bool CanAddConversation(
                bool& result, /*!< Returns true if Conversation can be added to this ContactGroup. @n */
                const ConversationRef conversation = 0 /*!< Conversation to be checked. @n */
        );
        
        /** Adds given conversation to the ContactGroup. @n  */
        bool AddConversation(
                const ConversationRef& conversation
        );
        
        /** Checks if the current user can remove given conversation from the ContactGroup. Again, returns false for most hardwired contact groups. @n  */
        bool CanRemoveConversation(
                bool& result /*!< true if RemoveConversation(contact) works on this group*/
        );
        
        /** Removes given conversation from the ContactGroup. @n  */
        bool RemoveConversation(
                const ConversationRef& conversation
        );
        
        /** conversation added or removed from this group */
        virtual void OnChangeConversation(
                const ConversationRef& conversation
        );
        
        /** Retrieves contact list. @n  */
        bool GetContacts(
                ContactRefs& contacts
        );
        
        /** Checks if the current user can add given contact to the ContactGroup.  @n  */
        bool CanAddContact(
                bool& result, /*!< returns true if AddContact(contact) works on this group. @n */
                const ContactRef contact = 0 /*!< Contact to be checked. @n */
        );
        
        /** Adds contact to a contact group. This only works for non-hardwired contact groups. @n  */
        bool AddContact(
                const ContactRef& contact
        );
        
        /** Checks if the current user can remove given contact from the ContactGroup. @n  */
        bool CanRemoveContact(
                bool& result /*!< true if RemoveContact(contact) works on this group*/
        );
        
        /** Removes contact from the ContactGroup. @n  */
        bool RemoveContact(
                const ContactRef& contact
        );
        
        /** A contact has been added or removed to this ContactGroup. @n NB! On rare occasions, the ContectRef argument to this callback can be NULL. You should always check whether the reference is valid, before accessing methods or properties. @n  */
        virtual void OnChange(
                const ContactRef& contact
        );
        
        /** ContactGroup::P_TYPE property get accessor */
        bool GetPropType(TYPE& type);
        
        /** ContactGroup::P_CUSTOM_GROUP_ID property get accessor */
        bool GetPropCustomGroupId(uint& custom_group_id);
        
        /** ContactGroup::P_GIVEN_DISPLAYNAME property get accessor */
        bool GetPropGivenDisplayname(Sid::String& given_displayname);
        
        /** ContactGroup::P_NROFCONTACTS property get accessor */
        bool GetPropNrofcontacts(uint& nrofcontacts);
        
        /** ContactGroup::P_NROFCONTACTS_ONLINE property get accessor */
        bool GetPropNrofcontactsOnline(uint& nrofcontacts_online);
        
};


/*! \class Contact skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  Address book entry. Encapsulates methods like GetType, GetIdentity, GetAvatar, SendAuthRequest, OpenConversation
 *  etc. Single contact can have additional phone numbers attached to it (ASSIGNED_PHONE1 .. ASSIGNED_PHONE3). Note that
 *  in the context of a conversation, Contacts are represented by Participant objects. @brief Contact member functions
 *  all return a Boolean indicating the success (true) or failure (false) of processing the request itself (transport,
 *  runtime availability, and so forth)?not the success or failure of its associated functionality. For example,
 *  Contact::IsMemberOf returns true if it was able to make a determination, and its result parameter reflects whether
 *  this Contact is a member of the target group. Similarly, Contact::IsMemberOf returns false if it was unable to make
 *  a determination, and the value of its result parameter is undefined. @n 
 */
class Contact : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        Contact(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~Contact();
        typedef ContactRef Ref;
        typedef ContactRefs Refs;
        enum { MODULE_ID = 2 };
        ContactRef ref() { 
                return ContactRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the Contact class */
        enum PROPERTY {
                P_TYPE = 202                    /*!< type: Contact::TYPE*/,
                P_SKYPENAME = 4                 /*!< defined if it is a SKYPE contact, type: Sid::String*/,
                P_PSTNNUMBER = 6                /*!< type: Sid::String*/,
                P_FULLNAME = 5                  /*!< type: Sid::String*/,
                P_BIRTHDAY = 7                  /*!< integer of YYYYMMDD format, type: uint*/,
                P_GENDER                        /*!< 1-male, 2-female, type: uint*/,
                P_LANGUAGES                     /*!< ISO language code list, space separated, type: Sid::String*/,
                P_COUNTRY                       /*!< ISO country code, type: Sid::String*/,
                P_PROVINCE                      /*!< type: Sid::String*/,
                P_CITY                          /*!< type: Sid::String*/,
                P_PHONE_HOME                    /*!< This corresponds to the Account::P_PHONE_HOME property. The value is set by the remote user in Account profile. As Contact property, this is read-only. @n , type: Sid::String*/,
                P_PHONE_OFFICE                  /*!< This corresponds to the Account::P_PHONE_OFFICE property. The value is set by the remote user in Account profile. As Contact property, this is read-only. If the Contact has not populated his Account profile with sufficient phone numbers, the UI should implement locally adding additional phone numbers to Contact P_ASSIGNED_PHONE1 .. P_ASSIGNED_PHONE3 properties (and corresponding labels). See Contact::SetPhoneNumber method for more information. @n , type: Sid::String*/,
                P_PHONE_MOBILE                  /*!< This corresponds to the Account::P_PHONE_MOBILE property. The value is set by the remote user in Account profile. As Contact property, this is read-only. @n , type: Sid::String*/,
                P_EMAILS                        /*!< will be hashed before advertising/querying, space separated, type: Sid::String*/,
                P_HOMEPAGE                      /*!< type: Sid::String*/,
                P_ABOUT                         /*!< arbitrary introductory text, type: Sid::String*/,
                P_AVATAR_IMAGE = 37             /*!< Contact avatar pictures are in JPG format. The original size of avatar pictures are in no more than 96 x 96 pixels in size. However, as they can be smaller, scaling the pictures up too much to fit your UI can lead to distorted images. @n , type: Sid::Binary*/,
                P_MOOD_TEXT = 26                /*!< Personal mood message (visible to authorized users only).  @n , type: Sid::String*/,
                P_RICH_MOOD_TEXT = 205          /*!< XML version of personal mood text, type: Sid::String*/,
                P_TIMEZONE = 27                 /*!< 24*3600+diff_to_UTC_in_seconds. NB! changes with DST @n , type: uint*/,
                P_CAPABILITIES = 36             /*!< binary tag that can be queried via Contact::HasCapability(), type: Sid::Binary*/,
                P_PROFILE_TIMESTAMP = 19        /*!< UNIX timestamp of last profile change, type: uint*/,
                P_NROF_AUTHED_BUDDIES = 28      /*!< count of this user's authorized contacts @n , type: uint*/,
                P_IPCOUNTRY                     /*!< ISO country code assigned by the IP, type: Sid::String*/,
                P_AVATAR_TIMESTAMP = 182        /*!< UNIX timestamp of when current avatar was set, type: uint*/,
                P_MOOD_TIMESTAMP                /*!< NOT SET FOR CONTACTS. For Account object, UNIX timestamp of when current mood was set, type: uint*/,
                P_RECEIVED_AUTHREQUEST = 20     /*!< set if the contact is waiting to be authorized. The value contains auth. request text @n , type: Sid::String*/,
                P_AUTHREQ_TIMESTAMP = 25        /*!< timestamp of last received auth-request, type: uint*/,
                P_LASTONLINE_TIMESTAMP = 35     /*!< X timestamp of last successful ping to that user, type: uint*/,
                P_AVAILABILITY = 34             /*!< Contact::AVAILABILITY, type: AVAILABILITY*/,
                P_DISPLAYNAME = 21              /*!< always set (assigned by lib by looking at various fields), type: Sid::String*/,
                P_REFRESHING                    /*!< true if querying additional information from p2p or server @n , type: bool*/,
                P_GIVEN_AUTHLEVEL               /*!< Contact::AUTHLEVEL, change via Contact::GiveAuthlevel(), type: AUTHLEVEL*/,
                P_GIVEN_DISPLAYNAME = 33        /*!< change via Contact::GiveDisplayname(), type: Sid::String*/,
                P_ASSIGNED_COMMENT = 180        /*!< change via Contact::AssignComment(), type: Sid::String*/,
                P_LASTUSED_TIMESTAMP = 39       /*!< UNIX timestamp of last outbound session (call, chat, FT, etc), type: uint*/,
                P_AUTHREQUEST_COUNT = 41        /*!< for contacts that have CONTACT_RECEIVED_AUTHREQUEST, how many times in a row they have requested it without positive answer, type: uint*/,
                P_ASSIGNED_PHONE1 = 184         /*!< Office phone no. of the contact. This property should NOT be used for SkypeOut contacts @n , type: Sid::String*/,
                P_ASSIGNED_PHONE1_LABEL         /*!< Value "1" in this property will be translated as "Office" by Skype Windows desktop client, according to UI language settings @n , type: Sid::String*/,
                P_ASSIGNED_PHONE2               /*!< Mobile phone no. of the contact @n , type: Sid::String*/,
                P_ASSIGNED_PHONE2_LABEL         /*!< Value "2" in this property will be translated as "Mobile" by Skype Windows desktop client, according to UI language settings @n , type: Sid::String*/,
                P_ASSIGNED_PHONE3               /*!< "Other phone no. of the contact, type: Sid::String*/,
                P_ASSIGNED_PHONE3_LABEL         /*!< Value "3" in this property will be translated as "Other" by Skype Windows desktop client, according to UI language settings @n , type: Sid::String*/,
                P_POPULARITY_ORD = 42           /*!< Contact's order by presence popularity @n , type: uint*/
        };
        
        /** Same as with CAPABILITY, enumerator is used by both Contact and Account objects. @n  */
        enum TYPE {
                UNRECOGNIZED = 0              /*!< Contact/account has no pre-identified type. This type is reported by default for SkypeKit clients. @n */,
                SKYPE                         /*!< Normal Skype contact. @n */,
                PSTN                          /*!< Normal PSTN contact. @n */,
                EMERGENCY_PSTN                /*!< Emergency number (i.e. 911). @n */,
                FREE_PSTN,
                UNDISCLOSED_PSTN              /*!< Undisclosed PSTN number. @n */,
                EXTERNAL                      /*!< This type is currently used by Windows desktop clients for contacts imported from Outlook. @n */
        };
        
        /** Getter method for the TYPE property. @n  */
        bool GetType(
                TYPE& type
        );
        
        /** Describes the recognized relational states between a local account and a remote contact. @n  */
        enum AUTHLEVEL {
                NONE = 0                      /*!< Authorization request is either ignored or pending. In this state several functionalities may be blocked, depending on settings. For example, accounts may only allow seeing online presence to be viewable or only receive calls from authorized contacts. @n */,
                AUTHORIZED_BY_ME              /*!< Contact has been authorized by the local account. @n */,
                BLOCKED_BY_ME                 /*!< Contact has been blocked by the local account. This prevents incoming calls, chat messages, additional authorization requests etc. @n */
        };
        
        /** Describes the superset list of possible Account and Contact online statuses. In case of Account they apply to local user, in case of Contact they apply to remote contacts. @n  */
        enum AVAILABILITY {
                UNKNOWN = 0                   /*!< Contact online status cannot be determined. This availability state should not normally reach the SkypeKit UI level. @n */,
                PENDINGAUTH = 8               /*!< Seeing Contact online status is blocked because authorization between contact and local account has not taken place. @n */,
                BLOCKED                       /*!< Remote contact has been blocked by local account. This applies to online accounts. @n */,
                BLOCKED_SKYPEOUT = 11         /*!< Remote SkypeOut contact has been blocked by local account. @n */,
                SKYPEOUT = 10                 /*!< Contact does not have an online status because he is a PSTN contact. @n */,
                OFFLINE = 1                   /*!< Contact appears to be offline. @n */,
                OFFLINE_BUT_VM_ABLE = 12      /*!< Contact appears to be offline but has voicemail enabled. @n */,
                OFFLINE_BUT_CF_ABLE           /*!< Contact appears to be offline but has enabled call forwarding, so calls may actually get through to him. @n */,
                ONLINE = 2                    /*!< Contact / Account is online @n */,
                AWAY                          /*!< Contact / Account is online but away from keyboard. This can be either turned on manually or by automatic timer. In Windows desktop client, the timer can be configured with minute precision. @n */,
                NOT_AVAILABLE                 /*!< This online status is marked as deprecated. If a remote contact indicates its status as NOT_AVAILABLE, the UI should handle this as equivalent of AWAY status. @n */,
                DO_NOT_DISTURB                /*!< Contact / Account is online but does not wish to be disturbed. This status supersedes AWAY status when the account is DO_NOT_DISTURB the AWAY timer should not modify the status. @n */,
                SKYPE_ME = 7                  /*!< This online status is marked as deprecated. If a remote contact indicates its status as SKYPE_ME, the UI should handle this as equivalent of ONLINE status. @n */,
                INVISIBLE = 6                 /*!< Account status is set to INVISIBLE. This status in not applicable to remote Contacts. When the remote contact has set his availability to INVISIBLE, he will appear as OFFLINE to others. @n */,
                CONNECTING = 14               /*!< only possible for local user/account*/,
                ONLINE_FROM_MOBILE,
                AWAY_FROM_MOBILE              /*!< *_FROM_MOBILE only possible for remote user*/,
                NOT_AVAILABLE_FROM_MOBILE,
                DO_NOT_DISTURB_FROM_MOBILE,
                SKYPE_ME_FROM_MOBILE = 20
        };
        
        bool GetIdentity(
                Sid::String& identity /*!< returns CONTACT_SKYPENAME or CONTACT_PSTNNUMBER value*/
        );
        
        /** Returns Conrtact's avatar image (JPG). @n  */
        bool GetAvatar(
                bool& present, /*!< @li true: the Contact has a custom avatar image @li false: the Contact does not have a custom avatar image @n */
                Sid::Binary& avatar /*!< The avatar image data (JPG). If present is false, this will be the Skype-assigned default avatar @n */
        );
        
        /** returns verified-by-Skype e-mail for this contact if exists and verifiable */
        bool GetVerifiedEmail(
                Sid::String& email
        );
        
        /** returns verified-by-Skype company for this contact if exists and verifiable */
        bool GetVerifiedCompany(
                Sid::String& company
        );
        
        /** Checks whether the contact is member of a contact group given in group reference argument. @n  */
        bool IsMemberOf(
                const ContactGroupRef& group, /*!< The target contact group @n */
                bool& result /*!< @li true: the Contact is a member of the target contact group @li false: the Contact is not a member of the target contact group @n */
        );
        
        /** Checks whether the contact is member of a pre-defined contact group given in the TYPE argument (type for this property comes from the ContactGroup class). @n  */
        bool IsMemberOfHardwiredGroup(
                const ContactGroup::TYPE& groupType, /*!< The type designator of the target pre-defined contact group. For example, specify this parameter as ContactGroup::TYPE.RECENTLY_CONTACTED_CONTACTS to determine if you've had a recent conversation with this Contact. @n */
                bool& result /*!< @li true: the Contact is a member of the target contact group @li false: the Contact is not a member of the target contact group @n */
        );
        
        /** Blocks or unblocks any further incoming communication attempts from this contact. @n  */
        bool SetBlocked(
                const bool& blocked, /*!< @li true: block this contact @li false: unblock this contact @n */
                const bool abuse = false /*!< Optional parameter to report abuse by this Skype user when blocking this Contact. Note that you can specifiy this parameter as true only when blocking a Contact. Defaults to false if omitted. @n */
        );
        
        /** Rejects and removes a pending authorization request from this Contact. @n  */
        bool IgnoreAuthRequest(
        );
        
        /** sets CONTACT_GIVEN_DISPLAYNAME. clears if size(name)==0 */
        bool GiveDisplayName(
                const Sid::String& name
        );
        
        /** Adds or removes this Contact from the ALL_BUDDIES hardwired group. @n  */
        bool SetBuddyStatus(
                const bool& isMyBuddy, /*!< @li true: add this contact to the ALL_BUDDIES group @li false: delete contact from the ALL_BUDDIES group @n */
                const bool syncAuth = true /*!< This argument is deprecated and should not be used. @n */
        );
        
        enum EXTRA_AUTHREQ_FIELDS {
                SEND_VERIFIED_EMAIL = 1       /*!< send verified e-mail blob with this auth request*/,
                SEND_VERIFIED_COMPANY         /*!< send verified company blob with this auth request*/
        };
        
        /** Sends a contact authorization request to this user. @n  */
        bool SendAuthRequest(
                const Sid::String& message, /*!< Text that typically introduces the requesting user and details the reason for the authorization request. This text will be set as Message::P_BODY_XML property of the notification Message the remote user will receive upon getting the authortization request. While this can be the empty string, it cannot be null. @n */
                const uint extras_bitmask = 0 /*!< Indicates additional information to include with this authorization request: @li 0 (zero): do not include any additional information @li SEND_VERIFIED_EMAIL: include the requestor's verified e-mail address @li SEND_VERIFIED_COMPANY: include verified information regarding the requestor's company @li SEND_VERIFIED_EMAIL + SEND_VERIFIED_COMPANY: include both e-mail and company information @n */
        );
        
        /** the contact has accepted my auth request */
        bool HasAuthorizedMe(
                bool& result
        );
        
        /** Sets the three P_ASSIGNED_PHONEx and P_ASSIGNED_PHONEx_LABEL properties, where x reflects the value of num.  @n  @n The Skype Windows desktop client uses the following conventions when interpreting and assigning values to these properties. While your solution can assign arbitrary values to these properties, we strongly recommend using these conventions to ensure interoperability with the Skype Windows desktop client. Keep in mind that the "number" of these property pairs has no relationship to how the Skype Windows client interprets their label property value strings. For example, the Skype Windows client will interpret P_ASSIGNED_PHONE3_LABEL as "Home" if its value is the string "0".  @n  @n Label strings: @n  @n Populate the label properties with string representations of the numbers "0" through "3", rather than descriptive strings like "Home", "Mobile", and so forth. The Skype desktop clients interpret the numeric string values as: @li "0" (zero) - "Home" @li "1" (one) - "Office" @li "2" - "Mobile" @li "3" - "Other" @n  @n Keep in mind that the "number" of a property pair has no relationship to its label string. For example, the Skype Windows client will interpret P_ASSIGNED_PHONE3_LABEL as "Home" if its value is the string "0". @n  @n Phone number strings: @n  @n The Skype Windows desktop client has distinct conventions for Skype Contacts and PSTN (SkypeOut) Contacts: any or all of the property pairs can be used for Skype Contacts; P_ASSIGNED_PHONE1 cannot be used for PSTN Contacts and P_ASSIGNED_PHONE1_LABEL has special meaning for PSTN Contacts. @n Specifically, the Skype desktop clients use P_ASSIGNED_PHONE1_LABEL as the label for a PSTN Contact's primary number (regardless of whether it's home, mobile, or office), and use P_PSTNNUMBER to hold the actual number. @n  @n  */
        bool SetPhoneNumber(
                const uint& num, /*!< The property pair being set, which must be in the range 0..3 @n */
                const Sid::String& label, /*!< The label text for the property being set @n */
                const Sid::String& number /*!< The phone number for the property being set @n */
        );
        
        /** Retrieves a dialog conversation with the Contact. @n  */
        bool OpenConversation(
                ConversationRef& conversation /*!< Retrieved dialog. @n */
        );
        
        /** This enumerator is used by both Contact and Account objects. Thus the items here can have slightly different meaning, depending on which context you will examine their values. In case of Contact, the values apply to a user - across all the instances that user has logged in with Skype. In case of Account, the capability is that of a local, currently logged in instance of Skype client. @n  @n The values that CAPABILITY items can have are also dependant on class context. In context of Contact, a capability can be CAPABILITY_MIXED. Which in case of CAPABILITY_VIDEO, for example, would mean that the remote use has logged in with different clients, some of which support video calls and some of which don't. In context of Account - there are no mixed result. Currently logged in Skype instance either supports video or it doesn't. @n  */
        enum CAPABILITY {
                CAPABILITY_VOICEMAIL = 0      /*!< For Account object, this is the same as CAPABILITY_CAN_BE_SENT_VM - it indicates that the currently logged in Skype instance supports voicemails. For Contact objects, it means that their remote system supports sending voicemails - there is no technical method for Skype to detect whether they are capable of receiving voicemails, so the assumption is that they can. @n */,
                CAPABILITY_SKYPEOUT           /*!< Indicates that the contact/account has SkypeOut and is thus capable of making PSTN calls. @n */,
                CAPABILITY_SKYPEIN            /*!< Indicates that the contact/account has SkypeIn and is thus capable of answering PSTN calls. @n */,
                CAPABILITY_CAN_BE_SENT_VM     /*!< For contacts, this is a combination of CAPABILITY_VOICEMAIL for local account (local Skype client supports sending voicemails) and CAPABILITY_VOICEMAIL of the Contact - if the contact supports sending voicemails then hopefully they can also receive them. @n */,
                CAPABILITY_CALL_FORWARD       /*!< Indicates that Account/Contact supports call forwarding. @n */,
                CAPABILITY_VIDEO              /*!< Indicates that Account/Contact supports call video calls. @n */,
                CAPABILITY_TEXT               /*!< In context of Contact, this indicates that the user is noticed running at least one Skype implementation that supports text messaging. Basically, it applies to a user. When applied to Account, the meaning is slightly different. In that case it indicates that currently running Skype implementation supports chat messaging. So, for Account objects, this is a node (rather than user) capability. @n */,
                CAPABILITY_SERVICE_PROVIDER   /*!< Indicates that the contact/account is flagged as SkypePrime service provider. This is linked to Account class SERVICE_PROVIDER_INFO property. @n */,
                CAPABILITY_LARGE_CONFERENCE   /*!< This is a legacy item, from the old times when conference calls with more than 5 people were limited to SkypePro accounts. In other words, this item is no longer relevant and will likely be removed at some point in future. @n */,
                CAPABILITY_COMMERCIAL_CONTACT,
                CAPABILITY_PSTN_TRANSFER      /*!< Indicates that Account/Contact supports call transfers to PSTN numbers. @n */,
                CAPABILITY_TEXT_EVER          /*!< Indicates that the user has had his chat capability removed by Skype. Basically, this means that the user is reported as spammer too many times. This applies for both Contact and Account objects - which means your client can check locally, if the currently logged in user has been marked as a spammer. @n */,
                CAPABILITY_VOICE_EVER         /*!< Indicates that the user (Account or Contact) has had his voice call capability removed by Skype. @n */,
                CAPABILITY_MOBILE_DEVICE      /*!< Indicates that the instance of Skype client Account/Contact is or in Contact's case has at least occasionally been flagged as a mobile device. @n */,
                CAPABILITY_PUBLIC_CONTACT
        };
        
        /** List of possible states of each of the Contact class CAPABILITY items. @n  */
        enum CAPABILITYSTATUS {
                NO_CAPABILITY = 0             /*!< Contact does not have the capability @n */,
                CAPABILITY_MIXED              /*!< Contact has occasionally logged in with Skype client that supports the capability. For example, a contact may have Skype client on several machines, only some of which have webcam - in which case CAPABILITY_VIDEO would have its value set as CAPABILITY_MIXED. @n */,
                CAPABILITY_EXISTS             /*!< Contact has the capability @n */
        };
        
        /** Retrieves a Contact::Capability value. Sets P_REFRESHING to true while querying from server. This method is functionally the same as Contact::GetCapabilityStatus except that it returns a bool value rather than Contact::CAPABILITYSTATUS @n  */
        bool HasCapability(
                const CAPABILITY& capability, /*!< The target capability @n */
                bool& result, /*!< @li true: the Contact has the target capability through at least one of the Skype clients they have logged into. Corresponds to CAPABILITY_MIXED and CAPABILITY_EXISTS @li false: the Contact does not have the target capability. Corresponds to NO_CAPABILITY @n */
                const bool queryServer = false /*!< @li true: obtains the data from the p2p network/server, and sets P_REFRESHING to true for the duration  @li false: obtains the data from the local client @n */
        );
        
        /** Retrieves a Contact::Capability value. Sets P_REFRESHING to true while querying from server. @n  */
        bool GetCapabilityStatus(
                const CAPABILITY& capability, /*!< The target capability, see Contact::Capability enumerator. @n */
                CAPABILITYSTATUS& status, /*!< Status of the target capability. @n */
                const bool queryServer = false /*!< @li true: obtains the data from the p2p network/server, and sets P_REFRESHING to true for the duration  @li false: obtains the data from the local client @n */
        );
        
        /** Refreshes all properties, capabilities, and statuses associated with this Contact from p2p/CBL, and sets P_REFRESHING to true for the duration. @n  */
        bool RefreshProfile(
        );
        
        /** Contact::P_TYPE property get accessor */
        bool GetPropType(Contact::TYPE& type);
        
        /** Contact::P_SKYPENAME property get accessor */
        bool GetPropSkypename(Sid::String& skypename);
        
        /** Contact::P_PSTNNUMBER property get accessor */
        bool GetPropPstnnumber(Sid::String& pstnnumber);
        
        /** Contact::P_FULLNAME property get accessor */
        bool GetPropFullname(Sid::String& fullname);
        
        /** Contact::P_BIRTHDAY property get accessor */
        bool GetPropBirthday(uint& birthday);
        
        /** Contact::P_GENDER property get accessor */
        bool GetPropGender(uint& gender);
        
        /** Contact::P_LANGUAGES property get accessor */
        bool GetPropLanguages(Sid::String& languages);
        
        /** Contact::P_COUNTRY property get accessor */
        bool GetPropCountry(Sid::String& country);
        
        /** Contact::P_PROVINCE property get accessor */
        bool GetPropProvince(Sid::String& province);
        
        /** Contact::P_CITY property get accessor */
        bool GetPropCity(Sid::String& city);
        
        /** Contact::P_PHONE_HOME property get accessor */
        bool GetPropPhoneHome(Sid::String& phone_home);
        
        /** Contact::P_PHONE_OFFICE property get accessor */
        bool GetPropPhoneOffice(Sid::String& phone_office);
        
        /** Contact::P_PHONE_MOBILE property get accessor */
        bool GetPropPhoneMobile(Sid::String& phone_mobile);
        
        /** Contact::P_EMAILS property get accessor */
        bool GetPropEmails(Sid::String& emails);
        
        /** Contact::P_HOMEPAGE property get accessor */
        bool GetPropHomepage(Sid::String& homepage);
        
        /** Contact::P_ABOUT property get accessor */
        bool GetPropAbout(Sid::String& about);
        
        /** Contact::P_AVATAR_IMAGE property get accessor */
        bool GetPropAvatarImage(Sid::Binary& avatar_image);
        
        /** Contact::P_MOOD_TEXT property get accessor */
        bool GetPropMoodText(Sid::String& mood_text);
        
        /** Contact::P_RICH_MOOD_TEXT property get accessor */
        bool GetPropRichMoodText(Sid::String& rich_mood_text);
        
        /** Contact::P_TIMEZONE property get accessor */
        bool GetPropTimezone(uint& timezone);
        
        /** Contact::P_CAPABILITIES property get accessor */
        bool GetPropCapabilities(Sid::Binary& capabilities);
        
        /** Contact::P_PROFILE_TIMESTAMP property get accessor */
        bool GetPropProfileTimestamp(uint& profile_timestamp);
        
        /** Contact::P_NROF_AUTHED_BUDDIES property get accessor */
        bool GetPropNrofAuthedBuddies(uint& nrof_authed_buddies);
        
        /** Contact::P_IPCOUNTRY property get accessor */
        bool GetPropIpcountry(Sid::String& ipcountry);
        
        /** Contact::P_AVATAR_TIMESTAMP property get accessor */
        bool GetPropAvatarTimestamp(uint& avatar_timestamp);
        
        /** Contact::P_MOOD_TIMESTAMP property get accessor */
        bool GetPropMoodTimestamp(uint& mood_timestamp);
        
        /** Contact::P_RECEIVED_AUTHREQUEST property get accessor */
        bool GetPropReceivedAuthrequest(Sid::String& received_authrequest);
        
        /** Contact::P_AUTHREQ_TIMESTAMP property get accessor */
        bool GetPropAuthreqTimestamp(uint& authreq_timestamp);
        
        /** Contact::P_LASTONLINE_TIMESTAMP property get accessor */
        bool GetPropLastonlineTimestamp(uint& lastonline_timestamp);
        
        /** Contact::P_AVAILABILITY property get accessor */
        bool GetPropAvailability(AVAILABILITY& availability);
        
        /** Contact::P_DISPLAYNAME property get accessor */
        bool GetPropDisplayname(Sid::String& displayname);
        
        /** Contact::P_REFRESHING property get accessor */
        bool GetPropRefreshing(bool& refreshing);
        
        /** Contact::P_GIVEN_AUTHLEVEL property get accessor */
        bool GetPropGivenAuthlevel(AUTHLEVEL& given_authlevel);
        
        /** Contact::P_GIVEN_DISPLAYNAME property get accessor */
        bool GetPropGivenDisplayname(Sid::String& given_displayname);
        
        /** Contact::P_ASSIGNED_COMMENT property get accessor */
        bool GetPropAssignedComment(Sid::String& assigned_comment);
        
        /** Contact::P_LASTUSED_TIMESTAMP property get accessor */
        bool GetPropLastusedTimestamp(uint& lastused_timestamp);
        
        /** Contact::P_AUTHREQUEST_COUNT property get accessor */
        bool GetPropAuthrequestCount(uint& authrequest_count);
        
        /** Contact::P_ASSIGNED_PHONE1 property get accessor */
        bool GetPropAssignedPhone1(Sid::String& assigned_phone1);
        
        /** Contact::P_ASSIGNED_PHONE1_LABEL property get accessor */
        bool GetPropAssignedPhone1Label(Sid::String& assigned_phone1_label);
        
        /** Contact::P_ASSIGNED_PHONE2 property get accessor */
        bool GetPropAssignedPhone2(Sid::String& assigned_phone2);
        
        /** Contact::P_ASSIGNED_PHONE2_LABEL property get accessor */
        bool GetPropAssignedPhone2Label(Sid::String& assigned_phone2_label);
        
        /** Contact::P_ASSIGNED_PHONE3 property get accessor */
        bool GetPropAssignedPhone3(Sid::String& assigned_phone3);
        
        /** Contact::P_ASSIGNED_PHONE3_LABEL property get accessor */
        bool GetPropAssignedPhone3Label(Sid::String& assigned_phone3_label);
        
        /** Contact::P_POPULARITY_ORD property get accessor */
        bool GetPropPopularityOrd(uint& popularity_ord);
        
};


/*! \class ContactSearch skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  This class encapsulates functionality for looking up contacts on the Skype network. Contacts can be searched by
 *  portion of their name, e-mail address, language preferences, etc. @brief @n  @n Contact search is asynchronous.
 *  ContactSearch::Submit is a non-blocking function that initiates the search. Upon finding a matching contact,
 *  ContactSearch::OnNewResult event gets fired, that gives you the reference to the discovered contact. You can get up
 *  to 100 matching contacts per search. Note that you will need to keep a live reference of the ContactSearch object
 *  while the search is doing its work.  @n  @n So, to perform a contact search:  @li create a contact search object @li
 *  specify search terms and conditions @li submit search @li in ContactSearch::OnNewResult callback, update your UI @li
 *  in ContactSearch::OnChange, check for terminal values of P_CONTACT_SEARCH_STATUS and update the UI accordingly. @n
 *  @n When the search has done its job, the ContactSearch::P_CONTACT_SEARCH_STATUS property will go to one of the
 *  terminal values. @n  @n The terminal values are: @li FINISHED - the search has stopped. Note that this does not mean
 *  any matches were actually found. @li FAILED - the search has failed. @li EXTENDABLE - this state should be
 *  considered the same as FINISHED. The feature of extending long search results is about to be deprecated. It is still
 *  possible for search objects to occasionally reach that state, so it should be handled in the UI (as FINISHED), but
 *  the extending feature itself should not be implemented in your UI. @n  @n There are three methods to create the
 *  ContactSearch objects. @n  @n A) Skype::CreateIdentitySearch @n  @n This method takes a string argument and looks
 *  for exact matches against Contact::P_SKYPENAME property. So for example, identity search for "echo" will return 0
 *  results and search for "echo123" will return exactly one.  @n  @n Identity in this case means skypename - contact
 *  search does not work with PSTN type contacts. However, it does work for SKYPE type contacts that have supplied
 *  P_PHONE_HOME, P_PHONE_OFFICE or P_PHONE_MOBILE values in their account data. To search for those, you will need to
 *  use complex search (see below). @n  @n Note that you should always check for boolean return value of the
 *  CreateIdentitySearch method. If the user submits a string that is not a valid skypename, the method will return
 *  false and the ContactSearchRef argument will return as NULL. @n  @n B) Skype::CreateBasicContactSearch @n  @n This
 *  method takes a string argument and looks for non-exact matches against both P_SKYPENAME and P_FULLNAME properties of
 *  the contact. If you intend to implement a simple, one-input search feature - this is the best method for you. The
 *  non-exact matching operates similarly to the SQL LIKE condition. @n  @n C) Skype::CreateContactSearch @n  @n This
 *  method enables you to implement advanced contact search, matching against multiple seach criteria. It takes no input
 *  arguments and expects search criteria to be added to the already constructed search object. @n  @n Criteria can be
 *  added with ContactSearch::AddStrTerm and ContactSearch::AddIntTerm methods. @n  @n These methods take Contact class
 *  porperty ID, condition, and the match pattern as inputs. @n  @n Only the following Contact properties can be used
 *  for search: @li P_SKYPENAME  @li P_FULLNAME @li P_BIRTHDAY (uint) @li P_GENDER (uint: 1-male, 2-female) @li
 *  P_LANGUAGES @li P_COUNTRY @li P_PROVINCE @li P_CITY @li P_PHONE_HOME @li P_PHONE_OFFICE @li P_PHONE_MOBILE @li
 *  P_EMAILS @li P_HOMEPAGE @li P_ABOUT @n  @n String searches are case insensitive, i.e. search for echo123 also
 *  matches ECHO123 @n  @n When adding multiple criteria, default behaviour is that the criterions are additive. I.e. a
 *  term skypename == "joe" followed by term country == "us" will result in intersection between all joes and everybody
 *  in US. @n  @n You can explicitly add an "OR" instead of "AND" between conditions, using the AddOr method. @n  @n By
 *  default, AND criteria are grouped together, before OR's, so that: @n  @n AddTerm(condition1) @n AddTerm(condition2)
 *  @n AddOr() @n AddTerm(condition3) @n AddTerm(condition4) @n  @n will result in the following logical statement: @n
 *  (condition1 AND condition2) OR (condition3 AND condition4) @n  @n However, you can add "global" critera, by using
 *  the add_to_subs argument of the AddXX methods. @n  @n AddTerm(condition1) @n AddTerm(condition2) @n AddOr() @n
 *  AddTerm(condition3) @n AddTerm(condition4, add_to_subs=true) @n  @n which would result in: @n (condition1 AND
 *  condition2 AND condition4) OR (condition3 AND condition4) @n  @n  @n Every one of the contact properties can only be
 *  used once, per search. For example, you cannot create a search for two different P_FULLNAME patterns. The &valid
 *  argument will still return tue if you do this, but the last criteria for any given property will override all
 *  previous ones. So, a search like this: @n  @n cs->AddStrTerm(Contact::P_FULLNAME, ContactSearch::EQ, "John Smith",
 *  isValid); @n cs->AddOr(); @n cs->AddStrTerm(Contact::P_FULLNAME, ContactSearch::EQ, "Ivan Sidorov", isValid); @n  @n
 *  will only return matches for "Ivan Sidorov" and none for "John Smith". @n  @n Some of the contact properties are
 *  automatically combined for purposes of search. @n  @n A search for P_SKYPENAME also returns matches from the
 *  P_FULLNAME property and vice versa. @n  @n So that this: @n cs->AddStrTerm(Contact::P_SKYPENAME, ContactSearch::EQ,
 *  "john.smith", isValid); @n  @n ..and this: @n cs->AddStrTerm(Contact::P_FULLNAME, ContactSearch::EQ, "John Smith",
 *  isValid); @n  @n ..and this: @n cs->AddStrTerm(Contact::P_SKYPENAME, ContactSearch::EQ, "john.smith", isValid); @n
 *  cs->AddOr(); @n cs->AddStrTerm(Contact::P_FULLNAME, ContactSearch::EQ, "John Smith", isValid); @n  @n ..all search
 *  from both the P_FULLNAME and P_SKYPENAME fields. @n  @n  @n Before using ContactGroup::Submit to start the search,
 *  you should always check whether the search criteria ended up being valid. This you can do with
 *  ContactSearch::IsValid method. @n  @n As you probably noticed, each of the AddXX methods also return a validity
 *  check boolean. However, it is a better practice to do the overall check as well, even if all the individual search
 *  criteria ended up looking Ok. @n  @n For example, lets take a search for contact's e-mail. This can be done with two
 *  different methods. Firstly we can use the ContactSearch::AddEmailTerm method. This method will actually validate
 *  whether the input is a valid e-mail address: @n  @n cs->AddEmailTerm ("test@test@test", isValid);  @n will return
 *  the isValid argument as false. @n  @n However, you can also add the e-mail search criterion as a simple string, like
 *  this: @n  @n cs->AddStrTerm(Contact::P_EMAILS, ContactSearch::EQ, "test@test@test@", isValid); @n in which case the
 *  isValid will return true. @n  @n However, if you then check entire search object with:  @n  @n cs->IsValid(isValid);
 *  @n  @n the isValid will correctly return false. @n 
 */
class ContactSearch : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        ContactSearch(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~ContactSearch();
        typedef ContactSearchRef Ref;
        typedef ContactSearchRefs Refs;
        enum { MODULE_ID = 1 };
        ContactSearchRef ref() { 
                return ContactSearchRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the ContactSearch class */
        enum PROPERTY {
                P_CONTACT_SEARCH_STATUS = 200   /*!< type: STATUS*/
        };
        
        /** Possible values for the ContactSearch.P_STATUS property. @n  */
        enum STATUS {
                CONSTRUCTION = 1              /*!< Transient state, obtained after submission and actually initiating the search on the network. @n */,
                PENDING                       /*!< Waiting for results to show up. This is a transient state. @n */,
                EXTENDABLE                    /*!< Enough matches are found. No more OnNewResult events will fire. The feature of extending long search results is about to be deprecated. It is still possible for search objects to occasionally reach that state, so it should be handled in the UI (as FINISHED), but the extending feature itself should not be implemented in your UI.  @n */,
                FINISHED                      /*!< The search is finished. No more matches are expected. This is a terminal state. @n */,
                FAILED                        /*!< ContactSearch failed. Better check if the search terms made any sense, with ContactSearch::IsValid. This is a terminal state. @n */
        };
        
        /** construct CONTACT_BIRTHDAY term based on current time */
        bool AddMinAgeTerm(
                const uint& min_age_in_years,
                bool& valid,
                const bool add_to_subs = false
        );
        
        /** construct CONTACT_BIRTHDAY term based on current time */
        bool AddMaxAgeTerm(
                const uint& max_age_in_years,
                bool& valid,
                const bool add_to_subs = false
        );
        
        /** Adds a search term against Contact::P_EMAILS property and pre-validates the value given in the email argument. @n  */
        bool AddEmailTerm(
                const Sid::String& email, /*!< e-mail addres to search for. @n */
                bool& valid, /*!< Returns false if the value in email property did not look like a valid email address. @n */
                const bool add_to_subs = false /*!< This argument enables you to group conditions. See ContactSearch class details for more information. @n */
        );
        
        bool AddLanguageTerm(
                const Sid::String& language,
                bool& valid,
                const bool add_to_subs = false
        );
        
        /** List of available  matching conditions that can be used in AddTerm methods. @n  */
        enum CONDITION {
                EQ = 0                        /*!< Equals @n */,
                GT                            /*!< Is greater than @n */,
                GE                            /*!< Is greater or equal. @n */,
                LT                            /*!< Is less than @n */,
                LE                            /*!< Less or equal @n */,
                PREFIX_EQ                     /*!< Start of a word macthes exactly (string properties only). @n */,
                PREFIX_GE                     /*!< Start of a word is greater or equal (string properties only). @n */,
                PREFIX_LE                     /*!< Start of a word is less or equal (string properties only). @n */,
                CONTAINS_WORDS                /*!< Contains the word (string properties only). @n */,
                CONTAINS_WORD_PREFIXES        /*!< One of the words starts with searched value (string properties only). @n */
        };
        
        /** Adds a string search term to a custom contact search object.  @n  */
        bool AddStrTerm(
                const int& prop, /*!< Following Contact class string propkeys can be used for Contact search: @li Contact::P_SKYPENAME @li Contact::P_FULLNAME @li Contact::P_LANGUAGES @li Contact::P_COUNTRY @li Contact::P_PROVINCE @li Contact::P_CITY @li Contact::P_PHONE_HOME @li Contact::P_PHONE_OFFICE @li Contact::P_PHONE_MOBILE @li Contact::P_EMAILS @li Contact::P_HOMEPAGE @li Contact::P_ABOUT @n Note that while Contact::P_EMAILS is technically a string and can be used in this method, it is recommended that you use ContactSearch.AddEmailTerm method instead. @n */
                const CONDITION& cond, /*!< Search condition (ContactSearch::CONDITION) @n */
                const Sid::String& value, /*!< Value to match against. @n */
                bool& valid, /*!< Returns true if the ContactSearch term-set remains valid after adding this term. @n */
                const bool add_to_subs = false /*!< This argument enables you to group conditions. See ContactSearch class details for more information. @n */
        );
        
        /** Adds a search term to a custom contact search object. For now, there are only two searchable Contact properties that are integers, so this can oly be used for Contact::P_BIRTHDAY and Contact::P_GENDER. @n  */
        bool AddIntTerm(
                const int& prop, /*!< Propkey to search for. Either Contact::P_BIRTHDAY or Contact::P_GENDER @n */
                const CONDITION& cond, /*!< Search condition (ContactSearch::CONDITION) @n */
                const uint& value, /*!< Value to match against. @n */
                bool& valid, /*!< Returns true if the ContactSearch term-set remains valid after adding this term. @n */
                const bool add_to_subs = false /*!< This argument enables you to group conditions. See ContactSearch class details for more information. @n */
        );
        
        /** used to group terms (AddTerm(1), AddTerm(2), Or(), AddTerm(3), AddTerm(4), etc) */
        bool AddOr(
        );
        
        /** checks that terms list is non-empty and does not contain unsupported keys */
        bool IsValid(
                bool& result
        );
        
        /** launch search */
        bool Submit(
        );
        
        /** extend if search is EXTENDABLE */
        bool Extend(
        );
        
        /** releases results that are not referenced from elsewhere */
        bool Release(
        );
        
        /** result list is dynamically updated */
        bool GetResults(
                ContactRefs& contacts,
                const uint from = 0,
                const uint count = MAX_UINT
        );
        
        /** This callback is fired when a new matching contact has been found during the search. @n  */
        virtual void OnNewResult(
                const ContactRef& contact,
                const uint& rankValue
        );
        
        /** ContactSearch::P_CONTACT_SEARCH_STATUS property get accessor */
        bool GetPropContactSearchStatus(STATUS& contact_search_status);
        
};

/** sync failure reasons when starting a transfer */
enum TRANSFER_SENDFILE_ERROR {
        TRANSFER_OPEN_SUCCESS = 0,
        TRANSFER_BAD_FILENAME,
        TRANSFER_OPEN_FAILED,
        TRANSFER_TOO_MANY_PARALLEL
};

/** Conversation and live state leave reasons for Participants. This type is used for the Participant::P_LAST_LEAVEREASON property. The primary use of this type is to provide detailed cause in case of a call failure. @n  */
enum LEAVE_REASON {
        LEAVE_REASON_NONE = 0,
        RETIRED_USER_INCAPABLE = 2    /*!< automatic, user cannot chat (only some older versions might set this)*/,
        RETIRED_ADDER_MUST_BE_FRIEND  /*!< automatic*/,
        RETIRED_ADDER_MUST_BE_AUTHORIZED/*!< automatic*/,
        RETIRED_DECLINE_ADD           /*!< manual reason (afaik no UI uses this)*/,
        RETIRED_UNSUBSCRIBE           /*!< manual reason*/,
        LIVE_NO_ANSWER = 100,
        LIVE_MANUAL                   /*!< live: User hung up*/,
        LIVE_BUSY,
        LIVE_CONNECTION_DROPPED,
        LIVE_NO_SKYPEOUT_SUBSCRIPTION,
        LIVE_INSUFFICIENT_FUNDS,
        LIVE_INTERNET_CONNECTION_LOST,
        LIVE_SKYPEOUT_ACCOUNT_BLOCKED,
        LIVE_PSTN_COULD_NOT_CONNECT_TO_SKYPE_PROXY,
        LIVE_PSTN_INVALID_NUMBER,
        LIVE_PSTN_NUMBER_FORBIDDEN,
        LIVE_PSTN_CALL_TIMED_OUT,
        LIVE_PSTN_BUSY,
        LIVE_PSTN_CALL_TERMINATED,
        LIVE_PSTN_NETWORK_ERROR,
        LIVE_NUMBER_UNAVAILABLE,
        LIVE_PSTN_CALL_REJECTED,
        LIVE_PSTN_MISC_ERROR,
        LIVE_INTERNAL_ERROR,
        LIVE_UNABLE_TO_CONNECT,
        LIVE_RECORDING_FAILED         /*!< live: Voicemail recording failed*/,
        LIVE_PLAYBACK_ERROR           /*!< live: Voicemail playback failed*/,
        LIVE_LEGACY_ERROR,
        LIVE_BLOCKED_BY_PRIVACY_SETTINGS,
        LIVE_ERROR                    /*!< live: Fallback error*/,
        LIVE_TRANSFER_FAILED,
        LIVE_TRANSFER_INSUFFICIENT_FUNDS,
        LIVE_BLOCKED_BY_US,
        LIVE_EMERGENCY_CALL_DENIED
};


/*! \class Participant skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  Conversation participant class. Instances of this class represent contacts when in the context of conversations.
 *  Amongst others, this class has a Ring method for requesting live status with the target contact. This class also
 *  holds typing indicator property and access rights for the contact in context of public conversations. @n 
 */
class Participant : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        Participant(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~Participant();
        typedef ParticipantRef Ref;
        typedef ParticipantRefs Refs;
        enum { MODULE_ID = 19 };
        ParticipantRef ref() { 
                return ParticipantRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the Participant class */
        enum PROPERTY {
                P_CONVO_ID = 930                /*!< [ALL] ID of corresponding conversation. Here and below, [ALL] tag indicates that the property has meaning and is set in context of all participants in the conversation. [OTHERS] tag has meaning only for participants who are not the local user. @n , type: ConversationRef*/,
                P_IDENTITY                      /*!< [ALL] skypename OR pstn_number OR namespace:identity, type: Sid::String*/,
                P_RANK                          /*!< [ALL] Participant::RANK, type: RANK*/,
                P_REQUESTED_RANK                /*!< Not set (should be: requested Participant::RANK, higher than the current one), type: RANK*/,
                P_TEXT_STATUS                   /*!< [ALL] the typing indicator, type: TEXT_STATUS*/,
                P_VOICE_STATUS                  /*!< [ALL] voice status, type: VOICE_STATUS*/,
                P_VIDEO_STATUS                  /*!< [ALL] video status, type: VIDEO_STATUS*/,
                P_LIVE_IDENTITY = 943           /*!< [ALL] identity that was used to establish current live session with that participant (can be different from participant identity), type: Sid::String*/,
                P_LIVE_PRICE_FOR_ME = 938       /*!< [OTHERS] 'price_per_minute_float currency' - eg '0.01 EUR'. Note that this property obtains value only after the participant goes into live state. It cannot be used to display call rates before the call starts. @n , type: Sid::String*/,
                P_LIVE_FWD_IDENTITIES = 948     /*!< [OTHERS] list of identities where the live session is being forwarded (if they are disclosed), space separated, type: Sid::String*/,
                P_LIVE_START_TIMESTAMP = 939    /*!< [ALL] time of joining the live session, type: uint*/,
                P_SOUND_LEVEL = 941             /*!< [ALL] current 'loudness' level when SPEAKING (0..10), type: uint*/,
                P_DEBUGINFO                     /*!< [OTHERS] call (audio and video) debug info, type: Sid::String*/,
                P_LAST_VOICE_ERROR = 947        /*!< [OTHERS] DEPRECATED, use last_leavereason instead, type: Sid::String*/,
                P_QUALITY_PROBLEMS = 949        /*!< [ALL] space separated tokens values: CPU_INUSE CPU_SLOW CPU_HIGH HIGH_ECHO HIGH_NOISE MUTED_INPUT LOW_INPUT MUTED_INPUT_ACTIVITY FW_STRONG FW_BAD NOT_UDP CALL_BW_LOW RECORD_ERROR + values in video debug info, type: Sid::String*/,
                P_LIVE_TYPE                     /*!< [ALL] participant type during livesession as specified in IDENTITYTYPE, type: contact::IDENTITYTYPE*/,
                P_LIVE_COUNTRY                  /*!< [OTHERS] participant livesession country code - used for emergency calls only atm, type: Sid::String*/,
                P_TRANSFERRED_BY                /*!< [OTHERS] Transferor identity (transferee side) @n , type: Sid::String*/,
                P_TRANSFERRED_TO                /*!< [OTHERS] Identity of recipient of transfer (transferor side, caller side) @n , type: Sid::String*/,
                P_ADDER                         /*!< [ALL] Identity of the user who added this participant to the conversation, type: Sid::String  @n , type: Sid::String*/,
                P_LAST_LEAVEREASON              /*!< [OTHERS] Last reason for leaving conversation or live session. Provides an enum alternative to last_voice_error as well as the reason this participant RETIRED from the conversation.  @n , type: LEAVE_REASON*/
        };
        
        /** Recognized values for the P_RANK property. The P_RANK controls participant's privileges in the conversation. See Participant::CanSetRankTo and Participant::SetRankTo methods. @n  */
        enum RANK {
                CREATOR = 1                   /*!< Creator of the chat. There can be only one participant with this type per conversation. Other participants cannot be promoted to Creator rank. @n */,
                ADMIN                         /*!< Participant who has administrator privileges @n */,
                SPEAKER                       /*!< Participant who can speak and write @n */,
                WRITER                        /*!< Participant who can write but not speak @n */,
                SPECTATOR                     /*!< Participant who can read but not write/speak @n */,
                APPLICANT                     /*!< Participant who is applying to join the conversation. Member cannot be demoted to applicants once they have been accepted.  @n */,
                RETIRED                       /*!< Participant who has eft or has been kicked from the conversation @n */,
                OUTLAW                        /*!< Participant who has been banned from the chat @n */
        };
        
        /** Recognized values for the P_TEXT_STATUS property. The P_TEXT_STATUS property has two uses. Firstly, you can use it to implement typing indicators in your UI, to notify the local user that an incoming chat message from this Participant is imminent.  @n  @n To set the P_TEXT_STATUS value, so that remote client UIs can display the local user's typing indicator in their UI, use Conversation::SetMyTextStatusTo method. @n  @n Transmission of P_TEXT_STATUS updates to remote participants of conversations is controlled via SETUPKEY_DISABLE_CHAT_ACTIVITY_INDICATION setup key. @n  @n Secondly, the TEXT_NA value enables you to detect participants who are running clients with no chat capability. @n  */
        enum TEXT_STATUS {
                TEXT_UNKNOWN = 0              /*!< Fallback state in case the text status is not (yet) deternmined. @n */,
                TEXT_NA                       /*!< Text status is not applicable as the participant is using a Skype client that does not support chat (for example: voice-only USB phones). @n */,
                READING                       /*!< Participant is currently not typing. @n */,
                WRITING                       /*!< Participant is currently typing. @n */,
                WRITING_AS_ANGRY              /*!< This state should be set when following two conditions are true: @li interval between keypresses are less than 20 ms; @li at least one of the keys adjacent to current key are pressed down. @n */,
                WRITING_AS_CAT                /*!< The "Cat on keyboard detection" algorthm in Skype is implemented in the UI level, and as such is not present in the SkypeKit API. Should you wish to implement similar algorthm in your own UI, you can get the basic logic from the PawSense FAQ - http://www.bitboost.com/pawsense/pawsense-faq.html @n */
        };
        
        /** Recognized values for the P_VOICE_STATUS property.  @n  */
        enum VOICE_STATUS {
                VOICE_UNKNOWN = 0             /*!< Participant voice status is not (yet) determined. @n */,
                VOICE_NA                      /*!< Participant is using a Skype client with no audio capability. @n */,
                VOICE_AVAILABLE               /*!< Participant is using a Skype client that supports audio. @n */,
                VOICE_CONNECTING              /*!< Participant is in process of joining current live session. This is a transitional state. @n */,
                RINGING                       /*!< Participant has been invited to join the live session but has not yet accepted. @n */,
                EARLY_MEDIA                   /*!< Participant is in process of joining current live session. This is another transitional state. @n */,
                LISTENING                     /*!< Participant has joined the current live session but is currently not transmitting audio. @n */,
                SPEAKING                      /*!< Participant has joined the current live session and is transmitting audio. The UI can either use this state to display appropriate "speaking" notification, or alternatively use Participant::P_SOUND_LEVEL if you want your speaking indicator to also reflect audio volume coming from the Participant. @n */,
                VOICE_ON_HOLD                 /*!< Participant has joined the current live session but the audio is currently on hold. @n */,
                VOICE_STOPPED                 /*!< Participant will be placed in this state for some seconds after live session has finished. This is another transitional state. @n */
        };
        
        /** Recognized values for the P_VIDEO_STATUS property. This property applies to Participant's video send capability, not capability to receive video. @n  */
        enum VIDEO_STATUS {
                VIDEO_UNKNOWN = 0             /*!< Video status is not (yet) determined. @n */,
                VIDEO_NA                      /*!< Indicates that this Participant does not have video available.. @n */,
                VIDEO_AVAILABLE               /*!< Indicates that video is available for this participant. When the Participant::P_VIDEO_STATUS obtains this state, it is possible to retrieve the Video object, using Participant::GetVideo method. Further operations, such as starting or stopping the video send/receive will then go through the Video object. @n */,
                VIDEO_CONNECTING              /*!< Transitional state indicating that the Participant is attempting to initiate video send. @n */,
                STREAMING                     /*!< Indicates that the participant is currently sending video. @n */,
                VIDEO_ON_HOLD                 /*!< Indicates that the participant video send is currently paused. @n */
        };
        
        enum DTMF {
                DTMF_0 = 0,
                DTMF_1,
                DTMF_2,
                DTMF_3,
                DTMF_4,
                DTMF_5,
                DTMF_6,
                DTMF_7,
                DTMF_8,
                DTMF_9,
                DTMF_STAR,
                DTMF_POUND
        };
        
        /** Checks whether the current user can set this Participant's conversation privileges to the specified RANK. This enables you to gray out or disable in your UI all the unavailable options for Participant::SetRankTo method. @n  */
        bool CanSetRankTo(
                const RANK& rank, /*!< Participant::RANK value to check for. @n */
                bool& result /*!< Returns true if local user can set participant's rank to the value given in rank argument. @n */
        );
        
        /** Sets Participant's conversation privileges to the given RANK @n  */
        bool SetRankTo(
                const RANK& rank /*!< Target Participant::RANK value. @n */
        );
        
        /** Initiates live conversation attempt with Participant. @n  */
        bool Ring(
                const Sid::String identityToUse = "", /*!< Ring an alternate identity, such as a PSTN number. @n */
                const bool videoCall = false, /*!< Enable video. @n */
                const uint nrofRedials = 0, /*!< Unused. @n */
                const uint redialPeriod = 0, /*!< Unused. @n */
                const bool autoStartVM = false, /*!< Unused. On dialog, if falling on VM, greeting and recording will be automatically started. @n */
                const Sid::String origin = "" /*!< When call is initiated from web link, must contain the URI that was used*/
        );
        
        /** Rings this participant, using P_LIVE_IDENTITY property if set. @n  */
        bool RingIt(
        );
        
        /** Sets LIVE_IDENTITY property, an alternate identity to use when ringing, such as a PSTN. @n  */
        bool SetLiveIdentityToUse(
                const Sid::String identityToUse = "" /*!< Empty string will reset it to default, i.e IDENTITY property value @n */
        );
        
        /** Retrieves a reference to the Video object that corresponds to the Participant. It can be either local video - you can check if this participant's name (P_IDENTITY property) matches the name of the currently logged in account (P_SKYPENAME property) or incoming video from a remote participant.  @n  @n Note that for GetVideo to be successful, the video has to be available for that participant. This can be checked for by examining Participant VIDEO_STATUS property - once it becomes VIDEO_AVAILABLE - you can use GetVideo to obtain the Video object. @n  */
        bool GetVideo(
                VideoRef& video /*!< Returns reference to a constructed video object. @n */
        );
        
        /** Removes this participant from the current live session. Note that this does not remove the participant from conversation (for this, use Participant::Retire). It only removes participant from live state. @n  */
        bool Hangup(
        );
        
        /** Forcibly removes this participant from conversation. This method is for removing other people from conversations (for example: as administrative punishment for flooding conversation with spam messages). For local user to leave a conversation, use Conversation::RetireFrom instead. @n  */
        bool Retire(
        );
        
        /** This event gets fired on receiving a DTMF signal sent by Participant. Note that this event will only fire if the Participant is also using a Skype client. Skype audio library does not monitor incoming voice streams for dial tones. DTMF events are propagated to remote participants via data channel. Incoming DTMF tones transmitted from, for example, mobile phones, will not cause this event to fire. In case of incoming DTMF signals from Skype clients, DTMF tones are also inserted into the audio stream. You don't have to inject those into local audio playback yourself. @n 
        * @param dtmf Returns Participant::DTMF value. @n 
        */
        virtual void OnIncomingDTMF(
                const DTMF& dtmf
        );
        
        /** Participant::P_CONVO_ID property get accessor */
        bool GetPropConvoId(ConversationRef& convo_id);
        
        /** Participant::P_IDENTITY property get accessor */
        bool GetPropIdentity(Sid::String& identity);
        
        /** Participant::P_RANK property get accessor */
        bool GetPropRank(RANK& rank);
        
        /** Participant::P_REQUESTED_RANK property get accessor */
        bool GetPropRequestedRank(RANK& requested_rank);
        
        /** Participant::P_TEXT_STATUS property get accessor */
        bool GetPropTextStatus(TEXT_STATUS& text_status);
        
        /** Participant::P_VOICE_STATUS property get accessor */
        bool GetPropVoiceStatus(VOICE_STATUS& voice_status);
        
        /** Participant::P_VIDEO_STATUS property get accessor */
        bool GetPropVideoStatus(VIDEO_STATUS& video_status);
        
        /** Participant::P_LIVE_IDENTITY property get accessor */
        bool GetPropLiveIdentity(Sid::String& live_identity);
        
        /** Participant::P_LIVE_PRICE_FOR_ME property get accessor */
        bool GetPropLivePriceForMe(Sid::String& live_price_for_me);
        
        /** Participant::P_LIVE_FWD_IDENTITIES property get accessor */
        bool GetPropLiveFwdIdentities(Sid::String& live_fwd_identities);
        
        /** Participant::P_LIVE_START_TIMESTAMP property get accessor */
        bool GetPropLiveStartTimestamp(uint& live_start_timestamp);
        
        /** Participant::P_SOUND_LEVEL property get accessor */
        bool GetPropSoundLevel(uint& sound_level);
        
        /** Participant::P_DEBUGINFO property get accessor */
        bool GetPropDebuginfo(Sid::String& debuginfo);
        
        /** Participant::P_LAST_VOICE_ERROR property get accessor */
        bool GetPropLastVoiceError(Sid::String& last_voice_error);
        
        /** Participant::P_QUALITY_PROBLEMS property get accessor */
        bool GetPropQualityProblems(Sid::String& quality_problems);
        
        /** Participant::P_LIVE_TYPE property get accessor */
        bool GetPropLiveType(uint& live_type);
        
        /** Participant::P_LIVE_COUNTRY property get accessor */
        bool GetPropLiveCountry(Sid::String& live_country);
        
        /** Participant::P_TRANSFERRED_BY property get accessor */
        bool GetPropTransferredBy(Sid::String& transferred_by);
        
        /** Participant::P_TRANSFERRED_TO property get accessor */
        bool GetPropTransferredTo(Sid::String& transferred_to);
        
        /** Participant::P_ADDER property get accessor */
        bool GetPropAdder(Sid::String& adder);
        
        /** Participant::P_LAST_LEAVEREASON property get accessor */
        bool GetPropLastLeavereason(LEAVE_REASON& last_leavereason);
        
};


/*! \class Conversation skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  The Conversation class encapsulates all types of communication possible with Skype client. Instant messaging, calls,
 *  video calls, file transfers, SMS, screen sharing - all take place within the context of a Conversation. Contacts are
 *  represented in Conversation as Participant objects. This also applies to contacts of PSTN type. All events in a
 *  conversation are represented as Message objects.  @n 
 */
class Conversation : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        Conversation(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~Conversation();
        typedef ConversationRef Ref;
        typedef ConversationRefs Refs;
        enum { MODULE_ID = 18 };
        ConversationRef ref() { 
                return ConversationRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the Conversation class */
        enum PROPERTY {
                P_IDENTITY = 972                /*!< contact identity in case of dialogs, chat name in case of conferences, type: Sid::String*/,
                P_TYPE = 902                    /*!< type of the conversation, type: Conversation::TYPE*/,
                P_LIVE_HOST = 918               /*!< host of current live session. none => no session. myself in case of 1:1 calls, type: Sid::String*/,
                P_LIVE_START_TIMESTAMP = 974    /*!< moment when first participant other than host joined the current or last live session, type: uint*/,
                P_LIVE_IS_MUTED = 996           /*!< if live session is muted, type: bool*/,
                P_ALERT_STRING = 920            /*!< '' everything matches, '=' nothing matches, '=string' string matches, type: Sid::String*/,
                P_IS_BOOKMARKED                 /*!< if conversation is bookmarked/flagged, type: bool*/,
                P_GIVEN_DISPLAYNAME = 925       /*!< name assigned via GiveDisplayname, type: Sid::String*/,
                P_DISPLAYNAME = 924             /*!< resulting display name of the conversation (based on given name, topic, participant list, etc), type: Sid::String*/,
                P_LOCAL_LIVESTATUS = 927        /*!< if the conversation is live and in which status it is then, type: LOCAL_LIVESTATUS*/,
                P_INBOX_TIMESTAMP               /*!< timestamp to sort the conversations in inbox by. 0 means not in inbox, type: uint*/,
                P_INBOX_MESSAGE_ID = 973        /*!< ID of the message that caused INBOX_TIMESTAMP to be set, type: MessageRef*/,
                P_UNCONSUMED_SUPPRESSED_MESSAGES = 975/*!< number of messages in UNCONSUMED_SUPPRESSED consumption status, type: uint*/,
                P_UNCONSUMED_NORMAL_MESSAGES    /*!< number of messages in UNCONSUMED_NORMAL consumption status, type: uint*/,
                P_UNCONSUMED_ELEVATED_MESSAGES  /*!< DEPRECATED, not set anymore, type: uint*/,
                P_UNCONSUMED_MESSAGES_VOICE = 970/*!< if there are unconsumed voice or call messages in the conversation, type: bool*/,
                P_ACTIVE_VM_ID                  /*!< ID of voice message that is being played or recorded in this conversation, type: VoicemailRef*/,
                P_CONSUMPTION_HORIZON = 979     /*!< consumption cutoff timestamp: messages after (but not including) this are considered unconsumed, type: uint*/,
                P_LAST_ACTIVITY_TIMESTAMP = 981 /*!< timestamp of last activity in conversation, type: uint*/,
                P_SPAWNED_FROM_CONVO_ID = 915   /*!< dialog this conference was spawned from, type: ConversationRef*/,
                P_CREATOR = 903                 /*!< identity of conversation creator (doesn't apply to dialogs), type: Sid::String*/,
                P_CREATION_TIMESTAMP            /*!< timestamp of creation, tells you how far you can retrieve messages, type: uint*/,
                P_MY_STATUS = 919               /*!< my status in this conversation (connecting, participating, retired, etc) (doesn't apply to dialogs), type: MY_STATUS*/,
                P_OPT_JOINING_ENABLED = 922     /*!< if it's a public conversation (doesn't apply to dialogs), type: bool*/,
                P_OPT_ENTRY_LEVEL_RANK = 906    /*!< rank that is auto-assigned at join (doesn't apply to dialogs), type: Participant::RANK*/,
                P_OPT_DISCLOSE_HISTORY          /*!< if history visible to new consumers (doesn't apply to dialogs), type: bool*/,
                P_OPT_ADMIN_ONLY_ACTIVITIES = 909/*!< activities that only ADMIN can do. Bitmap of ALLOWED_ACTIVITY values (doesn't apply to dialogs), type: ALLOWED_ACTIVITY*/,
                P_PASSWORDHINT = 980            /*!< public conversation password hint, use SetPassword to set (doesn't apply to dialogs), type: Sid::String*/,
                P_META_NAME = 910               /*!< user-set name, separate from topic (doesn't apply to dialogs), type: Sid::String*/,
                P_META_TOPIC                    /*!< conversation topic (doesn't apply to dialogs), type: Sid::String*/,
                P_META_GUIDELINES = 913         /*!< guidelines (doesn't apply to dialogs), type: Sid::String*/,
                P_META_PICTURE                  /*!< conversation picture, in jpeg format (doesn't apply to dialogs), type: Sid::Binary*/
        };
        
        /** Setupkey SETUPKEY_ENABLE_BIRTHDAY_NOTIFICATION type:int default value:"1" @n Enables/disables birthday notification messages. @li 0 - disable;  @li 1 - enable; @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_ENABLE_BIRTHDAY_NOTIFICATION "Lib/Conversation/EnableBirthday"
        
        /** Setupkey SETUPKEY_INBOX_UPDATE_TIMEOUT type:int  @n Timeout in seconds, how old the Conversation::P_INBOX_TIMESTAMP has to be for it to be re-sorted in the inbox. @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_INBOX_UPDATE_TIMEOUT "Lib/Conversation/InboxUpdateTimeout"
        
        /** Setupkey SETUPKEY_RECENTLY_LIVE_TIMEOUT type:int default value:"20" @n The number of seconds a formerly live conversation will remain the Conversation::LIVE_CONVERSATIONS filter. Note that while the conversation remains in Conversation::LIVE_CONVERSATIONS filter, Skype::OnConversationListChange events will not fire if there is another call coming up within the same conversation. Seeting this key to 0 will cause conversations to exit the Conversation::LIVE_CONVERSATIONS list immediately, after live state drops. @n  @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_RECENTLY_LIVE_TIMEOUT "Lib/Conversation/RecentlyLiveTimeout"
        
        /** Setupkey SETUPKEY_DISABLE_CHAT type:int  Disables chat (for voice only clients). @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLE_CHAT "Lib/Chat/DisableChat"
        
        /** Setupkey SETUPKEY_DISABLE_CHAT_HISTORY type:int  @n Disables storage of chat history. @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_DISABLE_CHAT_HISTORY "Lib/Message/DisableHistory"
        
        /** Setupkey SETUPKEY_CHAT_HISTORY_DAYS type:int  @n Time limit for keeping local chat message history. @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_CHAT_HISTORY_DAYS "Lib/Chat/HistoryDays"
        
        /** Setupkey SETUPKEY_CHATDB_LIMIT_KB type:int default value:"0" Use this key to limit the size of the chat db. Value is in KB. By default there is no limit. A minimum of 16 MB is recommended. */
        #define SETUPKEY_CHATDB_LIMIT_KB "Lib/Chat/ChatDBLimitKb"
        
        /** Setupkey SETUPKEY_DISABLE_CHAT_ACTIVITY_INDICATION type:int  @n Enables/disables transmitting typing indicator signals to othe participants of conversations. @li 0 - disable;  @li 1 - enable; @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_DISABLE_CHAT_ACTIVITY_INDICATION "Lib/Chat/DisableActivityIndication"
        
        /** Setupkey SETUPKEY_CALL_NOANSWER_TIMEOUT type:int default value:"15" @n Timeout in seconds after which the incoming live session will stop ringing (and if possible, proceed to voicemail or call forward). @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_CALL_NOANSWER_TIMEOUT "Lib/Call/NoAnswerTimeout"
        
        /** Setupkey SETUPKEY_CALL_SEND_TO_VM type:int  @n Autoforwarding of incoming calls to voicemail. @li 0 - off @li 1 - on @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_CALL_SEND_TO_VM "Lib/Call/SendToVM"
        
        /** Setupkey SETUPKEY_CALL_APPLY_CF type:int  @n Enables/disables call forwarding. @li 0 - disable;  @li 1 - enable; @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_CALL_APPLY_CF "Lib/Call/ApplyCF"
        
        /** Setupkey SETUPKEY_CALL_EMERGENCY_COUNTRY type:string  @n Country code for emergency calls @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_CALL_EMERGENCY_COUNTRY "Lib/Call/EmergencyCountry"
        
        enum TYPE {
                DIALOG = 1                    /*!< 1:1 conversations, there is a one dialog per identity*/,
                CONFERENCE                    /*!< equivalent of a multichat*/,
                TERMINATED_CONFERENCE         /*!< a conference that has been terminated (disbanded chat)*/,
                LEGACY_VOICE_CONFERENCE       /*!< voice-only conference, when host is using a legacy non-conversation client*/,
                LEGACY_SHAREDGROUP            /*!< chat used for legacy shared groups, can be ignored*/
        };
        
        enum MY_STATUS {
                CONNECTING = 1                /*!< connecting to conference*/,
                RETRY_CONNECTING,
                DOWNLOADING_MESSAGES          /*!< unused*/,
                QUEUED_TO_ENTER               /*!< conference is full for now, being queued*/,
                APPLICANT                     /*!< I'm applying to join the conference*/,
                APPLICATION_DENIED            /*!< My application to join the conference was denied*/,
                INVALID_ACCESS_TOKEN          /*!< The password I provided is incorrect*/,
                CONSUMER                      /*!< I'm part of the conference, I can participate*/,
                RETIRED_FORCEFULLY            /*!< I was kicked from the conference*/,
                RETIRED_VOLUNTARILY           /*!< I left the conference*/
        };
        
        enum LOCAL_LIVESTATUS {
                NONE = 0                      /*!< there isn't a live session*/,
                STARTING                      /*!< trying to start or join a live session*/,
                RINGING_FOR_ME                /*!< there is a live session ringing*/,
                IM_LIVE                       /*!< the conference is live for me*/,
                ON_HOLD_LOCALLY = 5           /*!< I put the live session on hold*/,
                ON_HOLD_REMOTELY              /*!< the live session was put on hold by someone else*/,
                OTHERS_ARE_LIVE               /*!< there is a live session on-going, I'm not participating but I could join*/,
                OTHERS_ARE_LIVE_FULL = 11     /*!< there is a live session on-going without me, but I can't join because it's full*/,
                PLAYING_VOICE_MESSAGE = 8     /*!< playing a voicemail (dialog only)*/,
                RECORDING_VOICE_MESSAGE       /*!< recording a voicemail (dialog only)*/,
                RECENTLY_LIVE                 /*!< a live session just finished, we stay in this state for RECENTLY_LIVE_TIMEOUT setup key*/,
                TRANSFERRING = 12             /*!< call is being transferred*/
        };
        
        /** values for opt_admin_only_activities property */
        enum ALLOWED_ACTIVITY {
                SET_META = 1                  /*!< allowed to set the CONVERSATION_META properties*/,
                ADD_CONSUMERS                 /*!< allowed to add participants to the conference*/,
                SPEAK = 4                     /*!< allowed to speak, but not write*/,
                SPEAK_AND_WRITE = 8           /*!< allowed to speak and write*/
        };
        
        /** Setter method for Conversation option properties. Option properties are all Conversation properties starting with OPT_ prefix. @n  */
        bool SetOption(
                const int& propKey, /*!< Conversation property key, for example: Conversation::OPT_JOINING_ENABLED @n */
                const uint& value /*!< New value for the option property. @n */
        );
        
        /** Setter for Conversation class META_TOPIC. This topic will be set for remote participants as well. @n  */
        bool SetTopic(
                const Sid::String& topic, /*!< New conversation topic. @n */
                const bool isXML = false /*!< Notifies remote UIs that the new topic contains xml tags. @n */
        );
        
        /** Setter for Conversation META_GUIDELINES. This property will be visible to remote participants of the conversation. @n  */
        bool SetGuidelines(
                const Sid::String& guidelines, /*!< New value for the META_GUIDELINES property. @n */
                const bool isXML = false /*!< Set true to notify remote UIs that the new guideline contains XML tags. @n */
        );
        
        /** Sets the conversation's avatar to the specified JPEG image, which is propagated to both local and remote participants. Before calling this method, you should use Skype::ValidateAvatar to verify that jpeg references a valid JPEG image. @n  */
        bool SetPicture(
                const Sid::Binary& jpeg /*!< Conversation avatar binary. @n */
        );
        
        /** When called from dialog conversation, this spawns a new conversation, with existing two dialog participants plus new contact identities given in the identitiesToAdd list. You do not need to add existing dialog participants to the string list. In fact, passing only the existing participants in the identities list will cause the method call to fail (return false), the same as if the list was empty. This method will also return false if the original conversation was not a dialog (contained more than two participants). Also note that this method always creates a new Conversation - even if a conversation with exactly the same participant list existed before. @n  */
        bool SpawnConference(
                const Sid::List_String& identitiesToAdd, /*!< String list of additional participant identities. You do not need to add existing two participants from the original dialog to this list. @n */
                ConversationRef& conference /*!< Returns the resulting conversation or 0 if the method call failed. @n */
        );
        
        /** Takes one or more Contact identities and creates corresponding Participant objects within the context of this Conversation, which must be of type CONFERENCE. If you have an existing dialog conversation, use SpawnConference instead. @n  */
        bool AddConsumers(
                const Sid::List_String& identities /*!< Contact identities to be added to the Conversation. @n */
        );
        
        /** Merges two live conversations. For example, if the user already has a live conversation up - let's call it conversation A. Then a new incoming call occurs - another conversation obtains LOCAL_LIVESTATUS == Conversation::RINGING_FOR_ME, let's call it conversation B. The user wishes to pick up the new incoming call and add it to the existing one. For this you can first call B->JoinLiveSession and then merge two calls with A->Assimilate(B, A). The second argument will return the merged conversation. Note that there are actually three conversation objects involved: A (before merge), B and C (after the merge). Normally it would make sense to have the first conversation (A) as the second argument, so that it gets overwritten with the assimilation result. @n  */
        bool Assimilate(
                const ConversationRef& otherConversation, /*!< The new conversation to be merged with the one already in live state. @n */
                ConversationRef& conversation /*!< Returns a 3rd live conversation, result of merging two existing ones. @n */
        );
        
        /** starts, answers or joins a live session (first one to join becomes LIVE_HOST) */
        bool JoinLiveSession(
                const Sid::String accessToken = "" /*!< if starting a live session, allows to set a custom access token*/
        );
        
        /** This is an alternative to calling Ring method for each Participant individually. This also works with dialogs (with identities containing only one item). @n  */
        bool RingOthers(
                const Sid::List_String identities = Sid::List_String(), /*!< List of Participants to ring. Leaving the list empty will result in ringing all participants of at least speaker level. @n */
                const bool videoCall = false, /*!< If true, indicates that we want to do a video call (video still needs to be separately enabled) @n */
                const Sid::String origin = "" /*!< When call is initiated from web link, this argument must contain the URI that was used @n */
        );
        
        /** Sets VOICE_STATUS to LISTENING in the Participant instance associated with us, causing any input from our microphone to be ignored. This is a Conversation class method, rather than Participant class, because this only applies to local participant. @n  */
        bool MuteMyMicrophone(
        );
        
        /** Sets VOICE_STATUS to SPEAKING in the Participant instance associated with us, causing any input from our microphone to be sent to the call host. This is a Conversation class method, rather than Participant class, because this only applies to local participant. @n  */
        bool UnmuteMyMicrophone(
        );
        
        /** Puts the conversation on hold - Conversation LOCAL_LIVESTATUS changes to ON_HOLD_LOCALLY and to ON_HOLD_REMOTELY for remote participants. @n  */
        bool HoldMyLiveSession(
        );
        
        /** Resumes call from local hold. @n  */
        bool ResumeMyLiveSession(
        );
        
        /** Hang up or refuse to answer an incoming call. Set postVoiceAutoresponse to true to enable a caller to leave a voicemail message. @n  */
        bool LeaveLiveSession(
                const bool postVoiceAutoresponse = false
        );
        
        /** Begin recording a voice mail for this conversation's remote participant. Applies to conversations of type DIALOG only. @n  */
        bool StartVoiceMessage(
        );
        
        /** This method is for doing call transfers. NB! Call transfers only work in one-on-one conversations (dialogs). Before attempting call transfer, you should check availability of transfer recipients with Conversation class CanTransferLiveSession method. If the current conversation has a live session up, that session (call) will be transferred to targets specified in the identities list. Note that identities is a string list - all identities in that list will get incoming calls. The first one of them to pick up the call - wins, and rest of the transfer targets will stop ringing. @n  @n Let's take a closer look how this works in practice. We have three call participants involved in the process, and two separate conversations. Let there be three callers: Caller A (call originator), Caller B (transferor) and Caller C (recipient of transfer). @n  @li Caller A - calls Caller B; Caller B picks up the call - live conversation C1 is now up with A and B in it. @li After awhile, Caller B initiates call transfers to Caller C (and optionally to Callers D, E, F.. ). LOCAL_LIVESTATUS of C1 will get set to TRANSFERRING for both A and B. @li Caller C picks up the call. Conversation C1 will go off live status. For Caller B, conversation C1 LOCAL_LIVESTATUS will change to RECENTLY_LIVE. Another live conversation - C2 gets spawned, with Caller A and Caller C in it. For caller C, participant object representing caller A will have TRANSFERRED_BY property set to identity of caller A. For Caller B (in now no longer live conversation C1), participant object representing caller A gets its TRANSFERRED_TO property set to identity of caller C. @n  */
        bool TransferLiveSession(
                const Sid::List_String& identities, /*!< String list of transfer target identities. As soon as first one in this list picks up the call, others will stop ringing. @n */
                const Sid::String transferTopic = "" /*!< Optional conversation topic. This value will get set as META_TOPIC property of the conversation at the transferee end. Note that this is the only case where META_TOPIC field is used in context of dialog conversations. Thus assumption that remote UI will display topic field in case of dialogs may not be 100% correct. @n */
        );
        
        /** Checks if the identity is available for receiving a transferred live session. If you are going to attempt to go for multiple transfer targets, you should use this check for all the target identities. @n  */
        bool CanTransferLiveSession(
                const Sid::String& identity, /*!< Target identity. @n */
                bool& result /*!< Returns true if call transfer to given target is possible. @n */
        );
        
        /** Sends DTMF tone to a live conversation. @n  */
        bool SendDTMF(
                const Participant::DTMF& dtmf, /*!< Outgoing dtmf tone, possible values come from Participant::DTMF enumerator. @n */
                const uint lengthInMS = 260 /*!< Duration in milliseconds. Defaults to 260 ms. Note that the DTMF tone can be also cancelled with Conversation::StopSendDTMF method. @n */
        );
        
        /** Stops the current DTMF tone being played into conversation. For example, use this method to cancel DTMF signals started with Conversation::SendDTMF before the duration given in lengthInMS runs out. @n  */
        bool StopSendDTMF(
        );
        
        /** Sets local user typing indicator in the Conversation. Remote Participants can display these in their UI. @n  */
        bool SetMyTextStatusTo(
                const Participant::TEXT_STATUS& status /*!< Typing indicator status value - Participant::TEXT_STATUS @n */
        );
        
        /** Posts the specified text the conversation, and populates message with a reference to the corresponding Message object (if no error occurred during execution). The isXML argument can be used if the client UI has already taken care of converting message text to xml (for example, your UI might enable users to use bold tags in text messages.) @n  */
        bool PostText(
                const Sid::String& text, /*!< Text value of the outgoing message (gets set as BODY_XML property of the Message object). @n */
                MessageRef& message, /*!< Returns the Message object created as a result of this method (if successful). @n */
                const bool isXML = false /*!< For cases where the text argument was already encoded as xml message. @n */
        );
        
        /** Takes a list of Contacts as an argument and posts the list into the Conversation. The purpose of this feature is to enable sharing contacts between users, without resorting to contact search. Instead, if user A has contacts B and C, he can post contact C into chat with contact B. At this point, Contact B can add contact C to his contact list. From remote side, the posted contacts will appear as messages with type Message::POSTED_CONTACTS appearing in the conversation. The UI should allow adding these contacts from messages with this type into the contact list. @n  @n The list of posted contacts can be retrieved with the Message::GetContacts method.  @n  @n Additionally, the UI then can parse the posted Contact data out of the Message::P_BODY_XML property. The list of contacts is wrapped between <contacts ..> </contacts> tags. Each contact item in the xml has following format: @li t - contact type. "s" - skype contact; "p" - phone number; @li s - skypename, present only in skypename contacts (t="s") @li p - phone number, present only in phone number contacts (t="p") @li f - contact's full name, if available @li d - contact's display name, if available @n  @n Note that only the type (t) field is mandatory. Depending on type, either skypename (s) or phone number (p) fields are always present. Full name and display name fields are optional. @n  @n Example BODY_XML with skypname contact:  @n @code @n <contacts alt="alt text"><c t="s" s="skypename" f="full name"/></contacts> @n @endcode @n  @n Example BODY_XML with PSTN contact:  @n @code @n <contacts alt="alt text"><c t="p" p="+37212345678" d="Some PSTN number"/></contacts> @n @endcode  @n  @n Example BODY_XML with multiple contacts:  @n @code @n <contacts alt="alt text"><c t="p" p="+37212345678" d="Some PSTN number"/><c t="s" s="someskypename"/></contacts> @n @endcode @n  */
        bool PostContacts(
                const ContactRefs& contacts /*!< List of Contact objects, to be posted in the conversation. @n */
        );
        
        /** Takes a list of fully-qualified filenames and initiates corresponding file transfers in the conversation. From the remote side, incoming file transfers will appear as a conversation message with type POSTED_FILES. Once such a message is detected, the list of file transfer objects can be retrieved with Message::GetTransfers. At that point, remote participants will need to accept or decline those transfers. @n  */
        bool PostFiles(
                const Sid::List_Filename& paths, /*!< list of fully-qualified filenames to be transferred @n */
                const Sid::String& body, /*!< Optional BODY_XML property for POSTED_FILES type messages that show up in remote UI. @n */
                TRANSFER_SENDFILE_ERROR& error_code, /*!< Error code, possible values come from the TRANSFER_SENDFILE_ERROR enumerator. This will be set for the first failed fail. The failed file is identified in the error_file return argument. @n */
                Sid::Filename& error_file /*!< Filename of the file that triggered error. @n */
        );
        
        /** Stops the active voicemail recording and sends it (dialog only) */
        bool PostVoiceMessage(
                const VoicemailRef& voicemail, /*!< This argument is deprecated as of SDK version 3.2. Instead of manually constructing Voicemail object, you can call Conversation::StartVoiceMessage method to start recording a voicemail in context of a dialog. PostVoiceMessage will stop recording this voicemail and post it in the dialog. If instead of sending Voicemail, the user decides to cancel it, you should use Conversation::LeaveLiveSession method (Voicemail::Cancel is deprecated). @n */
                const Sid::String& body /*!< Optional text message that remote UI can display in conversation, to notify the user of a new voicemail. @n */
        );
        
        /** Takes an SMS instance created by Skype::CreateOutgoingSms and posts it in the conversation. Note that you will need to set both Sms body text (Sms::SetBody) and recipient list (Sms::SetTargets) before you can post the object. @n  */
        bool PostSMS(
                const SmsRef& sms, /*!< SMS object. @n */
                const Sid::String body = "" /*!< This argument is currently ignored. The message text needs to be set with Sms::SetBody method, prior to passing the Sms object to this method @n */
        );
        
        /** Retrieves a binary join blob for joining public conversations, which are always of type CONFERENCE. If called for a dialog, the blob argument will contain the empty string. The best way to create a Public Chat is to first create a fresh conversation with Skype class CreateConference, then minimally apply the public chat options OPT_JOINING_ENABLED and OPT_ENTRY_LEVEL_RANK - options, like this (C++):  @n @code @n C->SetOption(Conversation::OPT_JOINING_ENABLED, true); @n @endcode @n  @n When that is done, you can call GetJoinBlob to retrieve the blob string. Use the blob string to generate and post an HTML link whose href attribute looks like this: href="skype:?chat&blob=_BLOB_GOES_HERE" A person running Skype desktop client can click this link to join the conversation and have that conversation opened in his UI. Note that the conversation host (creator) needs to be online for new joiners-via-link to start participating in the Public Chat. */
        bool GetJoinBlob(
                Sid::String& blob /*!< Returns the public conversation join blob. @n */
        );
        
        /** Tries to join a public conversation (aka public chat). This method is only useful if you have used Skype::GetConversationByBlob method with alsoJoin argument set to false. @n  */
        bool Join(
        );
        
        /** Submits password for joining password-protected conversation. @n  */
        bool EnterPassword(
                const Sid::String& password /*!< Password string. @n */
        );
        
        /** Sets password protection/new password for the conversation. @n  */
        bool SetPassword(
                const Sid::String& password, /*!< New password. @n */
                const Sid::String& hint /*!< Password hint. @n */
        );
        
        /** Leaves the conference. Not applicable to dialogs. @n  */
        bool RetireFrom(
        );
        
        /** Deletes this conversation, which must be of type CONFERENCE - dialogs between local user and any of his contacts are always persistant. Note that this also removes corresponding Message and Participant objects. @n  */
        bool Delete(
        );
        
        /** Changes the META_NAME property of the conversation. Note that unlike topic and guidelines, this rename is just local - remote participants can have their own private names for conversations. @n  */
        bool RenameTo(
                const Sid::String& name /*!< New name for the conversation. Passing an empty string in this argument causes the META_NAME to unset. @n */
        );
        
        /** Setter for Conversation class IS_BOOKMARKED. @n  */
        bool SetBookmark(
                const bool& bookmark /*!< Set true to set the bookmark, false to remove the bookmark. @n */
        );
        
        /** Setter for Conversation class ALERT_STRING property. The main use of this property is checking bodies of incoming messages in the conversation for the alert string and producing notifications in UI for the user, when appropriate. @n  */
        bool SetAlertString(
                const Sid::String& alertString /*!< Substring to check in BODY_XML property of incoming messages. @n */
        );
        
        /** Removes conversation from Inbox. @n  */
        bool RemoveFromInbox(
        );
        
        /** Sets Conversation inbox_timestamp property. If the timestamp argument is left empty or is greater than conversation consumption horizon, then the conversation will be restored to the inbox. @n  */
        bool AddToInbox(
                const uint timestamp = 0 /*!< If left empty or set to 0, the inbox_timestamp property is set to current time. @n */
        );
        
        /** This method can be used to set the consumption (read) status of messages in the conversation. It sets Message::CONSUMPTION_STATUS to Message::CONSUMED for all messages in the conversation, older than the given timestamp. If the second argument is set to true, it also modifies messages more recent than the timestamp, by marking them as unconsumed. @n  */
        bool SetConsumedHorizon(
                const uint& timestamp, /*!< Consumption cutoff timestamp. Setting this to current time will mark all messages in the conversation as consumed. @n */
                const bool also_unconsume = false /*!< If set to true, this also marks messages newer than the cutoff timestamp as unconsumed. For example, setting timestamp to 0 and also_unconsumed to true, will unconsume all messages in the conversation. @n */
        );
        
        /** sets consumption horizon to last inbox message id timestamp */
        bool MarkUnread(
        );
        
        /** Checks if the conversation is a member of the given ContactGroup @n  */
        bool IsMemberOf(
                const ContactGroupRef& group, /*!< ContactGroup @n */
                bool& result /*!< True if this conversation is a member of the ContactGroup specified by the group argument contains the conversation @n */
        );
        
        enum PARTICIPANTFILTER {
                ALL = 0                       /*!< All participants (may included some that are RETIRED or OUTLAW, but not all of them)*/,
                CONSUMERS                     /*!< Participants that can receive messages, including myself*/,
                APPLICANTS                    /*!< Only people who are applying to join the conversation*/,
                CONSUMERS_AND_APPLICANTS      /*!< Consumers and applicants*/,
                MYSELF                        /*!< Myself*/,
                OTHER_CONSUMERS               /*!< All consumers except myself*/
        };
        
        /** Retrieves the list of this conversation's current participants, which you can optionally request to be filtered. If no Participants pass the filter, an empty list will be returned (the method itself still returns true). @n  */
        bool GetParticipants(
                ParticipantRefs& participants, /*!< List of conversation Participant objects that passed the filter. @n */
                const Conversation::PARTICIPANTFILTER filter = Conversation::ALL /*!< Conversation::PARTICIPANTFILTER - defaults to Conversation::ALL @n */
        );
        
        /** Returns recent messages. The messages are returned in two lists - new messages (unconsumed) and recent message history (context messages). The context message list contains messages that are already above the consumption horizon but are fairly recent, making it likely that displaying them in UI would be good default behaviour. @n  */
        bool GetLastMessages(
                MessageRefs& contextMessages, /*!< Already consumed messages, provided for context*/
                MessageRefs& unconsumedMessages, /*!< Unconsumed messages*/
                const uint requireTimestamp = 0 /*!< If set to a non-zero value, includes messages no earlier than this timestamp, if not, includes messages from the last 24 hours only @n */
        );
        
        /** Finds the most recent Message object in the conversation that contains the substring specified by the text argument. If no matching messages are found, this method will return false. The search proceeds backwards in time, starting from the timestamp argument. To continue searching, you can start with timestamp=MAX_UINT, retrieve the TIMESTAMP property of the matching message, decrement it by one, and submit it as timestamp for the next FindMessage call. @n  */
        bool FindMessage(
                const Sid::String& text, /*!< Substring to search for. @n */
                MessageRef& message, /*!< Returns matching message or 0 if there was no match. As the likelihood of this object being invalid is quite high, you should always check for method return value before you start calling methods of this object. @n */
                const uint fromTimestampUp = MAX_UINT
        );
        
        enum LIST_TYPE {
                ALL_CONVERSATIONS = 0         /*!< bookmarked or in_inbox or live or with_meta_info or activity in last 30 days*/,
                INBOX_CONVERSATIONS           /*!< only last 6 months conversations are kept there*/,
                BOOKMARKED_CONVERSATIONS      /*!< is_bookmarked is set*/,
                LIVE_CONVERSATIONS            /*!< local_livestatus is different from NONE*/,
                REALLY_ALL_CONVERSATIONS = 5  /*!< all conversations, without any of the limits of ALL_CONVERSATIONS*/
        };
        
        /** This callback gets fired when participants join or leave the conversation. @n  */
        virtual void OnParticipantListChange(
        );
        
        /** Called for each message in currently loaded conversations */
        virtual void OnMessage(
                const MessageRef& message
        );
        
        /** This callback gets fired when a new Conversation is created using SpawnConference. @n 
        * @param spawned Conversation object that got created. @n 
        */
        virtual void OnSpawnConference(
                const ConversationRef& spawned
        );
        
        /** Conversation::P_IDENTITY property get accessor */
        bool GetPropIdentity(Sid::String& identity);
        
        /** Conversation::P_TYPE property get accessor */
        bool GetPropType(Conversation::TYPE& type);
        
        /** Conversation::P_LIVE_HOST property get accessor */
        bool GetPropLiveHost(Sid::String& live_host);
        
        /** Conversation::P_LIVE_START_TIMESTAMP property get accessor */
        bool GetPropLiveStartTimestamp(uint& live_start_timestamp);
        
        /** Conversation::P_LIVE_IS_MUTED property get accessor */
        bool GetPropLiveIsMuted(bool& live_is_muted);
        
        /** Conversation::P_ALERT_STRING property get accessor */
        bool GetPropAlertString(Sid::String& alert_string);
        
        /** Conversation::P_IS_BOOKMARKED property get accessor */
        bool GetPropIsBookmarked(bool& is_bookmarked);
        
        /** Conversation::P_GIVEN_DISPLAYNAME property get accessor */
        bool GetPropGivenDisplayname(Sid::String& given_displayname);
        
        /** Conversation::P_DISPLAYNAME property get accessor */
        bool GetPropDisplayname(Sid::String& displayname);
        
        /** Conversation::P_LOCAL_LIVESTATUS property get accessor */
        bool GetPropLocalLivestatus(LOCAL_LIVESTATUS& local_livestatus);
        
        /** Conversation::P_INBOX_TIMESTAMP property get accessor */
        bool GetPropInboxTimestamp(uint& inbox_timestamp);
        
        /** Conversation::P_INBOX_MESSAGE_ID property get accessor */
        bool GetPropInboxMessageId(MessageRef& inbox_message_id);
        
        /** Conversation::P_UNCONSUMED_SUPPRESSED_MESSAGES property get accessor */
        bool GetPropUnconsumedSuppressedMessages(uint& unconsumed_suppressed_messages);
        
        /** Conversation::P_UNCONSUMED_NORMAL_MESSAGES property get accessor */
        bool GetPropUnconsumedNormalMessages(uint& unconsumed_normal_messages);
        
        /** Conversation::P_UNCONSUMED_ELEVATED_MESSAGES property get accessor */
        bool GetPropUnconsumedElevatedMessages(uint& unconsumed_elevated_messages);
        
        /** Conversation::P_UNCONSUMED_MESSAGES_VOICE property get accessor */
        bool GetPropUnconsumedMessagesVoice(bool& unconsumed_messages_voice);
        
        /** Conversation::P_ACTIVE_VM_ID property get accessor */
        bool GetPropActiveVmId(VoicemailRef& active_vm_id);
        
        /** Conversation::P_CONSUMPTION_HORIZON property get accessor */
        bool GetPropConsumptionHorizon(uint& consumption_horizon);
        
        /** Conversation::P_LAST_ACTIVITY_TIMESTAMP property get accessor */
        bool GetPropLastActivityTimestamp(uint& last_activity_timestamp);
        
        /** Conversation::P_SPAWNED_FROM_CONVO_ID property get accessor */
        bool GetPropSpawnedFromConvoId(ConversationRef& spawned_from_convo_id);
        
        /** Conversation::P_CREATOR property get accessor */
        bool GetPropCreator(Sid::String& creator);
        
        /** Conversation::P_CREATION_TIMESTAMP property get accessor */
        bool GetPropCreationTimestamp(uint& creation_timestamp);
        
        /** Conversation::P_MY_STATUS property get accessor */
        bool GetPropMyStatus(MY_STATUS& my_status);
        
        /** Conversation::P_OPT_JOINING_ENABLED property get accessor */
        bool GetPropOptJoiningEnabled(bool& opt_joining_enabled);
        
        /** Conversation::P_OPT_ENTRY_LEVEL_RANK property get accessor */
        bool GetPropOptEntryLevelRank(Participant::RANK& opt_entry_level_rank);
        
        /** Conversation::P_OPT_DISCLOSE_HISTORY property get accessor */
        bool GetPropOptDiscloseHistory(bool& opt_disclose_history);
        
        /** Conversation::P_OPT_ADMIN_ONLY_ACTIVITIES property get accessor */
        bool GetPropOptAdminOnlyActivities(ALLOWED_ACTIVITY& opt_admin_only_activities);
        
        /** Conversation::P_PASSWORDHINT property get accessor */
        bool GetPropPasswordhint(Sid::String& passwordhint);
        
        /** Conversation::P_META_NAME property get accessor */
        bool GetPropMetaName(Sid::String& meta_name);
        
        /** Conversation::P_META_TOPIC property get accessor */
        bool GetPropMetaTopic(Sid::String& meta_topic);
        
        /** Conversation::P_META_GUIDELINES property get accessor */
        bool GetPropMetaGuidelines(Sid::String& meta_guidelines);
        
        /** Conversation::P_META_PICTURE property get accessor */
        bool GetPropMetaPicture(Sid::Binary& meta_picture);
        
};


/*! \class Message skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  Events in a conversation context are expressed as Messages. It is therefore useful to think of Message objects as
 *  events, rather than specifically text chat messages. @brief @n  @n Message member functions all return a Boolean
 *  indicating the success (true) or failure (false) of processing the request itself (transport, runtime availability,
 *  and so forth) - not the success or failure of its associated functionality. For example, Message::Edit returns true
 *  if it was able to make a determination, and its result parameter reflects whether this Message can be edited.
 *  Similarly, Message::Edit returns false if it was unable to make a determination, and the value of its result
 *  parameter is undefined. @n  @n Message member functions that are specific to a Message TYPE return false if this
 *  Message is not of that type. For example, Message::GetVoiceMessage will return false if this Message's type is not
 *  POSTED_VOICE_MESSAGE. @n  @n The actual meaning of a Message can be determined by its P_TYPE property. The meanings
 *  of most other Message properties depend on the value of P_TYPE. For example, let's take P_BODY_XML property. @n  @n
 *  Following messages have a text entered by the user as a body. It may contain emoticons, URLs, etc. @li POSTED_TEXT
 *  @li POSTED_EMOTE @li SET_METADATA @li REQUESTED_AUTH @n  @n Following messages have a custom XML format for the body
 *  (see the specific section on these message types for details): @li POSTED_CONTACTS @li POSTED_VOICE_MESSAGE @li
 *  POSTED_FILES @li POSTED_SMS @li STARTED_LIVESESSION and ENDED_LIVESESSION (same format) @n  @n Following messages do
 *  not use the body property: @li SPAWNED_CONFERENCE @li ADDED_CONSUMERS @li ADDED_APPLICANTS @li RETIRED_OTHERS @li
 *  RETIRED @li SET_RANK @li HAS_BIRTHDAY @li GRANTED_AUTH @li BLOCKED @n  @n Messages such as POSTED_TEXT use a small
 *  subset of a HTML-like markup to control the visual representation of the text. This markup is used by POSTED_TEXT
 *  and POSTED_EMOTE, but also for the conversation topic (CONVERSATION_META_TOPIC property and the body of the
 *  SET_METADATA message) and for authorization requests. @n  @n Having chat messages in XML format means that all
 *  formatting is indicated by XML tags. This includes emoticons and URls. The big advantage is that it makes the
 *  parsing of the message by the UI much easier. The UI does not need to do emoticons or URL detection, this is already
 *  done and it only needs to look for the XML tags. @n  @n For text messages, it is possible for the UI to simply
 *  ignore (meaning strip) the XML and the message will be understandable fine, it will only have lost some formatting.
 *  @n  @n But it is obviously nicer to display at least the most commonly used tags. @n  @n To strip the XML: @li if
 *  they have the alt="sometext" attribute set, return sometext as the output of that tag and ignore the rest of tag and
 *  all nested sub tags @li if no alt="" attribute set, use tag content as output - <sometag>hereissomething</sometag>
 *  is output as hereissomething@li if no alt="" and no tag content, ignore the tag altogether (return nothing) @n Skype
 *  for Windows supports displaying many XML tags, but only a sub-set is regularly used and should be supported by the
 *  UI for a good experience. These are the ones described here. @n Animated emoticons @n Emoticons are encoded with the
 *  "ss" tag. The element content is the plain text representation. It has a "type" attribute indicating the emoticons
 *  canonical name. Example:  @n @code @n Hi <ss type="smile">:-)</ss>  @n @endcode @n  @n Flag emoticons @n Flag
 *  emoticons are little flags. They are encoded with the "flag" tag. The element contents is the plain text
 *  representation and it has a "country" attribute which is a 2-letter ISO-3166 country code. The user can enter a flag
 *  using "(flag:XX)", where XX is a valid ISO country code. Example:  @n @code @n I am in <flag country="cc">CC</flag>
 *  @n @endcode @n  @n Links @n If the library detects a URL, it will encode it using the html "a" tag, with the "href"
 *  attribute indicating the URL. The plain text representation is what the user originally typed. Example:  @n @code @n
 *  I am in <a href="http://wwww.skype.com">www.skype.com</a> @n @endcode @n  @n Alert matches @n When a conversation is
 *  configured to display only alerts if specific words are present in the message (see "/alertson [text to match]"
 *  command), if a message matches the alert, it will be marked with the <alertmatch> tag. This allows the UI to
 *  highlight the word matching. Example:  @n @code @n Maybe <alertmatch>Vincent</alertmatch> knows the answer  @n
 *  @endcode @n  @n Bold, italic, etc @n Skype for Windows also supports displaying bold and italic text, using the "b"
 *  and "i" tags. @n  @n Encoding messages @n When sending a chat message via PostText(), there is the possibility to
 *  indicate if the library should do the XML encoding, or if the message has already been encoded. Usually, the UI can
 *  let library do the encoding. This is the case when the message does not contain any specific formatting. It may
 *  contain emoticons or URls, which will be detected by the library encoder and converted into XML tags. @n If the
 *  message has some more complex encoding, such as a quote or some bold text, it is up to the UI to encode the message.
 *  @n 
 */
class Message : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        Message(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~Message();
        typedef MessageRef Ref;
        typedef MessageRefs Refs;
        enum { MODULE_ID = 9 };
        MessageRef ref() { 
                return MessageRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the Message class */
        enum PROPERTY {
                P_CONVO_ID = 960                /*!< DB ID of corresponding conversation, type: ConversationRef*/,
                P_CONVO_GUID = 120              /*!< GUID of the Conversation. The GUID is a "global ID" - these values are shared accross Skype client instances and accross all the participants of the conversation. @n , type: Sid::String*/,
                P_AUTHOR = 122                  /*!< Identity of the sender. While this is almost always the same as SKYPENAME property of the Contact, in some rare cases it can also be a phone number - for example, incoming voicemail notification Messages (message type = POSTED_VOICE_MESSAGE). @n , type: Sid::String*/,
                P_AUTHOR_DISPLAYNAME            /*!< displayname of the sender at the time of posting, type: Sid::String*/,
                P_GUID = 792                    /*!< Unlike the message id, the GUID is the same on all instances and for all participants. @n , type: Sid::Binary*/,
                P_ORIGINALLY_MEANT_FOR = 790    /*!< This property gets set when a conference is spawned from dialog Conversation. In that case recent message history from the original dialog is copied to the target conversation. For all the copied messages, the ORIGINALLY_MEANT_FOR property will be set to identity of the remote participant of the original dialog. @n , type: Sid::String*/,
                P_TIMESTAMP = 121               /*!< UNIX timestamp (sent time, adjusted for local clock), type: uint*/,
                P_TYPE = 961                    /*!< type: Message::TYPE*/,
                P_SENDING_STATUS                /*!< type: Message::SENDING_STATUS*/,
                P_CONSUMPTION_STATUS = 968      /*!< type: Message::CONSUMPTION_STATUS*/,
                P_EDITED_BY = 222               /*!< Identity of the author that last edited this message. NULL if message has not been edited @n , type: Sid::String*/,
                P_EDIT_TIMESTAMP                /*!< UNIX timestamp of last edit, type: uint*/,
                P_PARAM_KEY = 963               /*!< Message type-specific parameter. See Message::SET_METADATA_KEY for more information. @n , type: uint*/,
                P_PARAM_VALUE                   /*!< Message type-specific parameter @n , type: uint*/,
                P_BODY_XML = 127                /*!< Message type-specific parameter @n , type: Sid::String*/,
                P_IDENTITIES = 125              /*!< Message type-specific parameter. Depending of Message type, this property contains: @li STARTED_LIVESESSION - list of participants in the cal; @li ENDED_LIVESESSION - list of participants in the call; @li POSTED_SMS - list of recipients of the message; @li SPAWNED_CONFERENCE - the list of identities that were added; @li ADDED_CONSUMERS - the list of identities that were added; @li RETIRED_OTHERS - the skypename of the participant who was kicked; @li SET_RANK - the skypename of the participant whose rank was changed; @li REQUESTED_AUTH - Message::P_AUTHOR and Message::P_IDENTITIES are set to the users receiving and requesting the authorization, depending if the message was received or sent; @li GRANTED_AUTH - the skypename of the user we granted authorization; @li BLOCKED - the skypename of the user who was blocked; @li HAS_BIRTHDAY - skypename of current logged in user. @n , type: Sid::String*/,
                P_REASON = 966                  /*!< Message type-specific parameter. Possible values for STARTED/ENDED_LIVESESSION (only set for dialogs): @li no_answer  @li manual  @li busy  @li connection_dropped @li no_skypeout_subscription; @li insufficient_funds @li internet_connection_lost @li skypeout_account_blocked @li pstn_could_not_connect_to_skype_proxy @li pstn_invalid_number @li pstn_number_forbidden @li pstn_call_timed_out @li pstn_busy @li pstn_call_terminated @li pstn_network_error @li number_unavailable @li pstn_call_rejected @li pstn_misc_error @li internal_error @li unable_to_connect @li connection_dropped @li recording_failed @li playback_error @li legacy_error @li blocked_by_privacy_settings @li error @li transfer_failed @li transfer_insufficient_funds @li blocked_by_us @li emergency_call_denied @n  @n This information is now available as an enum in LEAVEREASON @n , type: Sid::String*/,
                P_LEAVEREASON = 126             /*!< Leave reason for message of the RETIRED type, and STARTED/ENDED_LIVESESSION.                   Use for STARTED/ENDED_LIVESESSION is to provide simpler, enum based                   handling and deprecates the reason property (only set for dialogs), type: LEAVE_REASON*/,
                P_PARTICIPANT_COUNT = 982       /*!< Number of people who received this message (including local user) @n , type: uint*/
        };
        
        /** The P_TYPE property determines the actual meaning of the Message object. Only Messages of POSTED_TEXT type contain actual text messages. The meaning and content of the rest of the message properties are largely dependant of the value of the Message::P_TYPE. @n  */
        enum TYPE {
                SET_METADATA = 2              /*!< Conference metadata were changed*/,
                SPAWNED_CONFERENCE = 4        /*!< A conference was spawned from this dialog*/,
                ADDED_CONSUMERS = 10          /*!< Some users were added to the conference*/,
                ADDED_APPLICANTS              /*!< Some users are applying to be added to the conference*/,
                RETIRED_OTHERS                /*!< User was kicked from the conference*/,
                RETIRED                       /*!< User left the conference*/,
                SET_RANK = 21                 /*!< Changed the rank of a user in the Conversation (multichat administration) @n */,
                STARTED_LIVESESSION = 30      /*!< A live session started*/,
                ENDED_LIVESESSION = 39        /*!< A live session ended*/,
                REQUESTED_AUTH = 50           /*!< User requested authorization*/,
                GRANTED_AUTH                  /*!< User was granted authorization. Notification message that user is now an authorized contact (of the local user). @n */,
                BLOCKED = 53                  /*!< User was blocked*/,
                POSTED_TEXT = 61              /*!< A text message*/,
                POSTED_EMOTE = 60             /*!< An emote ('John Doe is laughing', cf /me chat command)*/,
                POSTED_CONTACTS = 63          /*!< The message represents (a set of) contact card(s) posted in the conversation. One message can contain more than one contact cards. The contacts can be retrieved from the message by parsing them out from the P_BODY_XML property. For more information, see Conversation::PostContacts @n */,
                POSTED_SMS                    /*!< The message represents an SMS object that was posted in the Conversation. See Conversation::PostSMS for more details. The Sms object itself can be retrieved from the Message with Message::GetSms The message BODY_XML contains a set of SMS properties, such as status, failurereason, targets, price and timestamp. @n */,
                POSTED_ALERT                  /*!< Deprecated, never sent*/,
                POSTED_VOICE_MESSAGE = 67     /*!< A voicemail*/,
                POSTED_FILES                  /*!< The message represents a (list of) file transfers that were posted in the Conversation with Conversation::PostFiles. Transfer objects can be retrieved from the Message with Message::GetTransfers @n */,
                POSTED_INVOICE                /*!< Currently unused. @n */,
                HAS_BIRTHDAY = 110            /*!< The message represents a Contact birthday notification. @n */
        };
        
        enum SENDING_STATUS {
                SENDING = 1                   /*!< Message has not been delivered to at least one of the participants @n */,
                SENT                          /*!< Message has been delivered to at least one other participant @n */,
                FAILED_TO_SEND                /*!< Message could not be delivered (for SMS this reflects the actual SMS, not the chat message) @n */
        };
        
        /** Indicates if a message has been consumed (meaning read) or not */
        enum CONSUMPTION_STATUS {
                CONSUMED = 0                  /*!< Message has been read. Note that this is a read-only property. Consumption status of individual messages can not be set selectively. Message consumption status is determined at the conversation level, based conversation consumption horizon and individual message timestamps. Conversation consumption horizon can be updated with Conversation::SetConsumedHorizon method.  @n */,
                UNCONSUMED_SUPPRESSED         /*!< Do not notify the user that they have this unread message @n */,
                UNCONSUMED_NORMAL             /*!< Notify the user that they have this unread message @n */,
                UNCONSUMED_ELEVATED           /*!< This message consumption state is marked as DEPRECATED @n */
        };
        
        /** For messages of type SET_METADATA that alert participants to changes to the associated Conversation's metadata, indicates which metadata property changed and its P_BODY_XML property contains the changed data. Your UI is expected to detect messages with PARAM_KEY set and to update its visual representation of Conversation accordingly. @n You can use the associated Conversation's properties and methods to obtain the updated metadata rather than parse the message body XML, for example, Conversation::P_META_PICTURE and Conversation::Conversation::GetPropMetaPicture. @n  */
        enum SET_METADATA_KEY {
                SET_META_NAME = 3640          /*!< Notification message that conversation name has changed. @n */,
                SET_META_TOPIC = 3644         /*!< Notification message that conversation topic has changed. @n */,
                SET_META_GUIDELINES = 3652    /*!< Notification message that conversation guidelines have changed. @n */,
                SET_META_PICTURE = 3658       /*!< Notification message that conversation picture has changed. @n */
        };
        
        /** Indicates the reason a user could not join or left a Conversation. SkypeKit automatically sets "could not join"-related values. "Left voluntarily"-related values are set as a result of explicit user actions. @n  */
        enum LEAVEREASON {
                USER_INCAPABLE = 2            /*!< User cannot chat (user is currently logged in with a client that has chat disabled - see Contact::CAPABILITY.CAPABILITY_TEXT) @n */,
                ADDER_MUST_BE_FRIEND          /*!< Attempt to add local user to a conversation by an unknown contact @n */,
                ADDER_MUST_BE_AUTHORIZED      /*!< Attempt to add local user to a conversation by an unauthorized contact @n */,
                DECLINE_ADD                   /*!< Local user declined an "invitation" to join a chat @n */,
                UNSUBSCRIBE                   /*!< User decided to end participation in an on-going multi-chat @n */
        };
        
        /** For Message types having a body, determines whether that body is editable by the user.  @n  */
        bool CanEdit(
                bool& result
        );
        
        /** For Message types that include a body and are editable:  @li alters BODY_XML of the message object @li sets EDITED_BY and EDIT_TIMESTAMP properties  @li propagates the changes to remote users. @n  */
        bool Edit(
                const Sid::String& newText, /*!< New value of the message BODY_XML property. @n */
                const bool isXML = false, /*!< Specify isXML as true if the message body is formatted as XML; omit it or specify it as false if the message body is plain text. @n */
                const bool undo = false /*!< Reverts the message body to the original version. newText argument is ignored when this is set. @n */
        );
        
        /** For messages of type POSTED_CONTACTS, parses the body XML and formats the data as a list of Contact instances. @n  */
        bool GetContacts(
                ContactRefs& contacts
        );
        
        /** For messages of type POSTED_FILES, parses the body XML and creates a list of Transfer instances. @n  */
        bool GetTransfers(
                TransferRefs& transfers
        );
        
        /** For messages of type POSTED_VOICE_MESSAGE, parses the body XML and creates a Voicemail instance. @n  */
        bool GetVoiceMessage(
                VoicemailRef& voicemail
        );
        
        /** For messages of type POSTED_SMS, parses the body XML and creates an SMS instances @n  */
        bool GetSMS(
                SmsRef& sms
        );
        
        /** Deletes this message from the local database. These deletions do not propagate to the other Skype instances that the user may have on other computers. Nor do they affect other participants that have the same message. This method is specifically from removing Message objects from the database - not for removing Messages from conversations. To remove a Message from a conversation, use Message::Edit method to replace the existing body text with an empty string. @n  */
        bool DeleteLocally(
        );
        
        /** Message::P_CONVO_ID property get accessor */
        bool GetPropConvoId(ConversationRef& convo_id);
        
        /** Message::P_CONVO_GUID property get accessor */
        bool GetPropConvoGuid(Sid::String& convo_guid);
        
        /** Message::P_AUTHOR property get accessor */
        bool GetPropAuthor(Sid::String& author);
        
        /** Message::P_AUTHOR_DISPLAYNAME property get accessor */
        bool GetPropAuthorDisplayname(Sid::String& author_displayname);
        
        /** Message::P_GUID property get accessor */
        bool GetPropGuid(Sid::Binary& guid);
        
        /** Message::P_ORIGINALLY_MEANT_FOR property get accessor */
        bool GetPropOriginallyMeantFor(Sid::String& originally_meant_for);
        
        /** Message::P_TIMESTAMP property get accessor */
        bool GetPropTimestamp(uint& timestamp);
        
        /** Message::P_TYPE property get accessor */
        bool GetPropType(Message::TYPE& type);
        
        /** Message::P_SENDING_STATUS property get accessor */
        bool GetPropSendingStatus(Message::SENDING_STATUS& sending_status);
        
        /** Message::P_CONSUMPTION_STATUS property get accessor */
        bool GetPropConsumptionStatus(Message::CONSUMPTION_STATUS& consumption_status);
        
        /** Message::P_EDITED_BY property get accessor */
        bool GetPropEditedBy(Sid::String& edited_by);
        
        /** Message::P_EDIT_TIMESTAMP property get accessor */
        bool GetPropEditTimestamp(uint& edit_timestamp);
        
        /** Message::P_PARAM_KEY property get accessor */
        bool GetPropParamKey(uint& param_key);
        
        /** Message::P_PARAM_VALUE property get accessor */
        bool GetPropParamValue(uint& param_value);
        
        /** Message::P_BODY_XML property get accessor */
        bool GetPropBodyXml(Sid::String& body_xml);
        
        /** Message::P_IDENTITIES property get accessor */
        bool GetPropIdentities(Sid::String& identities);
        
        /** Message::P_REASON property get accessor */
        bool GetPropReason(Sid::String& reason);
        
        /** Message::P_LEAVEREASON property get accessor */
        bool GetPropLeavereason(LEAVE_REASON& leavereason);
        
        /** Message::P_PARTICIPANT_COUNT property get accessor */
        bool GetPropParticipantCount(uint& participant_count);
        
};


/*! \class Video skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  This class contains basic video control functionality for live conversations with video. Basically, Video objects
 *  represent specific Participant's video state in a live Conversation. The Video class can represent both local
 *  (outgoing) and remote (incoming) video streams. Note that as of SkypeKit SDK version 3.2, this class no longer
 *  handles video rendering in the UI. @brief Currently available SkypeKit runtimes do not support multi-party video.
 *  The API however is designed with future compatibility in mind, so the Video class is attached to Participant class
 *  rather than Conversation class. Once multi-party video will become available for SkypeKit, the logic should go like
 *  this: @n  @n Let there be 4-way live conversation C and participants P1, P2, P3 and P4. P1 is the local user. Remote
 *  participants P2 and P3 are capable of sending video. Remote user P4 is not capable of sending video. You would then
 *  end up with 4 video objects: V1, V2, V3 and V0. @n  @li C->P1->V1-> outgoing video stream @li C->P2->V2-> incoming
 *  video stream 1 @li C->P3->V3-> incoming video stream 2 @li C->P4-> no video object as participant P4 does not
 *  advertise supporting video @li V0-> local webcam preview - this is not attached to any particular conversation,
 *  however the corresponding video object can be retrieved with Skype::GetPreviewVideo method. @n  @n As getting from a
 *  live conversation to running video streams involves three classes, it can be somewhat less than obvious. The basic
 *  state transition logic goes like this: @n  @n You start out with a Conversation, that suddenly becomes live @n  @n
 *  CONVERSATION::LOCAL_LIVESTATUS = IM_LIVE @n At this point you have access to participant list of that conversation.
 *  The next step will be to catch Partcipant property changes for PARTICIPANT::VIDEO_STATUS to see if any of the people
 *  in conversation have Video available. Note that you should not make assumptions on when this availability happens.
 *  Remote users may switch their video on-off as they please. @n  @n PARTICIPANT::VIDEO_STATUS = VIDEO_AVAILABLE @n If
 *  you get to VIDEO_AVAILABLE (not necessarily for all Participants), you can retrieve Video object, with
 *  Participant::GetVideo method. @n  @n Now you will need to handle Video::STATUS property changes. In case of
 *  successful video call, the sequence of Video::STATUS and Participant::VIDEO_STATUS changes for each Participant and
 *  Video object should look like this: @n  @li Video::STATUS = AVAILABLE @li Video::STATUS = STARTING @li Video::STATUS
 *  = CHECKING_SUBSCRIPTION @li Video::STATUS = STARTING @n  @n Participant::VIDEO_STATUS = VIDEO_CONNECTING @li
 *  Video::STATUS = RUNNING @li Participant::VIDEO_STATUS = STREAMING @n Both Video::STATUS == RUNNING and
 *  Participant::VIDEO_STATUS == STREAMING are indicative that the video for that particular participant is up and
 *  running, and your UI should update itself accordingly. @n  @n NB! Note that it is not enough to check for
 *  Video::STATUS property updates. By the time you obtain the Video object in your client, it may already it may
 *  already have progressed to a further status. You should always check the status property immediately after obtaining
 *  the Video object. @n 
 */
class Video : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        Video(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~Video();
        typedef VideoRef Ref;
        typedef VideoRefs Refs;
        enum { MODULE_ID = 11 };
        VideoRef ref() { 
                return VideoRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the Video class */
        enum PROPERTY {
                P_STATUS = 130                  /*!< Video::STATUS, type: STATUS*/,
                P_ERROR                         /*!< 'errorcode errortext' , type: Sid::String*/,
                P_DEBUGINFO                     /*!< space-separated string of tokens, type: Sid::String*/,
                P_DIMENSIONS                    /*!< This property does not currently work, always containing an empty string. For desktop video, you can get the frame dimensions from the video frame buffers API instead - the buffer struct retrieved with ipc.getFrame() or ipc.getNewFrame() has width and height fields, which you can then use in your UI. With RTP video solutions, you already have the frame sizes in your videohost code. Communicating these to the UI process is currently up to you. @n , type: Sid::String*/,
                P_MEDIA_TYPE                    /*!< Indicates whether the video object is streaming webcam video or screensharing session, values: MEDIA_SCREENSHARING, MEDIA_VIDEO @n , type: MEDIATYPE*/,
                P_CONVO_ID = 1104               /*!< conference id to be able to identify remote/local video in the same call, type: uint*/,
                P_DEVICE_PATH                   /*!< device path used by video object, type: Sid::String*/
        };
        
        /** Setupkey SETUPKEY_VIDEO_DEVICE type:string  @n Selected video device name @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_VIDEO_DEVICE "Lib/Video/Device"
        
        /** Setupkey SETUPKEY_VIDEO_DEVICE_PATH type:string  @n Currently selected video device path. @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_VIDEO_DEVICE_PATH "Lib/Video/DevicePath"
        
        /** Setupkey SETUPKEY_VIDEO_AUTOSEND type:int  @n Setting this to 1 starts sending video automatically when call starts @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_VIDEO_AUTOSEND "Lib/Video/AutoSend"
        
        /** Setupkey SETUPKEY_VIDEO_DISABLE type:int  @n Setting this to 1 disables all video functionality. @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_VIDEO_DISABLE "*Lib/Video/Disable"
        
        /** Setupkey SETUPKEY_VIDEO_RECVPOLICY type:string default value:"contacts" @n noone | contacts | callpolicy @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_VIDEO_RECVPOLICY "Lib/Video/RecvPolicy"
        
        /** Setupkey SETUPKEY_VIDEO_ADVERTPOLICY type:string default value:"contacts" @n noone | contacts | everyone @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_VIDEO_ADVERTPOLICY "Lib/Video/AdvertPolicy"
        
        enum STATUS {
                NOT_AVAILABLE = 0,
                AVAILABLE,
                STARTING,
                REJECTED,
                RUNNING,
                STOPPING,
                PAUSED,
                NOT_STARTED,
                HINT_IS_VIDEOCALL_RECEIVED,
                UNKNOWN,
                RENDERING,
                CHECKING_SUBSCRIPTION,
                SWITCHING_DEVICE
        };
        
        enum MEDIATYPE {
                MEDIA_SCREENSHARING = 1,
                MEDIA_VIDEO = 0
        };
        
        enum VIDEO_DEVICE_CAPABILITY {
                VIDEOCAP_HQ_CAPABLE = 0,
                VIDEOCAP_HQ_CERTIFIED,
                VIDEOCAP_REQ_DRIVERUPDATE,
                VIDEOCAP_USB_HIGHSPEED
        };
        
        /** This is a legacy method, used in earlier version of desktop video, for passing window handles from the UI to the rendering engine in the runtime. In current version of desktop video, the rendering takes place in the client. The video objects now need to be associated not with a specific window, but rather with an IPC channel that the Video object sends decoded video frames from the runtime to the client UI. See Video::SetRemoteRendererId for more information. @n  */
        bool SetScreen(
                const uint& windowh
        );
        
        /** This method starts either video send or video receive, depending on whether the video object is sender or receiver. In case of desktop video, the receiver side needs to instantiate a renderer object and associate it with the receiveing video (Video::SetRemoteRendererId).  @n  */
        bool Start(
        );
        
        /** This method stops either video send or video receive, depending on whether the video object is sender or receiver. In case of desktop video, the receiver side needs to dis-associate the video object from the renderer, by calling Video::SetRemoteRendererId(0).  @n  */
        bool Stop(
        );
        
        /** This method has no known effect in current version. @n  */
        bool SubmitCaptureRequest(
                bool& ret,
                uint& requestId
        );
        
        /** This method has no effect in current version. @n  */
        virtual void OnCaptureRequestCompleted(
                const uint& requestId,
                const bool& isSuccessful,
                const Sid::Binary& image,
                const int& width,
                const int& height
        );
        
        /** This method has no known effect in current version. @n  */
        bool SetScreenCaptureRectangle(
                const int& x0,
                const int& y0,
                const uint& width,
                const uint& height,
                const int monitorNumber = 0,
                const uint windowHandle = 0
        );
        
        /** Sets video render rectangle co-ordinates, relative to upper left corner of the rendering window's client area (there us no need to compensate for the title bar area or window borders). Note that the rendering rectangle does not get automatically updated after video window resize occurs. You will need to catch the resize events and update the rendering rectangle according to the new size of the video window. @n  @n NB! Currently, when the updating the render rectangle to the new size of the video window, you will also need to call Video class SetScreen method - before SetRenderRectangle. @n  */
        bool SetRenderRectangle(
                const int& x0,
                const int& y0,
                const uint& width,
                const uint& height
        );
        
        /** This method is specific to working with desktop video runtimes. In case of desktop video, this method associates a Video object with a rendering object. Once this association is in place, the Video object will start sending decoded video frames to the renderer object, over shared memory IPC transport. The renderer then periodically reads the shared memory buffer and draws bitmaps on the screen.  @n  @n The "remote renderer ID" value you are expected to pass in this method comes from the renderer. The renderer class is normally implemented by you (for a specific GUI framework). However, the shared memory transport mechanism, for receiving decoded frames, is independant of GUI framework and are provided with the SDK for all three supporeted operating systems.  @n  @n Your renderer class can include instance of the IPC transport as VideoTransportClient <WinShm> ipc or VideoTransportClient <PosixShm>  ipc; The ipc object then exposes ipc.key() function that returns the IPC channel ID. This ID can then be passed to the Video::SetRemoteRendererId method. A reference implementation of desktop video is included in the SDK (examples/cpp/qt_videocalls). @n  @n When the incoming video stream stops (or rendering is stopped by the user), your client should explicitly stop the frame transport by calling Video::SetRemoteRendererId(0). @n  */
        bool SetRemoteRendererId(
                const uint& id /*!< IPC channel ID retrieved from the shared memory transport class (see ipc/cpp/VideoBuffers in the SDK). @n */
        );
        
        /** This method has no effect in current version. @n  */
        bool SelectVideoSource(
                const Video::MEDIATYPE& mediaType,
                const Sid::String webcamName = "",
                const Sid::String devicePath = "",
                const bool updateSetup = false
        );
        
        bool GetCurrentVideoDevice(
                MEDIATYPE& mediatype,
                Sid::String& deviceName,
                Sid::String& devicePath
        );
        
        /** Video::P_STATUS property get accessor */
        bool GetPropStatus(STATUS& status);
        
        /** Video::P_ERROR property get accessor */
        bool GetPropError(Sid::String& error);
        
        /** Video::P_DEBUGINFO property get accessor */
        bool GetPropDebuginfo(Sid::String& debuginfo);
        
        /** Video::P_DIMENSIONS property get accessor */
        bool GetPropDimensions(Sid::String& dimensions);
        
        /** Video::P_MEDIA_TYPE property get accessor */
        bool GetPropMediaType(MEDIATYPE& media_type);
        
        /** Video::P_CONVO_ID property get accessor */
        bool GetPropConvoId(uint& convo_id);
        
        /** Video::P_DEVICE_PATH property get accessor */
        bool GetPropDevicePath(Sid::String& device_path);
        
};


/*! \class Voicemail skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  Wrapper class that includes voicemail-specific methods and properties. @brief In the Skype Conversation API,
 *  Voicemail is actually something of a misnomer for what would be more accurately called Voice Message. @n  @n The
 *  traditional Voicemail use case involves recording a voice message when an incoming call does not get answered in a
 *  pre-determined amount of time. In the Skype Conversation API, voicemail does not depend on a call going unanswered -
 *  you can post a voice message asynchronously into any dialog conversation at any time. @n  @n In fact, a high-level
 *  action flow directing unanswered incoming live sessions to voicemail is not something provided by the Conversation
 *  API - implementation of this use case is largely up to your UI. @n  @n The fact that your UI must retrieve incoming
 *  Voicemails by monitoring changes to a Conversation instance's Messages illustrates this conceptual difference
 *  between traditional voicemail and voice messages. The message type Message::POSTED_VOICE_MESSAGE indicates that a
 *  Message instance should be handled as a voice message instead of by displaying its body text in the Conversation UI.
 *  Message::GetVoiceMessage enables you to retrieve the associated Voicemail instance; Voicemail::StartPlayback enables
 *  you to listen to the message audio. @n  @n To put it another way, the object chain goes like this:  @n @code @n
 *  Contact->Conversation->Message->Voicemail  @n @endcode  @n  @n There are three basic types of Voicemail objects: @li
 *  INCOMING - received voice messages that can be retrieved from Message objects; @li OUTGOING - outbound voice
 *  messages that can be created with Conversation::StartVoiceMessage; @li GREETING - voice messages that represent
 *  auto-answer greetings, either recorded by the user (CUSTOM_GREETING) or included as part of SkypeKit
 *  (DEFAULT_GREETING). This is the audio that gets played back to sender of the voice message before he can start
 *  recording his voice message. @n  @n Before trying to send out a voicemail, you should ensure that target Contact has
 *  the capability to receive them. Use Contact::GetCapabilityStatus to check for Contact::CAPABILITY_CAN_BE_SENT_VM. @n
 *  @n Recording and Sending a Voice Message @n  @n The first step is to obtain a dialog Conversation with the target
 *  Contact. In that conversation, you can initiate the outgoing voice message with Conversation::StartVoiceMessage @n
 *  @n Note that this call does not return an actual Voicemail object. To catch when an object gets created, you will
 *  need to check Conversation::P_ACTIVE_VM_ID property updates. @n  @n After invoking Conversation::StartVoiceMessage,
 *  SkypeKit instantiates a Voicemail instance for the target Contact's greeting (with type CUSTOM_GREETING or
 *  DEFAULT_GREETING). At this point, the Conversation::P_ACTIVE_VM_ID property update fires, newVM contains a reference
 *  to the greeting, and playback of the greeting for the sender starts automatically.  @n  @n Once the greeting
 *  playback finishes, SkypeKit instantiates a second Voicemail instance for the outgoing voice message. At this point,
 *  the Conversation::P_ACTIVE_VM_ID property update fires again, newVM now contains a reference to the outgoing
 *  message, and recording starts automatically. If you want to include notification and/or error handling for whether
 *  this message was sent successfully, you should make a copy of newVM now. @n  @n Once the user finishes (or abandons)
 *  recording of their message, they want to either send the message or to cancel it. To send the message, use
 *  Conversation::PostVoiceMessage; to cancel the message, use Conversation::LeaveLiveSession. @n  @n Both of these
 *  actions results in the Conversation::P_ACTIVE_VM_ID property update firing for a third time, setting the value to
 *  NULL. However, the Voicemail object will actually continue its existence past this point. Saving a reference to the
 *  message's Voicemail object when you start recording it enables you to keep receiving Voicemail property updates.
 *  This in turn enables your UI to check whether voice message send succeeded or failed. @n  @n The relevant terminal
 *  state Voicemail::P_STATUS property values are: @li Voicemail::CANCELLED - recording and/or sending of this message
 *  was cancelled @li Voicemail::UPLOADED - message sent @li Voicemail::FAILED - message could not be sent @n  @n
 *  Receiving and Playing Back a Voice Message @n  @n On the remote side, the Voicemail appears as a Message object of
 *  type Message::POSTED_VOICE_MESSAGE. The message's author property contains the Skype Name of the Voicemail
 *  originator, and its BodyXml property contains the message length and title text in following format: @n  @n @code @n
 *  <voicemail alt="Sent voicemail to people in this conversation."><message length="5" ></message></voicemail>  @n
 *  @endcode @n  @n Receiver side UI can then retrieve the Voicemail object from the message with
 *  Message::GetVoiceMessage and @n start local playback with Message::StartPlayback. @n 
 */
class Voicemail : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        Voicemail(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~Voicemail();
        typedef VoicemailRef Ref;
        typedef VoicemailRefs Refs;
        enum { MODULE_ID = 7 };
        VoicemailRef ref() { 
                return VoicemailRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the Voicemail class */
        enum PROPERTY {
                P_TYPE = 100                    /*!< type: TYPE*/,
                P_PARTNER_HANDLE                /*!< registered username of the other party, type: Sid::String*/,
                P_PARTNER_DISPNAME              /*!< user's display name of the other party, type: Sid::String*/,
                P_STATUS                        /*!< type: STATUS*/,
                P_FAILUREREASON                 /*!< type: FAILUREREASON*/,
                P_SUBJECT                       /*!< DEPRECATED: subject line, type: Sid::String*/,
                P_TIMESTAMP                     /*!< timestamp of creation, type: uint*/,
                P_DURATION                      /*!< duration in seconds, type: uint*/,
                P_ALLOWED_DURATION              /*!< max allowed duration in seconds, type: uint*/,
                P_PLAYBACK_PROGRESS             /*!< VM playback progress in seconds, type: uint*/,
                P_CONVO_ID = 830                /*!< CONVERSATION_ID of corresponding conversation, type: ConversationRef*/,
                P_CHATMSG_GUID                  /*!< GUID of the message that the VM is tied to, type: Sid::Binary*/
        };
        
        enum TYPE {
                INCOMING = 1,
                DEFAULT_GREETING = 4,
                CUSTOM_GREETING = 2,
                OUTGOING
        };
        
        enum STATUS {
                NOTDOWNLOADED = 1,
                DOWNLOADING,
                UNPLAYED,
                BUFFERING,
                PLAYING,
                PLAYED,
                BLANK,
                RECORDING,
                RECORDED,
                UPLOADING,
                UPLOADED,
                DELETING,
                FAILED,
                DELETING_FAILED,
                CHECKING,
                CANCELLED
        };
        
        enum FAILUREREASON {
                MISC_ERROR = 1,
                CONNECT_ERROR,
                NO_VOICEMAIL_CAPABILITY,
                NO_SUCH_VOICEMAIL,
                FILE_READ_ERROR,
                FILE_WRITE_ERROR,
                RECORDING_ERROR,
                PLAYBACK_ERROR,
                NO_PERMISSION,
                RECEIVER_DISABLED_VOICEMAIL   /*!< receiver turned off voicemail*/,
                SENDER_NOT_AUTHORIZED         /*!< receiver has not authorized you and privacy is not set to anyone*/,
                SENDER_BLOCKED                /*!< receiver blocked sender*/
        };
        
        /** Start recording your own auto-answer greeting message (leave message after the beep...) only. Recording of outgoing Voicemail messages start automatically (using Conversation::StartVoiceMessage) after playback of the remote side greeting message has finished. @n  */
        bool StartRecording(
        );
        
        /** Stop recording of your own auto-answer greeting message only. To stop recording of and send an outgoing Voicemail, use Conversation::PostVoiceMessage. @n  */
        bool StopRecording(
        );
        
        /** Initiates playback of a voice message @n  */
        bool StartPlayback(
        );
        
        /** Terminates playback of a voice message @n  */
        bool StopPlayback(
        );
        
        /** first from server, and then the local copy */
        bool Delete(
        );
        
        /** Canceling recording of your own auto-answer greeting message. To stop recording of and cancel an outgoing Voicemail, use Conversation::LeaveLiveSession. @n  */
        bool Cancel(
        );
        
        /** check if we can send voicemail (unauth,blocked,no priv etc cases). only OUTGOING */
        bool CheckPermission(
                bool& result
        );
        
        /** Voicemail::P_TYPE property get accessor */
        bool GetPropType(TYPE& type);
        
        /** Voicemail::P_PARTNER_HANDLE property get accessor */
        bool GetPropPartnerHandle(Sid::String& partner_handle);
        
        /** Voicemail::P_PARTNER_DISPNAME property get accessor */
        bool GetPropPartnerDispname(Sid::String& partner_dispname);
        
        /** Voicemail::P_STATUS property get accessor */
        bool GetPropStatus(STATUS& status);
        
        /** Voicemail::P_FAILUREREASON property get accessor */
        bool GetPropFailurereason(FAILUREREASON& failurereason);
        
        /** Voicemail::P_SUBJECT property get accessor */
        bool GetPropSubject(Sid::String& subject);
        
        /** Voicemail::P_TIMESTAMP property get accessor */
        bool GetPropTimestamp(uint& timestamp);
        
        /** Voicemail::P_DURATION property get accessor */
        bool GetPropDuration(uint& duration);
        
        /** Voicemail::P_ALLOWED_DURATION property get accessor */
        bool GetPropAllowedDuration(uint& allowed_duration);
        
        /** Voicemail::P_PLAYBACK_PROGRESS property get accessor */
        bool GetPropPlaybackProgress(uint& playback_progress);
        
        /** Voicemail::P_CONVO_ID property get accessor */
        bool GetPropConvoId(ConversationRef& convo_id);
        
        /** Voicemail::P_CHATMSG_GUID property get accessor */
        bool GetPropChatmsgGuid(Sid::Binary& chatmsg_guid);
        
};


/*! \class Sms skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  Wrapper class that includes SMS-specific properties and methods, such as P_BODY and GetTargetPrice. Instantiate SMS
 *  instances using Skype::CreateOutgoingSms; post SMS messages to a Conversation using Conversation::PostSMS. @brief
 *  @n  @n Each SMS can have multiple targets (normalized phone numbers). Note that in a Conversation context, every SMS
 *  instance has a corresponding Message instance. Once you've posted an SMS, you can retrieve its corresponding Message
 *  instance using Sms::GetPropChatmsgId. That Message instance's P_BODY_XML property contains the SMS message data,
 *  such as price, target phone number(s), failure codes, and so forth, which you can parsed out and display in the UI.
 *  To put it another way, the object chain goes like this: @n  @n @code @n Conversation->Message->SMS  @n @endcode @n
 *  @n Note that SkypeKit SDK supports outgoing SMS messages only. SkypeKit clients, even when logged in with accounts
 *  that have SkypeIn numbers, cannot receive SMS messages. @n 
 */
class Sms : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        Sms(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~Sms();
        typedef SmsRef Ref;
        typedef SmsRefs Refs;
        enum { MODULE_ID = 12 };
        SmsRef ref() { 
                return SmsRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the Sms class */
        enum PROPERTY {
                P_TYPE = 190                    /*!< type: TYPE*/,
                P_STATUS                        /*!< type: STATUS*/,
                P_FAILUREREASON                 /*!< Set asynchronously and meaningful only after invoking Conversation::PostSMS and detecting Sms::STATUS of SOME_TARGETS_FAILED or FAILED. @n , type: FAILUREREASON*/,
                P_IS_FAILED_UNSEEN = 48         /*!< set to 1 when status goes to FAILED. use MarkSeen() to clear, type: bool*/,
                P_TIMESTAMP = 198               /*!< unix timestamp of message submission, type: uint*/,
                P_PRICE = 193                   /*!< The total price of sending this SMS message (sum of the individual prices to send to each recipient). Defaults to -1 on instantiation and incremented by the price for each recipient once that recipient's status reflects TARGET_ACCEPTABLE. Use Sms::GetTargetPrice to retrieve individual target prices. @n  @n A value of MAX_UINT indicates that SkypeKit is actively querying and/or updating the value. Note that P_PRICE is an integer value. Calculate the actual price (in units specified by P_PRICE_CURRENCY) using P_PRICE_PRECISION as: @n  @n @code @n actualPrice = price / 10^pricePrecision;  @n @endcode @n , type: uint*/,
                P_PRICE_PRECISION = 49          /*!< The decimal precision of the SMS price values, both individual and total. For example, a value of 2 indicates that you should divide the price (represented as an integer) by 100 (10^2) to obtain the actual price. @n , type: uint*/,
                P_PRICE_CURRENCY = 194          /*!< should be same as account currency at the time of composing/sending, type: Sid::String*/,
                P_REPLY_TO_NUMBER = 199         /*!< number that should receive the replies, type: Sid::String*/,
                P_TARGET_NUMBERS = 195          /*!< space-separated normalised pstn numbers, type: Sid::String*/,
                P_TARGET_STATUSES               /*!< binary blob. track with OnPropertyChange(), access with GetTargetStatus(target), type: Sid::Binary*/,
                P_BODY                          /*!< actual payload, type: Sid::String*/,
                P_CHATMSG_ID = 840              /*!< reference to Message, type: MessageRef*/
        };
        
        enum TYPE {
                OUTGOING = 2                  /*!< a normal outgoing SMS message*/,
                CONFIRMATION_CODE_REQUEST     /*!< a message requesting a SMS confirmation code be sent to the number provided*/,
                CONFIRMATION_CODE_SUBMIT      /*!< a message returning the SMS confirmation code received as a result of a CONFIRMATION_CODE_REQUEST to authorize it*/
        };
        
        enum STATUS {
                COMPOSING = 3,
                SENDING_TO_SERVER,
                SENT_TO_SERVER,
                DELIVERED,
                SOME_TARGETS_FAILED,
                FAILED
        };
        
        enum FAILUREREASON {
                MISC_ERROR = 1,
                SERVER_CONNECT_FAILED,
                NO_SMS_CAPABILITY,
                INSUFFICIENT_FUNDS,
                INVALID_CONFIRMATION_CODE,
                USER_BLOCKED,
                IP_BLOCKED,
                NODE_BLOCKED,
                NO_SENDERID_CAPABILITY
        };
        
        enum TARGETSTATUS {
                TARGET_ANALYZING = 1,
                TARGET_UNDEFINED,
                TARGET_ACCEPTABLE,
                TARGET_NOT_ROUTABLE,
                TARGET_DELIVERY_PENDING,
                TARGET_DELIVERY_SUCCESSFUL,
                TARGET_DELIVERY_FAILED
        };
        
        /** Retrieves the send status of this SMS to a particular recipient (P_TARGET_STATUSES) either prior to or after invoking Conversation::PostSMS. @n  */
        bool GetTargetStatus(
                const Sid::String& target, /*!< The normalized phone number of the target recipient. @n */
                TARGETSTATUS& status /*!< The send status of the target recipient, for example, TARGET_ANALYZING, TARGET_DELIVERY_PENDING, TARGET_DELIVERY_SUCCESSFUL, TARGET_DELIVERY_FAILED, and so forth. TARGET_UNDEFINED implies that the specified target is not a recipient of this SMS. @n */
        );
        
        /** Retrieves the amount of Skype credit necessary to send the SMS to a particular recipient. Defaults to -1 on instantiation and set only when that recipient's status reflects TARGET_ACCEPTABLE. Use Sms::GetPropPrice to retrieve the total cost of this SMS. @n  @n Note that the target price is an integer value. Calculate the actual price (in units specified by P_PRICE_CURRENCY) using P_PRICE_PRECISION as: @n @code @n actualTargetPrice = targetPrice / 10^pricePrecision;  @n @endcode @n  */
        bool GetTargetPrice(
                const Sid::String& target, /*!< The normalized phone number of the target recipient. @n */
                uint& price /*!< The price of sending this SMS message to the target recipient. @n */
        );
        
        enum SETBODYRESULT {
                BODY_INVALID = 0              /*!< body not set. message status wrong or invalid, or body not valid utf8 string*/,
                BODY_TRUNCATED                /*!< body too long. set, but truncated. charsUntilNextChunk contains maxChunks value*/,
                BODY_OK                       /*!< body was set OK*/,
                BODY_LASTCHAR_IGNORED         /*!< last unicode char was ignored, as some of the text would be deleted due to conversion*/
        };
        
        /** Sets the recipient(s) of this SMS. Note that each invocation replaces the target list and re-calculates all prices - they are not additive! @n  */
        bool SetTargets(
                const Sid::List_String& numbers, /*!< Normalized phone number(s) of the intended recipient(s). @n */
                bool& success /*!< Set to true if the target list appears to contain valid, normalized telephone numbers. Note that this check is not very reliable. Actual target validity checking occurs asynchronously in the background, and manifests itself as a series of Sms::P_TARGET_STATUSES property change events. @n */
        );
        
        /** -The- method for setting the body text of this SMS. While Conversation::PostSMS does have a body argument, that argument is currently unused. @n  */
        bool SetBody(
                const Sid::String& text, /*!< Message body text. @n */
                SETBODYRESULT& result, /*!< Whether the Message body was successfully set and if not, why not. @n */
                Sid::List_String& chunks, /*!< The Message body as a list of individual chunks. @n */
                uint& charsUntilNextChunk /*!< Number of available characters until creation of the next chunk becomes necessary. @n */
        );
        
        /** Retrieves string list of SMS text chunks in first argument, while the second argument contains the number of available characters until creation of the next chunk becomes necessary. @n  */
        bool GetBodyChunks(
                Sid::List_String& textChunks, /*!< List of text chunk strings @n */
                uint& charsUntilNextChunk /*!< Number of available characters until creation of the next chunk becomes necessary. @n */
        );
        
        enum CONFIRM_TYPE {
                ID_SMS = 1                    /*!< Confirm mobile number as SMS sender number*/,
                ID_MOBILE                     /*!< Confirm mobile number as CLI for SkypeOut calls*/,
                ID_SKYPEIN                    /*!< unused currently*/
        };
        
        /** Sms::P_TYPE property get accessor */
        bool GetPropType(TYPE& type);
        
        /** Sms::P_STATUS property get accessor */
        bool GetPropStatus(STATUS& status);
        
        /** Sms::P_FAILUREREASON property get accessor */
        bool GetPropFailurereason(FAILUREREASON& failurereason);
        
        /** Sms::P_IS_FAILED_UNSEEN property get accessor */
        bool GetPropIsFailedUnseen(bool& is_failed_unseen);
        
        /** Sms::P_TIMESTAMP property get accessor */
        bool GetPropTimestamp(uint& timestamp);
        
        /** Sms::P_PRICE property get accessor */
        bool GetPropPrice(uint& price);
        
        /** Sms::P_PRICE_PRECISION property get accessor */
        bool GetPropPricePrecision(uint& price_precision);
        
        /** Sms::P_PRICE_CURRENCY property get accessor */
        bool GetPropPriceCurrency(Sid::String& price_currency);
        
        /** Sms::P_REPLY_TO_NUMBER property get accessor */
        bool GetPropReplyToNumber(Sid::String& reply_to_number);
        
        /** Sms::P_TARGET_NUMBERS property get accessor */
        bool GetPropTargetNumbers(Sid::String& target_numbers);
        
        /** Sms::P_TARGET_STATUSES property get accessor */
        bool GetPropTargetStatuses(Sid::Binary& target_statuses);
        
        /** Sms::P_BODY property get accessor */
        bool GetPropBody(Sid::String& body);
        
        /** Sms::P_CHATMSG_ID property get accessor */
        bool GetPropChatmsgId(MessageRef& chatmsg_id);
        
};


/*! \class Transfer skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  Transfer in this context refers to transferring (sending/receiving) files among Skype Contacts, not transferring a
 *  call to another Skype or PSTN Contact. This class includes file transfer-specific properties and methods, such as
 *  FILESIZE, BYTESPERSECOND, Pause and Resume. Recipients of these file transfers must explicitly accept (or decline)
 *  any incoming transfer. @brief Transfer instances represent files being sent and received within a Conversation
 *  context. Each Transfer instance represents a single file transfer - if a conversation has multiple remote
 *  participants, a separate Transfer instance must be instantiated for each remote participant (a Transfer instance is
 *  not instantiated for the sender). @n  @n Transfer instances cannot be instantiated directly. Instead, you initiate a
 *  file transfer by invoking Conversation::PostFiles. This instantiates a Message instance of type POSTED_FILES, which
 *  is added to the Conversation for all the participants (including the sender). The Transfer instance is associated
 *  with @n this Message instance, and the Message::P_BODY_XML looks like this: @n  @n @code @n Some text<files
 *  alt=""><file size="2336020" index="0">test.zip</file></files>  @n @endcode @n  @n To put it another way, the object
 *  chain goes like this: @n @code @n Conversation->Message->Transfer  @n @endcode @n  @n The first part of the message
 *  (before the files section) comes from the Conversation::PostFiles body argument. For each file in the message, a
 *  file section is provided with three fields: @li file size in bytes @li index - more or less arbitrary order in which
 *  the transfers should be displayed in the UI @li file name. @n  @n For practical purposes, the Message::P_BODY_XML
 *  property is not all that useful in this context. The Transfer instances, however, contain the state and progress
 *  feedback for your UI. You retrieve these Transfer instances using Message::GetTransfers method. Since the sender can
 *  post multiple files to @n a Conversation using the same Message, Message:GetTransfers retrieves a list of Transfer
 *  instances - one per file per recipient. @n  @n You can determine the direction of particular Transfer instance by
 *  querying Transfer::P_TYPE (INCOMING/OUTGOING).  @n  @n You can implement a progress indicator by monitoring
 *  Transfer::P_BYTESTRANSFERRED. Note that when testing this on your local network, you will most likely catch these
 *  property change events at the beginning and the end of the transfer only - which does not look like too useful.
 *  However, for non-local network transfers where the transfer speeds are in the neighborhood of 200-300 KB per second,
 *  you should consider implementing progress feedback as being mandatory and expect to catch multiple property change
 *  events for all but the smallest files. @n  @n Another property of interest is Transfer::P_STATUS. The normal
 *  transfer status sequence during successful outgoing transfer is this: @li TRANSFER STATUS -> NEW @li TRANSFER STATUS
 *  -> WAITING_FOR_ACCEPT @li TRANSFER STATUS -> CONNECTING @li TRANSFER STATUS -> TRANSFERRING @li TRANSFER STATUS ->
 *  CONNECTING @li TRANSFER STATUS -> COMPLETED @n  @n The list of all terminal Transfer statuses is: @li COMPLETED @li
 *  FAILED @li CANCELLED @li CANCELLED_BY_REMOTE @n  @n In-progress transfers can be canceled with Transfer::Cancel and
 *  paused/resumed with Transfer::Pause and Transfer::Resume. For transfers that complete with a status of FAILED, your
 *  UI should provide feedback based on the value of Transfer::P_FAILUREREASON. @n  @n Incoming transfers, once
 *  accepted, overwrite existing files with the same name. Before accepting an incoming file transfer, @n your UI should
 *  prompt the recipient to: @li accept or decline the file @li if accepted, specify the directory of here to save the
 *  file (with a pre-filled default) @li if accepted and a file having the same name already exists at the specified
 *  destination, your UI should prompt for confirmation to overwrite and provide a way to alter the file name before
 *  accepting it @n  @n Similarly, your UI should verify the existence of outgoing files prior to invoking
 *  Conversation::PostFiles. @n  @n Note that you should provide both Conversation::PostFiles and Transfer::Accept
 *  methods fully-qualified paths. Otherwise, the paths will be assumed to be relative to the path of SkypeKit runtime,
 *  since the methods are actually executed in the runtime context. @n 
 */
class Transfer : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        Transfer(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~Transfer();
        typedef TransferRef Ref;
        typedef TransferRefs Refs;
        enum { MODULE_ID = 6 };
        TransferRef ref() { 
                return TransferRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the Transfer class */
        enum PROPERTY {
                P_TYPE = 80                     /*!< INCOMING / OUTGOING @n , type: TYPE*/,
                P_PARTNER_HANDLE                /*!< Skype Name of the remote party of the file transfer. If a file is posted in a conversation with more than one participant, Transfer objects are created for each of them - so a transfer is always to one single remote target. @n , type: Sid::String*/,
                P_PARTNER_DISPNAME              /*!< Display name of the remote participant. @n , type: Sid::String*/,
                P_STATUS                        /*!< Current state of the transfer @n , type: STATUS*/,
                P_FAILUREREASON                 /*!< Set whenever P_STATUS transitions to FAILED. @n , type: FAILUREREASON*/,
                P_STARTTIME                     /*!< UNIX timestamp of when this Transfer instance was instantiated, not when the transfer process actually started (was accepted from receiver side). Do not use this property when calculate the data transfer speed! Instead, monitor changes to P_BYTESPERSECOND. @n , type: uint*/,
                P_FINISHTIME                    /*!< UNIX timestamp of when this Transfer COMPLETED or FAILED. This property is never set if the receiving side (local or remote) canceled the transfer. @n , type: uint*/,
                P_FILEPATH                      /*!< The path -and- filename of the file being transfered (typically fully qualified). For the receiver, SkypeKit sets this property upon acceptance of the incoming transfer. If not fully qualified, the path is assumed to be relative to the path of the SkypeKit runtime. @n , type: Sid::String*/,
                P_FILENAME                      /*!< The filename -only- of the file being transfered. The receiver side can use this property to pre-populate relevant UI components while prompting the user to accept the incoming transfer. @n , type: Sid::String*/,
                P_FILESIZE                      /*!< The size of the file being transferred in bytes. Depending on the magnitude of this value, your UI might want to display the size in terms of kilobytes or even megabytes. @n , type: Sid::String*/,
                P_BYTESTRANSFERRED              /*!< The number of bytes already transferred. Calculate the percentage of the file transferred so far as: @n @code @n P_BYTESTRANSFERRED / (P_FILESIZE / 100);  @n @endcode @n  @n Use float variables to avoid problems with files smaller than 100 bytes! @n , type: Sid::String*/,
                P_BYTESPERSECOND                /*!< Current data transfer speed in bytes per second. Typically, your UI will want to display this value as kilobytes per second (KBps). @n , type: uint*/,
                P_CHATMSG_GUID                  /*!< The "global ID" of this Transfer's associated Message instance. GUIDs are shared across Skype client instances and across all users that can see this Message. @n , type: Sid::Binary*/,
                P_CHATMSG_INDEX                 /*!< A more or less arbitrary index for ordering multiple file transfers within the UI. @n , type: uint*/,
                P_CONVO_ID = 98                 /*!< The "global ID" of this Transfer's associated Conversation (as chained through its associated Message). GUIDs are shared across Skype client instances and across all users that can see this Conversation. @n  @n Note that currently SkypeKit sets this property for INCOMING file transfers only and returns 0 (zero) for all sending side transfers. This is a known bug. @n , type: ConversationRef*/
        };
        
        enum TYPE {
                INCOMING = 1,
                OUTGOING
        };
        
        /** Recognized values for the P_STATUS property. Reflects current state of this Transfer. @n  */
        enum STATUS {
                NEW = 0                       /*!< The file has either not been posted (sent) (OUTGOING), or not accepted (received) (INCOMING). @n */,
                CONNECTING                    /*!< A temporary state that transitions either into TRANSFERRING (relayed or direct) or to FAILED. For unknown reasons, outgoing transfers tend go into this state twice - immediately before the actual data transfer starts and immediately after it ends. @n */,
                WAITING_FOR_ACCEPT            /*!< The files have been posted but the recipient has not yet accepted (or has declined) the transfer. @n */,
                TRANSFERRING                  /*!< The transfer has been accepted and file data is being sent/received. Periodic updates of P_BYTESTRANSFERRED property should occur. @n */,
                TRANSFERRING_OVER_RELAY       /*!< The transfer has been accepted and file data is being sent/received but is going over at least one relay. Since relayed transfers tend to be significantly slower than direct transfers, you might want to differentiate the two in your UI and notify the user that relayed transfer typically take significantly longer to finish. @n */,
                PAUSED                        /*!< The local user (either the sender or a receiver) has paused the transfer. @n */,
                REMOTELY_PAUSED               /*!< A remote user has paused the transfer. For senders, a receiver has paused the transfer; for receivers, the sender has paused the transfer. @n */,
                CANCELLED                     /*!< Local side (either sender or receiver) has canceled the transfer. This is a final state of the STATE property. @n */,
                COMPLETED                     /*!< File transfer has completed. This is a terminal state. @n */,
                FAILED                        /*!< File transfer has failed. This is a terminal state. UI should provide feedback, based on value of P_FAILUREREASON. @n */,
                PLACEHOLDER                   /*!< Transfer whose existence has been hinted by corresponding chat message, but which is yet to arrive. @n */,
                OFFER_FROM_OTHER_INSTANCE     /*!< Outgoing transfer object from another instance of the same account as current local login, running on another system. Hinted through chat message - only implies an offer was made; not necessarily accepted, failed, or completed.  @n */,
                CANCELLED_BY_REMOTE           /*!< Remote side (either sender or receiver) has canceled the transfer. This is a final state of the STATE property. @n */
        };
        
        enum FAILUREREASON {
                SENDER_NOT_AUTHORISED = 1,
                REMOTELY_CANCELLED,
                FAILED_READ,
                FAILED_REMOTE_READ,
                FAILED_WRITE,
                FAILED_REMOTE_WRITE,
                REMOTE_DOES_NOT_SUPPORT_FT,
                REMOTE_OFFLINE_FOR_TOO_LONG,
                TOO_MANY_PARALLEL,
                PLACEHOLDER_TIMEOUT
        };
        
        /** Accepts an incoming file transfer and saves it to specified file on the local file system. If the specified file exists, SkypeKit will silently overwrite it. Your UI should prompting the user for confirmation in this case and provide a means for canceling the file transfer or specifying a different target file. @n  */
        bool Accept(
                const Sid::Filename& filenameWithPath, /*!< Where on the local file system to save the file being transferred. Note that you should specify the path as being fully-qualified. Otherwise, SkypeKit will be assume it to be relative to the SkypeKit runtime path, since the method is actually executed in the runtime context. @n */
                bool& success /*!< Set to true if the specified target file was successfully created on the local file system -and- the initial write(s) succeeded. However, the transfer itself can subsequently fail before completion due to its being canceled (either locally or remotely), network failure, local file system space/write issues, and so forth. @n */
        );
        
        /** Temporarily pauses an in-progress incoming or outgoing file transfer. For incoming transfers, only this affects the sender and the invoking recipient only. For outgoing transfers, this affects the sender and all recipients. @n  */
        bool Pause(
        );
        
        /** Resumes a previously paused file transfer. @n  */
        bool Resume(
        );
        
        /** Cancels an in-progress file transfer. Transfer::STATUS will transition to CANCELLED for incoming file transfers and to CANCELLED_BY_REMOTE for outgoing transfers. @n  */
        bool Cancel(
        );
        
        /** Transfer::P_TYPE property get accessor */
        bool GetPropType(TYPE& type);
        
        /** Transfer::P_PARTNER_HANDLE property get accessor */
        bool GetPropPartnerHandle(Sid::String& partner_handle);
        
        /** Transfer::P_PARTNER_DISPNAME property get accessor */
        bool GetPropPartnerDispname(Sid::String& partner_dispname);
        
        /** Transfer::P_STATUS property get accessor */
        bool GetPropStatus(STATUS& status);
        
        /** Transfer::P_FAILUREREASON property get accessor */
        bool GetPropFailurereason(FAILUREREASON& failurereason);
        
        /** Transfer::P_STARTTIME property get accessor */
        bool GetPropStarttime(uint& starttime);
        
        /** Transfer::P_FINISHTIME property get accessor */
        bool GetPropFinishtime(uint& finishtime);
        
        /** Transfer::P_FILEPATH property get accessor */
        bool GetPropFilepath(Sid::String& filepath);
        
        /** Transfer::P_FILENAME property get accessor */
        bool GetPropFilename(Sid::String& filename);
        
        /** Transfer::P_FILESIZE property get accessor */
        bool GetPropFilesize(Sid::String& filesize);
        
        /** Transfer::P_BYTESTRANSFERRED property get accessor */
        bool GetPropBytestransferred(Sid::String& bytestransferred);
        
        /** Transfer::P_BYTESPERSECOND property get accessor */
        bool GetPropBytespersecond(uint& bytespersecond);
        
        /** Transfer::P_CHATMSG_GUID property get accessor */
        bool GetPropChatmsgGuid(Sid::Binary& chatmsg_guid);
        
        /** Transfer::P_CHATMSG_INDEX property get accessor */
        bool GetPropChatmsgIndex(uint& chatmsg_index);
        
        /** Transfer::P_CONVO_ID property get accessor */
        bool GetPropConvoId(ConversationRef& convo_id);
        
};


/*! \class Account skype-embedded_2.h "skype-embedded_2.h"
 *  \brief
 *  \nosubgrouping
 *  Represents a local account. Encapsulates methods for Skype account creation, login and logout as well as account
 *  profile setting properties. NB! Unlike all the other SkypeKit classes, most of the Account class properties are
 *  actually read-write. @brief In fact, there are two sorts of Account's read-write properties: server-side properties
 *  and local properties. Different setter methods need to be used for those two kinds. @n  @n The subset of server-side
 *  properties consists of all the policy properties (everything with _POLICY suffix) that are all of type int and can
 *  be set with SetServersideIntProperty method. There is currently only one server-side string property -
 *  OFFLINE_CALLFORWARD which can be set with SetServersideStrProperty setter. @n  @n The set of writeable account
 *  profile properties (local profile) is as follows; @li P_FULLNAME, @li P_BIRTHDAY, @li P_GENDER, @li P_LANGUAGES, @li
 *  P_COUNTRY, @li P_PROVINCE, @li P_CITY, @li P_PHONE_HOME, @li P_PHONE_OFFICE, @li P_PHONE_MOBILE, @li P_EMAILS, @li
 *  P_HOMEPAGE, @li P_ABOUT, @li P_MOOD_TEXT, @li P_TIMEZONE, @li P_AVATAR_IMAGE, @li P_RICH_MOOD_TEXT @n  @n These can
 *  be set with SetIntProperty, SetStrProperty and SetBinProperty setter methods. Note that there are special methods
 *  for changing account online status (availability) and enabling/disabling auto-login - SetAvailability and
 *  SetSavePwd. @n  @n Note that to set Account properties, you that Account needs to be logged in. Generally,
 *  assumption is that the first thing you do with an Account object after you retrieve it with Skype::GetAccount is
 *  login, with exception of creating a new account. @n  @n The Account class has no default constructor and creating an
 *  Account instance is not the same as creating a Skype account. To create a Skype account: @n  @li obtain an Account
 *  instance by invoking Skype::GetAccount. This automatically sets P_SKYPENAME. @li set any additional profile-related
 *  properties. Skype recommends that you minimally set the following: @li their email address so that they can retrieve
 *  a lost password (P_EMAILS) @li the user's full name (P_FULLNAME) @li at least one phone number (P_PHONE_HOME,
 *  P_PHONE_HOME, P_PHONE_OFFICE) @li invoke Account::Register to actually create the account @n 
 */
class Account : public SEObject
{
        /** \cond INTERNAL */
        friend class Skype;
protected:
        Account(unsigned int oid, SERootObject* root);
        /** \endcond */
public:
        virtual ~Account();
        typedef AccountRef Ref;
        typedef AccountRefs Refs;
        enum { MODULE_ID = 5 };
        AccountRef ref() { 
                return AccountRef(object_id); 
        }
        
private:
        String getNamespace() const;
        const PMAP* getPMap() const;
        const EMAP* getEMap() const;
        unsigned int moduleID() const;
        void __clear_cache(void*);
        
public:
        /** Properties of the Account class */
        enum PROPERTY {
                P_STATUS = 70                   /*!< Account::STATUS, type: STATUS*/,
                P_PWDCHANGESTATUS               /*!< Account::PWDCHANGESTATUS, type: PWDCHANGESTATUS*/,
                P_LOGOUTREASON = 73             /*!< This property should only be examined when Account::P_STATUS is LOGGED_OUT or LOGGED_OUT_AND_PWD_SAVED. That is, you should not monitor changes to this property in Account::OnChange callback, other than after already having checked that P_STATUS property has appropriate value. The reason for this is that the LOGOUTREASON does not get reset before the next login attempt. For example: if a user tried to log in with a wrong password, the LOGOUTREASON gets set to INCORRECT_PASSWORD. Now, if the user tries to log in again, and yet again submits an incorrect password, the value of the LOGOUTREASON does not change anymore, because it already is set to INCORRECT_PASSWORD. Consequently, Account::OnChange will not fire in this case. @n , type: LOGOUTREASON*/,
                P_COMMITSTATUS = 78             /*!< Account::COMMITSTATUS, type: COMMITSTATUS*/,
                P_SUGGESTED_SKYPENAME = 72      /*!< suggested skypenames, semi-colon separated. present if logoutreason==SKYPENAME_TAKEN, type: Sid::String*/,
                P_SKYPEOUT_BALANCE_CURRENCY = 74/*!< 'EUR', 'USD', etc., type: Sid::String*/,
                P_SKYPEOUT_BALANCE              /*!< balance in 'cents', type: uint*/,
                P_SKYPEOUT_PRECISION = 804      /*!< decimal points in ACCOUNT_SKYPEOUT_BALANCE, type: uint*/,
                P_SKYPEIN_NUMBERS = 76          /*!< space-separated list of skypein numbers, type: Sid::String*/,
                P_CBLSYNCSTATUS = 79            /*!< Account::CBLSYNCSTATUS, type: CBLSYNCSTATUS*/,
                P_OFFLINE_CALLFORWARD = 77      /*!< space-separated list of 'begSecond,endSecond,identity' tokens, type: Sid::String*/,
                P_CHAT_POLICY = 160             /*!< Server-side account property, use SetServerside*Property() to set, type: CHATPOLICY*/,
                P_SKYPE_CALL_POLICY             /*!< Server-side account property, use SetServerside*Property() to set, type: SKYPECALLPOLICY*/,
                P_PSTN_CALL_POLICY              /*!< Server-side account property, use SetServerside*Property() to set, type: PSTNCALLPOLICY*/,
                P_AVATAR_POLICY                 /*!< Server-side account property, use SetServerside*Property() to set, type: AVATARPOLICY*/,
                P_BUDDYCOUNT_POLICY             /*!< Server-side account property, use SetServerside*Property() to set, type: BUDDYCOUNTPOLICY*/,
                P_TIMEZONE_POLICY               /*!< Server-side account property, use SetServerside*Property() to set, type: TIMEZONEPOLICY*/,
                P_WEBPRESENCE_POLICY            /*!< Server-side account property, use SetServerside*Property() to set, type: WEBPRESENCEPOLICY*/,
                P_PHONENUMBERS_POLICY = 168     /*!< Server-side account property, use SetServerside*Property() to set, type: PHONENUMBERSPOLICY*/,
                P_VOICEMAIL_POLICY              /*!< Server-side account property, use SetServerside*Property() to set, type: VOICEMAILPOLICY*/,
                P_PARTNER_OPTEDOUT = 773        /*!< Alerts: opted out partner id's, space separated, type: Sid::String*/,
                P_SERVICE_PROVIDER_INFO = 800   /*!< service information if the user is a paid service provider, type: Sid::String*/,
                P_REGISTRATION_TIMESTAMP        /*!< NB! Unlike your common UNIX timestamps, the registration_timestamp is special, as it counts MINUTES rather than seconds, from Epoch (January 1, 1970) @n , type: uint*/,
                P_NR_OF_OTHER_INSTANCES         /*!< number of times this user is logged in from other computers, type: uint*/,
                P_SKYPENAME = 4                 /*!< type: Sid::String*/,
                P_FULLNAME                      /*!< type: Sid::String*/,
                P_BIRTHDAY = 7                  /*!< YYYYMMDD, type: uint*/,
                P_GENDER                        /*!< 1-male, 2-female, type: uint*/,
                P_LANGUAGES                     /*!< ISO language codes, space-separated, type: Sid::String*/,
                P_COUNTRY                       /*!< ISO country code, type: Sid::String*/,
                P_PROVINCE                      /*!< type: Sid::String*/,
                P_CITY                          /*!< type: Sid::String*/,
                P_PHONE_HOME                    /*!< NB! string not integer, type: Sid::String*/,
                P_PHONE_OFFICE                  /*!< type: Sid::String*/,
                P_PHONE_MOBILE                  /*!< type: Sid::String*/,
                P_EMAILS                        /*!< Space separated, type: Sid::String*/,
                P_HOMEPAGE                      /*!< type: Sid::String*/,
                P_ABOUT                         /*!< arbitrary introductory text, type: Sid::String*/,
                P_PROFILE_TIMESTAMP             /*!< UNIX timestamp of last profile change, type: uint*/,
                P_MOOD_TEXT = 26                /*!< Personal mood text (visible to authorised users only). Max length 300 bytes. @n , type: Sid::String*/,
                P_TIMEZONE                      /*!< 24*3600+diff_to_UTC_in_seconds. nb! changes with DST, type: uint*/,
                P_NROF_AUTHED_BUDDIES           /*!< Count of this user's authorized contacts. @n , type: uint*/,
                P_AVAILABILITY = 34             /*!< Contact::AVAILABILITY, type: Contact::AVAILABILITY*/,
                P_AVATAR_IMAGE = 37             /*!< Account avatar picture can be set with Account::SetBinProperty method. The contact avatar picture is limited to max 96x96 pixels and 32000 bytes. If the submitted avatar picture exceeds either of these size limits, it is the responsibility of your client to scale the image down to appropriate size. @n  @n The avatar pictures have to be in JPG format. A SkypeKit client can enable the user to set the Account avatar in other picture formats, in which case the picture should be converted to JPG before submitting it.  @n  @n In any case, the avatar picture should be pre-validated with the Skype::ValidateAvatar method. @n , type: Sid::Binary*/,
                P_AVATAR_TIMESTAMP = 182        /*!< UNIX timestamp of when current avatar was set, type: uint*/,
                P_MOOD_TIMESTAMP                /*!< UNIX timestamp of when current mood was set, type: uint*/,
                P_RICH_MOOD_TEXT = 205          /*!< XML version of CONTACT_MOOD_TEXT. Max length 1000 bytes. @n , type: Sid::String*/
        };
        
        enum STATUS {
                LOGGED_OUT = 1,
                LOGGED_OUT_AND_PWD_SAVED      /*!< the account is logged out, but password is not needed for re-login*/,
                CONNECTING_TO_P2P             /*!< connecting to P2P network*/,
                CONNECTING_TO_SERVER          /*!< connecting to login server*/,
                LOGGING_IN                    /*!< waiting for response from server*/,
                INITIALIZING                  /*!< response OK. initialising account-specific lib structures*/,
                LOGGED_IN                     /*!< alright, we're good to go!*/,
                LOGGING_OUT                   /*!< Logout() has been called but not processed yet*/
        };
        
        bool GetStatusWithProgress(
                STATUS& status, /*!< Current value of this account's P_STATUS property*/
                uint& progress /*!< This argument returns 0. @n */
        );
        
        /** Recognized values for the P_CBLSYNCSTATUS property. CBL stands for Central Buddy List. In principle, this property and its states can be ignored by most UI developers. However, it can help to optimize UI buildup and behaviour, particularly in case of limited resources (such as mobile devices). @n  @n CBL is used to backup your contact list, contact groups, and profile information, and also used to synchronize this information with other Skype instances of your account (i.e. on another device). CBL sync can occur both during login and during normal operation. Note that CBL synchronization does not take place immediately after an Account property is changed. A delay between the first property change and CBL sync initiation enables the client to accumulate changes and do the synchronization in bulk. @n  @n Clients with limited resources might want to wait for CBL_IN_SYNC status before generating their UI's contact list representation. Otherwise it might be forced to redraw the contact list multiple times, as new updates get retrieved from the server-side. Similarly, applications that modify an account's mood message might want to know when the P_MOOD_TEXT or P_RICH_MOOD_TEXT property is synchronized to the server. Note that this sync is only for CBL and other logged in instances of the same account - other contacts will receive the mood message update directly. @n  */
        enum CBLSYNCSTATUS {
                CBL_INITIALIZING = 0          /*!< status is not clear (yet)*/,
                CBL_INITIAL_SYNC_PENDING      /*!< first sync with empty profile*/,
                CBL_SYNC_PENDING              /*!< Account properties are considered to be out of sync with CBL - attempt at synchronization is imminent. You might wish to wait with updating UI components that display the data that is about to change anyway.*/,
                CBL_SYNC_IN_PROGRESS          /*!< CBL synchronization is currently taking place.*/,
                CBL_IN_SYNC                   /*!< Account properties are up-to-date.*/,
                CBL_SYNC_FAILED               /*!< CBL sync has. Another attempt will be made after several minutes. If a second attempt also fails, subsequent attempts at synchronization will be made at ever increasing intervals.*/,
                CBL_REMOTE_SYNC_PENDING       /*!< we have received a hint that there is a remote data change in CBL*/
        };
        
        /** Recognized values for the Account::P_LOGOUTREASON. Note that this property should only be examined when Account::P_STATUS is LOGGED_OUT or LOGGED_OUT_AND_PWD_SAVED. That is, you should not monitor changes to this property in Account::OnChange callback, other than after already having checked that P_STATUS property has appropriate value. The reason for this is that the LOGOUTREASON does not get reset before the next login attempt. For example: if a user tried to log in with a wrong password, the LOGOUTREASON gets set to INCORRECT_PASSWORD. Now, if the user tries to log in again, and yet again submits an incorrect password, the value of the LOGOUTREASON does not change anymore, because it already is set to INCORRECT_PASSWORD. Consequently, Account::OnChange will not fire in this case. @n  */
        enum LOGOUTREASON {
                LOGOUT_CALLED = 1             /*!< manual logout (or unknown reason from previous session)*/,
                HTTPS_PROXY_AUTH_FAILED       /*!< sync errors at login/registration*/,
                SOCKS_PROXY_AUTH_FAILED       /*!< sync errors at login/registration*/,
                P2P_CONNECT_FAILED            /*!< sync errors at login/registration*/,
                SERVER_CONNECT_FAILED         /*!< sync errors at login/registration*/,
                SERVER_OVERLOADED             /*!< sync errors at login/registration*/,
                DB_IN_USE                     /*!< sync errors at login/registration*/,
                INVALID_SKYPENAME             /*!< sync errors at registration*/,
                INVALID_EMAIL                 /*!< sync errors at registration*/,
                UNACCEPTABLE_PASSWORD         /*!< sync errors at registration*/,
                SKYPENAME_TAKEN               /*!< sync errors at registration*/,
                REJECTED_AS_UNDERAGE          /*!< sync errors at registration*/,
                NO_SUCH_IDENTITY              /*!< sync errors at login*/,
                INCORRECT_PASSWORD            /*!< sync errors at login*/,
                TOO_MANY_LOGIN_ATTEMPTS       /*!< sync errors at login*/,
                PASSWORD_HAS_CHANGED          /*!< async errors (can happen anytime while logged in)*/,
                PERIODIC_UIC_UPDATE_FAILED    /*!< async errors (can happen anytime while logged in)*/,
                DB_DISK_FULL                  /*!< async errors (can happen anytime while logged in)*/,
                DB_IO_ERROR                   /*!< async errors (can happen anytime while logged in)*/,
                DB_CORRUPT                    /*!< async errors (can happen anytime while logged in)*/,
                DB_FAILURE                    /*!< deprecated (superceded by more detailed DB_* errors)*/,
                INVALID_APP_ID                /*!< platform sdk*/,
                APP_ID_FAILURE                /*!< platform sdk*/,
                UNSUPPORTED_VERSION           /*!< forced upgrade/discontinuation*/
        };
        
        /** Login an auto-login enabled account (P_STATUS is LOGGED_OUT_AND_PWD_SAVED) and optionally set the availability, for example login in as Contact::DO_NOT_DISTURB. To find out whether there is an auto-login enabled account available, you can use Skype::GetDefaultAccountName to retrieve the skypename of that account. You can then get an Account instance for login by invoking Skype::GetAccount. @n  @n If no accounts with stored login credentials are available (GetDefaultAccountName returns an empty string), then you will have to prompt the user for account name and password and then use LoginWithPassword. Account name field in the UI can be pre-populated with strings retrieved with Skype::GetExistingAccounts @n  */
        bool Login(
                const Contact::AVAILABILITY setAvailabilityTo = Contact::UNKNOWN /*!< force this account's initial online status to the specified Contact::AVAILABILITY value.*/
        );
        
        /**   @n Login in an account by specifying its password. To retrieve an account instance for login, invoke Skype::GetAccount. @n  */
        bool LoginWithPassword(
                const Sid::String& password, /*!< Password string (plaintext) for this account, which should be pre-validated (Skype::ValidatePassword) @n */
                const bool savePwd = false, /*!< @li true: Saves the password, ensuring that auto-login is enabled. @li false (default): Does not save the password, and so the user might not be able to effect auto-login until they explicitly invoke Account::SetPasswordSaved(true).*/
                const bool saveDataLocally = true /*!< For internal use only.*/
        );
        
        /** This command can be used to create a new Skype account, based on the Account object. When successful, this command will also log in with the newly created account. If the new account registration was not successful, Account status property will change to LOGGED_OUT. A common reason for registration failures is that the an account with that name already exists. In that case, Account LOGOUT_REASON will be set to SKYPENAME_TAKEN. Also, Account SUGGESTED_SKYPENAME property will be set to a list of similar but still available skypenames that could be used instead. The property will contain up to 5 semicolon-separated alternative skypenames. In if no suggested skypenames were available, then this property will contain an empty string. @n NB! You should pre-validate your P_SKYPENAME value and any email string (Skype::ValidateProfileString) prior to invoking this method. @n  */
        bool Register(
                const Sid::String& password, /*!< Password string (plaintext) for this account, which should be pre-validated (Skype::ValidatePassword)*/
                const bool savePwd = false, /*!< @li true: Saves the password and enables auto-login. @li false (default): Does not save the password, and the user needs to be prompted for password on the next login attempt.*/
                const bool saveDataLocally = true, /*!< For internal use only.*/
                const Sid::String email = "", /*!< An email address for retrieving lost passwords and receiving news and information from Skype.*/
                const bool allowSpam = false /*!< enable/disable news and information from Skype being sent to account's e-mail.*/
        );
        
        /** Logs out current account. Note that calling this on client application exit is optional. @n  */
        bool Logout(
                const bool clearSavedPwd = false /*!< @li true: Clears any saved password use with auto-login and so disables auto-login until you explicitly invoke Account::SetPasswordSaved(true). @li false (default): Does not clear any saved password and so does not affect existing auto-login behavior.*/
        );
        
        /** Recognized values for the P_PWDCHANGESTATUS property that provides information on whether a password change succeeded or failed, giving detailed failure reason. After successful return from the Change Password method, clients should monitor the P_PWDCHANGESTATUS property changes. @li PWD_CHANGING - consider displaying an "in progress" indicator and continue polling @li PWD_OK - consider displaying an updated indicator and stop polling @li PWD_OK_BUT_CHANGE_SUGGESTED - consider displaying an updated indicator, along with a recommendation to change again to a stronger password. @n  */
        enum PWDCHANGESTATUS {
                PWD_OK = 0                    /*!< Password change succeeded. @n */,
                PWD_CHANGING                  /*!< Password change is in progress. @n */,
                PWD_INVALID_OLD_PASSWORD      /*!< Old password was incorrect. @n */,
                PWD_SERVER_CONNECT_FAILED     /*!< Failed to verify password because of no connection to server. @n */,
                PWD_OK_BUT_CHANGE_SUGGESTED   /*!< Password was set but server didn't like it much. @n */,
                PWD_MUST_DIFFER_FROM_OLD      /*!< New password was exactly the same as old one. @n */,
                PWD_INVALID_NEW_PWD           /*!< The new password was unacceptable. (too short, too simple, etc.) @n */,
                PWD_MUST_LOG_IN_TO_CHANGE     /*!< Account was currently not logged in. @n */
        };
        
        /** Changes this account's password. Returns false if the change failed. NB! You should pre-validate your password strings (Skype::ValidatePassword) and ensure that they are different prior to invoking this method. @n  */
        bool ChangePassword(
                const Sid::String& oldPassword, /*!< "Current password string (plaintext) for this account*/
                const Sid::String& newPassword, /*!< New password string (plaintext) for this account*/
                const bool savePwd = false /*!< @li true: Saves the new password and enables auto-login. @li false (default): Clears any existing saved password and so the user cannot effect auto-login until they explicitly invoke Account::SetPasswordSaved(true)*/
        );
        
        /** save or clear credentials for auto-login whilst already logged in */
        bool SetPasswordSaved(
                const bool& savePwd
        );
        
        /** Setter for integer (and enum) server-side properties. For a list of writeable server-side properties, see the detailed description of the Account class. @n  */
        bool SetServersideIntProperty(
                const int& propKey,
                const uint& value
        );
        
        /** Setter for String server-side properties. For a list of writeable server-side properties, see the detailed description of the Account class. @n  */
        bool SetServersideStrProperty(
                const int& propKey,
                const Sid::String& value
        );
        
        /** The list of possible values of Account class COMMITSTATUS property. Note that this property and its values have nothing to do with (automatic) CBL synchronization. Rather, the COMMITSTATUS reflects commit status to account's server side properties initiated with calls to Account class SetServersideIntProperty and Account class SetServersideStrProperty methods. After those methods, your client UI may want to wait until the COMMITSTATUS becomes COMMITTING_TO_SERVER followed by COMMITTED and inform the user if the value becomes COMMIT_FAILED. SetServerside<type>Property methods are used for writing privacy policy related and call forwarding related Account properties to the server. Unlike CBL synchronization, those updates are executed immediately. @n  */
        enum COMMITSTATUS {
                COMMITTED = 1                 /*!< No pending updates to the server. @n */,
                COMMITTING_TO_SERVER          /*!< Update to the server in progress. @n */,
                COMMIT_FAILED                 /*!< Server update has failed. @n */
        };
        
        /** Cancels an attempt to commit a server-side P_XXX_POLICY or the P_OFFLINE_CALLFORWARD server-side property. Invoking this cancellation only makes sense whilst the P_COMMITTSTATUS is in COMMITTING_TO_SERVER state. @n  */
        bool CancelServerCommit(
        );
        
        /** Recognized values for the P_CHAT_POLICY property that controls whether non-authorized users can initiate text chat with the currently logged in account. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntProperty @n  */
        enum CHATPOLICY {
                EVERYONE_CAN_ADD = 0          /*!< Unauthorized contacts can initiate text chat. @n */,
                BUDDIES_OR_AUTHORIZED_CAN_ADD = 2/*!< Only authorized contacts can initiate chat (default policy). @n */
        };
        
        /** Recognized values for the P_SKYPECALLPOLICY property that controls acceptance of incoming Skype calls. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side. @n  */
        enum SKYPECALLPOLICY {
                EVERYONE_CAN_CALL = 0         /*!< Skype calls accepted from unauthorized contacts.*/,
                BUDDIES_OR_AUTHORIZED_CAN_CALL = 2/*!< Skype calls not accepted from unauthorized contacts.*/
        };
        
        /** Recognized values for the P_PSTNCALLPOLICY property that controls whether (and from whom) this account accepts incoming PSTN calls. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntProperty @n  */
        enum PSTNCALLPOLICY {
                ALL_NUMBERS_CAN_CALL = 0      /*!< All incoming PSTN calls are accepted.*/,
                DISCLOSED_NUMBERS_CAN_CALL    /*!< Only PSTN calls that report caller ID are accepted.*/,
                BUDDY_NUMBERS_CAN_CALL        /*!< Only calls from PSTN numbers found in local contact list are accepted.*/
        };
        
        /** Recognized values for the P_AVATAR_POLICY property that controls whether remote contacts can view local account's avatar image. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side. @n Note that setting account's AVATAR_POLICY to BUDDIES_OR_AUTHORIZED_CAN_SEE does not guarantee that remote users will be able to immediately retrieve the avatar picture via corresponding Contact object. Avatar changes propagate between clients only when direct sessions between clients are established. Direct sessions are established during live sessions or whilst online contacts are engaged in chat. @n  */
        enum AVATARPOLICY {
                BUDDIES_OR_AUTHORIZED_CAN_SEE = 0/*!< Only authorized remote users can see this user's avatar image*/,
                EVERYONE_CAN_SEE = 2          /*!< Everyone can see this user's avatar image, once the contact/account avatar property has been synchronized during a direct session. The avatar image may also become viewable on some Skype Web-based services.*/
        };
        
        /** Recognized values for the P_BUDDYCOUNT_POLICY property that controls whether the number of this user's authorized contacts is visible to other users, either through Account::GetPropNrofAuthedBuddies or Contact::GetPropNrofAuthedBuddies when those instances reference this user. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntProperty, like this: @n account->SetServersideIntProperty(Account::P_BUDDYCOUNT_POLICY, Account::DISCLOSE_TO_AUTHORIZED); @n account->SetServersideIntProperty(Account::P_BUDDYCOUNT_POLICY, Account::DISCLOSE_TO_NOONE ); @n  */
        enum BUDDYCOUNTPOLICY {
                DISCLOSE_TO_AUTHORIZED = 0    /*!< Authorized remote users can retrieve the number of this user's authorized contacts (Contact::P_NROF_AUTHED_BUDDIES)*/,
                DISCLOSE_TO_NOONE             /*!< No remote user - regardless their authorization status - can retrieve the number of this user's authorized contacts. Account::GetPropNrofAuthedBuddies and Contact::GetPropNrofAuthedBuddies will always return 0*/
        };
        
        /** Recognized values for the P_TIMEZONEPOLICY property that sets the rules for timezone offset so remote clients can determine your local time. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side. */
        enum TIMEZONEPOLICY {
                TZ_AUTOMATIC = 0              /*!< Account's timezone setting is determined automatically. @n */,
                TZ_MANUAL                     /*!< Account's timezone setting is set manually. @n */,
                TZ_UNDISCLOSED                /*!< Remote users will have no idea what your local time is. @n */
        };
        
        /** Recognized values for the P_WEBPRESENCEPOLICY property that controls whether your online status (presence) can be seen using the "Skype buttons" ( http://www.skype.com/share/buttons/ ) embedded in web pages. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side. @n  */
        enum WEBPRESENCEPOLICY {
                WEBPRESENCE_DISABLED = 0      /*!< Disable displaying online status on web for this account.*/,
                WEBPRESENCE_ENABLED           /*!< Enable displaying online status on web for this account.*/
        };
        
        /** Recognized values for the P_PHONENUMBERSPOLICY property that controls whether unauthorized remote users can see associated phone numbers in their UI (for reference, see the different phone number tabs in Windows desktop Client contact view). Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntProperty @n  */
        enum PHONENUMBERSPOLICY {
                PHONENUMBERS_VISIBLE_FOR_BUDDIES = 0/*!< Only authorized contacts can see the phone numbers.*/,
                PHONENUMBERS_VISIBLE_FOR_EVERYONE/*!< Everyone can see the phone numbers.*/
        };
        
        /** Recognized values for the P_VOICEMAILPOLICY property that controls acceptance of incoming voicemail messages. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side. @n  */
        enum VOICEMAILPOLICY {
                VOICEMAIL_ENABLED = 0         /*!< Incoming voicemails enabled.*/,
                VOICEMAIL_DISABLED            /*!< Incoming voicemails disabled.*/
        };
        
        /** Setter for integer properties. For a list of writeable account profile properties, see the detailed description of the Account class. @n  */
        bool SetIntProperty(
                const int& propKey,
                const uint& value
        );
        
        /** Setter for String properties. For a list of writeable account profile properties, see the detailed description of the Account class. NB! You should pre-validate your about and mood message strings (Skype::ValidateProfileString) prior to invoking this method. @n  */
        bool SetStrProperty(
                const int& propKey,
                const Sid::String& value
        );
        
        /** Setter for BLOB properties, such as its avatar image. For a list of writeable account profile properties, see the detailed description of the Account class. NB! You should pre-validate your avatar image (Skype::ValidateAvatar) prior to invoking this method. @n  */
        bool SetBinProperty(
                const int& propKey,
                const Sid::Binary& value
        );
        
        /** Sets online status of the currently logged in account to one of the values from Contact class AVAILABILITY enumerator. @n  */
        bool SetAvailability(
                const Contact::AVAILABILITY& availability /*!< only subset of all contact availabilities allowed*/
        );
        
        /** Transitions the availability of this account's associated user, who is assumed to be logged in. @n  */
        bool SetStandby(
                const bool& standby /*!< @li true: Saves the user's current availability, then sets it to CONTACT::AVAILABILITY.OFFLINE @li false: Reconnects the user and restores their previous availability*/
        );
        
        /** Account capabability statuses are possible values of Contact class CAPABILITY enumerator, when that enumerator is used in context of account. Compared to Contact class CAPABILITYSTATUS enums, Account class CAPABILITYSTATUS has additional items for subscription expiration warnings. @n  */
        enum CAPABILITYSTATUS {
                NO_CAPABILITY = 0             /*!< Capability is not supported by the currently logged in SkypeKit client.*/,
                CAPABILITY_EXISTS             /*!< Capability is supported by the currently logged in SkypeKit client. @n */,
                FIRST_EXPIRY_WARNING          /*!< Support for this capability ends this month (within 30 days) @n */,
                SECOND_EXPIRY_WARNING         /*!< Support for this capability ends this week (within 7 days)  @n */,
                FINAL_EXPIRY_WARNING          /*!< Support for this capability ends today @n */
        };
        
        /** Returns state of a given account capability. Takes Contact class CAPABILITY property as input argument and returns its state and expiration timestamp where appropriate. For example (C++ wrapper, with other wrappers the syntax may vary but the idea is the same) MyAccount.GetCapabilityStatus(Contact::CAPABILITY_SKYPEOUT, Cap, T); will return Account::CAPABILITY_EXISTS if local account has SkypeOut enabled. @n  */
        bool GetCapabilityStatus(
                const Contact::CAPABILITY& capability,
                CAPABILITYSTATUS& status,
                uint& expiryTimestamp
        );
        
        /** Response is empty when called with an inactive or invalid account */
        bool GetSkypenameHash(
                Sid::String& skypenameHash
        );
        
        /** returns verified-by-Skype e-mail for this account if exists and verifiable */
        bool GetVerifiedEmail(
                Sid::String& email
        );
        
        /** returns verified-by-Skype company for this account if exists and verifiable */
        bool GetVerifiedCompany(
                Sid::String& company
        );
        
        /** Deletes all account data stored locally. Does not remove any account data from the server! @n  */
        bool Delete(
        );
        
        /** Account::P_STATUS property get accessor */
        bool GetPropStatus(STATUS& status);
        
        /** Account::P_PWDCHANGESTATUS property get accessor */
        bool GetPropPwdchangestatus(PWDCHANGESTATUS& pwdchangestatus);
        
        /** Account::P_LOGOUTREASON property get accessor */
        bool GetPropLogoutreason(LOGOUTREASON& logoutreason);
        
        /** Account::P_COMMITSTATUS property get accessor */
        bool GetPropCommitstatus(COMMITSTATUS& commitstatus);
        
        /** Account::P_SUGGESTED_SKYPENAME property get accessor */
        bool GetPropSuggestedSkypename(Sid::String& suggested_skypename);
        
        /** Account::P_SKYPEOUT_BALANCE_CURRENCY property get accessor */
        bool GetPropSkypeoutBalanceCurrency(Sid::String& skypeout_balance_currency);
        
        /** Account::P_SKYPEOUT_BALANCE property get accessor */
        bool GetPropSkypeoutBalance(uint& skypeout_balance);
        
        /** Account::P_SKYPEOUT_PRECISION property get accessor */
        bool GetPropSkypeoutPrecision(uint& skypeout_precision);
        
        /** Account::P_SKYPEIN_NUMBERS property get accessor */
        bool GetPropSkypeinNumbers(Sid::String& skypein_numbers);
        
        /** Account::P_CBLSYNCSTATUS property get accessor */
        bool GetPropCblsyncstatus(CBLSYNCSTATUS& cblsyncstatus);
        
        /** Account::P_OFFLINE_CALLFORWARD property get accessor */
        bool GetPropOfflineCallforward(Sid::String& offline_callforward);
        
        /** Account::P_CHAT_POLICY property get accessor */
        bool GetPropChatPolicy(CHATPOLICY& chat_policy);
        
        /** Account::P_SKYPE_CALL_POLICY property get accessor */
        bool GetPropSkypeCallPolicy(SKYPECALLPOLICY& skype_call_policy);
        
        /** Account::P_PSTN_CALL_POLICY property get accessor */
        bool GetPropPstnCallPolicy(PSTNCALLPOLICY& pstn_call_policy);
        
        /** Account::P_AVATAR_POLICY property get accessor */
        bool GetPropAvatarPolicy(AVATARPOLICY& avatar_policy);
        
        /** Account::P_BUDDYCOUNT_POLICY property get accessor */
        bool GetPropBuddycountPolicy(BUDDYCOUNTPOLICY& buddycount_policy);
        
        /** Account::P_TIMEZONE_POLICY property get accessor */
        bool GetPropTimezonePolicy(TIMEZONEPOLICY& timezone_policy);
        
        /** Account::P_WEBPRESENCE_POLICY property get accessor */
        bool GetPropWebpresencePolicy(WEBPRESENCEPOLICY& webpresence_policy);
        
        /** Account::P_PHONENUMBERS_POLICY property get accessor */
        bool GetPropPhonenumbersPolicy(PHONENUMBERSPOLICY& phonenumbers_policy);
        
        /** Account::P_VOICEMAIL_POLICY property get accessor */
        bool GetPropVoicemailPolicy(VOICEMAILPOLICY& voicemail_policy);
        
        /** Account::P_PARTNER_OPTEDOUT property get accessor */
        bool GetPropPartnerOptedout(Sid::String& partner_optedout);
        
        /** Account::P_SERVICE_PROVIDER_INFO property get accessor */
        bool GetPropServiceProviderInfo(Sid::String& service_provider_info);
        
        /** Account::P_REGISTRATION_TIMESTAMP property get accessor */
        bool GetPropRegistrationTimestamp(uint& registration_timestamp);
        
        /** Account::P_NR_OF_OTHER_INSTANCES property get accessor */
        bool GetPropNrOfOtherInstances(uint& nr_of_other_instances);
        
        /** Account::P_SKYPENAME property get accessor */
        bool GetPropSkypename(Sid::String& skypename);
        
        /** Account::P_FULLNAME property get accessor */
        bool GetPropFullname(Sid::String& fullname);
        
        /** Account::P_BIRTHDAY property get accessor */
        bool GetPropBirthday(uint& birthday);
        
        /** Account::P_GENDER property get accessor */
        bool GetPropGender(uint& gender);
        
        /** Account::P_LANGUAGES property get accessor */
        bool GetPropLanguages(Sid::String& languages);
        
        /** Account::P_COUNTRY property get accessor */
        bool GetPropCountry(Sid::String& country);
        
        /** Account::P_PROVINCE property get accessor */
        bool GetPropProvince(Sid::String& province);
        
        /** Account::P_CITY property get accessor */
        bool GetPropCity(Sid::String& city);
        
        /** Account::P_PHONE_HOME property get accessor */
        bool GetPropPhoneHome(Sid::String& phone_home);
        
        /** Account::P_PHONE_OFFICE property get accessor */
        bool GetPropPhoneOffice(Sid::String& phone_office);
        
        /** Account::P_PHONE_MOBILE property get accessor */
        bool GetPropPhoneMobile(Sid::String& phone_mobile);
        
        /** Account::P_EMAILS property get accessor */
        bool GetPropEmails(Sid::String& emails);
        
        /** Account::P_HOMEPAGE property get accessor */
        bool GetPropHomepage(Sid::String& homepage);
        
        /** Account::P_ABOUT property get accessor */
        bool GetPropAbout(Sid::String& about);
        
        /** Account::P_PROFILE_TIMESTAMP property get accessor */
        bool GetPropProfileTimestamp(uint& profile_timestamp);
        
        /** Account::P_MOOD_TEXT property get accessor */
        bool GetPropMoodText(Sid::String& mood_text);
        
        /** Account::P_TIMEZONE property get accessor */
        bool GetPropTimezone(uint& timezone);
        
        /** Account::P_NROF_AUTHED_BUDDIES property get accessor */
        bool GetPropNrofAuthedBuddies(uint& nrof_authed_buddies);
        
        /** Account::P_AVAILABILITY property get accessor */
        bool GetPropAvailability(Contact::AVAILABILITY& availability);
        
        /** Account::P_AVATAR_IMAGE property get accessor */
        bool GetPropAvatarImage(Sid::Binary& avatar_image);
        
        /** Account::P_AVATAR_TIMESTAMP property get accessor */
        bool GetPropAvatarTimestamp(uint& avatar_timestamp);
        
        /** Account::P_MOOD_TIMESTAMP property get accessor */
        bool GetPropMoodTimestamp(uint& mood_timestamp);
        
        /** Account::P_RICH_MOOD_TEXT property get accessor */
        bool GetPropRichMoodText(Sid::String& rich_mood_text);
        
};


/*! \class Skype skype-embedded.h "skype-embedded.h"
 *  \brief
 *  \nosubgrouping
 *  The main class that exposes Skype-related functionality to your application. Currently the SDK only supports one
 *  Skype class instance per runtime. Note that most of the Skype class methods will fail until you have successfully
 *  logged in with an account. For more information on constructing a Skype object and the login sequence, see first
 *  tutorial example. @n 
 */
class Skype : public SERootObject
{
        friend class SEObject;
        friend class ContactGroup;
        friend class Contact;
        friend class ContactSearch;
        friend class Participant;
        friend class Conversation;
        friend class Message;
        friend class Video;
        friend class Voicemail;
        friend class Sms;
        friend class Transfer;
        friend class Account;
        protected:
        virtual ContactGroup * newContactGroup(int oid);
        virtual Contact * newContact(int oid);
        virtual ContactSearch * newContactSearch(int oid);
        virtual Participant * newParticipant(int oid);
        virtual Conversation * newConversation(int oid);
        virtual Message * newMessage(int oid);
        virtual Video * newVideo(int oid);
        virtual Voicemail * newVoicemail(int oid);
        virtual Sms * newSms(int oid);
        virtual Transfer * newTransfer(int oid);
        virtual Account * newAccount(int oid);

public:
        Skype(int num_threads = 1);
        virtual ~Skype();
        
        const unsigned int moduleID() const;
        String getNamespace() const { return __EA(SKYPE);}
        const PMAP* getPMap() const { return 0;} 
        const EMAP* getEMap() const { return 0;}
        static Skype* instance(){ return (Skype*)SERootObject::instance(); }
        Sid::Field* get_property_desc(const uint& modid, const uint& propid);
        SEObject* factory(const uint& modid, const uint& oid);
        
        void* prop_factory(const uint& moduleid);
        
        Sid::List_uint* prop_cached_list(void* propstruct, const uint& moduleid);
        
        Sid::Protocol::Status dispatch_event(Sid::CommandInitiator* thread, Sid::Protocol::ClientInterface* protocol, const uint& modid, const uint& evid);
        /** Setupkey SETUPKEY_DB_STORAGE_QUOTA_KB type:int default value:"0" @n Use this key to limit the size of the main.db file. Value is in KB. Quota are disabled by default. @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DB_STORAGE_QUOTA_KB "*Lib/DbManager/StorageQuotaKb"
        
        /** returns the runtime version as a string */
        bool GetVersionString(
                Sid::String& version
        );
        
        /** Returns the time as used in SkypeKit, in the form of a Unix timestamp (number of seconds since 1.1.1970).                   If the local system time is incorrect my more than one year, the time provided                   by the Skype network will be provided, which is correct. Therefore this function                   can be used to adjust the system time if set incorrectly (e.g. if set to 1.1.1970). */
        bool GetUnixTimestamp(
                uint& timestamp
        );
        
        /** Takes TYPE argument (TYPE comes from ContactGroup class) and returns reference to the corresponding hardwired contact group. For example (C++): skype->GetHardwiredContactGroup(ContactGroup::ONLINE_BUDDIES, GroupRef) would return the list of all contacts that are currently online. @n  */
        bool GetHardwiredContactGroup(
                const ContactGroup::TYPE& type,
                ContactGroupRef& contactGroup
        );
        
        /** Returns a list of custom contact group references, i.e. all contact groups that are not hardwired. @n  */
        bool GetCustomContactGroups(
                ContactGroupRefs& groups
        );
        
        /** Creates a new empty contact group object and returns a reference to it. The group will then show up in the custom group list that you can get with Skype class GetCustomContactGroups method. Existing contacts can be added to the new group with ContactGroup class AddContact method and a custom name can be given to it with GiveDisplayName method. @n Note that no check is made for existing of displaynames with the same name - if you wish to force uniqueness in custom group names you will have to check that yourself before creating the group. @n  */
        bool CreateCustomContactGroup(
                ContactGroupRef& group
        );
        
        /** 
        * @param group eg, new contact group loaded from CBL
        */
        virtual void OnNewCustomContactGroup(
                const ContactGroupRef& group
        );
        
        /** analyzes the identity for contact type */
        bool GetContactType(
                const Sid::String& identity,
                Contact::TYPE& type
        );
        
        /** Returns a Contact object reference. If a matching contact is not found in the existing contact list, a new Contact object will be created. Note that if you pass in a phone number in the identity argument, the type for the newly created Contact will be automatically set to Contact::PSTN (Contact::SKYPE otherwise). @n  */
        bool GetContact(
                const Sid::String& identity, /*!< Either skypename or a phone number @n */
                ContactRef& contact /*!< Returns a contact object. @n */
        );
        
        bool FindContactByPstnNumber(
                const Sid::String& number,
                bool& found,
                ContactRef& contact,
                uint& foundInKey /*!< type is actually PROPKEY*/
        );
        
        enum IDENTITYTYPE {
                UNRECOGNIZED = 0,
                SKYPE,
                SKYPE_MYSELF,
                SKYPE_UNDISCLOSED,
                PSTN,
                PSTN_EMERGENCY,
                PSTN_FREE,
                PSTN_UNDISCLOSED,
                CONFERENCE,
                EXTERNAL
        };
        
        /** This takes skypename or a phone number string as argument and returns corresponding identity type (SKYPE, SKYPE_MYSELF, PSTN, etc.) @n  */
        bool GetIdentityType(
                const Sid::String& identity,
                IDENTITYTYPE& type
        );
        
        enum NORMALIZERESULT {
                IDENTITY_OK = 0,
                IDENTITY_EMPTY,
                IDENTITY_TOO_LONG,
                IDENTITY_CONTAINS_INVALID_CHAR,
                PSTN_NUMBER_TOO_SHORT,
                PSTN_NUMBER_HAS_INVALID_PREFIX/*!< identity looks like pstn number but does not start with +/00/011*/,
                SKYPENAME_STARTS_WITH_NONALPHA,
                SKYPENAME_SHORTER_THAN_6_CHARS/*!< returned only when isNewSkypeName*/
        };
        
        /** compares two identities to see if they match */
        bool IdentitiesMatch(
                const Sid::String& identityA,
                const Sid::String& identityB,
                bool& result
        );
        
        /** This method is deprecated. Use ValidateProfileString method instead. @n  */
        bool NormalizeIdentity(
                const Sid::String& original,
                NORMALIZERESULT& result,
                Sid::String& normalized,
                const bool isNewSkypeName = false
        );
        
        /** NormalizePSTNWithCountry checks if the phone number starts with + if it doesn't, it prefixes the output with +XXX (where XXX is the country code). It also converts letters to numbers based on the standard phone keypad, so that the phone number string 212CALLME1 with country code 372 (Estonia) would be normalized to +3722122255631. If the method cannot normalize the phone number (because it's too long, too short, etc.), it returns an error code in &result. @n  */
        bool NormalizePSTNWithCountry(
                const Sid::String& original,
                NORMALIZERESULT& result,
                Sid::String& normalized,
                const uint countryPrefix = 0
        );
        
        /** This contact has appeared online. display alert */
        virtual void OnContactOnlineAppearance(
                const ContactRef& contact
        );
        
        /** This contact has gone offline. display alert */
        virtual void OnContactGoneOffline(
                const ContactRef& contact
        );
        
        /** list of (min,max) pairs */
        bool GetOptimalAgeRanges(
                Sid::List_uint& rangeList
        );
        
        /** Creates a blank contact search object, in which you can add your custom search terms. For more information how asynchronous contact search works, see ContactSearch class details. @n  */
        bool CreateContactSearch(
                ContactSearchRef& search /*!< Returns blank ContactSearch object. @n */
        );
        
        /** searches skypenames, aliases, fullnames, emails. false if not valid */
        bool CreateBasicContactSearch(
                const Sid::String& text,
                ContactSearchRef& search
        );
        
        /** searches skypenames and aliases. returns 0 or 1 results. false if not valid */
        bool CreateIdentitySearch(
                const Sid::String& identity,
                ContactSearchRef& search
        );
        
        /** Creates a new empty conversation object and returns a reference to it. @n  */
        bool CreateConference(
                ConversationRef& conference
        );
        
        /** Returns reference tp conversation object by conversation ID string (equivalent of old chat ID). NB! ID here is that of conversation, rather than skypename of dialog partner. If you want to retrieve a conversation object with any particular person, then Skype class GetConversationByParticipants method is what you are looking for. @n  */
        bool GetConversationByIdentity(
                const Sid::String& convoIdentity,
                ConversationRef& conversation
        );
        
        /** myself not included */
        bool GetConversationByParticipants(
                const Sid::List_String& participantIdentities,
                ConversationRef& conversation,
                const bool createIfNonExisting = false,
                const bool ignoreBookmarkedOrNamed = false
        );
        
        /** Retrieves a Conversation object by Public Conversation BLOB. Public conversation blobs are globally unique conversation IDs that provide a method for joining conversation without explicitly being added to the conversation by someone already in it. Programmatically, a Conversation BLOB can be retrieved with Conversation::GetJoinBlob method. In Skype desktop clients, the BLOB can be retrieved by typing "/get uri" in a conversation. The conversation can then be joined by people who have somehow received that BLOB. @n  */
        bool GetConversationByBlob(
                const Sid::String& joinBlob, /*!< The BLOB string. @n */
                ConversationRef& conversation, /*!< Returns Conversation object if successful. @n */
                const bool alsoJoin = true /*!< If set to true, automatically joins current user into the Conversation. @n */
        );
        
        /** Returns a list of Conversation objects by Conversation::LIST_TYPE filter. @n  */
        bool GetConversationList(
                ConversationRefs& conversations, /*!< List of conversations matching the filter. @n */
                const Conversation::LIST_TYPE type = Conversation::ALL_CONVERSATIONS /*!< Filter. @n */
        );
        
        /** This event gets fired when a Conversation item is added or removed from the list specified in the type argument. The primary use of this event is to detect creation of new Conversation objects. It can also be used for detecting occurance of live sessions - by monitoring added = true in Conversation::LIVE_CONVERSATIONS. Note that this method is not entirely sufficient for detecting live session termination (added = false and type = Conversation::LIVE_CONVERSATIONS). When the live session goes down, the default behaviour is that the Conversation object remains in the LIVE_CONVERSATIONS list for approximately 10 seconds. When another live session comes up within the same Conversation, the OnConversationListChange event will not fire - because the conversation was already in that list. There are two ways of getting around that. Firstly you can have all the conversations referenced at all times and then monitor Conversation::P_LOCAL_LIVESTATUS property changes, in which case you can pick up incoming live sessions from there. Alternatively, you can remove the delay between live session termination and conversation's removal from the LIVE_CONVERSATIONS list. This delay is controlled by the SETUPKEY_RECENTLY_LIVE_TIMEOUT setup key. To remove the delay, use Skype::SetInt(SETUPKEY_RECENTLY_LIVE_TIMEOUT, 0). Note that this setup key is account-based. You will need to have an account logged in in order to modify its value. @n 
        * @param conversation Conversation object that was added or removed to a list specified in the type argument. @n 
        * @param type Specifies the list, into which the conversation was added or removed from. @n 
        * @param added Specifies whether the conversation was added or removed. For ALL_CONVERSATIONS list, the removed event is only fired when the conversation is actually deleted. @n 
        */
        virtual void OnConversationListChange(
                const ConversationRef& conversation,
                const Conversation::LIST_TYPE& type,
                const bool& added
        );
        
        /** Retrieves a Message object by the P_GUID property (globally unique ID, same for all the participants of the conversation, in which this message occured). @n  */
        bool GetMessageByGuid(
                const Sid::Binary& guid, /*!< Globally unique ID of the message. @n */
                MessageRef& message /*!< Returns a Message object if a match was found. @n */
        );
        
        /** Returns all messages of the given type,                   the most recent POSTED_EMOTE or POSTED_TEXT of each conversation. */
        bool GetMessageListByType(
                const Message::TYPE& type, /*!< Type of messages requested*/
                const bool& latestPerConvOnly, /*!< Whether to return only the most recent message per conversation*/
                MessageRefs& messages,
                const uint fromTimestampInc = 0, /*!< Starting timestamp for reqested range, inclusive*/
                const uint toTimestampExc = MAX_UINT /*!< Ending timestamp for requested range, exclusive*/
        );
        
        /** 
        * @param changesInboxTimestamp if changesInboxTimestamp==true is a hint that tray alert should probably be displayed
        * @param supersedesHistoryMessage DEPRECATED, not set anymore
        */
        virtual void OnMessage(
                const MessageRef& message,
                const bool& changesInboxTimestamp,
                const MessageRef& supersedesHistoryMessage,
                const ConversationRef& conversation
        );
        
        /** This method returns a table in form of two string lists of equal length and an uint argument that returns the count of items i both lists. The first list contains video recording device handles and the second list descriptive names of those devices. NB! This method requires videortphost to be running, otherwise it will return empty lists. @n  */
        bool GetAvailableVideoDevices(
                Sid::List_String& deviceNames,
                Sid::List_String& devicePaths,
                uint& count
        );
        
        /** Queries whether the given video device has a specific Video::VIDEO_DEVICE_CAPABILITY. Use Skype::GetAvailableVideoDevices method to retrieve sstring lists with available deviceName and devicePath values. @n  */
        bool HasVideoDeviceCapability(
                const Sid::String& deviceName, /*!< Human readable device name. @n */
                const Sid::String& devicePath, /*!< Device ID. @n */
                const Video::VIDEO_DEVICE_CAPABILITY& cap, /*!< Any of the Video::VIDEO_DEVICE_CAPABILITY values. @n */
                bool& result /*!< Returns true if the given device has given capability. @n */
        );
        
        bool DisplayVideoDeviceTuningDialog(
                const Sid::String& deviceName,
                const Sid::String& devicePath
        );
        
        /** Warning: Will be deprecated soon */
        bool GetPreviewVideo(
                const Video::MEDIATYPE& type,
                VideoRef& video,
                const Sid::String deviceName = "", /*!< name and path to be used only with media type VIDEO*/
                const Sid::String devicePath = ""
        );
        
        /** Avaible to Video Engines using the Video RTP API */
        bool VideoCommand(
                const Sid::String& command,
                Sid::String& response
        );
        
        /** This callback gets fired when there are changes in the system video device list (USB webcam gets plugged in or is detached.) @n  */
        virtual void OnAvailableVideoDeviceListChange(
        );
        
        /** Event is implemented only in SkypeKit builds. Fired when Skype video library uses software H264 codec for the first time on the particular hardware by particular SkypeKit-based application */
        virtual void OnH264Activated(
        );
        
        bool GetGreeting(
                const Sid::String& skypeName,
                VoicemailRef& greeting
        );
        
        /** Setupkey SETUPKEY_DISABLED_CODECS type:string  @n Space-separated array of disabled codecs @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLED_CODECS "*Lib/Audio/DisableCodecs"
        
        /** Setupkey SETUPKEY_DISABLE_AEC type:boolean  @n Disables Skype echo canceller @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLE_AEC "*Lib/Audio/DisableAEC"
        
        /** Setupkey SETUPKEY_DISABLE_NOISE_SUPPRESSOR type:boolean  @n Disables Skype noise suppressor @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLE_NOISE_SUPPRESSOR "*Lib/Audio/DisableNS"
        
        /** Setupkey SETUPKEY_DISABLE_AGC type:boolean  Disables Skype automatic gain controller @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLE_AGC "*Lib/Audio/DisableAGC"
        
        /** Setupkey SETUPKEY_DISABLE_DIGITAL_NEAR_END_AGC type:boolean  @n Disables Skype digital near-end gain controller @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLE_DIGITAL_NEAR_END_AGC "*Lib/Audio/DisableDigitalNearEndAGC"
        
        /** Setupkey SETUPKEY_DISABLE_DIGITAL_FAR_END_AGC type:boolean  @n Disables Skype digital far-end gain controller @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLE_DIGITAL_FAR_END_AGC "*Lib/Audio/DisableDigitalFarEndAGC"
        
        /** Setupkey SETUPKEY_BEAMFORMER_MIC_SPACING type:string  @n Space-separated array of 1 (in case of 2 microphones) or 2 (in case of 4 microphones) integers. SAL beamforming currently only supports 2 and 4-microphone configurations. The values represent the spacing between microphones (in millimeters). @n In case of 2-microphone setup, Only the first value is used. @n  @n In case of 4-microphone setup, The first value is the distance between inner pair of microphones. The second value is the distance between inner pair of microphones and the outer pair. Like this: @n  @n Let the microphones be on straight line, A B C D. @n Microphones B and C form the inner pair, while A and D form the outer pair. @n The first value in the setup string would be distance between B and C. @n The second value would be distance between A and B (which is the same as distance between C and D). @n  @n With 4-mic setup, you will need to use two channels. The inner pair should go to one channel (left) and the outer pair should go to another (right). @n  @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_BEAMFORMER_MIC_SPACING "*Lib/Audio/BeamformerMicSpacing"
        
        /** Setupkey SETUPKEY_DISABLE_AUDIO_DEVICE_PROBING type:boolean  @n Disables audio devices probing @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLE_AUDIO_DEVICE_PROBING "*Lib/QualityMonitor/DisableAudioDeviceProbing"
        
        enum PREPARESOUNDRESULT {
                PREPARESOUND_SUCCESS = 0,
                PREPARESOUND_MISC_ERROR,
                PREPARESOUND_FILE_NOT_FOUND,
                PREPARESOUND_FILE_TOO_BIG,
                PREPARESOUND_FILE_READ_ERROR,
                PREPARESOUND_UNSUPPORTED_FILE_FORMAT,
                PREPARESOUND_PLAYBACK_NOT_SUPPORTED
        };
        
        enum AUDIODEVICE_CAPABILITIES {
                HAS_VIDEO_CAPTURE = 1,
                HAS_USB_INTERFACE,
                POSSIBLY_HEADSET = 4,
                HAS_AUDIO_CAPTURE = 8,
                HAS_AUDIO_RENDERING = 16,
                HAS_LOWBANDWIDTH_CAPTURE = 32,
                IS_WEBCAM = 64,
                IS_HEADSET = 128,
                POSSIBLY_WEBCAM = 256,
                HAS_VIDEO_RENDERING = 2048,
                HAS_BLUETOOTH_INTERFACE = 4096
        };
        
        /** Takes audio data that comes from the sound argument and mixes it into playback or notification device, depending on the value passed in the useCallOutDevice argument. The sound argument contains the audio data in in follwing format: first 4 bytes of the binary contain the sample rate, followed by 16 bit (mono) samples. The soundid argument is an arbitrary ID that you can pass in and then later use as an argument for Skype class PlayStop method. To mix the audio into playback device stream, set useCallOutDevice to true, to mic it into notification stream, set useCallOutDevice to false. @n  */
        bool PlayStart(
                const uint& soundid,
                const Sid::Binary& sound,
                const bool& loop,
                const bool useCallOutDevice = false
        );
        
        bool PlayStartFromFile(
                const uint& soundid,
                const Sid::Filename& datafile,
                const bool& loop,
                PREPARESOUNDRESULT& result,
                const bool useCallOutDevice = false
        );
        
        /** Stops playback of the soundfile. The argument is the same ID you passed in the Skype class StartPlayback method. @n  */
        bool PlayStop(
                const uint& soundid
        );
        
        bool StartRecordingTest(
                const bool recordAndPlaybackData = false
        );
        
        bool StopRecordingTest(
        );
        
        /** This method returns a table in form of three string lists of equal lengths. The first list contains audio output device handles ('hw:0,0', 'hw:0,1', etc.) The second list contains descriptive names of those devices (Ensoniq AudioPCI etc.) The third list contains device product IDs. Note that the values in these lists depend on which audio engine you are running (SAL, PCM, RTP). @n  */
        bool GetAvailableOutputDevices(
                Sid::List_String& handleList,
                Sid::List_String& nameList,
                Sid::List_String& productIdList
        );
        
        /** This method returns a table in form of three string lists of equal length. The first list contains audio recording device handles ('hw:0,0', 'hw:0,1', etc.) The second list contains descriptive names of those devices (Ensoniq AudioPCI etc.) The third list contains device product IDs. Note that the values in these lists depend on which audio engine you are running (SAL, PCM, RTP). @n  */
        bool GetAvailableRecordingDevices(
                Sid::List_String& handleList,
                Sid::List_String& nameList,
                Sid::List_String& productIdList
        );
        
        /** Sets audio devices given in arguments as active audio devices. This command selects all three devices - microphone, playback and the notification channel. Valid input values for this method come from the first string list you get back from Skype class GetAvailableOutputDevices (handleList). @n  */
        bool SelectSoundDevices(
                const Sid::String& callInDevice,
                const Sid::String& callOutDevice,
                const Sid::String& waveOutDevice
        );
        
        /** The uint argument returns AUDIODEVICE_CAPABILITIES (declared in Skype class) @n  */
        bool GetAudioDeviceCapabilities(
                const Sid::String& deviceHandle,
                Sid::String& interfaceString,
                uint& capabilities /*!< bit set of AUDIODEVICE_CAPABILITIES*/
        );
        
        /** Returns current audio stream volume for both playback and microphone streams. Useful for displaying visual audio indicators in you UI. See also Skype class OnNrgLevelsChange callback that gets fired each time the these values are changed. @n  */
        bool GetNrgLevels(
                uint& micLevel,
                uint& speakerLevel
        );
        
        /** NB! This command only works if its implemented in external audiohost (RTP or PCM host). The command can be is used for passing custom commands from client UI to the audio implementation. @n  */
        bool VoiceCommand(
                const Sid::String& command,
                Sid::String& response
        );
        
        /** Returns value of audio playback volume setting (0..100). @n  */
        bool GetSpeakerVolume(
                uint& volume
        );
        
        /** This method is for setting speaker volume. It will set the level for Skype digital gain control. Skype audio library will not control gain of audio device itself. @n  */
        bool SetSpeakerVolume(
                const uint& volume
        );
        
        /** Returns value of microphone volume setting (0..100). It will return the analog gain of audio device set by Skype AGC. For real-time microphone volume, use GetNrgLevels method or OnNrgLevelsChange callback (both are methods of Skype class). @n  */
        bool GetMicVolume(
                uint& micVolume
        );
        
        /** This method is for setting the microphone volume level. This does not work when Skype AGC (Automatic Gain Control) is enabled, which it is by default. It is currently impossible to disable AGC, so for now this method is here for purpose of future compatibility. @n  */
        bool SetMicVolume(
                const uint& volume
        );
        
        /** Returns true in &muted argument if the currently selected playback device is muted. @n  */
        bool IsSpeakerMuted(
                bool& muted
        );
        
        /** Returns true in &muted argument if the currently selected microphone is muted. @n  */
        bool IsMicrophoneMuted(
                bool& muted
        );
        
        /** Sets currently selected playback device mute status according to argument. @n  */
        bool MuteSpeakers(
                const bool& mute
        );
        
        /** Sets currently selected microphone mute status according to argument. @n  */
        bool MuteMicrophone(
                const bool& mute
        );
        
        /** This callback gets fired when there are changes in the system audio device list (USB headset gets plugged in or is detached.) @n  */
        virtual void OnAvailableDeviceListChange(
        );
        
        /** This callback gets fired when the audio strength changes in either playback or recording audio streams. Useful for providing visual indicators of audio activity in your UI. @n  */
        virtual void OnNrgLevelsChange(
        );
        
        enum OPERATING_MEDIA {
                OM_UNKNOWN = 0,
                OM_FREE,
                OM_FREE_WIRELESS,
                OM_3G,
                OM_4G
        };
        
        bool SetOperatingMedia(
                const OPERATING_MEDIA& media,
                const uint& maxUplinkBps,
                const uint& maxDownlinkBps
        );
        
        /** creates and sends a CONFIRMATION_CODE_REQUEST message                   this sends a confirmation code to the number provided */
        bool RequestConfirmationCode(
                const Sms::CONFIRM_TYPE& type,
                const Sid::String& number,
                SmsRef& sms
        );
        
        /** creates and sends a CONFIRMATION_CODE_SUBMIT message                   this authorizes the number with the server for the purpose given in RequestConfirmationCode */
        bool SubmitConfirmationCode(
                const Sid::String& number,
                const Sid::String& code,
                SmsRef& sms
        );
        
        /** creates an OUTGOING/COMPOSING SMS message */
        bool CreateOutgoingSms(
                SmsRef& sms
        );
        
        /** Setupkey SETUPKEY_FT_AUTOACCEPT type:int  @n Controls file transfer auto-accept.  @li 0 - off @li 1 - on @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_FT_AUTOACCEPT "Lib/FileTransfer/AutoAccept"
        
        /** Setupkey SETUPKEY_FT_SAVEPATH type:string  @n Full local path to save incoming file transfers (used for AutoAccept feature) @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_FT_SAVEPATH "Lib/FileTransfer/SavePath"
        
        /** Setupkey SETUPKEY_FT_INCOMING_LIMIT type:uint  @n Number of simultaneous incoming file transfers (per user). Value 0 means no limitation.  @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_FT_INCOMING_LIMIT "Lib/FileTransfer/IncomingLimit"
        
        /** Setupkey SETUPKEY_IDLE_TIME_FOR_AWAY type:int  @n Number of seconds since the last keyboard or mouse activity, after which the online status of currently logged in account should be set to AWAY. See Account::SetAvailability method for more information. @n This is account-specific setup key. It can only be used while an account is logged in. @n  */
        #define SETUPKEY_IDLE_TIME_FOR_AWAY "Lib/Account/IdleTimeForAway"
        
        /** Setupkey SETUPKEY_IDLE_TIME_FOR_NA type:int  @n The Contact::AVAILABILITY.NOT_AVAILABLE online status has been deprecated. This setup key is no longer in use. @n  */
        #define SETUPKEY_IDLE_TIME_FOR_NA "Lib/Account/IdleTimeForNA"
        
        /** Retrieves an Account object by Skype name (identity). This should normally be one of the first method calls after Skype object initialization. Nearly all the other methods require successful account login in order to work properly. The list of accounts that have been used on the local machine/database can be retrieved with Skype::GetExistingAccounts method. If a matching identity is not found, a new Account object is created. This object can then be used to populate requred fields and then use Account::Register method for new account creation. This method returns false on error. @n  */
        bool GetAccount(
                const Sid::String& identity, /*!< Account skypename. @n */
                AccountRef& account /*!< Returns account object if successful. @n */
        );
        
        /** Returns a list of possible profiles used before on this machine */
        bool GetExistingAccounts(
                Sid::List_String& accountNameList
        );
        
        /** return most recently used account that has pwd saved. empty string if none */
        bool GetDefaultAccountName(
                Sid::String& account
        );
        
        /** suggest a nice skypename to go with given fullname */
        bool GetSuggestedSkypename(
                const Sid::String& fullname,
                Sid::String& suggestedName
        );
        
        /** A value of this type can be returned by one of the following methods (of Skype class): ValidateAvatar, ValidateProfileString, ValidatePassword.  @n  */
        enum VALIDATERESULT {
                NOT_VALIDATED = 0             /*!< Given property could not be validated. The length of the field was within limits and the value is assumed to be Ok. Your client should treat this value as equivalent to VALIDATED_OK. @n */,
                VALIDATED_OK                  /*!< Avatar or profile string validation succeeded. @n */,
                TOO_SHORT                     /*!< Password is too short. @n */,
                TOO_LONG                      /*!< The value exceeds max size limit for the given property. @n */,
                CONTAINS_INVALID_CHAR         /*!< Value contains illegal characters. @n */,
                CONTAINS_SPACE                /*!< Value contains whitespace. @n */,
                SAME_AS_USERNAME              /*!< Password cannot be the same as skypename. @n */,
                INVALID_FORMAT                /*!< Value has invalid format. @n */,
                CONTAINS_INVALID_WORD         /*!< Value contains invalid word. @n */,
                TOO_SIMPLE                    /*!< Password is too simple. @n */,
                STARTS_WITH_INVALID_CHAR      /*!< Value starts with an invalid character. @n */
        };
        
        bool ValidateAvatar(
                const Sid::Binary& value,
                VALIDATERESULT& result,
                int& freeBytesLeft
        );
        
        /** This method should be used for validating skypenames before registering new accounts, if the propKey is set to SKYPENAME (Contact class) and forRegistration argument is set to true. If the forRegistration argument is false, only string length check is applied. It is also useful to probe, what the size limits are, for each string property (e.g. 300 characters for moodmessage) @n  */
        bool ValidateProfileString(
                const int& propKey,
                const Sid::String& strValue,
                VALIDATERESULT& result,
                int& freeBytesLeft,
                const bool forRegistration = false
        );
        
        /** This method is for pre-validating account passwords before account creation or password change. The result will return either VALIDATED_OK or one of many possible reasons the password is unacceptable (too short, too simple, etc.) @n  */
        bool ValidatePassword(
                const Sid::String& username,
                const Sid::String& password,
                VALIDATERESULT& result
        );
        
        /** Setupkey SETUPKEY_PORT type:int  @n Suggested port number (lib will *try* to use that) @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_PORT "*Lib/Connection/Port"
        
        /** Setupkey SETUPKEY_HTTPS_PROXY_ENABLE type:int  @n Set to 0 for automatic proxy detect, 1 to use proxy config below @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_HTTPS_PROXY_ENABLE "*Lib/Connection/HttpsProxy/Enable"
        
        /** Setupkey SETUPKEY_HTTPS_PROXY_ADDR type:string  @n name:port of HTTP proxy server @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_HTTPS_PROXY_ADDR "*Lib/Connection/HttpsProxy/Addr"
        
        /** Setupkey SETUPKEY_HTTPS_PROXY_USER type:string  @n HTTPS proxy server username @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_HTTPS_PROXY_USER "*Lib/Connection/HttpsProxy/User"
        
        /** Setupkey SETUPKEY_HTTPS_PROXY_PWD type:string  @n HTTPS proxy server password (base64 encoded) @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_HTTPS_PROXY_PWD "*Lib/Connection/HttpsProxy/Pwd"
        
        /** Setupkey SETUPKEY_SOCKS_PROXY_ENABLE type:int  @n Set to non-zero to enable socks proxy support @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_SOCKS_PROXY_ENABLE "*Lib/Connection/SocksProxy/Enable"
        
        /** Setupkey SETUPKEY_SOCKS_PROXY_ADDR type:string  @n name:port of SOCKS proxy server @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_SOCKS_PROXY_ADDR "*Lib/Connection/SocksProxy/Addr"
        
        /** Setupkey SETUPKEY_SOCKS_PROXY_USER type:string  @n SOCKS proxy server username @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_SOCKS_PROXY_USER "*Lib/Connection/SocksProxy/User"
        
        /** Setupkey SETUPKEY_SOCKS_PROXY_PWD type:string  @n SOCKS proxy server password (base64 encoded) @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_SOCKS_PROXY_PWD "*Lib/Connection/SocksProxy/Pwd"
        
        /** Setupkey SETUPKEY_LOCALADDRESS type:string  @n local interface to listen to @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_LOCALADDRESS "*Lib/Connection/LocalAddress"
        
        /** Setupkey SETUPKEY_DISABLE_PORT80 type:int  @n 1 disables listening of alternative ports (80, 443) @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLE_PORT80 "*Lib/Connection/DisablePort80"
        
        /** Setupkey SETUPKEY_DISABLE_UDP type:int  @n 1 disables UDP port binding. should be set before connect @n This setup key is machine-specific and affects all local accounts. @n  */
        #define SETUPKEY_DISABLE_UDP "*Lib/Connection/DisableUDP"
        
        enum PROXYTYPE {
                HTTPS_PROXY = 0,
                SOCKS_PROXY
        };
        
        virtual void OnProxyAuthFailure(
                const PROXYTYPE& type
        );
        
        /** Port number that the runtime ended up listening to. Usually equal to SETUPKEY_PORT. 0 if none used (disconnected or binding failed). @n  */
        bool GetUsedPort(
                uint& port
        );
        
        /** This is used for retrieving local setup keys of type string. For more information, see Defines section in the skype-embedded_2.h  @n  */
        bool GetStr(
                const Sid::String& key,
                Sid::String& value
        );
        
        /** This is used for retrieving local setup keys of type int. For more information, see Defines section in the skype-embedded_2.h  @n  */
        bool GetInt(
                const Sid::String& key,
                int& value
        );
        
        /** This is used for retrieving local setup keys of type binary. For more information, see Defines section in the skype-embedded_2.h  @n  */
        bool GetBin(
                const Sid::String& key,
                Sid::Binary& value
        );
        
        /** This is used for setting local setup keys of type string. For more information, see Defines section in the skype-embedded_2.h  @n  */
        bool SetStr(
                const Sid::String& key,
                const Sid::String& value
        );
        
        /** This is used for setting local setup keys of type int. For more information, see Defines section in the skype-embedded_2.h  @n  */
        bool SetInt(
                const Sid::String& key,
                const int& value
        );
        
        /** This is used for setting local setup keys of type binary. For more information, see Defines section in the skype-embedded_2.h  @n  */
        bool SetBin(
                const Sid::String& key,
                const Sid::Binary& value
        );
        
        /** Returns true if the given setup key is defined in local setup. For more information, see Defines section in the skype-embedded_2.h  @n  */
        bool IsDefined(
                const Sid::String& key,
                bool& value
        );
        
        bool Delete(
                const Sid::String& key
        );
        
        bool GetSubKeys(
                const Sid::String& key,
                Sid::List_String& value
        );
        
        /** Returns two string lists. First of them will contain list of two-letter language codes (ISO 639-1) The second list contains names of corresponding languages. @n  */
        bool GetISOLanguageInfo(
                Sid::List_String& languageCodeList,
                Sid::List_String& languageNameList /*!< assumes UI has set correct language*/
        );
        
        /** Returns three string lists and one int array, containing 2-letter country code, country name, dialing prefix and example dial string (not available for all items). This method does currently return 0 for South Georgia and the South Sandwich Islands. @n  */
        bool GetISOCountryInfo(
                Sid::List_String& countryCodeList,
                Sid::List_String& countryNameList, /*!< assumes UI has set correct language*/
                Sid::List_uint& countryPrefixList,
                Sid::List_String& countryDialExampleList
        );
        
        /** Returns 2-letter country code based on PSTN number. The input argument has to be without + in from of it - '37212345678' will return 'ee' while '+37212345678' will return an empty string. @n  */
        bool GetISOCountryCodebyPhoneNo(
                const Sid::String& number,
                Sid::String& countryCode
        );
        
        /** App2AppCreate method constructs a local instance of an App2App application. App2App applications are not abstracted in the SkypeKit API as a separate class. Rather, they can be created with App2AppCreate, supplying their name as ID, and then connected to remote parties using App2AppConnect method. @n  @n App2App portion of the SkypeKit API enables you to implement arbitrary data exchange protocols between Skype clients. Basically, if you are ever tempted to use conversation chat messages for something other than actual human-readable chat - you should consider using your own custom App2App protocol instead. @n  @n The downside of App2App is that all the participants need to be running a client that supports the same App2App application. Although, it is possible to have one side using a custom SkypeKit client and another side using Skype desktop client - App2App is supported in both, in case of desktop client via Public API - you are still limited to remote side running something that can recognize your protocol and react to connection attempts from your side. @n  @n To establish connection between each other, all participants need to create their local instances of the application (with the same ID, and then connect to each other. More than one App2App applications can be active in a local client at the same time. Also, more than two clients can be connected with the same application. @n  @n Once connection is established, you can choose between two communication methods - datagrams and stream read/write methods. Overall, there are not much principal difference between the two. Datagram packet size is limited to 1500 bytes and stream packet size to 32 KB of payload data. Implementation-wise, datagrams are probably somewhat easier to deal with. @n  */
        bool App2AppCreate(
                const Sid::String& appname, /*!< Application ID. This ID is used by the rest of the App2App commands to differentiate between applications, should there be more than one app2app applications running on the local system. @n */
                bool& result /*!< Returns true if the app creation was successful. Returns false when an application with the same name already exists in the local system. @n */
        );
        
        /** Disconnects and deletes the App2App application. @n  */
        bool App2AppDelete(
                const Sid::String& appname, /*!< application ID. @n */
                bool& result /*!< Returns true if the deletion was successful (application with such ID actually existed) @n */
        );
        
        /** The App2AppConnect connect result does not mean the connection was immediately established. It will return Ok even if the remote party was offline. The actual connection will be established when both parties have fired App2AppConnect with the same application name, at eachother. At that point, OnApp2AppStreamListChange event will fire for both local and remote party (with listType argument set to ALL_STREAMS) and you can start exchanging data, using either App2App datagrams or App2AppRead App2AppWrite methods. @n  */
        bool App2AppConnect(
                const Sid::String& appname, /*!< Application ID. This needs to match with application ID connecting from the remote side. @n */
                const Sid::String& skypename, /*!< Skype Name of the remote party. @n */
                bool& result /*!< NB! This argument will return true even if the remote party has not yet connected (or is not even online yet) - it merely indicates that the connect command was successfuly processed in runtime. The actual connection success will be indicated when the OnApp2AppStreamListChange event fires, i.e. when App2App stream will be established between connecting parties. @n */
        );
        
        /** Disconnects an App2App stream. This is different from App2AppDelete command in that it enables you to disconnect remote parties selectively - in case there are more than two participants in the App2App stream pool. @n  */
        bool App2AppDisconnect(
                const Sid::String& appname, /*!< application ID @n */
                const Sid::String& stream, /*!< stream ID. @n */
                bool& result /*!< returns true when the stream disconnect was successful. @n */
        );
        
        /** Sends a stream packet to the remote party specified in the stream argument. The max size of stream write packet is 32KB. After calling this method, OnApp2AppStreamListChange will fire for both parties. In local ssytem with listType set to SENDING_STREAMS and on remote system with listType set to RECEIVED_STREAMS. This event can be used to read received packets out of the stream. @n  */
        bool App2AppWrite(
                const Sid::String& appname, /*!< application ID @n */
                const Sid::String& stream, /*!< stream ID @n */
                const Sid::Binary& data, /*!< packet payload @n */
                bool& result /*!< returns true if the call was successful. Note that this does indicate the packet was actually received by remote party. @n */
        );
        
        /** Sends a datagram to the remote party specified in the stream argument. The max size of datagram payload is 1500 bytes. @n  */
        bool App2AppDatagram(
                const Sid::String& appname, /*!< application ID - from App2AppCreate @n */
                const Sid::String& stream, /*!< stream ID - either from App2AppGetStreamsList or from OnApp2AppStreamListChange*/
                const Sid::Binary& data, /*!< datagram payload (max 1500 bytes)*/
                bool& result /*!< returns true on method success. Note that this does mean the remote party has actually received your datagram - that sort of feedback, should you want it, is up to you to implement in your custom protocol.*/
        );
        
        /** Reads data from the specified stream. This method should be called from the OnApp2AppStreamListChange event callback, when this callback gets fired with listType argument set to RECEIVED_STREAMS. @n  */
        bool App2AppRead(
                const Sid::String& appname, /*!< application ID @n */
                const Sid::String& stream, /*!< stream ID @n */
                bool& result, /*!< returns true on method success. Note that this does mean the remote party has actually received your packet - that sort of feedback, should you want it, is up to you to implement in your custom protocol. @n */
                Sid::Binary& data /*!< stream packet payload @n */
        );
        
        /** App2AppGetConnectableUsers returns a list of currently online contacts. It does not return a list of contacts who have an app2app application running. There is currently no way of querying whether an application has been launched on the remote side - other than trying to connect to the remote side and waiting for timeout. NB! if you use App2AppGetConnectableUsers immediately after login - then the online presence of your contact list has not yet updated itself - so this method will most likely return either an empty list or a list with echo123 in it. @n  */
        bool App2AppGetConnectableUsers(
                const Sid::String& appname, /*!< application ID @n */
                bool& result, /*!< returns true on method success @n */
                Sid::List_String& users /*!< stringlist with Skype Names of connectable users @n */
        );
        
        enum APP2APP_STREAMS {
                ALL_STREAMS = 0,
                SENDING_STREAMS,
                RECEIVED_STREAMS
        };
        
        /** Takes application ID and list type filter and returns a stringlist with streamID's that pass the filter. @n  */
        bool App2AppGetStreamsList(
                const Sid::String& appname, /*!< application ID @n */
                const APP2APP_STREAMS& listType, /*!< list type filter @n */
                bool& result, /*!< results true if the method call was successful @n */
                Sid::List_String& streams, /*!< string list with stream IDs @n */
                Sid::List_uint& receivedSizes /*!< For RECEIVED_STREAMS, contains the number of bytes in each stream waiting to be read*/
        );
        
        /** This event gets fired on incoming app2app datagram. @n 
        * @param appname App2app application ID. @n 
        * @param stream App2app stream ID - see OnApp2AppStreamListChange event for obtaining stream IDs. @n 
        * @param data Datagram payload - limited to 1500 bytes. @n 
        */
        virtual void OnApp2AppDatagram(
                const Sid::String& appname,
                const Sid::String& stream,
                const Sid::Binary& data
        );
        
        /** In context of datagrams, this event will fire when: @li Connection is established between two app2app applications. That is, when both parties have an app up with the same name and -both- used App2AppConnect In that case, both parties get this event, with listType ALL_STREAMS @li When a datagram is sent, the sender will get this event with listType SENDING_STREAMS Receiver of the datagram will get OnApp2AppDatagram event instead. @li When the remote party drops app2app connection, the local user will get OnApp2AppStreamListChange with listType ALL_STREAMS and streams.size() zero. @n In context of stream reads/writes, this event will fire for both the sender (listType == SENDING_STREAMS)and the receiver (listType == RECEIVED_STREAMS). For receiver side, this is the place to put your reading code - App2AppRead. @n 
        * @param appname application ID - the name you supplied in App2AppCreate. @n 
        * @param listType application list type (read/write/all) @n 
        * @param streams SEStringlist with affected stream IDs. @n 
        * @param receivedSizes For RECEIVED_STREAMS, contains the number of bytes in each stream waiting to be read
        */
        virtual void OnApp2AppStreamListChange(
                const Sid::String& appname,
                const APP2APP_STREAMS& listType,
                const Sid::List_String& streams,
                const Sid::List_uint& receivedSizes
        );
        
};
#ifdef SE_USE_NAMESPACE
};
#endif

#endif //compile-guard
