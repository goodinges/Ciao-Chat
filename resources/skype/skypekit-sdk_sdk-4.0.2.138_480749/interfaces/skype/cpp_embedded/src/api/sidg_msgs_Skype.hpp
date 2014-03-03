#ifndef __SIDG_MSGS_SKYPEHPP_INCLUDED__
#define __SIDG_MSGS_SKYPEHPP_INCLUDED__

#include "sidg_Skyperefs.hpp"

#ifdef SE_USE_NAMESPACE
namespace Skype {
#endif
        extern Sid::Field SkypeFields[];
        struct MsgconfigGetVersionStringResponse {
                Sid::String version;
                enum { BEGIN = 0, END = 1 };
        };
        struct MsgconfigGetUnixTimestampResponse {
                uint timestamp;
                enum { BEGIN = 1, END = 2 };
        };
        struct MsgcontactContactGroupGiveDisplayNameRequest {
                ContactGroupRef objectID;
                Sid::String     name;
                enum { BEGIN = 2, END = 4 };
        };
        struct MsgcontactContactGroupDeleteRequest {
                ContactGroupRef objectID;
                enum { BEGIN = 4, END = 5 };
        };
        struct MsgcontactContactGroupDeleteResponse {
                bool result;
                enum { BEGIN = 5, END = 6 };
        };
        struct MsgcontactContactGroupGetConversationsRequest {
                ContactGroupRef objectID;
                enum { BEGIN = 6, END = 7 };
        };
        struct MsgcontactContactGroupGetConversationsResponse {
                ConversationRefs conversations;
                enum { BEGIN = 7, END = 9 };
        };
        struct MsgcontactContactGroupCanAddConversationRequest {
                ContactGroupRef objectID;
                ConversationRef conversation;
                enum { BEGIN = 9, END = 11 };
        };
        struct MsgcontactContactGroupCanAddConversationResponse {
                bool result;
                enum { BEGIN = 11, END = 12 };
        };
        struct MsgcontactContactGroupAddConversationRequest {
                ContactGroupRef objectID;
                ConversationRef conversation;
                enum { BEGIN = 12, END = 14 };
        };
        struct MsgcontactContactGroupCanRemoveConversationRequest {
                ContactGroupRef objectID;
                enum { BEGIN = 14, END = 15 };
        };
        struct MsgcontactContactGroupCanRemoveConversationResponse {
                bool result;
                enum { BEGIN = 15, END = 16 };
        };
        struct MsgcontactContactGroupRemoveConversationRequest {
                ContactGroupRef objectID;
                ConversationRef conversation;
                enum { BEGIN = 16, END = 18 };
        };
        struct MsgcontactContactGroupOnChangeConversation {
                ContactGroupRef objectID;
                ConversationRef conversation;
                enum { BEGIN = 18, END = 20 };
        };
        struct MsgcontactContactGroupGetContactsRequest {
                ContactGroupRef objectID;
                enum { BEGIN = 20, END = 21 };
        };
        struct MsgcontactContactGroupGetContactsResponse {
                ContactRefs contacts;
                enum { BEGIN = 21, END = 23 };
        };
        struct MsgcontactContactGroupCanAddContactRequest {
                ContactGroupRef objectID;
                ContactRef      contact;
                enum { BEGIN = 23, END = 25 };
        };
        struct MsgcontactContactGroupCanAddContactResponse {
                bool result;
                enum { BEGIN = 25, END = 26 };
        };
        struct MsgcontactContactGroupAddContactRequest {
                ContactGroupRef objectID;
                ContactRef      contact;
                enum { BEGIN = 26, END = 28 };
        };
        struct MsgcontactContactGroupCanRemoveContactRequest {
                ContactGroupRef objectID;
                enum { BEGIN = 28, END = 29 };
        };
        struct MsgcontactContactGroupCanRemoveContactResponse {
                bool result;
                enum { BEGIN = 29, END = 30 };
        };
        struct MsgcontactContactGroupRemoveContactRequest {
                ContactGroupRef objectID;
                ContactRef      contact;
                enum { BEGIN = 30, END = 32 };
        };
        struct MsgcontactContactGroupOnChange {
                ContactGroupRef objectID;
                ContactRef      contact;
                enum { BEGIN = 32, END = 34 };
        };
        struct MsgcontactContactGroup {
                Sid::String given_displayname;
                uint        nrofcontacts;
                uint        nrofcontacts_online;
                uint        custom_group_id;
                int         type;
                Sid::List_uint cachedProps;
                enum { BEGIN = 34, END = 39 };
        };
        struct MsgcontactGetHardwiredContactGroupRequest {
                int type;
                enum { BEGIN = 39, END = 40 };
        };
        struct MsgcontactGetHardwiredContactGroupResponse {
                ContactGroupRef contactGroup;
                enum { BEGIN = 40, END = 41 };
        };
        struct MsgcontactGetCustomContactGroupsResponse {
                ContactGroupRefs groups;
                enum { BEGIN = 41, END = 43 };
        };
        struct MsgcontactCreateCustomContactGroupResponse {
                ContactGroupRef group;
                enum { BEGIN = 43, END = 44 };
        };
        struct MsgcontactOnNewCustomContactGroup {
                ContactGroupRef group;
                enum { BEGIN = 44, END = 45 };
        };
        struct MsgcontactContactGetTypeRequest {
                ContactRef objectID;
                enum { BEGIN = 45, END = 46 };
        };
        struct MsgcontactContactGetTypeResponse {
                int type;
                enum { BEGIN = 46, END = 47 };
        };
        struct MsgcontactContactGetIdentityRequest {
                ContactRef objectID;
                enum { BEGIN = 47, END = 48 };
        };
        struct MsgcontactContactGetIdentityResponse {
                Sid::String identity;
                enum { BEGIN = 48, END = 49 };
        };
        struct MsgcontactContactGetAvatarRequest {
                ContactRef objectID;
                enum { BEGIN = 49, END = 50 };
        };
        struct MsgcontactContactGetAvatarResponse {
                bool        present;
                Sid::Binary avatar;
                enum { BEGIN = 50, END = 52 };
        };
        struct MsgcontactContactGetVerifiedEmailRequest {
                ContactRef objectID;
                enum { BEGIN = 52, END = 53 };
        };
        struct MsgcontactContactGetVerifiedEmailResponse {
                Sid::String email;
                enum { BEGIN = 53, END = 54 };
        };
        struct MsgcontactContactGetVerifiedCompanyRequest {
                ContactRef objectID;
                enum { BEGIN = 54, END = 55 };
        };
        struct MsgcontactContactGetVerifiedCompanyResponse {
                Sid::String company;
                enum { BEGIN = 55, END = 56 };
        };
        struct MsgcontactContactIsMemberOfRequest {
                ContactRef      objectID;
                ContactGroupRef group;
                enum { BEGIN = 56, END = 58 };
        };
        struct MsgcontactContactIsMemberOfResponse {
                bool result;
                enum { BEGIN = 58, END = 59 };
        };
        struct MsgcontactContactIsMemberOfHardwiredGroupRequest {
                ContactRef objectID;
                int        groupType;
                enum { BEGIN = 59, END = 61 };
        };
        struct MsgcontactContactIsMemberOfHardwiredGroupResponse {
                bool result;
                enum { BEGIN = 61, END = 62 };
        };
        struct MsgcontactContactSetBlockedRequest {
                ContactRef objectID;
                bool       blocked;
                bool       abuse;
                enum { BEGIN = 62, END = 65 };
        };
        struct MsgcontactContactIgnoreAuthRequestRequest {
                ContactRef objectID;
                enum { BEGIN = 65, END = 66 };
        };
        struct MsgcontactContactGiveDisplayNameRequest {
                ContactRef  objectID;
                Sid::String name;
                enum { BEGIN = 66, END = 68 };
        };
        struct MsgcontactContactSetBuddyStatusRequest {
                ContactRef objectID;
                bool       isMyBuddy;
                bool       syncAuth;
                enum { BEGIN = 68, END = 71 };
        };
        struct MsgcontactContactSendAuthRequestRequest {                ContactRef  objectID;
                Sid::String message;
                uint        extras_bitmask;
                enum { BEGIN = 71, END = 74 };
        };
        struct MsgcontactContactHasAuthorizedMeRequest {
                ContactRef objectID;
                enum { BEGIN = 74, END = 75 };
        };
        struct MsgcontactContactHasAuthorizedMeResponse {
                bool result;
                enum { BEGIN = 75, END = 76 };
        };
        struct MsgcontactContactSetPhoneNumberRequest {
                ContactRef  objectID;
                uint        num;
                Sid::String label;
                Sid::String number;
                enum { BEGIN = 76, END = 80 };
        };
        struct MsgcontactContactOpenConversationRequest {
                ContactRef objectID;
                enum { BEGIN = 80, END = 81 };
        };
        struct MsgcontactContactOpenConversationResponse {
                ConversationRef conversation;
                enum { BEGIN = 81, END = 82 };
        };
        struct MsgcontactContactHasCapabilityRequest {
                ContactRef objectID;
                int        capability;
                bool       queryServer;
                enum { BEGIN = 82, END = 85 };
        };
        struct MsgcontactContactHasCapabilityResponse {
                bool result;
                enum { BEGIN = 85, END = 86 };
        };
        struct MsgcontactContactGetCapabilityStatusRequest {
                ContactRef objectID;
                int        capability;
                bool       queryServer;
                enum { BEGIN = 86, END = 89 };
        };
        struct MsgcontactContactGetCapabilityStatusResponse {
                int status;
                enum { BEGIN = 89, END = 90 };
        };
        struct MsgcontactContactRefreshProfileRequest {
                ContactRef objectID;
                enum { BEGIN = 90, END = 91 };
        };
        struct MsgcontactContact {
                Sid::String skypename;
                Sid::String fullname;
                Sid::String pstnnumber;
                uint        birthday;
                uint        gender;
                Sid::String languages;
                Sid::String country;
                Sid::String province;
                Sid::String city;
                Sid::String phone_home;
                Sid::String phone_office;
                Sid::String phone_mobile;
                Sid::String emails;
                Sid::String homepage;
                Sid::String about;
                uint        profile_timestamp;
                Sid::String received_authrequest;
                Sid::String displayname;
                bool        refreshing;
                int         given_authlevel;
                uint        authreq_timestamp;
                Sid::String mood_text;
                uint        timezone;
                uint        nrof_authed_buddies;
                Sid::String ipcountry;
                Sid::String given_displayname;
                int         availability;
                uint        lastonline_timestamp;
                Sid::Binary capabilities;
                Sid::Binary avatar_image;
                uint        lastused_timestamp;
                uint        authrequest_count;
                uint        popularity_ord;
                Sid::String assigned_comment;
                uint        avatar_timestamp;
                uint        mood_timestamp;
                Sid::String assigned_phone1;
                Sid::String assigned_phone1_label;
                Sid::String assigned_phone2;
                Sid::String assigned_phone2_label;
                Sid::String assigned_phone3;
                Sid::String assigned_phone3_label;
                int         type;
                Sid::String rich_mood_text;
                Sid::List_uint cachedProps;
                enum { BEGIN = 91, END = 135 };
        };
        struct MsgcontactGetContactTypeRequest {
                Sid::String identity;
                enum { BEGIN = 135, END = 136 };
        };
        struct MsgcontactGetContactTypeResponse {
                int type;
                enum { BEGIN = 136, END = 137 };
        };
        struct MsgcontactGetContactRequest {
                Sid::String identity;
                enum { BEGIN = 137, END = 138 };
        };
        struct MsgcontactGetContactResponse {
                ContactRef contact;
                enum { BEGIN = 138, END = 139 };
        };
        struct MsgcontactFindContactByPstnNumberRequest {
                Sid::String number;
                enum { BEGIN = 139, END = 140 };
        };
        struct MsgcontactFindContactByPstnNumberResponse {
                bool       found;
                ContactRef contact;
                uint       foundInKey;
                enum { BEGIN = 140, END = 143 };
        };
        struct MsgcontactGetIdentityTypeRequest {
                Sid::String identity;
                enum { BEGIN = 143, END = 144 };
        };
        struct MsgcontactGetIdentityTypeResponse {
                int type;
                enum { BEGIN = 144, END = 145 };
        };
        struct MsgcontactIdentitiesMatchRequest {
                Sid::String identityA;
                Sid::String identityB;
                enum { BEGIN = 145, END = 147 };
        };
        struct MsgcontactIdentitiesMatchResponse {
                bool result;
                enum { BEGIN = 147, END = 148 };
        };
        struct MsgcontactNormalizeIdentityRequest {
                Sid::String original;
                bool        isNewSkypeName;
                enum { BEGIN = 148, END = 150 };
        };
        struct MsgcontactNormalizeIdentityResponse {
                int         result;
                Sid::String normalized;
                enum { BEGIN = 150, END = 152 };
        };
        struct MsgcontactNormalizePSTNWithCountryRequest {
                Sid::String original;
                uint        countryPrefix;
                enum { BEGIN = 152, END = 154 };
        };
        struct MsgcontactNormalizePSTNWithCountryResponse {
                int         result;
                Sid::String normalized;
                enum { BEGIN = 154, END = 156 };
        };
        struct MsgcontactOnContactOnlineAppearance {
                ContactRef contact;
                enum { BEGIN = 156, END = 157 };
        };
        struct MsgcontactOnContactGoneOffline {
                ContactRef contact;
                enum { BEGIN = 157, END = 158 };
        };
        struct MsgcontactsearchContactSearchAddMinAgeTermRequest {
                ContactSearchRef objectID;
                uint             min_age_in_years;
                bool             add_to_subs;
                enum { BEGIN = 158, END = 161 };
        };
        struct MsgcontactsearchContactSearchAddMinAgeTermResponse {
                bool valid;
                enum { BEGIN = 161, END = 162 };
        };
        struct MsgcontactsearchContactSearchAddMaxAgeTermRequest {
                ContactSearchRef objectID;
                uint             max_age_in_years;
                bool             add_to_subs;
                enum { BEGIN = 162, END = 165 };
        };
        struct MsgcontactsearchContactSearchAddMaxAgeTermResponse {
                bool valid;
                enum { BEGIN = 165, END = 166 };
        };
        struct MsgcontactsearchContactSearchAddEmailTermRequest {
                ContactSearchRef objectID;
                Sid::String      email;
                bool             add_to_subs;
                enum { BEGIN = 166, END = 169 };
        };
        struct MsgcontactsearchContactSearchAddEmailTermResponse {
                bool valid;
                enum { BEGIN = 169, END = 170 };
        };
        struct MsgcontactsearchContactSearchAddLanguageTermRequest {
                ContactSearchRef objectID;
                Sid::String      language;
                bool             add_to_subs;
                enum { BEGIN = 170, END = 173 };
        };
        struct MsgcontactsearchContactSearchAddLanguageTermResponse {
                bool valid;                enum { BEGIN = 173, END = 174 };
        };
        struct MsgcontactsearchContactSearchAddStrTermRequest {
                ContactSearchRef objectID;
                int              prop;
                int              cond;
                Sid::String      value;
                bool             add_to_subs;
                enum { BEGIN = 174, END = 179 };
        };
        struct MsgcontactsearchContactSearchAddStrTermResponse {
                bool valid;
                enum { BEGIN = 179, END = 180 };
        };
        struct MsgcontactsearchContactSearchAddIntTermRequest {
                ContactSearchRef objectID;
                int              prop;
                int              cond;
                uint             value;
                bool             add_to_subs;
                enum { BEGIN = 180, END = 185 };
        };
        struct MsgcontactsearchContactSearchAddIntTermResponse {
                bool valid;
                enum { BEGIN = 185, END = 186 };
        };
        struct MsgcontactsearchContactSearchAddOrRequest {
                ContactSearchRef objectID;
                enum { BEGIN = 186, END = 187 };
        };
        struct MsgcontactsearchContactSearchIsValidRequest {
                ContactSearchRef objectID;
                enum { BEGIN = 187, END = 188 };
        };
        struct MsgcontactsearchContactSearchIsValidResponse {
                bool result;
                enum { BEGIN = 188, END = 189 };
        };
        struct MsgcontactsearchContactSearchSubmitRequest {
                ContactSearchRef objectID;
                enum { BEGIN = 189, END = 190 };
        };
        struct MsgcontactsearchContactSearchExtendRequest {
                ContactSearchRef objectID;
                enum { BEGIN = 190, END = 191 };
        };
        struct MsgcontactsearchContactSearchReleaseRequest {
                ContactSearchRef objectID;
                enum { BEGIN = 191, END = 192 };
        };
        struct MsgcontactsearchContactSearchGetResultsRequest {
                ContactSearchRef objectID;
                uint             from;
                uint             count;
                enum { BEGIN = 192, END = 195 };
        };
        struct MsgcontactsearchContactSearchGetResultsResponse {
                ContactRefs contacts;
                enum { BEGIN = 195, END = 197 };
        };
        struct MsgcontactsearchContactSearchOnNewResult {
                ContactSearchRef objectID;
                ContactRef       contact;
                uint             rankValue;
                enum { BEGIN = 197, END = 200 };
        };
        struct MsgcontactsearchContactSearch {
                int contact_search_status;
                Sid::List_uint cachedProps;
                enum { BEGIN = 200, END = 201 };
        };
        struct MsgcontactsearchGetOptimalAgeRangesResponse {
                Sid::List_uint rangeList;
                enum { BEGIN = 201, END = 203 };
        };
        struct MsgcontactsearchCreateContactSearchResponse {
                ContactSearchRef search;
                enum { BEGIN = 203, END = 204 };
        };
        struct MsgcontactsearchCreateBasicContactSearchRequest {
                Sid::String text;
                enum { BEGIN = 204, END = 205 };
        };
        struct MsgcontactsearchCreateBasicContactSearchResponse {
                ContactSearchRef search;
                enum { BEGIN = 205, END = 206 };
        };
        struct MsgcontactsearchCreateIdentitySearchRequest {
                Sid::String identity;
                enum { BEGIN = 206, END = 207 };
        };
        struct MsgcontactsearchCreateIdentitySearchResponse {
                ContactSearchRef search;
                enum { BEGIN = 207, END = 208 };
        };
        struct MsgconversationParticipantCanSetRankToRequest {
                ParticipantRef objectID;
                int            rank;
                enum { BEGIN = 208, END = 210 };
        };
        struct MsgconversationParticipantCanSetRankToResponse {
                bool result;
                enum { BEGIN = 210, END = 211 };
        };
        struct MsgconversationParticipantSetRankToRequest {
                ParticipantRef objectID;
                int            rank;
                enum { BEGIN = 211, END = 213 };
        };
        struct MsgconversationParticipantRingRequest {
                ParticipantRef objectID;
                Sid::String    identityToUse;
                bool           videoCall;
                uint           nrofRedials;
                uint           redialPeriod;
                bool           autoStartVM;
                Sid::String    origin;
                enum { BEGIN = 213, END = 220 };
        };
        struct MsgconversationParticipantRingItRequest {
                ParticipantRef objectID;
                enum { BEGIN = 220, END = 221 };
        };
        struct MsgconversationParticipantSetLiveIdentityToUseRequest {
                ParticipantRef objectID;
                Sid::String    identityToUse;
                enum { BEGIN = 221, END = 223 };
        };
        struct MsgconversationParticipantGetVideoRequest {
                ParticipantRef objectID;
                enum { BEGIN = 223, END = 224 };
        };
        struct MsgconversationParticipantGetVideoResponse {
                VideoRef video;
                enum { BEGIN = 224, END = 225 };
        };
        struct MsgconversationParticipantHangupRequest {
                ParticipantRef objectID;
                enum { BEGIN = 225, END = 226 };
        };
        struct MsgconversationParticipantRetireRequest {
                ParticipantRef objectID;
                enum { BEGIN = 226, END = 227 };
        };
        struct MsgconversationParticipantOnIncomingDTMF {
                ParticipantRef objectID;
                int            dtmf;
                enum { BEGIN = 227, END = 229 };
        };
        struct MsgconversationParticipant {
                ConversationRef convo_id;
                Sid::String     identity;
                int             rank;
                int             requested_rank;
                int             text_status;
                int             voice_status;
                int             video_status;
                Sid::String     live_price_for_me;
                uint            live_start_timestamp;
                uint            sound_level;
                Sid::String     debuginfo;
                Sid::String     live_identity;
                Sid::String     last_voice_error;
                Sid::String     live_fwd_identities;
                Sid::String     quality_problems;
                int             live_type;
                Sid::String     live_country;
                Sid::String     transferred_by;
                Sid::String     transferred_to;
                Sid::String     adder;
                int             last_leavereason;
                Sid::List_uint cachedProps;
                enum { BEGIN = 229, END = 250 };
        };
        struct MsgconversationConversationSetOptionRequest {
                ConversationRef objectID;
                int             propKey;
                uint            value;
                enum { BEGIN = 250, END = 253 };
        };
        struct MsgconversationConversationSetTopicRequest {
                ConversationRef objectID;
                Sid::String     topic;
                bool            isXML;
                enum { BEGIN = 253, END = 256 };
        };
        struct MsgconversationConversationSetGuidelinesRequest {
                ConversationRef objectID;
                Sid::String     guidelines;
                bool            isXML;
                enum { BEGIN = 256, END = 259 };
        };
        struct MsgconversationConversationSetPictureRequest {
                ConversationRef objectID;
                Sid::Binary     jpeg;
                enum { BEGIN = 259, END = 261 };
        };
        struct MsgconversationConversationSpawnConferenceRequest {
                ConversationRef  objectID;
                Sid::List_String identitiesToAdd;                enum { BEGIN = 261, END = 264 };
        };
        struct MsgconversationConversationSpawnConferenceResponse {
                ConversationRef conference;
                enum { BEGIN = 264, END = 265 };
        };
        struct MsgconversationConversationAddConsumersRequest {
                ConversationRef  objectID;
                Sid::List_String identities;
                enum { BEGIN = 265, END = 268 };
        };
        struct MsgconversationConversationAssimilateRequest {
                ConversationRef objectID;
                ConversationRef otherConversation;
                enum { BEGIN = 268, END = 270 };
        };
        struct MsgconversationConversationAssimilateResponse {
                ConversationRef conversation;
                enum { BEGIN = 270, END = 271 };
        };
        struct MsgconversationConversationJoinLiveSessionRequest {
                ConversationRef objectID;
                Sid::String     accessToken;
                enum { BEGIN = 271, END = 273 };
        };
        struct MsgconversationConversationRingOthersRequest {
                ConversationRef  objectID;
                Sid::List_String identities;
                bool             videoCall;
                Sid::String      origin;
                enum { BEGIN = 273, END = 278 };
        };
        struct MsgconversationConversationMuteMyMicrophoneRequest {
                ConversationRef objectID;
                enum { BEGIN = 278, END = 279 };
        };
        struct MsgconversationConversationUnmuteMyMicrophoneRequest {
                ConversationRef objectID;
                enum { BEGIN = 279, END = 280 };
        };
        struct MsgconversationConversationHoldMyLiveSessionRequest {
                ConversationRef objectID;
                enum { BEGIN = 280, END = 281 };
        };
        struct MsgconversationConversationResumeMyLiveSessionRequest {
                ConversationRef objectID;
                enum { BEGIN = 281, END = 282 };
        };
        struct MsgconversationConversationLeaveLiveSessionRequest {
                ConversationRef objectID;
                bool            postVoiceAutoresponse;
                enum { BEGIN = 282, END = 284 };
        };
        struct MsgconversationConversationStartVoiceMessageRequest {
                ConversationRef objectID;
                enum { BEGIN = 284, END = 285 };
        };
        struct MsgconversationConversationTransferLiveSessionRequest {
                ConversationRef  objectID;
                Sid::List_String identities;
                Sid::String      transferTopic;
                enum { BEGIN = 285, END = 289 };
        };
        struct MsgconversationConversationCanTransferLiveSessionRequest {
                ConversationRef objectID;
                Sid::String     identity;
                enum { BEGIN = 289, END = 291 };
        };
        struct MsgconversationConversationCanTransferLiveSessionResponse {
                bool result;
                enum { BEGIN = 291, END = 292 };
        };
        struct MsgconversationConversationSendDTMFRequest {
                ConversationRef objectID;
                int             dtmf;
                uint            lengthInMS;
                enum { BEGIN = 292, END = 295 };
        };
        struct MsgconversationConversationStopSendDTMFRequest {
                ConversationRef objectID;
                enum { BEGIN = 295, END = 296 };
        };
        struct MsgconversationConversationSetMyTextStatusToRequest {
                ConversationRef objectID;
                int             status;
                enum { BEGIN = 296, END = 298 };
        };
        struct MsgconversationConversationPostTextRequest {
                ConversationRef objectID;
                Sid::String     text;
                bool            isXML;
                enum { BEGIN = 298, END = 301 };
        };
        struct MsgconversationConversationPostTextResponse {
                MessageRef message;
                enum { BEGIN = 301, END = 302 };
        };
        struct MsgconversationConversationPostContactsRequest {
                ConversationRef objectID;
                ContactRefs     contacts;
                enum { BEGIN = 302, END = 305 };
        };
        struct MsgconversationConversationPostFilesRequest {
                ConversationRef    objectID;
                Sid::List_Filename paths;
                Sid::String        body;
                enum { BEGIN = 305, END = 309 };
        };
        struct MsgconversationConversationPostFilesResponse {
                int           error_code;
                Sid::Filename error_file;
                enum { BEGIN = 309, END = 311 };
        };
        struct MsgconversationConversationPostVoiceMessageRequest {
                ConversationRef objectID;
                VoicemailRef    voicemail;
                Sid::String     body;
                enum { BEGIN = 311, END = 314 };
        };
        struct MsgconversationConversationPostSMSRequest {
                ConversationRef objectID;
                SmsRef          sms;
                Sid::String     body;
                enum { BEGIN = 314, END = 317 };
        };
        struct MsgconversationConversationGetJoinBlobRequest {
                ConversationRef objectID;
                enum { BEGIN = 317, END = 318 };
        };
        struct MsgconversationConversationGetJoinBlobResponse {
                Sid::String blob;
                enum { BEGIN = 318, END = 319 };
        };
        struct MsgconversationConversationJoinRequest {
                ConversationRef objectID;
                enum { BEGIN = 319, END = 320 };
        };
        struct MsgconversationConversationEnterPasswordRequest {
                ConversationRef objectID;
                Sid::String     password;
                enum { BEGIN = 320, END = 322 };
        };
        struct MsgconversationConversationSetPasswordRequest {
                ConversationRef objectID;
                Sid::String     password;
                Sid::String     hint;
                enum { BEGIN = 322, END = 325 };
        };
        struct MsgconversationConversationRetireFromRequest {
                ConversationRef objectID;
                enum { BEGIN = 325, END = 326 };
        };
        struct MsgconversationConversationDeleteRequest {
                ConversationRef objectID;
                enum { BEGIN = 326, END = 327 };
        };
        struct MsgconversationConversationRenameToRequest {
                ConversationRef objectID;
                Sid::String     name;
                enum { BEGIN = 327, END = 329 };
        };
        struct MsgconversationConversationSetBookmarkRequest {
                ConversationRef objectID;
                bool            bookmark;
                enum { BEGIN = 329, END = 331 };
        };
        struct MsgconversationConversationSetAlertStringRequest {
                ConversationRef objectID;
                Sid::String     alertString;
                enum { BEGIN = 331, END = 333 };
        };
        struct MsgconversationConversationRemoveFromInboxRequest {
                ConversationRef objectID;
                enum { BEGIN = 333, END = 334 };
        };
        struct MsgconversationConversationAddToInboxRequest {
                ConversationRef objectID;
                uint            timestamp;
                enum { BEGIN = 334, END = 336 };
        };
        struct MsgconversationConversationSetConsumedHorizonRequest {
                ConversationRef objectID;
                uint            timestamp;
                bool            also_unconsume;
                enum { BEGIN = 336, END = 339 };
        };
        struct MsgconversationConversationMarkUnreadRequest {
                ConversationRef objectID;
                enum { BEGIN = 339, END = 340 };
        };
        struct MsgconversationConversationIsMemberOfRequest {
                ConversationRef objectID;
                ContactGroupRef group;
                enum { BEGIN = 340, END = 342 };
        };
        struct MsgconversationConversationIsMemberOfResponse {
                bool result;                enum { BEGIN = 342, END = 343 };
        };
        struct MsgconversationConversationGetParticipantsRequest {
                ConversationRef objectID;
                int             filter;
                enum { BEGIN = 343, END = 345 };
        };
        struct MsgconversationConversationGetParticipantsResponse {
                ParticipantRefs participants;
                enum { BEGIN = 345, END = 347 };
        };
        struct MsgconversationConversationGetLastMessagesRequest {
                ConversationRef objectID;
                uint            requireTimestamp;
                enum { BEGIN = 347, END = 349 };
        };
        struct MsgconversationConversationGetLastMessagesResponse {
                MessageRefs contextMessages;
                MessageRefs unconsumedMessages;
                enum { BEGIN = 349, END = 353 };
        };
        struct MsgconversationConversationFindMessageRequest {
                ConversationRef objectID;
                Sid::String     text;
                uint            fromTimestampUp;
                enum { BEGIN = 353, END = 356 };
        };
        struct MsgconversationConversationFindMessageResponse {
                MessageRef message;
                enum { BEGIN = 356, END = 357 };
        };
        struct MsgconversationConversationOnParticipantListChange {
                ConversationRef objectID;
                enum { BEGIN = 357, END = 358 };
        };
        struct MsgconversationConversationOnMessage {
                ConversationRef objectID;
                MessageRef      message;
                enum { BEGIN = 358, END = 360 };
        };
        struct MsgconversationConversationOnSpawnConference {
                ConversationRef objectID;
                ConversationRef spawned;
                enum { BEGIN = 360, END = 362 };
        };
        struct MsgconversationConversation {
                int             type;
                Sid::String     creator;
                uint            creation_timestamp;
                int             opt_entry_level_rank;
                bool            opt_disclose_history;
                int             opt_admin_only_activities;
                Sid::String     meta_name;
                Sid::String     meta_topic;
                Sid::String     meta_guidelines;
                Sid::Binary     meta_picture;
                ConversationRef spawned_from_convo_id;
                Sid::String     live_host;
                int             my_status;
                Sid::String     alert_string;
                bool            is_bookmarked;
                bool            opt_joining_enabled;
                Sid::String     displayname;
                Sid::String     given_displayname;
                int             local_livestatus;
                uint            inbox_timestamp;
                bool            unconsumed_messages_voice;
                VoicemailRef    active_vm_id;
                Sid::String     identity;
                MessageRef      inbox_message_id;
                uint            live_start_timestamp;
                uint            unconsumed_suppressed_messages;
                uint            unconsumed_normal_messages;
                uint            unconsumed_elevated_messages;
                uint            consumption_horizon;
                Sid::String     passwordhint;
                uint            last_activity_timestamp;
                bool            live_is_muted;
                Sid::List_uint cachedProps;
                enum { BEGIN = 362, END = 394 };
        };
        struct MsgconversationCreateConferenceResponse {
                ConversationRef conference;
                enum { BEGIN = 394, END = 395 };
        };
        struct MsgconversationGetConversationByIdentityRequest {
                Sid::String convoIdentity;
                enum { BEGIN = 395, END = 396 };
        };
        struct MsgconversationGetConversationByIdentityResponse {
                ConversationRef conversation;
                enum { BEGIN = 396, END = 397 };
        };
        struct MsgconversationGetConversationByParticipantsRequest {
                Sid::List_String participantIdentities;
                bool             createIfNonExisting;
                bool             ignoreBookmarkedOrNamed;
                enum { BEGIN = 397, END = 401 };
        };
        struct MsgconversationGetConversationByParticipantsResponse {
                ConversationRef conversation;
                enum { BEGIN = 401, END = 402 };
        };
        struct MsgconversationGetConversationByBlobRequest {
                Sid::String joinBlob;
                bool        alsoJoin;
                enum { BEGIN = 402, END = 404 };
        };
        struct MsgconversationGetConversationByBlobResponse {
                ConversationRef conversation;
                enum { BEGIN = 404, END = 405 };
        };
        struct MsgconversationGetConversationListRequest {
                int type;
                enum { BEGIN = 405, END = 406 };
        };
        struct MsgconversationGetConversationListResponse {
                ConversationRefs conversations;
                enum { BEGIN = 406, END = 408 };
        };
        struct MsgconversationOnConversationListChange {
                ConversationRef conversation;
                int             type;
                bool            added;
                enum { BEGIN = 408, END = 411 };
        };
        struct MsgconversationMessageCanEditRequest {
                MessageRef objectID;
                enum { BEGIN = 411, END = 412 };
        };
        struct MsgconversationMessageCanEditResponse {
                bool result;
                enum { BEGIN = 412, END = 413 };
        };
        struct MsgconversationMessageEditRequest {
                MessageRef  objectID;
                Sid::String newText;
                bool        isXML;
                bool        undo;
                enum { BEGIN = 413, END = 417 };
        };
        struct MsgconversationMessageGetContactsRequest {
                MessageRef objectID;
                enum { BEGIN = 417, END = 418 };
        };
        struct MsgconversationMessageGetContactsResponse {
                ContactRefs contacts;
                enum { BEGIN = 418, END = 420 };
        };
        struct MsgconversationMessageGetTransfersRequest {
                MessageRef objectID;
                enum { BEGIN = 420, END = 421 };
        };
        struct MsgconversationMessageGetTransfersResponse {
                TransferRefs transfers;
                enum { BEGIN = 421, END = 423 };
        };
        struct MsgconversationMessageGetVoiceMessageRequest {
                MessageRef objectID;
                enum { BEGIN = 423, END = 424 };
        };
        struct MsgconversationMessageGetVoiceMessageResponse {
                VoicemailRef voicemail;
                enum { BEGIN = 424, END = 425 };
        };
        struct MsgconversationMessageGetSMSRequest {
                MessageRef objectID;
                enum { BEGIN = 425, END = 426 };
        };
        struct MsgconversationMessageGetSMSResponse {
                SmsRef sms;
                enum { BEGIN = 426, END = 427 };
        };
        struct MsgconversationMessageDeleteLocallyRequest {
                MessageRef objectID;
                enum { BEGIN = 427, END = 428 };
        };
        struct MsgconversationMessage {
                Sid::String     convo_guid;
                uint            timestamp;
                Sid::String     author;
                Sid::String     author_displayname;
                Sid::String     identities;
                int             leavereason;
                Sid::String     body_xml;
                Sid::String     edited_by;
                uint            edit_timestamp;
                Sid::String     originally_meant_for;
                Sid::Binary     guid;
                ConversationRef convo_id;
                int             type;
                int             sending_status;
                uint            param_key;
                uint            param_value;                Sid::String     reason;
                int             consumption_status;
                uint            participant_count;
                Sid::List_uint cachedProps;
                enum { BEGIN = 428, END = 447 };
        };
        struct MsgconversationGetMessageByGuidRequest {
                Sid::Binary guid;
                enum { BEGIN = 447, END = 448 };
        };
        struct MsgconversationGetMessageByGuidResponse {
                MessageRef message;
                enum { BEGIN = 448, END = 449 };
        };
        struct MsgconversationGetMessageListByTypeRequest {
                int  type;
                bool latestPerConvOnly;
                uint fromTimestampInc;
                uint toTimestampExc;
                enum { BEGIN = 449, END = 453 };
        };
        struct MsgconversationGetMessageListByTypeResponse {
                MessageRefs messages;
                enum { BEGIN = 453, END = 455 };
        };
        struct MsgconversationOnMessage {
                MessageRef      message;
                bool            changesInboxTimestamp;
                MessageRef      supersedesHistoryMessage;
                ConversationRef conversation;
                enum { BEGIN = 455, END = 459 };
        };
        struct MsgvideoVideoSetScreenRequest {
                VideoRef objectID;
                uint     windowh;
                enum { BEGIN = 459, END = 461 };
        };
        struct MsgvideoVideoStartRequest {
                VideoRef objectID;
                enum { BEGIN = 461, END = 462 };
        };
        struct MsgvideoVideoStopRequest {
                VideoRef objectID;
                enum { BEGIN = 462, END = 463 };
        };
        struct MsgvideoVideoSubmitCaptureRequestRequest {
                VideoRef objectID;
                enum { BEGIN = 463, END = 464 };
        };
        struct MsgvideoVideoSubmitCaptureRequestResponse {
                bool ret;
                uint requestId;
                enum { BEGIN = 464, END = 466 };
        };
        struct MsgvideoVideoOnCaptureRequestCompleted {
                VideoRef    objectID;
                uint        requestId;
                bool        isSuccessful;
                Sid::Binary image;
                int         width;
                int         height;
                enum { BEGIN = 466, END = 472 };
        };
        struct MsgvideoVideoSetScreenCaptureRectangleRequest {
                VideoRef objectID;
                int      x0;
                int      y0;
                uint     width;
                uint     height;
                int      monitorNumber;
                uint     windowHandle;
                enum { BEGIN = 472, END = 479 };
        };
        struct MsgvideoVideoSetRenderRectangleRequest {
                VideoRef objectID;
                int      x0;
                int      y0;
                uint     width;
                uint     height;
                enum { BEGIN = 479, END = 484 };
        };
        struct MsgvideoVideoSetRemoteRendererIdRequest {
                VideoRef objectID;
                uint     id;
                enum { BEGIN = 484, END = 486 };
        };
        struct MsgvideoVideoSelectVideoSourceRequest {
                VideoRef    objectID;
                int         mediaType;
                Sid::String webcamName;
                Sid::String devicePath;
                bool        updateSetup;
                enum { BEGIN = 486, END = 491 };
        };
        struct MsgvideoVideoGetCurrentVideoDeviceRequest {
                VideoRef objectID;
                enum { BEGIN = 491, END = 492 };
        };
        struct MsgvideoVideoGetCurrentVideoDeviceResponse {
                int         mediatype;
                Sid::String deviceName;
                Sid::String devicePath;
                enum { BEGIN = 492, END = 495 };
        };
        struct MsgvideoVideo {
                int         status;
                Sid::String error;
                Sid::String debuginfo;
                Sid::String dimensions;
                int         media_type;
                uint        convo_id;
                Sid::String device_path;
                Sid::List_uint cachedProps;
                enum { BEGIN = 495, END = 502 };
        };
        struct MsgvideoGetAvailableVideoDevicesResponse {
                Sid::List_String deviceNames;
                Sid::List_String devicePaths;
                uint             count;
                enum { BEGIN = 502, END = 507 };
        };
        struct MsgvideoHasVideoDeviceCapabilityRequest {
                Sid::String deviceName;
                Sid::String devicePath;
                int         cap;
                enum { BEGIN = 507, END = 510 };
        };
        struct MsgvideoHasVideoDeviceCapabilityResponse {
                bool result;
                enum { BEGIN = 510, END = 511 };
        };
        struct MsgvideoDisplayVideoDeviceTuningDialogRequest {
                Sid::String deviceName;
                Sid::String devicePath;
                enum { BEGIN = 511, END = 513 };
        };
        struct MsgvideoGetPreviewVideoRequest {
                int         type;
                Sid::String deviceName;
                Sid::String devicePath;
                enum { BEGIN = 513, END = 516 };
        };
        struct MsgvideoGetPreviewVideoResponse {
                VideoRef video;
                enum { BEGIN = 516, END = 517 };
        };
        struct MsgvideoVideoCommandRequest {
                Sid::String command;
                enum { BEGIN = 517, END = 518 };
        };
        struct MsgvideoVideoCommandResponse {
                Sid::String response;
                enum { BEGIN = 518, END = 519 };
        };
        struct MsgvmVoicemailStartRecordingRequest {
                VoicemailRef objectID;
                enum { BEGIN = 519, END = 520 };
        };
        struct MsgvmVoicemailStopRecordingRequest {
                VoicemailRef objectID;
                enum { BEGIN = 520, END = 521 };
        };
        struct MsgvmVoicemailStartPlaybackRequest {
                VoicemailRef objectID;
                enum { BEGIN = 521, END = 522 };
        };
        struct MsgvmVoicemailStopPlaybackRequest {
                VoicemailRef objectID;
                enum { BEGIN = 522, END = 523 };
        };
        struct MsgvmVoicemailDeleteRequest {
                VoicemailRef objectID;
                enum { BEGIN = 523, END = 524 };
        };
        struct MsgvmVoicemailCancelRequest {
                VoicemailRef objectID;
                enum { BEGIN = 524, END = 525 };
        };
        struct MsgvmVoicemailCheckPermissionRequest {
                VoicemailRef objectID;
                enum { BEGIN = 525, END = 526 };
        };
        struct MsgvmVoicemailCheckPermissionResponse {
                bool result;
                enum { BEGIN = 526, END = 527 };
        };
        struct MsgvmVoicemail {
                int             type;
                Sid::String     partner_handle;
                Sid::String     partner_dispname;
                int             status;
                int             failurereason;
                Sid::String     subject;
                uint            timestamp;
                uint            duration;
                uint            allowed_duration;
                uint            playback_progress;
                ConversationRef convo_id;
                Sid::Binary     chatmsg_guid;
                Sid::List_uint cachedProps;
                enum { BEGIN = 527, END = 539 };
        };
        struct MsgvmGetGreetingRequest {
                Sid::String skypeName;
                enum { BEGIN = 539, END = 540 };
        };
        struct MsgvmGetGreetingResponse {
                VoicemailRef greeting;
                enum { BEGIN = 540, END = 541 };
        };
        struct MsgaudioPlayStartRequest {
                uint        soundid;
                Sid::Binary sound;
                bool        loop;
                bool        useCallOutDevice;
                enum { BEGIN = 541, END = 545 };
        };        struct MsgaudioPlayStartFromFileRequest {
                uint          soundid;
                Sid::Filename datafile;
                bool          loop;
                bool          useCallOutDevice;
                enum { BEGIN = 545, END = 549 };
        };
        struct MsgaudioPlayStartFromFileResponse {
                int result;
                enum { BEGIN = 549, END = 550 };
        };
        struct MsgaudioPlayStopRequest {
                uint soundid;
                enum { BEGIN = 550, END = 551 };
        };
        struct MsgaudioStartRecordingTestRequest {
                bool recordAndPlaybackData;
                enum { BEGIN = 551, END = 552 };
        };
        struct MsgaudioGetAvailableOutputDevicesResponse {
                Sid::List_String handleList;
                Sid::List_String nameList;
                Sid::List_String productIdList;
                enum { BEGIN = 552, END = 558 };
        };
        struct MsgaudioGetAvailableRecordingDevicesResponse {
                Sid::List_String handleList;
                Sid::List_String nameList;
                Sid::List_String productIdList;
                enum { BEGIN = 558, END = 564 };
        };
        struct MsgaudioSelectSoundDevicesRequest {
                Sid::String callInDevice;
                Sid::String callOutDevice;
                Sid::String waveOutDevice;
                enum { BEGIN = 564, END = 567 };
        };
        struct MsgaudioGetAudioDeviceCapabilitiesRequest {
                Sid::String deviceHandle;
                enum { BEGIN = 567, END = 568 };
        };
        struct MsgaudioGetAudioDeviceCapabilitiesResponse {
                Sid::String interfaceString;
                uint        capabilities;
                enum { BEGIN = 568, END = 570 };
        };
        struct MsgaudioGetNrgLevelsResponse {
                uint micLevel;
                uint speakerLevel;
                enum { BEGIN = 570, END = 572 };
        };
        struct MsgaudioVoiceCommandRequest {
                Sid::String command;
                enum { BEGIN = 572, END = 573 };
        };
        struct MsgaudioVoiceCommandResponse {
                Sid::String response;
                enum { BEGIN = 573, END = 574 };
        };
        struct MsgaudioGetSpeakerVolumeResponse {
                uint volume;
                enum { BEGIN = 574, END = 575 };
        };
        struct MsgaudioSetSpeakerVolumeRequest {
                uint volume;
                enum { BEGIN = 575, END = 576 };
        };
        struct MsgaudioGetMicVolumeResponse {
                uint micVolume;
                enum { BEGIN = 576, END = 577 };
        };
        struct MsgaudioSetMicVolumeRequest {
                uint volume;
                enum { BEGIN = 577, END = 578 };
        };
        struct MsgaudioIsSpeakerMutedResponse {
                bool muted;
                enum { BEGIN = 578, END = 579 };
        };
        struct MsgaudioIsMicrophoneMutedResponse {
                bool muted;
                enum { BEGIN = 579, END = 580 };
        };
        struct MsgaudioMuteSpeakersRequest {
                bool mute;
                enum { BEGIN = 580, END = 581 };
        };
        struct MsgaudioMuteMicrophoneRequest {
                bool mute;
                enum { BEGIN = 581, END = 582 };
        };
        struct MsgbwmSetOperatingMediaRequest {
                int  media;
                uint maxUplinkBps;
                uint maxDownlinkBps;
                enum { BEGIN = 582, END = 585 };
        };
        struct MsgsmsSmsGetTargetStatusRequest {
                SmsRef      objectID;
                Sid::String target;
                enum { BEGIN = 585, END = 587 };
        };
        struct MsgsmsSmsGetTargetStatusResponse {
                int status;
                enum { BEGIN = 587, END = 588 };
        };
        struct MsgsmsSmsGetTargetPriceRequest {
                SmsRef      objectID;
                Sid::String target;
                enum { BEGIN = 588, END = 590 };
        };
        struct MsgsmsSmsGetTargetPriceResponse {
                uint price;
                enum { BEGIN = 590, END = 591 };
        };
        struct MsgsmsSmsSetTargetsRequest {
                SmsRef           objectID;
                Sid::List_String numbers;
                enum { BEGIN = 591, END = 594 };
        };
        struct MsgsmsSmsSetTargetsResponse {
                bool success;
                enum { BEGIN = 594, END = 595 };
        };
        struct MsgsmsSmsSetBodyRequest {
                SmsRef      objectID;
                Sid::String text;
                enum { BEGIN = 595, END = 597 };
        };
        struct MsgsmsSmsSetBodyResponse {
                int              result;
                Sid::List_String chunks;
                uint             charsUntilNextChunk;
                enum { BEGIN = 597, END = 601 };
        };
        struct MsgsmsSmsGetBodyChunksRequest {
                SmsRef objectID;
                enum { BEGIN = 601, END = 602 };
        };
        struct MsgsmsSmsGetBodyChunksResponse {
                Sid::List_String textChunks;
                uint             charsUntilNextChunk;
                enum { BEGIN = 602, END = 605 };
        };
        struct MsgsmsSms {
                bool        is_failed_unseen;
                uint        price_precision;
                int         type;
                int         status;
                int         failurereason;
                uint        price;
                Sid::String price_currency;
                Sid::String target_numbers;
                Sid::Binary target_statuses;
                Sid::String body;
                uint        timestamp;
                Sid::String reply_to_number;
                MessageRef  chatmsg_id;
                Sid::List_uint cachedProps;
                enum { BEGIN = 605, END = 618 };
        };
        struct MsgsmsRequestConfirmationCodeRequest {
                int         type;
                Sid::String number;
                enum { BEGIN = 618, END = 620 };
        };
        struct MsgsmsRequestConfirmationCodeResponse {
                SmsRef sms;
                enum { BEGIN = 620, END = 621 };
        };
        struct MsgsmsSubmitConfirmationCodeRequest {
                Sid::String number;
                Sid::String code;
                enum { BEGIN = 621, END = 623 };
        };
        struct MsgsmsSubmitConfirmationCodeResponse {
                SmsRef sms;
                enum { BEGIN = 623, END = 624 };
        };
        struct MsgsmsCreateOutgoingSmsResponse {
                SmsRef sms;
                enum { BEGIN = 624, END = 625 };
        };
        struct MsgftTransferAcceptRequest {
                TransferRef   objectID;
                Sid::Filename filenameWithPath;
                enum { BEGIN = 625, END = 627 };
        };
        struct MsgftTransferAcceptResponse {
                bool success;
                enum { BEGIN = 627, END = 628 };
        };
        struct MsgftTransferPauseRequest {
                TransferRef objectID;
                enum { BEGIN = 628, END = 629 };
        };
        struct MsgftTransferResumeRequest {
                TransferRef objectID;
                enum { BEGIN = 629, END = 630 };
        };
        struct MsgftTransferCancelRequest {
                TransferRef objectID;
                enum { BEGIN = 630, END = 631 };
        };
        struct MsgftTransfer {
                int             type;
                Sid::String     partner_handle;
                Sid::String     partner_dispname;
                int             status;
                int             failurereason;
                uint            starttime;
                uint            finishtime;
                Sid::String     filepath;
                Sid::String     filename;
                Sid::String     filesize;
                Sid::String     bytestransferred;
                uint            bytespersecond;
                Sid::Binary     chatmsg_guid;
                uint            chatmsg_index;
                ConversationRef convo_id;                Sid::List_uint cachedProps;
                enum { BEGIN = 631, END = 646 };
        };
        struct MsgaccountAccountGetStatusWithProgressRequest {
                AccountRef objectID;
                enum { BEGIN = 646, END = 647 };
        };
        struct MsgaccountAccountGetStatusWithProgressResponse {
                int  status;
                uint progress;
                enum { BEGIN = 647, END = 649 };
        };
        struct MsgaccountAccountLoginRequest {
                AccountRef objectID;
                int        setAvailabilityTo;
                enum { BEGIN = 649, END = 651 };
        };
        struct MsgaccountAccountLoginWithPasswordRequest {
                AccountRef  objectID;
                Sid::String password;
                bool        savePwd;
                bool        saveDataLocally;
                enum { BEGIN = 651, END = 655 };
        };
        struct MsgaccountAccountRegisterRequest {
                AccountRef  objectID;
                Sid::String password;
                bool        savePwd;
                bool        saveDataLocally;
                Sid::String email;
                bool        allowSpam;
                enum { BEGIN = 655, END = 661 };
        };
        struct MsgaccountAccountLogoutRequest {
                AccountRef objectID;
                bool       clearSavedPwd;
                enum { BEGIN = 661, END = 663 };
        };
        struct MsgaccountAccountChangePasswordRequest {
                AccountRef  objectID;
                Sid::String oldPassword;
                Sid::String newPassword;
                bool        savePwd;
                enum { BEGIN = 663, END = 667 };
        };
        struct MsgaccountAccountSetPasswordSavedRequest {
                AccountRef objectID;
                bool       savePwd;
                enum { BEGIN = 667, END = 669 };
        };
        struct MsgaccountAccountSetServersideIntPropertyRequest {
                AccountRef objectID;
                int        propKey;
                uint       value;
                enum { BEGIN = 669, END = 672 };
        };
        struct MsgaccountAccountSetServersideStrPropertyRequest {
                AccountRef  objectID;
                int         propKey;
                Sid::String value;
                enum { BEGIN = 672, END = 675 };
        };
        struct MsgaccountAccountCancelServerCommitRequest {
                AccountRef objectID;
                enum { BEGIN = 675, END = 676 };
        };
        struct MsgaccountAccountSetIntPropertyRequest {
                AccountRef objectID;
                int        propKey;
                uint       value;
                enum { BEGIN = 676, END = 679 };
        };
        struct MsgaccountAccountSetStrPropertyRequest {
                AccountRef  objectID;
                int         propKey;
                Sid::String value;
                enum { BEGIN = 679, END = 682 };
        };
        struct MsgaccountAccountSetBinPropertyRequest {
                AccountRef  objectID;
                int         propKey;
                Sid::Binary value;
                enum { BEGIN = 682, END = 685 };
        };
        struct MsgaccountAccountSetAvailabilityRequest {
                AccountRef objectID;
                int        availability;
                enum { BEGIN = 685, END = 687 };
        };
        struct MsgaccountAccountSetStandbyRequest {
                AccountRef objectID;
                bool       standby;
                enum { BEGIN = 687, END = 689 };
        };
        struct MsgaccountAccountGetCapabilityStatusRequest {
                AccountRef objectID;
                int        capability;
                enum { BEGIN = 689, END = 691 };
        };
        struct MsgaccountAccountGetCapabilityStatusResponse {
                int  status;
                uint expiryTimestamp;
                enum { BEGIN = 691, END = 693 };
        };
        struct MsgaccountAccountGetSkypenameHashRequest {
                AccountRef objectID;
                enum { BEGIN = 693, END = 694 };
        };
        struct MsgaccountAccountGetSkypenameHashResponse {
                Sid::String skypenameHash;
                enum { BEGIN = 694, END = 695 };
        };
        struct MsgaccountAccountGetVerifiedEmailRequest {
                AccountRef objectID;
                enum { BEGIN = 695, END = 696 };
        };
        struct MsgaccountAccountGetVerifiedEmailResponse {
                Sid::String email;
                enum { BEGIN = 696, END = 697 };
        };
        struct MsgaccountAccountGetVerifiedCompanyRequest {
                AccountRef objectID;
                enum { BEGIN = 697, END = 698 };
        };
        struct MsgaccountAccountGetVerifiedCompanyResponse {
                Sid::String company;
                enum { BEGIN = 698, END = 699 };
        };
        struct MsgaccountAccountDeleteRequest {
                AccountRef objectID;
                enum { BEGIN = 699, END = 700 };
        };
        struct MsgaccountAccount {
                Sid::String skypename;
                Sid::String fullname;
                uint        birthday;
                uint        gender;
                Sid::String languages;
                Sid::String country;
                Sid::String province;
                Sid::String city;
                Sid::String phone_home;
                Sid::String phone_office;
                Sid::String phone_mobile;
                Sid::String emails;
                Sid::String homepage;
                Sid::String about;
                uint        profile_timestamp;
                Sid::String mood_text;
                uint        timezone;
                uint        nrof_authed_buddies;
                int         availability;
                Sid::Binary avatar_image;
                int         status;
                int         pwdchangestatus;
                Sid::String suggested_skypename;
                int         logoutreason;
                Sid::String skypeout_balance_currency;
                uint        skypeout_balance;
                Sid::String skypein_numbers;
                Sid::String offline_callforward;
                int         commitstatus;
                int         cblsyncstatus;
                int         chat_policy;
                int         skype_call_policy;
                int         pstn_call_policy;
                int         avatar_policy;
                int         buddycount_policy;
                int         timezone_policy;
                int         webpresence_policy;
                int         phonenumbers_policy;
                int         voicemail_policy;
                uint        avatar_timestamp;
                uint        mood_timestamp;
                Sid::String rich_mood_text;
                Sid::String partner_optedout;
                Sid::String service_provider_info;
                uint        registration_timestamp;
                uint        nr_of_other_instances;
                uint        skypeout_precision;
                Sid::List_uint cachedProps;
                enum { BEGIN = 700, END = 747 };
        };
        struct MsgaccountGetAccountRequest {
                Sid::String identity;
                enum { BEGIN = 747, END = 748 };
        };
        struct MsgaccountGetAccountResponse {
                AccountRef account;
                enum { BEGIN = 748, END = 749 };
        };
        struct MsgaccountGetExistingAccountsResponse {
                Sid::List_String accountNameList;
                enum { BEGIN = 749, END = 751 };
        };
        struct MsgaccountGetDefaultAccountNameResponse {
                Sid::String account;
                enum { BEGIN = 751, END = 752 };
        };
        struct MsgaccountGetSuggestedSkypenameRequest {
                Sid::String fullname;
                enum { BEGIN = 752, END = 753 };
        };
        struct MsgaccountGetSuggestedSkypenameResponse {
                Sid::String suggestedName;
                enum { BEGIN = 753, END = 754 };
        };
        struct MsgaccountValidateAvatarRequest {                Sid::Binary value;
                enum { BEGIN = 754, END = 755 };
        };
        struct MsgaccountValidateAvatarResponse {
                int result;
                int freeBytesLeft;
                enum { BEGIN = 755, END = 757 };
        };
        struct MsgaccountValidateProfileStringRequest {
                int         propKey;
                Sid::String strValue;
                bool        forRegistration;
                enum { BEGIN = 757, END = 760 };
        };
        struct MsgaccountValidateProfileStringResponse {
                int result;
                int freeBytesLeft;
                enum { BEGIN = 760, END = 762 };
        };
        struct MsgaccountValidatePasswordRequest {
                Sid::String username;
                Sid::String password;
                enum { BEGIN = 762, END = 764 };
        };
        struct MsgaccountValidatePasswordResponse {
                int result;
                enum { BEGIN = 764, END = 765 };
        };
        struct MsgconnectionOnProxyAuthFailure {
                int type;
                enum { BEGIN = 765, END = 766 };
        };
        struct MsgconnectionGetUsedPortResponse {
                uint port;
                enum { BEGIN = 766, END = 767 };
        };
        struct MsgsetupGetStrRequest {
                Sid::String key;
                enum { BEGIN = 767, END = 768 };
        };
        struct MsgsetupGetStrResponse {
                Sid::String value;
                enum { BEGIN = 768, END = 769 };
        };
        struct MsgsetupGetIntRequest {
                Sid::String key;
                enum { BEGIN = 769, END = 770 };
        };
        struct MsgsetupGetIntResponse {
                int value;
                enum { BEGIN = 770, END = 771 };
        };
        struct MsgsetupGetBinRequest {
                Sid::String key;
                enum { BEGIN = 771, END = 772 };
        };
        struct MsgsetupGetBinResponse {
                Sid::Binary value;
                enum { BEGIN = 772, END = 773 };
        };
        struct MsgsetupSetStrRequest {
                Sid::String key;
                Sid::String value;
                enum { BEGIN = 773, END = 775 };
        };
        struct MsgsetupSetIntRequest {
                Sid::String key;
                int         value;
                enum { BEGIN = 775, END = 777 };
        };
        struct MsgsetupSetBinRequest {
                Sid::String key;
                Sid::Binary value;
                enum { BEGIN = 777, END = 779 };
        };
        struct MsgsetupIsDefinedRequest {
                Sid::String key;
                enum { BEGIN = 779, END = 780 };
        };
        struct MsgsetupIsDefinedResponse {
                bool value;
                enum { BEGIN = 780, END = 781 };
        };
        struct MsgsetupDeleteRequest {
                Sid::String key;
                enum { BEGIN = 781, END = 782 };
        };
        struct MsgsetupGetSubKeysRequest {
                Sid::String key;
                enum { BEGIN = 782, END = 783 };
        };
        struct MsgsetupGetSubKeysResponse {
                Sid::List_String value;
                enum { BEGIN = 783, END = 785 };
        };
        struct MsgisoGetISOLanguageInfoResponse {
                Sid::List_String languageCodeList;
                Sid::List_String languageNameList;
                enum { BEGIN = 785, END = 789 };
        };
        struct MsgisoGetISOCountryInfoResponse {
                Sid::List_String countryCodeList;
                Sid::List_String countryNameList;
                Sid::List_uint   countryPrefixList;
                Sid::List_String countryDialExampleList;
                enum { BEGIN = 789, END = 797 };
        };
        struct MsgisoGetISOCountryCodebyPhoneNoRequest {
                Sid::String number;
                enum { BEGIN = 797, END = 798 };
        };
        struct MsgisoGetISOCountryCodebyPhoneNoResponse {
                Sid::String countryCode;
                enum { BEGIN = 798, END = 799 };
        };
        struct Msgapp2appApp2AppCreateRequest {
                Sid::String appname;
                enum { BEGIN = 799, END = 800 };
        };
        struct Msgapp2appApp2AppCreateResponse {
                bool result;
                enum { BEGIN = 800, END = 801 };
        };
        struct Msgapp2appApp2AppDeleteRequest {
                Sid::String appname;
                enum { BEGIN = 801, END = 802 };
        };
        struct Msgapp2appApp2AppDeleteResponse {
                bool result;
                enum { BEGIN = 802, END = 803 };
        };
        struct Msgapp2appApp2AppConnectRequest {
                Sid::String appname;
                Sid::String skypename;
                enum { BEGIN = 803, END = 805 };
        };
        struct Msgapp2appApp2AppConnectResponse {
                bool result;
                enum { BEGIN = 805, END = 806 };
        };
        struct Msgapp2appApp2AppDisconnectRequest {
                Sid::String appname;
                Sid::String stream;
                enum { BEGIN = 806, END = 808 };
        };
        struct Msgapp2appApp2AppDisconnectResponse {
                bool result;
                enum { BEGIN = 808, END = 809 };
        };
        struct Msgapp2appApp2AppWriteRequest {
                Sid::String appname;
                Sid::String stream;
                Sid::Binary data;
                enum { BEGIN = 809, END = 812 };
        };
        struct Msgapp2appApp2AppWriteResponse {
                bool result;
                enum { BEGIN = 812, END = 813 };
        };
        struct Msgapp2appApp2AppDatagramRequest {
                Sid::String appname;
                Sid::String stream;
                Sid::Binary data;
                enum { BEGIN = 813, END = 816 };
        };
        struct Msgapp2appApp2AppDatagramResponse {
                bool result;
                enum { BEGIN = 816, END = 817 };
        };
        struct Msgapp2appApp2AppReadRequest {
                Sid::String appname;
                Sid::String stream;
                enum { BEGIN = 817, END = 819 };
        };
        struct Msgapp2appApp2AppReadResponse {
                bool        result;
                Sid::Binary data;
                enum { BEGIN = 819, END = 821 };
        };
        struct Msgapp2appApp2AppGetConnectableUsersRequest {
                Sid::String appname;
                enum { BEGIN = 821, END = 822 };
        };
        struct Msgapp2appApp2AppGetConnectableUsersResponse {
                bool             result;
                Sid::List_String users;
                enum { BEGIN = 822, END = 825 };
        };
        struct Msgapp2appApp2AppGetStreamsListRequest {
                Sid::String appname;
                int         listType;
                enum { BEGIN = 825, END = 827 };
        };
        struct Msgapp2appApp2AppGetStreamsListResponse {
                bool             result;
                Sid::List_String streams;
                Sid::List_uint   receivedSizes;
                enum { BEGIN = 827, END = 832 };
        };
        struct Msgapp2appOnApp2AppDatagram {
                Sid::String appname;
                Sid::String stream;
                Sid::Binary data;
                enum { BEGIN = 832, END = 835 };
        };
        struct Msgapp2appOnApp2AppStreamListChange {
                Sid::String      appname;
                int              listType;
                Sid::List_String streams;
                Sid::List_uint   receivedSizes;
                enum { BEGIN = 835, END = 841 };
        };
#ifdef SE_USE_NAMESPACE
} // Skype
#endif

#endif // __SIDG_MSGS_SKYPEHPP_INCLUDED____
