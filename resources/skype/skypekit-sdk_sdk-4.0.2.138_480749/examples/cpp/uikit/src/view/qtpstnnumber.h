#ifndef _QTPSTNNUMBER_H
#define _QTPSTNNUMBER_H

#include <QDialog>

class Ui_PSTNNumber;

namespace Mvp
{

namespace View
{

/**
 * @class QtPSTNNumber qtpstnnumber.h qtpstnnumber.h
 * @brief Provides a way to start a call or send a SMS to a specific PSTN
 * number.
 *
 * This class is a dialog that is meant to provide a way to start a call or
 * send a SMS to a specific PSTN number. There're two ways to enter the target
 * PSTN number: 1) Using a regular physical keyboard. 2) Using the virtual
 * dialpad keyboard provided by the widget.
 */
class QtPSTNNumber: public QDialog
{
    Q_OBJECT

public:
    enum Action {
        None = 0,
        Call,
        SMS
    };

    /**
     * Constructor.
     *
     * @param parent A pointer to the object's parent window if any.
     */
    explicit QtPSTNNumber(QWidget *parent = 0);

    /**
      * Destructor.
      */
    ~QtPSTNNumber();

    /**
     * Returns the type of Action that was requested.
     *
     * There're two possible actions to be requested: 1) Call Phone. 2) Send
     * SMS. If it returns None it means that no request was set.
     *
     * @return The requested action.
     */
    Action action() const;

    /**
     * Returns the informed PSTN number.
     *
     * @return The PSTN number to be used for a call or SMS.
     */
    QString number() const;

private slots:
    void onCallClicked();
    void onSmsClicked();
    void onNumberChanged(const QString&);
    void onDialpadClicked(const QString&);

private:
    Q_DISABLE_COPY(QtPSTNNumber)

    void enableControls(bool enabled = false);

    Action m_action;
    QString m_number;

    /**
     * A pointer to the ui from qt-pstnnumber.ui's generated source code.
     */
    Ui_PSTNNumber *ui;
};

};

};

#endif // _QTPSTNNUMBER_H
