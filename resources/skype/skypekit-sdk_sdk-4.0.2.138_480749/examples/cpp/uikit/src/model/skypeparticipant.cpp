#include "skypeparticipant.h"
#include "skypecore.h"

namespace Mvp
{

namespace Model
{

SkypeParticipant::SkypeParticipant(unsigned int oid, SERootObject* root)
    : Participant(oid, root),
    m_live(false),
    m_videoSupport(false)
{
    SEString participantId;
    GetPropIdentity(participantId);

    // Check if this participant is the local one (the account user).
    m_localParticipant =
        (participantId == SkypeCore::instance()->currentIdentity());

    Participant::VOICE_STATUS status;
    GetPropVoiceStatus(status);

    // We need to initialize m_live.
    m_live = (status == Participant::LISTENING ||
         status == Participant::SPEAKING ||
         status == Participant::RINGING);

    printf("New Participant created.\n.");
}

SkypeParticipant::~SkypeParticipant()
{
}

bool SkypeParticipant::isLive() const
{
    return m_live;
}

bool SkypeParticipant::isLocalParticipant() const
{
    return m_localParticipant;
}

void SkypeParticipant::OnChange(int prop)
{
    /* Here is the place where you can capture all property changes when they
     * happen. It's important to know that this method will receive ALL changes
     * from ALL properties, which means that it's necessary to create a nice and
     * robust filter inside this method to handle each type of property,
     * otherwise it can fail by acting incorrectly or even lose some events.
     *
     * Another interesting thing is that this method does not receive the new
     * value of the changed property. It's necessary to get the value from the
     * Participant class itself, because once this method is called the property
     * is already changed in the class.
     */

    /* For our Participant class we will ony watch for the following
     * properties:
     *
     * - Participant::P_TEXT_STATUS  : Changes when the text status is changed.
     * - Participant::P_VOICE_STATUS : Changes when the voice status is changed.
     */

    /* We watch for P_VOICE_STATUS changes here, so we can send a signal when
     * this participant has joined the call (The voice status has become either
     * LISTENING, SPEAKING, or RINGING), or when this participant has left
     * the call (The voice status has become VOICE_STOPPED).
     */
    if (prop == Participant::P_VOICE_STATUS) {
        Participant::VOICE_STATUS status;
        GetPropVoiceStatus(status);

        // Test if the participant is joining the call.
        if ((status == Participant::LISTENING ||
             status == Participant::SPEAKING ||
             status == Participant::RINGING) &&
            !m_live) {

            // Set m_live so we wont emit the signal more than once.
            m_live = true;

            Poco::EventArgs args;
            joinedCall(this, args);
        }
        // Test if the participant left the call.
        else if (m_live && status == Participant::VOICE_STOPPED) {
            // Clear m_live so we wont emit the signal more than once.
            m_live = false;

            Poco::EventArgs args;
            leftCall(this, args);
        }
    }
    else if (prop == Participant::P_VIDEO_STATUS) {
        // Get the video status.
        Participant::VIDEO_STATUS status;
        GetPropVideoStatus(status);

        // If it's become available, emit videoAvailable(true).
        if (status == Participant::VIDEO_AVAILABLE && !m_videoSupport) {
            m_videoSupport = true;
            videoAvailable(this, m_videoSupport);
        }
        // If it's no longer available, emit videoAvailable(false).
        else if (status == Participant::VIDEO_UNKNOWN && m_videoSupport) {
            m_videoSupport = false;
            videoAvailable(this, m_videoSupport);
        }
    }
}

};

};

