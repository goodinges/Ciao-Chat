#include "qtpstnnumber.h"

#include "ui_qt-pstnnumber.h"

namespace Mvp
{

namespace View
{

// The minimal length that a PSTN Number can has.
const int minimalLength = 4;

QtPSTNNumber::QtPSTNNumber(QWidget *parent)
    : QDialog(parent),
    m_action(None),
    m_number(""),
    ui(new Ui_PSTNNumber)
{
    ui->setupUi(this);

    // Connect signals.
    connect(ui->editNumber, SIGNAL(textChanged(const QString&)),
            this, SLOT(onNumberChanged(const QString&)));
    connect(ui->buttonCall, SIGNAL(clicked()),
            this, SLOT(onCallClicked()));
    connect(ui->buttonSms, SIGNAL(clicked()),
            this, SLOT(onSmsClicked()));
    connect(ui->dialpadWidget, SIGNAL(buttonClicked(const QString&)),
            this, SLOT(onDialpadClicked(const QString&)));

    // With this flag the widget will automatically delete itself when closed.
    setAttribute(Qt::WA_DeleteOnClose);
}

QtPSTNNumber::~QtPSTNNumber()
{
    // Clean up.
    delete ui;
    ui = 0;
}

QtPSTNNumber::Action QtPSTNNumber::action() const
{
    return m_action;
}

QString QtPSTNNumber::number() const
{
    return m_number;
}

void QtPSTNNumber::enableControls(bool enabled)
{
    // Enable/disable controls.
    ui->buttonCall->setEnabled(enabled);
    ui->buttonSms->setEnabled(enabled);
}

void QtPSTNNumber::onCallClicked()
{
    // Assign the action and the PSTN number.
    m_action = QtPSTNNumber::Call;
    m_number = ui->labelNumber->text();

    // Tell dialog that it was accepted.
    accept();
}

void QtPSTNNumber::onSmsClicked()
{
    // Assign the action and the PSTN number.
    m_action = QtPSTNNumber::SMS;
    m_number = ui->labelNumber->text();

    // Tell dialog that it was accepted.
    accept();
}

void QtPSTNNumber::onNumberChanged(const QString &text)
{
    // Only enable the controls is the text is bigger than the minimalLength.
    enableControls((text.length() >= minimalLength));

    // Always add the country indicator in front of the number.
    if (text.length() > 0)
        ui->labelNumber->setText((text[0] == '+') ? text : "+" + text);
    else
        ui->labelNumber->setText("+");

}

void QtPSTNNumber::onDialpadClicked(const QString &name)
{
    // When a dialpad number is clicked we add it to the PSTN number field.
    ui->editNumber->setText(ui->editNumber->text() + name);
}

};

};
