#include "imuworker.h"
#include <iostream>

using namespace std;

imuWorker::imuWorker(QObject *parent) : QObject(parent)
{

}

void imuWorker::processData(QString imuText)
{
    QRegExp rx("[, = \r\n]");
    QString t = "a";
    QStringList angles= imuText.split(rx,QString::SkipEmptyParts);
    for (int i =0;i<angles.size();i++){

        cout << angles[i].toStdString() << "***";

    }
    cout << "" << angles.size() << endl;



}

void imuWorker::saveData(QString state)
{
    bool checked;
    /*
    if (checked == true){
        logFile.setFileName(ui->filename->text());
        qDebug()<< logFile.open(QFile::ReadWrite);
        QString firstLine = QString("log data name = %1 started at %2 (h) %3 (m) %4 (sec) %5(msec) \r\n").arg(ui->filename->text()).arg(QTime::currentTime().hour()).arg(QTime::currentTime().minute()).arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
        logFile.write(QByteArray::fromStdString(firstLine.toStdString()));


    }else{
        QString lastLine  = QString("log data name = %1 ended at %2 (h) %3 (m) %4 (sec) %5 (msec) \r\n").arg(ui->filename->text()).arg(QTime::currentTime().hour()).arg(QTime::currentTime().minute()).arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
        logFile.write(QByteArray::fromStdString(lastLine.toStdString()));
        logFile.close();
        mLog.clear();
    }*/
}
