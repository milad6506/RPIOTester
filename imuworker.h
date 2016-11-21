#ifndef IMUWORKER_H
#define IMUWORKER_H

#include <QObject>

class imuWorker : public QObject
{
    Q_OBJECT
public:
    explicit imuWorker(QObject *parent = 0);

signals:

public slots:
    void processData(QString imuText);
};

#endif // IMUWORKER_H
