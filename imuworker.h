#ifndef IMUWORKER_H
#define IMUWORKER_H

#include <QObject>
#include <QFile>

class imuWorker : public QObject
{
    Q_OBJECT
    QFile logFile;
    // variables
    QString ax= "a",ay= "a",az= "a",magx= "a",magy= "a",magz= "a",gx= "a",gy= "a",gz = "a";
    // variables
public:
    explicit imuWorker(QObject *parent = 0);

signals:

public slots:
    void processData(QString imuText);
    void saveData(QString state);
};

#endif // IMUWORKER_H
