#include "qtdialpadwidget.h"

#include "ui_qt-dialpad.h"

#include <QSignalMapper>
#include <QGridLayout>
#include <QPushButton>

namespace Mvp
{

namespace View
{

QtDialpadWidget::QtDialpadWidget(QWidget *parent)
    : QWidget(parent),
    m_buttonMapper(new QSignalMapper(this)),
    ui(new Ui_DialpadWidget)
{
    ui->setupUi(this);

    // Get the layout containing the buttons.
    QGridLayout *grid = ui->gridLayout;
    unsigned int gridCount = grid->count();

    // Add all buttons to a signal map.
    for (unsigned int i = 0; i < gridCount; ++i) {
        QPushButton *button = qobject_cast<QPushButton*>(
                    grid->itemAt(i)->widget());

        // Connect the click event to a common map slot.
        connect(button, SIGNAL(clicked()), m_buttonMapper, SLOT(map()));
        m_buttonMapper->setMapping(button, button->text());
    }

    // Connect the signal map to our common signal.
    connect(m_buttonMapper, SIGNAL(mapped(const QString &)),
            this, SIGNAL(buttonClicked(const QString &)));
}

QtDialpadWidget::~QtDialpadWidget()
{
    // Clean up.
    delete m_buttonMapper;
    m_buttonMapper = 0;

    delete ui;
    ui = 0;
}

};

};
