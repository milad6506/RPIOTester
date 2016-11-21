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

        cout << "angles are " << angles[i].toStdString() << "***";

    }
    cout << "" << endl;



}
