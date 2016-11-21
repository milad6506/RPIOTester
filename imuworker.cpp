#include "imuworker.h"
#include <iostream>
#include <QTime>
#include <QDebug>

using namespace std;

imuWorker::imuWorker(QObject *parent) : QObject(parent)
{

}

void imuWorker::processData(QString imuText)
{
    QRegExp rx("[, = \r\n]");
    QString t = "a";
    QStringList angles= imuText.split(rx,QString::SkipEmptyParts);
    if (!angles.contains("#YPR")){
        cout << angles.size() << endl;

        /*
        axh.append(angles.at(ai+1));
        ayh.append(angles.at(ai+2));
        azh.append(angles.at(ai+3));
        gxh.append(angles.at(gi+1));
        gyh.append(angles.at(gi+2));
        gzh.append(angles.at(gi+3));
        magxh.append(angles.at(mi+1));
        magyh.append(angles.at(mi+2));
        magzh.append(angles.at(mi+3));*/
        if (angles.contains("#A-R")){
            int ai = angles.indexOf("#A-R");
            cout << "ax" << angles.at(ai+1);
            cout << "ay" << angles.at(ai+2);
            cout << "az" << angles.at(ai+3);
        }
        if (angles.contains("#G-R")){
            int gi = angles.indexOf("#G-R");
            cout << "gx" << angles.at(gi+1);
            cout << "gy" << angles.at(gi+2);
            cout << "gz" << angles.at(gi+3);
        }

        if (angles.contains("#M-R")){
            int mi = angles.indexOf("#M-R");
            cout << "magx" << angles.at(mi+1);
            cout << "magy" << angles.at(mi+2);
            cout << "magz" << angles.at(mi+3);
        }


    }





}

void imuWorker::saveData(QString state)
{

    if (!state.contains("false")){
        logFile.setFileName(state);
        logFile.open(QFile::ReadWrite);
        QString firstLine = QString("log data name = %1 started at %2 (h) %3 (m) %4 (sec) %5(msec) \r\n").arg(state).arg(QTime::currentTime().hour()).arg(QTime::currentTime().minute()).arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
        logFile.write(QByteArray::fromStdString(firstLine.toStdString()));
        for (int i=0;i<axh.size();i++){
            QString line = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9 \r\n").arg(axh[i]).arg(ayh[i]).arg(azh[i]).arg(gxh[i]).arg(gyh[i]).arg(gzh[i]).arg(magxh[i]).arg(magyh[i]).arg(magzh[i]);
            logFile.write(QByteArray::fromStdString(line.toStdString()));
        }


    }else{
        QString lastLine  = QString("log ended at %2 (h) %3 (m) %4 (sec) %5 (msec) \r\n").arg(QTime::currentTime().hour()).arg(QTime::currentTime().minute()).arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
        logFile.write(QByteArray::fromStdString(lastLine.toStdString()));
        logFile.close();
    }
}
