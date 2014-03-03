
#ifndef _QTHISTORYMODEL_H
#define _QTHISTORYMODEL_H

#include <QAbstractListModel>
#include <Poco/SharedPtr.h>

#include <vector>

#include "model/historydata.h"

namespace Mvp
{

namespace View
{

/**
 * @class QtHistoryModel qthistorymodel.h qthistorymodel.h
 * @brief A model used for storing and formatting History data.
 *
 * This class is a data model that will store a list of history and feed
 * the History view with the right information.
 *
 * @see QtHistoryView
 */
class QtHistoryModel: public QAbstractListModel
{
Q_OBJECT
public:
    enum Roles {
        ItemType = Qt::UserRole,
        Timestamp,
        Read
    };

    /**
     * Constructor.
     *
     * @param parent A pointer to the object's parent.
     */
    QtHistoryModel(QObject *parent = 0);

    /**
     * Destructor.
     */
    ~QtHistoryModel();

    /**
     * Load the initial list of history.
     *
     * @param history a vector containing a set of history items.
     */
    void setHistory(std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> > history);

    /**
     * Get the data for a given index.
     *
     * @return the contact data for the index given.
     */
    Poco::SharedPtr<Mvp::Model::HistoryData> dataFromIndex(const QModelIndex &index);

    /**
     * Return the number of rows.
     *
     * @return the number of rows.
     */
    int rowCount(const QModelIndex &) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> > m_history;
};

};

};

#endif
