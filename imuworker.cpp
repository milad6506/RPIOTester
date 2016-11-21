#include "imuworker.h"
#include <iostream>
#include <QTime>
#include <QDebug>

using namespace std;

int imuWorker::minHistory()
{
    QList <int> sizes;
    sizes.append(axh.size());
    sizes.append(ayh.size());
    sizes.append(azh.size());
    sizes.append(gxh.size());
    sizes.append(gyh.size());
    sizes.append(gzh.size());
    sizes.append(magxh.size());
    sizes.append(magyh.size());
    sizes.append(magzh.size());
    int minh = sizes[0];
    for (int i=1;i< sizes.size();i++ ){
        if (sizes[i] < minh){
            minh = sizes[i];
        }
    }
    return minh;
}

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
        if (angles.contains("#A-R")){
            int ai = angles.indexOf("#A-R");
            axh.append(angles.at(ai+1));
            ayh.append(angles.at(ai+2));
            azh.append(angles.at(ai+3));
        }
        if (angles.contains("#G-R")){
            int gi = angles.indexOf("#G-R");
            gxh.append(angles.at(gi+1));
            gyh.append(angles.at(gi+2));
            gzh.append(angles.at(gi+3));
        }

        if (angles.contains("#M-R")){
            int mi = angles.indexOf("#M-R");
            magxh.append(angles.at(mi+1));
            magyh.append(angles.at(mi+2));
            magzh.append(angles.at(mi+3));
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
        int minCount = minHistory();
        for (int i=0;i<minCount;i++){
            QString line = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9 \r\n").arg(axh[i]).arg(ayh[i]).arg(azh[i]).arg(gxh[i]).arg(gyh[i]).arg(gzh[i]).arg(magxh[i]).arg(magyh[i]).arg(magzh[i]);
            logFile.write(QByteArray::fromStdString(line.toStdString()));
        }


    }else{
        QString lastLine  = QString("log ended at %2 (h) %3 (m) %4 (sec) %5 (msec) \r\n").arg(QTime::currentTime().hour()).arg(QTime::currentTime().minute()).arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
        logFile.write(QByteArray::fromStdString(lastLine.toStdString()));
        logFile.close();
    }
}
