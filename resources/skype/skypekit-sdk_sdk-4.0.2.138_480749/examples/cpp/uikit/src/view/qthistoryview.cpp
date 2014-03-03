#include "qthistoryview.h"
#include "skypeimages.h"
#include "qthistorymodel.h"
#include "ui_qt-history.h"

#include <QDebug>

#include <QSortFilterProxyModel>

namespace Mvp
{

namespace View
{

QtHistoryView::QtHistoryView(QWidget *parent)
    :QWidget(parent),
    ui(new Ui_HistoryView),
    m_model(new QtHistoryModel),
    m_sortModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    // Create the sort model and initialize it to sort by time.
    m_sortModel->setSortRole(QtHistoryModel::Timestamp);

    m_sortModel->setSourceModel(m_model);

    ui->historyListView->setModel(m_sortModel);

    // Initialize the sort role.
    onSortOrderChanged(ui->sortComboBox->currentIndex());

    connect(ui->sortComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onSortOrderChanged(int)));

    connect(ui->historyListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(onItemDoubleClicked(QModelIndex)));

    connect(ui->markReadButton, SIGNAL(clicked()),
            this, SLOT(onMarkReadButtonClicked()));
}

QtHistoryView::~QtHistoryView()
{
    delete m_model;
    delete m_sortModel;
    delete ui;
}

void QtHistoryView::destroyView()
{
    deleteLater();
}

void QtHistoryView::initialize()
{
}

void QtHistoryView::showView()
{
    // Call QDialog::show with invokeMethod so it runs in the Qt GUI thread.
    if (isVisible() == false)
        show();
    else
        activateWindow();
}

void QtHistoryView::onSortOrderChanged(int sortIndex)
{
    switch (sortIndex) {
        case 0: // Read
            m_sortModel->setSortRole(QtHistoryModel::Read);
            break;
        case 1: // Date
            m_sortModel->setSortRole(QtHistoryModel::Timestamp);
            break;
        case 2: // Name
            m_sortModel->setSortRole(Qt::DisplayRole);
            break;
        case 3: // Type
            m_sortModel->setSortRole(QtHistoryModel::ItemType);
            break;
    }

    reSort();
}

void QtHistoryView::onItemDoubleClicked(const QModelIndex &index)
{
    QModelIndex realIndex = m_sortModel->mapToSource(index);
    Poco::SharedPtr<Mvp::Model::HistoryData> data = m_model->dataFromIndex(realIndex);

    std::string conversationId = data->conversationIdentity;

    switch (data->type) {
        case Mvp::Model::Chat:
            chatConversation(this, conversationId);
            break;
        case Mvp::Model::IncomingCall:
        case Mvp::Model::OutgoingCall:
        case Mvp::Model::MissedCall:
        case Mvp::Model::ConferenceIn:
        case Mvp::Model::ConferenceOut:
        case Mvp::Model::MissedConference:
            callConversation(this, conversationId);
		default:
            break;
    }
}

void QtHistoryView::onMarkReadButtonClicked()
{
    qDebug() << "markReadButton clicked.";
    Poco::EventArgs args;
    markHistoryRead(this, args);
}

void QtHistoryView::setHistory(
    std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> > history,
    bool unreadItems)
{
    qDebug() << "View: setting history view history with " << history.size()
        << " entries.";

    m_model->setHistory(history);
    QMetaObject::invokeMethod(this, "reSort");

    QMetaObject::invokeMethod(ui->markReadButton, "setEnabled",
        Qt::QueuedConnection, Q_ARG(bool, unreadItems));
}

void QtHistoryView::reSort()
{
    // Use descending order when sorting by date.
    if (ui->sortComboBox->currentIndex() == 1)
        m_sortModel->sort(0, Qt::DescendingOrder);
    // Otherwise sort in ascending order.
    else
        m_sortModel->sort(0, Qt::AscendingOrder);
}

void QtHistoryView::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent caled";
    Poco::EventArgs args;
    viewClosed(this, args);
}

};

};

