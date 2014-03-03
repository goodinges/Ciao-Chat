#include "qtcontactlistmodel.h"
#include "qtimagecache.h"

#include <QImage>
#include <QDebug>

namespace Mvp
{

namespace View
{

QtContactListModel::QtContactListModel(QObject *parent)
{
}

QtContactListModel::~QtContactListModel()
{
}

void QtContactListModel::setContacts(
        std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts)
{
    beginResetModel();

    m_contacts = contacts;

    qDebug() << "m_contacts now has size of " << m_contacts.size();

    endResetModel();
}

std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > QtContactListModel::contacts() const
{
    return m_contacts;
}

void QtContactListModel::updateContact(
        const Poco::SharedPtr<Mvp::Model::ContactData> data)
{
    QString key(data->skypeName.data());

    unsigned int size = m_contacts.size();
    for (unsigned int i = 0; i < size; ++i) {
        // Get this contact.
        Poco::SharedPtr<Mvp::Model::ContactData> contact = m_contacts[i];

        // If it matches, update it.
        if (contact->skypeName.data() == key) {
            qDebug() << "Updating contact:" << key;

            m_contacts[i] = data;

            const QModelIndex realIndex = index(i, 0);

            // Tell the view we changed it.
            emit dataChanged(realIndex, realIndex);

            break;
        }
    }
}

void QtContactListModel::removeContact(const std::string &skypeName)
{
    QString key(skypeName.c_str());

    unsigned int size = m_contacts.size();
    for (unsigned int i = 0; i < size; ++i) {

        // If this contact matches.
        if (m_contacts[i]->skypeName.data() == key) {

            // Tell the view we are going to remove it.
            beginRemoveRows(QModelIndex(), i, i);

            // Then remove it.
            m_contacts.erase(m_contacts.begin() + i);

            // Then tell the view we removed it.
            endRemoveRows();

            // Then stop looking.
            break;
        }
    }
}

void QtContactListModel::addContact(const Poco::SharedPtr<Mvp::Model::ContactData> contact)
{
    beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());

    m_contacts.push_back(contact);

    endInsertRows();
}

int QtContactListModel::rowCount(const QModelIndex &) const
{
    return m_contacts.size();
}

Poco::SharedPtr<Mvp::Model::ContactData> QtContactListModel::dataFromIndex(
    const QModelIndex &index)
{
    Poco::SharedPtr<Mvp::Model::ContactData> data(0);

    // Check the range of the index, and return the requested data.
    if (index.row() < (int)m_contacts.size() && index.row() >= 0)
        data = m_contacts[index.row()];

    return data;
}

QVariant QtContactListModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if (index.isValid()  && index.row() <= (int)m_contacts.size()) {

        Poco::SharedPtr<Mvp::Model::ContactData> contact =
            m_contacts[index.row()];

        switch (role) {
            case QtContactListModel::DisplayName:
                value = QString(contact->displayName.c_str());
                break;
            case QtContactListModel::SkypeName:
                value = QString(contact->skypeName.c_str());
                break;
            case QtContactListModel::AvailabilityDesc:
                value = QString(contact->availability.c_str());
                break;
            case QtContactListModel::AvailabilityStatus:
                value = contact->status;
                break;
            case QtContactListModel::Avatar:
                if (contact->avatar.size() > 0) {
                    QImage avatar;
                    avatar.loadFromData(reinterpret_cast<const uchar *>(
                                contact->avatar.rawContent()),
                                contact->avatar.size());
                    value = QVariant(avatar);
                }
                break;
            case QtContactListModel::AvailabilityIcon:
                value = QVariant(
                        QtImageCache::instance()->image(contact->status));
                break;
            case QtContactListModel::CanIM:
                value = contact->canIM;
                break;
            case QtContactListModel::CanCall:
                value = contact->canCall;
                break;
            case QtContactListModel::CanFT:
                value = contact->canFT;
                break;
            case Qt::ToolTipRole:
                // ToolTip contains displayname, skypename, and availability.
                value = QString("%1 (%2) - %3")
                    .arg(contact->displayName.c_str())
                    .arg(contact->skypeName.c_str())
                    .arg(contact->availability.c_str());
                break;
            default:
                break;
        }
    }

    return value;
}

};

};
