#ifndef _SKYPECORE_H
#define _SKYPECORE_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>

using namespace Sid;

namespace Mvp
{

namespace Model
{
/**
 * @class SkypeCore skypecore.h skypecore.h
 * @brief Implements basic features of the Skype Core Engine.
 *
 * In order to intercept Skype Kit events it's necessary to subclass
 * the Skype class and override some basic virtual methods that will
 * be called when a new event arrives. The two most important virtual
 * methods are:
 *
 * Account *newAccount(): This method will be called whenever an
 * account object is requested. By overwriting this method it's possible
 * to return an instance of your own subclassed Account.
 *
 * void OnConversationListChanged(): This method will be called when the
 * conversation list gets changed. Here new conversations will be handled and
 * old ones will be deleted.
 *
 * As the SkypeCore is a very important and useful class it's being implemented
 * as a singleton class, which means that it can be accessed from any part of
 * the application code, by calling the SkypeCore::instance() method. It will
 * also guarantee that the application will have only one instance of this
 * class. Example:
 *
 * <code>
 * SkypeCore *core = SkypeCore::instance();
 * </code>
 *
 * @note The SkypeCore class is not responsible to start up the Skype Kit
 * Runtime and considers it ALREADY running.
 */
class SkypeCore : public Skype
{
public:
	
    /**
     * Allocate and intialize the unique instance of the SkypeCore class.
     *
     * This method is the only way to initialize the SkypeCore Singleton.
	 * It must be called before any other call to instance().  Here we
     * pass in the apptoken buffer used to initialize the skype runtime.
     */
	static void initializeInstance(const char* apptoken);
	
    /**
     * Returns the unique instance of the SkypeCore class.
     *
     * This method is part of the Singleton pattern. Instead of creating a
     * new instance of the SkypeCore class using the constructor, it should
     * be create by calling the static instance method, which will guarantee
     * that only one instance of this class will live in the application code,
     * providing an easy way to access it.
     *
     * @return SkypeCore instance.
     */
    static SkypeCore *instance();

    /**
     * Destructor.
     */
    ~SkypeCore();

    /**
     * Request a new chat with given contact.
     *
     * This method can be called to request a chat conversation with a contact.
     * It will not start a new chat session if there is already an active
     * conversation open with the given contact. If a new chat is started,
     * newIncomingConversation is sent so the new Conversation can be handled
     * by a new state, view, presenter, etc.
     *
     * @param skypeName the name of the contact to begin a chat with.
     * @param smsMode true to start the chat in sms mode, false for im mode.
     */
    void requestChat(const std::string &skypeName, bool smsMode = false);

    /**
     * Request a new chat with given conversationId.
     *
     * This is the same as requestChat, but takes a conversationId instead
     * of a skypeName.
     *
     * @param conversationId the id of the conversation to begin a chat with.
     */
    void requestChatByConversation(const std::string &conversationId);

    /**
     * Request a new call with given contact.
     *
     * This method can be called to request a call conversation with
     * the given contact.  It will not start a new call session if there
     * is already an active conversation open with the given contact. If
     * a new call is started, newIncomingConversation is sent so the new
     * Conversation can be handled by a new state, view, presenter, etc.
     *
     * @param skypeName the name of the contact to begin a chat with.
     */
    void requestCall(const std::string &skypeName);

    /**
     * Request a new call with given conversationId.
     *
     * This is the same as requestCall, but takes a conversationId instead
     * of a skypeName
     *
     * @param conversationId the id of the conversation to begin a chat with.
     */
    void requestCallByConversation(const std::string &conversationId);

    /*
     * Request a new file transfer session with a given contact.
     *
     * This method can be called to request a new file transfer session with
     * the given contact. It can be used to fire more than one file per time
     * and will emit the newFileTransfer signal when a new session is started.
     *
     * @param skypeName the name of the contact to begin a transfer with.
     * @param finePath the path of the file that is going  to be sent.
     */
    void requestFileTransfer(const std::string &skypeName,
            const std::string &filePath);

    /**
     * Returns a SkypeAccount instance.
     *
     * This method will be called whenever a new instance of the Account object
     * is requested. By overriding this method it's possible to return an
     * instance of your own subclassed Account.
     *
     * @param oid the object id.
     * @return Account instance.
     */
    Account *newAccount(int oid);

    /**
     * Returns a SkypeContact instance.
     *
     * This method will be called whenever a new instance of the Contact object
     * is requested. By overriding this method it's possible to return an
     * instance of your own subclassed Contact.
     *
     * @param oid the object id.
     * @return Contact instance.
     */
    Contact *newContact(int oid);

    /**
     * Returns a SkypeContactGroup instance.
     *
     * This method is called whenever a new ContactGroup object is needed.  By
     * overriding this method our own subclass is used for all ContactGroup
     * objects.
     */
    ContactGroup *newContactGroup(int oid);

    /**
     * Returns a SkypeConversation instance.
     *
     * This method is called when a new Conversiation object is needed.  By
     * overriding this method our own subclass is used for all Conversation
     * objects.
     */
    Conversation *newConversation(int oid);

    /**
     * Returns a SkypeMessage instance.
     *
     * This method is called when a new Message object is needed.  By
     * overriding this method our own subclass is used for all Message objects.
     */
    Message *newMessage(int oid);

    /**
     * Returns a SkypeParticipant instance.
     *
     * This method is called when a new Participant object is needed.  By
     * overriding this method our own subclass is used for all Participant
     * objects.
     */
    Participant *newParticipant(int oid);

    /**
     * Returns a SkypeSms instance.
     *
     * This method is called when a new Sms object is needed. By overriding
     * this method our own subclass is used for all Sms objects.
     */
    Sms *newSms(int oid);

    /**
     * Returns a SkypeVideo instance.
     *
     * This method is called when a new Video object is needed.  By
     * overriding this method our own subclass is used for all Video objects.
     */
    Video *newVideo(int oid);

    /**
     * Returns a SkypeTransfer instance.
     *
     * This method is called when a new Transfer object is needed. By
     * overriding this method our own subclass is used for all Transfer objects.
     */
    Transfer *newTransfer(int oid);

    /**
     * Check if there's an ongoing live session.
     *
     * return True if a live session is up, otherwise False.
     */
    bool isLiveSessionUp() ;

    /**
     * Returns the current logged-in identity.
     *
     * @return current Skype identity string.
     */
    SEString currentIdentity() const;

    /**
     * Set the current identity.
     *
     * @param identity the current logged in identity.
     */
    void setCurrentIdentity(SEString identity);

    /**
     * Search for ongoing live sessions.
     *
     * As SkypeKit can only handle one ongoing live session per time we need
     * a way to search for already ongoing live sessions. This method will
     * query SkypeKit for a list of already existing live sessions and will
     * check if one of them has the current status as IM_LIVE.
     *
     * @return A string containing the live conversation identity.
     */
    SEString findLiveSession();

    /**
     * Signal that a new chat conversation has started.
     */
    Poco::BasicEvent<const ConversationRef> newChatConversation;

    /**
     * Signal that a new sms mode conversation has started.
     */
    Poco::BasicEvent<const ConversationRef> newSmsConversation;

    /**
     * Signal that a new call conversation has begun.
     */
    Poco::BasicEvent<const ConversationRef> newCallConversation;

    /**
     * Signal that a new file transfer has started.
     */
    Poco::BasicEvent<const TransferRef> newFileTransfer;

private:
    /**
     * Constructor.
     */
    SkypeCore(const char* token);

    /**
     * Reflect changes on the conversation list.
     *
     * Whenever the internal conversation list is changed this method will
     * be called. Note that all changes need to be handled, otherwise they
     * will be lost.
     *
     * @param conversation the conversation object reference.
     * @param type the type of the conversation.
     * @param added True if new conversations has been added.
     */
    void OnConversationListChange(const ConversationRef &conversation,
        const Conversation::LIST_TYPE &type,
        const bool &added);

    /**
     * Triggered when a new message arrives.
     *
     * Whenever a new message arrive in the SkypeKit Context this method will be
     * called. The message can represent a simple plain text content sent by
     * another peer in a chat or even a notification that a new live session has
     * been started. All possible types are defined inside the Message class.
     *
     * @param message the message object reference.
     * @param changesInboxTimestamp if changesInboxTimestamp == true is a hint
     * that tray alert should probably be displayed.
     * @param supersedesHistoryMessage replace supersedesHistoryMessage message
     * in history view with this one.
     * @param conversation the conversation object reference.
     */
    void OnMessage(const MessageRef &message,
        const bool &changesInboxTimestamp,
        const MessageRef &supersedesHistoryMessage,
        const ConversationRef &conversation);

    /**
     * Helper method to setup the sound device.
     */
    void setupSoundDevice();

    /**
     * Slot to react to new incoming calls.
     *
     * @param sender a pointer to the object that sent the signal.
     * @param conv the new incoming call's conversation object.
     */
    void onIncomingCall(const void *sender, ConversationRef &conv);

    /**
     * Request skypekit to begin a chat for the given conversation.
     *
     * @param conversation the conversation to start a chat for.
     */
    void startChatForConversation(ConversationRef &conversation);

    /**
     * Request skypekit to begin an sms chat for the given conversation.
     *
     * @param conversation the conversation to start an sms chat for.
     */
    void startSmsForConversation(ConversationRef &conversation);

    /**
     * Request skypekit to begin a call for the given conversation.
     *
     * @param conversation the conversation to start a call for.
     */
    void startCallForConversation(ConversationRef &conversation);

    SEString m_currentIdentity;
    static SkypeCore *m_instance;
    static SEString m_inetAddr;
    static uint m_portNum;
};

};

};

#endif

