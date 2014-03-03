# -*- coding: utf-8 -*-
# module Skype
import skypekit
import warnings
module_id2classes = {}

class ContactGroup(skypekit.Object):
    """
    Collects and manages Contacts related by type, status, or some other arbitrary criteria. SkypeKit recognizes two distinct ContactGroup flavors - "hardwired" and "custom". SkypeKit both defines the criteria for and dynamically manages all "hardwired" ContactGroups. Individual users explicitly create and manage all "custom" ContactGroups. 
    
    "Hardwired" groups are primarily organizational tools, for example, they enable you to display a list of all Contacts awaiting authorization by you. "Custom" groups are also organizational tools, for example, they enable you to display a list of all Contacts in a particular geographical area or belonging to a particular professional association, social clubs, and so forth. Primarily, though, "custom" groups are functional tools that enable you to establish conference calls, group chats, and so forth. 
    
    "Hardwired" ContactGroups are defined for and available to all users. SkypeKit determines membership in a particular "hardwired" group dynamically whenever a user invokes Skype.GetHardwiredContactGroup for that group. Subsequent changes to a Contact's status might result in its being added to (for example, the Contact is now authorized) or removed from (for example, the Contact is now removed or blocked) one or more "hardwired" groups. 
    
    SkypeKit fires OnChange events for all affected ContractGroup instances. Essentially all ContactGroup methods related to explicitly adding and removing members and conversations from the group return false, and CanAdd and CanRemove additionally return a false result. 
    
    "Custom" ContactGroups can be defined by a particular Skype user through the UI. Your UI should implement Creation, deletion and filtering contact list by custom contact groups, as well as adding and removing contacts in those groups. 
    
    A Contact can belong to multiple non-mutually exclusive "hardwired" groups at the same time, for example, an authorized contact is typically in your "buddy" group, but a Contact cannot belong to CONTACTS_AUTHORIZED_BY_ME if they are awaiting authorization. Similarly, a Contact can belong to multiple "custom" groups and mutual exclusivity is typically not an issue. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "ContactGroup %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(ContactGroup, "P_"+propname.upper())
    module_id = 10
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    TYPE = {
        1 :'ALL_KNOWN_CONTACTS',
        2 :'ALL_BUDDIES',
        3 :'SKYPE_BUDDIES',
        4 :'SKYPEOUT_BUDDIES',
        5 :'ONLINE_BUDDIES',
        6 :'UNKNOWN_OR_PENDINGAUTH_BUDDIES',
        7 :'RECENTLY_CONTACTED_CONTACTS',
        8 :'CONTACTS_WAITING_MY_AUTHORIZATION',
        9 :'CONTACTS_AUTHORIZED_BY_ME',
        10:'CONTACTS_BLOCKED_BY_ME',
        11:'UNGROUPED_BUDDIES',
        12:'CUSTOM_GROUP',
        13:'PROPOSED_SHARED_GROUP',
        14:'SHARED_GROUP',
        15:'EXTERNAL_CONTACTS',
        'ALL_KNOWN_CONTACTS'
        
                
        : 1,
        'ALL_BUDDIES'
        
                
               : 2,
        'SKYPE_BUDDIES'
        
                
             : 3,
        'SKYPEOUT_BUDDIES'
        
                
          : 4,
        'ONLINE_BUDDIES'
        
                
            : 5,
        'UNKNOWN_OR_PENDINGAUTH_BUDDIES'
        
            : 6,
        'RECENTLY_CONTACTED_CONTACTS'
        
               : 7,
        'CONTACTS_WAITING_MY_AUTHORIZATION'
        
         : 8,
        'CONTACTS_AUTHORIZED_BY_ME'
        
                 : 9,
        'CONTACTS_BLOCKED_BY_ME'
        
                    :10,
        'UNGROUPED_BUDDIES'
        
                
         :11,
        'CUSTOM_GROUP'
        
                
              :12,
        'PROPOSED_SHARED_GROUP'
        
                     :13,
        'SHARED_GROUP'
        
                
              :14,
        'EXTERNAL_CONTACTS'
        
                
         :15
    }
    """
    The list of all possible ContactGroup types. A value of this type can be passed to Skype class GetHardwiredContactGroup to retrieve the relevant ContactGroup object. 
    
     - ALL_KNOWN_CONTACTS - The superset of all "hardwired" contact groups. 
    
     - ALL_BUDDIES - The set of all authorized contacts, that is, contacts that were last the target of Contact::SetBuddyStatus(false) plus all SkypeOut contacts. 
    
     - SKYPE_BUDDIES - The set of all authorized Skype contacts (Contact:_SKYPENAME is non-null). 
    Note that this excludes Skype contacts that have either never been the target of Contact::SetBuddyStatus(true) or were last the target of Contactact::SetBuddyStatus(false). 
    
     - SKYPEOUT_BUDDIES - The set of all SkypeOut contacts (Contact:_PSTNNUMBER is non-null). PSTN contacts can be added to the contact list by retrieving a new contact object with Skype::GetContact, passing in the phone number as string, and then either using Contact::SetBuddyStatus(true) or adding the contact to the SKYPEOUT_BUDDIES group with ContactGroup::AddContact. 
    
     - ONLINE_BUDDIES - The subset of ALL_BUDDIES that are currently online, including those currently marked as DO_NOT_DISTURBED and AWAY. 
    
     - UNKNOWN_OR_PENDINGAUTH_BUDDIES - The set of all contacts whose Contact:_TYPE reflects UNRECOGNIZED OR have not authorized the local user yet. 
    
     - RECENTLY_CONTACTED_CONTACTS - This filter returns top 10 most recently contacted contacts, based on Contact::P_LASTUSED_TIMESTAMP property values. This is not configurable. Note that the P_LASTUSED_TIMESTAMP property does not propagate between different Skype instances - thus this filter only works in context of the local database. Recent contacts that were in touch with the user on some other Skype installation will not show up in this filter. 
    
     - CONTACTS_WAITING_MY_AUTHORIZATION - Contacts to whose authorization request the user has not responded yet. The UI should enable the user to accept, decline the authorization request and in case of decline, optionally block further incoming communication from the contact. See: Contact::SetBuddyStatus, Contact::SetBlocked and Contact::IgnoreAuthRequest for more information. 
    
     - CONTACTS_AUTHORIZED_BY_ME - All contacts authorized by the user. 
    
     - CONTACTS_BLOCKED_BY_ME - Group of contacts the user has blocked from further incoming communications. If the UI enables contact blocking, it should also provide interface for the user to unblock the blocked contacts. Note that a contact can simultaneously be in both CONTACTS_BLOCKED_BY_ME and CONTACTS_AUTHORIZED_BY_ME groups. 
    
     - UNGROUPED_BUDDIES - The set of all "buddies" that are not also a member of a custom group. 
    
     - CUSTOM_GROUP - A custom group defined by user. 
    
     - PROPOSED_SHARED_GROUP - The shared contact group functionality is no longer supported. This contact group type can be ignored. 
    
     - SHARED_GROUP - The shared contact group functionality is no longer supported. This contact group type can be ignored. 
    
     - EXTERNAL_CONTACTS - The set of all contacts that were originally imported from an external address book. 
    
    """
    
    def _sk_get_type(self):
        """
        ContactGroup::TYPE
        """
        return skypekit.enumof(ContactGroup.TYPE, self._sk_property("ZG\233\001]\012", 155, True))
    type = property(_sk_get_type)
    propid2label[155] = "type"
    P_TYPE = 155
    def _sk_get_custom_group_id(self):
        """
        unique 32-bit ID for custom groups
        """
        return self._sk_property("ZG\232\001]\012", 154, True)
    custom_group_id = property(_sk_get_custom_group_id)
    propid2label[154] = "custom_group_id"
    P_CUSTOM_GROUP_ID = 154
    def _sk_get_given_displayname(self):
        """
        change via ContactGroup::GiveDisplayname()
        """
        return self._sk_property("ZG\227\001]\012", 151, True)
    given_displayname = property(_sk_get_given_displayname)
    propid2label[151] = "given_displayname"
    P_GIVEN_DISPLAYNAME = 151
    def _sk_get_nrofcontacts(self):
        """
        Number of contacts in the group. NB! The value of this property does not get updated until 5 seconds after account login. During these initial 5 seconds, the value of this property remains 0. The reason for this 5 second delay is to reduce the flurry of property update traffic that occurs during the CBL synchronization phase, following successful login. Note that if you absolutely need to have this value immediately after login, you can still get it by retrieving the contact list with ContactGroup::GetContacts method and examining its size. 
        
        """
        return self._sk_property("ZG\230\001]\012", 152, True)
    nrofcontacts = property(_sk_get_nrofcontacts)
    propid2label[152] = "nrofcontacts"
    P_NROFCONTACTS = 152
    def _sk_get_nrofcontacts_online(self):
        """
        number of contacts online in the group
        """
        return self._sk_property("ZG\231\001]\012", 153, True)
    nrofcontacts_online = property(_sk_get_nrofcontacts_online)
    propid2label[153] = "nrofcontacts_online"
    P_NROFCONTACTS_ONLINE = 153
    
    def GiveDisplayName(
        self,
        name
        ):
        """
        Setter for ContactGroup class GIVEN_DISPLAYNAME property. 
        
        
        B{Arguments:}
         - B{name}
        """
        l_request = skypekit.XCallRequest("ZR\012\001", 10, 1)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, name)
        self.transport.xcall(l_request)
    def Delete(self):
        """
        Removes the contact group. This is synced across instances logged in with the same account - which can take several minutes for the sync to happen. 
        
        
        B{Return values:}
         - B{result}
        """
        l_request = skypekit.XCallRequest("ZR\012\002", 10, 2)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def GetConversations(self):
        """
        Returns list of conversations in the ContactGroup. 
        
        
        B{Return values:}
         - B{conversations}
        """
        l_request = skypekit.XCallRequest("ZR\012\003", 10, 3)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = [module_id2classes[18](oid, self.transport) for oid in l_response.get(1, [])]
        return l_result
    def CanAddConversation(
        self,
        conversation = 0
        ):
        """
        Checks if the current user can add given conversation to the ContactGroup. Returns false for most of the hardwired contact groups for example. 
        
        
        B{Arguments:}
         - B{conversation} - Conversation to be checked. 
        
        
        B{Return values:}
         - B{result} - Returns true if Conversation can be added to this ContactGroup. 
        
        """
        l_request = skypekit.XCallRequest("ZR\012\004", 10, 4)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, conversation)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def AddConversation(
        self,
        conversation
        ):
        """
        Adds given conversation to the ContactGroup. 
        
        
        B{Arguments:}
         - B{conversation}
        """
        l_request = skypekit.XCallRequest("ZR\012\005", 10, 5)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, conversation)
        self.transport.xcall(l_request)
    def CanRemoveConversation(self):
        """
        Checks if the current user can remove given conversation from the ContactGroup. Again, returns false for most hardwired contact groups. 
        
        
        B{Return values:}
         - B{result} - true if RemoveConversation(contact) works on this group
        """
        l_request = skypekit.XCallRequest("ZR\012\006", 10, 6)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def RemoveConversation(
        self,
        conversation
        ):
        """
        Removes given conversation from the ContactGroup. 
        
        
        B{Arguments:}
         - B{conversation}
        """
        l_request = skypekit.XCallRequest("ZR\012\007", 10, 7)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, conversation)
        self.transport.xcall(l_request)
    def OnChangeConversation(
        self,
        conversation
        ):
        """
        conversation added or removed from this group
        
        B{Return values:}
         - B{conversation}
        """
        pass
    event_handlers[1] = "_sk_on_change_conversation"
    def _sk_on_change_conversation(self, parms):
        """ internal event dispatcher for OnChangeConversation
  """
        l_cleanparms = module_id2classes[18](parms.get(1), self.transport)
        self.OnChangeConversation(l_cleanparms)
    def GetContacts(self):
        """
        Retrieves contact list. 
        
        
        B{Return values:}
         - B{contacts}
        """
        l_request = skypekit.XCallRequest("ZR\012\010", 10, 8)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = [module_id2classes[2](oid, self.transport) for oid in l_response.get(1, [])]
        return l_result
    def CanAddContact(
        self,
        contact = 0
        ):
        """
        Checks if the current user can add given contact to the ContactGroup. 
        
        
        B{Arguments:}
         - B{contact} - Contact to be checked. 
        
        
        B{Return values:}
         - B{result} - returns true if AddContact(contact) works on this group. 
        
        """
        l_request = skypekit.XCallRequest("ZR\012\011", 10, 9)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, contact)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def AddContact(
        self,
        contact
        ):
        """
        Adds contact to a contact group. This only works for non-hardwired contact groups. 
        
        
        B{Arguments:}
         - B{contact}
        """
        l_request = skypekit.XCallRequest("ZR\012\012", 10, 10)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, contact)
        self.transport.xcall(l_request)
    def CanRemoveContact(self):
        """
        Checks if the current user can remove given contact from the ContactGroup. 
        
        
        B{Return values:}
         - B{result} - true if RemoveContact(contact) works on this group
        """
        l_request = skypekit.XCallRequest("ZR\012\013", 10, 11)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def RemoveContact(
        self,
        contact
        ):
        """
        Removes contact from the ContactGroup. 
        
        
        B{Arguments:}
         - B{contact}
        """
        l_request = skypekit.XCallRequest("ZR\012\014", 10, 12)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, contact)
        self.transport.xcall(l_request)
    def OnChange(
        self,
        contact
        ):
        """
        A contact has been added or removed to this ContactGroup. 
        NB! On rare occasions, the ContectRef argument to this callback can be NULL. You should always check whether the reference is valid, before accessing methods or properties. 
        
        
        B{Return values:}
         - B{contact}
        """
        pass
    event_handlers[2] = "_sk_on_change"
    def _sk_on_change(self, parms):
        """ internal event dispatcher for OnChange
  """
        l_cleanparms = module_id2classes[2](parms.get(1), self.transport)
        self.OnChange(l_cleanparms)
module_id2classes[10] = ContactGroup

class Contact(skypekit.Object):
    """
    Address book entry. Encapsulates methods like GetType, GetIdentity, GetAvatar, SendAuthRequest, OpenConversation etc. Single contact can have additional phone numbers attached to it (ASSIGNED_PHONE1 .. ASSIGNED_PHONE3). Note that in the context of a conversation, Contacts are represented by Participant objects. Contact member functions all return a Boolean indicating the success (true) or failure (false) of processing the request itself (transport, runtime availability, and so forth)?not the success or failure of its associated functionality. For example, Contact::IsMemberOf returns true if it was able to make a determination, and its result parameter reflects whether this Contact is a member of the target group. Similarly, Contact::IsMemberOf returns false if it was unable to make a determination, and the value of its result parameter is undefined. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "Contact %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(Contact, "P_"+propname.upper())
    module_id = 2
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    TYPE = {0:'UNRECOGNIZED', 'UNRECOGNIZED':0, 1:'SKYPE', 'SKYPE':1, 2:'PSTN', 'PSTN':2, 3:'EMERGENCY_PSTN', 'EMERGENCY_PSTN':3, 4:'FREE_PSTN', 'FREE_PSTN':4, 5:'UNDISCLOSED_PSTN', 'UNDISCLOSED_PSTN':5, 6:'EXTERNAL', 'EXTERNAL':6}
    """
    Same as with CAPABILITY, enumerator is used by both Contact and Account objects. 
    
     - UNRECOGNIZED - Contact/account has no pre-identified type. This type is reported by default for SkypeKit clients. 
    
     - SKYPE - Normal Skype contact. 
    
     - PSTN - Normal PSTN contact. 
    
     - EMERGENCY_PSTN - Emergency number (i.e. 911). 
    
     - FREE_PSTN
     - UNDISCLOSED_PSTN - Undisclosed PSTN number. 
    
     - EXTERNAL - This type is currently used by Windows desktop clients for contacts imported from Outlook. 
    
    """
    AUTHLEVEL = {0:'NONE', 'NONE':0, 1:'AUTHORIZED_BY_ME', 'AUTHORIZED_BY_ME':1, 2:'BLOCKED_BY_ME', 'BLOCKED_BY_ME':2}
    """
    Describes the recognized relational states between a local account and a remote contact. 
    
     - NONE - Authorization request is either ignored or pending. In this state several functionalities may be blocked, depending on settings. For example, accounts may only allow seeing online presence to be viewable or only receive calls from authorized contacts. 
    
     - AUTHORIZED_BY_ME - Contact has been authorized by the local account. 
    
     - BLOCKED_BY_ME - Contact has been blocked by the local account. This prevents incoming calls, chat messages, additional authorization requests etc. 
    
    """
    AVAILABILITY = {
        0 :'UNKNOWN',
        8 :'PENDINGAUTH',
        9 :'BLOCKED',
        11:'BLOCKED_SKYPEOUT',
        10:'SKYPEOUT',
        1 :'OFFLINE',
        12:'OFFLINE_BUT_VM_ABLE',
        13:'OFFLINE_BUT_CF_ABLE',
        2 :'ONLINE',
        3 :'AWAY',
        4 :'NOT_AVAILABLE',
        5 :'DO_NOT_DISTURB',
        7 :'SKYPE_ME',
        6 :'INVISIBLE',
        14:'CONNECTING',
        15:'ONLINE_FROM_MOBILE',
        16:'AWAY_FROM_MOBILE',
        17:'NOT_AVAILABLE_FROM_MOBILE',
        18:'DO_NOT_DISTURB_FROM_MOBILE',
        20:'SKYPE_ME_FROM_MOBILE',
        'UNKNOWN'
        
                
                   : 0,
        'PENDINGAUTH'
        
                
               : 8,
        'BLOCKED'
        
                
                   : 9,
        'BLOCKED_SKYPEOUT'
        
                
          :11,
        'SKYPEOUT'
        
                
                  :10,
        'OFFLINE'
        
                
                   : 1,
        'OFFLINE_BUT_VM_ABLE'
        
                       :12,
        'OFFLINE_BUT_CF_ABLE'
        
                       :13,
        'ONLINE'
        
                
                    : 2,
        'AWAY'
        
                
                      : 3,
        'NOT_AVAILABLE'
        
                
             : 4,
        'DO_NOT_DISTURB'
        
                
            : 5,
        'SKYPE_ME'
        
                
                  : 7,
        'INVISIBLE'
        
                
                 : 6,
        'CONNECTING'
        
                
                :14,
        'ONLINE_FROM_MOBILE'
        
                
        :15,
        'AWAY_FROM_MOBILE'
        
                
          :16,
        'NOT_AVAILABLE_FROM_MOBILE'
        
                 :17,
        'DO_NOT_DISTURB_FROM_MOBILE'
        
                :18,
        'SKYPE_ME_FROM_MOBILE'
        
                      :20
    }
    """
    Describes the superset list of possible Account and Contact online statuses. In case of Account they apply to local user, in case of Contact they apply to remote contacts. 
    
     - UNKNOWN - Contact online status cannot be determined. This availability state should not normally reach the SkypeKit UI level. 
    
     - PENDINGAUTH - Seeing Contact online status is blocked because authorization between contact and local account has not taken place. 
    
     - BLOCKED - Remote contact has been blocked by local account. This applies to online accounts. 
    
     - BLOCKED_SKYPEOUT - Remote SkypeOut contact has been blocked by local account. 
    
     - SKYPEOUT - Contact does not have an online status because he is a PSTN contact. 
    
     - OFFLINE - Contact appears to be offline. 
    
     - OFFLINE_BUT_VM_ABLE - Contact appears to be offline but has voicemail enabled. 
    
     - OFFLINE_BUT_CF_ABLE - Contact appears to be offline but has enabled call forwarding, so calls may actually get through to him. 
    
     - ONLINE - Contact / Account is online 
    
     - AWAY - Contact / Account is online but away from keyboard. This can be either turned on manually or by automatic timer. In Windows desktop client, the timer can be configured with minute precision. 
    
     - NOT_AVAILABLE - This online status is marked as deprecated. If a remote contact indicates its status as NOT_AVAILABLE, the UI should handle this as equivalent of AWAY status. 
    
     - DO_NOT_DISTURB - Contact / Account is online but does not wish to be disturbed. This status supersedes AWAY status when the account is DO_NOT_DISTURB the AWAY timer should not modify the status. 
    
     - SKYPE_ME - This online status is marked as deprecated. If a remote contact indicates its status as SKYPE_ME, the UI should handle this as equivalent of ONLINE status. 
    
     - INVISIBLE - Account status is set to INVISIBLE. This status in not applicable to remote Contacts. When the remote contact has set his availability to INVISIBLE, he will appear as OFFLINE to others. 
    
     - CONNECTING - only possible for local user/account
     - ONLINE_FROM_MOBILE
     - AWAY_FROM_MOBILE - *_FROM_MOBILE only possible for remote user
     - NOT_AVAILABLE_FROM_MOBILE
     - DO_NOT_DISTURB_FROM_MOBILE
     - SKYPE_ME_FROM_MOBILE
    """
    EXTRA_AUTHREQ_FIELDS = {1:'SEND_VERIFIED_EMAIL', 'SEND_VERIFIED_EMAIL':1, 2:'SEND_VERIFIED_COMPANY', 'SEND_VERIFIED_COMPANY':2}
    """
     - SEND_VERIFIED_EMAIL - send verified e-mail blob with this auth request
     - SEND_VERIFIED_COMPANY - send verified company blob with this auth request
    """
    CAPABILITY = {
        0 :'CAPABILITY_VOICEMAIL',
        1 :'CAPABILITY_SKYPEOUT',
        2 :'CAPABILITY_SKYPEIN',
        3 :'CAPABILITY_CAN_BE_SENT_VM',
        4 :'CAPABILITY_CALL_FORWARD',
        5 :'CAPABILITY_VIDEO',
        6 :'CAPABILITY_TEXT',
        7 :'CAPABILITY_SERVICE_PROVIDER',
        8 :'CAPABILITY_LARGE_CONFERENCE',
        9 :'CAPABILITY_COMMERCIAL_CONTACT',
        10:'CAPABILITY_PSTN_TRANSFER',
        11:'CAPABILITY_TEXT_EVER',
        12:'CAPABILITY_VOICE_EVER',
        13:'CAPABILITY_MOBILE_DEVICE',
        14:'CAPABILITY_PUBLIC_CONTACT',
        'CAPABILITY_VOICEMAIL'
        
                      : 0,
        'CAPABILITY_SKYPEOUT'
        
                       : 1,
        'CAPABILITY_SKYPEIN'
        
                
        : 2,
        'CAPABILITY_CAN_BE_SENT_VM'
        
                 : 3,
        'CAPABILITY_CALL_FORWARD'
        
                   : 4,
        'CAPABILITY_VIDEO'
        
                
          : 5,
        'CAPABILITY_TEXT'
        
                
           : 6,
        'CAPABILITY_SERVICE_PROVIDER'
        
               : 7,
        'CAPABILITY_LARGE_CONFERENCE'
        
               : 8,
        'CAPABILITY_COMMERCIAL_CONTACT'
        
             : 9,
        'CAPABILITY_PSTN_TRANSFER'
        
                  :10,
        'CAPABILITY_TEXT_EVER'
        
                      :11,
        'CAPABILITY_VOICE_EVER'
        
                     :12,
        'CAPABILITY_MOBILE_DEVICE'
        
                  :13,
        'CAPABILITY_PUBLIC_CONTACT'
        
                 :14
    }
    """
    This enumerator is used by both Contact and Account objects. Thus the items here can have slightly different meaning, depending on which context you will examine their values. In case of Contact, the values apply to a user - across all the instances that user has logged in with Skype. In case of Account, the capability is that of a local, currently logged in instance of Skype client. 
    
    The values that CAPABILITY items can have are also dependant on class context. In context of Contact, a capability can be CAPABILITY_MIXED. Which in case of CAPABILITY_VIDEO, for example, would mean that the remote use has logged in with different clients, some of which support video calls and some of which don't. In context of Account - there are no mixed result. Currently logged in Skype instance either supports video or it doesn't. 
    
     - CAPABILITY_VOICEMAIL - For Account object, this is the same as CAPABILITY_CAN_BE_SENT_VM - it indicates that the currently logged in Skype instance supports voicemails. For Contact objects, it means that their remote system supports sending voicemails - there is no technical method for Skype to detect whether they are capable of receiving voicemails, so the assumption is that they can. 
    
     - CAPABILITY_SKYPEOUT - Indicates that the contact/account has SkypeOut and is thus capable of making PSTN calls. 
    
     - CAPABILITY_SKYPEIN - Indicates that the contact/account has SkypeIn and is thus capable of answering PSTN calls. 
    
     - CAPABILITY_CAN_BE_SENT_VM - For contacts, this is a combination of CAPABILITY_VOICEMAIL for local account (local Skype client supports sending voicemails) and CAPABILITY_VOICEMAIL of the Contact - if the contact supports sending voicemails then hopefully they can also receive them. 
    
     - CAPABILITY_CALL_FORWARD - Indicates that Account/Contact supports call forwarding. 
    
     - CAPABILITY_VIDEO - Indicates that Account/Contact supports call video calls. 
    
     - CAPABILITY_TEXT - In context of Contact, this indicates that the user is noticed running at least one Skype implementation that supports text messaging. Basically, it applies to a user. When applied to Account, the meaning is slightly different. In that case it indicates that currently running Skype implementation supports chat messaging. So, for Account objects, this is a node (rather than user) capability. 
    
     - CAPABILITY_SERVICE_PROVIDER - Indicates that the contact/account is flagged as SkypePrime service provider. This is linked to Account class SERVICE_PROVIDER_INFO property. 
    
     - CAPABILITY_LARGE_CONFERENCE - This is a legacy item, from the old times when conference calls with more than 5 people were limited to SkypePro accounts. In other words, this item is no longer relevant and will likely be removed at some point in future. 
    
     - CAPABILITY_COMMERCIAL_CONTACT
     - CAPABILITY_PSTN_TRANSFER - Indicates that Account/Contact supports call transfers to PSTN numbers. 
    
     - CAPABILITY_TEXT_EVER - Indicates that the user has had his chat capability removed by Skype. Basically, this means that the user is reported as spammer too many times. This applies for both Contact and Account objects - which means your client can check locally, if the currently logged in user has been marked as a spammer. 
    
     - CAPABILITY_VOICE_EVER - Indicates that the user (Account or Contact) has had his voice call capability removed by Skype. 
    
     - CAPABILITY_MOBILE_DEVICE - Indicates that the instance of Skype client Account/Contact is or in Contact's case has at least occasionally been flagged as a mobile device. 
    
     - CAPABILITY_PUBLIC_CONTACT
    """
    CAPABILITYSTATUS = {0:'NO_CAPABILITY', 'NO_CAPABILITY':0, 1:'CAPABILITY_MIXED', 'CAPABILITY_MIXED':1, 2:'CAPABILITY_EXISTS', 'CAPABILITY_EXISTS':2}
    """
    List of possible states of each of the Contact class CAPABILITY items. 
    
     - NO_CAPABILITY - Contact does not have the capability 
    
     - CAPABILITY_MIXED - Contact has occasionally logged in with Skype client that supports the capability. For example, a contact may have Skype client on several machines, only some of which have webcam - in which case CAPABILITY_VIDEO would have its value set as CAPABILITY_MIXED. 
    
     - CAPABILITY_EXISTS - Contact has the capability 
    
    """
    
    def _sk_get_type(self):
        return skypekit.enumof(Contact.TYPE, self._sk_property("ZG\312\001]\002", 202, True))
    type = property(_sk_get_type)
    propid2label[202] = "type"
    P_TYPE = 202
    def _sk_get_skypename(self):
        """
        defined if it is a SKYPE contact
        """
        return self._sk_property("ZG\004]\002", 4, True)
    skypename = property(_sk_get_skypename)
    propid2label[4] = "skypename"
    P_SKYPENAME = 4
    def _sk_get_pstnnumber(self):
        return self._sk_property("ZG\006]\002", 6, True)
    pstnnumber = property(_sk_get_pstnnumber)
    propid2label[6] = "pstnnumber"
    P_PSTNNUMBER = 6
    def _sk_get_fullname(self):
        return self._sk_property("ZG\005]\002", 5, True)
    fullname = property(_sk_get_fullname)
    propid2label[5] = "fullname"
    P_FULLNAME = 5
    def _sk_get_birthday(self):
        """
        integer of YYYYMMDD format
        """
        return self._sk_property("ZG\007]\002", 7, True)
    birthday = property(_sk_get_birthday)
    propid2label[7] = "birthday"
    P_BIRTHDAY = 7
    def _sk_get_gender(self):
        """
        1-male, 2-female
        """
        return self._sk_property("ZG\010]\002", 8, True)
    gender = property(_sk_get_gender)
    propid2label[8] = "gender"
    P_GENDER = 8
    def _sk_get_languages(self):
        """
        ISO language code list, space separated
        """
        return self._sk_property("ZG\011]\002", 9, True)
    languages = property(_sk_get_languages)
    propid2label[9] = "languages"
    P_LANGUAGES = 9
    def _sk_get_country(self):
        """
        ISO country code
        """
        return self._sk_property("ZG\012]\002", 10, True)
    country = property(_sk_get_country)
    propid2label[10] = "country"
    P_COUNTRY = 10
    def _sk_get_province(self):
        return self._sk_property("ZG\013]\002", 11, True)
    province = property(_sk_get_province)
    propid2label[11] = "province"
    P_PROVINCE = 11
    def _sk_get_city(self):
        return self._sk_property("ZG\014]\002", 12, True)
    city = property(_sk_get_city)
    propid2label[12] = "city"
    P_CITY = 12
    def _sk_get_phone_home(self):
        """
        This corresponds to the Account::P_PHONE_HOME property. The value is set by the remote user in Account profile. As Contact property, this is read-only. 
        
        """
        return self._sk_property("ZG\015]\002", 13, True)
    phone_home = property(_sk_get_phone_home)
    propid2label[13] = "phone_home"
    P_PHONE_HOME = 13
    def _sk_get_phone_office(self):
        """
        This corresponds to the Account::P_PHONE_OFFICE property. The value is set by the remote user in Account profile. As Contact property, this is read-only. If the Contact has not populated his Account profile with sufficient phone numbers, the UI should implement locally adding additional phone numbers to Contact P_ASSIGNED_PHONE1 .. P_ASSIGNED_PHONE3 properties (and corresponding labels). See Contact::SetPhoneNumber method for more information. 
        
        """
        return self._sk_property("ZG\016]\002", 14, True)
    phone_office = property(_sk_get_phone_office)
    propid2label[14] = "phone_office"
    P_PHONE_OFFICE = 14
    def _sk_get_phone_mobile(self):
        """
        This corresponds to the Account::P_PHONE_MOBILE property. The value is set by the remote user in Account profile. As Contact property, this is read-only. 
        
        """
        return self._sk_property("ZG\017]\002", 15, True)
    phone_mobile = property(_sk_get_phone_mobile)
    propid2label[15] = "phone_mobile"
    P_PHONE_MOBILE = 15
    def _sk_get_emails(self):
        """
        will be hashed before advertising/querying, space separated
        """
        return self._sk_property("ZG\020]\002", 16, True)
    emails = property(_sk_get_emails)
    propid2label[16] = "emails"
    P_EMAILS = 16
    def _sk_get_homepage(self):
        return self._sk_property("ZG\021]\002", 17, True)
    homepage = property(_sk_get_homepage)
    propid2label[17] = "homepage"
    P_HOMEPAGE = 17
    def _sk_get_about(self):
        """
        arbitrary introductory text
        """
        return self._sk_property("ZG\022]\002", 18, True)
    about = property(_sk_get_about)
    propid2label[18] = "about"
    P_ABOUT = 18
    def _sk_get_avatar_image(self):
        """
        Contact avatar pictures are in JPG format. The original size of avatar pictures are in no more than 96 x 96 pixels in size. However, as they can be smaller, scaling the pictures up too much to fit your UI can lead to distorted images. 
        
        """
        return self._sk_property("ZG%]\002", 37, False)
    avatar_image = property(_sk_get_avatar_image)
    propid2label[37] = "avatar_image"
    P_AVATAR_IMAGE = 37
    def _sk_get_mood_text(self):
        """
        Personal mood message (visible to authorized users only). 
        
        """
        return self._sk_property("ZG\032]\002", 26, True)
    mood_text = property(_sk_get_mood_text)
    propid2label[26] = "mood_text"
    P_MOOD_TEXT = 26
    def _sk_get_rich_mood_text(self):
        """
        XML version of personal mood text
        """
        return self._sk_property("ZG\315\001]\002", 205, True)
    rich_mood_text = property(_sk_get_rich_mood_text)
    propid2label[205] = "rich_mood_text"
    P_RICH_MOOD_TEXT = 205
    def _sk_get_timezone(self):
        """
        24*3600+diff_to_UTC_in_seconds. NB! changes with DST 
        
        """
        return self._sk_property("ZG\033]\002", 27, True)
    timezone = property(_sk_get_timezone)
    propid2label[27] = "timezone"
    P_TIMEZONE = 27
    def _sk_get_capabilities(self):
        """
        binary tag that can be queried via Contact::HasCapability()
        """
        return self._sk_property("ZG$]\002", 36, False)
    capabilities = property(_sk_get_capabilities)
    propid2label[36] = "capabilities"
    P_CAPABILITIES = 36
    def _sk_get_profile_timestamp(self):
        """
        UNIX timestamp of last profile change
        """
        return self._sk_property("ZG\023]\002", 19, True)
    profile_timestamp = property(_sk_get_profile_timestamp)
    propid2label[19] = "profile_timestamp"
    P_PROFILE_TIMESTAMP = 19
    def _sk_get_nrof_authed_buddies(self):
        """
        count of this user's authorized contacts 
        
        """
        return self._sk_property("ZG\034]\002", 28, True)
    nrof_authed_buddies = property(_sk_get_nrof_authed_buddies)
    propid2label[28] = "nrof_authed_buddies"
    P_NROF_AUTHED_BUDDIES = 28
    def _sk_get_ipcountry(self):
        """
        ISO country code assigned by the IP
        """
        return self._sk_property("ZG\035]\002", 29, True)
    ipcountry = property(_sk_get_ipcountry)
    propid2label[29] = "ipcountry"
    P_IPCOUNTRY = 29
    def _sk_get_avatar_timestamp(self):
        """
        UNIX timestamp of when current avatar was set
        """
        return self._sk_property("ZG\266\001]\002", 182, False)
    avatar_timestamp = property(_sk_get_avatar_timestamp)
    propid2label[182] = "avatar_timestamp"
    P_AVATAR_TIMESTAMP = 182
    def _sk_get_mood_timestamp(self):
        """
        NOT SET FOR CONTACTS. For Account object, UNIX timestamp of when current mood was set
        """
        return self._sk_property("ZG\267\001]\002", 183, True)
    mood_timestamp = property(_sk_get_mood_timestamp)
    propid2label[183] = "mood_timestamp"
    P_MOOD_TIMESTAMP = 183
    def _sk_get_received_authrequest(self):
        """
        set if the contact is waiting to be authorized. The value contains auth. request text 
        
        """
        return self._sk_property("ZG\024]\002", 20, True)
    received_authrequest = property(_sk_get_received_authrequest)
    propid2label[20] = "received_authrequest"
    P_RECEIVED_AUTHREQUEST = 20
    def _sk_get_authreq_timestamp(self):
        """
        timestamp of last received auth-request
        """
        return self._sk_property("ZG\031]\002", 25, True)
    authreq_timestamp = property(_sk_get_authreq_timestamp)
    propid2label[25] = "authreq_timestamp"
    P_AUTHREQ_TIMESTAMP = 25
    def _sk_get_lastonline_timestamp(self):
        """
        X timestamp of last successful ping to that user
        """
        return self._sk_property("ZG#]\002", 35, False)
    lastonline_timestamp = property(_sk_get_lastonline_timestamp)
    propid2label[35] = "lastonline_timestamp"
    P_LASTONLINE_TIMESTAMP = 35
    def _sk_get_availability(self):
        """
        Contact::AVAILABILITY
        """
        return skypekit.enumof(Contact.AVAILABILITY, self._sk_property("ZG\042]\002", 34, True))
    availability = property(_sk_get_availability)
    propid2label[34] = "availability"
    P_AVAILABILITY = 34
    def _sk_get_displayname(self):
        """
        always set (assigned by lib by looking at various fields)
        """
        return self._sk_property("ZG\025]\002", 21, True)
    displayname = property(_sk_get_displayname)
    propid2label[21] = "displayname"
    P_DISPLAYNAME = 21
    def _sk_get_refreshing(self):
        """
        true if querying additional information from p2p or server 
        
        """
        return self._sk_property("ZG\026]\002", 22, True)
    refreshing = property(_sk_get_refreshing)
    propid2label[22] = "refreshing"
    P_REFRESHING = 22
    def _sk_get_given_authlevel(self):
        """
        Contact::AUTHLEVEL, change via Contact::GiveAuthlevel()
        """
        return skypekit.enumof(Contact.AUTHLEVEL, self._sk_property("ZG\027]\002", 23, False))
    given_authlevel = property(_sk_get_given_authlevel)
    propid2label[23] = "given_authlevel"
    P_GIVEN_AUTHLEVEL = 23
    def _sk_get_given_displayname(self):
        """
        change via Contact::GiveDisplayname()
        """
        return self._sk_property("ZG!]\002", 33, True)
    given_displayname = property(_sk_get_given_displayname)
    propid2label[33] = "given_displayname"
    P_GIVEN_DISPLAYNAME = 33
    def _sk_get_assigned_comment(self):
        """
        change via Contact::AssignComment()
        """
        return self._sk_property("ZG\264\001]\002", 180, True)
    assigned_comment = property(_sk_get_assigned_comment)
    propid2label[180] = "assigned_comment"
    P_ASSIGNED_COMMENT = 180
    def _sk_get_lastused_timestamp(self):
        """
        UNIX timestamp of last outbound session (call, chat, FT, etc)
        """
        return self._sk_property("ZG']\002", 39, True)
    lastused_timestamp = property(_sk_get_lastused_timestamp)
    propid2label[39] = "lastused_timestamp"
    P_LASTUSED_TIMESTAMP = 39
    def _sk_get_authrequest_count(self):
        """
        for contacts that have CONTACT_RECEIVED_AUTHREQUEST, how many times in a row they have requested it without positive answer
        """
        return self._sk_property("ZG)]\002", 41, True)
    authrequest_count = property(_sk_get_authrequest_count)
    propid2label[41] = "authrequest_count"
    P_AUTHREQUEST_COUNT = 41
    def _sk_get_assigned_phone1(self):
        """
        Office phone no. of the contact. This property should NOT be used for SkypeOut contacts 
        
        """
        return self._sk_property("ZG\270\001]\002", 184, True)
    assigned_phone1 = property(_sk_get_assigned_phone1)
    propid2label[184] = "assigned_phone1"
    P_ASSIGNED_PHONE1 = 184
    def _sk_get_assigned_phone1_label(self):
        """
        Value "1" in this property will be translated as "Office" by Skype Windows desktop client, according to UI language settings 
        
        """
        return self._sk_property("ZG\271\001]\002", 185, True)
    assigned_phone1_label = property(_sk_get_assigned_phone1_label)
    propid2label[185] = "assigned_phone1_label"
    P_ASSIGNED_PHONE1_LABEL = 185
    def _sk_get_assigned_phone2(self):
        """
        Mobile phone no. of the contact 
        
        """
        return self._sk_property("ZG\272\001]\002", 186, True)
    assigned_phone2 = property(_sk_get_assigned_phone2)
    propid2label[186] = "assigned_phone2"
    P_ASSIGNED_PHONE2 = 186
    def _sk_get_assigned_phone2_label(self):
        """
        Value "2" in this property will be translated as "Mobile" by Skype Windows desktop client, according to UI language settings 
        
        """
        return self._sk_property("ZG\273\001]\002", 187, True)
    assigned_phone2_label = property(_sk_get_assigned_phone2_label)
    propid2label[187] = "assigned_phone2_label"
    P_ASSIGNED_PHONE2_LABEL = 187
    def _sk_get_assigned_phone3(self):
        """
        "Other phone no. of the contact
        """
        return self._sk_property("ZG\274\001]\002", 188, True)
    assigned_phone3 = property(_sk_get_assigned_phone3)
    propid2label[188] = "assigned_phone3"
    P_ASSIGNED_PHONE3 = 188
    def _sk_get_assigned_phone3_label(self):
        """
        Value "3" in this property will be translated as "Other" by Skype Windows desktop client, according to UI language settings 
        
        """
        return self._sk_property("ZG\275\001]\002", 189, True)
    assigned_phone3_label = property(_sk_get_assigned_phone3_label)
    propid2label[189] = "assigned_phone3_label"
    P_ASSIGNED_PHONE3_LABEL = 189
    def _sk_get_popularity_ord(self):
        """
        Contact's order by presence popularity 
        
        """
        return self._sk_property("ZG*]\002", 42, True)
    popularity_ord = property(_sk_get_popularity_ord)
    propid2label[42] = "popularity_ord"
    P_POPULARITY_ORD = 42
    
    def GetType(self):
        """
        Getter method for the TYPE property. 
        
        
        B{Return values:}
         - B{type_}
        """
        l_request = skypekit.XCallRequest("ZR\002\001", 2, 1)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = skypekit.enumof(Contact.TYPE, l_response.get(1))
        return l_result
    def GetIdentity(self):
        """
        No description available.
        
        B{Return values:}
         - B{identity} - returns CONTACT_SKYPENAME or CONTACT_PSTNNUMBER value
        """
        l_request = skypekit.XCallRequest("ZR\002\002", 2, 2)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def GetAvatar(self):
        """
        Returns Conrtact's avatar image (JPG). 
        
        
        B{Return values:}
         - B{present} - 
          - true: the Contact has a custom avatar image 
          - false: the Contact does not have a custom avatar image 
        
         - B{avatar} - The avatar image data (JPG). If present is false, this will be the Skype-assigned default avatar 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\004", 2, 4)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, False)),
        l_result += (l_response.get(2, '')),
        return l_result
    def GetVerifiedEmail(self):
        """
        returns verified-by-Skype e-mail for this contact if exists and verifiable
        
        B{Return values:}
         - B{email}
        """
        l_request = skypekit.XCallRequest("ZR\002\003", 2, 3)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def GetVerifiedCompany(self):
        """
        returns verified-by-Skype company for this contact if exists and verifiable
        
        B{Return values:}
         - B{company}
        """
        l_request = skypekit.XCallRequest("ZR\002\010", 2, 8)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def IsMemberOf(
        self,
        group
        ):
        """
        Checks whether the contact is member of a contact group given in group reference argument. 
        
        
        B{Arguments:}
         - B{group} - The target contact group 
        
        
        B{Return values:}
         - B{result} - 
          - true: the Contact is a member of the target contact group 
          - false: the Contact is not a member of the target contact group 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\006", 2, 6)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, group)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def IsMemberOfHardwiredGroup(
        self,
        group_type
        ):
        """
        Checks whether the contact is member of a pre-defined contact group given in the TYPE argument (type for this property comes from the ContactGroup class). 
        
        
        B{Arguments:}
         - B{group_type} - The type designator of the target pre-defined contact group. For example, specify this parameter as ContactGroup::TYPE.RECENTLY_CONTACTED_CONTACTS to determine if you've had a recent conversation with this Contact. 
        
        
        B{Return values:}
         - B{result} - 
          - true: the Contact is a member of the target contact group 
          - false: the Contact is not a member of the target contact group 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\007", 2, 7)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, ContactGroup.TYPE[group_type])
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def SetBlocked(
        self,
        blocked,
        abuse = False
        ):
        """
        Blocks or unblocks any further incoming communication attempts from this contact. 
        
        
        B{Arguments:}
         - B{blocked} - 
          - true: block this contact 
          - false: unblock this contact 
        
         - B{abuse} - Optional parameter to report abuse by this Skype user when blocking this Contact. Note that you can specifiy this parameter as true only when blocking a Contact. Defaults to false if omitted. 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\026", 2, 22)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('b', 1, blocked)
        l_request.add_parm('b', 2, abuse)
        self.transport.xcall(l_request)
    def IgnoreAuthRequest(self):
        """
        Rejects and removes a pending authorization request from this Contact. 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\025", 2, 21)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def GiveDisplayName(
        self,
        name
        ):
        """
        sets CONTACT_GIVEN_DISPLAYNAME. clears if size(name)==0
        
        B{Arguments:}
         - B{name}
        """
        l_request = skypekit.XCallRequest("ZR\002\012", 2, 10)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, name)
        self.transport.xcall(l_request)
    def SetBuddyStatus(
        self,
        is_my_buddy,
        sync_auth = True
        ):
        """
        Adds or removes this Contact from the ALL_BUDDIES hardwired group. 
        
        
        B{Arguments:}
         - B{is_my_buddy} - 
          - true: add this contact to the ALL_BUDDIES group 
          - false: delete contact from the ALL_BUDDIES group 
        
         - B{sync_auth} - This argument is deprecated and should not be used. 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\014", 2, 12)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('b', 1, is_my_buddy)
        l_request.add_parm('b', 2, sync_auth)
        self.transport.xcall(l_request)
    def SendAuthRequest(
        self,
        message,
        extras_bitmask = 0
        ):
        """
        Sends a contact authorization request to this user. 
        
        
        B{Arguments:}
         - B{message} - Text that typically introduces the requesting user and details the reason for the authorization request. This text will be set as Message::P_BODY_XML property of the notification Message the remote user will receive upon getting the authortization request. While this can be the empty string, it cannot be null. 
        
         - B{extras_bitmask} - Indicates additional information to include with this authorization request: 
          - 0 (zero): do not include any additional information 
          - SEND_VERIFIED_EMAIL: include the requestor's verified e-mail address 
          - SEND_VERIFIED_COMPANY: include verified information regarding the requestor's company 
          - SEND_VERIFIED_EMAIL + SEND_VERIFIED_COMPANY: include both e-mail and company information 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\015", 2, 13)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, message)
        l_request.add_parm('u', 2, extras_bitmask)
        self.transport.xcall(l_request)
    def HasAuthorizedMe(self):
        """
        the contact has accepted my auth request
        
        B{Return values:}
         - B{result}
        """
        l_request = skypekit.XCallRequest("ZR\002\016", 2, 14)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def SetPhoneNumber(
        self,
        num,
        label,
        number
        ):
        """
        Sets the three P_ASSIGNED_PHONEx and P_ASSIGNED_PHONEx_LABEL properties, where x reflects the value of num. 
        
        The Skype Windows desktop client uses the following conventions when interpreting and assigning values to these properties. While your solution can assign arbitrary values to these properties, we strongly recommend using these conventions to ensure interoperability with the Skype Windows desktop client. Keep in mind that the "number" of these property pairs has no relationship to how the Skype Windows client interprets their label property value strings. For example, the Skype Windows client will interpret P_ASSIGNED_PHONE3_LABEL as "Home" if its value is the string "0". 
        
        Label strings: 
        
        Populate the label properties with string representations of the numbers "0" through "3", rather than descriptive strings like "Home", "Mobile", and so forth. The Skype desktop clients interpret the numeric string values as: 
          - "0" (zero) - "Home" 
          - "1" (one) - "Office" 
          - "2" - "Mobile" 
          - "3" - "Other" 
        
        Keep in mind that the "number" of a property pair has no relationship to its label string. For example, the Skype Windows client will interpret P_ASSIGNED_PHONE3_LABEL as "Home" if its value is the string "0". 
        
        Phone number strings: 
        
        The Skype Windows desktop client has distinct conventions for Skype Contacts and PSTN (SkypeOut) Contacts: any or all of the property pairs can be used for Skype Contacts; P_ASSIGNED_PHONE1 cannot be used for PSTN Contacts and P_ASSIGNED_PHONE1_LABEL has special meaning for PSTN Contacts. 
        Specifically, the Skype desktop clients use P_ASSIGNED_PHONE1_LABEL as the label for a PSTN Contact's primary number (regardless of whether it's home, mobile, or office), and use P_PSTNNUMBER to hold the actual number. 
        
        
        
        B{Arguments:}
         - B{num} - The property pair being set, which must be in the range 0..3 
        
         - B{label} - The label text for the property being set 
        
         - B{number} - The phone number for the property being set 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\017", 2, 15)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('u', 1, num)
        l_request.add_parm('S', 2, label)
        l_request.add_parm('S', 3, number)
        self.transport.xcall(l_request)
    def OpenConversation(self):
        """
        Retrieves a dialog conversation with the Contact. 
        
        
        B{Return values:}
         - B{conversation} - Retrieved dialog. 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\021", 2, 17)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[18](l_response.get(1), self.transport)
        return l_result
    def HasCapability(
        self,
        capability,
        query_server = False
        ):
        """
        Retrieves a Contact::Capability value. Sets P_REFRESHING to true while querying from server. This method is functionally the same as Contact::GetCapabilityStatus except that it returns a bool value rather than Contact::CAPABILITYSTATUS 
        
        
        B{Arguments:}
         - B{capability} - The target capability 
        
         - B{query_server} - 
          - true: obtains the data from the p2p network/server, and sets P_REFRESHING to true for the duration 
          - false: obtains the data from the local client 
        
        
        B{Return values:}
         - B{result} - 
          - true: the Contact has the target capability through at least one of the Skype clients they have logged into. Corresponds to CAPABILITY_MIXED and CAPABILITY_EXISTS 
          - false: the Contact does not have the target capability. Corresponds to NO_CAPABILITY 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\022", 2, 18)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Contact.CAPABILITY[capability])
        l_request.add_parm('b', 2, query_server)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def GetCapabilityStatus(
        self,
        capability,
        query_server = False
        ):
        """
        Retrieves a Contact::Capability value. Sets P_REFRESHING to true while querying from server. 
        
        
        B{Arguments:}
         - B{capability} - The target capability, see Contact::Capability enumerator. 
        
         - B{query_server} - 
          - true: obtains the data from the p2p network/server, and sets P_REFRESHING to true for the duration 
          - false: obtains the data from the local client 
        
        
        B{Return values:}
         - B{status} - Status of the target capability. 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\023", 2, 19)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Contact.CAPABILITY[capability])
        l_request.add_parm('b', 2, query_server)
        l_response = self.transport.xcall(l_request)
        l_result = skypekit.enumof(Contact.CAPABILITYSTATUS, l_response.get(1))
        return l_result
    def RefreshProfile(self):
        """
        Refreshes all properties, capabilities, and statuses associated with this Contact from p2p/CBL, and sets P_REFRESHING to true for the duration. 
        
        """
        l_request = skypekit.XCallRequest("ZR\002\024", 2, 20)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def mget_profile(self):
        """ get multiple properties from self """
        self.multiget("ZG\004,\006,\005,\032,\020,\015,\016,\017,\007,\010,\011,\012,\013,\014,\021,\022,\033]\002")
module_id2classes[2] = Contact

class ContactSearch(skypekit.Object):
    """
    This class encapsulates functionality for looking up contacts on the Skype network. Contacts can be searched by portion of their name, e-mail address, language preferences, etc. 
    
    Contact search is asynchronous. ContactSearch::Submit is a non-blocking function that initiates the search. Upon finding a matching contact, ContactSearch::OnNewResult event gets fired, that gives you the reference to the discovered contact. You can get up to 100 matching contacts per search. Note that you will need to keep a live reference of the ContactSearch object while the search is doing its work. 
    
    So, to perform a contact search: 
      - create a contact search object 
      - specify search terms and conditions 
      - submit search 
      - in ContactSearch::OnNewResult callback, update your UI 
      - in ContactSearch::OnChange, check for terminal values of P_CONTACT_SEARCH_STATUS and update the UI accordingly. 
    
    When the search has done its job, the ContactSearch::P_CONTACT_SEARCH_STATUS property will go to one of the terminal values. 
    
    The terminal values are: 
      - FINISHED - the search has stopped. Note that this does not mean any matches were actually found. 
      - FAILED - the search has failed. 
      - EXTENDABLE - this state should be considered the same as FINISHED. The feature of extending long search results is about to be deprecated. It is still possible for search objects to occasionally reach that state, so it should be handled in the UI (as FINISHED), but the extending feature itself should not be implemented in your UI. 
    
    There are three methods to create the ContactSearch objects. 
    
    A) Skype::CreateIdentitySearch 
    
    This method takes a string argument and looks for exact matches against Contact::P_SKYPENAME property. So for example, identity search for "echo" will return 0 results and search for "echo123" will return exactly one. 
    
    Identity in this case means skypename - contact search does not work with PSTN type contacts. However, it does work for SKYPE type contacts that have supplied P_PHONE_HOME, P_PHONE_OFFICE or P_PHONE_MOBILE values in their account data. To search for those, you will need to use complex search (see below). 
    
    Note that you should always check for boolean return value of the CreateIdentitySearch method. If the user submits a string that is not a valid skypename, the method will return false and the ContactSearchRef argument will return as NULL. 
    
    B) Skype::CreateBasicContactSearch 
    
    This method takes a string argument and looks for non-exact matches against both P_SKYPENAME and P_FULLNAME properties of the contact. If you intend to implement a simple, one-input search feature - this is the best method for you. The non-exact matching operates similarly to the SQL LIKE condition. 
    
    C) Skype::CreateContactSearch 
    
    This method enables you to implement advanced contact search, matching against multiple seach criteria. It takes no input arguments and expects search criteria to be added to the already constructed search object. 
    
    Criteria can be added with ContactSearch::AddStrTerm and ContactSearch::AddIntTerm methods. 
    
    These methods take Contact class porperty ID, condition, and the match pattern as inputs. 
    
    Only the following Contact properties can be used for search: 
      - P_SKYPENAME 
      - P_FULLNAME 
      - P_BIRTHDAY (uint) 
      - P_GENDER (uint: 1-male, 2-female) 
      - P_LANGUAGES 
      - P_COUNTRY 
      - P_PROVINCE 
      - P_CITY 
      - P_PHONE_HOME 
      - P_PHONE_OFFICE 
      - P_PHONE_MOBILE 
      - P_EMAILS 
      - P_HOMEPAGE 
      - P_ABOUT 
    
    String searches are case insensitive, i.e. search for echo123 also matches ECHO123 
    
    When adding multiple criteria, default behaviour is that the criterions are additive. I.e. a term skypename == "joe" followed by term country == "us" will result in intersection between all joes and everybody in US. 
    
    You can explicitly add an "OR" instead of "AND" between conditions, using the AddOr method. 
    
    By default, AND criteria are grouped together, before OR's, so that: 
    
    AddTerm(condition1) 
    AddTerm(condition2) 
    AddOr() 
    AddTerm(condition3) 
    AddTerm(condition4) 
    
    will result in the following logical statement: 
    (condition1 AND condition2) OR (condition3 AND condition4) 
    
    However, you can add "global" critera, by using the add_to_subs argument of the AddXX methods. 
    
    AddTerm(condition1) 
    AddTerm(condition2) 
    AddOr() 
    AddTerm(condition3) 
    AddTerm(condition4, add_to_subs=true) 
    
    which would result in: 
    (condition1 AND condition2 AND condition4) OR (condition3 AND condition4) 
    
    
    Every one of the contact properties can only be used once, per search. For example, you cannot create a search for two different P_FULLNAME patterns. The &valid argument will still return tue if you do this, but the last criteria for any given property will override all previous ones. So, a search like this: 
    
    cs->AddStrTerm(Contact::P_FULLNAME, ContactSearch::EQ, "John Smith", isValid); 
    cs->AddOr(); 
    cs->AddStrTerm(Contact::P_FULLNAME, ContactSearch::EQ, "Ivan Sidorov", isValid); 
    
    will only return matches for "Ivan Sidorov" and none for "John Smith". 
    
    Some of the contact properties are automatically combined for purposes of search. 
    
    A search for P_SKYPENAME also returns matches from the P_FULLNAME property and vice versa. 
    
    So that this: 
    cs->AddStrTerm(Contact::P_SKYPENAME, ContactSearch::EQ, "john.smith", isValid); 
    
    ..and this: 
    cs->AddStrTerm(Contact::P_FULLNAME, ContactSearch::EQ, "John Smith", isValid); 
    
    ..and this: 
    cs->AddStrTerm(Contact::P_SKYPENAME, ContactSearch::EQ, "john.smith", isValid); 
    cs->AddOr(); 
    cs->AddStrTerm(Contact::P_FULLNAME, ContactSearch::EQ, "John Smith", isValid); 
    
    ..all search from both the P_FULLNAME and P_SKYPENAME fields. 
    
    
    Before using ContactGroup::Submit to start the search, you should always check whether the search criteria ended up being valid. This you can do with ContactSearch::IsValid method. 
    
    As you probably noticed, each of the AddXX methods also return a validity check boolean. However, it is a better practice to do the overall check as well, even if all the individual search criteria ended up looking Ok. 
    
    For example, lets take a search for contact's e-mail. This can be done with two different methods. Firstly we can use the ContactSearch::AddEmailTerm method. This method will actually validate whether the input is a valid e-mail address: 
    
    cs->AddEmailTerm ("test@test@test", isValid); 
    will return the isValid argument as false. 
    
    However, you can also add the e-mail search criterion as a simple string, like this: 
    
    cs->AddStrTerm(Contact::P_EMAILS, ContactSearch::EQ, "test@test@test@", isValid); 
    in which case the isValid will return true. 
    
    However, if you then check entire search object with: 
    
    cs->IsValid(isValid); 
    
    the isValid will correctly return false. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "ContactSearch %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(ContactSearch, "P_"+propname.upper())
    module_id = 1
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    STATUS = {1:'CONSTRUCTION', 'CONSTRUCTION':1, 2:'PENDING', 'PENDING':2, 3:'EXTENDABLE', 'EXTENDABLE':3, 4:'FINISHED', 'FINISHED':4, 5:'FAILED', 'FAILED':5}
    """
    Possible values for the ContactSearch.P_STATUS property. 
    
     - CONSTRUCTION - Transient state, obtained after submission and actually initiating the search on the network. 
    
     - PENDING - Waiting for results to show up. This is a transient state. 
    
     - EXTENDABLE - Enough matches are found. No more OnNewResult events will fire. The feature of extending long search results is about to be deprecated. It is still possible for search objects to occasionally reach that state, so it should be handled in the UI (as FINISHED), but the extending feature itself should not be implemented in your UI. 
    
     - FINISHED - The search is finished. No more matches are expected. This is a terminal state. 
    
     - FAILED - ContactSearch failed. Better check if the search terms made any sense, with ContactSearch::IsValid. This is a terminal state. 
    
    """
    CONDITION = {
        0:'EQ',
        1:'GT',
        2:'GE',
        3:'LT',
        4:'LE',
        5:'PREFIX_EQ',
        6:'PREFIX_GE',
        7:'PREFIX_LE',
        8:'CONTAINS_WORDS',
        9:'CONTAINS_WORD_PREFIXES',
        'EQ'
        
                
                
        :0,
        'GT'
        
                
                
        :1,
        'GE'
        
                
                
        :2,
        'LT'
        
                
                
        :3,
        'LE'
        
                
                
        :4,
        'PREFIX_EQ'
        
                
                 :5,
        'PREFIX_GE'
        
                
                 :6,
        'PREFIX_LE'
        
                
                 :7,
        'CONTAINS_WORDS'
        
                
            :8,
        'CONTAINS_WORD_PREFIXES'
        
                    :9
    }
    """
    List of available matching conditions that can be used in AddTerm methods. 
    
     - EQ - Equals 
    
     - GT - Is greater than 
    
     - GE - Is greater or equal. 
    
     - LT - Is less than 
    
     - LE - Less or equal 
    
     - PREFIX_EQ - Start of a word macthes exactly (string properties only). 
    
     - PREFIX_GE - Start of a word is greater or equal (string properties only). 
    
     - PREFIX_LE - Start of a word is less or equal (string properties only). 
    
     - CONTAINS_WORDS - Contains the word (string properties only). 
    
     - CONTAINS_WORD_PREFIXES - One of the words starts with searched value (string properties only). 
    
    """
    
    def _sk_get_contact_search_status(self):
        return skypekit.enumof(ContactSearch.STATUS, self._sk_property("ZG\310\001]\001", 200, True))
    contact_search_status = property(_sk_get_contact_search_status)
    propid2label[200] = "contact_search_status"
    P_CONTACT_SEARCH_STATUS = 200
    
    def AddMinAgeTerm(
        self,
        min_age_in_years,
        add_to_subs = False
        ):
        """
        construct CONTACT_BIRTHDAY term based on current time
        
        B{Arguments:}
         - B{min_age_in_years}
         - B{add_to_subs}
        
        B{Return values:}
         - B{valid}
        """
        l_request = skypekit.XCallRequest("ZR\001\001", 1, 1)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('u', 1, min_age_in_years)
        l_request.add_parm('b', 2, add_to_subs)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def AddMaxAgeTerm(
        self,
        max_age_in_years,
        add_to_subs = False
        ):
        """
        construct CONTACT_BIRTHDAY term based on current time
        
        B{Arguments:}
         - B{max_age_in_years}
         - B{add_to_subs}
        
        B{Return values:}
         - B{valid}
        """
        l_request = skypekit.XCallRequest("ZR\001\002", 1, 2)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('u', 1, max_age_in_years)
        l_request.add_parm('b', 2, add_to_subs)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def AddEmailTerm(
        self,
        email,
        add_to_subs = False
        ):
        """
        Adds a search term against Contact::P_EMAILS property and pre-validates the value given in the email argument. 
        
        
        B{Arguments:}
         - B{email} - e-mail addres to search for. 
        
         - B{add_to_subs} - This argument enables you to group conditions. See ContactSearch class details for more information. 
        
        
        B{Return values:}
         - B{valid} - Returns false if the value in email property did not look like a valid email address. 
        
        """
        l_request = skypekit.XCallRequest("ZR\001\003", 1, 3)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, email)
        l_request.add_parm('b', 2, add_to_subs)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def AddLanguageTerm(
        self,
        language,
        add_to_subs = False
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{language}
         - B{add_to_subs}
        
        B{Return values:}
         - B{valid}
        """
        l_request = skypekit.XCallRequest("ZR\001\004", 1, 4)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, language)
        l_request.add_parm('b', 2, add_to_subs)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def AddStrTerm(
        self,
        prop,
        cond,
        value,
        add_to_subs = False
        ):
        """
        Adds a string search term to a custom contact search object. 
        
        
        B{Arguments:}
         - B{prop} - Following Contact class string propkeys can be used for Contact search: 
          - Contact::P_SKYPENAME 
          - Contact::P_FULLNAME 
          - Contact::P_LANGUAGES 
          - Contact::P_COUNTRY 
          - Contact::P_PROVINCE 
          - Contact::P_CITY 
          - Contact::P_PHONE_HOME 
          - Contact::P_PHONE_OFFICE 
          - Contact::P_PHONE_MOBILE 
          - Contact::P_EMAILS 
          - Contact::P_HOMEPAGE 
          - Contact::P_ABOUT 
        Note that while Contact::P_EMAILS is technically a string and can be used in this method, it is recommended that you use ContactSearch.AddEmailTerm method instead. 
        
         - B{cond} - Search condition (ContactSearch::CONDITION) 
        
         - B{value} - Value to match against. 
        
         - B{add_to_subs} - This argument enables you to group conditions. See ContactSearch class details for more information. 
        
        
        B{Return values:}
         - B{valid} - Returns true if the ContactSearch term-set remains valid after adding this term. 
        
        """
        l_request = skypekit.XCallRequest("ZR\001\005", 1, 5)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, prop)
        l_request.add_parm('e', 2, ContactSearch.CONDITION[cond])
        l_request.add_parm('S', 3, value)
        l_request.add_parm('b', 4, add_to_subs)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def AddIntTerm(
        self,
        prop,
        cond,
        value,
        add_to_subs = False
        ):
        """
        Adds a search term to a custom contact search object. For now, there are only two searchable Contact properties that are integers, so this can oly be used for Contact::P_BIRTHDAY and Contact::P_GENDER. 
        
        
        B{Arguments:}
         - B{prop} - Propkey to search for. Either Contact::P_BIRTHDAY or Contact::P_GENDER 
        
         - B{cond} - Search condition (ContactSearch::CONDITION) 
        
         - B{value} - Value to match against. 
        
         - B{add_to_subs} - This argument enables you to group conditions. See ContactSearch class details for more information. 
        
        
        B{Return values:}
         - B{valid} - Returns true if the ContactSearch term-set remains valid after adding this term. 
        
        """
        l_request = skypekit.XCallRequest("ZR\001\006", 1, 6)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, prop)
        l_request.add_parm('e', 2, ContactSearch.CONDITION[cond])
        l_request.add_parm('u', 3, value)
        l_request.add_parm('b', 4, add_to_subs)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def AddOr(self):
        """
        used to group terms (AddTerm(1), AddTerm(2), Or(), AddTerm(3), AddTerm(4), etc)
        """
        l_request = skypekit.XCallRequest("ZR\001\007", 1, 7)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def IsValid(self):
        """
        checks that terms list is non-empty and does not contain unsupported keys
        
        B{Return values:}
         - B{result}
        """
        l_request = skypekit.XCallRequest("ZR\001\010", 1, 8)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def Submit(self):
        """
        launch search
        """
        l_request = skypekit.XCallRequest("ZR\001\011", 1, 9)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def Extend(self):
        """
        extend if search is EXTENDABLE
        """
        l_request = skypekit.XCallRequest("ZR\001\012", 1, 10)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def Release(self):
        """
        releases results that are not referenced from elsewhere
        """
        l_request = skypekit.XCallRequest("ZR\001\014", 1, 12)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def GetResults(
        self,
        from_ = 0,
        count = skypekit.MAX_UINT
        ):
        """
        result list is dynamically updated
        
        B{Arguments:}
         - B{from_}
         - B{count}
        
        B{Return values:}
         - B{contacts}
        """
        l_request = skypekit.XCallRequest("ZR\001\013", 1, 11)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('u', 1, from_)
        l_request.add_parm('u', 2, count)
        l_response = self.transport.xcall(l_request)
        l_result = [module_id2classes[2](oid, self.transport) for oid in l_response.get(1, [])]
        return l_result
    def OnNewResult(
        self,
        contact,
        rank_value
        ):
        """
        This callback is fired when a new matching contact has been found during the search. 
        
        
        B{Return values:}
         - B{contact}
         - B{rank_value}
        """
        pass
    event_handlers[1] = "_sk_on_new_result"
    def _sk_on_new_result(self, parms):
        """ internal event dispatcher for OnNewResult
  """
        l_cleanparms = (module_id2classes[2](parms.get(1), self.transport)),
        l_cleanparms += (parms.get(2, 0)),
        self.OnNewResult(*l_cleanparms)
module_id2classes[1] = ContactSearch

class Participant(skypekit.Object):
    """
    Conversation participant class. Instances of this class represent contacts when in the context of conversations. Amongst others, this class has a Ring method for requesting live status with the target contact. This class also holds typing indicator property and access rights for the contact in context of public conversations. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "Participant %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(Participant, "P_"+propname.upper())
    module_id = 19
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    RANK = {1:'CREATOR', 'CREATOR':1, 2:'ADMIN', 'ADMIN':2, 3:'SPEAKER', 'SPEAKER':3, 4:'WRITER', 'WRITER':4, 5:'SPECTATOR', 'SPECTATOR':5, 6:'APPLICANT', 'APPLICANT':6, 7:'RETIRED', 'RETIRED':7, 8:'OUTLAW', 'OUTLAW':8}
    """
    Recognized values for the P_RANK property. The P_RANK controls participant's privileges in the conversation. See Participant::CanSetRankTo and Participant::SetRankTo methods. 
    
     - CREATOR - Creator of the chat. There can be only one participant with this type per conversation. Other participants cannot be promoted to Creator rank. 
    
     - ADMIN - Participant who has administrator privileges 
    
     - SPEAKER - Participant who can speak and write 
    
     - WRITER - Participant who can write but not speak 
    
     - SPECTATOR - Participant who can read but not write/speak 
    
     - APPLICANT - Participant who is applying to join the conversation. Member cannot be demoted to applicants once they have been accepted. 
    
     - RETIRED - Participant who has eft or has been kicked from the conversation 
    
     - OUTLAW - Participant who has been banned from the chat 
    
    """
    TEXT_STATUS = {0:'TEXT_UNKNOWN', 'TEXT_UNKNOWN':0, 1:'TEXT_NA', 'TEXT_NA':1, 2:'READING', 'READING':2, 3:'WRITING', 'WRITING':3, 4:'WRITING_AS_ANGRY', 'WRITING_AS_ANGRY':4, 5:'WRITING_AS_CAT', 'WRITING_AS_CAT':5}
    """
    Recognized values for the P_TEXT_STATUS property. The P_TEXT_STATUS property has two uses. Firstly, you can use it to implement typing indicators in your UI, to notify the local user that an incoming chat message from this Participant is imminent. 
    
    To set the P_TEXT_STATUS value, so that remote client UIs can display the local user's typing indicator in their UI, use Conversation::SetMyTextStatusTo method. 
    
    Transmission of P_TEXT_STATUS updates to remote participants of conversations is controlled via SETUPKEY_DISABLE_CHAT_ACTIVITY_INDICATION setup key. 
    
    Secondly, the TEXT_NA value enables you to detect participants who are running clients with no chat capability. 
    
     - TEXT_UNKNOWN - Fallback state in case the text status is not (yet) deternmined. 
    
     - TEXT_NA - Text status is not applicable as the participant is using a Skype client that does not support chat (for example: voice-only USB phones). 
    
     - READING - Participant is currently not typing. 
    
     - WRITING - Participant is currently typing. 
    
     - WRITING_AS_ANGRY - This state should be set when following two conditions are true: 
      - interval between keypresses are less than 20 ms; 
      - at least one of the keys adjacent to current key are pressed down. 
    
     - WRITING_AS_CAT - The "Cat on keyboard detection" algorthm in Skype is implemented in the UI level, and as such is not present in the SkypeKit API. Should you wish to implement similar algorthm in your own UI, you can get the basic logic from the PawSense FAQ - http://www.bitboost.com/pawsense/pawsense-faq.html 
    
    """
    VOICE_STATUS = {
        0:'VOICE_UNKNOWN',
        1:'VOICE_NA',
        2:'VOICE_AVAILABLE',
        3:'VOICE_CONNECTING',
        4:'RINGING',
        5:'EARLY_MEDIA',
        6:'LISTENING',
        7:'SPEAKING',
        8:'VOICE_ON_HOLD',
        9:'VOICE_STOPPED',
        'VOICE_UNKNOWN'
        
                
             :0,
        'VOICE_NA'
        
                
                  :1,
        'VOICE_AVAILABLE'
        
                
           :2,
        'VOICE_CONNECTING'
        
                
          :3,
        'RINGING'
        
                
                   :4,
        'EARLY_MEDIA'
        
                
               :5,
        'LISTENING'
        
                
                 :6,
        'SPEAKING'
        
                
                  :7,
        'VOICE_ON_HOLD'
        
                
             :8,
        'VOICE_STOPPED'
        
                
             :9
    }
    """
    Recognized values for the P_VOICE_STATUS property. 
    
     - VOICE_UNKNOWN - Participant voice status is not (yet) determined. 
    
     - VOICE_NA - Participant is using a Skype client with no audio capability. 
    
     - VOICE_AVAILABLE - Participant is using a Skype client that supports audio. 
    
     - VOICE_CONNECTING - Participant is in process of joining current live session. This is a transitional state. 
    
     - RINGING - Participant has been invited to join the live session but has not yet accepted. 
    
     - EARLY_MEDIA - Participant is in process of joining current live session. This is another transitional state. 
    
     - LISTENING - Participant has joined the current live session but is currently not transmitting audio. 
    
     - SPEAKING - Participant has joined the current live session and is transmitting audio. The UI can either use this state to display appropriate "speaking" notification, or alternatively use Participant::P_SOUND_LEVEL if you want your speaking indicator to also reflect audio volume coming from the Participant. 
    
     - VOICE_ON_HOLD - Participant has joined the current live session but the audio is currently on hold. 
    
     - VOICE_STOPPED - Participant will be placed in this state for some seconds after live session has finished. This is another transitional state. 
    
    """
    VIDEO_STATUS = {0:'VIDEO_UNKNOWN', 'VIDEO_UNKNOWN':0, 1:'VIDEO_NA', 'VIDEO_NA':1, 2:'VIDEO_AVAILABLE', 'VIDEO_AVAILABLE':2, 3:'VIDEO_CONNECTING', 'VIDEO_CONNECTING':3, 4:'STREAMING', 'STREAMING':4, 5:'VIDEO_ON_HOLD', 'VIDEO_ON_HOLD':5}
    """
    Recognized values for the P_VIDEO_STATUS property. This property applies to Participant's video send capability, not capability to receive video. 
    
     - VIDEO_UNKNOWN - Video status is not (yet) determined. 
    
     - VIDEO_NA - Indicates that this Participant does not have video available.. 
    
     - VIDEO_AVAILABLE - Indicates that video is available for this participant. When the Participant::P_VIDEO_STATUS obtains this state, it is possible to retrieve the Video object, using Participant::GetVideo method. Further operations, such as starting or stopping the video send/receive will then go through the Video object. 
    
     - VIDEO_CONNECTING - Transitional state indicating that the Participant is attempting to initiate video send. 
    
     - STREAMING - Indicates that the participant is currently sending video. 
    
     - VIDEO_ON_HOLD - Indicates that the participant video send is currently paused. 
    
    """
    DTMF = {
        0 :'DTMF_0',
        1 :'DTMF_1',
        2 :'DTMF_2',
        3 :'DTMF_3',
        4 :'DTMF_4',
        5 :'DTMF_5',
        6 :'DTMF_6',
        7 :'DTMF_7',
        8 :'DTMF_8',
        9 :'DTMF_9',
        10:'DTMF_STAR',
        11:'DTMF_POUND',
        'DTMF_0'
        
                
                    : 0,
        'DTMF_1'
        
                
                    : 1,
        'DTMF_2'
        
                
                    : 2,
        'DTMF_3'
        
                
                    : 3,
        'DTMF_4'
        
                
                    : 4,
        'DTMF_5'
        
                
                    : 5,
        'DTMF_6'
        
                
                    : 6,
        'DTMF_7'
        
                
                    : 7,
        'DTMF_8'
        
                
                    : 8,
        'DTMF_9'
        
                
                    : 9,
        'DTMF_STAR'
        
                
                 :10,
        'DTMF_POUND'
        
                
                :11
    }
    """
     - DTMF_0
     - DTMF_1
     - DTMF_2
     - DTMF_3
     - DTMF_4
     - DTMF_5
     - DTMF_6
     - DTMF_7
     - DTMF_8
     - DTMF_9
     - DTMF_STAR
     - DTMF_POUND
    """
    
    def _sk_get_convo_id(self):
        """
        [ALL] ID of corresponding conversation. Here and below, [ALL] tag indicates that the property has meaning and is set in context of all participants in the conversation. [OTHERS] tag has meaning only for participants who are not the local user. 
        
        """
        return module_id2classes[18](self._sk_property("ZG\242\007]\023",
  930, True), self.transport)
    convo_id = property(_sk_get_convo_id)
    propid2label[930] = "convo_id"
    P_CONVO_ID = 930
    def _sk_get_identity(self):
        """
        [ALL] skypename OR pstn_number OR namespace:identity
        """
        return self._sk_property("ZG\243\007]\023", 931, True)
    identity = property(_sk_get_identity)
    propid2label[931] = "identity"
    P_IDENTITY = 931
    def _sk_get_rank(self):
        """
        [ALL] Participant::RANK
        """
        return skypekit.enumof(Participant.RANK, self._sk_property("ZG\244\007]\023", 932, True))
    rank = property(_sk_get_rank)
    propid2label[932] = "rank"
    P_RANK = 932
    def _sk_get_requested_rank(self):
        """
        Not set (should be: requested Participant::RANK, higher than the current one)
        """
        return skypekit.enumof(Participant.RANK, self._sk_property("ZG\245\007]\023", 933, True))
    requested_rank = property(_sk_get_requested_rank)
    propid2label[933] = "requested_rank"
    P_REQUESTED_RANK = 933
    def _sk_get_text_status(self):
        """
        [ALL] the typing indicator
        """
        return skypekit.enumof(Participant.TEXT_STATUS, self._sk_property("ZG\246\007]\023", 934, True))
    text_status = property(_sk_get_text_status)
    propid2label[934] = "text_status"
    P_TEXT_STATUS = 934
    def _sk_get_voice_status(self):
        """
        [ALL] voice status
        """
        return skypekit.enumof(Participant.VOICE_STATUS, self._sk_property("ZG\247\007]\023", 935, True))
    voice_status = property(_sk_get_voice_status)
    propid2label[935] = "voice_status"
    P_VOICE_STATUS = 935
    def _sk_get_video_status(self):
        """
        [ALL] video status
        """
        return skypekit.enumof(Participant.VIDEO_STATUS, self._sk_property("ZG\250\007]\023", 936, True))
    video_status = property(_sk_get_video_status)
    propid2label[936] = "video_status"
    P_VIDEO_STATUS = 936
    def _sk_get_live_identity(self):
        """
        [ALL] identity that was used to establish current live session with that participant (can be different from participant identity)
        """
        return self._sk_property("ZG\257\007]\023", 943, False)
    live_identity = property(_sk_get_live_identity)
    propid2label[943] = "live_identity"
    P_LIVE_IDENTITY = 943
    def _sk_get_live_price_for_me(self):
        """
        [OTHERS] 'price_per_minute_float currency' - eg '0.01 EUR'. Note that this property obtains value only after the participant goes into live state. It cannot be used to display call rates before the call starts. 
        
        """
        return self._sk_property("ZG\252\007]\023", 938, True)
    live_price_for_me = property(_sk_get_live_price_for_me)
    propid2label[938] = "live_price_for_me"
    P_LIVE_PRICE_FOR_ME = 938
    def _sk_get_live_fwd_identities(self):
        """
        [OTHERS] list of identities where the live session is being forwarded (if they are disclosed), space separated
        """
        return self._sk_property("ZG\264\007]\023", 948, True)
    live_fwd_identities = property(_sk_get_live_fwd_identities)
    propid2label[948] = "live_fwd_identities"
    P_LIVE_FWD_IDENTITIES = 948
    def _sk_get_live_start_timestamp(self):
        """
        [ALL] time of joining the live session
        """
        return self._sk_property("ZG\253\007]\023", 939, True)
    live_start_timestamp = property(_sk_get_live_start_timestamp)
    propid2label[939] = "live_start_timestamp"
    P_LIVE_START_TIMESTAMP = 939
    def _sk_get_sound_level(self):
        """
        [ALL] current 'loudness' level when SPEAKING (0..10)
        """
        return self._sk_property("ZG\255\007]\023", 941, True)
    sound_level = property(_sk_get_sound_level)
    propid2label[941] = "sound_level"
    P_SOUND_LEVEL = 941
    def _sk_get_debuginfo(self):
        """
        [OTHERS] call (audio and video) debug info
        """
        return self._sk_property("ZG\256\007]\023", 942, False)
    debuginfo = property(_sk_get_debuginfo)
    propid2label[942] = "debuginfo"
    P_DEBUGINFO = 942
    def _sk_get_last_voice_error(self):
        """
        [OTHERS] DEPRECATED, use last_leavereason instead
        """
        return self._sk_property("ZG\263\007]\023", 947, True)
    last_voice_error = property(_sk_get_last_voice_error)
    propid2label[947] = "last_voice_error"
    P_LAST_VOICE_ERROR = 947
    def _sk_get_quality_problems(self):
        """
        [ALL] space separated tokens values: CPU_INUSE CPU_SLOW CPU_HIGH HIGH_ECHO HIGH_NOISE MUTED_INPUT LOW_INPUT MUTED_INPUT_ACTIVITY FW_STRONG FW_BAD NOT_UDP CALL_BW_LOW RECORD_ERROR + values in video debug info
        """
        return self._sk_property("ZG\265\007]\023", 949, True)
    quality_problems = property(_sk_get_quality_problems)
    propid2label[949] = "quality_problems"
    P_QUALITY_PROBLEMS = 949
    def _sk_get_live_type(self):
        """
        [ALL] participant type during livesession as specified in IDENTITYTYPE
        """
        return skypekit.enumof(Skype.IDENTITYTYPE, self._sk_property("ZG\266\007]\023", 950, True))
    live_type = property(_sk_get_live_type)
    propid2label[950] = "live_type"
    P_LIVE_TYPE = 950
    def _sk_get_live_country(self):
        """
        [OTHERS] participant livesession country code - used for emergency calls only atm
        """
        return self._sk_property("ZG\267\007]\023", 951, False)
    live_country = property(_sk_get_live_country)
    propid2label[951] = "live_country"
    P_LIVE_COUNTRY = 951
    def _sk_get_transferred_by(self):
        """
        [OTHERS] Transferor identity (transferee side) 
        
        """
        return self._sk_property("ZG\270\007]\023", 952, True)
    transferred_by = property(_sk_get_transferred_by)
    propid2label[952] = "transferred_by"
    P_TRANSFERRED_BY = 952
    def _sk_get_transferred_to(self):
        """
        [OTHERS] Identity of recipient of transfer (transferor side, caller side) 
        
        """
        return self._sk_property("ZG\271\007]\023", 953, True)
    transferred_to = property(_sk_get_transferred_to)
    propid2label[953] = "transferred_to"
    P_TRANSFERRED_TO = 953
    def _sk_get_adder(self):
        """
        [ALL] Identity of the user who added this participant to the conversation, type: Sid::String 
        
        """
        return self._sk_property("ZG\272\007]\023", 954, True)
    adder = property(_sk_get_adder)
    propid2label[954] = "adder"
    P_ADDER = 954
    def _sk_get_last_leavereason(self):
        """
        [OTHERS] Last reason for leaving conversation or live session. Provides an enum alternative to last_voice_error as well as the reason this participant RETIRED from the conversation. 
        
        """
        return skypekit.enumof(Skype.LEAVE_REASON, self._sk_property("ZG\273\007]\023", 955, True))
    last_leavereason = property(_sk_get_last_leavereason)
    propid2label[955] = "last_leavereason"
    P_LAST_LEAVEREASON = 955
    
    def CanSetRankTo(
        self,
        rank
        ):
        """
        Checks whether the current user can set this Participant's conversation privileges to the specified RANK. This enables you to gray out or disable in your UI all the unavailable options for Participant::SetRankTo method. 
        
        
        B{Arguments:}
         - B{rank} - Participant::RANK value to check for. 
        
        
        B{Return values:}
         - B{result} - Returns true if local user can set participant's rank to the value given in rank argument. 
        
        """
        l_request = skypekit.XCallRequest("ZR\023\001", 19, 1)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Participant.RANK[rank])
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def SetRankTo(
        self,
        rank
        ):
        """
        Sets Participant's conversation privileges to the given RANK 
        
        
        B{Arguments:}
         - B{rank} - Target Participant::RANK value. 
        
        """
        l_request = skypekit.XCallRequest("ZR\023\002", 19, 2)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Participant.RANK[rank])
        self.transport.xcall(l_request)
    def Ring(
        self,
        identity_to_use = "",
        video_call = False,
        nrof_redials = 0,
        redial_period = 0,
        auto_start_vm = False,
        origin = ""
        ):
        """
        Initiates live conversation attempt with Participant. 
        
        
        B{Arguments:}
         - B{identity_to_use} - Ring an alternate identity, such as a PSTN number. 
        
         - B{video_call} - Enable video. 
        
         - B{nrof_redials} - Unused. 
        
         - B{redial_period} - Unused. 
        
         - B{auto_start_vm} - Unused. On dialog, if falling on VM, greeting and recording will be automatically started. 
        
         - B{origin} - When call is initiated from web link, must contain the URI that was used
        """
        l_request = skypekit.XCallRequest("ZR\023\003", 19, 3)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, identity_to_use)
        l_request.add_parm('b', 2, video_call)
        l_request.add_parm('u', 3, nrof_redials)
        l_request.add_parm('u', 4, redial_period)
        l_request.add_parm('b', 5, auto_start_vm)
        l_request.add_parm('S', 6, origin)
        self.transport.xcall(l_request)
    def RingIt(self):
        """
        Rings this participant, using P_LIVE_IDENTITY property if set. 
        
        """
        l_request = skypekit.XCallRequest("ZR\023\010", 19, 8)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def SetLiveIdentityToUse(
        self,
        identity_to_use = ""
        ):
        """
        Sets LIVE_IDENTITY property, an alternate identity to use when ringing, such as a PSTN. 
        
        
        B{Arguments:}
         - B{identity_to_use} - Empty string will reset it to default, i.e IDENTITY property value 
        
        """
        l_request = skypekit.XCallRequest("ZR\023\007", 19, 7)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, identity_to_use)
        self.transport.xcall(l_request)
    def GetVideo(self):
        """
        Retrieves a reference to the Video object that corresponds to the Participant. It can be either local video - you can check if this participant's name (P_IDENTITY property) matches the name of the currently logged in account (P_SKYPENAME property) or incoming video from a remote participant. 
        
        Note that for GetVideo to be successful, the video has to be available for that participant. This can be checked for by examining Participant VIDEO_STATUS property - once it becomes VIDEO_AVAILABLE - you can use GetVideo to obtain the Video object. 
        
        
        B{Return values:}
         - B{video} - Returns reference to a constructed video object. 
        
        """
        l_request = skypekit.XCallRequest("ZR\023\004", 19, 4)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[11](l_response.get(1), self.transport)
        return l_result
    def Hangup(self):
        """
        Removes this participant from the current live session. Note that this does not remove the participant from conversation (for this, use Participant::Retire). It only removes participant from live state. 
        
        """
        l_request = skypekit.XCallRequest("ZR\023\005", 19, 5)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def Retire(self):
        """
        Forcibly removes this participant from conversation. This method is for removing other people from conversations (for example: as administrative punishment for flooding conversation with spam messages). For local user to leave a conversation, use Conversation::RetireFrom instead. 
        
        """
        l_request = skypekit.XCallRequest("ZR\023\006", 19, 6)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def OnIncomingDTMF(
        self,
        dtmf
        ):
        """
        This event gets fired on receiving a DTMF signal sent by Participant. Note that this event will only fire if the Participant is also using a Skype client. Skype audio library does not monitor incoming voice streams for dial tones. DTMF events are propagated to remote participants via data channel. Incoming DTMF tones transmitted from, for example, mobile phones, will not cause this event to fire. In case of incoming DTMF signals from Skype clients, DTMF tones are also inserted into the audio stream. You don't have to inject those into local audio playback yourself. 
        
        
        B{Return values:}
         - B{dtmf} - Returns Participant::DTMF value. 
        
        """
        pass
    event_handlers[1] = "_sk_on_incoming_dtmf"
    def _sk_on_incoming_dtmf(self, parms):
        """ internal event dispatcher for OnIncomingDTMF
  """
        l_cleanparms = Participant.DTMF[parms.get(1)]
        self.OnIncomingDTMF(l_cleanparms)
module_id2classes[19] = Participant

class Conversation(skypekit.Object):
    """
    The Conversation class encapsulates all types of communication possible with Skype client. Instant messaging, calls, video calls, file transfers, SMS, screen sharing - all take place within the context of a Conversation. Contacts are represented in Conversation as Participant objects. This also applies to contacts of PSTN type. All events in a conversation are represented as Message objects. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "Conversation %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(Conversation, "P_"+propname.upper())
    module_id = 18
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    TYPE = {1:'DIALOG', 'DIALOG':1, 2:'CONFERENCE', 'CONFERENCE':2, 3:'TERMINATED_CONFERENCE', 'TERMINATED_CONFERENCE':3, 4:'LEGACY_VOICE_CONFERENCE', 'LEGACY_VOICE_CONFERENCE':4, 5:'LEGACY_SHAREDGROUP', 'LEGACY_SHAREDGROUP':5}
    """
     - DIALOG - 1:1 conversations, there is a one dialog per identity
     - CONFERENCE - equivalent of a multichat
     - TERMINATED_CONFERENCE - a conference that has been terminated (disbanded chat)
     - LEGACY_VOICE_CONFERENCE - voice-only conference, when host is using a legacy non-conversation client
     - LEGACY_SHAREDGROUP - chat used for legacy shared groups, can be ignored
    """
    MY_STATUS = {
        1 :'CONNECTING',
        2 :'RETRY_CONNECTING',
        3 :'DOWNLOADING_MESSAGES',
        4 :'QUEUED_TO_ENTER',
        5 :'APPLICANT',
        6 :'APPLICATION_DENIED',
        7 :'INVALID_ACCESS_TOKEN',
        8 :'CONSUMER',
        9 :'RETIRED_FORCEFULLY',
        10:'RETIRED_VOLUNTARILY',
        'CONNECTING'
        
                
                : 1,
        'RETRY_CONNECTING'
        
                
          : 2,
        'DOWNLOADING_MESSAGES'
        
                      : 3,
        'QUEUED_TO_ENTER'
        
                
           : 4,
        'APPLICANT'
        
                
                 : 5,
        'APPLICATION_DENIED'
        
                
        : 6,
        'INVALID_ACCESS_TOKEN'
        
                      : 7,
        'CONSUMER'
        
                
                  : 8,
        'RETIRED_FORCEFULLY'
        
                
        : 9,
        'RETIRED_VOLUNTARILY'
        
                       :10
    }
    """
     - CONNECTING - connecting to conference
     - RETRY_CONNECTING
     - DOWNLOADING_MESSAGES - unused
     - QUEUED_TO_ENTER - conference is full for now, being queued
     - APPLICANT - I'm applying to join the conference
     - APPLICATION_DENIED - My application to join the conference was denied
     - INVALID_ACCESS_TOKEN - The password I provided is incorrect
     - CONSUMER - I'm part of the conference, I can participate
     - RETIRED_FORCEFULLY - I was kicked from the conference
     - RETIRED_VOLUNTARILY - I left the conference
    """
    LOCAL_LIVESTATUS = {
        0 :'NONE',
        1 :'STARTING',
        2 :'RINGING_FOR_ME',
        3 :'IM_LIVE',
        5 :'ON_HOLD_LOCALLY',
        6 :'ON_HOLD_REMOTELY',
        7 :'OTHERS_ARE_LIVE',
        11:'OTHERS_ARE_LIVE_FULL',
        8 :'PLAYING_VOICE_MESSAGE',
        9 :'RECORDING_VOICE_MESSAGE',
        10:'RECENTLY_LIVE',
        12:'TRANSFERRING',
        'NONE'
        
                
                      : 0,
        'STARTING'
        
                
                  : 1,
        'RINGING_FOR_ME'
        
                
            : 2,
        'IM_LIVE'
        
                
                   : 3,
        'ON_HOLD_LOCALLY'
        
                
           : 5,
        'ON_HOLD_REMOTELY'
        
                
          : 6,
        'OTHERS_ARE_LIVE'
        
                
           : 7,
        'OTHERS_ARE_LIVE_FULL'
        
                      :11,
        'PLAYING_VOICE_MESSAGE'
        
                     : 8,
        'RECORDING_VOICE_MESSAGE'
        
                   : 9,
        'RECENTLY_LIVE'
        
                
             :10,
        'TRANSFERRING'
        
                
              :12
    }
    """
     - NONE - there isn't a live session
     - STARTING - trying to start or join a live session
     - RINGING_FOR_ME - there is a live session ringing
     - IM_LIVE - the conference is live for me
     - ON_HOLD_LOCALLY - I put the live session on hold
     - ON_HOLD_REMOTELY - the live session was put on hold by someone else
     - OTHERS_ARE_LIVE - there is a live session on-going, I'm not participating but I could join
     - OTHERS_ARE_LIVE_FULL - there is a live session on-going without me, but I can't join because it's full
     - PLAYING_VOICE_MESSAGE - playing a voicemail (dialog only)
     - RECORDING_VOICE_MESSAGE - recording a voicemail (dialog only)
     - RECENTLY_LIVE - a live session just finished, we stay in this state for RECENTLY_LIVE_TIMEOUT setup key
     - TRANSFERRING - call is being transferred
    """
    ALLOWED_ACTIVITY = {1:'SET_META', 'SET_META':1, 2:'ADD_CONSUMERS', 'ADD_CONSUMERS':2, 4:'SPEAK', 'SPEAK':4, 8:'SPEAK_AND_WRITE', 'SPEAK_AND_WRITE':8}
    """
    values for opt_admin_only_activities property
     - SET_META - allowed to set the CONVERSATION_META properties
     - ADD_CONSUMERS - allowed to add participants to the conference
     - SPEAK - allowed to speak, but not write
     - SPEAK_AND_WRITE - allowed to speak and write
    """
    PARTICIPANTFILTER = {0:'ALL', 'ALL':0, 1:'CONSUMERS', 'CONSUMERS':1, 2:'APPLICANTS', 'APPLICANTS':2, 3:'CONSUMERS_AND_APPLICANTS', 'CONSUMERS_AND_APPLICANTS':3, 4:'MYSELF', 'MYSELF':4, 5:'OTHER_CONSUMERS', 'OTHER_CONSUMERS':5}
    """
     - ALL - All participants (may included some that are RETIRED or OUTLAW, but not all of them)
     - CONSUMERS - Participants that can receive messages, including myself
     - APPLICANTS - Only people who are applying to join the conversation
     - CONSUMERS_AND_APPLICANTS - Consumers and applicants
     - MYSELF - Myself
     - OTHER_CONSUMERS - All consumers except myself
    """
    LIST_TYPE = {0:'ALL_CONVERSATIONS', 'ALL_CONVERSATIONS':0, 1:'INBOX_CONVERSATIONS', 'INBOX_CONVERSATIONS':1, 2:'BOOKMARKED_CONVERSATIONS', 'BOOKMARKED_CONVERSATIONS':2, 3:'LIVE_CONVERSATIONS', 'LIVE_CONVERSATIONS':3, 5:'REALLY_ALL_CONVERSATIONS', 'REALLY_ALL_CONVERSATIONS':5}
    """
     - ALL_CONVERSATIONS - bookmarked or in_inbox or live or with_meta_info or activity in last 30 days
     - INBOX_CONVERSATIONS - only last 6 months conversations are kept there
     - BOOKMARKED_CONVERSATIONS - is_bookmarked is set
     - LIVE_CONVERSATIONS - local_livestatus is different from NONE
     - REALLY_ALL_CONVERSATIONS - all conversations, without any of the limits of ALL_CONVERSATIONS
    """
    
    def _sk_get_identity(self):
        """
        contact identity in case of dialogs, chat name in case of conferences
        """
        return self._sk_property("ZG\314\007]\022", 972, True)
    identity = property(_sk_get_identity)
    propid2label[972] = "identity"
    P_IDENTITY = 972
    def _sk_get_type(self):
        """
        type of the conversation
        """
        return skypekit.enumof(Conversation.TYPE, self._sk_property("ZG\206\007]\022", 902, True))
    type = property(_sk_get_type)
    propid2label[902] = "type"
    P_TYPE = 902
    def _sk_get_live_host(self):
        """
        host of current live session. none => no session. myself in case of 1:1 calls
        """
        return self._sk_property("ZG\226\007]\022", 918, True)
    live_host = property(_sk_get_live_host)
    propid2label[918] = "live_host"
    P_LIVE_HOST = 918
    def _sk_get_live_start_timestamp(self):
        """
        moment when first participant other than host joined the current or last live session
        """
        return self._sk_property("ZG\316\007]\022", 974, True)
    live_start_timestamp = property(_sk_get_live_start_timestamp)
    propid2label[974] = "live_start_timestamp"
    P_LIVE_START_TIMESTAMP = 974
    def _sk_get_live_is_muted(self):
        """
        if live session is muted
        """
        return self._sk_property("ZG\344\007]\022", 996, True)
    live_is_muted = property(_sk_get_live_is_muted)
    propid2label[996] = "live_is_muted"
    P_LIVE_IS_MUTED = 996
    def _sk_get_alert_string(self):
        """
        '' everything matches, '=' nothing matches, '=string' string matches
        """
        return self._sk_property("ZG\230\007]\022", 920, True)
    alert_string = property(_sk_get_alert_string)
    propid2label[920] = "alert_string"
    P_ALERT_STRING = 920
    def _sk_get_is_bookmarked(self):
        """
        if conversation is bookmarked/flagged
        """
        return self._sk_property("ZG\231\007]\022", 921, True)
    is_bookmarked = property(_sk_get_is_bookmarked)
    propid2label[921] = "is_bookmarked"
    P_IS_BOOKMARKED = 921
    def _sk_get_given_displayname(self):
        """
        name assigned via GiveDisplayname
        """
        return self._sk_property("ZG\235\007]\022", 925, True)
    given_displayname = property(_sk_get_given_displayname)
    propid2label[925] = "given_displayname"
    P_GIVEN_DISPLAYNAME = 925
    def _sk_get_displayname(self):
        """
        resulting display name of the conversation (based on given name, topic, participant list, etc)
        """
        return self._sk_property("ZG\234\007]\022", 924, True)
    displayname = property(_sk_get_displayname)
    propid2label[924] = "displayname"
    P_DISPLAYNAME = 924
    def _sk_get_local_livestatus(self):
        """
        if the conversation is live and in which status it is then
        """
        return skypekit.enumof(Conversation.LOCAL_LIVESTATUS, self._sk_property("ZG\237\007]\022", 927, True))
    local_livestatus = property(_sk_get_local_livestatus)
    propid2label[927] = "local_livestatus"
    P_LOCAL_LIVESTATUS = 927
    def _sk_get_inbox_timestamp(self):
        """
        timestamp to sort the conversations in inbox by. 0 means not in inbox
        """
        return self._sk_property("ZG\240\007]\022", 928, True)
    inbox_timestamp = property(_sk_get_inbox_timestamp)
    propid2label[928] = "inbox_timestamp"
    P_INBOX_TIMESTAMP = 928
    def _sk_get_inbox_message_id(self):
        """
        ID of the message that caused INBOX_TIMESTAMP to be set
        """
        return module_id2classes[9](self._sk_property("ZG\315\007]\022",
  973, True), self.transport)
    inbox_message_id = property(_sk_get_inbox_message_id)
    propid2label[973] = "inbox_message_id"
    P_INBOX_MESSAGE_ID = 973
    def _sk_get_unconsumed_suppressed_messages(self):
        """
        number of messages in UNCONSUMED_SUPPRESSED consumption status
        """
        return self._sk_property("ZG\317\007]\022", 975, True)
    unconsumed_suppressed_messages = property(_sk_get_unconsumed_suppressed_messages)
    propid2label[975] = "unconsumed_suppressed_messages"
    P_UNCONSUMED_SUPPRESSED_MESSAGES = 975
    def _sk_get_unconsumed_normal_messages(self):
        """
        number of messages in UNCONSUMED_NORMAL consumption status
        """
        return self._sk_property("ZG\320\007]\022", 976, True)
    unconsumed_normal_messages = property(_sk_get_unconsumed_normal_messages)
    propid2label[976] = "unconsumed_normal_messages"
    P_UNCONSUMED_NORMAL_MESSAGES = 976
    def _sk_get_unconsumed_elevated_messages(self):
        """
        DEPRECATED, not set anymore
        """
        return self._sk_property("ZG\321\007]\022", 977, True)
    unconsumed_elevated_messages = property(_sk_get_unconsumed_elevated_messages)
    propid2label[977] = "unconsumed_elevated_messages"
    P_UNCONSUMED_ELEVATED_MESSAGES = 977
    def _sk_get_unconsumed_messages_voice(self):
        """
        if there are unconsumed voice or call messages in the conversation
        """
        return self._sk_property("ZG\312\007]\022", 970, True)
    unconsumed_messages_voice = property(_sk_get_unconsumed_messages_voice)
    propid2label[970] = "unconsumed_messages_voice"
    P_UNCONSUMED_MESSAGES_VOICE = 970
    def _sk_get_active_vm_id(self):
        """
        ID of voice message that is being played or recorded in this conversation
        """
        return module_id2classes[7](self._sk_property("ZG\313\007]\022",
  971, True), self.transport)
    active_vm_id = property(_sk_get_active_vm_id)
    propid2label[971] = "active_vm_id"
    P_ACTIVE_VM_ID = 971
    def _sk_get_consumption_horizon(self):
        """
        consumption cutoff timestamp: messages after (but not including) this are considered unconsumed
        """
        return self._sk_property("ZG\323\007]\022", 979, True)
    consumption_horizon = property(_sk_get_consumption_horizon)
    propid2label[979] = "consumption_horizon"
    P_CONSUMPTION_HORIZON = 979
    def _sk_get_last_activity_timestamp(self):
        """
        timestamp of last activity in conversation
        """
        return self._sk_property("ZG\325\007]\022", 981, True)
    last_activity_timestamp = property(_sk_get_last_activity_timestamp)
    propid2label[981] = "last_activity_timestamp"
    P_LAST_ACTIVITY_TIMESTAMP = 981
    def _sk_get_spawned_from_convo_id(self):
        """
        dialog this conference was spawned from
        """
        return module_id2classes[18](self._sk_property("ZG\223\007]\022",
  915, True), self.transport)
    spawned_from_convo_id = property(_sk_get_spawned_from_convo_id)
    propid2label[915] = "spawned_from_convo_id"
    P_SPAWNED_FROM_CONVO_ID = 915
    def _sk_get_creator(self):
        """
        identity of conversation creator (doesn't apply to dialogs)
        """
        return self._sk_property("ZG\207\007]\022", 903, True)
    creator = property(_sk_get_creator)
    propid2label[903] = "creator"
    P_CREATOR = 903
    def _sk_get_creation_timestamp(self):
        """
        timestamp of creation, tells you how far you can retrieve messages
        """
        return self._sk_property("ZG\210\007]\022", 904, True)
    creation_timestamp = property(_sk_get_creation_timestamp)
    propid2label[904] = "creation_timestamp"
    P_CREATION_TIMESTAMP = 904
    def _sk_get_my_status(self):
        """
        my status in this conversation (connecting, participating, retired, etc) (doesn't apply to dialogs)
        """
        return skypekit.enumof(Conversation.MY_STATUS, self._sk_property("ZG\227\007]\022", 919, True))
    my_status = property(_sk_get_my_status)
    propid2label[919] = "my_status"
    P_MY_STATUS = 919
    def _sk_get_opt_joining_enabled(self):
        """
        if it's a public conversation (doesn't apply to dialogs)
        """
        return self._sk_property("ZG\232\007]\022", 922, True)
    opt_joining_enabled = property(_sk_get_opt_joining_enabled)
    propid2label[922] = "opt_joining_enabled"
    P_OPT_JOINING_ENABLED = 922
    def _sk_get_opt_entry_level_rank(self):
        """
        rank that is auto-assigned at join (doesn't apply to dialogs)
        """
        return skypekit.enumof(Participant.RANK, self._sk_property("ZG\212\007]\022", 906, True))
    opt_entry_level_rank = property(_sk_get_opt_entry_level_rank)
    propid2label[906] = "opt_entry_level_rank"
    P_OPT_ENTRY_LEVEL_RANK = 906
    def _sk_get_opt_disclose_history(self):
        """
        if history visible to new consumers (doesn't apply to dialogs)
        """
        return self._sk_property("ZG\213\007]\022", 907, True)
    opt_disclose_history = property(_sk_get_opt_disclose_history)
    propid2label[907] = "opt_disclose_history"
    P_OPT_DISCLOSE_HISTORY = 907
    def _sk_get_opt_admin_only_activities(self):
        """
        activities that only ADMIN can do. Bitmap of ALLOWED_ACTIVITY values (doesn't apply to dialogs)
        """
        return self._sk_property("ZG\215\007]\022", 909, True)
    opt_admin_only_activities = property(_sk_get_opt_admin_only_activities)
    propid2label[909] = "opt_admin_only_activities"
    P_OPT_ADMIN_ONLY_ACTIVITIES = 909
    def _sk_get_passwordhint(self):
        """
        public conversation password hint, use SetPassword to set (doesn't apply to dialogs)
        """
        return self._sk_property("ZG\324\007]\022", 980, True)
    passwordhint = property(_sk_get_passwordhint)
    propid2label[980] = "passwordhint"
    P_PASSWORDHINT = 980
    def _sk_get_meta_name(self):
        """
        user-set name, separate from topic (doesn't apply to dialogs)
        """
        return self._sk_property("ZG\216\007]\022", 910, True)
    meta_name = property(_sk_get_meta_name)
    propid2label[910] = "meta_name"
    P_META_NAME = 910
    def _sk_get_meta_topic(self):
        """
        conversation topic (doesn't apply to dialogs)
        """
        return self._sk_property("ZG\217\007]\022", 911, True)
    meta_topic = property(_sk_get_meta_topic)
    propid2label[911] = "meta_topic"
    P_META_TOPIC = 911
    def _sk_get_meta_guidelines(self):
        """
        guidelines (doesn't apply to dialogs)
        """
        return self._sk_property("ZG\221\007]\022", 913, True)
    meta_guidelines = property(_sk_get_meta_guidelines)
    propid2label[913] = "meta_guidelines"
    P_META_GUIDELINES = 913
    def _sk_get_meta_picture(self):
        """
        conversation picture, in jpeg format (doesn't apply to dialogs)
        """
        return self._sk_property("ZG\222\007]\022", 914, True)
    meta_picture = property(_sk_get_meta_picture)
    propid2label[914] = "meta_picture"
    P_META_PICTURE = 914
    
    SETUPKEY_ENABLE_BIRTHDAY_NOTIFICATION = "Lib/Conversation/EnableBirthday"
    SETUPKEY_INBOX_UPDATE_TIMEOUT = "Lib/Conversation/InboxUpdateTimeout"
    SETUPKEY_RECENTLY_LIVE_TIMEOUT = "Lib/Conversation/RecentlyLiveTimeout"
    SETUPKEY_DISABLE_CHAT = "Lib/Chat/DisableChat"
    SETUPKEY_DISABLE_CHAT_HISTORY = "Lib/Message/DisableHistory"
    SETUPKEY_CHAT_HISTORY_DAYS = "Lib/Chat/HistoryDays"
    SETUPKEY_CHATDB_LIMIT_KB = "Lib/Chat/ChatDBLimitKb"
    SETUPKEY_DISABLE_CHAT_ACTIVITY_INDICATION = "Lib/Chat/DisableActivityIndication"
    SETUPKEY_CALL_NOANSWER_TIMEOUT = "Lib/Call/NoAnswerTimeout"
    SETUPKEY_CALL_SEND_TO_VM = "Lib/Call/SendToVM"
    SETUPKEY_CALL_APPLY_CF = "Lib/Call/ApplyCF"
    SETUPKEY_CALL_EMERGENCY_COUNTRY = "Lib/Call/EmergencyCountry"
    def SetOption(
        self,
        prop_key,
        value
        ):
        """
        Setter method for Conversation option properties. Option properties are all Conversation properties starting with OPT_ prefix. 
        
        
        B{Arguments:}
         - B{prop_key} - Conversation property key, for example: Conversation::OPT_JOINING_ENABLED 
        
         - B{value} - New value for the option property. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\001", 18, 1)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, prop_key)
        l_request.add_parm('u', 2, value)
        self.transport.xcall(l_request)
    def SetTopic(
        self,
        topic,
        is_xml = False
        ):
        """
        Setter for Conversation class META_TOPIC. This topic will be set for remote participants as well. 
        
        
        B{Arguments:}
         - B{topic} - New conversation topic. 
        
         - B{is_xml} - Notifies remote UIs that the new topic contains xml tags. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\002", 18, 2)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, topic)
        l_request.add_parm('b', 2, is_xml)
        self.transport.xcall(l_request)
    def SetGuidelines(
        self,
        guidelines,
        is_xml = False
        ):
        """
        Setter for Conversation META_GUIDELINES. This property will be visible to remote participants of the conversation. 
        
        
        B{Arguments:}
         - B{guidelines} - New value for the META_GUIDELINES property. 
        
         - B{is_xml} - Set true to notify remote UIs that the new guideline contains XML tags. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\003", 18, 3)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, guidelines)
        l_request.add_parm('b', 2, is_xml)
        self.transport.xcall(l_request)
    def SetPicture(
        self,
        jpeg
        ):
        """
        Sets the conversation's avatar to the specified JPEG image, which is propagated to both local and remote participants. Before calling this method, you should use Skype::ValidateAvatar to verify that jpeg references a valid JPEG image. 
        
        
        B{Arguments:}
         - B{jpeg} - Conversation avatar binary. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\004", 18, 4)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('B', 1, jpeg)
        self.transport.xcall(l_request)
    def SpawnConference(
        self,
        identities_to_add
        ):
        """
        When called from dialog conversation, this spawns a new conversation, with existing two dialog participants plus new contact identities given in the identitiesToAdd list. You do not need to add existing dialog participants to the string list. In fact, passing only the existing participants in the identities list will cause the method call to fail (return false), the same as if the list was empty. This method will also return false if the original conversation was not a dialog (contained more than two participants). Also note that this method always creates a new Conversation - even if a conversation with exactly the same participant list existed before. 
        
        
        B{Arguments:}
         - B{identities_to_add} - String list of additional participant identities. You do not need to add existing two participants from the original dialog to this list. 
        
        
        B{Return values:}
         - B{conference} - Returns the resulting conversation or 0 if the method call failed. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\006", 18, 6)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, identities_to_add)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[18](l_response.get(1), self.transport)
        return l_result
    def AddConsumers(
        self,
        identities
        ):
        """
        Takes one or more Contact identities and creates corresponding Participant objects within the context of this Conversation, which must be of type CONFERENCE. If you have an existing dialog conversation, use SpawnConference instead. 
        
        
        B{Arguments:}
         - B{identities} - Contact identities to be added to the Conversation. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\007", 18, 7)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, identities)
        self.transport.xcall(l_request)
    def Assimilate(
        self,
        other_conversation
        ):
        """
        Merges two live conversations. For example, if the user already has a live conversation up - let's call it conversation A. Then a new incoming call occurs - another conversation obtains LOCAL_LIVESTATUS == Conversation::RINGING_FOR_ME, let's call it conversation B. The user wishes to pick up the new incoming call and add it to the existing one. For this you can first call B->JoinLiveSession and then merge two calls with A->Assimilate(B, A). The second argument will return the merged conversation. Note that there are actually three conversation objects involved: A (before merge), B and C (after the merge). Normally it would make sense to have the first conversation (A) as the second argument, so that it gets overwritten with the assimilation result. 
        
        
        B{Arguments:}
         - B{other_conversation} - The new conversation to be merged with the one already in live state. 
        
        
        B{Return values:}
         - B{conversation} - Returns a 3rd live conversation, result of merging two existing ones. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\011", 18, 9)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, other_conversation)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[18](l_response.get(1), self.transport)
        return l_result
    def JoinLiveSession(
        self,
        access_token = ""
        ):
        """
        starts, answers or joins a live session (first one to join becomes LIVE_HOST)
        
        B{Arguments:}
         - B{access_token} - if starting a live session, allows to set a custom access token
        """
        l_request = skypekit.XCallRequest("ZR\022\012", 18, 10)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, access_token)
        self.transport.xcall(l_request)
    def RingOthers(
        self,
        identities = [],
        video_call = False,
        origin = ""
        ):
        """
        This is an alternative to calling Ring method for each Participant individually. This also works with dialogs (with identities containing only one item). 
        
        
        B{Arguments:}
         - B{identities} - List of Participants to ring. Leaving the list empty will result in ringing all participants of at least speaker level. 
        
         - B{video_call} - If true, indicates that we want to do a video call (video still needs to be separately enabled) 
        
         - B{origin} - When call is initiated from web link, this argument must contain the URI that was used 
        
        """
        l_request = skypekit.XCallRequest("ZR\022$", 18, 36)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, identities)
        l_request.add_parm('b', 2, video_call)
        l_request.add_parm('S', 3, origin)
        self.transport.xcall(l_request)
    def MuteMyMicrophone(self):
        """
        Sets VOICE_STATUS to LISTENING in the Participant instance associated with us, causing any input from our microphone to be ignored. This is a Conversation class method, rather than Participant class, because this only applies to local participant. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\013", 18, 11)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def UnmuteMyMicrophone(self):
        """
        Sets VOICE_STATUS to SPEAKING in the Participant instance associated with us, causing any input from our microphone to be sent to the call host. This is a Conversation class method, rather than Participant class, because this only applies to local participant. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\014", 18, 12)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def HoldMyLiveSession(self):
        """
        Puts the conversation on hold - Conversation LOCAL_LIVESTATUS changes to ON_HOLD_LOCALLY and to ON_HOLD_REMOTELY for remote participants. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\015", 18, 13)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def ResumeMyLiveSession(self):
        """
        Resumes call from local hold. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\016", 18, 14)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def LeaveLiveSession(
        self,
        post_voice_autoresponse = False
        ):
        """
        Hang up or refuse to answer an incoming call. Set postVoiceAutoresponse to true to enable a caller to leave a voicemail message. 
        
        
        B{Arguments:}
         - B{post_voice_autoresponse}
        """
        l_request = skypekit.XCallRequest("ZR\022\017", 18, 15)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('b', 1, post_voice_autoresponse)
        self.transport.xcall(l_request)
    def StartVoiceMessage(self):
        """
        Begin recording a voice mail for this conversation's remote participant. Applies to conversations of type DIALOG only. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022-", 18, 45)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def TransferLiveSession(
        self,
        identities,
        transfer_topic = ""
        ):
        """
        This method is for doing call transfers. NB! Call transfers only work in one-on-one conversations (dialogs). Before attempting call transfer, you should check availability of transfer recipients with Conversation class CanTransferLiveSession method. If the current conversation has a live session up, that session (call) will be transferred to targets specified in the identities list. Note that identities is a string list - all identities in that list will get incoming calls. The first one of them to pick up the call - wins, and rest of the transfer targets will stop ringing. 
        
        Let's take a closer look how this works in practice. We have three call participants involved in the process, and two separate conversations. Let there be three callers: Caller A (call originator), Caller B (transferor) and Caller C (recipient of transfer). 
        
          - Caller A - calls Caller B; Caller B picks up the call - live conversation C1 is now up with A and B in it. 
          - After awhile, Caller B initiates call transfers to Caller C (and optionally to Callers D, E, F.. ). LOCAL_LIVESTATUS of C1 will get set to TRANSFERRING for both A and B. 
          - Caller C picks up the call. Conversation C1 will go off live status. For Caller B, conversation C1 LOCAL_LIVESTATUS will change to RECENTLY_LIVE. Another live conversation - C2 gets spawned, with Caller A and Caller C in it. For caller C, participant object representing caller A will have TRANSFERRED_BY property set to identity of caller A. For Caller B (in now no longer live conversation C1), participant object representing caller A gets its TRANSFERRED_TO property set to identity of caller C. 
        
        
        B{Arguments:}
         - B{identities} - String list of transfer target identities. As soon as first one in this list picks up the call, others will stop ringing. 
        
         - B{transfer_topic} - Optional conversation topic. This value will get set as META_TOPIC property of the conversation at the transferee end. Note that this is the only case where META_TOPIC field is used in context of dialog conversations. Thus assumption that remote UI will display topic field in case of dialogs may not be 100% correct. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022(", 18, 40)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, identities)
        l_request.add_parm('S', 2, transfer_topic)
        self.transport.xcall(l_request)
    def CanTransferLiveSession(
        self,
        identity
        ):
        """
        Checks if the identity is available for receiving a transferred live session. If you are going to attempt to go for multiple transfer targets, you should use this check for all the target identities. 
        
        
        B{Arguments:}
         - B{identity} - Target identity. 
        
        
        B{Return values:}
         - B{result} - Returns true if call transfer to given target is possible. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022.", 18, 46)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, identity)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def SendDTMF(
        self,
        dtmf,
        length_in_ms = 260
        ):
        """
        Sends DTMF tone to a live conversation. 
        
        
        B{Arguments:}
         - B{dtmf} - Outgoing dtmf tone, possible values come from Participant::DTMF enumerator. 
        
         - B{length_in_ms} - Duration in milliseconds. Defaults to 260 ms. Note that the DTMF tone can be also cancelled with Conversation::StopSendDTMF method. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\020", 18, 16)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Participant.DTMF[dtmf])
        l_request.add_parm('u', 2, length_in_ms)
        self.transport.xcall(l_request)
    def StopSendDTMF(self):
        """
        Stops the current DTMF tone being played into conversation. For example, use this method to cancel DTMF signals started with Conversation::SendDTMF before the duration given in lengthInMS runs out. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\060", 18, 48)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def SetMyTextStatusTo(
        self,
        status
        ):
        """
        Sets local user typing indicator in the Conversation. Remote Participants can display these in their UI. 
        
        
        B{Arguments:}
         - B{status} - Typing indicator status value - Participant::TEXT_STATUS 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\022", 18, 18)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Participant.TEXT_STATUS[status])
        self.transport.xcall(l_request)
    def PostText(
        self,
        text,
        is_xml = False
        ):
        """
        Posts the specified text the conversation, and populates message with a reference to the corresponding Message object (if no error occurred during execution). The isXML argument can be used if the client UI has already taken care of converting message text to xml (for example, your UI might enable users to use bold tags in text messages.) 
        
        
        B{Arguments:}
         - B{text} - Text value of the outgoing message (gets set as BODY_XML property of the Message object). 
        
         - B{is_xml} - For cases where the text argument was already encoded as xml message. 
        
        
        B{Return values:}
         - B{message} - Returns the Message object created as a result of this method (if successful). 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\023", 18, 19)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, text)
        l_request.add_parm('b', 2, is_xml)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[9](l_response.get(1), self.transport)
        return l_result
    def PostContacts(
        self,
        contacts
        ):
        """
        Takes a list of Contacts as an argument and posts the list into the Conversation. The purpose of this feature is to enable sharing contacts between users, without resorting to contact search. Instead, if user A has contacts B and C, he can post contact C into chat with contact B. At this point, Contact B can add contact C to his contact list. From remote side, the posted contacts will appear as messages with type Message::POSTED_CONTACTS appearing in the conversation. The UI should allow adding these contacts from messages with this type into the contact list. 
        
        The list of posted contacts can be retrieved with the Message::GetContacts method. 
        
        Additionally, the UI then can parse the posted Contact data out of the Message::P_BODY_XML property. The list of contacts is wrapped between <contacts ..> </contacts> tags. Each contact item in the xml has following format: 
          - t - contact type. "s" - skype contact; "p" - phone number; 
          - s - skypename, present only in skypename contacts (t="s") 
          - p - phone number, present only in phone number contacts (t="p") 
          - f - contact's full name, if available 
          - d - contact's display name, if available 
        
        Note that only the type (t) field is mandatory. Depending on type, either skypename (s) or phone number (p) fields are always present. Full name and display name fields are optional. 
        
        Example BODY_XML with skypname contact: 
        ::
          <contacts alt="alt text"><c t="s" s="skypename" f="full name"/></contacts> 
         
        
        Example BODY_XML with PSTN contact: 
        ::
          <contacts alt="alt text"><c t="p" p="+37212345678" d="Some PSTN number"/></contacts> 
         
        
        Example BODY_XML with multiple contacts: 
        ::
          <contacts alt="alt text"><c t="p" p="+37212345678" d="Some PSTN number"/><c t="s" s="someskypename"/></contacts> 
         
        
        
        B{Arguments:}
         - B{contacts} - List of Contact objects, to be posted in the conversation. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\024", 18, 20)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, contacts)
        self.transport.xcall(l_request)
    def PostFiles(
        self,
        paths,
        body
        ):
        """
        Takes a list of fully-qualified filenames and initiates corresponding file transfers in the conversation. From the remote side, incoming file transfers will appear as a conversation message with type POSTED_FILES. Once such a message is detected, the list of file transfer objects can be retrieved with Message::GetTransfers. At that point, remote participants will need to accept or decline those transfers. 
        
        
        B{Arguments:}
         - B{paths} - list of fully-qualified filenames to be transferred 
        
         - B{body} - Optional BODY_XML property for POSTED_FILES type messages that show up in remote UI. 
        
        
        B{Return values:}
         - B{error_code} - Error code, possible values come from the TRANSFER_SENDFILE_ERROR enumerator. This will be set for the first failed fail. The failed file is identified in the error_file return argument. 
        
         - B{error_file} - Filename of the file that triggered error. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\025", 18, 21)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('f', 1, paths)
        l_request.add_parm('S', 2, body)
        l_response = self.transport.xcall(l_request)
        l_result = (skypekit.enumof(Skype.TRANSFER_SENDFILE_ERROR, l_response.get(1))),
        l_result += (l_response.get(2, '')),
        return l_result
    def PostVoiceMessage(
        self,
        voicemail,
        body
        ):
        """
        Stops the active voicemail recording and sends it (dialog only)
        
        B{Arguments:}
         - B{voicemail} - This argument is deprecated as of SDK version 3.2. Instead of manually constructing Voicemail object, you can call Conversation::StartVoiceMessage method to start recording a voicemail in context of a dialog. PostVoiceMessage will stop recording this voicemail and post it in the dialog. If instead of sending Voicemail, the user decides to cancel it, you should use Conversation::LeaveLiveSession method (Voicemail::Cancel is deprecated). 
        
         - B{body} - Optional text message that remote UI can display in conversation, to notify the user of a new voicemail. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\026", 18, 22)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, voicemail)
        l_request.add_parm('S', 2, body)
        self.transport.xcall(l_request)
    def PostSMS(
        self,
        sms,
        body = ""
        ):
        """
        Takes an SMS instance created by Skype::CreateOutgoingSms and posts it in the conversation. Note that you will need to set both Sms body text (Sms::SetBody) and recipient list (Sms::SetTargets) before you can post the object. 
        
        
        B{Arguments:}
         - B{sms} - SMS object. 
        
         - B{body} - This argument is currently ignored. The message text needs to be set with Sms::SetBody method, prior to passing the Sms object to this method 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\027", 18, 23)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, sms)
        l_request.add_parm('S', 2, body)
        self.transport.xcall(l_request)
    def GetJoinBlob(self):
        """
        Retrieves a binary join blob for joining public conversations, which are always of type CONFERENCE. If called for a dialog, the blob argument will contain the empty string. The best way to create a Public Chat is to first create a fresh conversation with Skype class CreateConference, then minimally apply the public chat options OPT_JOINING_ENABLED and OPT_ENTRY_LEVEL_RANK - options, like this (C++): 
        ::
          C->SetOption(Conversation::OPT_JOINING_ENABLED, true); 
         
        
        When that is done, you can call GetJoinBlob to retrieve the blob string. Use the blob string to generate and post an HTML link whose href attribute looks like this: href="skype:?chat&blob=_BLOB_GOES_HERE" A person running Skype desktop client can click this link to join the conversation and have that conversation opened in his UI. Note that the conversation host (creator) needs to be online for new joiners-via-link to start participating in the Public Chat.
        
        B{Return values:}
         - B{blob} - Returns the public conversation join blob. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\030", 18, 24)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def Join(self):
        """
        Tries to join a public conversation (aka public chat). This method is only useful if you have used Skype::GetConversationByBlob method with alsoJoin argument set to false. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\031", 18, 25)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def EnterPassword(
        self,
        password
        ):
        """
        Submits password for joining password-protected conversation. 
        
        
        B{Arguments:}
         - B{password} - Password string. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\032", 18, 26)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, password)
        self.transport.xcall(l_request)
    def SetPassword(
        self,
        password,
        hint
        ):
        """
        Sets password protection/new password for the conversation. 
        
        
        B{Arguments:}
         - B{password} - New password. 
        
         - B{hint} - Password hint. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\033", 18, 27)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, password)
        l_request.add_parm('S', 2, hint)
        self.transport.xcall(l_request)
    def RetireFrom(self):
        """
        Leaves the conference. Not applicable to dialogs. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\034", 18, 28)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def Delete(self):
        """
        Deletes this conversation, which must be of type CONFERENCE - dialogs between local user and any of his contacts are always persistant. Note that this also removes corresponding Message and Participant objects. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022/", 18, 47)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def RenameTo(
        self,
        name
        ):
        """
        Changes the META_NAME property of the conversation. Note that unlike topic and guidelines, this rename is just local - remote participants can have their own private names for conversations. 
        
        
        B{Arguments:}
         - B{name} - New name for the conversation. Passing an empty string in this argument causes the META_NAME to unset. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\035", 18, 29)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, name)
        self.transport.xcall(l_request)
    def SetBookmark(
        self,
        bookmark
        ):
        """
        Setter for Conversation class IS_BOOKMARKED. 
        
        
        B{Arguments:}
         - B{bookmark} - Set true to set the bookmark, false to remove the bookmark. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\036", 18, 30)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('b', 1, bookmark)
        self.transport.xcall(l_request)
    def SetAlertString(
        self,
        alert_string
        ):
        """
        Setter for Conversation class ALERT_STRING property. The main use of this property is checking bodies of incoming messages in the conversation for the alert string and producing notifications in UI for the user, when appropriate. 
        
        
        B{Arguments:}
         - B{alert_string} - Substring to check in BODY_XML property of incoming messages. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\037", 18, 31)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, alert_string)
        self.transport.xcall(l_request)
    def RemoveFromInbox(self):
        """
        Removes conversation from Inbox. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022 ", 18, 32)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def AddToInbox(
        self,
        timestamp = 0
        ):
        """
        Sets Conversation inbox_timestamp property. If the timestamp argument is left empty or is greater than conversation consumption horizon, then the conversation will be restored to the inbox. 
        
        
        B{Arguments:}
         - B{timestamp} - If left empty or set to 0, the inbox_timestamp property is set to current time. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022!", 18, 33)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('u', 1, timestamp)
        self.transport.xcall(l_request)
    def SetConsumedHorizon(
        self,
        timestamp,
        also_unconsume = False
        ):
        """
        This method can be used to set the consumption (read) status of messages in the conversation. It sets Message::CONSUMPTION_STATUS to Message::CONSUMED for all messages in the conversation, older than the given timestamp. If the second argument is set to true, it also modifies messages more recent than the timestamp, by marking them as unconsumed. 
        
        
        B{Arguments:}
         - B{timestamp} - Consumption cutoff timestamp. Setting this to current time will mark all messages in the conversation as consumed. 
        
         - B{also_unconsume} - If set to true, this also marks messages newer than the cutoff timestamp as unconsumed. For example, setting timestamp to 0 and also_unconsumed to true, will unconsume all messages in the conversation. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022\042", 18, 34)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('u', 1, timestamp)
        l_request.add_parm('b', 2, also_unconsume)
        self.transport.xcall(l_request)
    def MarkUnread(self):
        """
        sets consumption horizon to last inbox message id timestamp
        """
        l_request = skypekit.XCallRequest("ZR\022#", 18, 35)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def IsMemberOf(
        self,
        group
        ):
        """
        Checks if the conversation is a member of the given ContactGroup 
        
        
        B{Arguments:}
         - B{group} - ContactGroup 
        
        
        B{Return values:}
         - B{result} - True if this conversation is a member of the ContactGroup specified by the group argument contains the conversation 
        
        """
        l_request = skypekit.XCallRequest("ZR\022%", 18, 37)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('O', 1, group)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def GetParticipants(
        self,
        filter = "ALL"
        ):
        """
        Retrieves the list of this conversation's current participants, which you can optionally request to be filtered. If no Participants pass the filter, an empty list will be returned (the method itself still returns true). 
        
        
        B{Arguments:}
         - B{filter} - Conversation::PARTICIPANTFILTER - defaults to Conversation::ALL 
        
        
        B{Return values:}
         - B{participants} - List of conversation Participant objects that passed the filter. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022&", 18, 38)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Conversation.PARTICIPANTFILTER[filter])
        l_response = self.transport.xcall(l_request)
        l_result = [module_id2classes[19](oid, self.transport) for oid in l_response.get(1, [])]
        return l_result
    def GetLastMessages(
        self,
        require_timestamp = 0
        ):
        """
        Returns recent messages. The messages are returned in two lists - new messages (unconsumed) and recent message history (context messages). The context message list contains messages that are already above the consumption horizon but are fairly recent, making it likely that displaying them in UI would be good default behaviour. 
        
        
        B{Arguments:}
         - B{require_timestamp} - If set to a non-zero value, includes messages no earlier than this timestamp, if not, includes messages from the last 24 hours only 
        
        
        B{Return values:}
         - B{context_messages} - Already consumed messages, provided for context
         - B{unconsumed_messages} - Unconsumed messages
        """
        l_request = skypekit.XCallRequest("ZR\022'", 18, 39)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('u', 1, require_timestamp)
        l_response = self.transport.xcall(l_request)
        l_result = ([module_id2classes[9](oid, self.transport) for oid in l_response.get(1, [])]),
        l_result += ([module_id2classes[9](oid, self.transport) for oid in l_response.get(2, [])]),
        return l_result
    def FindMessage(
        self,
        text,
        from_timestamp_up = skypekit.MAX_UINT
        ):
        """
        Finds the most recent Message object in the conversation that contains the substring specified by the text argument. If no matching messages are found, this method will return false. The search proceeds backwards in time, starting from the timestamp argument. To continue searching, you can start with timestamp=MAX_UINT, retrieve the TIMESTAMP property of the matching message, decrement it by one, and submit it as timestamp for the next FindMessage call. 
        
        
        B{Arguments:}
         - B{text} - Substring to search for. 
        
         - B{from_timestamp_up}
        
        B{Return values:}
         - B{message} - Returns matching message or 0 if there was no match. As the likelihood of this object being invalid is quite high, you should always check for method return value before you start calling methods of this object. 
        
        """
        l_request = skypekit.XCallRequest("ZR\022)", 18, 41)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, text)
        l_request.add_parm('u', 2, from_timestamp_up)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[9](l_response.get(1), self.transport)
        return l_result
    def OnParticipantListChange(self):
        """
        This callback gets fired when participants join or leave the conversation. 
        
        """
        pass
    def _sk_on_participant_list_change(self, parms):
        """ internal event dispatcher for OnParticipantListChange
  """
        self.OnParticipantListChange()
    event_handlers[1] = "_sk_on_participant_list_change"
    def OnMessage(
        self,
        message
        ):
        """
        Called for each message in currently loaded conversations
        
        B{Return values:}
         - B{message}
        """
        pass
    event_handlers[2] = "_sk_on_message"
    def _sk_on_message(self, parms):
        """ internal event dispatcher for OnMessage
  """
        l_cleanparms = module_id2classes[9](parms.get(1), self.transport)
        self.OnMessage(l_cleanparms)
    def OnSpawnConference(
        self,
        spawned
        ):
        """
        This callback gets fired when a new Conversation is created using SpawnConference. 
        
        
        B{Return values:}
         - B{spawned} - Conversation object that got created. 
        
        """
        pass
    event_handlers[3] = "_sk_on_spawn_conference"
    def _sk_on_spawn_conference(self, parms):
        """ internal event dispatcher for OnSpawnConference
  """
        l_cleanparms = module_id2classes[18](parms.get(1), self.transport)
        self.OnSpawnConference(l_cleanparms)
module_id2classes[18] = Conversation

class Message(skypekit.Object):
    """
    Events in a conversation context are expressed as Messages. It is therefore useful to think of Message objects as events, rather than specifically text chat messages. 
    
    Message member functions all return a Boolean indicating the success (true) or failure (false) of processing the request itself (transport, runtime availability, and so forth) - not the success or failure of its associated functionality. For example, Message::Edit returns true if it was able to make a determination, and its result parameter reflects whether this Message can be edited. Similarly, Message::Edit returns false if it was unable to make a determination, and the value of its result parameter is undefined. 
    
    Message member functions that are specific to a Message TYPE return false if this Message is not of that type. For example, Message::GetVoiceMessage will return false if this Message's type is not POSTED_VOICE_MESSAGE. 
    
    The actual meaning of a Message can be determined by its P_TYPE property. The meanings of most other Message properties depend on the value of P_TYPE. For example, let's take P_BODY_XML property. 
    
    Following messages have a text entered by the user as a body. It may contain emoticons, URLs, etc. 
      - POSTED_TEXT 
      - POSTED_EMOTE 
      - SET_METADATA 
      - REQUESTED_AUTH 
    
    Following messages have a custom XML format for the body (see the specific section on these message types for details): 
      - POSTED_CONTACTS 
      - POSTED_VOICE_MESSAGE 
      - POSTED_FILES 
      - POSTED_SMS 
      - STARTED_LIVESESSION and ENDED_LIVESESSION (same format) 
    
    Following messages do not use the body property: 
      - SPAWNED_CONFERENCE 
      - ADDED_CONSUMERS 
      - ADDED_APPLICANTS 
      - RETIRED_OTHERS 
      - RETIRED 
      - SET_RANK 
      - HAS_BIRTHDAY 
      - GRANTED_AUTH 
      - BLOCKED 
    
    Messages such as POSTED_TEXT use a small subset of a HTML-like markup to control the visual representation of the text. This markup is used by POSTED_TEXT and POSTED_EMOTE, but also for the conversation topic (CONVERSATION_META_TOPIC property and the body of the SET_METADATA message) and for authorization requests. 
    
    Having chat messages in XML format means that all formatting is indicated by XML tags. This includes emoticons and URls. The big advantage is that it makes the parsing of the message by the UI much easier. The UI does not need to do emoticons or URL detection, this is already done and it only needs to look for the XML tags. 
    
    For text messages, it is possible for the UI to simply ignore (meaning strip) the XML and the message will be understandable fine, it will only have lost some formatting. 
    
    But it is obviously nicer to display at least the most commonly used tags. 
    
    To strip the XML: 
      - if they have the alt="sometext" attribute set, return sometext as the output of that tag and ignore the rest of tag and all nested sub tags 
      - if no alt="" attribute set, use tag content as output - <sometag>hereissomething</sometag> is output as hereissomething
      - if no alt="" and no tag content, ignore the tag altogether (return nothing) 
    Skype for Windows supports displaying many XML tags, but only a sub-set is regularly used and should be supported by the UI for a good experience. These are the ones described here. 
    Animated emoticons 
    Emoticons are encoded with the "ss" tag. The element content is the plain text representation. It has a "type" attribute indicating the emoticons canonical name. Example: 
    ::
      Hi <ss type="smile">:-)</ss> 
     
    
    Flag emoticons 
    Flag emoticons are little flags. They are encoded with the "flag" tag. The element contents is the plain text representation and it has a "country" attribute which is a 2-letter ISO-3166 country code. The user can enter a flag using "(flag:XX)", where XX is a valid ISO country code. Example: 
    ::
      I am in <flag country="cc">CC</flag> 
     
    
    Links 
    If the library detects a URL, it will encode it using the html "a" tag, with the "href" attribute indicating the URL. The plain text representation is what the user originally typed. Example: 
    ::
      I am in <a href="http://wwww.skype.com">www.skype.com</a> 
     
    
    Alert matches 
    When a conversation is configured to display only alerts if specific words are present in the message (see "/alertson [text to match]" command), if a message matches the alert, it will be marked with the <alertmatch> tag. This allows the UI to highlight the word matching. Example: 
    ::
      Maybe <alertmatch>Vincent</alertmatch> knows the answer 
     
    
    Bold, italic, etc 
    Skype for Windows also supports displaying bold and italic text, using the "b" and "i" tags. 
    
    Encoding messages 
    When sending a chat message via PostText(), there is the possibility to indicate if the library should do the XML encoding, or if the message has already been encoded. Usually, the UI can let library do the encoding. This is the case when the message does not contain any specific formatting. It may contain emoticons or URls, which will be detected by the library encoder and converted into XML tags. 
    If the message has some more complex encoding, such as a quote or some bold text, it is up to the UI to encode the message. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "Message %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(Message, "P_"+propname.upper())
    module_id = 9
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    TYPE = {
        2
  :'SET_METADATA',
        4
  :'SPAWNED_CONFERENCE',
        10 :'ADDED_CONSUMERS',
        11 :'ADDED_APPLICANTS',
        12 :'RETIRED_OTHERS',
        13 :'RETIRED',
        21 :'SET_RANK',
        30 :'STARTED_LIVESESSION',
        39 :'ENDED_LIVESESSION',
        50 :'REQUESTED_AUTH',
        51 :'GRANTED_AUTH',
        53 :'BLOCKED',
        61 :'POSTED_TEXT',
        60 :'POSTED_EMOTE',
        63 :'POSTED_CONTACTS',
        64 :'POSTED_SMS',
        65 :'POSTED_ALERT',
        67 :'POSTED_VOICE_MESSAGE',
        68 :'POSTED_FILES',
        69 :'POSTED_INVOICE',
        110:'HAS_BIRTHDAY',
        'SET_METADATA'
        
                
              :  2,
        'SPAWNED_CONFERENCE'
        
                
        :  4,
        'ADDED_CONSUMERS'
        
                
           : 10,
        'ADDED_APPLICANTS'
        
                
          : 11,
        'RETIRED_OTHERS'
        
                
            : 12,
        'RETIRED'
        
                
                   : 13,
        'SET_RANK'
        
                
                  : 21,
        'STARTED_LIVESESSION'
        
                       : 30,
        'ENDED_LIVESESSION'
        
                
         : 39,
        'REQUESTED_AUTH'
        
                
            : 50,
        'GRANTED_AUTH'
        
                
              : 51,
        'BLOCKED'
        
                
                   : 53,
        'POSTED_TEXT'
        
                
               : 61,
        'POSTED_EMOTE'
        
                
              : 60,
        'POSTED_CONTACTS'
        
                
           : 63,
        'POSTED_SMS'
        
                
                : 64,
        'POSTED_ALERT'
        
                
              : 65,
        'POSTED_VOICE_MESSAGE'
        
                      : 67,
        'POSTED_FILES'
        
                
              : 68,
        'POSTED_INVOICE'
        
                
            : 69,
        'HAS_BIRTHDAY'
        
                
              :110
    }
    """
    The P_TYPE property determines the actual meaning of the Message object. Only Messages of POSTED_TEXT type contain actual text messages. The meaning and content of the rest of the message properties are largely dependant of the value of the Message::P_TYPE. 
    
     - SET_METADATA - Conference metadata were changed
     - SPAWNED_CONFERENCE - A conference was spawned from this dialog
     - ADDED_CONSUMERS - Some users were added to the conference
     - ADDED_APPLICANTS - Some users are applying to be added to the conference
     - RETIRED_OTHERS - User was kicked from the conference
     - RETIRED - User left the conference
     - SET_RANK - Changed the rank of a user in the Conversation (multichat administration) 
    
     - STARTED_LIVESESSION - A live session started
     - ENDED_LIVESESSION - A live session ended
     - REQUESTED_AUTH - User requested authorization
     - GRANTED_AUTH - User was granted authorization. Notification message that user is now an authorized contact (of the local user). 
    
     - BLOCKED - User was blocked
     - POSTED_TEXT - A text message
     - POSTED_EMOTE - An emote ('John Doe is laughing', cf /me chat command)
     - POSTED_CONTACTS - The message represents (a set of) contact card(s) posted in the conversation. One message can contain more than one contact cards. The contacts can be retrieved from the message by parsing them out from the P_BODY_XML property. For more information, see Conversation::PostContacts 
    
     - POSTED_SMS - The message represents an SMS object that was posted in the Conversation. See Conversation::PostSMS for more details. The Sms object itself can be retrieved from the Message with Message::GetSms The message BODY_XML contains a set of SMS properties, such as status, failurereason, targets, price and timestamp. 
    
     - POSTED_ALERT - Deprecated, never sent
     - POSTED_VOICE_MESSAGE - A voicemail
     - POSTED_FILES - The message represents a (list of) file transfers that were posted in the Conversation with Conversation::PostFiles. Transfer objects can be retrieved from the Message with Message::GetTransfers 
    
     - POSTED_INVOICE - Currently unused. 
    
     - HAS_BIRTHDAY - The message represents a Contact birthday notification. 
    
    """
    SENDING_STATUS = {1:'SENDING', 'SENDING':1, 2:'SENT', 'SENT':2, 3:'FAILED_TO_SEND', 'FAILED_TO_SEND':3}
    """
     - SENDING - Message has not been delivered to at least one of the participants 
    
     - SENT - Message has been delivered to at least one other participant 
    
     - FAILED_TO_SEND - Message could not be delivered (for SMS this reflects the actual SMS, not the chat message) 
    
    """
    CONSUMPTION_STATUS = {0:'CONSUMED', 'CONSUMED':0, 1:'UNCONSUMED_SUPPRESSED', 'UNCONSUMED_SUPPRESSED':1, 2:'UNCONSUMED_NORMAL', 'UNCONSUMED_NORMAL':2, 3:'UNCONSUMED_ELEVATED', 'UNCONSUMED_ELEVATED':3}
    """
    Indicates if a message has been consumed (meaning read) or not
     - CONSUMED - Message has been read. Note that this is a read-only property. Consumption status of individual messages can not be set selectively. Message consumption status is determined at the conversation level, based conversation consumption horizon and individual message timestamps. Conversation consumption horizon can be updated with Conversation::SetConsumedHorizon method. 
    
     - UNCONSUMED_SUPPRESSED - Do not notify the user that they have this unread message 
    
     - UNCONSUMED_NORMAL - Notify the user that they have this unread message 
    
     - UNCONSUMED_ELEVATED - This message consumption state is marked as DEPRECATED 
    
    """
    SET_METADATA_KEY = {3640:'SET_META_NAME', 'SET_META_NAME':3640, 3644:'SET_META_TOPIC', 'SET_META_TOPIC':3644, 3652:'SET_META_GUIDELINES', 'SET_META_GUIDELINES':3652, 3658:'SET_META_PICTURE', 'SET_META_PICTURE':3658}
    """
    For messages of type SET_METADATA that alert participants to changes to the associated Conversation's metadata, indicates which metadata property changed and its P_BODY_XML property contains the changed data. Your UI is expected to detect messages with PARAM_KEY set and to update its visual representation of Conversation accordingly. 
    You can use the associated Conversation's properties and methods to obtain the updated metadata rather than parse the message body XML, for example, Conversation::P_META_PICTURE and Conversation::Conversation::GetPropMetaPicture. 
    
     - SET_META_NAME - Notification message that conversation name has changed. 
    
     - SET_META_TOPIC - Notification message that conversation topic has changed. 
    
     - SET_META_GUIDELINES - Notification message that conversation guidelines have changed. 
    
     - SET_META_PICTURE - Notification message that conversation picture has changed. 
    
    """
    LEAVEREASON = {2:'USER_INCAPABLE', 'USER_INCAPABLE':2, 3:'ADDER_MUST_BE_FRIEND', 'ADDER_MUST_BE_FRIEND':3, 4:'ADDER_MUST_BE_AUTHORIZED', 'ADDER_MUST_BE_AUTHORIZED':4, 5:'DECLINE_ADD', 'DECLINE_ADD':5, 6:'UNSUBSCRIBE', 'UNSUBSCRIBE':6}
    """
    Indicates the reason a user could not join or left a Conversation. SkypeKit automatically sets "could not join"-related values. "Left voluntarily"-related values are set as a result of explicit user actions. 
    
     - USER_INCAPABLE - User cannot chat (user is currently logged in with a client that has chat disabled - see Contact::CAPABILITY.CAPABILITY_TEXT) 
    
     - ADDER_MUST_BE_FRIEND - Attempt to add local user to a conversation by an unknown contact 
    
     - ADDER_MUST_BE_AUTHORIZED - Attempt to add local user to a conversation by an unauthorized contact 
    
     - DECLINE_ADD - Local user declined an "invitation" to join a chat 
    
     - UNSUBSCRIBE - User decided to end participation in an on-going multi-chat 
    
    """
    
    def _sk_get_convo_id(self):
        """
        DB ID of corresponding conversation
        """
        return module_id2classes[18](self._sk_property("ZG\300\007]\011",
  960, True), self.transport)
    convo_id = property(_sk_get_convo_id)
    propid2label[960] = "convo_id"
    P_CONVO_ID = 960
    def _sk_get_convo_guid(self):
        """
        GUID of the Conversation. The GUID is a "global ID" - these values are shared accross Skype client instances and accross all the participants of the conversation. 
        
        """
        return self._sk_property("ZGx]\011", 120, True)
    convo_guid = property(_sk_get_convo_guid)
    propid2label[120] = "convo_guid"
    P_CONVO_GUID = 120
    def _sk_get_author(self):
        """
        Identity of the sender. While this is almost always the same as SKYPENAME property of the Contact, in some rare cases it can also be a phone number - for example, incoming voicemail notification Messages (message type = POSTED_VOICE_MESSAGE). 
        
        """
        return self._sk_property("ZGz]\011", 122, True)
    author = property(_sk_get_author)
    propid2label[122] = "author"
    P_AUTHOR = 122
    def _sk_get_author_displayname(self):
        """
        displayname of the sender at the time of posting
        """
        return self._sk_property("ZG{]\011", 123, True)
    author_displayname = property(_sk_get_author_displayname)
    propid2label[123] = "author_displayname"
    P_AUTHOR_DISPLAYNAME = 123
    def _sk_get_guid(self):
        """
        Unlike the message id, the GUID is the same on all instances and for all participants. 
        
        """
        return self._sk_property("ZG\230\006]\011", 792, True)
    guid = property(_sk_get_guid)
    propid2label[792] = "guid"
    P_GUID = 792
    def _sk_get_originally_meant_for(self):
        """
        This property gets set when a conference is spawned from dialog Conversation. In that case recent message history from the original dialog is copied to the target conversation. For all the copied messages, the ORIGINALLY_MEANT_FOR property will be set to identity of the remote participant of the original dialog. 
        
        """
        return self._sk_property("ZG\226\006]\011", 790, True)
    originally_meant_for = property(_sk_get_originally_meant_for)
    propid2label[790] = "originally_meant_for"
    P_ORIGINALLY_MEANT_FOR = 790
    def _sk_get_timestamp(self):
        """
        UNIX timestamp (sent time, adjusted for local clock)
        """
        return self._sk_property("ZGy]\011", 121, True)
    timestamp = property(_sk_get_timestamp)
    propid2label[121] = "timestamp"
    P_TIMESTAMP = 121
    def _sk_get_type(self):
        return skypekit.enumof(Message.TYPE, self._sk_property("ZG\301\007]\011", 961, True))
    type = property(_sk_get_type)
    propid2label[961] = "type"
    P_TYPE = 961
    def _sk_get_sending_status(self):
        return skypekit.enumof(Message.SENDING_STATUS, self._sk_property("ZG\302\007]\011", 962, True))
    sending_status = property(_sk_get_sending_status)
    propid2label[962] = "sending_status"
    P_SENDING_STATUS = 962
    def _sk_get_consumption_status(self):
        return skypekit.enumof(Message.CONSUMPTION_STATUS, self._sk_property("ZG\310\007]\011", 968, True))
    consumption_status = property(_sk_get_consumption_status)
    propid2label[968] = "consumption_status"
    P_CONSUMPTION_STATUS = 968
    def _sk_get_edited_by(self):
        """
        Identity of the author that last edited this message. NULL if message has not been edited 
        
        """
        return self._sk_property("ZG\336\001]\011", 222, True)
    edited_by = property(_sk_get_edited_by)
    propid2label[222] = "edited_by"
    P_EDITED_BY = 222
    def _sk_get_edit_timestamp(self):
        """
        UNIX timestamp of last edit
        """
        return self._sk_property("ZG\337\001]\011", 223, True)
    edit_timestamp = property(_sk_get_edit_timestamp)
    propid2label[223] = "edit_timestamp"
    P_EDIT_TIMESTAMP = 223
    def _sk_get_param_key(self):
        """
        Message type-specific parameter. See Message::SET_METADATA_KEY for more information. 
        
        """
        return self._sk_property("ZG\303\007]\011", 963, True)
    param_key = property(_sk_get_param_key)
    propid2label[963] = "param_key"
    P_PARAM_KEY = 963
    def _sk_get_param_value(self):
        """
        Message type-specific parameter 
        
        """
        return self._sk_property("ZG\304\007]\011", 964, True)
    param_value = property(_sk_get_param_value)
    propid2label[964] = "param_value"
    P_PARAM_VALUE = 964
    def _sk_get_body_xml(self):
        """
        Message type-specific parameter 
        
        """
        return self._sk_property("ZG\177]\011", 127, True)
    body_xml = property(_sk_get_body_xml)
    propid2label[127] = "body_xml"
    P_BODY_XML = 127
    def _sk_get_identities(self):
        """
        Message type-specific parameter. Depending of Message type, this property contains: 
          - STARTED_LIVESESSION - list of participants in the cal; 
          - ENDED_LIVESESSION - list of participants in the call; 
          - POSTED_SMS - list of recipients of the message; 
          - SPAWNED_CONFERENCE - the list of identities that were added; 
          - ADDED_CONSUMERS - the list of identities that were added; 
          - RETIRED_OTHERS - the skypename of the participant who was kicked; 
          - SET_RANK - the skypename of the participant whose rank was changed; 
          - REQUESTED_AUTH - Message::P_AUTHOR and Message::P_IDENTITIES are set to the users receiving and requesting the authorization, depending if the message was received or sent; 
          - GRANTED_AUTH - the skypename of the user we granted authorization; 
          - BLOCKED - the skypename of the user who was blocked; 
          - HAS_BIRTHDAY - skypename of current logged in user. 
        
        """
        return self._sk_property("ZG}]\011", 125, True)
    identities = property(_sk_get_identities)
    propid2label[125] = "identities"
    P_IDENTITIES = 125
    def _sk_get_reason(self):
        """
        Message type-specific parameter. Possible values for STARTED/ENDED_LIVESESSION (only set for dialogs): 
          - no_answer 
          - manual 
          - busy 
          - connection_dropped 
          - no_skypeout_subscription; 
          - insufficient_funds 
          - internet_connection_lost 
          - skypeout_account_blocked 
          - pstn_could_not_connect_to_skype_proxy 
          - pstn_invalid_number 
          - pstn_number_forbidden 
          - pstn_call_timed_out 
          - pstn_busy 
          - pstn_call_terminated 
          - pstn_network_error 
          - number_unavailable 
          - pstn_call_rejected 
          - pstn_misc_error 
          - internal_error 
          - unable_to_connect 
          - connection_dropped 
          - recording_failed 
          - playback_error 
          - legacy_error 
          - blocked_by_privacy_settings 
          - error 
          - transfer_failed 
          - transfer_insufficient_funds 
          - blocked_by_us 
          - emergency_call_denied 
        
        This information is now available as an enum in LEAVEREASON 
        
        """
        return self._sk_property("ZG\306\007]\011", 966, True)
    reason = property(_sk_get_reason)
    propid2label[966] = "reason"
    P_REASON = 966
    def _sk_get_leavereason(self):
        """
        Leave reason for message of the RETIRED type, and STARTED/ENDED_LIVESESSION. Use for STARTED/ENDED_LIVESESSION is to provide simpler, enum based handling and deprecates the reason property (only set for dialogs)
        """
        return skypekit.enumof(Skype.LEAVE_REASON, self._sk_property("ZG~]\011", 126, True))
    leavereason = property(_sk_get_leavereason)
    propid2label[126] = "leavereason"
    P_LEAVEREASON = 126
    def _sk_get_participant_count(self):
        """
        Number of people who received this message (including local user) 
        
        """
        return self._sk_property("ZG\326\007]\011", 982, True)
    participant_count = property(_sk_get_participant_count)
    propid2label[982] = "participant_count"
    P_PARTICIPANT_COUNT = 982
    
    def CanEdit(self):
        """
        For Message types having a body, determines whether that body is editable by the user. 
        
        
        B{Return values:}
         - B{result}
        """
        l_request = skypekit.XCallRequest("ZR\011\001", 9, 1)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def Edit(
        self,
        new_text,
        is_xml = False,
        undo = False
        ):
        """
        For Message types that include a body and are editable: 
          - alters BODY_XML of the message object 
          - sets EDITED_BY and EDIT_TIMESTAMP properties 
          - propagates the changes to remote users. 
        
        
        B{Arguments:}
         - B{new_text} - New value of the message BODY_XML property. 
        
         - B{is_xml} - Specify isXML as true if the message body is formatted as XML; omit it or specify it as false if the message body is plain text. 
        
         - B{undo} - Reverts the message body to the original version. newText argument is ignored when this is set. 
        
        """
        l_request = skypekit.XCallRequest("ZR\011\002", 9, 2)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, new_text)
        l_request.add_parm('b', 2, is_xml)
        l_request.add_parm('b', 3, undo)
        self.transport.xcall(l_request)
    def GetContacts(self):
        """
        For messages of type POSTED_CONTACTS, parses the body XML and formats the data as a list of Contact instances. 
        
        
        B{Return values:}
         - B{contacts}
        """
        l_request = skypekit.XCallRequest("ZR\011\003", 9, 3)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = [module_id2classes[2](oid, self.transport) for oid in l_response.get(1, [])]
        return l_result
    def GetTransfers(self):
        """
        For messages of type POSTED_FILES, parses the body XML and creates a list of Transfer instances. 
        
        
        B{Return values:}
         - B{transfers}
        """
        l_request = skypekit.XCallRequest("ZR\011\004", 9, 4)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = [module_id2classes[6](oid, self.transport) for oid in l_response.get(1, [])]
        return l_result
    def GetVoiceMessage(self):
        """
        For messages of type POSTED_VOICE_MESSAGE, parses the body XML and creates a Voicemail instance. 
        
        
        B{Return values:}
         - B{voicemail}
        """
        l_request = skypekit.XCallRequest("ZR\011\005", 9, 5)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[7](l_response.get(1), self.transport)
        return l_result
    def GetSMS(self):
        """
        For messages of type POSTED_SMS, parses the body XML and creates an SMS instances 
        
        
        B{Return values:}
         - B{sms}
        """
        l_request = skypekit.XCallRequest("ZR\011\006", 9, 6)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[12](l_response.get(1), self.transport)
        return l_result
    def DeleteLocally(self):
        """
        Deletes this message from the local database. These deletions do not propagate to the other Skype instances that the user may have on other computers. Nor do they affect other participants that have the same message. This method is specifically from removing Message objects from the database - not for removing Messages from conversations. To remove a Message from a conversation, use Message::Edit method to replace the existing body text with an empty string. 
        
        """
        l_request = skypekit.XCallRequest("ZR\011\010", 9, 8)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
module_id2classes[9] = Message

class Video(skypekit.Object):
    """
    This class contains basic video control functionality for live conversations with video. Basically, Video objects represent specific Participant's video state in a live Conversation. The Video class can represent both local (outgoing) and remote (incoming) video streams. Note that as of SkypeKit SDK version 3.2, this class no longer handles video rendering in the UI. Currently available SkypeKit runtimes do not support multi-party video. The API however is designed with future compatibility in mind, so the Video class is attached to Participant class rather than Conversation class. Once multi-party video will become available for SkypeKit, the logic should go like this: 
    
    Let there be 4-way live conversation C and participants P1, P2, P3 and P4. P1 is the local user. Remote participants P2 and P3 are capable of sending video. Remote user P4 is not capable of sending video. You would then end up with 4 video objects: V1, V2, V3 and V0. 
    
      - C->P1->V1-> outgoing video stream 
      - C->P2->V2-> incoming video stream 1 
      - C->P3->V3-> incoming video stream 2 
      - C->P4-> no video object as participant P4 does not advertise supporting video 
      - V0-> local webcam preview - this is not attached to any particular conversation, however the corresponding video object can be retrieved with Skype::GetPreviewVideo method. 
    
    As getting from a live conversation to running video streams involves three classes, it can be somewhat less than obvious. The basic state transition logic goes like this: 
    
    You start out with a Conversation, that suddenly becomes live 
    
    CONVERSATION::LOCAL_LIVESTATUS = IM_LIVE 
    At this point you have access to participant list of that conversation. The next step will be to catch Partcipant property changes for PARTICIPANT::VIDEO_STATUS to see if any of the people in conversation have Video available. Note that you should not make assumptions on when this availability happens. Remote users may switch their video on-off as they please. 
    
    PARTICIPANT::VIDEO_STATUS = VIDEO_AVAILABLE 
    If you get to VIDEO_AVAILABLE (not necessarily for all Participants), you can retrieve Video object, with Participant::GetVideo method. 
    
    Now you will need to handle Video::STATUS property changes. In case of successful video call, the sequence of Video::STATUS and Participant::VIDEO_STATUS changes for each Participant and Video object should look like this: 
    
      - Video::STATUS = AVAILABLE 
      - Video::STATUS = STARTING 
      - Video::STATUS = CHECKING_SUBSCRIPTION 
      - Video::STATUS = STARTING 
    
    Participant::VIDEO_STATUS = VIDEO_CONNECTING 
      - Video::STATUS = RUNNING 
      - Participant::VIDEO_STATUS = STREAMING 
    Both Video::STATUS == RUNNING and Participant::VIDEO_STATUS == STREAMING are indicative that the video for that particular participant is up and running, and your UI should update itself accordingly. 
    
    NB! Note that it is not enough to check for Video::STATUS property updates. By the time you obtain the Video object in your client, it may already it may already have progressed to a further status. You should always check the status property immediately after obtaining the Video object. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "Video %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(Video, "P_"+propname.upper())
    module_id = 11
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    STATUS = {
        0 :'NOT_AVAILABLE',
        1 :'AVAILABLE',
        2 :'STARTING',
        3 :'REJECTED',
        4 :'RUNNING',
        5 :'STOPPING',
        6 :'PAUSED',
        7 :'NOT_STARTED',
        8 :'HINT_IS_VIDEOCALL_RECEIVED',
        9 :'UNKNOWN',
        10:'RENDERING',
        11:'CHECKING_SUBSCRIPTION',
        12:'SWITCHING_DEVICE',
        'NOT_AVAILABLE'
        
                
             : 0,
        'AVAILABLE'
        
                
                 : 1,
        'STARTING'
        
                
                  : 2,
        'REJECTED'
        
                
                  : 3,
        'RUNNING'
        
                
                   : 4,
        'STOPPING'
        
                
                  : 5,
        'PAUSED'
        
                
                    : 6,
        'NOT_STARTED'
        
                
               : 7,
        'HINT_IS_VIDEOCALL_RECEIVED'
        
                : 8,
        'UNKNOWN'
        
                
                   : 9,
        'RENDERING'
        
                
                 :10,
        'CHECKING_SUBSCRIPTION'
        
                     :11,
        'SWITCHING_DEVICE'
        
                
          :12
    }
    """
     - NOT_AVAILABLE
     - AVAILABLE
     - STARTING
     - REJECTED
     - RUNNING
     - STOPPING
     - PAUSED
     - NOT_STARTED
     - HINT_IS_VIDEOCALL_RECEIVED
     - UNKNOWN
     - RENDERING
     - CHECKING_SUBSCRIPTION
     - SWITCHING_DEVICE
    """
    MEDIATYPE = {1:'MEDIA_SCREENSHARING', 'MEDIA_SCREENSHARING':1, 0:'MEDIA_VIDEO', 'MEDIA_VIDEO':0}
    """
     - MEDIA_SCREENSHARING
     - MEDIA_VIDEO
    """
    VIDEO_DEVICE_CAPABILITY = {0:'VIDEOCAP_HQ_CAPABLE', 'VIDEOCAP_HQ_CAPABLE':0, 1:'VIDEOCAP_HQ_CERTIFIED', 'VIDEOCAP_HQ_CERTIFIED':1, 2:'VIDEOCAP_REQ_DRIVERUPDATE', 'VIDEOCAP_REQ_DRIVERUPDATE':2, 3:'VIDEOCAP_USB_HIGHSPEED', 'VIDEOCAP_USB_HIGHSPEED':3}
    """
     - VIDEOCAP_HQ_CAPABLE
     - VIDEOCAP_HQ_CERTIFIED
     - VIDEOCAP_REQ_DRIVERUPDATE
     - VIDEOCAP_USB_HIGHSPEED
    """
    
    def _sk_get_status(self):
        """
        Video::STATUS
        """
        return skypekit.enumof(Video.STATUS, self._sk_property("ZG\202\001]\013", 130, True))
    status = property(_sk_get_status)
    propid2label[130] = "status"
    P_STATUS = 130
    def _sk_get_error(self):
        """
        'errorcode errortext' 
        """
        return self._sk_property("ZG\203\001]\013", 131, True)
    error = property(_sk_get_error)
    propid2label[131] = "error"
    P_ERROR = 131
    def _sk_get_debuginfo(self):
        """
        space-separated string of tokens
        """
        return self._sk_property("ZG\204\001]\013", 132, True)
    debuginfo = property(_sk_get_debuginfo)
    propid2label[132] = "debuginfo"
    P_DEBUGINFO = 132
    def _sk_get_dimensions(self):
        """
        This property does not currently work, always containing an empty string. For desktop video, you can get the frame dimensions from the video frame buffers API instead - the buffer struct retrieved with ipc.getFrame() or ipc.getNewFrame() has width and height fields, which you can then use in your UI. With RTP video solutions, you already have the frame sizes in your videohost code. Communicating these to the UI process is currently up to you. 
        
        """
        return self._sk_property("ZG\205\001]\013", 133, True)
    dimensions = property(_sk_get_dimensions)
    propid2label[133] = "dimensions"
    P_DIMENSIONS = 133
    def _sk_get_media_type(self):
        """
        Indicates whether the video object is streaming webcam video or screensharing session, values: MEDIA_SCREENSHARING, MEDIA_VIDEO 
        
        """
        return skypekit.enumof(Video.MEDIATYPE, self._sk_property("ZG\206\001]\013", 134, True))
    media_type = property(_sk_get_media_type)
    propid2label[134] = "media_type"
    P_MEDIA_TYPE = 134
    def _sk_get_convo_id(self):
        """
        conference id to be able to identify remote/local video in the same call
        """
        return self._sk_property("ZG\320\010]\013", 1104, True)
    convo_id = property(_sk_get_convo_id)
    propid2label[1104] = "convo_id"
    P_CONVO_ID = 1104
    def _sk_get_device_path(self):
        """
        device path used by video object
        """
        return self._sk_property("ZG\321\010]\013", 1105, True)
    device_path = property(_sk_get_device_path)
    propid2label[1105] = "device_path"
    P_DEVICE_PATH = 1105
    
    SETUPKEY_VIDEO_DEVICE = "Lib/Video/Device"
    SETUPKEY_VIDEO_DEVICE_PATH = "Lib/Video/DevicePath"
    SETUPKEY_VIDEO_AUTOSEND = "Lib/Video/AutoSend"
    SETUPKEY_VIDEO_DISABLE = "*Lib/Video/Disable"
    SETUPKEY_VIDEO_RECVPOLICY = "Lib/Video/RecvPolicy"
    SETUPKEY_VIDEO_ADVERTPOLICY = "Lib/Video/AdvertPolicy"
    def SetScreen(
        self,
        windowh
        ):
        """
        This is a legacy method, used in earlier version of desktop video, for passing window handles from the UI to the rendering engine in the runtime. In current version of desktop video, the rendering takes place in the client. The video objects now need to be associated not with a specific window, but rather with an IPC channel that the Video object sends decoded video frames from the runtime to the client UI. See Video::SetRemoteRendererId for more information. 
        
        
        B{Arguments:}
         - B{windowh}
        """
        l_request = skypekit.XCallRequest("ZR\013\001", 11, 1)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('u', 1, windowh)
        self.transport.xcall(l_request)
    def Start(self):
        """
        This method starts either video send or video receive, depending on whether the video object is sender or receiver. In case of desktop video, the receiver side needs to instantiate a renderer object and associate it with the receiveing video (Video::SetRemoteRendererId). 
        
        """
        l_request = skypekit.XCallRequest("ZR\013\002", 11, 2)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def Stop(self):
        """
        This method stops either video send or video receive, depending on whether the video object is sender or receiver. In case of desktop video, the receiver side needs to dis-associate the video object from the renderer, by calling Video::SetRemoteRendererId(0). 
        
        """
        l_request = skypekit.XCallRequest("ZR\013\003", 11, 3)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def SubmitCaptureRequest(self):
        """
        This method has no known effect in current version. 
        
        
        B{Return values:}
         - B{ret}
         - B{request_id}
        """
        l_request = skypekit.XCallRequest("ZR\013\013", 11, 11)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, False)),
        l_result += (l_response.get(2, 0)),
        return l_result
    def OnCaptureRequestCompleted(
        self,
        request_id,
        is_successful,
        image,
        width,
        height
        ):
        """
        This method has no effect in current version. 
        
        
        B{Return values:}
         - B{request_id}
         - B{is_successful}
         - B{image}
         - B{width}
         - B{height}
        """
        pass
    event_handlers[2] = "_sk_on_capture_request_completed"
    def _sk_on_capture_request_completed(self, parms):
        """ internal event dispatcher for OnCaptureRequestCompleted
  """
        l_cleanparms = (parms.get(1, 0)),
        l_cleanparms += (parms.get(2, False)),
        l_cleanparms += (parms.get(3, '')),
        l_cleanparms += (parms.get(4, 0)),
        l_cleanparms += (parms.get(5, 0)),
        self.OnCaptureRequestCompleted(*l_cleanparms)
    def SetScreenCaptureRectangle(
        self,
        x0,
        y0,
        width,
        height,
        monitor_number = 0,
        window_handle = 0
        ):
        """
        This method has no known effect in current version. 
        
        
        B{Arguments:}
         - B{x0}
         - B{y0}
         - B{width}
         - B{height}
         - B{monitor_number}
         - B{window_handle}
        """
        l_request = skypekit.XCallRequest("ZR\013\005", 11, 5)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('i', 1, x0)
        l_request.add_parm('i', 2, y0)
        l_request.add_parm('u', 3, width)
        l_request.add_parm('u', 4, height)
        l_request.add_parm('i', 5, monitor_number)
        l_request.add_parm('u', 6, window_handle)
        self.transport.xcall(l_request)
    def SetRenderRectangle(
        self,
        x0,
        y0,
        width,
        height
        ):
        """
        Sets video render rectangle co-ordinates, relative to upper left corner of the rendering window's client area (there us no need to compensate for the title bar area or window borders). Note that the rendering rectangle does not get automatically updated after video window resize occurs. You will need to catch the resize events and update the rendering rectangle according to the new size of the video window. 
        
        NB! Currently, when the updating the render rectangle to the new size of the video window, you will also need to call Video class SetScreen method - before SetRenderRectangle. 
        
        
        B{Arguments:}
         - B{x0}
         - B{y0}
         - B{width}
         - B{height}
        """
        l_request = skypekit.XCallRequest("ZR\013\006", 11, 6)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('i', 1, x0)
        l_request.add_parm('i', 2, y0)
        l_request.add_parm('u', 3, width)
        l_request.add_parm('u', 4, height)
        self.transport.xcall(l_request)
    def SetRemoteRendererId(
        self,
        id
        ):
        """
        This method is specific to working with desktop video runtimes. In case of desktop video, this method associates a Video object with a rendering object. Once this association is in place, the Video object will start sending decoded video frames to the renderer object, over shared memory IPC transport. The renderer then periodically reads the shared memory buffer and draws bitmaps on the screen. 
        
        The "remote renderer ID" value you are expected to pass in this method comes from the renderer. The renderer class is normally implemented by you (for a specific GUI framework). However, the shared memory transport mechanism, for receiving decoded frames, is independant of GUI framework and are provided with the SDK for all three supporeted operating systems. 
        
        Your renderer class can include instance of the IPC transport as VideoTransportClient <WinShm> ipc or VideoTransportClient <PosixShm> ipc; The ipc object then exposes ipc.key() function that returns the IPC channel ID. This ID can then be passed to the Video::SetRemoteRendererId method. A reference implementation of desktop video is included in the SDK (examples/cpp/qt_videocalls). 
        
        When the incoming video stream stops (or rendering is stopped by the user), your client should explicitly stop the frame transport by calling Video::SetRemoteRendererId(0). 
        
        
        B{Arguments:}
         - B{id} - IPC channel ID retrieved from the shared memory transport class (see ipc/cpp/VideoBuffers in the SDK). 
        
        """
        l_request = skypekit.XCallRequest("ZR\013\016", 11, 14)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('u', 1, id)
        self.transport.xcall(l_request)
    def SelectVideoSource(
        self,
        media_type,
        webcam_name = "",
        device_path = "",
        update_setup = False
        ):
        """
        This method has no effect in current version. 
        
        
        B{Arguments:}
         - B{media_type}
         - B{webcam_name}
         - B{device_path}
         - B{update_setup}
        """
        l_request = skypekit.XCallRequest("ZR\013\007", 11, 7)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Video.MEDIATYPE[media_type])
        l_request.add_parm('S', 2, webcam_name)
        l_request.add_parm('S', 3, device_path)
        l_request.add_parm('b', 4, update_setup)
        self.transport.xcall(l_request)
    def GetCurrentVideoDevice(self):
        """
        No description available.
        
        B{Return values:}
         - B{mediatype}
         - B{device_name}
         - B{device_path}
        """
        l_request = skypekit.XCallRequest("ZR\013\012", 11, 10)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = (skypekit.enumof(Video.MEDIATYPE, l_response.get(1))),
        l_result += (l_response.get(2, '')),
        l_result += (l_response.get(3, '')),
        return l_result
module_id2classes[11] = Video

class Voicemail(skypekit.Object):
    """
    Wrapper class that includes voicemail-specific methods and properties. In the Skype Conversation API, Voicemail is actually something of a misnomer for what would be more accurately called Voice Message. 
    
    The traditional Voicemail use case involves recording a voice message when an incoming call does not get answered in a pre-determined amount of time. In the Skype Conversation API, voicemail does not depend on a call going unanswered - you can post a voice message asynchronously into any dialog conversation at any time. 
    
    In fact, a high-level action flow directing unanswered incoming live sessions to voicemail is not something provided by the Conversation API - implementation of this use case is largely up to your UI. 
    
    The fact that your UI must retrieve incoming Voicemails by monitoring changes to a Conversation instance's Messages illustrates this conceptual difference between traditional voicemail and voice messages. The message type Message::POSTED_VOICE_MESSAGE indicates that a Message instance should be handled as a voice message instead of by displaying its body text in the Conversation UI. Message::GetVoiceMessage enables you to retrieve the associated Voicemail instance; Voicemail::StartPlayback enables you to listen to the message audio. 
    
    To put it another way, the object chain goes like this: 
    ::
      Contact->Conversation->Message->Voicemail 
     
    
    There are three basic types of Voicemail objects: 
      - INCOMING - received voice messages that can be retrieved from Message objects; 
      - OUTGOING - outbound voice messages that can be created with Conversation::StartVoiceMessage; 
      - GREETING - voice messages that represent auto-answer greetings, either recorded by the user (CUSTOM_GREETING) or included as part of SkypeKit (DEFAULT_GREETING). This is the audio that gets played back to sender of the voice message before he can start recording his voice message. 
    
    Before trying to send out a voicemail, you should ensure that target Contact has the capability to receive them. Use Contact::GetCapabilityStatus to check for Contact::CAPABILITY_CAN_BE_SENT_VM. 
    
    Recording and Sending a Voice Message 
    
    The first step is to obtain a dialog Conversation with the target Contact. In that conversation, you can initiate the outgoing voice message with Conversation::StartVoiceMessage 
    
    Note that this call does not return an actual Voicemail object. To catch when an object gets created, you will need to check Conversation::P_ACTIVE_VM_ID property updates. 
    
    After invoking Conversation::StartVoiceMessage, SkypeKit instantiates a Voicemail instance for the target Contact's greeting (with type CUSTOM_GREETING or DEFAULT_GREETING). At this point, the Conversation::P_ACTIVE_VM_ID property update fires, newVM contains a reference to the greeting, and playback of the greeting for the sender starts automatically. 
    
    Once the greeting playback finishes, SkypeKit instantiates a second Voicemail instance for the outgoing voice message. At this point, the Conversation::P_ACTIVE_VM_ID property update fires again, newVM now contains a reference to the outgoing message, and recording starts automatically. If you want to include notification and/or error handling for whether this message was sent successfully, you should make a copy of newVM now. 
    
    Once the user finishes (or abandons) recording of their message, they want to either send the message or to cancel it. To send the message, use Conversation::PostVoiceMessage; to cancel the message, use Conversation::LeaveLiveSession. 
    
    Both of these actions results in the Conversation::P_ACTIVE_VM_ID property update firing for a third time, setting the value to NULL. However, the Voicemail object will actually continue its existence past this point. Saving a reference to the message's Voicemail object when you start recording it enables you to keep receiving Voicemail property updates. This in turn enables your UI to check whether voice message send succeeded or failed. 
    
    The relevant terminal state Voicemail::P_STATUS property values are: 
      - Voicemail::CANCELLED - recording and/or sending of this message was cancelled 
      - Voicemail::UPLOADED - message sent 
      - Voicemail::FAILED - message could not be sent 
    
    Receiving and Playing Back a Voice Message 
    
    On the remote side, the Voicemail appears as a Message object of type Message::POSTED_VOICE_MESSAGE. The message's author property contains the Skype Name of the Voicemail originator, and its BodyXml property contains the message length and title text in following format: 
    
    ::
      <voicemail alt="Sent voicemail to people in this conversation."><message length="5" ></message></voicemail> 
     
    
    Receiver side UI can then retrieve the Voicemail object from the message with Message::GetVoiceMessage and 
    start local playback with Message::StartPlayback. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "Voicemail %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(Voicemail, "P_"+propname.upper())
    module_id = 7
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    TYPE = {1:'INCOMING', 'INCOMING':1, 4:'DEFAULT_GREETING', 'DEFAULT_GREETING':4, 2:'CUSTOM_GREETING', 'CUSTOM_GREETING':2, 3:'OUTGOING', 'OUTGOING':3}
    """
     - INCOMING
     - DEFAULT_GREETING
     - CUSTOM_GREETING
     - OUTGOING
    """
    STATUS = {
        1 :'NOTDOWNLOADED',
        2 :'DOWNLOADING',
        3 :'UNPLAYED',
        4 :'BUFFERING',
        5 :'PLAYING',
        6 :'PLAYED',
        7 :'BLANK',
        8 :'RECORDING',
        9 :'RECORDED',
        10:'UPLOADING',
        11:'UPLOADED',
        12:'DELETING',
        13:'FAILED',
        14:'DELETING_FAILED',
        15:'CHECKING',
        16:'CANCELLED',
        'NOTDOWNLOADED'
        
                
             : 1,
        'DOWNLOADING'
        
                
               : 2,
        'UNPLAYED'
        
                
                  : 3,
        'BUFFERING'
        
                
                 : 4,
        'PLAYING'
        
                
                   : 5,
        'PLAYED'
        
                
                    : 6,
        'BLANK'
        
                
                     : 7,
        'RECORDING'
        
                
                 : 8,
        'RECORDED'
        
                
                  : 9,
        'UPLOADING'
        
                
                 :10,
        'UPLOADED'
        
                
                  :11,
        'DELETING'
        
                
                  :12,
        'FAILED'
        
                
                    :13,
        'DELETING_FAILED'
        
                
           :14,
        'CHECKING'
        
                
                  :15,
        'CANCELLED'
        
                
                 :16
    }
    """
     - NOTDOWNLOADED
     - DOWNLOADING
     - UNPLAYED
     - BUFFERING
     - PLAYING
     - PLAYED
     - BLANK
     - RECORDING
     - RECORDED
     - UPLOADING
     - UPLOADED
     - DELETING
     - FAILED
     - DELETING_FAILED
     - CHECKING
     - CANCELLED
    """
    FAILUREREASON = {
        1 :'MISC_ERROR',
        2 :'CONNECT_ERROR',
        3 :'NO_VOICEMAIL_CAPABILITY',
        4 :'NO_SUCH_VOICEMAIL',
        5 :'FILE_READ_ERROR',
        6 :'FILE_WRITE_ERROR',
        7 :'RECORDING_ERROR',
        8 :'PLAYBACK_ERROR',
        9 :'NO_PERMISSION',
        10:'RECEIVER_DISABLED_VOICEMAIL',
        11:'SENDER_NOT_AUTHORIZED',
        12:'SENDER_BLOCKED',
        'MISC_ERROR'
        
                
                : 1,
        'CONNECT_ERROR'
        
                
             : 2,
        'NO_VOICEMAIL_CAPABILITY'
        
                   : 3,
        'NO_SUCH_VOICEMAIL'
        
                
         : 4,
        'FILE_READ_ERROR'
        
                
           : 5,
        'FILE_WRITE_ERROR'
        
                
          : 6,
        'RECORDING_ERROR'
        
                
           : 7,
        'PLAYBACK_ERROR'
        
                
            : 8,
        'NO_PERMISSION'
        
                
             : 9,
        'RECEIVER_DISABLED_VOICEMAIL'
        
               :10,
        'SENDER_NOT_AUTHORIZED'
        
                     :11,
        'SENDER_BLOCKED'
        
                
            :12
    }
    """
     - MISC_ERROR
     - CONNECT_ERROR
     - NO_VOICEMAIL_CAPABILITY
     - NO_SUCH_VOICEMAIL
     - FILE_READ_ERROR
     - FILE_WRITE_ERROR
     - RECORDING_ERROR
     - PLAYBACK_ERROR
     - NO_PERMISSION
     - RECEIVER_DISABLED_VOICEMAIL - receiver turned off voicemail
     - SENDER_NOT_AUTHORIZED - receiver has not authorized you and privacy is not set to anyone
     - SENDER_BLOCKED - receiver blocked sender
    """
    
    def _sk_get_type(self):
        return skypekit.enumof(Voicemail.TYPE, self._sk_property("ZGd]\007", 100, True))
    type = property(_sk_get_type)
    propid2label[100] = "type"
    P_TYPE = 100
    def _sk_get_partner_handle(self):
        """
        registered username of the other party
        """
        return self._sk_property("ZGe]\007", 101, True)
    partner_handle = property(_sk_get_partner_handle)
    propid2label[101] = "partner_handle"
    P_PARTNER_HANDLE = 101
    def _sk_get_partner_dispname(self):
        """
        user's display name of the other party
        """
        return self._sk_property("ZGf]\007", 102, True)
    partner_dispname = property(_sk_get_partner_dispname)
    propid2label[102] = "partner_dispname"
    P_PARTNER_DISPNAME = 102
    def _sk_get_status(self):
        return skypekit.enumof(Voicemail.STATUS, self._sk_property("ZGg]\007", 103, True))
    status = property(_sk_get_status)
    propid2label[103] = "status"
    P_STATUS = 103
    def _sk_get_failurereason(self):
        return skypekit.enumof(Voicemail.FAILUREREASON, self._sk_property("ZGh]\007", 104, True))
    failurereason = property(_sk_get_failurereason)
    propid2label[104] = "failurereason"
    P_FAILUREREASON = 104
    def _sk_get_subject(self):
        """
        DEPRECATED: subject line
        """
        return self._sk_property("ZGi]\007", 105, True)
    subject = property(_sk_get_subject)
    propid2label[105] = "subject"
    P_SUBJECT = 105
    def _sk_get_timestamp(self):
        """
        timestamp of creation
        """
        return self._sk_property("ZGj]\007", 106, True)
    timestamp = property(_sk_get_timestamp)
    propid2label[106] = "timestamp"
    P_TIMESTAMP = 106
    def _sk_get_duration(self):
        """
        duration in seconds
        """
        return self._sk_property("ZGk]\007", 107, True)
    duration = property(_sk_get_duration)
    propid2label[107] = "duration"
    P_DURATION = 107
    def _sk_get_allowed_duration(self):
        """
        max allowed duration in seconds
        """
        return self._sk_property("ZGl]\007", 108, True)
    allowed_duration = property(_sk_get_allowed_duration)
    propid2label[108] = "allowed_duration"
    P_ALLOWED_DURATION = 108
    def _sk_get_playback_progress(self):
        """
        VM playback progress in seconds
        """
        return self._sk_property("ZGm]\007", 109, True)
    playback_progress = property(_sk_get_playback_progress)
    propid2label[109] = "playback_progress"
    P_PLAYBACK_PROGRESS = 109
    def _sk_get_convo_id(self):
        """
        CONVERSATION_ID of corresponding conversation
        """
        return module_id2classes[18](self._sk_property("ZG\276\006]\007",
  830, True), self.transport)
    convo_id = property(_sk_get_convo_id)
    propid2label[830] = "convo_id"
    P_CONVO_ID = 830
    def _sk_get_chatmsg_guid(self):
        """
        GUID of the message that the VM is tied to
        """
        return self._sk_property("ZG\277\006]\007", 831, True)
    chatmsg_guid = property(_sk_get_chatmsg_guid)
    propid2label[831] = "chatmsg_guid"
    P_CHATMSG_GUID = 831
    
    def StartRecording(self):
        """
        Start recording your own auto-answer greeting message (leave message after the beep...) only. Recording of outgoing Voicemail messages start automatically (using Conversation::StartVoiceMessage) after playback of the remote side greeting message has finished. 
        
        """
        l_request = skypekit.XCallRequest("ZR\007\003", 7, 3)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def StopRecording(self):
        """
        Stop recording of your own auto-answer greeting message only. To stop recording of and send an outgoing Voicemail, use Conversation::PostVoiceMessage. 
        
        """
        l_request = skypekit.XCallRequest("ZR\007\004", 7, 4)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def StartPlayback(self):
        """
        Initiates playback of a voice message 
        
        """
        l_request = skypekit.XCallRequest("ZR\007\005", 7, 5)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def StopPlayback(self):
        """
        Terminates playback of a voice message 
        
        """
        l_request = skypekit.XCallRequest("ZR\007\006", 7, 6)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def Delete(self):
        """
        first from server, and then the local copy
        """
        l_request = skypekit.XCallRequest("ZR\007\007", 7, 7)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def Cancel(self):
        """
        Canceling recording of your own auto-answer greeting message. To stop recording of and cancel an outgoing Voicemail, use Conversation::LeaveLiveSession. 
        
        """
        l_request = skypekit.XCallRequest("ZR\007\010", 7, 8)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def CheckPermission(self):
        """
        check if we can send voicemail (unauth,blocked,no priv etc cases). only OUTGOING
        
        B{Return values:}
         - B{result}
        """
        l_request = skypekit.XCallRequest("ZR\007\015", 7, 13)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
module_id2classes[7] = Voicemail

class Sms(skypekit.Object):
    """
    Wrapper class that includes SMS-specific properties and methods, such as P_BODY and GetTargetPrice. Instantiate SMS instances using Skype::CreateOutgoingSms; post SMS messages to a Conversation using Conversation::PostSMS. 
    
    Each SMS can have multiple targets (normalized phone numbers). Note that in a Conversation context, every SMS instance has a corresponding Message instance. Once you've posted an SMS, you can retrieve its corresponding Message instance using Sms::GetPropChatmsgId. That Message instance's P_BODY_XML property contains the SMS message data, such as price, target phone number(s), failure codes, and so forth, which you can parsed out and display in the UI. To put it another way, the object chain goes like this: 
    
    ::
      Conversation->Message->SMS 
     
    
    Note that SkypeKit SDK supports outgoing SMS messages only. SkypeKit clients, even when logged in with accounts that have SkypeIn numbers, cannot receive SMS messages. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "Sms %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(Sms, "P_"+propname.upper())
    module_id = 12
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    TYPE = {2:'OUTGOING', 'OUTGOING':2, 3:'CONFIRMATION_CODE_REQUEST', 'CONFIRMATION_CODE_REQUEST':3, 4:'CONFIRMATION_CODE_SUBMIT', 'CONFIRMATION_CODE_SUBMIT':4}
    """
     - OUTGOING - a normal outgoing SMS message
     - CONFIRMATION_CODE_REQUEST - a message requesting a SMS confirmation code be sent to the number provided
     - CONFIRMATION_CODE_SUBMIT - a message returning the SMS confirmation code received as a result of a CONFIRMATION_CODE_REQUEST to authorize it
    """
    STATUS = {3:'COMPOSING', 'COMPOSING':3, 4:'SENDING_TO_SERVER', 'SENDING_TO_SERVER':4, 5:'SENT_TO_SERVER', 'SENT_TO_SERVER':5, 6:'DELIVERED', 'DELIVERED':6, 7:'SOME_TARGETS_FAILED', 'SOME_TARGETS_FAILED':7, 8:'FAILED', 'FAILED':8}
    """
     - COMPOSING
     - SENDING_TO_SERVER
     - SENT_TO_SERVER
     - DELIVERED
     - SOME_TARGETS_FAILED
     - FAILED
    """
    FAILUREREASON = {
        1:'MISC_ERROR',
        2:'SERVER_CONNECT_FAILED',
        3:'NO_SMS_CAPABILITY',
        4:'INSUFFICIENT_FUNDS',
        5:'INVALID_CONFIRMATION_CODE',
        6:'USER_BLOCKED',
        7:'IP_BLOCKED',
        8:'NODE_BLOCKED',
        9:'NO_SENDERID_CAPABILITY',
        'MISC_ERROR'
        
                
                :1,
        'SERVER_CONNECT_FAILED'
        
                     :2,
        'NO_SMS_CAPABILITY'
        
                
         :3,
        'INSUFFICIENT_FUNDS'
        
                
        :4,
        'INVALID_CONFIRMATION_CODE'
        
                 :5,
        'USER_BLOCKED'
        
                
              :6,
        'IP_BLOCKED'
        
                
                :7,
        'NODE_BLOCKED'
        
                
              :8,
        'NO_SENDERID_CAPABILITY'
        
                    :9
    }
    """
     - MISC_ERROR
     - SERVER_CONNECT_FAILED
     - NO_SMS_CAPABILITY
     - INSUFFICIENT_FUNDS
     - INVALID_CONFIRMATION_CODE
     - USER_BLOCKED
     - IP_BLOCKED
     - NODE_BLOCKED
     - NO_SENDERID_CAPABILITY
    """
    TARGETSTATUS = {1:'TARGET_ANALYZING', 'TARGET_ANALYZING':1, 2:'TARGET_UNDEFINED', 'TARGET_UNDEFINED':2, 3:'TARGET_ACCEPTABLE', 'TARGET_ACCEPTABLE':3, 4:'TARGET_NOT_ROUTABLE', 'TARGET_NOT_ROUTABLE':4, 5:'TARGET_DELIVERY_PENDING', 'TARGET_DELIVERY_PENDING':5, 6:'TARGET_DELIVERY_SUCCESSFUL', 'TARGET_DELIVERY_SUCCESSFUL':6, 7:'TARGET_DELIVERY_FAILED', 'TARGET_DELIVERY_FAILED':7}
    """
     - TARGET_ANALYZING
     - TARGET_UNDEFINED
     - TARGET_ACCEPTABLE
     - TARGET_NOT_ROUTABLE
     - TARGET_DELIVERY_PENDING
     - TARGET_DELIVERY_SUCCESSFUL
     - TARGET_DELIVERY_FAILED
    """
    SETBODYRESULT = {0:'BODY_INVALID', 'BODY_INVALID':0, 1:'BODY_TRUNCATED', 'BODY_TRUNCATED':1, 2:'BODY_OK', 'BODY_OK':2, 3:'BODY_LASTCHAR_IGNORED', 'BODY_LASTCHAR_IGNORED':3}
    """
     - BODY_INVALID - body not set. message status wrong or invalid, or body not valid utf8 string
     - BODY_TRUNCATED - body too long. set, but truncated. charsUntilNextChunk contains maxChunks value
     - BODY_OK - body was set OK
     - BODY_LASTCHAR_IGNORED - last unicode char was ignored, as some of the text would be deleted due to conversion
    """
    CONFIRM_TYPE = {1:'ID_SMS', 'ID_SMS':1, 2:'ID_MOBILE', 'ID_MOBILE':2, 3:'ID_SKYPEIN', 'ID_SKYPEIN':3}
    """
     - ID_SMS - Confirm mobile number as SMS sender number
     - ID_MOBILE - Confirm mobile number as CLI for SkypeOut calls
     - ID_SKYPEIN - unused currently
    """
    
    def _sk_get_type(self):
        return skypekit.enumof(Sms.TYPE, self._sk_property("ZG\276\001]\014", 190, True))
    type = property(_sk_get_type)
    propid2label[190] = "type"
    P_TYPE = 190
    def _sk_get_status(self):
        return skypekit.enumof(Sms.STATUS, self._sk_property("ZG\277\001]\014", 191, True))
    status = property(_sk_get_status)
    propid2label[191] = "status"
    P_STATUS = 191
    def _sk_get_failurereason(self):
        """
        Set asynchronously and meaningful only after invoking Conversation::PostSMS and detecting Sms::STATUS of SOME_TARGETS_FAILED or FAILED. 
        
        """
        return skypekit.enumof(Sms.FAILUREREASON, self._sk_property("ZG\300\001]\014", 192, True))
    failurereason = property(_sk_get_failurereason)
    propid2label[192] = "failurereason"
    P_FAILUREREASON = 192
    def _sk_get_is_failed_unseen(self):
        """
        set to 1 when status goes to FAILED. use MarkSeen() to clear
        """
        return self._sk_property("ZG0]\014", 48, True)
    is_failed_unseen = property(_sk_get_is_failed_unseen)
    propid2label[48] = "is_failed_unseen"
    P_IS_FAILED_UNSEEN = 48
    def _sk_get_timestamp(self):
        """
        unix timestamp of message submission
        """
        return self._sk_property("ZG\306\001]\014", 198, True)
    timestamp = property(_sk_get_timestamp)
    propid2label[198] = "timestamp"
    P_TIMESTAMP = 198
    def _sk_get_price(self):
        """
        The total price of sending this SMS message (sum of the individual prices to send to each recipient). Defaults to -1 on instantiation and incremented by the price for each recipient once that recipient's status reflects TARGET_ACCEPTABLE. Use Sms::GetTargetPrice to retrieve individual target prices. 
        
        A value of MAX_UINT indicates that SkypeKit is actively querying and/or updating the value. Note that P_PRICE is an integer value. Calculate the actual price (in units specified by P_PRICE_CURRENCY) using P_PRICE_PRECISION as: 
        
        ::
          actualPrice = price / 10^pricePrecision; 
         
        
        """
        return self._sk_property("ZG\301\001]\014", 193, True)
    price = property(_sk_get_price)
    propid2label[193] = "price"
    P_PRICE = 193
    def _sk_get_price_precision(self):
        """
        The decimal precision of the SMS price values, both individual and total. For example, a value of 2 indicates that you should divide the price (represented as an integer) by 100 (10^2) to obtain the actual price. 
        
        """
        return self._sk_property("ZG1]\014", 49, True)
    price_precision = property(_sk_get_price_precision)
    propid2label[49] = "price_precision"
    P_PRICE_PRECISION = 49
    def _sk_get_price_currency(self):
        """
        should be same as account currency at the time of composing/sending
        """
        return self._sk_property("ZG\302\001]\014", 194, True)
    price_currency = property(_sk_get_price_currency)
    propid2label[194] = "price_currency"
    P_PRICE_CURRENCY = 194
    def _sk_get_reply_to_number(self):
        """
        number that should receive the replies
        """
        return self._sk_property("ZG\307\001]\014", 199, True)
    reply_to_number = property(_sk_get_reply_to_number)
    propid2label[199] = "reply_to_number"
    P_REPLY_TO_NUMBER = 199
    def _sk_get_target_numbers(self):
        """
        space-separated normalised pstn numbers
        """
        return self._sk_property("ZG\303\001]\014", 195, True)
    target_numbers = property(_sk_get_target_numbers)
    propid2label[195] = "target_numbers"
    P_TARGET_NUMBERS = 195
    def _sk_get_target_statuses(self):
        """
        binary blob. track with OnPropertyChange(), access with GetTargetStatus(target)
        """
        return self._sk_property("ZG\304\001]\014", 196, True)
    target_statuses = property(_sk_get_target_statuses)
    propid2label[196] = "target_statuses"
    P_TARGET_STATUSES = 196
    def _sk_get_body(self):
        """
        actual payload
        """
        return self._sk_property("ZG\305\001]\014", 197, True)
    body = property(_sk_get_body)
    propid2label[197] = "body"
    P_BODY = 197
    def _sk_get_chatmsg_id(self):
        """
        reference to Message
        """
        return module_id2classes[9](self._sk_property("ZG\310\006]\014",
  840, True), self.transport)
    chatmsg_id = property(_sk_get_chatmsg_id)
    propid2label[840] = "chatmsg_id"
    P_CHATMSG_ID = 840
    
    def GetTargetStatus(
        self,
        target
        ):
        """
        Retrieves the send status of this SMS to a particular recipient (P_TARGET_STATUSES) either prior to or after invoking Conversation::PostSMS. 
        
        
        B{Arguments:}
         - B{target} - The normalized phone number of the target recipient. 
        
        
        B{Return values:}
         - B{status} - The send status of the target recipient, for example, TARGET_ANALYZING, TARGET_DELIVERY_PENDING, TARGET_DELIVERY_SUCCESSFUL, TARGET_DELIVERY_FAILED, and so forth. TARGET_UNDEFINED implies that the specified target is not a recipient of this SMS. 
        
        """
        l_request = skypekit.XCallRequest("ZR\014\004", 12, 4)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, target)
        l_response = self.transport.xcall(l_request)
        l_result = skypekit.enumof(Sms.TARGETSTATUS, l_response.get(1))
        return l_result
    def GetTargetPrice(
        self,
        target
        ):
        """
        Retrieves the amount of Skype credit necessary to send the SMS to a particular recipient. Defaults to -1 on instantiation and set only when that recipient's status reflects TARGET_ACCEPTABLE. Use Sms::GetPropPrice to retrieve the total cost of this SMS. 
        
        Note that the target price is an integer value. Calculate the actual price (in units specified by P_PRICE_CURRENCY) using P_PRICE_PRECISION as: 
        ::
          actualTargetPrice = targetPrice / 10^pricePrecision; 
         
        
        
        B{Arguments:}
         - B{target} - The normalized phone number of the target recipient. 
        
        
        B{Return values:}
         - B{price} - The price of sending this SMS message to the target recipient. 
        
        """
        l_request = skypekit.XCallRequest("ZR\014\015", 12, 13)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, target)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, 0)
        return l_result
    def SetTargets(
        self,
        numbers
        ):
        """
        Sets the recipient(s) of this SMS. Note that each invocation replaces the target list and re-calculates all prices - they are not additive! 
        
        
        B{Arguments:}
         - B{numbers} - Normalized phone number(s) of the intended recipient(s). 
        
        
        B{Return values:}
         - B{success} - Set to true if the target list appears to contain valid, normalized telephone numbers. Note that this check is not very reliable. Actual target validity checking occurs asynchronously in the background, and manifests itself as a series of Sms::P_TARGET_STATUSES property change events. 
        
        """
        l_request = skypekit.XCallRequest("ZR\014\006", 12, 6)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, numbers)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def SetBody(
        self,
        text
        ):
        """
        -The- method for setting the body text of this SMS. While Conversation::PostSMS does have a body argument, that argument is currently unused. 
        
        
        B{Arguments:}
         - B{text} - Message body text. 
        
        
        B{Return values:}
         - B{result} - Whether the Message body was successfully set and if not, why not. 
        
         - B{chunks} - The Message body as a list of individual chunks. 
        
         - B{chars_until_next_chunk} - Number of available characters until creation of the next chunk becomes necessary. 
        
        """
        l_request = skypekit.XCallRequest("ZR\014\007", 12, 7)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, text)
        l_response = self.transport.xcall(l_request)
        l_result = (skypekit.enumof(Sms.SETBODYRESULT, l_response.get(1))),
        l_result += (l_response.get(2, [])),
        l_result += (l_response.get(3, 0)),
        return l_result
    def GetBodyChunks(self):
        """
        Retrieves string list of SMS text chunks in first argument, while the second argument contains the number of available characters until creation of the next chunk becomes necessary. 
        
        
        B{Return values:}
         - B{text_chunks} - List of text chunk strings 
        
         - B{chars_until_next_chunk} - Number of available characters until creation of the next chunk becomes necessary. 
        
        """
        l_request = skypekit.XCallRequest("ZR\014\010", 12, 8)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, [])),
        l_result += (l_response.get(2, 0)),
        return l_result
module_id2classes[12] = Sms

class Transfer(skypekit.Object):
    """
    Transfer in this context refers to transferring (sending/receiving) files among Skype Contacts, not transferring a call to another Skype or PSTN Contact. This class includes file transfer-specific properties and methods, such as FILESIZE, BYTESPERSECOND, Pause and Resume. Recipients of these file transfers must explicitly accept (or decline) any incoming transfer. Transfer instances represent files being sent and received within a Conversation context. Each Transfer instance represents a single file transfer - if a conversation has multiple remote participants, a separate Transfer instance must be instantiated for each remote participant (a Transfer instance is not instantiated for the sender). 
    
    Transfer instances cannot be instantiated directly. Instead, you initiate a file transfer by invoking Conversation::PostFiles. This instantiates a Message instance of type POSTED_FILES, which is added to the Conversation for all the participants (including the sender). The Transfer instance is associated with 
    this Message instance, and the Message::P_BODY_XML looks like this: 
    
    ::
      Some text<files alt=""><file size="2336020" index="0">test.zip</file></files> 
     
    
    To put it another way, the object chain goes like this: 
    ::
      Conversation->Message->Transfer 
     
    
    The first part of the message (before the files section) comes from the Conversation::PostFiles body argument. For each file in the message, a file section is provided with three fields: 
      - file size in bytes 
      - index - more or less arbitrary order in which the transfers should be displayed in the UI 
      - file name. 
    
    For practical purposes, the Message::P_BODY_XML property is not all that useful in this context. The Transfer instances, however, contain the state and progress feedback for your UI. You retrieve these Transfer instances using Message::GetTransfers method. Since the sender can post multiple files to 
    a Conversation using the same Message, Message:GetTransfers retrieves a list of Transfer instances - one per file per recipient. 
    
    You can determine the direction of particular Transfer instance by querying Transfer::P_TYPE (INCOMING/OUTGOING). 
    
    You can implement a progress indicator by monitoring Transfer::P_BYTESTRANSFERRED. Note that when testing this on your local network, you will most likely catch these property change events at the beginning and the end of the transfer only - which does not look like too useful. However, for non-local network transfers where the transfer speeds are in the neighborhood of 200-300 KB per second, you should consider implementing progress feedback as being mandatory and expect to catch multiple property change events for all but the smallest files. 
    
    Another property of interest is Transfer::P_STATUS. The normal transfer status sequence during successful outgoing transfer is this: 
      - TRANSFER STATUS -> NEW 
      - TRANSFER STATUS -> WAITING_FOR_ACCEPT 
      - TRANSFER STATUS -> CONNECTING 
      - TRANSFER STATUS -> TRANSFERRING 
      - TRANSFER STATUS -> CONNECTING 
      - TRANSFER STATUS -> COMPLETED 
    
    The list of all terminal Transfer statuses is: 
      - COMPLETED 
      - FAILED 
      - CANCELLED 
      - CANCELLED_BY_REMOTE 
    
    In-progress transfers can be canceled with Transfer::Cancel and paused/resumed with Transfer::Pause and Transfer::Resume. For transfers that complete with a status of FAILED, your UI should provide feedback based on the value of Transfer::P_FAILUREREASON. 
    
    Incoming transfers, once accepted, overwrite existing files with the same name. Before accepting an incoming file transfer, 
    your UI should prompt the recipient to: 
      - accept or decline the file 
      - if accepted, specify the directory of here to save the file (with a pre-filled default) 
      - if accepted and a file having the same name already exists at the specified destination, your UI should prompt for confirmation to overwrite and provide a way to alter the file name before accepting it 
    
    Similarly, your UI should verify the existence of outgoing files prior to invoking Conversation::PostFiles. 
    
    Note that you should provide both Conversation::PostFiles and Transfer::Accept methods fully-qualified paths. Otherwise, the paths will be assumed to be relative to the path of SkypeKit runtime, since the methods are actually executed in the runtime context. 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "Transfer %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(Transfer, "P_"+propname.upper())
    module_id = 6
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    TYPE = {1:'INCOMING', 'INCOMING':1, 2:'OUTGOING', 'OUTGOING':2}
    """
     - INCOMING
     - OUTGOING
    """
    STATUS = {
        0 :'NEW',
        1 :'CONNECTING',
        2 :'WAITING_FOR_ACCEPT',
        3 :'TRANSFERRING',
        4 :'TRANSFERRING_OVER_RELAY',
        5 :'PAUSED',
        6 :'REMOTELY_PAUSED',
        7 :'CANCELLED',
        8 :'COMPLETED',
        9 :'FAILED',
        10:'PLACEHOLDER',
        11:'OFFER_FROM_OTHER_INSTANCE',
        12:'CANCELLED_BY_REMOTE',
        'NEW'
        
                
                       : 0,
        'CONNECTING'
        
                
                : 1,
        'WAITING_FOR_ACCEPT'
        
                
        : 2,
        'TRANSFERRING'
        
                
              : 3,
        'TRANSFERRING_OVER_RELAY'
        
                   : 4,
        'PAUSED'
        
                
                    : 5,
        'REMOTELY_PAUSED'
        
                
           : 6,
        'CANCELLED'
        
                
                 : 7,
        'COMPLETED'
        
                
                 : 8,
        'FAILED'
        
                
                    : 9,
        'PLACEHOLDER'
        
                
               :10,
        'OFFER_FROM_OTHER_INSTANCE'
        
                 :11,
        'CANCELLED_BY_REMOTE'
        
                       :12
    }
    """
    Recognized values for the P_STATUS property. Reflects current state of this Transfer. 
    
     - NEW - The file has either not been posted (sent) (OUTGOING), or not accepted (received) (INCOMING). 
    
     - CONNECTING - A temporary state that transitions either into TRANSFERRING (relayed or direct) or to FAILED. For unknown reasons, outgoing transfers tend go into this state twice - immediately before the actual data transfer starts and immediately after it ends. 
    
     - WAITING_FOR_ACCEPT - The files have been posted but the recipient has not yet accepted (or has declined) the transfer. 
    
     - TRANSFERRING - The transfer has been accepted and file data is being sent/received. Periodic updates of P_BYTESTRANSFERRED property should occur. 
    
     - TRANSFERRING_OVER_RELAY - The transfer has been accepted and file data is being sent/received but is going over at least one relay. Since relayed transfers tend to be significantly slower than direct transfers, you might want to differentiate the two in your UI and notify the user that relayed transfer typically take significantly longer to finish. 
    
     - PAUSED - The local user (either the sender or a receiver) has paused the transfer. 
    
     - REMOTELY_PAUSED - A remote user has paused the transfer. For senders, a receiver has paused the transfer; for receivers, the sender has paused the transfer. 
    
     - CANCELLED - Local side (either sender or receiver) has canceled the transfer. This is a final state of the STATE property. 
    
     - COMPLETED - File transfer has completed. This is a terminal state. 
    
     - FAILED - File transfer has failed. This is a terminal state. UI should provide feedback, based on value of P_FAILUREREASON. 
    
     - PLACEHOLDER - Transfer whose existence has been hinted by corresponding chat message, but which is yet to arrive. 
    
     - OFFER_FROM_OTHER_INSTANCE - Outgoing transfer object from another instance of the same account as current local login, running on another system. Hinted through chat message - only implies an offer was made; not necessarily accepted, failed, or completed. 
    
     - CANCELLED_BY_REMOTE - Remote side (either sender or receiver) has canceled the transfer. This is a final state of the STATE property. 
    
    """
    FAILUREREASON = {
        1 :'SENDER_NOT_AUTHORISED',
        2 :'REMOTELY_CANCELLED',
        3 :'FAILED_READ',
        4 :'FAILED_REMOTE_READ',
        5 :'FAILED_WRITE',
        6 :'FAILED_REMOTE_WRITE',
        7 :'REMOTE_DOES_NOT_SUPPORT_FT',
        8 :'REMOTE_OFFLINE_FOR_TOO_LONG',
        9 :'TOO_MANY_PARALLEL',
        10:'PLACEHOLDER_TIMEOUT',
        'SENDER_NOT_AUTHORISED'
        
                     : 1,
        'REMOTELY_CANCELLED'
        
                
        : 2,
        'FAILED_READ'
        
                
               : 3,
        'FAILED_REMOTE_READ'
        
                
        : 4,
        'FAILED_WRITE'
        
                
              : 5,
        'FAILED_REMOTE_WRITE'
        
                       : 6,
        'REMOTE_DOES_NOT_SUPPORT_FT'
        
                : 7,
        'REMOTE_OFFLINE_FOR_TOO_LONG'
        
               : 8,
        'TOO_MANY_PARALLEL'
        
                
         : 9,
        'PLACEHOLDER_TIMEOUT'
        
                       :10
    }
    """
     - SENDER_NOT_AUTHORISED
     - REMOTELY_CANCELLED
     - FAILED_READ
     - FAILED_REMOTE_READ
     - FAILED_WRITE
     - FAILED_REMOTE_WRITE
     - REMOTE_DOES_NOT_SUPPORT_FT
     - REMOTE_OFFLINE_FOR_TOO_LONG
     - TOO_MANY_PARALLEL
     - PLACEHOLDER_TIMEOUT
    """
    
    def _sk_get_type(self):
        """
        INCOMING / OUTGOING 
        
        """
        return skypekit.enumof(Transfer.TYPE, self._sk_property("ZGP]\006", 80, True))
    type = property(_sk_get_type)
    propid2label[80] = "type"
    P_TYPE = 80
    def _sk_get_partner_handle(self):
        """
        Skype Name of the remote party of the file transfer. If a file is posted in a conversation with more than one participant, Transfer objects are created for each of them - so a transfer is always to one single remote target. 
        
        """
        return self._sk_property("ZGQ]\006", 81, True)
    partner_handle = property(_sk_get_partner_handle)
    propid2label[81] = "partner_handle"
    P_PARTNER_HANDLE = 81
    def _sk_get_partner_dispname(self):
        """
        Display name of the remote participant. 
        
        """
        return self._sk_property("ZGR]\006", 82, True)
    partner_dispname = property(_sk_get_partner_dispname)
    propid2label[82] = "partner_dispname"
    P_PARTNER_DISPNAME = 82
    def _sk_get_status(self):
        """
        Current state of the transfer 
        
        """
        return skypekit.enumof(Transfer.STATUS, self._sk_property("ZGS]\006", 83, True))
    status = property(_sk_get_status)
    propid2label[83] = "status"
    P_STATUS = 83
    def _sk_get_failurereason(self):
        """
        Set whenever P_STATUS transitions to FAILED. 
        
        """
        return skypekit.enumof(Transfer.FAILUREREASON, self._sk_property("ZGT]\006", 84, True))
    failurereason = property(_sk_get_failurereason)
    propid2label[84] = "failurereason"
    P_FAILUREREASON = 84
    def _sk_get_starttime(self):
        """
        UNIX timestamp of when this Transfer instance was instantiated, not when the transfer process actually started (was accepted from receiver side). Do not use this property when calculate the data transfer speed! Instead, monitor changes to P_BYTESPERSECOND. 
        
        """
        return self._sk_property("ZGU]\006", 85, True)
    starttime = property(_sk_get_starttime)
    propid2label[85] = "starttime"
    P_STARTTIME = 85
    def _sk_get_finishtime(self):
        """
        UNIX timestamp of when this Transfer COMPLETED or FAILED. This property is never set if the receiving side (local or remote) canceled the transfer. 
        
        """
        return self._sk_property("ZGV]\006", 86, True)
    finishtime = property(_sk_get_finishtime)
    propid2label[86] = "finishtime"
    P_FINISHTIME = 86
    def _sk_get_filepath(self):
        """
        The path -and- filename of the file being transfered (typically fully qualified). For the receiver, SkypeKit sets this property upon acceptance of the incoming transfer. If not fully qualified, the path is assumed to be relative to the path of the SkypeKit runtime. 
        
        """
        return self._sk_property("ZGW]\006", 87, True)
    filepath = property(_sk_get_filepath)
    propid2label[87] = "filepath"
    P_FILEPATH = 87
    def _sk_get_filename(self):
        """
        The filename -only- of the file being transfered. The receiver side can use this property to pre-populate relevant UI components while prompting the user to accept the incoming transfer. 
        
        """
        return self._sk_property("ZGX]\006", 88, True)
    filename = property(_sk_get_filename)
    propid2label[88] = "filename"
    P_FILENAME = 88
    def _sk_get_filesize(self):
        """
        The size of the file being transferred in bytes. Depending on the magnitude of this value, your UI might want to display the size in terms of kilobytes or even megabytes. 
        
        """
        return self._sk_property("ZGY]\006", 89, True)
    filesize = property(_sk_get_filesize)
    propid2label[89] = "filesize"
    P_FILESIZE = 89
    def _sk_get_bytestransferred(self):
        """
        The number of bytes already transferred. Calculate the percentage of the file transferred so far as: 
        ::
          P_BYTESTRANSFERRED / (P_FILESIZE / 100); 
         
        
        Use float variables to avoid problems with files smaller than 100 bytes! 
        
        """
        return self._sk_property("ZGZ]\006", 90, True)
    bytestransferred = property(_sk_get_bytestransferred)
    propid2label[90] = "bytestransferred"
    P_BYTESTRANSFERRED = 90
    def _sk_get_bytespersecond(self):
        """
        Current data transfer speed in bytes per second. Typically, your UI will want to display this value as kilobytes per second (KBps). 
        
        """
        return self._sk_property("ZG[]\006", 91, True)
    bytespersecond = property(_sk_get_bytespersecond)
    propid2label[91] = "bytespersecond"
    P_BYTESPERSECOND = 91
    def _sk_get_chatmsg_guid(self):
        """
        The "global ID" of this Transfer's associated Message instance. GUIDs are shared across Skype client instances and across all users that can see this Message. 
        
        """
        return self._sk_property("ZG\134]\006", 92, True)
    chatmsg_guid = property(_sk_get_chatmsg_guid)
    propid2label[92] = "chatmsg_guid"
    P_CHATMSG_GUID = 92
    def _sk_get_chatmsg_index(self):
        """
        A more or less arbitrary index for ordering multiple file transfers within the UI. 
        
        """
        return self._sk_property("ZG]]\006", 93, True)
    chatmsg_index = property(_sk_get_chatmsg_index)
    propid2label[93] = "chatmsg_index"
    P_CHATMSG_INDEX = 93
    def _sk_get_convo_id(self):
        """
        The "global ID" of this Transfer's associated Conversation (as chained through its associated Message). GUIDs are shared across Skype client instances and across all users that can see this Conversation. 
        
        Note that currently SkypeKit sets this property for INCOMING file transfers only and returns 0 (zero) for all sending side transfers. This is a known bug. 
        
        """
        return module_id2classes[18](self._sk_property("ZGb]\006",
  98, True), self.transport)
    convo_id = property(_sk_get_convo_id)
    propid2label[98] = "convo_id"
    P_CONVO_ID = 98
    
    def Accept(
        self,
        filename_with_path
        ):
        """
        Accepts an incoming file transfer and saves it to specified file on the local file system. If the specified file exists, SkypeKit will silently overwrite it. Your UI should prompting the user for confirmation in this case and provide a means for canceling the file transfer or specifying a different target file. 
        
        
        B{Arguments:}
         - B{filename_with_path} - Where on the local file system to save the file being transferred. Note that you should specify the path as being fully-qualified. Otherwise, SkypeKit will be assume it to be relative to the SkypeKit runtime path, since the method is actually executed in the runtime context. 
        
        
        B{Return values:}
         - B{success} - Set to true if the specified target file was successfully created on the local file system -and- the initial write(s) succeeded. However, the transfer itself can subsequently fail before completion due to its being canceled (either locally or remotely), network failure, local file system space/write issues, and so forth. 
        
        """
        l_request = skypekit.XCallRequest("ZR\006\003", 6, 3)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('f', 1, filename_with_path)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def Pause(self):
        """
        Temporarily pauses an in-progress incoming or outgoing file transfer. For incoming transfers, only this affects the sender and the invoking recipient only. For outgoing transfers, this affects the sender and all recipients. 
        
        """
        l_request = skypekit.XCallRequest("ZR\006\004", 6, 4)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def Resume(self):
        """
        Resumes a previously paused file transfer. 
        
        """
        l_request = skypekit.XCallRequest("ZR\006\005", 6, 5)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def Cancel(self):
        """
        Cancels an in-progress file transfer. Transfer::STATUS will transition to CANCELLED for incoming file transfers and to CANCELLED_BY_REMOTE for outgoing transfers. 
        
        """
        l_request = skypekit.XCallRequest("ZR\006\006", 6, 6)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
module_id2classes[6] = Transfer

class Account(skypekit.Object):
    """
    Represents a local account. Encapsulates methods for Skype account creation, login and logout as well as account profile setting properties. NB! Unlike all the other SkypeKit classes, most of the Account class properties are actually read-write. In fact, there are two sorts of Account's read-write properties: server-side properties and local properties. Different setter methods need to be used for those two kinds. 
    
    The subset of server-side properties consists of all the policy properties (everything with _POLICY suffix) that are all of type int and can be set with SetServersideIntProperty method. There is currently only one server-side string property - OFFLINE_CALLFORWARD which can be set with SetServersideStrProperty setter. 
    
    The set of writeable account profile properties (local profile) is as follows; 
      - P_FULLNAME, 
      - P_BIRTHDAY, 
      - P_GENDER, 
      - P_LANGUAGES, 
      - P_COUNTRY, 
      - P_PROVINCE, 
      - P_CITY, 
      - P_PHONE_HOME, 
      - P_PHONE_OFFICE, 
      - P_PHONE_MOBILE, 
      - P_EMAILS, 
      - P_HOMEPAGE, 
      - P_ABOUT, 
      - P_MOOD_TEXT, 
      - P_TIMEZONE, 
      - P_AVATAR_IMAGE, 
      - P_RICH_MOOD_TEXT 
    
    These can be set with SetIntProperty, SetStrProperty and SetBinProperty setter methods. Note that there are special methods for changing account online status (availability) and enabling/disabling auto-login - SetAvailability and SetSavePwd. 
    
    Note that to set Account properties, you that Account needs to be logged in. Generally, assumption is that the first thing you do with an Account object after you retrieve it with Skype::GetAccount is login, with exception of creating a new account. 
    
    The Account class has no default constructor and creating an Account instance is not the same as creating a Skype account. To create a Skype account: 
    
      - obtain an Account instance by invoking Skype::GetAccount. This automatically sets P_SKYPENAME. 
      - set any additional profile-related properties. Skype recommends that you minimally set the following: 
      - their email address so that they can retrieve a lost password (P_EMAILS) 
      - the user's full name (P_FULLNAME) 
      - at least one phone number (P_PHONE_HOME, P_PHONE_HOME, P_PHONE_OFFICE) 
      - invoke Account::Register to actually create the account 
    
    """
    event_handlers = {}
    propid2label = {}
    def _sk_init_(self, object_id, transport):
        """ actual constructor """
        skypekit.Object._sk_init_(self, object_id, transport)
    def __str__(self):
        return "Account %s" % (self.object_id, )
    @staticmethod
    def propid(propname):
        """ convert a property name to the enum of the property """
        return getattr(Account, "P_"+propname.upper())
    module_id = 5
    def OnPropertyChange(self, property_name):
        """ notifies from a property change """
        
        pass
    STATUS = {1:'LOGGED_OUT', 'LOGGED_OUT':1, 2:'LOGGED_OUT_AND_PWD_SAVED', 'LOGGED_OUT_AND_PWD_SAVED':2, 3:'CONNECTING_TO_P2P', 'CONNECTING_TO_P2P':3, 4:'CONNECTING_TO_SERVER', 'CONNECTING_TO_SERVER':4, 5:'LOGGING_IN', 'LOGGING_IN':5, 6:'INITIALIZING', 'INITIALIZING':6, 7:'LOGGED_IN', 'LOGGED_IN':7, 8:'LOGGING_OUT', 'LOGGING_OUT':8}
    """
     - LOGGED_OUT
     - LOGGED_OUT_AND_PWD_SAVED - the account is logged out, but password is not needed for re-login
     - CONNECTING_TO_P2P - connecting to P2P network
     - CONNECTING_TO_SERVER - connecting to login server
     - LOGGING_IN - waiting for response from server
     - INITIALIZING - response OK. initialising account-specific lib structures
     - LOGGED_IN - alright, we're good to go!
     - LOGGING_OUT - Logout() has been called but not processed yet
    """
    CBLSYNCSTATUS = {0:'CBL_INITIALIZING', 'CBL_INITIALIZING':0, 1:'CBL_INITIAL_SYNC_PENDING', 'CBL_INITIAL_SYNC_PENDING':1, 2:'CBL_SYNC_PENDING', 'CBL_SYNC_PENDING':2, 3:'CBL_SYNC_IN_PROGRESS', 'CBL_SYNC_IN_PROGRESS':3, 4:'CBL_IN_SYNC', 'CBL_IN_SYNC':4, 5:'CBL_SYNC_FAILED', 'CBL_SYNC_FAILED':5, 6:'CBL_REMOTE_SYNC_PENDING', 'CBL_REMOTE_SYNC_PENDING':6}
    """
    Recognized values for the P_CBLSYNCSTATUS property. CBL stands for Central Buddy List. In principle, this property and its states can be ignored by most UI developers. However, it can help to optimize UI buildup and behaviour, particularly in case of limited resources (such as mobile devices). 
    
    CBL is used to backup your contact list, contact groups, and profile information, and also used to synchronize this information with other Skype instances of your account (i.e. on another device). CBL sync can occur both during login and during normal operation. Note that CBL synchronization does not take place immediately after an Account property is changed. A delay between the first property change and CBL sync initiation enables the client to accumulate changes and do the synchronization in bulk. 
    
    Clients with limited resources might want to wait for CBL_IN_SYNC status before generating their UI's contact list representation. Otherwise it might be forced to redraw the contact list multiple times, as new updates get retrieved from the server-side. Similarly, applications that modify an account's mood message might want to know when the P_MOOD_TEXT or P_RICH_MOOD_TEXT property is synchronized to the server. Note that this sync is only for CBL and other logged in instances of the same account - other contacts will receive the mood message update directly. 
    
     - CBL_INITIALIZING - status is not clear (yet)
     - CBL_INITIAL_SYNC_PENDING - first sync with empty profile
     - CBL_SYNC_PENDING - Account properties are considered to be out of sync with CBL - attempt at synchronization is imminent. You might wish to wait with updating UI components that display the data that is about to change anyway.
     - CBL_SYNC_IN_PROGRESS - CBL synchronization is currently taking place.
     - CBL_IN_SYNC - Account properties are up-to-date.
     - CBL_SYNC_FAILED - CBL sync has. Another attempt will be made after several minutes. If a second attempt also fails, subsequent attempts at synchronization will be made at ever increasing intervals.
     - CBL_REMOTE_SYNC_PENDING - we have received a hint that there is a remote data change in CBL
    """
    LOGOUTREASON = {
        1 :'LOGOUT_CALLED',
        2 :'HTTPS_PROXY_AUTH_FAILED',
        3 :'SOCKS_PROXY_AUTH_FAILED',
        4 :'P2P_CONNECT_FAILED',
        5 :'SERVER_CONNECT_FAILED',
        6 :'SERVER_OVERLOADED',
        7 :'DB_IN_USE',
        8 :'INVALID_SKYPENAME',
        9 :'INVALID_EMAIL',
        10:'UNACCEPTABLE_PASSWORD',
        11:'SKYPENAME_TAKEN',
        12:'REJECTED_AS_UNDERAGE',
        13:'NO_SUCH_IDENTITY',
        14:'INCORRECT_PASSWORD',
        15:'TOO_MANY_LOGIN_ATTEMPTS',
        16:'PASSWORD_HAS_CHANGED',
        17:'PERIODIC_UIC_UPDATE_FAILED',
        18:'DB_DISK_FULL',
        19:'DB_IO_ERROR',
        20:'DB_CORRUPT',
        21:'DB_FAILURE',
        22:'INVALID_APP_ID',
        23:'APP_ID_FAILURE',
        24:'UNSUPPORTED_VERSION',
        'LOGOUT_CALLED'
        
                
             : 1,
        'HTTPS_PROXY_AUTH_FAILED'
        
                   : 2,
        'SOCKS_PROXY_AUTH_FAILED'
        
                   : 3,
        'P2P_CONNECT_FAILED'
        
                
        : 4,
        'SERVER_CONNECT_FAILED'
        
                     : 5,
        'SERVER_OVERLOADED'
        
                
         : 6,
        'DB_IN_USE'
        
                
                 : 7,
        'INVALID_SKYPENAME'
        
                
         : 8,
        'INVALID_EMAIL'
        
                
             : 9,
        'UNACCEPTABLE_PASSWORD'
        
                     :10,
        'SKYPENAME_TAKEN'
        
                
           :11,
        'REJECTED_AS_UNDERAGE'
        
                      :12,
        'NO_SUCH_IDENTITY'
        
                
          :13,
        'INCORRECT_PASSWORD'
        
                
        :14,
        'TOO_MANY_LOGIN_ATTEMPTS'
        
                   :15,
        'PASSWORD_HAS_CHANGED'
        
                      :16,
        'PERIODIC_UIC_UPDATE_FAILED'
        
                :17,
        'DB_DISK_FULL'
        
                
              :18,
        'DB_IO_ERROR'
        
                
               :19,
        'DB_CORRUPT'
        
                
                :20,
        'DB_FAILURE'
        
                
                :21,
        'INVALID_APP_ID'
        
                
            :22,
        'APP_ID_FAILURE'
        
                
            :23,
        'UNSUPPORTED_VERSION'
        
                       :24
    }
    """
    Recognized values for the Account::P_LOGOUTREASON. Note that this property should only be examined when Account::P_STATUS is LOGGED_OUT or LOGGED_OUT_AND_PWD_SAVED. That is, you should not monitor changes to this property in Account::OnChange callback, other than after already having checked that P_STATUS property has appropriate value. The reason for this is that the LOGOUTREASON does not get reset before the next login attempt. For example: if a user tried to log in with a wrong password, the LOGOUTREASON gets set to INCORRECT_PASSWORD. Now, if the user tries to log in again, and yet again submits an incorrect password, the value of the LOGOUTREASON does not change anymore, because it already is set to INCORRECT_PASSWORD. Consequently, Account::OnChange will not fire in this case. 
    
     - LOGOUT_CALLED - manual logout (or unknown reason from previous session)
     - HTTPS_PROXY_AUTH_FAILED - sync errors at login/registration
     - SOCKS_PROXY_AUTH_FAILED - sync errors at login/registration
     - P2P_CONNECT_FAILED - sync errors at login/registration
     - SERVER_CONNECT_FAILED - sync errors at login/registration
     - SERVER_OVERLOADED - sync errors at login/registration
     - DB_IN_USE - sync errors at login/registration
     - INVALID_SKYPENAME - sync errors at registration
     - INVALID_EMAIL - sync errors at registration
     - UNACCEPTABLE_PASSWORD - sync errors at registration
     - SKYPENAME_TAKEN - sync errors at registration
     - REJECTED_AS_UNDERAGE - sync errors at registration
     - NO_SUCH_IDENTITY - sync errors at login
     - INCORRECT_PASSWORD - sync errors at login
     - TOO_MANY_LOGIN_ATTEMPTS - sync errors at login
     - PASSWORD_HAS_CHANGED - async errors (can happen anytime while logged in)
     - PERIODIC_UIC_UPDATE_FAILED - async errors (can happen anytime while logged in)
     - DB_DISK_FULL - async errors (can happen anytime while logged in)
     - DB_IO_ERROR - async errors (can happen anytime while logged in)
     - DB_CORRUPT - async errors (can happen anytime while logged in)
     - DB_FAILURE - deprecated (superceded by more detailed DB_* errors)
     - INVALID_APP_ID - platform sdk
     - APP_ID_FAILURE - platform sdk
     - UNSUPPORTED_VERSION - forced upgrade/discontinuation
    """
    PWDCHANGESTATUS = {0:'PWD_OK', 'PWD_OK':0, 1:'PWD_CHANGING', 'PWD_CHANGING':1, 2:'PWD_INVALID_OLD_PASSWORD', 'PWD_INVALID_OLD_PASSWORD':2, 3:'PWD_SERVER_CONNECT_FAILED', 'PWD_SERVER_CONNECT_FAILED':3, 4:'PWD_OK_BUT_CHANGE_SUGGESTED', 'PWD_OK_BUT_CHANGE_SUGGESTED':4, 5:'PWD_MUST_DIFFER_FROM_OLD', 'PWD_MUST_DIFFER_FROM_OLD':5, 6:'PWD_INVALID_NEW_PWD', 'PWD_INVALID_NEW_PWD':6, 7:'PWD_MUST_LOG_IN_TO_CHANGE', 'PWD_MUST_LOG_IN_TO_CHANGE':7}
    """
    Recognized values for the P_PWDCHANGESTATUS property that provides information on whether a password change succeeded or failed, giving detailed failure reason. After successful return from the Change Password method, clients should monitor the P_PWDCHANGESTATUS property changes. 
      - PWD_CHANGING - consider displaying an "in progress" indicator and continue polling 
      - PWD_OK - consider displaying an updated indicator and stop polling 
      - PWD_OK_BUT_CHANGE_SUGGESTED - consider displaying an updated indicator, along with a recommendation to change again to a stronger password. 
    
     - PWD_OK - Password change succeeded. 
    
     - PWD_CHANGING - Password change is in progress. 
    
     - PWD_INVALID_OLD_PASSWORD - Old password was incorrect. 
    
     - PWD_SERVER_CONNECT_FAILED - Failed to verify password because of no connection to server. 
    
     - PWD_OK_BUT_CHANGE_SUGGESTED - Password was set but server didn't like it much. 
    
     - PWD_MUST_DIFFER_FROM_OLD - New password was exactly the same as old one. 
    
     - PWD_INVALID_NEW_PWD - The new password was unacceptable. (too short, too simple, etc.) 
    
     - PWD_MUST_LOG_IN_TO_CHANGE - Account was currently not logged in. 
    
    """
    COMMITSTATUS = {1:'COMMITTED', 'COMMITTED':1, 2:'COMMITTING_TO_SERVER', 'COMMITTING_TO_SERVER':2, 3:'COMMIT_FAILED', 'COMMIT_FAILED':3}
    """
    The list of possible values of Account class COMMITSTATUS property. Note that this property and its values have nothing to do with (automatic) CBL synchronization. Rather, the COMMITSTATUS reflects commit status to account's server side properties initiated with calls to Account class SetServersideIntProperty and Account class SetServersideStrProperty methods. After those methods, your client UI may want to wait until the COMMITSTATUS becomes COMMITTING_TO_SERVER followed by COMMITTED and inform the user if the value becomes COMMIT_FAILED. SetServerside<type>Property methods are used for writing privacy policy related and call forwarding related Account properties to the server. Unlike CBL synchronization, those updates are executed immediately. 
    
     - COMMITTED - No pending updates to the server. 
    
     - COMMITTING_TO_SERVER - Update to the server in progress. 
    
     - COMMIT_FAILED - Server update has failed. 
    
    """
    CHATPOLICY = {0:'EVERYONE_CAN_ADD', 'EVERYONE_CAN_ADD':0, 2:'BUDDIES_OR_AUTHORIZED_CAN_ADD', 'BUDDIES_OR_AUTHORIZED_CAN_ADD':2}
    """
    Recognized values for the P_CHAT_POLICY property that controls whether non-authorized users can initiate text chat with the currently logged in account. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntProperty 
    
     - EVERYONE_CAN_ADD - Unauthorized contacts can initiate text chat. 
    
     - BUDDIES_OR_AUTHORIZED_CAN_ADD - Only authorized contacts can initiate chat (default policy). 
    
    """
    SKYPECALLPOLICY = {0:'EVERYONE_CAN_CALL', 'EVERYONE_CAN_CALL':0, 2:'BUDDIES_OR_AUTHORIZED_CAN_CALL', 'BUDDIES_OR_AUTHORIZED_CAN_CALL':2}
    """
    Recognized values for the P_SKYPECALLPOLICY property that controls acceptance of incoming Skype calls. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side. 
    
     - EVERYONE_CAN_CALL - Skype calls accepted from unauthorized contacts.
     - BUDDIES_OR_AUTHORIZED_CAN_CALL - Skype calls not accepted from unauthorized contacts.
    """
    PSTNCALLPOLICY = {0:'ALL_NUMBERS_CAN_CALL', 'ALL_NUMBERS_CAN_CALL':0, 1:'DISCLOSED_NUMBERS_CAN_CALL', 'DISCLOSED_NUMBERS_CAN_CALL':1, 2:'BUDDY_NUMBERS_CAN_CALL', 'BUDDY_NUMBERS_CAN_CALL':2}
    """
    Recognized values for the P_PSTNCALLPOLICY property that controls whether (and from whom) this account accepts incoming PSTN calls. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntProperty 
    
     - ALL_NUMBERS_CAN_CALL - All incoming PSTN calls are accepted.
     - DISCLOSED_NUMBERS_CAN_CALL - Only PSTN calls that report caller ID are accepted.
     - BUDDY_NUMBERS_CAN_CALL - Only calls from PSTN numbers found in local contact list are accepted.
    """
    AVATARPOLICY = {0:'BUDDIES_OR_AUTHORIZED_CAN_SEE', 'BUDDIES_OR_AUTHORIZED_CAN_SEE':0, 2:'EVERYONE_CAN_SEE', 'EVERYONE_CAN_SEE':2}
    """
    Recognized values for the P_AVATAR_POLICY property that controls whether remote contacts can view local account's avatar image. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side. 
    Note that setting account's AVATAR_POLICY to BUDDIES_OR_AUTHORIZED_CAN_SEE does not guarantee that remote users will be able to immediately retrieve the avatar picture via corresponding Contact object. Avatar changes propagate between clients only when direct sessions between clients are established. Direct sessions are established during live sessions or whilst online contacts are engaged in chat. 
    
     - BUDDIES_OR_AUTHORIZED_CAN_SEE - Only authorized remote users can see this user's avatar image
     - EVERYONE_CAN_SEE - Everyone can see this user's avatar image, once the contact/account avatar property has been synchronized during a direct session. The avatar image may also become viewable on some Skype Web-based services.
    """
    BUDDYCOUNTPOLICY = {0:'DISCLOSE_TO_AUTHORIZED', 'DISCLOSE_TO_AUTHORIZED':0, 1:'DISCLOSE_TO_NOONE', 'DISCLOSE_TO_NOONE':1}
    """
    Recognized values for the P_BUDDYCOUNT_POLICY property that controls whether the number of this user's authorized contacts is visible to other users, either through Account::GetPropNrofAuthedBuddies or Contact::GetPropNrofAuthedBuddies when those instances reference this user. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntProperty, like this: 
    account->SetServersideIntProperty(Account::P_BUDDYCOUNT_POLICY, Account::DISCLOSE_TO_AUTHORIZED); 
    account->SetServersideIntProperty(Account::P_BUDDYCOUNT_POLICY, Account::DISCLOSE_TO_NOONE ); 
    
     - DISCLOSE_TO_AUTHORIZED - Authorized remote users can retrieve the number of this user's authorized contacts (Contact::P_NROF_AUTHED_BUDDIES)
     - DISCLOSE_TO_NOONE - No remote user - regardless their authorization status - can retrieve the number of this user's authorized contacts. Account::GetPropNrofAuthedBuddies and Contact::GetPropNrofAuthedBuddies will always return 0
    """
    TIMEZONEPOLICY = {0:'TZ_AUTOMATIC', 'TZ_AUTOMATIC':0, 1:'TZ_MANUAL', 'TZ_MANUAL':1, 2:'TZ_UNDISCLOSED', 'TZ_UNDISCLOSED':2}
    """
    Recognized values for the P_TIMEZONEPOLICY property that sets the rules for timezone offset so remote clients can determine your local time. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side.
     - TZ_AUTOMATIC - Account's timezone setting is determined automatically. 
    
     - TZ_MANUAL - Account's timezone setting is set manually. 
    
     - TZ_UNDISCLOSED - Remote users will have no idea what your local time is. 
    
    """
    WEBPRESENCEPOLICY = {0:'WEBPRESENCE_DISABLED', 'WEBPRESENCE_DISABLED':0, 1:'WEBPRESENCE_ENABLED', 'WEBPRESENCE_ENABLED':1}
    """
    Recognized values for the P_WEBPRESENCEPOLICY property that controls whether your online status (presence) can be seen using the "Skype buttons" ( http://www.skype.com/share/buttons/ ) embedded in web pages. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side. 
    
     - WEBPRESENCE_DISABLED - Disable displaying online status on web for this account.
     - WEBPRESENCE_ENABLED - Enable displaying online status on web for this account.
    """
    PHONENUMBERSPOLICY = {0:'PHONENUMBERS_VISIBLE_FOR_BUDDIES', 'PHONENUMBERS_VISIBLE_FOR_BUDDIES':0, 1:'PHONENUMBERS_VISIBLE_FOR_EVERYONE', 'PHONENUMBERS_VISIBLE_FOR_EVERYONE':1}
    """
    Recognized values for the P_PHONENUMBERSPOLICY property that controls whether unauthorized remote users can see associated phone numbers in their UI (for reference, see the different phone number tabs in Windows desktop Client contact view). Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntProperty 
    
     - PHONENUMBERS_VISIBLE_FOR_BUDDIES - Only authorized contacts can see the phone numbers.
     - PHONENUMBERS_VISIBLE_FOR_EVERYONE - Everyone can see the phone numbers.
    """
    VOICEMAILPOLICY = {0:'VOICEMAIL_ENABLED', 'VOICEMAIL_ENABLED':0, 1:'VOICEMAIL_DISABLED', 'VOICEMAIL_DISABLED':1}
    """
    Recognized values for the P_VOICEMAILPOLICY property that controls acceptance of incoming voicemail messages. Note that since this set of values is associated with a server-side property, you must set that property using Account::SetServersideIntPropertyserver-side. 
    
     - VOICEMAIL_ENABLED - Incoming voicemails enabled.
     - VOICEMAIL_DISABLED - Incoming voicemails disabled.
    """
    CAPABILITYSTATUS = {0:'NO_CAPABILITY', 'NO_CAPABILITY':0, 1:'CAPABILITY_EXISTS', 'CAPABILITY_EXISTS':1, 2:'FIRST_EXPIRY_WARNING', 'FIRST_EXPIRY_WARNING':2, 3:'SECOND_EXPIRY_WARNING', 'SECOND_EXPIRY_WARNING':3, 4:'FINAL_EXPIRY_WARNING', 'FINAL_EXPIRY_WARNING':4}
    """
    Account capabability statuses are possible values of Contact class CAPABILITY enumerator, when that enumerator is used in context of account. Compared to Contact class CAPABILITYSTATUS enums, Account class CAPABILITYSTATUS has additional items for subscription expiration warnings. 
    
     - NO_CAPABILITY - Capability is not supported by the currently logged in SkypeKit client.
     - CAPABILITY_EXISTS - Capability is supported by the currently logged in SkypeKit client. 
    
     - FIRST_EXPIRY_WARNING - Support for this capability ends this month (within 30 days) 
    
     - SECOND_EXPIRY_WARNING - Support for this capability ends this week (within 7 days) 
    
     - FINAL_EXPIRY_WARNING - Support for this capability ends today 
    
    """
    
    def _sk_get_status(self):
        """
        Account::STATUS
        """
        return skypekit.enumof(Account.STATUS, self._sk_property("ZGF]\005", 70, True))
    status = property(_sk_get_status)
    propid2label[70] = "status"
    P_STATUS = 70
    def _sk_get_pwdchangestatus(self):
        """
        Account::PWDCHANGESTATUS
        """
        return skypekit.enumof(Account.PWDCHANGESTATUS, self._sk_property("ZGG]\005", 71, True))
    pwdchangestatus = property(_sk_get_pwdchangestatus)
    propid2label[71] = "pwdchangestatus"
    P_PWDCHANGESTATUS = 71
    def _sk_get_logoutreason(self):
        """
        This property should only be examined when Account::P_STATUS is LOGGED_OUT or LOGGED_OUT_AND_PWD_SAVED. That is, you should not monitor changes to this property in Account::OnChange callback, other than after already having checked that P_STATUS property has appropriate value. The reason for this is that the LOGOUTREASON does not get reset before the next login attempt. For example: if a user tried to log in with a wrong password, the LOGOUTREASON gets set to INCORRECT_PASSWORD. Now, if the user tries to log in again, and yet again submits an incorrect password, the value of the LOGOUTREASON does not change anymore, because it already is set to INCORRECT_PASSWORD. Consequently, Account::OnChange will not fire in this case. 
        
        """
        return skypekit.enumof(Account.LOGOUTREASON, self._sk_property("ZGI]\005", 73, True))
    logoutreason = property(_sk_get_logoutreason)
    propid2label[73] = "logoutreason"
    P_LOGOUTREASON = 73
    def _sk_get_commitstatus(self):
        """
        Account::COMMITSTATUS
        """
        return skypekit.enumof(Account.COMMITSTATUS, self._sk_property("ZGN]\005", 78, True))
    commitstatus = property(_sk_get_commitstatus)
    propid2label[78] = "commitstatus"
    P_COMMITSTATUS = 78
    def _sk_get_suggested_skypename(self):
        """
        suggested skypenames, semi-colon separated. present if logoutreason==SKYPENAME_TAKEN
        """
        return self._sk_property("ZGH]\005", 72, True)
    suggested_skypename = property(_sk_get_suggested_skypename)
    propid2label[72] = "suggested_skypename"
    P_SUGGESTED_SKYPENAME = 72
    def _sk_get_skypeout_balance_currency(self):
        """
        'EUR', 'USD', etc.
        """
        return self._sk_property("ZGJ]\005", 74, True)
    skypeout_balance_currency = property(_sk_get_skypeout_balance_currency)
    propid2label[74] = "skypeout_balance_currency"
    P_SKYPEOUT_BALANCE_CURRENCY = 74
    def _sk_get_skypeout_balance(self):
        """
        balance in 'cents'
        """
        return self._sk_property("ZGK]\005", 75, True)
    skypeout_balance = property(_sk_get_skypeout_balance)
    propid2label[75] = "skypeout_balance"
    P_SKYPEOUT_BALANCE = 75
    def _sk_get_skypeout_precision(self):
        """
        decimal points in ACCOUNT_SKYPEOUT_BALANCE
        """
        return self._sk_property("ZG\244\006]\005", 804, True)
    skypeout_precision = property(_sk_get_skypeout_precision)
    propid2label[804] = "skypeout_precision"
    P_SKYPEOUT_PRECISION = 804
    def _sk_get_skypein_numbers(self):
        """
        space-separated list of skypein numbers
        """
        return self._sk_property("ZGL]\005", 76, True)
    skypein_numbers = property(_sk_get_skypein_numbers)
    propid2label[76] = "skypein_numbers"
    P_SKYPEIN_NUMBERS = 76
    def _sk_get_cblsyncstatus(self):
        """
        Account::CBLSYNCSTATUS
        """
        return skypekit.enumof(Account.CBLSYNCSTATUS, self._sk_property("ZGO]\005", 79, True))
    cblsyncstatus = property(_sk_get_cblsyncstatus)
    propid2label[79] = "cblsyncstatus"
    P_CBLSYNCSTATUS = 79
    def _sk_get_offline_callforward(self):
        """
        space-separated list of 'begSecond,endSecond,identity' tokens
        """
        return self._sk_property("ZGM]\005", 77, True)
    offline_callforward = property(_sk_get_offline_callforward)
    propid2label[77] = "offline_callforward"
    P_OFFLINE_CALLFORWARD = 77
    def _sk_get_chat_policy(self):
        """
        Server-side account property, use SetServerside*Property() to set
        """
        return skypekit.enumof(Account.CHATPOLICY, self._sk_property("ZG\240\001]\005", 160, True))
    chat_policy = property(_sk_get_chat_policy)
    propid2label[160] = "chat_policy"
    P_CHAT_POLICY = 160
    def _sk_get_skype_call_policy(self):
        """
        Server-side account property, use SetServerside*Property() to set
        """
        return skypekit.enumof(Account.SKYPECALLPOLICY, self._sk_property("ZG\241\001]\005", 161, True))
    skype_call_policy = property(_sk_get_skype_call_policy)
    propid2label[161] = "skype_call_policy"
    P_SKYPE_CALL_POLICY = 161
    def _sk_get_pstn_call_policy(self):
        """
        Server-side account property, use SetServerside*Property() to set
        """
        return skypekit.enumof(Account.PSTNCALLPOLICY, self._sk_property("ZG\242\001]\005", 162, True))
    pstn_call_policy = property(_sk_get_pstn_call_policy)
    propid2label[162] = "pstn_call_policy"
    P_PSTN_CALL_POLICY = 162
    def _sk_get_avatar_policy(self):
        """
        Server-side account property, use SetServerside*Property() to set
        """
        return skypekit.enumof(Account.AVATARPOLICY, self._sk_property("ZG\243\001]\005", 163, True))
    avatar_policy = property(_sk_get_avatar_policy)
    propid2label[163] = "avatar_policy"
    P_AVATAR_POLICY = 163
    def _sk_get_buddycount_policy(self):
        """
        Server-side account property, use SetServerside*Property() to set
        """
        return skypekit.enumof(Account.BUDDYCOUNTPOLICY, self._sk_property("ZG\244\001]\005", 164, True))
    buddycount_policy = property(_sk_get_buddycount_policy)
    propid2label[164] = "buddycount_policy"
    P_BUDDYCOUNT_POLICY = 164
    def _sk_get_timezone_policy(self):
        """
        Server-side account property, use SetServerside*Property() to set
        """
        return skypekit.enumof(Account.TIMEZONEPOLICY, self._sk_property("ZG\245\001]\005", 165, True))
    timezone_policy = property(_sk_get_timezone_policy)
    propid2label[165] = "timezone_policy"
    P_TIMEZONE_POLICY = 165
    def _sk_get_webpresence_policy(self):
        """
        Server-side account property, use SetServerside*Property() to set
        """
        return skypekit.enumof(Account.WEBPRESENCEPOLICY, self._sk_property("ZG\246\001]\005", 166, True))
    webpresence_policy = property(_sk_get_webpresence_policy)
    propid2label[166] = "webpresence_policy"
    P_WEBPRESENCE_POLICY = 166
    def _sk_get_phonenumbers_policy(self):
        """
        Server-side account property, use SetServerside*Property() to set
        """
        return skypekit.enumof(Account.PHONENUMBERSPOLICY, self._sk_property("ZG\250\001]\005", 168, True))
    phonenumbers_policy = property(_sk_get_phonenumbers_policy)
    propid2label[168] = "phonenumbers_policy"
    P_PHONENUMBERS_POLICY = 168
    def _sk_get_voicemail_policy(self):
        """
        Server-side account property, use SetServerside*Property() to set
        """
        return skypekit.enumof(Account.VOICEMAILPOLICY, self._sk_property("ZG\251\001]\005", 169, True))
    voicemail_policy = property(_sk_get_voicemail_policy)
    propid2label[169] = "voicemail_policy"
    P_VOICEMAIL_POLICY = 169
    def _sk_get_partner_optedout(self):
        """
        Alerts: opted out partner id's, space separated
        """
        return self._sk_property("ZG\205\006]\005", 773, True)
    partner_optedout = property(_sk_get_partner_optedout)
    propid2label[773] = "partner_optedout"
    P_PARTNER_OPTEDOUT = 773
    def _sk_get_service_provider_info(self):
        """
        service information if the user is a paid service provider
        """
        return self._sk_property("ZG\240\006]\005", 800, True)
    service_provider_info = property(_sk_get_service_provider_info)
    propid2label[800] = "service_provider_info"
    P_SERVICE_PROVIDER_INFO = 800
    def _sk_get_registration_timestamp(self):
        """
        NB! Unlike your common UNIX timestamps, the registration_timestamp is special, as it counts MINUTES rather than seconds, from Epoch (January 1, 1970) 
        
        """
        return self._sk_property("ZG\241\006]\005", 801, True)
    registration_timestamp = property(_sk_get_registration_timestamp)
    propid2label[801] = "registration_timestamp"
    P_REGISTRATION_TIMESTAMP = 801
    def _sk_get_nr_of_other_instances(self):
        """
        number of times this user is logged in from other computers
        """
        return self._sk_property("ZG\242\006]\005", 802, True)
    nr_of_other_instances = property(_sk_get_nr_of_other_instances)
    propid2label[802] = "nr_of_other_instances"
    P_NR_OF_OTHER_INSTANCES = 802
    def _sk_get_skypename(self):
        return self._sk_property("ZG\004]\005", 4, True)
    skypename = property(_sk_get_skypename)
    propid2label[4] = "skypename"
    P_SKYPENAME = 4
    def _sk_get_fullname(self):
        return self._sk_property("ZG\005]\005", 5, True)
    fullname = property(_sk_get_fullname)
    propid2label[5] = "fullname"
    P_FULLNAME = 5
    def _sk_get_birthday(self):
        """
        YYYYMMDD
        """
        return self._sk_property("ZG\007]\005", 7, True)
    birthday = property(_sk_get_birthday)
    propid2label[7] = "birthday"
    P_BIRTHDAY = 7
    def _sk_get_gender(self):
        """
        1-male, 2-female
        """
        return self._sk_property("ZG\010]\005", 8, True)
    gender = property(_sk_get_gender)
    propid2label[8] = "gender"
    P_GENDER = 8
    def _sk_get_languages(self):
        """
        ISO language codes, space-separated
        """
        return self._sk_property("ZG\011]\005", 9, True)
    languages = property(_sk_get_languages)
    propid2label[9] = "languages"
    P_LANGUAGES = 9
    def _sk_get_country(self):
        """
        ISO country code
        """
        return self._sk_property("ZG\012]\005", 10, True)
    country = property(_sk_get_country)
    propid2label[10] = "country"
    P_COUNTRY = 10
    def _sk_get_province(self):
        return self._sk_property("ZG\013]\005", 11, True)
    province = property(_sk_get_province)
    propid2label[11] = "province"
    P_PROVINCE = 11
    def _sk_get_city(self):
        return self._sk_property("ZG\014]\005", 12, True)
    city = property(_sk_get_city)
    propid2label[12] = "city"
    P_CITY = 12
    def _sk_get_phone_home(self):
        """
        NB! string not integer
        """
        return self._sk_property("ZG\015]\005", 13, True)
    phone_home = property(_sk_get_phone_home)
    propid2label[13] = "phone_home"
    P_PHONE_HOME = 13
    def _sk_get_phone_office(self):
        return self._sk_property("ZG\016]\005", 14, True)
    phone_office = property(_sk_get_phone_office)
    propid2label[14] = "phone_office"
    P_PHONE_OFFICE = 14
    def _sk_get_phone_mobile(self):
        return self._sk_property("ZG\017]\005", 15, True)
    phone_mobile = property(_sk_get_phone_mobile)
    propid2label[15] = "phone_mobile"
    P_PHONE_MOBILE = 15
    def _sk_get_emails(self):
        """
        Space separated
        """
        return self._sk_property("ZG\020]\005", 16, True)
    emails = property(_sk_get_emails)
    propid2label[16] = "emails"
    P_EMAILS = 16
    def _sk_get_homepage(self):
        return self._sk_property("ZG\021]\005", 17, True)
    homepage = property(_sk_get_homepage)
    propid2label[17] = "homepage"
    P_HOMEPAGE = 17
    def _sk_get_about(self):
        """
        arbitrary introductory text
        """
        return self._sk_property("ZG\022]\005", 18, True)
    about = property(_sk_get_about)
    propid2label[18] = "about"
    P_ABOUT = 18
    def _sk_get_profile_timestamp(self):
        """
        UNIX timestamp of last profile change
        """
        return self._sk_property("ZG\023]\005", 19, True)
    profile_timestamp = property(_sk_get_profile_timestamp)
    propid2label[19] = "profile_timestamp"
    P_PROFILE_TIMESTAMP = 19
    def _sk_get_mood_text(self):
        """
        Personal mood text (visible to authorised users only). Max length 300 bytes. 
        
        """
        return self._sk_property("ZG\032]\005", 26, True)
    mood_text = property(_sk_get_mood_text)
    propid2label[26] = "mood_text"
    P_MOOD_TEXT = 26
    def _sk_get_timezone(self):
        """
        24*3600+diff_to_UTC_in_seconds. nb! changes with DST
        """
        return self._sk_property("ZG\033]\005", 27, True)
    timezone = property(_sk_get_timezone)
    propid2label[27] = "timezone"
    P_TIMEZONE = 27
    def _sk_get_nrof_authed_buddies(self):
        """
        Count of this user's authorized contacts. 
        
        """
        return self._sk_property("ZG\034]\005", 28, True)
    nrof_authed_buddies = property(_sk_get_nrof_authed_buddies)
    propid2label[28] = "nrof_authed_buddies"
    P_NROF_AUTHED_BUDDIES = 28
    def _sk_get_availability(self):
        """
        Contact::AVAILABILITY
        """
        return skypekit.enumof(Contact.AVAILABILITY, self._sk_property("ZG\042]\005", 34, True))
    availability = property(_sk_get_availability)
    propid2label[34] = "availability"
    P_AVAILABILITY = 34
    def _sk_get_avatar_image(self):
        """
        Account avatar picture can be set with Account::SetBinProperty method. The contact avatar picture is limited to max 96x96 pixels and 32000 bytes. If the submitted avatar picture exceeds either of these size limits, it is the responsibility of your client to scale the image down to appropriate size. 
        
        The avatar pictures have to be in JPG format. A SkypeKit client can enable the user to set the Account avatar in other picture formats, in which case the picture should be converted to JPG before submitting it. 
        
        In any case, the avatar picture should be pre-validated with the Skype::ValidateAvatar method. 
        
        """
        return self._sk_property("ZG%]\005", 37, True)
    avatar_image = property(_sk_get_avatar_image)
    propid2label[37] = "avatar_image"
    P_AVATAR_IMAGE = 37
    def _sk_get_avatar_timestamp(self):
        """
        UNIX timestamp of when current avatar was set
        """
        return self._sk_property("ZG\266\001]\005", 182, True)
    avatar_timestamp = property(_sk_get_avatar_timestamp)
    propid2label[182] = "avatar_timestamp"
    P_AVATAR_TIMESTAMP = 182
    def _sk_get_mood_timestamp(self):
        """
        UNIX timestamp of when current mood was set
        """
        return self._sk_property("ZG\267\001]\005", 183, True)
    mood_timestamp = property(_sk_get_mood_timestamp)
    propid2label[183] = "mood_timestamp"
    P_MOOD_TIMESTAMP = 183
    def _sk_get_rich_mood_text(self):
        """
        XML version of CONTACT_MOOD_TEXT. Max length 1000 bytes. 
        
        """
        return self._sk_property("ZG\315\001]\005", 205, True)
    rich_mood_text = property(_sk_get_rich_mood_text)
    propid2label[205] = "rich_mood_text"
    P_RICH_MOOD_TEXT = 205
    
    def GetStatusWithProgress(self):
        """
        No description available.
        
        B{Return values:}
         - B{status} - Current value of this account's P_STATUS property
         - B{progress} - This argument returns 0. 
        
        """
        l_request = skypekit.XCallRequest("ZR\005\001", 5, 1)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = (skypekit.enumof(Account.STATUS, l_response.get(1))),
        l_result += (l_response.get(2, 0)),
        return l_result
    def Login(
        self,
        set_availability_to = "UNKNOWN"
        ):
        """
        Login an auto-login enabled account (P_STATUS is LOGGED_OUT_AND_PWD_SAVED) and optionally set the availability, for example login in as Contact::DO_NOT_DISTURB. To find out whether there is an auto-login enabled account available, you can use Skype::GetDefaultAccountName to retrieve the skypename of that account. You can then get an Account instance for login by invoking Skype::GetAccount. 
        
        If no accounts with stored login credentials are available (GetDefaultAccountName returns an empty string), then you will have to prompt the user for account name and password and then use LoginWithPassword. Account name field in the UI can be pre-populated with strings retrieved with Skype::GetExistingAccounts 
        
        
        B{Arguments:}
         - B{set_availability_to} - force this account's initial online status to the specified Contact::AVAILABILITY value.
        """
        l_request = skypekit.XCallRequest("ZR\005\005", 5, 5)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Contact.AVAILABILITY[set_availability_to])
        self.transport.xcall(l_request)
    def LoginWithPassword(
        self,
        password,
        save_pwd = False,
        save_data_locally = True
        ):
        """
         
        Login in an account by specifying its password. To retrieve an account instance for login, invoke Skype::GetAccount. 
        
        
        B{Arguments:}
         - B{password} - Password string (plaintext) for this account, which should be pre-validated (Skype::ValidatePassword) 
        
         - B{save_pwd} - 
          - true: Saves the password, ensuring that auto-login is enabled. 
          - false (default): Does not save the password, and so the user might not be able to effect auto-login until they explicitly invoke Account::SetPasswordSaved(true).
         - B{save_data_locally} - For internal use only.
        """
        l_request = skypekit.XCallRequest("ZR\005\006", 5, 6)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, password)
        l_request.add_parm('b', 2, save_pwd)
        l_request.add_parm('b', 3, save_data_locally)
        self.transport.xcall(l_request)
    def Register(
        self,
        password,
        save_pwd = False,
        save_data_locally = True,
        email = "",
        allow_spam = False
        ):
        """
        This command can be used to create a new Skype account, based on the Account object. When successful, this command will also log in with the newly created account. If the new account registration was not successful, Account status property will change to LOGGED_OUT. A common reason for registration failures is that the an account with that name already exists. In that case, Account LOGOUT_REASON will be set to SKYPENAME_TAKEN. Also, Account SUGGESTED_SKYPENAME property will be set to a list of similar but still available skypenames that could be used instead. The property will contain up to 5 semicolon-separated alternative skypenames. In if no suggested skypenames were available, then this property will contain an empty string. 
        NB! You should pre-validate your P_SKYPENAME value and any email string (Skype::ValidateProfileString) prior to invoking this method. 
        
        
        B{Arguments:}
         - B{password} - Password string (plaintext) for this account, which should be pre-validated (Skype::ValidatePassword)
         - B{save_pwd} - 
          - true: Saves the password and enables auto-login. 
          - false (default): Does not save the password, and the user needs to be prompted for password on the next login attempt.
         - B{save_data_locally} - For internal use only.
         - B{email} - An email address for retrieving lost passwords and receiving news and information from Skype.
         - B{allow_spam} - enable/disable news and information from Skype being sent to account's e-mail.
        """
        l_request = skypekit.XCallRequest("ZR\005\007", 5, 7)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, password)
        l_request.add_parm('b', 2, save_pwd)
        l_request.add_parm('b', 3, save_data_locally)
        l_request.add_parm('S', 4, email)
        l_request.add_parm('b', 5, allow_spam)
        self.transport.xcall(l_request)
    def Logout(
        self,
        clear_saved_pwd = False
        ):
        """
        Logs out current account. Note that calling this on client application exit is optional. 
        
        
        B{Arguments:}
         - B{clear_saved_pwd} - 
          - true: Clears any saved password use with auto-login and so disables auto-login until you explicitly invoke Account::SetPasswordSaved(true). 
          - false (default): Does not clear any saved password and so does not affect existing auto-login behavior.
        """
        l_request = skypekit.XCallRequest("ZR\005\010", 5, 8)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('b', 1, clear_saved_pwd)
        self.transport.xcall(l_request)
    def ChangePassword(
        self,
        old_password,
        new_password,
        save_pwd = False
        ):
        """
        Changes this account's password. Returns false if the change failed. NB! You should pre-validate your password strings (Skype::ValidatePassword) and ensure that they are different prior to invoking this method. 
        
        
        B{Arguments:}
         - B{old_password} - "Current password string (plaintext) for this account
         - B{new_password} - New password string (plaintext) for this account
         - B{save_pwd} - 
          - true: Saves the new password and enables auto-login. 
          - false (default): Clears any existing saved password and so the user cannot effect auto-login until they explicitly invoke Account::SetPasswordSaved(true)
        """
        l_request = skypekit.XCallRequest("ZR\005\013", 5, 11)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('S', 1, old_password)
        l_request.add_parm('S', 2, new_password)
        l_request.add_parm('b', 3, save_pwd)
        self.transport.xcall(l_request)
    def SetPasswordSaved(
        self,
        save_pwd
        ):
        """
        save or clear credentials for auto-login whilst already logged in
        
        B{Arguments:}
         - B{save_pwd}
        """
        l_request = skypekit.XCallRequest("ZR\005\031", 5, 25)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('b', 1, save_pwd)
        self.transport.xcall(l_request)
    def SetServersideIntProperty(
        self,
        prop_key,
        value
        ):
        """
        Setter for integer (and enum) server-side properties. For a list of writeable server-side properties, see the detailed description of the Account class. 
        
        
        B{Arguments:}
         - B{prop_key}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\005\014", 5, 12)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, prop_key)
        l_request.add_parm('u', 2, value)
        self.transport.xcall(l_request)
    def SetServersideStrProperty(
        self,
        prop_key,
        value
        ):
        """
        Setter for String server-side properties. For a list of writeable server-side properties, see the detailed description of the Account class. 
        
        
        B{Arguments:}
         - B{prop_key}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\005\015", 5, 13)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, prop_key)
        l_request.add_parm('S', 2, value)
        self.transport.xcall(l_request)
    def CancelServerCommit(self):
        """
        Cancels an attempt to commit a server-side P_XXX_POLICY or the P_OFFLINE_CALLFORWARD server-side property. Invoking this cancellation only makes sense whilst the P_COMMITTSTATUS is in COMMITTING_TO_SERVER state. 
        
        """
        l_request = skypekit.XCallRequest("ZR\005\017", 5, 15)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def SetIntProperty(
        self,
        prop_key,
        value
        ):
        """
        Setter for integer properties. For a list of writeable account profile properties, see the detailed description of the Account class. 
        
        
        B{Arguments:}
         - B{prop_key}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\005\020", 5, 16)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, prop_key)
        l_request.add_parm('u', 2, value)
        self.transport.xcall(l_request)
    def SetStrProperty(
        self,
        prop_key,
        value
        ):
        """
        Setter for String properties. For a list of writeable account profile properties, see the detailed description of the Account class. NB! You should pre-validate your about and mood message strings (Skype::ValidateProfileString) prior to invoking this method. 
        
        
        B{Arguments:}
         - B{prop_key}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\005\021", 5, 17)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, prop_key)
        l_request.add_parm('S', 2, value)
        self.transport.xcall(l_request)
    def SetBinProperty(
        self,
        prop_key,
        value
        ):
        """
        Setter for BLOB properties, such as its avatar image. For a list of writeable account profile properties, see the detailed description of the Account class. NB! You should pre-validate your avatar image (Skype::ValidateAvatar) prior to invoking this method. 
        
        
        B{Arguments:}
         - B{prop_key}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\005\022", 5, 18)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, prop_key)
        l_request.add_parm('B', 2, value)
        self.transport.xcall(l_request)
    def SetAvailability(
        self,
        availability
        ):
        """
        Sets online status of the currently logged in account to one of the values from Contact class AVAILABILITY enumerator. 
        
        
        B{Arguments:}
         - B{availability} - only subset of all contact availabilities allowed
        """
        l_request = skypekit.XCallRequest("ZR\005\023", 5, 19)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Contact.AVAILABILITY[availability])
        self.transport.xcall(l_request)
    def SetStandby(
        self,
        standby
        ):
        """
        Transitions the availability of this account's associated user, who is assumed to be logged in. 
        
        
        B{Arguments:}
         - B{standby} - 
          - true: Saves the user's current availability, then sets it to CONTACT::AVAILABILITY.OFFLINE 
          - false: Reconnects the user and restores their previous availability
        """
        l_request = skypekit.XCallRequest("ZR\005\012", 5, 10)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('b', 1, standby)
        self.transport.xcall(l_request)
    def GetCapabilityStatus(
        self,
        capability
        ):
        """
        Returns state of a given account capability. Takes Contact class CAPABILITY property as input argument and returns its state and expiration timestamp where appropriate. For example (C++ wrapper, with other wrappers the syntax may vary but the idea is the same) MyAccount.GetCapabilityStatus(Contact::CAPABILITY_SKYPEOUT, Cap, T); will return Account::CAPABILITY_EXISTS if local account has SkypeOut enabled. 
        
        
        B{Arguments:}
         - B{capability}
        
        B{Return values:}
         - B{status}
         - B{expiry_timestamp}
        """
        l_request = skypekit.XCallRequest("ZR\005\025", 5, 21)
        l_request.add_parm('O', 0, self)
        l_request.add_parm('e', 1, Contact.CAPABILITY[capability])
        l_response = self.transport.xcall(l_request)
        l_result = (skypekit.enumof(Account.CAPABILITYSTATUS, l_response.get(1))),
        l_result += (l_response.get(2, 0)),
        return l_result
    def GetSkypenameHash(self):
        """
        Response is empty when called with an inactive or invalid account
        
        B{Return values:}
         - B{skypename_hash}
        """
        l_request = skypekit.XCallRequest("ZR\005\026", 5, 22)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def GetVerifiedEmail(self):
        """
        returns verified-by-Skype e-mail for this account if exists and verifiable
        
        B{Return values:}
         - B{email}
        """
        l_request = skypekit.XCallRequest("ZR\005\002", 5, 2)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def GetVerifiedCompany(self):
        """
        returns verified-by-Skype company for this account if exists and verifiable
        
        B{Return values:}
         - B{company}
        """
        l_request = skypekit.XCallRequest("ZR\005\003", 5, 3)
        l_request.add_parm('O', 0, self)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def Delete(self):
        """
        Deletes all account data stored locally. Does not remove any account data from the server! 
        
        """
        l_request = skypekit.XCallRequest("ZR\005\030", 5, 24)
        l_request.add_parm('O', 0, self)
        self.transport.xcall(l_request)
    def mget_profile(self):
        """ get multiple properties from self """
        self.multiget("ZG\004,\005,\032,\020,\015,\016,\017,\007,\010,\011,\012,\013,\014,\021,\022,\033]\005")
module_id2classes[5] = Account
class Skype(object):
    """
    The main class that exposes Skype-related functionality to your application. Currently the SDK only supports one Skype class instance per runtime. Note that most of the Skype class methods will fail until you have successfully logged in with an account. For more information on constructing a Skype object and the login sequence, see first tutorial example. 
    
    """
    module_id = 0
    event_handlers = {}
    def __init__(self, transport):
        self.object_id = 0
        self.transport = transport
        transport.set_root(self)
    ''' Stop background operations (background threads). Might take some time.
    '''
    def stop(self):
        self.transport.stop()
    def Stop(self):
        warnings.warn("use stop() instead of Stop()", DeprecationWarning, stacklevel=2)
        self.stop()
    SETUPKEY_DB_STORAGE_QUOTA_KB = "*Lib/DbManager/StorageQuotaKb"
    def GetVersionString(self):
        """
        returns the runtime version as a string
        
        B{Return values:}
         - B{version}
        """
        l_request = skypekit.XCallRequest("ZR\000\034", 0, 28)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def GetUnixTimestamp(self):
        """
        Returns the time as used in SkypeKit, in the form of a Unix timestamp (number of seconds since 1.1.1970). If the local system time is incorrect my more than one year, the time provided by the Skype network will be provided, which is correct. Therefore this function can be used to adjust the system time if set incorrectly (e.g. if set to 1.1.1970).
        
        B{Return values:}
         - B{timestamp}
        """
        l_request = skypekit.XCallRequest("ZR\000\206\001", 0, 134)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, 0)
        return l_result
    def GetHardwiredContactGroup(
        self,
        type_
        ):
        """
        Takes TYPE argument (TYPE comes from ContactGroup class) and returns reference to the corresponding hardwired contact group. For example (C++): skype->GetHardwiredContactGroup(ContactGroup::ONLINE_BUDDIES, GroupRef) would return the list of all contacts that are currently online. 
        
        
        B{Arguments:}
         - B{type_}
        
        B{Return values:}
         - B{contact_group}
        """
        l_request = skypekit.XCallRequest("ZR\000\001", 0, 1)
        l_request.add_parm('e', 1, ContactGroup.TYPE[type_])
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[10](l_response.get(1), self.transport)
        return l_result
    def GetCustomContactGroups(self):
        """
        Returns a list of custom contact group references, i.e. all contact groups that are not hardwired. 
        
        
        B{Return values:}
         - B{groups}
        """
        l_request = skypekit.XCallRequest("ZR\000\002", 0, 2)
        l_response = self.transport.xcall(l_request)
        l_result = [module_id2classes[10](oid, self.transport) for oid in l_response.get(1, [])]
        return l_result
    def CreateCustomContactGroup(self):
        """
        Creates a new empty contact group object and returns a reference to it. The group will then show up in the custom group list that you can get with Skype class GetCustomContactGroups method. Existing contacts can be added to the new group with ContactGroup class AddContact method and a custom name can be given to it with GiveDisplayName method. 
        Note that no check is made for existing of displaynames with the same name - if you wish to force uniqueness in custom group names you will have to check that yourself before creating the group. 
        
        
        B{Return values:}
         - B{group}
        """
        l_request = skypekit.XCallRequest("ZR\000\003", 0, 3)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[10](l_response.get(1), self.transport)
        return l_result
    def OnNewCustomContactGroup(
        self,
        group
        ):
        """
        No description available.
        
        B{Return values:}
         - B{group} - eg, new contact group loaded from CBL
        """
        pass
    event_handlers[1] = "_sk_on_new_custom_contact_group"
    def _sk_on_new_custom_contact_group(self, parms):
        """ internal event dispatcher for OnNewCustomContactGroup
  """
        l_cleanparms = module_id2classes[10](parms.get(1), self.transport)
        self.OnNewCustomContactGroup(l_cleanparms)
    def mget_info_from_contacts(self, objects):
        """ get multiple properties from objects """
        self.transport.multiget("ZG\042,\025]\002", objects)
    def GetContactType(
        self,
        identity
        ):
        """
        analyzes the identity for contact type
        
        B{Arguments:}
         - B{identity}
        
        B{Return values:}
         - B{type_}
        """
        l_request = skypekit.XCallRequest("ZR\000\005", 0, 5)
        l_request.add_parm('S', 1, identity)
        l_response = self.transport.xcall(l_request)
        l_result = skypekit.enumof(Contact.TYPE, l_response.get(1))
        return l_result
    def GetContact(
        self,
        identity
        ):
        """
        Returns a Contact object reference. If a matching contact is not found in the existing contact list, a new Contact object will be created. Note that if you pass in a phone number in the identity argument, the type for the newly created Contact will be automatically set to Contact::PSTN (Contact::SKYPE otherwise). 
        
        
        B{Arguments:}
         - B{identity} - Either skypename or a phone number 
        
        
        B{Return values:}
         - B{contact} - Returns a contact object. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\006", 0, 6)
        l_request.add_parm('S', 1, identity)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[2](l_response.get(2), self.transport)
        return l_result
    def FindContactByPstnNumber(
        self,
        number
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{number}
        
        B{Return values:}
         - B{found}
         - B{contact}
         - B{found_in_key} - type is actually PROPKEY
        """
        l_request = skypekit.XCallRequest("ZR\000\010", 0, 8)
        l_request.add_parm('S', 1, number)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, False)),
        l_result += (module_id2classes[2](l_response.get(2), self.transport)),
        l_result += (l_response.get(3, 0)),
        return l_result
    IDENTITYTYPE = {
        0:'UNRECOGNIZED',
        1:'SKYPE',
        2:'SKYPE_MYSELF',
        3:'SKYPE_UNDISCLOSED',
        4:'PSTN',
        5:'PSTN_EMERGENCY',
        6:'PSTN_FREE',
        7:'PSTN_UNDISCLOSED',
        8:'CONFERENCE',
        9:'EXTERNAL',
        'UNRECOGNIZED'
        
                
              :0,
        'SKYPE'
        
                
                     :1,
        'SKYPE_MYSELF'
        
                
              :2,
        'SKYPE_UNDISCLOSED'
        
                
         :3,
        'PSTN'
        
                
                      :4,
        'PSTN_EMERGENCY'
        
                
            :5,
        'PSTN_FREE'
        
                
                 :6,
        'PSTN_UNDISCLOSED'
        
                
          :7,
        'CONFERENCE'
        
                
                :8,
        'EXTERNAL'
        
                
                  :9
    }
    """
     - UNRECOGNIZED
     - SKYPE
     - SKYPE_MYSELF
     - SKYPE_UNDISCLOSED
     - PSTN
     - PSTN_EMERGENCY
     - PSTN_FREE
     - PSTN_UNDISCLOSED
     - CONFERENCE
     - EXTERNAL
    """
    def GetIdentityType(
        self,
        identity
        ):
        """
        This takes skypename or a phone number string as argument and returns corresponding identity type (SKYPE, SKYPE_MYSELF, PSTN, etc.) 
        
        
        B{Arguments:}
         - B{identity}
        
        B{Return values:}
         - B{type_}
        """
        l_request = skypekit.XCallRequest("ZR\000\023", 0, 19)
        l_request.add_parm('S', 1, identity)
        l_response = self.transport.xcall(l_request)
        l_result = skypekit.enumof(Skype.IDENTITYTYPE, l_response.get(1))
        return l_result
    NORMALIZERESULT = {0:'IDENTITY_OK', 'IDENTITY_OK':0, 1:'IDENTITY_EMPTY', 'IDENTITY_EMPTY':1, 2:'IDENTITY_TOO_LONG', 'IDENTITY_TOO_LONG':2, 3:'IDENTITY_CONTAINS_INVALID_CHAR', 'IDENTITY_CONTAINS_INVALID_CHAR':3, 4:'PSTN_NUMBER_TOO_SHORT', 'PSTN_NUMBER_TOO_SHORT':4, 5:'PSTN_NUMBER_HAS_INVALID_PREFIX', 'PSTN_NUMBER_HAS_INVALID_PREFIX':5, 6:'SKYPENAME_STARTS_WITH_NONALPHA', 'SKYPENAME_STARTS_WITH_NONALPHA':6, 7:'SKYPENAME_SHORTER_THAN_6_CHARS', 'SKYPENAME_SHORTER_THAN_6_CHARS':7}
    """
     - IDENTITY_OK
     - IDENTITY_EMPTY
     - IDENTITY_TOO_LONG
     - IDENTITY_CONTAINS_INVALID_CHAR
     - PSTN_NUMBER_TOO_SHORT
     - PSTN_NUMBER_HAS_INVALID_PREFIX - identity looks like pstn number but does not start with +/00/011
     - SKYPENAME_STARTS_WITH_NONALPHA
     - SKYPENAME_SHORTER_THAN_6_CHARS - returned only when isNewSkypeName
    """
    def IdentitiesMatch(
        self,
        identity_a,
        identity_b
        ):
        """
        compares two identities to see if they match
        
        B{Arguments:}
         - B{identity_a}
         - B{identity_b}
        
        B{Return values:}
         - B{result}
        """
        l_request = skypekit.XCallRequest("ZR\000X", 0, 88)
        l_request.add_parm('S', 1, identity_a)
        l_request.add_parm('S', 2, identity_b)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def NormalizeIdentity(
        self,
        original,
        is_new_skype_name = False
        ):
        """
        This method is deprecated. Use ValidateProfileString method instead. 
        
        
        B{Arguments:}
         - B{original}
         - B{is_new_skype_name}
        
        B{Return values:}
         - B{result}
         - B{normalized}
        """
        l_request = skypekit.XCallRequest("ZR\000\011", 0, 9)
        l_request.add_parm('S', 1, original)
        l_request.add_parm('b', 2, is_new_skype_name)
        l_response = self.transport.xcall(l_request)
        l_result = (skypekit.enumof(Skype.NORMALIZERESULT, l_response.get(1))),
        l_result += (l_response.get(2, '')),
        return l_result
    def NormalizePSTNWithCountry(
        self,
        original,
        country_prefix = 0
        ):
        """
        NormalizePSTNWithCountry checks if the phone number starts with + if it doesn't, it prefixes the output with +XXX (where XXX is the country code). It also converts letters to numbers based on the standard phone keypad, so that the phone number string 212CALLME1 with country code 372 (Estonia) would be normalized to +3722122255631. If the method cannot normalize the phone number (because it's too long, too short, etc.), it returns an error code in &result. 
        
        
        B{Arguments:}
         - B{original}
         - B{country_prefix}
        
        B{Return values:}
         - B{result}
         - B{normalized}
        """
        l_request = skypekit.XCallRequest("ZR\000\315\001", 0, 205)
        l_request.add_parm('S', 1, original)
        l_request.add_parm('u', 2, country_prefix)
        l_response = self.transport.xcall(l_request)
        l_result = (skypekit.enumof(Skype.NORMALIZERESULT, l_response.get(1))),
        l_result += (l_response.get(2, '')),
        return l_result
    def OnContactOnlineAppearance(
        self,
        contact
        ):
        """
        This contact has appeared online. display alert
        
        B{Return values:}
         - B{contact}
        """
        pass
    event_handlers[2] = "_sk_on_contact_online_appearance"
    def _sk_on_contact_online_appearance(self, parms):
        """ internal event dispatcher for OnContactOnlineAppearance
  """
        l_cleanparms = module_id2classes[2](parms.get(1), self.transport)
        self.OnContactOnlineAppearance(l_cleanparms)
    def OnContactGoneOffline(
        self,
        contact
        ):
        """
        This contact has gone offline. display alert
        
        B{Return values:}
         - B{contact}
        """
        pass
    event_handlers[3] = "_sk_on_contact_gone_offline"
    def _sk_on_contact_gone_offline(self, parms):
        """ internal event dispatcher for OnContactGoneOffline
  """
        l_cleanparms = module_id2classes[2](parms.get(1), self.transport)
        self.OnContactGoneOffline(l_cleanparms)
    def GetOptimalAgeRanges(self):
        """
        list of (min,max) pairs
        
        B{Return values:}
         - B{range_list}
        """
        l_request = skypekit.XCallRequest("ZR\000M", 0, 77)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, [])
        return l_result
    def CreateContactSearch(self):
        """
        Creates a blank contact search object, in which you can add your custom search terms. For more information how asynchronous contact search works, see ContactSearch class details. 
        
        
        B{Return values:}
         - B{search} - Returns blank ContactSearch object. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\012", 0, 10)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[1](l_response.get(1), self.transport)
        return l_result
    def CreateBasicContactSearch(
        self,
        text
        ):
        """
        searches skypenames, aliases, fullnames, emails. false if not valid
        
        B{Arguments:}
         - B{text}
        
        B{Return values:}
         - B{search}
        """
        l_request = skypekit.XCallRequest("ZR\000\013", 0, 11)
        l_request.add_parm('S', 1, text)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[1](l_response.get(1), self.transport)
        return l_result
    def CreateIdentitySearch(
        self,
        identity
        ):
        """
        searches skypenames and aliases. returns 0 or 1 results. false if not valid
        
        B{Arguments:}
         - B{identity}
        
        B{Return values:}
         - B{search}
        """
        l_request = skypekit.XCallRequest("ZR\000\014", 0, 12)
        l_request.add_parm('S', 1, identity)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[1](l_response.get(1), self.transport)
        return l_result
    TRANSFER_SENDFILE_ERROR = {0:'TRANSFER_OPEN_SUCCESS', 'TRANSFER_OPEN_SUCCESS':0, 1:'TRANSFER_BAD_FILENAME', 'TRANSFER_BAD_FILENAME':1, 2:'TRANSFER_OPEN_FAILED', 'TRANSFER_OPEN_FAILED':2, 3:'TRANSFER_TOO_MANY_PARALLEL', 'TRANSFER_TOO_MANY_PARALLEL':3}
    """
    sync failure reasons when starting a transfer
     - TRANSFER_OPEN_SUCCESS
     - TRANSFER_BAD_FILENAME
     - TRANSFER_OPEN_FAILED
     - TRANSFER_TOO_MANY_PARALLEL
    """
    LEAVE_REASON = {
        0
  :'LEAVE_REASON_NONE',
        2
  :'RETIRED_USER_INCAPABLE',
        3
  :'RETIRED_ADDER_MUST_BE_FRIEND',
        4
  :'RETIRED_ADDER_MUST_BE_AUTHORIZED',
        5
  :'RETIRED_DECLINE_ADD',
        6
  :'RETIRED_UNSUBSCRIBE',
        100:'LIVE_NO_ANSWER',
        101:'LIVE_MANUAL',
        102:'LIVE_BUSY',
        103:'LIVE_CONNECTION_DROPPED',
        104:'LIVE_NO_SKYPEOUT_SUBSCRIPTION',
        105:'LIVE_INSUFFICIENT_FUNDS',
        106:'LIVE_INTERNET_CONNECTION_LOST',
        107:'LIVE_SKYPEOUT_ACCOUNT_BLOCKED',
        108:'LIVE_PSTN_COULD_NOT_CONNECT_TO_SKYPE_PROXY',
        109:'LIVE_PSTN_INVALID_NUMBER',
        110:'LIVE_PSTN_NUMBER_FORBIDDEN',
        111:'LIVE_PSTN_CALL_TIMED_OUT',
        112:'LIVE_PSTN_BUSY',
        113:'LIVE_PSTN_CALL_TERMINATED',
        114:'LIVE_PSTN_NETWORK_ERROR',
        115:'LIVE_NUMBER_UNAVAILABLE',
        116:'LIVE_PSTN_CALL_REJECTED',
        117:'LIVE_PSTN_MISC_ERROR',
        118:'LIVE_INTERNAL_ERROR',
        119:'LIVE_UNABLE_TO_CONNECT',
        120:'LIVE_RECORDING_FAILED',
        121:'LIVE_PLAYBACK_ERROR',
        122:'LIVE_LEGACY_ERROR',
        123:'LIVE_BLOCKED_BY_PRIVACY_SETTINGS',
        124:'LIVE_ERROR',
        125:'LIVE_TRANSFER_FAILED',
        126:'LIVE_TRANSFER_INSUFFICIENT_FUNDS',
        127:'LIVE_BLOCKED_BY_US',
        128:'LIVE_EMERGENCY_CALL_DENIED',
        'LEAVE_REASON_NONE'
        
                
         :  0,
        'RETIRED_USER_INCAPABLE'
        
                    :  2,
        'RETIRED_ADDER_MUST_BE_FRIEND'
        
              :  3,
        'RETIRED_ADDER_MUST_BE_AUTHORIZED'
        
          :  4,
        'RETIRED_DECLINE_ADD'
        
                       :  5,
        'RETIRED_UNSUBSCRIBE'
        
                       :  6,
        'LIVE_NO_ANSWER'
        
                
            :100,
        'LIVE_MANUAL'
        
                
               :101,
        'LIVE_BUSY'
        
                
                 :102,
        'LIVE_CONNECTION_DROPPED'
        
                   :103,
        'LIVE_NO_SKYPEOUT_SUBSCRIPTION'
        
             :104,
        'LIVE_INSUFFICIENT_FUNDS'
        
                   :105,
        'LIVE_INTERNET_CONNECTION_LOST'
        
             :106,
        'LIVE_SKYPEOUT_ACCOUNT_BLOCKED'
        
             :107,
        'LIVE_PSTN_COULD_NOT_CONNECT_TO_SKYPE_PROXY'
        :108,
        'LIVE_PSTN_INVALID_NUMBER'
        
                  :109,
        'LIVE_PSTN_NUMBER_FORBIDDEN'
        
                :110,
        'LIVE_PSTN_CALL_TIMED_OUT'
        
                  :111,
        'LIVE_PSTN_BUSY'
        
                
            :112,
        'LIVE_PSTN_CALL_TERMINATED'
        
                 :113,
        'LIVE_PSTN_NETWORK_ERROR'
        
                   :114,
        'LIVE_NUMBER_UNAVAILABLE'
        
                   :115,
        'LIVE_PSTN_CALL_REJECTED'
        
                   :116,
        'LIVE_PSTN_MISC_ERROR'
        
                      :117,
        'LIVE_INTERNAL_ERROR'
        
                       :118,
        'LIVE_UNABLE_TO_CONNECT'
        
                    :119,
        'LIVE_RECORDING_FAILED'
        
                     :120,
        'LIVE_PLAYBACK_ERROR'
        
                       :121,
        'LIVE_LEGACY_ERROR'
        
                
         :122,
        'LIVE_BLOCKED_BY_PRIVACY_SETTINGS'
        
          :123,
        'LIVE_ERROR'
        
                
                :124,
        'LIVE_TRANSFER_FAILED'
        
                      :125,
        'LIVE_TRANSFER_INSUFFICIENT_FUNDS'
        
          :126,
        'LIVE_BLOCKED_BY_US'
        
                
        :127,
        'LIVE_EMERGENCY_CALL_DENIED'
        
                :128
    }
    """
    Conversation and live state leave reasons for Participants. This type is used for the Participant::P_LAST_LEAVEREASON property. The primary use of this type is to provide detailed cause in case of a call failure. 
    
     - LEAVE_REASON_NONE
     - RETIRED_USER_INCAPABLE - automatic, user cannot chat (only some older versions might set this)
     - RETIRED_ADDER_MUST_BE_FRIEND - automatic
     - RETIRED_ADDER_MUST_BE_AUTHORIZED - automatic
     - RETIRED_DECLINE_ADD - manual reason (afaik no UI uses this)
     - RETIRED_UNSUBSCRIBE - manual reason
     - LIVE_NO_ANSWER
     - LIVE_MANUAL - live: User hung up
     - LIVE_BUSY
     - LIVE_CONNECTION_DROPPED
     - LIVE_NO_SKYPEOUT_SUBSCRIPTION
     - LIVE_INSUFFICIENT_FUNDS
     - LIVE_INTERNET_CONNECTION_LOST
     - LIVE_SKYPEOUT_ACCOUNT_BLOCKED
     - LIVE_PSTN_COULD_NOT_CONNECT_TO_SKYPE_PROXY
     - LIVE_PSTN_INVALID_NUMBER
     - LIVE_PSTN_NUMBER_FORBIDDEN
     - LIVE_PSTN_CALL_TIMED_OUT
     - LIVE_PSTN_BUSY
     - LIVE_PSTN_CALL_TERMINATED
     - LIVE_PSTN_NETWORK_ERROR
     - LIVE_NUMBER_UNAVAILABLE
     - LIVE_PSTN_CALL_REJECTED
     - LIVE_PSTN_MISC_ERROR
     - LIVE_INTERNAL_ERROR
     - LIVE_UNABLE_TO_CONNECT
     - LIVE_RECORDING_FAILED - live: Voicemail recording failed
     - LIVE_PLAYBACK_ERROR - live: Voicemail playback failed
     - LIVE_LEGACY_ERROR
     - LIVE_BLOCKED_BY_PRIVACY_SETTINGS
     - LIVE_ERROR - live: Fallback error
     - LIVE_TRANSFER_FAILED
     - LIVE_TRANSFER_INSUFFICIENT_FUNDS
     - LIVE_BLOCKED_BY_US
     - LIVE_EMERGENCY_CALL_DENIED
    """
    def mget_info_from_participants(self, objects):
        """ get multiple properties from objects """
        self.transport.multiget("ZG\247\007,\246\007,\250\007,\244\007,\266\007,\252\007,\243\007]\023", objects)
    def mget_info_from_conversations(self, objects):
        """ get multiple properties from objects """
        self.transport.multiget("ZG\234\007,\320\007,\240\007]\022", objects)
    def CreateConference(self):
        """
        Creates a new empty conversation object and returns a reference to it. 
        
        
        B{Return values:}
         - B{conference}
        """
        l_request = skypekit.XCallRequest("ZR\000\015", 0, 13)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[18](l_response.get(1), self.transport)
        return l_result
    def GetConversationByIdentity(
        self,
        convo_identity
        ):
        """
        Returns reference tp conversation object by conversation ID string (equivalent of old chat ID). NB! ID here is that of conversation, rather than skypename of dialog partner. If you want to retrieve a conversation object with any particular person, then Skype class GetConversationByParticipants method is what you are looking for. 
        
        
        B{Arguments:}
         - B{convo_identity}
        
        B{Return values:}
         - B{conversation}
        """
        l_request = skypekit.XCallRequest("ZR\000\017", 0, 15)
        l_request.add_parm('S', 1, convo_identity)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[18](l_response.get(1), self.transport)
        return l_result
    def GetConversationByParticipants(
        self,
        participant_identities,
        create_if_non_existing = False,
        ignore_bookmarked_or_named = False
        ):
        """
        myself not included
        
        B{Arguments:}
         - B{participant_identities}
         - B{create_if_non_existing}
         - B{ignore_bookmarked_or_named}
        
        B{Return values:}
         - B{conversation}
        """
        l_request = skypekit.XCallRequest("ZR\000\020", 0, 16)
        l_request.add_parm('S', 1, participant_identities)
        l_request.add_parm('b', 2, create_if_non_existing)
        l_request.add_parm('b', 3, ignore_bookmarked_or_named)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[18](l_response.get(1), self.transport)
        return l_result
    def GetConversationByBlob(
        self,
        join_blob,
        also_join = True
        ):
        """
        Retrieves a Conversation object by Public Conversation BLOB. Public conversation blobs are globally unique conversation IDs that provide a method for joining conversation without explicitly being added to the conversation by someone already in it. Programmatically, a Conversation BLOB can be retrieved with Conversation::GetJoinBlob method. In Skype desktop clients, the BLOB can be retrieved by typing "/get uri" in a conversation. The conversation can then be joined by people who have somehow received that BLOB. 
        
        
        B{Arguments:}
         - B{join_blob} - The BLOB string. 
        
         - B{also_join} - If set to true, automatically joins current user into the Conversation. 
        
        
        B{Return values:}
         - B{conversation} - Returns Conversation object if successful. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\021", 0, 17)
        l_request.add_parm('S', 1, join_blob)
        l_request.add_parm('b', 2, also_join)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[18](l_response.get(1), self.transport)
        return l_result
    def GetConversationList(
        self,
        type_ = "ALL_CONVERSATIONS"
        ):
        """
        Returns a list of Conversation objects by Conversation::LIST_TYPE filter. 
        
        
        B{Arguments:}
         - B{type_} - Filter. 
        

        
        B{Return values:}
         - B{conversations} - List of conversations matching the filter. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\022", 0, 18)
        l_request.add_parm('e', 1, Conversation.LIST_TYPE[type_])
        l_response = self.transport.xcall(l_request)
        l_result = [module_id2classes[18](oid, self.transport) for oid in l_response.get(1, [])]
        return l_result
    def OnConversationListChange(
        self,
        conversation,
        type_,
        added
        ):
        """
        This event gets fired when a Conversation item is added or removed from the list specified in the type argument. The primary use of this event is to detect creation of new Conversation objects. It can also be used for detecting occurance of live sessions - by monitoring added = true in Conversation::LIVE_CONVERSATIONS. Note that this method is not entirely sufficient for detecting live session termination (added = false and type = Conversation::LIVE_CONVERSATIONS). When the live session goes down, the default behaviour is that the Conversation object remains in the LIVE_CONVERSATIONS list for approximately 10 seconds. When another live session comes up within the same Conversation, the OnConversationListChange event will not fire - because the conversation was already in that list. There are two ways of getting around that. Firstly you can have all the conversations referenced at all times and then monitor Conversation::P_LOCAL_LIVESTATUS property changes, in which case you can pick up incoming live sessions from there. Alternatively, you can remove the delay between live session termination and conversation's removal from the LIVE_CONVERSATIONS list. This delay is controlled by the SETUPKEY_RECENTLY_LIVE_TIMEOUT setup key. To remove the delay, use Skype::SetInt(SETUPKEY_RECENTLY_LIVE_TIMEOUT, 0). Note that this setup key is account-based. You will need to have an account logged in in order to modify its value. 
        
        
        B{Return values:}
         - B{conversation} - Conversation object that was added or removed to a list specified in the type argument. 
        
         - B{type_} - Specifies the list, into which the conversation was added or removed from. 
        
         - B{added} - Specifies whether the conversation was added or removed. For ALL_CONVERSATIONS list, the removed event is only fired when the conversation is actually deleted. 
        
        """
        pass
    event_handlers[4] = "_sk_on_conversation_list_change"
    def _sk_on_conversation_list_change(self, parms):
        """ internal event dispatcher for OnConversationListChange
  """
        l_cleanparms = (module_id2classes[18](parms.get(1), self.transport)),
        l_cleanparms += (Conversation.LIST_TYPE[parms.get(2)]),
        l_cleanparms += (parms.get(3, False)),
        self.OnConversationListChange(*l_cleanparms)
    def mget_info_from_messages(self, objects):
        """ get multiple properties from objects """
        self.transport.multiget("ZG\300\007,{,\301\007,\177,y]\011", objects)
    def GetMessageByGuid(
        self,
        guid
        ):
        """
        Retrieves a Message object by the P_GUID property (globally unique ID, same for all the participants of the conversation, in which this message occured). 
        
        
        B{Arguments:}
         - B{guid} - Globally unique ID of the message. 
        
        
        B{Return values:}
         - B{message} - Returns a Message object if a match was found. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\025", 0, 21)
        l_request.add_parm('B', 1, guid)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[9](l_response.get(1), self.transport)
        return l_result
    def GetMessageListByType(
        self,
        type_,
        latest_per_conv_only,
        from_timestamp_inc = 0,
        to_timestamp_exc = skypekit.MAX_UINT
        ):
        """
        Returns all messages of the given type, the most recent POSTED_EMOTE or POSTED_TEXT of each conversation.
        
        B{Arguments:}
         - B{type_} - Type of messages requested
         - B{latest_per_conv_only} - Whether to return only the most recent message per conversation
         - B{from_timestamp_inc} - Starting timestamp for reqested range, inclusive
         - B{to_timestamp_exc} - Ending timestamp for requested range, exclusive
        
        B{Return values:}
         - B{messages}
        """
        l_request = skypekit.XCallRequest("ZR\000\210\001", 0, 136)
        l_request.add_parm('e', 1, Message.TYPE[type_])
        l_request.add_parm('b', 2, latest_per_conv_only)
        l_request.add_parm('u', 3, from_timestamp_inc)
        l_request.add_parm('u', 4, to_timestamp_exc)
        l_response = self.transport.xcall(l_request)
        l_result = [module_id2classes[9](oid, self.transport) for oid in l_response.get(1, [])]
        return l_result
    def OnMessage(
        self,
        message,
        changes_inbox_timestamp,
        supersedes_history_message,
        conversation
        ):
        """
        No description available.
        
        B{Return values:}
         - B{message}
         - B{changes_inbox_timestamp} - if changesInboxTimestamp==true is a hint that tray alert should probably be displayed
         - B{supersedes_history_message} - DEPRECATED, not set anymore
         - B{conversation}
        """
        pass
    event_handlers[5] = "_sk_on_message"
    def _sk_on_message(self, parms):
        """ internal event dispatcher for OnMessage
  """
        l_cleanparms = (module_id2classes[9](parms.get(1), self.transport)),
        l_cleanparms += (parms.get(2, False)),
        l_cleanparms += (module_id2classes[9](parms.get(3), self.transport)),
        l_cleanparms += (module_id2classes[18](parms.get(4), self.transport)),
        self.OnMessage(*l_cleanparms)
        l_cleanparms[3].OnMessage(l_cleanparms[0])
    def GetAvailableVideoDevices(self):
        """
        This method returns a table in form of two string lists of equal length and an uint argument that returns the count of items i both lists. The first list contains video recording device handles and the second list descriptive names of those devices. NB! This method requires videortphost to be running, otherwise it will return empty lists. 
        
        
        B{Return values:}
         - B{device_names}
         - B{device_paths}
         - B{count}
        """
        l_request = skypekit.XCallRequest("ZR\000P", 0, 80)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, [])),
        l_result += (l_response.get(2, [])),
        l_result += (l_response.get(3, 0)),
        return l_result
    def HasVideoDeviceCapability(
        self,
        device_name,
        device_path,
        cap
        ):
        """
        Queries whether the given video device has a specific Video::VIDEO_DEVICE_CAPABILITY. Use Skype::GetAvailableVideoDevices method to retrieve sstring lists with available deviceName and devicePath values. 
        
        
        B{Arguments:}
         - B{device_name} - Human readable device name. 
        
         - B{device_path} - Device ID. 
        
         - B{cap} - Any of the Video::VIDEO_DEVICE_CAPABILITY values. 
        
        
        B{Return values:}
         - B{result} - Returns true if the given device has given capability. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000!", 0, 33)
        l_request.add_parm('S', 1, device_name)
        l_request.add_parm('S', 2, device_path)
        l_request.add_parm('e', 3, Video.VIDEO_DEVICE_CAPABILITY[cap])
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def DisplayVideoDeviceTuningDialog(
        self,
        device_name,
        device_path
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{device_name}
         - B{device_path}
        """
        l_request = skypekit.XCallRequest("ZR\000\042", 0, 34)
        l_request.add_parm('S', 1, device_name)
        l_request.add_parm('S', 2, device_path)
        self.transport.xcall(l_request)
    def GetPreviewVideo(
        self,
        type_,
        device_name = "",
        device_path = ""
        ):
        """
        Warning: Will be deprecated soon
        
        B{Arguments:}
         - B{type_}
         - B{device_name} - name and path to be used only with media type VIDEO
         - B{device_path}
        
        B{Return values:}
         - B{video}
        """
        l_request = skypekit.XCallRequest("ZR\000#", 0, 35)
        l_request.add_parm('e', 1, Video.MEDIATYPE[type_])
        l_request.add_parm('S', 2, device_name)
        l_request.add_parm('S', 3, device_path)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[11](l_response.get(1), self.transport)
        return l_result
    def VideoCommand(
        self,
        command
        ):
        """
        Avaible to Video Engines using the Video RTP API
        
        B{Arguments:}
         - B{command}
        
        B{Return values:}
         - B{response}
        """
        l_request = skypekit.XCallRequest("ZR\000;", 0, 59)
        l_request.add_parm('S', 1, command)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def OnAvailableVideoDeviceListChange(self):
        """
        This callback gets fired when there are changes in the system video device list (USB webcam gets plugged in or is detached.) 
        
        """
        pass
    def _sk_on_available_video_device_list_change(self, parms):
        """ internal event dispatcher for OnAvailableVideoDeviceListChange
  """
        self.OnAvailableVideoDeviceListChange()
    event_handlers[7] = "_sk_on_available_video_device_list_change"
    def OnH264Activated(self):
        """
        Event is implemented only in SkypeKit builds. Fired when Skype video library uses software H264 codec for the first time on the particular hardware by particular SkypeKit-based application
        """
        pass
    def _sk_on_h264activated(self, parms):
        """ internal event dispatcher for OnH264Activated
  """
        self.OnH264Activated()
    event_handlers[44] = "_sk_on_h264activated"
    def GetGreeting(
        self,
        skype_name
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{skype_name}
        
        B{Return values:}
         - B{greeting}
        """
        l_request = skypekit.XCallRequest("ZR\000-", 0, 45)
        l_request.add_parm('S', 1, skype_name)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[7](l_response.get(1), self.transport)
        return l_result
    SETUPKEY_DISABLED_CODECS = "*Lib/Audio/DisableCodecs"
    SETUPKEY_DISABLE_AEC = "*Lib/Audio/DisableAEC"
    SETUPKEY_DISABLE_NOISE_SUPPRESSOR = "*Lib/Audio/DisableNS"
    SETUPKEY_DISABLE_AGC = "*Lib/Audio/DisableAGC"
    SETUPKEY_DISABLE_DIGITAL_NEAR_END_AGC = "*Lib/Audio/DisableDigitalNearEndAGC"
    SETUPKEY_DISABLE_DIGITAL_FAR_END_AGC = "*Lib/Audio/DisableDigitalFarEndAGC"
    SETUPKEY_BEAMFORMER_MIC_SPACING = "*Lib/Audio/BeamformerMicSpacing"
    SETUPKEY_DISABLE_AUDIO_DEVICE_PROBING = "*Lib/QualityMonitor/DisableAudioDeviceProbing"
    PREPARESOUNDRESULT = {0:'PREPARESOUND_SUCCESS', 'PREPARESOUND_SUCCESS':0, 1:'PREPARESOUND_MISC_ERROR', 'PREPARESOUND_MISC_ERROR':1, 2:'PREPARESOUND_FILE_NOT_FOUND', 'PREPARESOUND_FILE_NOT_FOUND':2, 3:'PREPARESOUND_FILE_TOO_BIG', 'PREPARESOUND_FILE_TOO_BIG':3, 4:'PREPARESOUND_FILE_READ_ERROR', 'PREPARESOUND_FILE_READ_ERROR':4, 5:'PREPARESOUND_UNSUPPORTED_FILE_FORMAT', 'PREPARESOUND_UNSUPPORTED_FILE_FORMAT':5, 6:'PREPARESOUND_PLAYBACK_NOT_SUPPORTED', 'PREPARESOUND_PLAYBACK_NOT_SUPPORTED':6}
    """
     - PREPARESOUND_SUCCESS
     - PREPARESOUND_MISC_ERROR
     - PREPARESOUND_FILE_NOT_FOUND
     - PREPARESOUND_FILE_TOO_BIG
     - PREPARESOUND_FILE_READ_ERROR
     - PREPARESOUND_UNSUPPORTED_FILE_FORMAT
     - PREPARESOUND_PLAYBACK_NOT_SUPPORTED
    """
    AUDIODEVICE_CAPABILITIES = {
        1
   :'HAS_VIDEO_CAPTURE',
        2
   :'HAS_USB_INTERFACE',
        4
   :'POSSIBLY_HEADSET',
        8
   :'HAS_AUDIO_CAPTURE',
        16
  :'HAS_AUDIO_RENDERING',
        32
  :'HAS_LOWBANDWIDTH_CAPTURE',
        64
  :'IS_WEBCAM',
        128 :'IS_HEADSET',
        256 :'POSSIBLY_WEBCAM',
        2048:'HAS_VIDEO_RENDERING',
        4096:'HAS_BLUETOOTH_INTERFACE',
        'HAS_VIDEO_CAPTURE'
        
                
         :   1,
        'HAS_USB_INTERFACE'
        
                
         :   2,
        'POSSIBLY_HEADSET'
        
                
          :   4,
        'HAS_AUDIO_CAPTURE'
        
                
         :   8,
        'HAS_AUDIO_RENDERING'
        
                       :  16,
        'HAS_LOWBANDWIDTH_CAPTURE'
        
                  :  32,
        'IS_WEBCAM'
        
                
                 :  64,
        'IS_HEADSET'
        
                
                : 128,
        'POSSIBLY_WEBCAM'
        
                
           : 256,
        'HAS_VIDEO_RENDERING'
        
                       :2048,
        'HAS_BLUETOOTH_INTERFACE'
        
                   :4096
    }
    """
     - HAS_VIDEO_CAPTURE
     - HAS_USB_INTERFACE
     - POSSIBLY_HEADSET
     - HAS_AUDIO_CAPTURE
     - HAS_AUDIO_RENDERING
     - HAS_LOWBANDWIDTH_CAPTURE
     - IS_WEBCAM
     - IS_HEADSET
     - POSSIBLY_WEBCAM
     - HAS_VIDEO_RENDERING
     - HAS_BLUETOOTH_INTERFACE
    """
    def PlayStart(
        self,
        soundid,
        sound,
        loop,
        use_call_out_device = False
        ):
        """
        Takes audio data that comes from the sound argument and mixes it into playback or notification device, depending on the value passed in the useCallOutDevice argument. The sound argument contains the audio data in in follwing format: first 4 bytes of the binary contain the sample rate, followed by 16 bit (mono) samples. The soundid argument is an arbitrary ID that you can pass in and then later use as an argument for Skype class PlayStop method. To mix the audio into playback device stream, set useCallOutDevice to true, to mic it into notification stream, set useCallOutDevice to false. 
        
        
        B{Arguments:}
         - B{soundid}
         - B{sound}
         - B{loop}
         - B{use_call_out_device}
        """
        l_request = skypekit.XCallRequest("ZR\000\060", 0, 48)
        l_request.add_parm('u', 1, soundid)
        l_request.add_parm('B', 2, sound)
        l_request.add_parm('b', 3, loop)
        l_request.add_parm('b', 4, use_call_out_device)
        self.transport.xcall(l_request)
    def PlayStartFromFile(
        self,
        soundid,
        datafile,
        loop,
        use_call_out_device = False
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{soundid}
         - B{datafile}
         - B{loop}
         - B{use_call_out_device}
        
        B{Return values:}
         - B{result}
        """
        l_request = skypekit.XCallRequest("ZR\000\324\001", 0, 212)
        l_request.add_parm('u', 1, soundid)
        l_request.add_parm('f', 2, datafile)
        l_request.add_parm('b', 3, loop)
        l_request.add_parm('b', 4, use_call_out_device)
        l_response = self.transport.xcall(l_request)
        l_result = skypekit.enumof(Skype.PREPARESOUNDRESULT, l_response.get(1))
        return l_result
    def PlayStop(
        self,
        soundid
        ):
        """
        Stops playback of the soundfile. The argument is the same ID you passed in the Skype class StartPlayback method. 
        
        
        B{Arguments:}
         - B{soundid}
        """
        l_request = skypekit.XCallRequest("ZR\000\061", 0, 49)
        l_request.add_parm('u', 1, soundid)
        self.transport.xcall(l_request)
    def StartRecordingTest(
        self,
        record_and_playback_data = False
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{record_and_playback_data}
        """
        l_request = skypekit.XCallRequest("ZR\000\062", 0, 50)
        l_request.add_parm('b', 1, record_and_playback_data)
        self.transport.xcall(l_request)
    def StopRecordingTest(self):
        """
        No description available.
        """
        l_request = skypekit.XCallRequest("ZR\000\063", 0, 51)
        self.transport.xcall(l_request)
    def GetAvailableOutputDevices(self):
        """
        This method returns a table in form of three string lists of equal lengths. The first list contains audio output device handles ('hw:0,0', 'hw:0,1', etc.) The second list contains descriptive names of those devices (Ensoniq AudioPCI etc.) The third list contains device product IDs. Note that the values in these lists depend on which audio engine you are running (SAL, PCM, RTP). 
        
        
        B{Return values:}
         - B{handle_list}
         - B{name_list}
         - B{product_id_list}
        """
        l_request = skypekit.XCallRequest("ZR\000\065", 0, 53)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, [])),
        l_result += (l_response.get(2, [])),
        l_result += (l_response.get(3, [])),
        return l_result
    def GetAvailableRecordingDevices(self):
        """
        This method returns a table in form of three string lists of equal length. The first list contains audio recording device handles ('hw:0,0', 'hw:0,1', etc.) The second list contains descriptive names of those devices (Ensoniq AudioPCI etc.) The third list contains device product IDs. Note that the values in these lists depend on which audio engine you are running (SAL, PCM, RTP). 
        
        
        B{Return values:}
         - B{handle_list}
         - B{name_list}
         - B{product_id_list}
        """
        l_request = skypekit.XCallRequest("ZR\000\066", 0, 54)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, [])),
        l_result += (l_response.get(2, [])),
        l_result += (l_response.get(3, [])),
        return l_result
    def SelectSoundDevices(
        self,
        call_in_device,
        call_out_device,
        wave_out_device
        ):
        """
        Sets audio devices given in arguments as active audio devices. This command selects all three devices - microphone, playback and the notification channel. Valid input values for this method come from the first string list you get back from Skype class GetAvailableOutputDevices (handleList). 
        
        
        B{Arguments:}
         - B{call_in_device}
         - B{call_out_device}
         - B{wave_out_device}
        """
        l_request = skypekit.XCallRequest("ZR\000\067", 0, 55)
        l_request.add_parm('S', 1, call_in_device)
        l_request.add_parm('S', 2, call_out_device)
        l_request.add_parm('S', 3, wave_out_device)
        self.transport.xcall(l_request)
    def GetAudioDeviceCapabilities(
        self,
        device_handle
        ):
        """
        The uint argument returns AUDIODEVICE_CAPABILITIES (declared in Skype class) 
        
        
        B{Arguments:}
         - B{device_handle}
        
        B{Return values:}
         - B{interface_string}
         - B{capabilities} - bit set of AUDIODEVICE_CAPABILITIES
        """
        l_request = skypekit.XCallRequest("ZR\000\070", 0, 56)
        l_request.add_parm('S', 1, device_handle)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, '')),
        l_result += (l_response.get(2, 0)),
        return l_result
    def GetNrgLevels(self):
        """
        Returns current audio stream volume for both playback and microphone streams. Useful for displaying visual audio indicators in you UI. See also Skype class OnNrgLevelsChange callback that gets fired each time the these values are changed. 
        
        
        B{Return values:}
         - B{mic_level}
         - B{speaker_level}
        """
        l_request = skypekit.XCallRequest("ZR\000\071", 0, 57)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, 0)),
        l_result += (l_response.get(2, 0)),
        return l_result
    def VoiceCommand(
        self,
        command
        ):
        """
        NB! This command only works if its implemented in external audiohost (RTP or PCM host). The command can be is used for passing custom commands from client UI to the audio implementation. 
        
        
        B{Arguments:}
         - B{command}
        
        B{Return values:}
         - B{response}
        """
        l_request = skypekit.XCallRequest("ZR\000:", 0, 58)
        l_request.add_parm('S', 1, command)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def GetSpeakerVolume(self):
        """
        Returns value of audio playback volume setting (0..100). 
        
        
        B{Return values:}
         - B{volume}
        """
        l_request = skypekit.XCallRequest("ZR\000<", 0, 60)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, 0)
        return l_result
    def SetSpeakerVolume(
        self,
        volume
        ):
        """
        This method is for setting speaker volume. It will set the level for Skype digital gain control. Skype audio library will not control gain of audio device itself. 
        
        
        B{Arguments:}
         - B{volume}
        """
        l_request = skypekit.XCallRequest("ZR\000=", 0, 61)
        l_request.add_parm('u', 1, volume)
        self.transport.xcall(l_request)
    def GetMicVolume(self):
        """
        Returns value of microphone volume setting (0..100). It will return the analog gain of audio device set by Skype AGC. For real-time microphone volume, use GetNrgLevels method or OnNrgLevelsChange callback (both are methods of Skype class). 
        
        
        B{Return values:}
         - B{mic_volume}
        """
        l_request = skypekit.XCallRequest("ZR\000>", 0, 62)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, 0)
        return l_result
    def SetMicVolume(
        self,
        volume
        ):
        """
        This method is for setting the microphone volume level. This does not work when Skype AGC (Automatic Gain Control) is enabled, which it is by default. It is currently impossible to disable AGC, so for now this method is here for purpose of future compatibility. 
        
        
        B{Arguments:}
         - B{volume}
        """
        l_request = skypekit.XCallRequest("ZR\000?", 0, 63)
        l_request.add_parm('u', 1, volume)
        self.transport.xcall(l_request)
    def IsSpeakerMuted(self):
        """
        Returns true in &muted argument if the currently selected playback device is muted. 
        
        
        B{Return values:}
         - B{muted}
        """
        l_request = skypekit.XCallRequest("ZR\000@", 0, 64)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def IsMicrophoneMuted(self):
        """
        Returns true in &muted argument if the currently selected microphone is muted. 
        
        
        B{Return values:}
         - B{muted}
        """
        l_request = skypekit.XCallRequest("ZR\000\101", 0, 65)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def MuteSpeakers(
        self,
        mute
        ):
        """
        Sets currently selected playback device mute status according to argument. 
        
        
        B{Arguments:}
         - B{mute}
        """
        l_request = skypekit.XCallRequest("ZR\000\102", 0, 66)
        l_request.add_parm('b', 1, mute)
        self.transport.xcall(l_request)
    def MuteMicrophone(
        self,
        mute
        ):
        """
        Sets currently selected microphone mute status according to argument. 
        
        
        B{Arguments:}
         - B{mute}
        """
        l_request = skypekit.XCallRequest("ZR\000\103", 0, 67)
        l_request.add_parm('b', 1, mute)
        self.transport.xcall(l_request)
    def OnAvailableDeviceListChange(self):
        """
        This callback gets fired when there are changes in the system audio device list (USB headset gets plugged in or is detached.) 
        
        """
        pass
    def _sk_on_available_device_list_change(self, parms):
        """ internal event dispatcher for OnAvailableDeviceListChange
  """
        self.OnAvailableDeviceListChange()
    event_handlers[10] = "_sk_on_available_device_list_change"
    def OnNrgLevelsChange(self):
        """
        This callback gets fired when the audio strength changes in either playback or recording audio streams. Useful for providing visual indicators of audio activity in your UI. 
        
        """
        pass
    def _sk_on_nrg_levels_change(self, parms):
        """ internal event dispatcher for OnNrgLevelsChange
  """
        self.OnNrgLevelsChange()
    event_handlers[11] = "_sk_on_nrg_levels_change"
    OPERATING_MEDIA = {0:'OM_UNKNOWN', 'OM_UNKNOWN':0, 1:'OM_FREE', 'OM_FREE':1, 2:'OM_FREE_WIRELESS', 'OM_FREE_WIRELESS':2, 3:'OM_3G', 'OM_3G':3, 4:'OM_4G', 'OM_4G':4}
    """
     - OM_UNKNOWN
     - OM_FREE
     - OM_FREE_WIRELESS
     - OM_3G
     - OM_4G
    """
    def SetOperatingMedia(
        self,
        media,
        max_uplink_bps,
        max_downlink_bps
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{media}
         - B{max_uplink_bps}
         - B{max_downlink_bps}
        """
        l_request = skypekit.XCallRequest("ZR\000\377\001", 0, 255)
        l_request.add_parm('e', 1, Skype.OPERATING_MEDIA[media])
        l_request.add_parm('u', 2, max_uplink_bps)
        l_request.add_parm('u', 3, max_downlink_bps)
        self.transport.xcall(l_request)
    def RequestConfirmationCode(
        self,
        type_,
        number
        ):
        """
        creates and sends a CONFIRMATION_CODE_REQUEST message this sends a confirmation code to the number provided
        
        B{Arguments:}
         - B{type_}
         - B{number}
        
        B{Return values:}
         - B{sms}
        """
        l_request = skypekit.XCallRequest("ZR\000\035", 0, 29)
        l_request.add_parm('e', 1, Sms.CONFIRM_TYPE[type_])
        l_request.add_parm('S', 2, number)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[12](l_response.get(1), self.transport)
        return l_result
    def SubmitConfirmationCode(
        self,
        number,
        code
        ):
        """
        creates and sends a CONFIRMATION_CODE_SUBMIT message this authorizes the number with the server for the purpose given in RequestConfirmationCode
        
        B{Arguments:}
         - B{number}
         - B{code}
        
        B{Return values:}
         - B{sms}
        """
        l_request = skypekit.XCallRequest("ZR\000\036", 0, 30)
        l_request.add_parm('S', 1, number)
        l_request.add_parm('S', 2, code)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[12](l_response.get(1), self.transport)
        return l_result
    def CreateOutgoingSms(self):
        """
        creates an OUTGOING/COMPOSING SMS message
        
        B{Return values:}
         - B{sms}
        """
        l_request = skypekit.XCallRequest("ZR\000\106", 0, 70)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[12](l_response.get(1), self.transport)
        return l_result
    SETUPKEY_FT_AUTOACCEPT = "Lib/FileTransfer/AutoAccept"
    SETUPKEY_FT_SAVEPATH = "Lib/FileTransfer/SavePath"
    SETUPKEY_FT_INCOMING_LIMIT = "Lib/FileTransfer/IncomingLimit"
    SETUPKEY_IDLE_TIME_FOR_AWAY = "Lib/Account/IdleTimeForAway"
    SETUPKEY_IDLE_TIME_FOR_NA = "Lib/Account/IdleTimeForNA"
    def GetAccount(
        self,
        identity
        ):
        """
        Retrieves an Account object by Skype name (identity). This should normally be one of the first method calls after Skype object initialization. Nearly all the other methods require successful account login in order to work properly. The list of accounts that have been used on the local machine/database can be retrieved with Skype::GetExistingAccounts method. If a matching identity is not found, a new Account object is created. This object can then be used to populate requred fields and then use Account::Register method for new account creation. This method returns false on error. 
        
        
        B{Arguments:}
         - B{identity} - Account skypename. 
        
        
        B{Return values:}
         - B{account} - Returns account object if successful. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000s", 0, 115)
        l_request.add_parm('S', 1, identity)
        l_response = self.transport.xcall(l_request)
        l_result = module_id2classes[5](l_response.get(1), self.transport)
        return l_result
    def GetExistingAccounts(self):
        """
        Returns a list of possible profiles used before on this machine
        
        B{Return values:}
         - B{account_name_list}
        """
        l_request = skypekit.XCallRequest("ZR\000q", 0, 113)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, [])
        return l_result
    def GetDefaultAccountName(self):
        """
        return most recently used account that has pwd saved. empty string if none
        
        B{Return values:}
         - B{account}
        """
        l_request = skypekit.XCallRequest("ZR\000r", 0, 114)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def GetSuggestedSkypename(
        self,
        fullname
        ):
        """
        suggest a nice skypename to go with given fullname
        
        B{Arguments:}
         - B{fullname}
        
        B{Return values:}
         - B{suggested_name}
        """
        l_request = skypekit.XCallRequest("ZR\000t", 0, 116)
        l_request.add_parm('S', 1, fullname)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    VALIDATERESULT = {
        0 :'NOT_VALIDATED',
        1 :'VALIDATED_OK',
        2 :'TOO_SHORT',
        3 :'TOO_LONG',
        4 :'CONTAINS_INVALID_CHAR',
        5 :'CONTAINS_SPACE',
        6 :'SAME_AS_USERNAME',
        7 :'INVALID_FORMAT',
        8 :'CONTAINS_INVALID_WORD',
        9 :'TOO_SIMPLE',
        10:'STARTS_WITH_INVALID_CHAR',
        'NOT_VALIDATED'
        
                
             : 0,
        'VALIDATED_OK'
        
                
              : 1,
        'TOO_SHORT'
        
                
                 : 2,
        'TOO_LONG'
        
                
                  : 3,
        'CONTAINS_INVALID_CHAR'
        
                     : 4,
        'CONTAINS_SPACE'
        
                
            : 5,
        'SAME_AS_USERNAME'
        
                
          : 6,
        'INVALID_FORMAT'
        
                
            : 7,
        'CONTAINS_INVALID_WORD'
        
                     : 8,
        'TOO_SIMPLE'
        
                
                : 9,
        'STARTS_WITH_INVALID_CHAR'
        
                  :10
    }
    """
    A value of this type can be returned by one of the following methods (of Skype class): ValidateAvatar, ValidateProfileString, ValidatePassword. 
    
     - NOT_VALIDATED - Given property could not be validated. The length of the field was within limits and the value is assumed to be Ok. Your client should treat this value as equivalent to VALIDATED_OK. 
    
     - VALIDATED_OK - Avatar or profile string validation succeeded. 
    
     - TOO_SHORT - Password is too short. 
    
     - TOO_LONG - The value exceeds max size limit for the given property. 
    
     - CONTAINS_INVALID_CHAR - Value contains illegal characters. 
    
     - CONTAINS_SPACE - Value contains whitespace. 
    
     - SAME_AS_USERNAME - Password cannot be the same as skypename. 
    
     - INVALID_FORMAT - Value has invalid format. 
    
     - CONTAINS_INVALID_WORD - Value contains invalid word. 
    
     - TOO_SIMPLE - Password is too simple. 
    
     - STARTS_WITH_INVALID_CHAR - Value starts with an invalid character.
     
    """
    def ValidateAvatar(
        self,
        value
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{value}
        
        B{Return values:}
         - B{result}
         - B{free_bytes_left}
        """
        l_request = skypekit.XCallRequest("ZR\000w", 0, 119)
        l_request.add_parm('B', 1, value)
        l_response = self.transport.xcall(l_request)
        l_result = (skypekit.enumof(Skype.VALIDATERESULT, l_response.get(1))),
        l_result += (l_response.get(2, 0)),
        return l_result
    def ValidateProfileString(
        self,
        prop_key,
        str_value,
        for_registration = False
        ):
        """
        This method should be used for validating skypenames before registering new accounts, if the propKey is set to SKYPENAME (Contact class) and forRegistration argument is set to true. If the forRegistration argument is false, only string length check is applied. It is also useful to probe, what the size limits are, for each string property (e.g. 300 characters for moodmessage) 
        
        
        B{Arguments:}
         - B{prop_key}
         - B{str_value}
         - B{for_registration}
        
        B{Return values:}
         - B{result}
         - B{free_bytes_left}
        """
        l_request = skypekit.XCallRequest("ZR\000f", 0, 102)
        l_request.add_parm('e', 1, prop_key)
        l_request.add_parm('S', 2, str_value)
        l_request.add_parm('b', 3, for_registration)
        l_response = self.transport.xcall(l_request)
        l_result = (skypekit.enumof(Skype.VALIDATERESULT, l_response.get(1))),
        l_result += (l_response.get(2, 0)),
        return l_result
    def ValidatePassword(
        self,
        username,
        password
        ):
        """
        This method is for pre-validating account passwords before account creation or password change. The result will return either VALIDATED_OK or one of many possible reasons the password is unacceptable (too short, too simple, etc.) 
        
        
        B{Arguments:}
         - B{username}
         - B{password}
        
        B{Return values:}
         - B{result}
        """
        l_request = skypekit.XCallRequest("ZR\000G", 0, 71)
        l_request.add_parm('S', 1, username)
        l_request.add_parm('S', 2, password)
        l_response = self.transport.xcall(l_request)
        l_result = skypekit.enumof(Skype.VALIDATERESULT, l_response.get(1))
        return l_result
    SETUPKEY_PORT = "*Lib/Connection/Port"
    SETUPKEY_HTTPS_PROXY_ENABLE = "*Lib/Connection/HttpsProxy/Enable"
    SETUPKEY_HTTPS_PROXY_ADDR = "*Lib/Connection/HttpsProxy/Addr"
    SETUPKEY_HTTPS_PROXY_USER = "*Lib/Connection/HttpsProxy/User"
    SETUPKEY_HTTPS_PROXY_PWD = "*Lib/Connection/HttpsProxy/Pwd"
    SETUPKEY_SOCKS_PROXY_ENABLE = "*Lib/Connection/SocksProxy/Enable"
    SETUPKEY_SOCKS_PROXY_ADDR = "*Lib/Connection/SocksProxy/Addr"
    SETUPKEY_SOCKS_PROXY_USER = "*Lib/Connection/SocksProxy/User"
    SETUPKEY_SOCKS_PROXY_PWD = "*Lib/Connection/SocksProxy/Pwd"
    SETUPKEY_LOCALADDRESS = "*Lib/Connection/LocalAddress"
    SETUPKEY_DISABLE_PORT80 = "*Lib/Connection/DisablePort80"
    SETUPKEY_DISABLE_UDP = "*Lib/Connection/DisableUDP"
    PROXYTYPE = {0:'HTTPS_PROXY', 'HTTPS_PROXY':0, 1:'SOCKS_PROXY', 'SOCKS_PROXY':1}
    """
     - HTTPS_PROXY
     - SOCKS_PROXY
    """
    def OnProxyAuthFailure(
        self,
        type_
        ):
        """
        No description available.
        
        B{Return values:}
         - B{type_}
        """
        pass
    event_handlers[12] = "_sk_on_proxy_auth_failure"
    def _sk_on_proxy_auth_failure(self, parms):
        """ internal event dispatcher for OnProxyAuthFailure
  """
        l_cleanparms = Skype.PROXYTYPE[parms.get(1)]
        self.OnProxyAuthFailure(l_cleanparms)
    def GetUsedPort(self):
        """
        Port number that the runtime ended up listening to. Usually equal to SETUPKEY_PORT. 0 if none used (disconnected or binding failed). 
        
        
        B{Return values:}
         - B{port}
        """
        l_request = skypekit.XCallRequest("ZR\000\202\001", 0, 130)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, 0)
        return l_result
    def GetStr(
        self,
        key
        ):
        """
        This is used for retrieving local setup keys of type string. For more information, see Defines section in the skype-embedded_2.h 
        
        
        B{Arguments:}
         - B{key}
        
        B{Return values:}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\000x", 0, 120)
        l_request.add_parm('S', 1, key)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def GetInt(
        self,
        key
        ):
        """
        This is used for retrieving local setup keys of type int. For more information, see Defines section in the skype-embedded_2.h 
        
        
        B{Arguments:}
         - B{key}
        
        B{Return values:}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\000y", 0, 121)
        l_request.add_parm('S', 1, key)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, 0)
        return l_result
    def GetBin(
        self,
        key
        ):
        """
        This is used for retrieving local setup keys of type binary. For more information, see Defines section in the skype-embedded_2.h 
        
        
        B{Arguments:}
         - B{key}
        
        B{Return values:}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\000z", 0, 122)
        l_request.add_parm('S', 1, key)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def SetStr(
        self,
        key,
        value
        ):
        """
        This is used for setting local setup keys of type string. For more information, see Defines section in the skype-embedded_2.h 
        
        
        B{Arguments:}
         - B{key}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\000{", 0, 123)
        l_request.add_parm('S', 1, key)
        l_request.add_parm('S', 2, value)
        self.transport.xcall(l_request)
    def SetInt(
        self,
        key,
        value
        ):
        """
        This is used for setting local setup keys of type int. For more information, see Defines section in the skype-embedded_2.h 
        
        
        B{Arguments:}
         - B{key}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\000|", 0, 124)
        l_request.add_parm('S', 1, key)
        l_request.add_parm('i', 2, value)
        self.transport.xcall(l_request)
    def SetBin(
        self,
        key,
        value
        ):
        """
        This is used for setting local setup keys of type binary. For more information, see Defines section in the skype-embedded_2.h 
        
        
        B{Arguments:}
         - B{key}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\000}", 0, 125)
        l_request.add_parm('S', 1, key)
        l_request.add_parm('B', 2, value)
        self.transport.xcall(l_request)
    def IsDefined(
        self,
        key
        ):
        """
        Returns true if the given setup key is defined in local setup. For more information, see Defines section in the skype-embedded_2.h 
        
        
        B{Arguments:}
         - B{key}
        
        B{Return values:}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\000~", 0, 126)
        l_request.add_parm('S', 1, key)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def Delete(
        self,
        key
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{key}
        """
        l_request = skypekit.XCallRequest("ZR\000\177", 0, 127)
        l_request.add_parm('S', 1, key)
        self.transport.xcall(l_request)
    def GetSubKeys(
        self,
        key
        ):
        """
        No description available.
        
        B{Arguments:}
         - B{key}
        
        B{Return values:}
         - B{value}
        """
        l_request = skypekit.XCallRequest("ZR\000\200\001", 0, 128)
        l_request.add_parm('S', 1, key)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, [])
        return l_result
    def GetISOLanguageInfo(self):
        """
        Returns two string lists. First of them will contain list of two-letter language codes (ISO 639-1) The second list contains names of corresponding languages. 
        
        
        B{Return values:}
         - B{language_code_list}
         - B{language_name_list} - assumes UI has set correct language
        """
        l_request = skypekit.XCallRequest("ZR\000\317\001", 0, 207)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, [])),
        l_result += (l_response.get(2, [])),
        return l_result
    def GetISOCountryInfo(self):
        """
        Returns three string lists and one int array, containing 2-letter country code, country name, dialing prefix and example dial string (not available for all items). This method does currently return 0 for South Georgia and the South Sandwich Islands. 
        
        
        B{Return values:}
         - B{country_code_list}
         - B{country_name_list} - assumes UI has set correct language
         - B{country_prefix_list}
         - B{country_dial_example_list}
        """
        l_request = skypekit.XCallRequest("ZR\000\320\001", 0, 208)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, [])),
        l_result += (l_response.get(2, [])),
        l_result += (l_response.get(3, [])),
        l_result += (l_response.get(4, [])),
        return l_result
    def GetISOCountryCodebyPhoneNo(
        self,
        number
        ):
        """
        Returns 2-letter country code based on PSTN number. The input argument has to be without + in from of it - '37212345678' will return 'ee' while '+37212345678' will return an empty string. 
        
        
        B{Arguments:}
         - B{number}
        
        B{Return values:}
         - B{country_code}
        """
        l_request = skypekit.XCallRequest("ZR\000\323\001", 0, 211)
        l_request.add_parm('S', 1, number)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, '')
        return l_result
    def App2AppCreate(
        self,
        appname
        ):
        """
        App2AppCreate method constructs a local instance of an App2App application. App2App applications are not abstracted in the SkypeKit API as a separate class. Rather, they can be created with App2AppCreate, supplying their name as ID, and then connected to remote parties using App2AppConnect method. 
        
        App2App portion of the SkypeKit API enables you to implement arbitrary data exchange protocols between Skype clients. Basically, if you are ever tempted to use conversation chat messages for something other than actual human-readable chat - you should consider using your own custom App2App protocol instead. 
        
        The downside of App2App is that all the participants need to be running a client that supports the same App2App application. Although, it is possible to have one side using a custom SkypeKit client and another side using Skype desktop client - App2App is supported in both, in case of desktop client via Public API - you are still limited to remote side running something that can recognize your protocol and react to connection attempts from your side. 
        
        To establish connection between each other, all participants need to create their local instances of the application (with the same ID, and then connect to each other. More than one App2App applications can be active in a local client at the same time. Also, more than two clients can be connected with the same application. 
        
        Once connection is established, you can choose between two communication methods - datagrams and stream read/write methods. Overall, there are not much principal difference between the two. Datagram packet size is limited to 1500 bytes and stream packet size to 32 KB of payload data. Implementation-wise, datagrams are probably somewhat easier to deal with. 
        
        
        B{Arguments:}
         - B{appname} - Application ID. This ID is used by the rest of the App2App commands to differentiate between applications, should there be more than one app2app applications running on the local system. 
        
        
        B{Return values:}
         - B{result} - Returns true if the app creation was successful. Returns false when an application with the same name already exists in the local system. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\327\001", 0, 215)
        l_request.add_parm('S', 1, appname)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def App2AppDelete(
        self,
        appname
        ):
        """
        Disconnects and deletes the App2App application. 
        
        
        B{Arguments:}
         - B{appname} - application ID. 
        
        
        B{Return values:}
         - B{result} - Returns true if the deletion was successful (application with such ID actually existed) 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\330\001", 0, 216)
        l_request.add_parm('S', 1, appname)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def App2AppConnect(
        self,
        appname,
        skypename
        ):
        """
        The App2AppConnect connect result does not mean the connection was immediately established. It will return Ok even if the remote party was offline. The actual connection will be established when both parties have fired App2AppConnect with the same application name, at eachother. At that point, OnApp2AppStreamListChange event will fire for both local and remote party (with listType argument set to ALL_STREAMS) and you can start exchanging data, using either App2App datagrams or App2AppRead App2AppWrite methods. 
        
        
        B{Arguments:}
         - B{appname} - Application ID. This needs to match with application ID connecting from the remote side. 
        
         - B{skypename} - Skype Name of the remote party. 
        
        
        B{Return values:}
         - B{result} - NB! This argument will return true even if the remote party has not yet connected (or is not even online yet) - it merely indicates that the connect command was successfuly processed in runtime. The actual connection success will be indicated when the OnApp2AppStreamListChange event fires, i.e. when App2App stream will be established between connecting parties. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\331\001", 0, 217)
        l_request.add_parm('S', 1, appname)
        l_request.add_parm('S', 2, skypename)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def App2AppDisconnect(
        self,
        appname,
        stream
        ):
        """
        Disconnects an App2App stream. This is different from App2AppDelete command in that it enables you to disconnect remote parties selectively - in case there are more than two participants in the App2App stream pool. 
        
        
        B{Arguments:}
         - B{appname} - application ID 
        
         - B{stream} - stream ID. 
        
        
        B{Return values:}
         - B{result} - returns true when the stream disconnect was successful. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\332\001", 0, 218)
        l_request.add_parm('S', 1, appname)
        l_request.add_parm('S', 2, stream)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def App2AppWrite(
        self,
        appname,
        stream,
        data
        ):
        """
        Sends a stream packet to the remote party specified in the stream argument. The max size of stream write packet is 32KB. After calling this method, OnApp2AppStreamListChange will fire for both parties. In local ssytem with listType set to SENDING_STREAMS and on remote system with listType set to RECEIVED_STREAMS. This event can be used to read received packets out of the stream. 
        
        
        B{Arguments:}
         - B{appname} - application ID 
        
         - B{stream} - stream ID 
        
         - B{data} - packet payload 
        
        
        B{Return values:}
         - B{result} - returns true if the call was successful. Note that this does indicate the packet was actually received by remote party. 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\333\001", 0, 219)
        l_request.add_parm('S', 1, appname)
        l_request.add_parm('S', 2, stream)
        l_request.add_parm('B', 3, data)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def App2AppDatagram(
        self,
        appname,
        stream,
        data
        ):
        """
        Sends a datagram to the remote party specified in the stream argument. The max size of datagram payload is 1500 bytes. 
        
        
        B{Arguments:}
         - B{appname} - application ID - from App2AppCreate 
        
         - B{stream} - stream ID - either from App2AppGetStreamsList or from OnApp2AppStreamListChange
         - B{data} - datagram payload (max 1500 bytes)
        
        B{Return values:}
         - B{result} - returns true on method success. Note that this does mean the remote party has actually received your datagram - that sort of feedback, should you want it, is up to you to implement in your custom protocol.
        """
        l_request = skypekit.XCallRequest("ZR\000\334\001", 0, 220)
        l_request.add_parm('S', 1, appname)
        l_request.add_parm('S', 2, stream)
        l_request.add_parm('B', 3, data)
        l_response = self.transport.xcall(l_request)
        l_result = l_response.get(1, False)
        return l_result
    def App2AppRead(
        self,
        appname,
        stream
        ):
        """
        Reads data from the specified stream. This method should be called from the OnApp2AppStreamListChange event callback, when this callback gets fired with listType argument set to RECEIVED_STREAMS. 
        
        
        B{Arguments:}
         - B{appname} - application ID 
        
         - B{stream} - stream ID 
        
        
        B{Return values:}
         - B{result} - returns true on method success. Note that this does mean the remote party has actually received your packet - that sort of feedback, should you want it, is up to you to implement in your custom protocol. 
        
         - B{data} - stream packet payload 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\335\001", 0, 221)
        l_request.add_parm('S', 1, appname)
        l_request.add_parm('S', 2, stream)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, False)),
        l_result += (l_response.get(2, '')),
        return l_result
    def App2AppGetConnectableUsers(
        self,
        appname
        ):
        """
        App2AppGetConnectableUsers returns a list of currently online contacts. It does not return a list of contacts who have an app2app application running. There is currently no way of querying whether an application has been launched on the remote side - other than trying to connect to the remote side and waiting for timeout. NB! if you use App2AppGetConnectableUsers immediately after login - then the online presence of your contact list has not yet updated itself - so this method will most likely return either an empty list or a list with echo123 in it. 
        
        
        B{Arguments:}
         - B{appname} - application ID 
        
        
        B{Return values:}
         - B{result} - returns true on method success 
        
         - B{users} - stringlist with Skype Names of connectable users 
        
        """
        l_request = skypekit.XCallRequest("ZR\000\336\001", 0, 222)
        l_request.add_parm('S', 1, appname)
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, False)),
        l_result += (l_response.get(2, [])),
        return l_result
    APP2APP_STREAMS = {0:'ALL_STREAMS', 'ALL_STREAMS':0, 1:'SENDING_STREAMS', 'SENDING_STREAMS':1, 2:'RECEIVED_STREAMS', 'RECEIVED_STREAMS':2}
    """
     - ALL_STREAMS
     - SENDING_STREAMS
     - RECEIVED_STREAMS
    """
    def App2AppGetStreamsList(
        self,
        appname,
        list_type
        ):
        """
        Takes application ID and list type filter and returns a stringlist with streamID's that pass the filter. 
        
        
        B{Arguments:}
         - B{appname} - application ID 
        
         - B{list_type} - list type filter 
        
        
        B{Return values:}
         - B{result} - results true if the method call was successful 
        
         - B{streams} - string list with stream IDs 
        
         - B{received_sizes} - For RECEIVED_STREAMS, contains the number of bytes in each stream waiting to be read
        """
        l_request = skypekit.XCallRequest("ZR\000\337\001", 0, 223)
        l_request.add_parm('S', 1, appname)
        l_request.add_parm('e', 2, Skype.APP2APP_STREAMS[list_type])
        l_response = self.transport.xcall(l_request)
        l_result = (l_response.get(1, False)),
        l_result += (l_response.get(2, [])),
        l_result += (l_response.get(3, [])),
        return l_result
    def OnApp2AppDatagram(
        self,
        appname,
        stream,
        data
        ):
        """
        This event gets fired on incoming app2app datagram. 
        
        
        B{Return values:}
         - B{appname} - App2app application ID. 
        
         - B{stream} - App2app stream ID - see OnApp2AppStreamListChange event for obtaining stream IDs. 
        
         - B{data} - Datagram payload - limited to 1500 bytes. 
        
        """
        pass
    event_handlers[6] = "_sk_on_app2app_datagram"
    def _sk_on_app2app_datagram(self, parms):
        """ internal event dispatcher for OnApp2AppDatagram
  """
        l_cleanparms = (parms.get(1, '')),
        l_cleanparms += (parms.get(2, '')),
        l_cleanparms += (parms.get(3, '')),
        self.OnApp2AppDatagram(*l_cleanparms)
    def OnApp2AppStreamListChange(
        self,
        appname,
        list_type,
        streams,
        received_sizes
        ):
        """
        In context of datagrams, this event will fire when: 
          - Connection is established between two app2app applications. That is, when both parties have an app up with the same name and -both- used App2AppConnect In that case, both parties get this event, with listType ALL_STREAMS 
          - When a datagram is sent, the sender will get this event with listType SENDING_STREAMS Receiver of the datagram will get OnApp2AppDatagram event instead. 
          - When the remote party drops app2app connection, the local user will get OnApp2AppStreamListChange with listType ALL_STREAMS and streams.size() zero. 
        In context of stream reads/writes, this event will fire for both the sender (listType == SENDING_STREAMS)and the receiver (listType == RECEIVED_STREAMS). For receiver side, this is the place to put your reading code - App2AppRead. 
        
        
        B{Return values:}
         - B{appname} - application ID - the name you supplied in App2AppCreate. 
        
         - B{list_type} - application list type (read/write/all) 
        
         - B{streams} - SEStringlist with affected stream IDs. 
        
         - B{received_sizes} - For RECEIVED_STREAMS, contains the number of bytes in each stream waiting to be read
        """
        pass
    event_handlers[8] = "_sk_on_app2app_stream_list_change"
    def _sk_on_app2app_stream_list_change(self, parms):
        """ internal event dispatcher for OnApp2AppStreamListChange
  """
        l_cleanparms = (parms.get(1, '')),
        l_cleanparms += (Skype.APP2APP_STREAMS[parms.get(2)]),
        l_cleanparms += (parms.get(3, [])),
        l_cleanparms += (parms.get(4, [])),
        self.OnApp2AppStreamListChange(*l_cleanparms)

def GetSkype(apptoken, has_event_thread=True, host='127.0.0.1', port=8963, logtransport=False, secure=True):
    setup = "SkypeKit/SubscribedProperties=10:155,10:154,10:151,10:152,10:153,2:202,2:4,2:6,2:5,2:7,2:8,2:9,2:10,2:11,2:12,2:13,2:14,2:15,2:16,2:17,2:18,2:37,2:26,2:205,2:27,2:36,2:19,2:28,2:29,2:182,2:183,2:20,2:25,2:35,2:34,2:21,2:22,2:23,2:33,2:180,2:39,2:41,2:184,2:185,2:186,2:187,2:188,2:189,2:42,1:200,19:930,19:931,19:932,19:933,19:934,19:935,19:936,19:943,19:938,19:948,19:939,19:941,19:942,19:947,19:949,19:950,19:951,19:952,19:953,19:954,19:955,18:972,18:902,18:918,18:974,18:996,18:920,18:921,18:925,18:924,18:927,18:928,18:973,18:975,18:976,18:977,18:970,18:971,18:979,18:981,18:915,18:903,18:904,18:919,18:922,18:906,18:907,18:909,18:980,18:910,18:911,18:913,18:914,9:960,9:120,9:122,9:123,9:792,9:790,9:121,9:961,9:962,9:968,9:222,9:223,9:963,9:964,9:127,9:125,9:966,9:126,9:982,11:130,11:131,11:132,11:133,11:134,11:1104,11:1105,7:100,7:101,7:102,7:103,7:104,7:105,7:106,7:107,7:108,7:109,7:830,7:831,12:190,12:191,12:192,12:48,12:198,12:193,12:49,12:194,12:199,12:195,12:196,12:197,12:840,6:80,6:81,6:82,6:83,6:84,6:85,6:86,6:87,6:88,6:89,6:90,6:91,6:92,6:93,6:98,5:70,5:71,5:73,5:78,5:72,5:74,5:75,5:804,5:76,5:79,5:77,5:160,5:161,5:162,5:163,5:164,5:165,5:166,5:168,5:169,5:773,5:800,5:801,5:802,5:4,5:5,5:7,5:8,5:9,5:10,5:11,5:12,5:13,5:14,5:15,5:16,5:17,5:18,5:19,5:26,5:27,5:28,5:34,5:37,5:182,5:183,5:205\nSkypeKit/SubscribedEvents=10:1,10:2,0:1,0:2,0:3,1:1,19:1,18:1,18:2,18:3,0:4,0:5,11:2,0:7,0:44,0:10,0:11,0:12,0:6,0:8\n"
    return Skype(skypekit.SkypeKit(apptoken, module_id2classes, has_event_thread, host, port, logtransport, secure, setup))
