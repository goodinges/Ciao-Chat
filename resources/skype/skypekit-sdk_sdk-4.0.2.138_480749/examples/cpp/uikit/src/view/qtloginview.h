#ifndef QtLoginView_H
#define QtLoginView_H

#include <QDialog>
#include "abstractloginview.h"

class QCloseEvent;

class Ui_LoginView;

namespace Mvp
{

namespace View
{

/**
 * @class QtLoginView qtloginview.h qtloginview.h
 * @brief A view used for logging in to the Skype Network.
 *
 * This class is a view with a place to enter a skypename and a password.  It
 * also has an Ok button and a cancel button, and a label for showing Account
 * Status (Logged In, Logged Out, etc.).
 *
 * @see AbstractLoginView
 */
class QtLoginView : public QDialog, public AbstractLoginView
{
Q_OBJECT
public:
    /**
     * Constructor.
     */
    QtLoginView();

    /**
     * Destructor.
     */
    ~QtLoginView();

    /**
     * Accessor for the username from the username lineedit.
     *
     * @return A std::string of the data the user entered into the Username
     *   QLineEdit in the user interface.
     */
    std::string username();

    /**
     * Accessor for the password from the lineedit.
     *
     * @return A std::string of the data that the user entered into the
     * password QLineEdit in the user interface.
     */
    std::string password();

    /**
     * Overridden from the base class to show the given status in the
     * QLabel beneath the password field.
     */
    void showStatus(AbstractLoginView::LoginStatus);

    /**
     * Show the given error code in the user interface.
     *
     * Subclasses should implement this to show the given error code somehow
     * in the interface, within the guidelines of the skypekit certification
     * documents.
     *
     * @param code the error code to show.
     */
    void setLoginError(LoginError code);

    /**
     * Initialize the dialog.
     */
    void initialize();

    /**
     * Ask the view to display.
     *
     * This method just calls QDialog::show().
     */
    void showView();

    /**
     * Ask the view to hide.
     *
     * This method just calls QDialog::hide().
     */
    void hideView();

    /**
     * Destroy the view object.
     *
     * In Qt views the only thing needed in destroyView() is a call to deleteLater() so the
     * widget will get deleted from the gui thread.
     *
     * @see AbstractView::destroyView()
     */
    void destroyView();

public slots:
    /**
     * Overridden from QDialog class to perform login when enter is pressed.
     */
    void accept();

protected:

    void closeEvent(QCloseEvent *event);

private:
    Q_DISABLE_COPY(QtLoginView)

    /**
     * A QDialog object loaded from qt-loginview.ui's generated code.
     */
    Ui_LoginView *ui;
};

};

};
#endif
