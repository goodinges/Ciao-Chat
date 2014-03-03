#include <QtGui>

#include "qtsimpleclientapp.h"

#include "model/skypecore.h"
#include "presenter/state/statemachine.h"
#include "view/qtviewfactory.h"
#include "view/skypeimages.h"

// Assuming that the skypekit runtime is somewhere in the system path.
const char *SkypeRuntime = "skypekit-uikit-runtime";

// Settings key constants.  Left at this scope for now.
const char *AppTokenFileKey = "apptoken";
const char *DataPathKey = "datapath";

QtSimpleClientApplication::QtSimpleClientApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
	// set application parameters related to use of QSettings
	setOrganizationName("Skype");
	setApplicationName("SkypeUIKit");
	setOrganizationDomain("skype.net");
	setApplicationVersion("0.0.1.alpha");
	
    Q_INIT_RESOURCE(skypeicons);
	
    setWindowIcon(QIcon(Mvp::View::SkypeImage::SkypeBlueIcon));

    // Using QSettings for its platform independence, but not allowing
    // any settings to persists accross sessions for now.
	QSettings s;
    s.clear();

	// parse command line arguments and store in QSettings
	readCommandLineArguments();
	
	if ( ! s.contains(AppTokenFileKey) ) {
		// if no token was provided in prior seesions or on the command line,
		// look for a pem file in the executable directory
		findCertificateFile();
	}

    // Let's listen to skype runtime process signals.
    connect(&m_runtime, SIGNAL(started()), this, SLOT(onStarted()));
    connect(&m_runtime, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(onError(QProcess::ProcessError)));
	
    // Start skype runtime process up.
    QString rte = QCoreApplication::applicationDirPath() + "/" + SkypeRuntime;
    QStringList args;

    args << "-x"; // no netlogger, reduces runtime startup time

    if (s.contains(DataPathKey))
    {
        args << "-f" <<  s.value(DataPathKey).toString();
    }
    qDebug() << "Starting runtime:" << rte << args;
    m_runtime.start(rte, args);	
}

QtSimpleClientApplication::~QtSimpleClientApplication()
{
    // If runtime still running, kill it!
    if (m_runtime.state() == QProcess::Running) {
        qDebug() << "Terminating the skype runtime";
        m_runtime.terminate();
    }
}

void QtSimpleClientApplication::readCommandLineArguments()
{
	// process all command line options
	QSettings s;
	QStringList args = arguments();
	for (int i = 1; i < args.size(); i++)
	{
		if (args[i] == "-t" || args[i] == "\t")
		{
			// app token text
			QString pemfile = args[++i];
			qDebug() << "applicaiton token certificate file:" << pemfile;
			QFileInfo f(pemfile);
			if (f.exists())
			{
				s.setValue(AppTokenFileKey, f.absoluteFilePath());
			}
			else
			{
				qWarning() << "Invalid token file specified on command line:" << pemfile;
			}

		}
		else if (args[i] == "-f" || args[i] == "/f")
		{
			// data path
			QString datapath = args[++i];
			qDebug() << "setting data path:" << datapath;
			s.setValue(DataPathKey, datapath);
		}
		else if (args[i] == "-p" || args[i] == "/p")
		{
			// port number
			bool ok;
			int port = args[++i].toInt(&ok);
			if (ok) 
			{
				qDebug() << "setting port:" << port;
				s.setValue("port", port);
			}
			else
			{
				qWarning() << "Unable to read port number argument:" << args[i];
			}
		}
		
	}
}

void QtSimpleClientApplication::findCertificateFile()
{
	// look for .pem certificate files in the executable directory
	QString exe_path = QCoreApplication::applicationDirPath();
	QFileInfoList pems = QDir(exe_path).entryInfoList(QStringList("*.pem"));
	
	qDebug() << "PEM files found:" << pems.size();
	if (pems.isEmpty())
		return;
	
	QFileInfo p = pems.first();
	qDebug() << "using PEM file:" << p.absoluteFilePath();
	QSettings s;
	s.setValue(AppTokenFileKey, p.absoluteFilePath());
	
}

void QtSimpleClientApplication::onStarted()
{
    qDebug() << "Initialize Skype now that runtime has launched";

	QSettings s;
	if (s.contains(AppTokenFileKey))
	{
		// Initialize SkypeCore with app token
		QString tokenFile = s.value(AppTokenFileKey).toString();
		qDebug() << "Reading apptoken file:" << tokenFile;
		QFile pem(tokenFile);
		pem.open(QIODevice::ReadOnly | QIODevice::Text);
		QByteArray pembytes = pem.readAll();
		qDebug() << "...read" << pembytes.size() << "bytes";
		Mvp::Model::SkypeCore::initializeInstance(pembytes.data());
	}
	else
	{
		qFatal("No application token found.");
	}

    qDebug() << "Initializing the user interface";

    // Allocate the Qt view factory, so the states can use it to create views.
    Mvp::View::QtViewFactory::instance();
    m_stateMachine = new Mvp::Presenter::StateMachine;
}

void QtSimpleClientApplication::onError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart) {
        qDebug() << "Error initializing the Skype Runtime.";

        /* If an error occur when initializing the runtime we have to close
         * our application.
         */
        quit();
    }
}

int QtSimpleClientApplication::exec()
{
    return QApplication::exec();
}
