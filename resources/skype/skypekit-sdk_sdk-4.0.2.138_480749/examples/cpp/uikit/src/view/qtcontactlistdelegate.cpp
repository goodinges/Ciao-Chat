#include <QApplication>
#include <QFont>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QSortFilterProxyModel>

#include <QDebug>

#include "qtcontactlistmodel.h"
#include "qtcontactlistdelegate.h"
#include "skypeimages.h"

namespace Mvp
{

namespace View
{

static const int Margin = 5;
static const int Elide = 10;

QtContactListDelegate::QtContactListDelegate(QObject *parent)
        : QStyledItemDelegate(parent)
{
    // Initialize the default avatar image.
    m_avatarImage = QImage(Mvp::View::SkypeImage::Contact);
}

QtContactListDelegate::~QtContactListDelegate()
{
}

void QtContactListDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    if (index.isValid()) {
        painter->save();

        painter->setRenderHint(QPainter::Antialiasing, true);

        /* Force drawing panel primitives, like item selection, on mouse over
         * and click.
         */
        QStyleOptionViewItemV4 opt(option);
        initStyleOption(&opt, index);
        QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt,
                painter);

        // Get font details.
        QFont font = option.font;


        // Draw Avatar image.
        QVariant variant  = index.data(QtContactListModel::Avatar);
        QImage drawImage;

        /* First let's check if the contact has an avatar. If not,
         * let's fallback to the default avatar image.
         */
        if (variant.type() == QVariant::Image && !variant.isNull())
            drawImage = variant.value<QImage>().scaled(QSize(32, 32));
        else
            drawImage = m_avatarImage;

        // Next calculate the avatar location, and paint it.
        QPoint avatarPoint((option.rect.right() - drawImage.rect().width()) - Margin,
                            option.rect.top() + ((option.rect.height() -
                                               drawImage.rect().height()) / 2));
        QRect avatarImageRect(avatarPoint, QSize(drawImage.rect().width(),
                             drawImage.rect().height()));

        painter->drawImage(avatarImageRect, drawImage);


        // Draw the Availability Status.
        QImage statusImage;
        variant = index.data(QtContactListModel::AvailabilityIcon);
        if (variant.type() == QVariant::Image && !variant.isNull())
            statusImage = variant.value<QImage>();

        QRect availStatusRect(option.rect.left() + Margin,
                option.rect.top() + ((option.rect.height() -
                        statusImage.rect().height()) / 2),
                statusImage.rect().width(), statusImage.rect().height());
        painter->drawImage(availStatusRect, statusImage);


        // Draw the contact's display name + skype name.
        QString text = index.data(QtContactListModel::DisplayName).toString();

        font.setPointSize(12);
        painter->setFont(font);

        qreal fontHeight = QFontMetricsF(font).height();

        QPoint firstRowPoint(availStatusRect.right() + (Margin * 2),
                option.rect.top() + Margin);
        text = QFontMetricsF(font).elidedText(text, Qt::ElideRight,
                (avatarImageRect.x() - Elide));
        QRect firstRowRect(firstRowPoint, QSize(QFontMetricsF(font).width(text),
                    fontHeight));

        painter->drawText(firstRowRect, Qt::AlignLeft, text);


        // Draw the contact's availability description.
        text = index.data(QtContactListModel::AvailabilityDesc).toString();

        font.setPointSize(9);
        font.setItalic(true);
        painter->setFont(font);
        painter->setPen(QPalette::Inactive);

        fontHeight = QFontMetricsF(font).height();

        QPoint secondRowPoint(availStatusRect.right() + (Margin * 2),
                ((option.rect.top() + (Margin * 2)) + fontHeight));
        text = QFontMetricsF(font).elidedText(text, Qt::ElideRight,
                avatarImageRect.x() - Elide);
        QRect secondRowRect(secondRowPoint,
                QSize(QFontMetricsF(font).width(text), fontHeight));

        painter->drawText(secondRowRect, Qt::AlignLeft, text);

        painter->restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize QtContactListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    // index and option are unused because all rows are of equal size.
    Q_UNUSED(index);
    Q_UNUSED(option);

    int height = 0;

    QFont boldfont(option.font);
    boldfont.setBold(true);
    QFontMetricsF bfm(boldfont);

    int fontHeight = bfm.height();
    int rowCount = 2;

    height = (Margin * 3) + (rowCount * fontHeight);

    return QSize(120, height);
}

};

};
