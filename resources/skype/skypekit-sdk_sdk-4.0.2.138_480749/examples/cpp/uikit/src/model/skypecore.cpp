#include <assert.h>

#include "skypecore.h"
#include "skypeaccount.h"
#include "skypecontact.h"
#include "skypecontactgroup.h"
#include "skypeconversation.h"
#include "skypemessage.h"
#include "skypesms.h"
#include "skypeparticipant.h"
#include "skypevideo.h"
#include "skypetransfer.h"

#include <Poco/Delegate.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

// Initialize static variables to zero.
SkypeCore *SkypeCore::m_instance = 0;

// Using the Loopback and default port to connect to SkypeKit Runtime.
SEString SkypeCore::m_inetAddr = "127.0.0.1";
uint SkypeCore::m_portNum = 8963;

void SkypeCore::initializeInstance(const char* appTokenBuf)
{
	/* Create our customized Skype class, which will also start up. This
	 * class will initialize the Skype engine, create important types when
	 * requested and control event and requests from other classes.
	 */
	assert(m_instance == NULL);
	m_instance = new SkypeCore(appTokenBuf);
}

SkypeCore *SkypeCore::instance()
{
	/* his method uses the Singleton design pattern. It will restrict the
     * instantiation of the SkypeCore class to one object only.  Unlike
     * most Singleton implementation, we have a separate initialization
     * call that must be called before accessing the single instance here.
     */
	assert(SkypeCore::m_instance);
    return m_instance;
}

SkypeCore::SkypeCore(const char* appTokenBuf)
    : Skype(),
    m_currentIdentity("")
{
    /* It's time to start up the SkypeKit event loop in its own thread. For
     * this it's necessary to execute some precedures which need a bit more
     * attention.
     *
     * The first thing we need to pass when initializing the Skype engine is
     * the Application Token. This token will be submitted to the server and
     * will grant permission to our application log into the Skype Network. As
     * the Token can change from time to time depending on the needs of the
     * Skype Network, we need to pay attention and use a valid one.
     *
     * The second information we need to start up the engine is Skype Runtime
     * address and port number. On this case we already set it to use as a
     * local connection.
     */
	printf("Calling init");
	
    init(appTokenBuf, m_inetAddr, m_portNum, "qtsimpleclient.log");

    // Now that everything is initialized properly, lets start the engine.
	printf("Calling start");
    start();

    // It's also needed to setup the playback and recording sounds device.
    setupSoundDevice();
}

SkypeCore::~SkypeCore()
{
    // Stopping Skype Core.
    stop();
}

bool SkypeCore::isLiveSessionUp()
{
    SEString oid = findLiveSession();

    return (oid.isEmpty() || oid.isNull() ? false : true);
}

SEString SkypeCore::currentIdentity() const
{
    return m_currentIdentity;
}

void SkypeCore::setCurrentIdentity(SEString identity)
{
    m_currentIdentity = identity;
}

void SkypeCore::OnConversationListChange(const ConversationRef &conversation,
                                       const Conversation::LIST_TYPE &type,
                                       const bool &added)
{
    /* In order to catch new conversations (like calls, video calls, IM and
     * etc) it's necessary to overwrite the Skype::OnConversationListChange
     * method. Thus each time a conversation list gets changed this method
     * will be called, taking the right action for the event.
     */

    /* It's important to remember that only one live conversation can be
     * happening at any time. There can be a conference with multiple
     * participants, but only one conversation in a Skype instance can be live.
     * The correct way of doing this would involve checking
     * Skype::GetConversationList with the LIVE_CONVERSATIONS filter. if you
     * find a live conversation with the live status as IM_LIVE it means that
     * there's already a live conversation ongoing.
     */

    // Get the list name and the current live status.
    SEString listName;
    Conversation::LOCAL_LIVESTATUS liveStatus;
    conversation->GetPropLocalLivestatus(liveStatus);

    // Get a SkypeConversation so we can check if it is Active already.
    SkypeConversation::Ref conv(conversation);

    /* Now it's time check which kind of list has changed. The possible types
     * are:
     * ALL_CONVERSATIONS : All conversations in last 30 days;
     * INBOX_CONVERSATIONS : The last 6 months conversations are kept there;
     * BOOKMARKED_CONVERSATIONS : Marked as bookmarked (is_bookmarked is set);
     * LIVE_CONVERSATIONS : The local_livestatus is different from NONE.
     */
    if (type == Conversation::LIVE_CONVERSATIONS &&
        liveStatus == Conversation::RINGING_FOR_ME) {
        if (isLiveSessionUp()) {
            printf("Another call is coming in.\n");
            printf("We already have a live session up, we will reject it. \n");

            /* There's already another live session ongoing. We need to
             * refuse this new one.
             */
            conv->LeaveLiveSession(true);
        }
        else {
            printf("Handling new incoming call.\n");

            /* When an incoming call arrives to an already existing conversation
             * this method will not be called again, since the conversation list
             * will not be changed. The incoming call event will be signalled
             * inside the existing conversation, that will need to handle it.
             * Thus it's necessary to listen for the Conversation's incomingCall
             * signal that will be emitted once a new call arrive to that
             * object.
             */
            conv->incomingCall += Poco::Delegate<SkypeCore, ConversationRef>
                (this, &SkypeCore::onIncomingCall);

            // No other live session ongoing, lets join this one.
            newCallConversation(this, conv);
        }
    }
}

void SkypeCore::onIncomingCall(const void *sender, ConversationRef &conv)
{
    newCallConversation(this, conv);
}

void SkypeCore::OnMessage(const MessageRef &message,
    const bool &changesInboxTimestamp,
    const MessageRef &sepersedesHistoryMessage,
    const ConversationRef &conversation)
{
    Message::TYPE messageType;
    message->GetPropType(messageType);

    // Get a SkypeConversation so we can check if it is Active already.
    SkypeConversation::Ref conv(conversation);

    if ((messageType == Message::POSTED_TEXT
        || messageType == Message::POSTED_EMOTE)
            && !conv->isActive()) {
        newChatConversation(this, conv);
    }
    else if (messageType == Message::POSTED_FILES) {
        /* It seems that we have a new file transfer request. The first thing we
         * need to do is to get all transfers (one per file) contained in the
         * message.
         */
        TransferRefs transfers;
        message->GetTransfers(transfers);

        /* Lets emit a new file transfer signal for each one of this transfer,
         * since they will be handled individually.
         */
        unsigned int size = transfers.size();
        for (unsigned int i = 0; i < size; ++i)
            newFileTransfer(this, transfers[i]);
    }
}

void SkypeCore::requestChat(const std::string &skypeName, bool smsMode)
{
    /* We need to get the conversation associated with the given skypename,
     * so we need to make a participant list containing our one contact.
     */
    Sid::List_String participants;
    participants.push_back(skypeName.c_str());

    // Now we get the conversation from our participant list.
    ConversationRef conversation;
    GetConversationByParticipants(participants, conversation, true);

    if (smsMode)
        startSmsForConversation(conversation);
    else
        startChatForConversation(conversation);
}

void SkypeCore::requestChatByConversation(const std::string &conversationId)
{
    ConversationRef conversation;
    SEString idString(conversationId.c_str());
    GetConversationByIdentity(idString, conversation);

    startChatForConversation(conversation);
}

void SkypeCore::startChatForConversation(ConversationRef &conversation)
{
    if (!conversation) {
        printf("Fatal: Invalid conversation Id.\n");
        return;
    }

    // Send our signal notifying that a new Chat was requested.
    Mvp::Model::SkypeConversation::Ref skypeconversation(conversation);
    newChatConversation(this, skypeconversation);
}

void SkypeCore::startSmsForConversation(ConversationRef &conversation)
{
    if (!conversation) {
        printf("Fatal: Invalid conversation Id.\n");
        return;
    }

    // Send our signal notifying that a new Chat was requested.
    Mvp::Model::SkypeConversation::Ref skypeconversation(conversation);
    newSmsConversation(this, skypeconversation);
}

void SkypeCore::requestCall(const std::string &skypeName)
{
    /* There's already another live session ongoing. We cannot start a new
     * one now.
     */
    if (isLiveSessionUp()) {
        printf("We already have a live session up, dropping call request. \n");
        return;
    }

    /* We need to get the conversation associated with the given skypename,
     * and to do so we need to convert it to a participant list.
     */
    Sid::List_String participants;
    participants.push_back(skypeName.c_str());

    // Now we get the conversation from our participant list.
    ConversationRef conversation;
    GetConversationByParticipants(participants, conversation, true);

    startCallForConversation(conversation);
}

void SkypeCore::requestCallByConversation(const std::string &conversationId)
{
    ConversationRef conversation;
    SEString idString(conversationId.c_str());
    GetConversationByIdentity(idString, conversation);

    startCallForConversation(conversation);
}

void SkypeCore::startCallForConversation(ConversationRef &conversation)
{
    if (!conversation) {
        printf("Fatal: Invalid conversation Id.\n");
        return;
    }

    ParticipantRefs participantList;
    conversation->GetParticipants(participantList, Conversation::ALL);

    // Now we need to get the real participant obj and call it!
    for (unsigned int i = 0; i < participantList.size(); ++i) {
        SEString oid;
        participantList[i]->GetPropIdentity(oid);

        if (oid != m_currentIdentity) {
            participantList[i]->Ring();
        }
    }

    SkypeConversation::Ref conv(conversation);

    /* When an incoming call arrives to an already existing conversation
     * this method will not be called again. The incoming call event will
     * be signalled inside the existing conversation, that will need to
     * handle it. Thus it's necessary to listen for the Conversation's
     * incomingCall signal that will be emitted once a new call arrive to
     * that object.
     */
    conv->incomingCall += Poco::Delegate<SkypeCore, ConversationRef>
        (this, &SkypeCore::onIncomingCall);

    Mvp::Model::SkypeConversation::Ref skypeconversation(conversation);
    newCallConversation(this, skypeconversation);
}

void SkypeCore::requestFileTransfer(const std::string &skypeName,
        const std::string &filePath)
{
    /* We need to get the conversation associated with the given skypename,
     * and to do so we need to convert it to a participant list.
     */
    Sid::List_String participants;
    participants.push_back(skypeName.c_str());

    /* We also need to add the file path to a list of files, in case
     * we want to send more than one file in one single shot.
     */
    Sid::List_Filename files;
    files.push_back(filePath.c_str());

    // Lets give the file a default description.
    Sid::String fileDescription = "New file";

    // Now we get the conversation from our participant list.
    ConversationRef conversation;
    GetConversationByParticipants(participants, conversation, true);

    TRANSFER_SENDFILE_ERROR errorCode;
    Sid::Filename errorFile;

    conversation->PostFiles(files, fileDescription, errorCode, errorFile);
}

SEString SkypeCore::findLiveSession()
{
    SEString oid;
    ConversationRefs conversations;

    // Query Skypekit for all Live Session conversations.
    if (GetConversationList(conversations, Conversation::LIVE_CONVERSATIONS)) {
        int listSize = conversations.size();
        Conversation::LOCAL_LIVESTATUS status;

        // Lets check if one of them has the IM_LIVE status.
        for (int i = 0; i < listSize; ++i) {
            status = Conversation::NONE;

            // Get the conversation Local live status.
            conversations[i]->GetPropLocalLivestatus(status);

            // If it's IM_LIVE we already have a live conversation ongoing.
            if (status == Conversation::IM_LIVE) {
                conversations[i]->GetPropIdentity(oid);
                break;
            }
        }
    }

    return oid;
}

void SkypeCore::setupSoundDevice()
{
    SEStringList outHandles, outNames, outProductIDs;
    SEStringList inHandles, inNames, inProductIDs;

    // Search for playback devices.
    GetAvailableOutputDevices(outHandles, outNames, outProductIDs);

    // Search for recording devices.
    GetAvailableRecordingDevices(inHandles, inNames, inProductIDs);

    /* As we are not going to have client configuration window lets
     * use the first available device for both situations.
     */
    if (outHandles.size() > 0 && inHandles.size() > 0)
        SelectSoundDevices(inHandles[0], outHandles[0], outHandles[0]);
}

Account *SkypeCore::newAccount(int oid)
{
    /* This method will be called whenever a new account is needed, so lets
     * return an instance of our customized Account class.
     */
    return new SkypeAccount(oid, this);
}

Contact *SkypeCore::newContact(int oid)
{
    /* This method will be called whenever a new contact is needed, so lets
     * return an instance of our customized Contact class.
     */
    return new SkypeContact(oid, this);
}

ContactGroup *SkypeCore::newContactGroup(int oid)
{
    /* This method is called when a new contactgroup is needed, so lets return
     * an instance of our customized ContactGroup class.
     */
    return new SkypeContactGroup(oid, this);
}

Conversation *SkypeCore::newConversation(int oid)
{
    /* This method is called when a new Conversation is needed, so lets return
     * an instance of our SkypeConversation class.
     */
    return new SkypeConversation(oid, this);
}

Message *SkypeCore::newMessage(int oid)
{
    /* This method is called when a new Message is needed, so lets return
     * an instance of our SkypeMessage class.
     */
    return new SkypeMessage(oid, this);
}

Participant *SkypeCore::newParticipant(int oid)
{
    /* This method is called when a new Participant is needed, so lets return
     * an instance of our SkypeParticipant class.
     */
    return new SkypeParticipant(oid, this);
}

Sms *SkypeCore::newSms(int oid)
{
    /* This method is called when a new Sms is needed, so lets return an
     * instance of our SkypeSms class.
     */
    return new SkypeSms(oid, this);
}

Video *SkypeCore::newVideo(int oid)
{
    /* This method is called when a new Video is needed, so lets return an
     * instance of our SkypeVideo class.
     */
    return new SkypeVideo(oid, this);
}

Transfer *SkypeCore::newTransfer(int oid)
{
    /* This method is called when a new Transfer is needed, so lets return an
     * instance of our SkypeTransfer class.
     */
    return new SkypeTransfer(oid, this);
}

};

};
