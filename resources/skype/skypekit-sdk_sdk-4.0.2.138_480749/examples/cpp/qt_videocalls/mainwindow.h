#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QSKClasses.h>

#ifdef WIN32
    #include <ssl.h>
#endif

#include "videowidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void prefillLogin (QString accName, QString accPsw);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

//---------------------------------------------------------------------------
// Custom methods and slots

public:
    VideoWidget* previewVideoWindow;
    VideoWidget* incomingVideoWindow;


    void log (QString S);

    void SendPlaceCallSignal (QString target) { emit PlaceCallSignal(target); }

public slots:
    void on_LogMessage (QString S);
    void on_UpdateContactList ();
    void on_EnableLoginBtn (bool B);
    void on_EnableCallBtn (bool B);
    void on_callButtonDropsCall (bool B);

    void on_OpenIncomingVideoWindow(Participant::Ref part);
    void on_CloseIncomingVideoWindow(Participant::Ref part);


private slots:    
    void on_loginButton_clicked();
    void on_previewButton_clicked();
    void on_callButton_clicked();    
    void on_contactList_doubleClicked(QModelIndex index);

signals:
    void PlaceCallSignal (QString);
};

extern MainWindow*          mainForm;

#endif // MAINWINDOW_H

