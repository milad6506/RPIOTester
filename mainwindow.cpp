#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <iostream>
#include <QSerialPort>
#include <QString>
#include <QStringList>
#include <QList>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->IMUOutText->setFontPointSize(5);
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
    ui->sentence->setDisabled(true);
    defcom = "#YPR";
}

MainWindow::~MainWindow()
{
    delete ui;
    IMU->close();
    delete IMU;
}

void MainWindow::on_i2tcheck_clicked()
{

    if (IMUButton == false){
        cout << IMU->open(QIODevice::ReadWrite) << "port openning state" << endl;
        IMUButton = true;
    }else{
        IMU->close();
        IMUButton = false;
    }


}

void MainWindow::showIMUData()
{


    while (!IMU->atEnd()){
        imudata.append(IMU->readAll());
    }


    QByteArray dataEnd = "\r\n";
    if (imudata.contains(dataEnd)){
        separateYPR(QString::fromStdString(imudata.toStdString()));
    }



}

void MainWindow::separateYPR(QString idata)
{

    count ++;
    imudata.clear();

    QRegExp rx("[, = \r\n]");
    QStringList angles= idata.split(rx,QString::SkipEmptyParts);
    //QString toShow = QString(" yaw is %1 , pitch is %2 , roll is %3").arg(angles.at(1)).arg(angles.at(2)).arg(angles.at(3));
    QString toShow = idata;
    cout << idata.toStdString() << " data" << endl;

    if (IMUResults.size() >= 6){
        IMUResults.removeAt(0);

    }
    IMUResults.append(toShow);
    QString IMUTTLString;
    for (int i=0;i<IMUResults.size();i++){
        IMUTTLString.append(IMUResults[i]);
        IMUTTLString.append("\n");
    }

    ui->IMUOutText->setPlainText(toShow);



}



void MainWindow::on_IMUSPIRadio_clicked()
{
    // SPI Nshould be powered on


}

void MainWindow::on_sentence_clicked()
{
    QString comm =  ui->sentenceIn->text();

    QByteArray command = "#";

    command.append(comm);
    if (comm.size()==0){
        command = QByteArray::fromStdString(defcom.toStdString());
    }
    while(IMU->canReadLine()){
    }
    IMU->write(command);
}

void MainWindow::on_sentenceIn_textEdited(const QString &arg1)
{
    ui->sentence->setEnabled(true);
}
