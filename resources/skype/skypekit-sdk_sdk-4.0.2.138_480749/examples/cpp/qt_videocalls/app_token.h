
#include <QFile>

QString keyFileName = "../qt_videocalls/tkclient.pem";

bool getKeyPair (char*& keyBuf)
{
    QFile keyFile(keyFileName);
    if (keyFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        uint        keyLen;
        QByteArray  qBuff;
        qBuff       = keyFile.readAll();
        keyLen      = qBuff.size();
        keyBuf      = new char[keyLen + 1];

        memcpy(keyBuf, qBuff.data(), keyLen);
        keyBuf[keyLen] = 0;
        keyFile.close();
        return true;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("ERROR: Unable to open " + keyFileName);
        msgBox.exec();
        return false;
    };
};
