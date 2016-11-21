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
#include <QFile>
#include <QTime>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->IMUOutText->setFontPointSize(10);
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
    ui->saveCheck->setDisabled(true);
    imuProcessingThread = new QThread;
    imuProcessor = new imuWorker;
    connect(imuProcessingThread,SIGNAL(finished()),imuProcessor,SLOT(deleteLater()));
    connect(this,SIGNAL(dataReady(QString)),imuProcessor,SLOT(processData(QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(savingState(QString)),imuProcessor,SLOT(saveData(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    IMU->close();
    delete IMU;
    imuProcessingThread->quit();


}

void MainWindow::on_i2tcheck_clicked()
{

    if (IMUButton == false){

        cout << IMU->open(QIODevice::ReadWrite) << "port openning state" << endl;
        IMUButton = true;
        imuProcessingThread->start();


    }else{
        while (IMU->canReadLine()) {
        }
        IMU->close();
        IMUButton = false;

    }


}

void MainWindow::showIMUData()
{


    while (IMU->canReadLine()){
        imudata.append(IMU->readLine());
    }


    //QByteArray dataEnd = "\r\n";
    //if (imudata.contains(dataEnd)){
        count++;
        ui->messageNumber->display(count);
        ui->IMUOutText->setPlainText(QString::fromStdString(imudata.toStdString()));
        QString tbs = QString::fromStdString(imudata.toStdString());
        imudata.clear();
        emit dataReady(tbs);


    //}



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


void MainWindow::on_filename_textEdited(const QString &arg1)
{
    ui->saveCheck->setEnabled(true);
}

void MainWindow::on_saveCheck_clicked(bool checked)
{
    if (checked){
        emit savingState(ui->filename->text());
    }else{
        emit savingState("false");
    }

}


