#ifndef _QTIMAGECACHE_H
#define _QTIMAGECACHE_H

#include "model/contactdata.h"
#include "model/historydata.h"
#include "skypeimages.h"

#include <QObject>
#include <QImage>
#include <QMap>

namespace Mvp
{

namespace View
{

/**
 * @class QtImageCache qtimagecache.h qtimagecache.h
 * @brief This class will cache images to a better performance.
 *
 * In order to avoid loading each image file from disk every time we need to use
 * it, this class will load all of them and cache in memory for easy and
 * quick access.
 */
class QtImageCache: public QObject
{
Q_OBJECT
public:
    /**
     * Returns the unique instance of the QtImageCache class.
     *
     * This method is part of the Singleton pattern. Instead of creating a
     * new instance of the QtImageCache class using the constructor, it should
     * be created by calling the static instance method, which will guarantee
     * that only one instance of this class will live in the application code,
     * providing an easy way to access it.
     *
     * @returns QtImageCache instance.
     */
    static QtImageCache *instance();

    /**
     * Destructor
     */
    ~QtImageCache();

    /**
     * Return the status image for the given availability status.
     *
     * @param availability the availability status requested.
     * @return a QImage that represents the given status.
     */
    QImage image(const Mvp::Model::ContactAvailability availability) const;

    /**
     * Return the history type image for the given history type.
     *
     * @param historyType the history type requested.
     * @return a QImage that represents the given history type.
     */
    QImage historyImage(const Mvp::Model::HistoryType historyType) const;

private:
    Q_DISABLE_COPY(QtImageCache)

    /**
     * This is a singleton, so its constructor is private.
     */
    QtImageCache();

    void loadImages();

    static QtImageCache *m_instance;
    QMap<Mvp::Model::ContactAvailability, QImage> m_cache;
    QMap<Mvp::Model::HistoryType, QImage> m_historyCache;
};

};

};
#endif
