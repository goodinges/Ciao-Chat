#ifndef _QTCONTACTLISTMODEL_H
#define _QTCONTACTLISTMODEL_H

#include <vector>

#include <QAbstractListModel>

#include <Poco/SharedPtr.h>

#include "model/contactdata.h"

class Ui_ContactsDialog;
class QListWidgetItem;

namespace Mvp
{

namespace View
{

/**
 * @class QtContactListModel qtcontactlistmodel.h qtcontactlistmodel.h
 * @brief A model used for storing and formatting contacts.
 *
 * This class is a data model that will store a list of contacts and feed
 * the Contact List view with the right information.
 *
 * @see QtContactListView
 */
class QtContactListModel : public QAbstractListModel
{
Q_OBJECT
public:
    enum Roles
    {
        DisplayName = Qt::UserRole,
        SkypeName,
        AvailabilityDesc,
        AvailabilityStatus,
        Avatar,
        AvailabilityIcon,
        CanIM,
        CanCall,
        CanFT
    };

    /**
     * Constructor.
     *
     * @param parent A pointer to the object's parent.
     */
    QtContactListModel(QObject *parent = 0);

    /**
     * Destructor.
     */
    ~QtContactListModel();

    /**
     * Load the initial list of contacts.
     *
     * @param contacts a vector containing a set of contacts.
     */
    void setContacts(std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts);

    /**
     * Get the list of contacts.
     *
     * @return a vector of contactdata currently in the model.
     */
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > contacts() const;

    /**
     * Get the data for a given index.
     *
     * @return the contact data for the index given.
     */
    Poco::SharedPtr<Mvp::Model::ContactData> dataFromIndex(const QModelIndex &index);

    /**
     * Update an existing Contact.
     *
     * This method is used to update an existing contact when one
     * of its properties is changed.
     *
     * @param data the new Contact data information.
     */
    void updateContact(const Poco::SharedPtr<Mvp::Model::ContactData> data);

    /**
     * Remove an existing contact from the model.
     *
     * This method is used to remove an existing contact from the model.
     *
     * @param skypeName the skypename of the contact to remove.
     */
    void removeContact(const std::string &skypeName);

    /**
     * Add a contact to the model.
     *
     * This method is used to add a new contact to the model.
     *
     * @param contact the Contact data to add to the model.
     */
    void addContact(const Poco::SharedPtr<Mvp::Model::ContactData> contact);

    /**
     * Return the number of rows.
     *
     * @return the number of rows.
     */
    int rowCount(const QModelIndex &) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    std::vector<Poco::SharedPtr<Mvp::Model::ContactData> > m_contacts;
};

};

};
#endif
