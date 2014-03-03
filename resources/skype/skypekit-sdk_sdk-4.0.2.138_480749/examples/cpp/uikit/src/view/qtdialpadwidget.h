#ifndef _QTDIALPADWIDGET_H
#define _QTDIALPADWIDGET_H

#include <QWidget>

class QSignalMapper;
class Ui_DialpadWidget;

namespace Mvp
{

namespace View
{

/**
 * @class QtDialpadWidget qtdialpadwidget.h qtdialpadwidget.h
 * @brief A generic widget that displays a dialpad numeric keyboard.
 *
 * This class is a widget that is meant to display and control a numeric
 * keyboard. The widget will emit a common signal with the name of the
 * clicked key.
 */
class QtDialpadWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent A pointer to the object's parent window if any.
     */
    explicit QtDialpadWidget(QWidget *parent = 0);

    /**
      * Destructor.
      */
    ~QtDialpadWidget();

signals:
    /**
     * Signal that a key was clicked.
     *
     * @param name the name of the clicked key.
     */
    void buttonClicked(const QString &name);

private:
    Q_DISABLE_COPY(QtDialpadWidget)

    QSignalMapper *m_buttonMapper;

    /**
     * A pointer to the ui from qt-dialpad.ui's generated source code.
     */
    Ui_DialpadWidget *ui;
};

};

};

#endif // _QTDIALPADWIDGET_H
