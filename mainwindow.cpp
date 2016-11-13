#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <iostream>
#include <QSerialPort>
#include <QString>
#include <QStringList>
#include <QList>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QSerialPortInfo> available = QSerialPortInfo::availablePorts();
    for (int i = 0; i < available.count(); i++) {
        cout << (available.at(i).portName() == "ttyAMA0") << "port is found?" << endl;

    }
    // setting up IMU for ttl
    IMU = new QSerialPort;
    IMU->setPortName("ttyAMA0");
    IMU->setBaudRate(QSerialPort::Baud57600);
    IMU->setParity(QSerialPort::NoParity);
    IMU->setDataBits(QSerialPort::Data8);
    IMU->setStopBits(QSerialPort::OneStop);
    IMU->setFlowControl(QSerialPort::NoFlowControl);
    connect(IMU,SIGNAL(readyRead()),this,SLOT(showIMUData()));
    IMUButton = false;
    count = 0;

    connect(ui->actionClose,SIGNAL(triggered(bool)),this,SLOT(close()));


}

MainWindow::~MainWindow()
{
    delete ui;
    IMU->close();
}

void MainWindow::on_i2tcheck_clicked()
{

    if (IMUButton == false){
        cout << IMU->open(QIODevice::ReadWrite) << "port openning state" << endl;
        while(IMU->canReadLine()){
            cout << IMU->readLine().toStdString() << endl;
        }
       // QByteArray command = QByteArray::fromStdString(QString("#osct").toStdString());
        // IMU->write(command);

        IMUButton = true;
    }else{

        IMU->close();
        IMUButton = false;
    }


}

void MainWindow::showIMUData()
{

/*
    while (!IMU->atEnd()){
        imudata.append(IMU->readAll());
    }*/
    cout << " data recieved"<< endl;
    //ui->IMUOutText->setPlainText(QString::fromStdString(imudata.toStdString()));
    /*
    QByteArray dataEnd = "\r\n";
    if (imudata.contains(dataEnd)){
        separateYPR(QString::fromStdString(imudata.toStdString()));
    }*/



}

void MainWindow::separateYPR(QString idata)
{
    /*
    count ++;
    imudata.clear();

    QRegExp rx("[, = \r\n]");
    QStringList angles= idata.split(rx,QString::SkipEmptyParts);
    QString toShow = QString(" yaw is %1 , pitch is %2 , roll is %3").arg(angles.at(1)).arg(angles.at(2)).arg(angles.at(3));
    cout << angles.size() << " size of input" << endl;

    if (IMUResults.size() >= 6){
        IMUResults.removeAt(0);

    }
    IMUResults.append(toShow);
    QString IMUTTLString;
    for (int i=0;i<IMUResults.size();i++){
        IMUTTLString.append(IMUResults[i]);
        IMUTTLString.append("\n");
    }

    ui->IMUOutText->setPlainText(toShow);*/



}



void MainWindow::on_IMUSPIRadio_clicked()
{
    // SPI Nshould be powered on

}
