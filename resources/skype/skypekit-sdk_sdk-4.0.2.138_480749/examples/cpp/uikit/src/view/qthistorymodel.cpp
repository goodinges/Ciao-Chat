#include "qthistorymodel.h"

#include "qtimagecache.h"
#include "skypeimages.h"

#include <QDateTime>
#include <QDebug>
#include <QFont>
#include <QImage>
#include <QUrl>


namespace Mvp
{

namespace View
{

QtHistoryModel::QtHistoryModel(QObject *parent)
{
}

QtHistoryModel::~QtHistoryModel()
{
}

void QtHistoryModel::setHistory(
        std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> > history)
{
    beginResetModel();

    m_history = history;

    endResetModel();
}

int QtHistoryModel::rowCount(const QModelIndex &) const
{
    return m_history.size();
}

Poco::SharedPtr<Mvp::Model::HistoryData> QtHistoryModel::dataFromIndex(
    const QModelIndex &index)
{
    Poco::SharedPtr<Mvp::Model::HistoryData> data(0);

    // Check the range of the index, and return the requested data.
    if (index.row() < (int)m_history.size() && index.row() >= 0)
        data = m_history[index.row()];

    return data;
}

QVariant QtHistoryModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if (index.isValid()  && index.row() <= (int)m_history.size()) {

        Poco::SharedPtr<Mvp::Model::HistoryData> item =
            m_history[index.row()];

        QString temp = item->text.c_str();
        temp.replace("&amp;", "&");
        temp.replace("&apos;", "'");
        temp.replace("&quot;", "\"");
        temp.replace("&lt;", "<");
        temp.replace("&gt;", ">");

        switch (role) {
            case Qt::DisplayRole:
                if (temp.isEmpty())
                    value = QString(item->displayName.c_str());
                else
                    value = QString("%1 - %2")
                        .arg(item->displayName.c_str())
                        .arg(temp);
                break;
            case Qt::FontRole:
            {
                QFont font;
                // Set unread items to be bold.
                font.setBold(!item->read);
                value = font;
                break;
            }
            case Qt::DecorationRole:
                value = QtImageCache::instance()->historyImage(item->type);
                break;
            case QtHistoryModel::ItemType:
                value = item->type;
                break;
            case QtHistoryModel::Timestamp:
                value = item->timestamp;
                break;
            case QtHistoryModel::Read:
                value = item->read;
                break;
            case Qt::ToolTipRole:
                // ToolTip contains displayname, timestamp.
                value = QString("%1 (%2)")
                    .arg(item->displayName.c_str())
                    .arg(QDateTime::fromTime_t(item->timestamp).toString());
                break;
            default:
                break;
        }
    }

    return value;
}

};

};
