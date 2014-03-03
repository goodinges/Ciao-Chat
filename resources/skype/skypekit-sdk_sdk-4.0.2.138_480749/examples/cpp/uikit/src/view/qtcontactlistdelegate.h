#ifndef CONTACTLISTDELEGATE_H
#define CONTACTLISTDELEGATE_H

#include <QStyledItemDelegate>

namespace Mvp
{

namespace View
{

/**
 * @class QtContactListDelegate qtcontactlistdelegate.h qtcontactlistdelegate.h
 * @brief a delegate used for painting the contact list.
 *
 * This class is a delegate which paints a list of contacts and their
 * availability.
 *
 * @see QtContactListView
 */
class QtContactListDelegate : public QStyledItemDelegate
{
Q_OBJECT
Q_DISABLE_COPY(QtContactListDelegate)

public:
    /**
     * Constructor.
     *
     * @param parent a pointer to the object's parent, if any.
     */
    QtContactListDelegate(QObject *parent = 0);

    /**
     * Destructor
     */
    ~QtContactListDelegate();

    /**
     * Paint method responsible for painting our custom view.
     *
     * This method does all the work of painting the data of the given index.
     * It draws the availability icon, the user's displayname, availability
     * text, and their avatar if any.
     *
     * @param painter the painter to paint with.
     * @param option the style option to use for this item.
     * @param index the index of the item to paint.
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    /**
     * Get the sizehint for a given index.
     *
     * This method is called by the view to get the sizehint to use for
     * a given index with a given style option.
     *
     * @param option the style option to use for this item.
     * @param index the index of the item to get the sizehint for.
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

Q_SIGNALS:
    /**
     * Signal for when an index is clicked. Currently the star in that index
     * is not used to show the detailed view when clicked.
     */
    void indexClicked(const QModelIndex &index);

private:

    QImage m_avatarImage;
};

};

};
#endif // CONTACTLISTDELEGATE_H
