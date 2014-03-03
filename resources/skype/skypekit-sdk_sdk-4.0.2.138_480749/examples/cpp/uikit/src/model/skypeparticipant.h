#ifndef _SKYPEPARTICIPANT_H
#define _SKYPEPARTICIPANT_H

#include "skype-embedded_2.h"

#include <Poco/BasicEvent.h>
#include <Poco/EventArgs.h>

namespace Mvp
{

namespace Model
{

/**
 * @class SkypeParticipant skypeparticipant.h skypeparticipant.h
 * @brief Implements basic features of the SkypeKit Participant class.
 *
 * In order to intercept the participant's property change events it's necessary
 * to subclass the Participant class and overwrite some basic virtual methods that
 * will be called when a property is changed. The most important virtual method
 * is:
 *
 * void OnChange(int prop): This method will be called whenever a property is
 * changed. Here it's possible to catch all changes related to the participant,
 * like: VoiceStatus, TextStatus, VideoStatus, etc.
 */
class SkypeParticipant : public Participant
{
public:
    typedef DRef<SkypeParticipant, Participant> Ref;

    /**
     * Destructor.
     */
    ~SkypeParticipant();

    /**
     * Return if the participant is in a live session.
     *
     * @return true if the participant is live, otherwise false.
     */
    bool isLive() const;

    /**
     * Return if the participant is the local one (the account user).
     *
     * @return true if the participant identity is the same used for the
     * account login, otherwise false.
     */
    bool isLocalParticipant() const;

    /**
     * Participant joined the voice call.
     *
     * This signal is emitted when the participant's voice status changes
     * from RINGING or NA or such to LISTENING or SPEAKING.  This is needed
     * to know when a participant has joined the call and is not just part
     * of it e.g. the participant is live.
     */
    Poco::BasicEvent<Poco::EventArgs> joinedCall;

    /**
     * Participant left the voice call.
     *
     * This signal is emitted when the participant's voice status changes
     * to VOICE_STOPPED, after they left a voice call.
     */
    Poco::BasicEvent<Poco::EventArgs> leftCall;

    /**
     * Signal that the participant has video support.
     *
     * When the participant's VIDEO_STATUS changes from anything to
     * VIDEO_AVAILABLE this signal is emitted with the value of true.
     * When the participant's VIDEO_STATUS changes to VIDEO_UNKNOWN after
     * having been available, this signal is emitted with false.
     */
    Poco::BasicEvent<bool> videoAvailable;

protected:
    /**
     * Constructor.
     */
    SkypeParticipant(unsigned int oid, SERootObject *root);

    /**
     * Reflects property changes.
     *
     * When a Participant's property is changed this method will be called.
     * It's important to know that when this method is called the property
     * itself is already changed inside the Participant's class, which means
     * that you have to get the new property value by using the property index
     * passed to this method.
     *
     * @param prop the property index.
     */
    void OnChange(int prop);

private:
    friend class SkypeCore;

    bool m_live;
    bool m_videoSupport;
    bool m_localParticipant;
};

};

};

#endif

