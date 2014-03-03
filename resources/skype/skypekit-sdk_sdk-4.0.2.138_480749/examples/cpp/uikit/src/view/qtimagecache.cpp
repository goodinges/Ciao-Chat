#include "qtimagecache.h"

#include <QDebug>

namespace Mvp
{

namespace View
{

// Declare QtImageCache::m_instance so it exists.
QtImageCache *QtImageCache::m_instance = 0;

QtImageCache *QtImageCache::instance()
{
    // if m_instance hasn't been allocated, allocate it of our type.
    if (!m_instance)
        m_instance = new QtImageCache();

    return m_instance;
}

QtImageCache::~QtImageCache()
{
    m_cache.clear();
}

QImage QtImageCache::image(
        const Mvp::Model::ContactAvailability availability) const
{
    return m_cache[availability];
}

QImage QtImageCache::historyImage(const Mvp::Model::HistoryType historyType)
const
{
    return m_historyCache[historyType];
}

QtImageCache::QtImageCache()
{
    qDebug() << "Creating image cache.";

    loadImages();
}

void QtImageCache::loadImages()
{
    qDebug() << "Loading images.";

    // Loads all status image.
    m_cache.insert(Mvp::Model::Available,
        QImage(Mvp::View::SkypeImage::PresenceOnline));
    m_cache.insert(Mvp::Model::Away,
        QImage(Mvp::View::SkypeImage::PresenceAway));
    m_cache.insert(Mvp::Model::Busy,
        QImage(Mvp::View::SkypeImage::PresenceBusy));
    m_cache.insert(Mvp::Model::SkypeOut,
        QImage(Mvp::View::SkypeImage::SkypeOut));
    m_cache.insert(Mvp::Model::Offline,
        QImage(Mvp::View::SkypeImage::PresenceOffline));

    m_historyCache.insert(Mvp::Model::IncomingCall,
        QImage(Mvp::View::SkypeImage::CallIn));
    m_historyCache.insert(Mvp::Model::OutgoingCall,
        QImage(Mvp::View::SkypeImage::CallOut));
    m_historyCache.insert(Mvp::Model::FailedCall,
        QImage(Mvp::View::SkypeImage::CallOutFailed));
    m_historyCache.insert(Mvp::Model::MissedCall,
        QImage(Mvp::View::SkypeImage::CallMissed));
    m_historyCache.insert(Mvp::Model::Chat,
        QImage(Mvp::View::SkypeImage::Message));
    m_historyCache.insert(Mvp::Model::ConferenceIn,
        QImage(Mvp::View::SkypeImage::CallConferenceIn));
    m_historyCache.insert(Mvp::Model::ConferenceOut,
        QImage(Mvp::View::SkypeImage::CallConferenceOut));
    m_historyCache.insert(Mvp::Model::MissedConference,
        QImage(Mvp::View::SkypeImage::CallConferenceMissed));
    m_historyCache.insert(Mvp::Model::VoiceMail,
        QImage(Mvp::View::SkypeImage::Voicemail));
    m_historyCache.insert(Mvp::Model::OutgoingTransfer,
        QImage(Mvp::View::SkypeImage::FileUpload));
    m_historyCache.insert(Mvp::Model::IncomingTransfer,
        QImage(Mvp::View::SkypeImage::FileDownload));
}

};

};
