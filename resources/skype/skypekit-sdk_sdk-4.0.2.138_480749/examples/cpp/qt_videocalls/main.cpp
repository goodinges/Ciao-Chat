#include <QtGui/QApplication>
#include <QtGui>
#include "mainwindow.h"
#include "skype-embedded_2.h"
#include "QSKClasses.h"
#include "app_token.h"


#ifdef WIN32
    QString runtimePath = "..//..//..//bin//windows-x86-skypekit.exe";
#endif

#ifdef __APPLE__
    QString runtimePath = "..//..//..//bin//mac-x86-skypekit";
#endif

#ifdef __linux__
    QString runtimePath = "..//..//..//bin//linux-x86-skypekit";
#endif


int main(int argc, char *argv[])
{
    QApplication    application(argc, argv);
    QMessageBox     msgBox;

    // Creating application's main window and a dispatcher object for communicating
    // between GUI and SkypeKit callbacks.

    mainForm            = new MainWindow();
    dispatcher          = new QSKSignalDispatcher();
    dispatcher->ConnectToUI();


    // Prefilling login credential input fields from command-line
    if (argc == 3) { mainForm->prefillLogin(argv[1], argv[2]); };

    // Attempting to programmatically start runtime.
    // Note that QProcess::startDetached leaves the shell terminal open on desktop.
    // For demp application this is ok - for production apps you might want go for other
    // methods of starting the runtime process. On Windows, ShellExecute will give you
    // an option to run a program with SW_HIDE switch, for example.

    QProcess runtime;
    if (!runtime.startDetached(runtimePath))
    {
        QMessageBox msgBox;
        msgBox.setText("ERROR: Unable to execute " + runtimePath);
        msgBox.exec();
    }
    else
    {
        skype = new QSKSkype();
        TransportInterface::Status skypeStatus;

        char* keyBuf = 0;
        getKeyPair (keyBuf);
        skypeStatus = skype->init(keyBuf, "127.0.0.1", 8963);

        if (skypeStatus != TransportInterface::OK)
        {
            msgBox.setText("ERROR: Could not connect to SkypeKit runtime. Are you using correct .pem file?");
            msgBox.exec();
        }
        else
        {
            skype->start();
            mainForm->on_EnableCallBtn(false);
            mainForm->show();
            application.exec();
        };

        skype->stop();
        delete skype;
    };
};
