
#ifndef _QTSIMPLECLIENTAPP_H
#define _QTSIMPLECLIENTAPP_H

#include <QApplication>
#include <QProcess>

namespace Mvp
{

namespace Presenter
{
class StateMachine;
};

};


class QtSimpleClientApplication: public QApplication
{
Q_OBJECT
public:
    QtSimpleClientApplication(int &argc, char **argv);
    ~QtSimpleClientApplication();

    int exec();

private slots:
    void onStarted();
    void onError(QProcess::ProcessError error);

private:
	void readCommandLineArguments();
	void findCertificateFile();
	
    QProcess m_runtime;
    Mvp::Presenter::StateMachine *m_stateMachine;
};

#endif
