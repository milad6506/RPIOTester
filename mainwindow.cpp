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
        while (IMU->canReadLine()) {
        }
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
    ui->messageNumber->display(count);
    mLog.append(idata);
    ui->IMUOutText->setPlainText(idata);

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
    saveCheckbox = checked;
    if (checked == true){
        logFile.setFileName(ui->filename->text());
        qDebug()<< logFile.open(QFile::ReadWrite);
        QString firstLine = QString("log data name = %1 started at %2 (h) %3 (m) %4 (sec) %5(msec) \r\n").arg(ui->filename->text()).arg(QTime::currentTime().hour()).arg(QTime::currentTime().minute()).arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
        logFile.write(QByteArray::fromStdString(firstLine.toStdString()));
        QRegExp rx("[, = \r\n]");
        QString t = "a";

        for (int i=0;i<mLog.size();i++){

            QStringList angles= mLog[i].split(rx,QString::SkipEmptyParts);
            if (angles.contains("#A-R")){
                int a = angles.indexOf("#A-R");
                ax = angles.at(a+1);
                ay = angles.at(a+2);
                az = angles.at(a+3);
            }
            if (angles.contains("#G-R")){
                int g = angles.indexOf("#G-R");
                gx = angles.at(g+1);
                gy = angles.at(g+2);
                gz = angles.at(g+3);
            }
            if (angles.contains("#M-R")){
                int m = angles.indexOf("#M-R");
                magx = angles.at(m+1);
                magy = angles.at(m+2);
                magz = angles.at(m+3);
            }
            /*
            if (angles.at(0) == "#A-R"){
                ax = angles.at(1);
                ay = angles.at(2);
                az = angles.at(3);
            }else if (angles.at(0) == "#G-R"){
                gx = angles.at(1);
                gy = angles.at(2);
                gz = angles.at(3);
            }else if (angles.at(0) == "#M-R"){
                magx = angles.at(1);
                magy = angles.at(2);
                magz = angles.at(3);
            }*/

            if((ax != t)&&(ay != t)&&(az != t)&&(magx != t)&&(magy != t)&&(magz != t)&&(gx != t)&&(gy != t)&&(gz != t)){
                QString line = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9 \r\n").arg(ax).arg(ay).arg(az).arg(magx).arg(magy).arg(magz).arg(gx).arg(gy).arg(gz);
                logFile.write(QByteArray::fromStdString(line.toStdString()));
                cout << line.toStdString() << endl;
                ax = "a";ay = "a";az = "a";gx = "a";gy = "a";gz = "a";magx = "a";magy = "a";magz = "a";
            }

        }
        ui->IMUOutText->setPlainText("saving finished");

    }else{
        QString lastLine  = QString("log data name = %1 ended at %2 (h) %3 (m) %4 (sec) %5 (msec) \r\n").arg(ui->filename->text()).arg(QTime::currentTime().hour()).arg(QTime::currentTime().minute()).arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
        logFile.write(QByteArray::fromStdString(lastLine.toStdString()));
        logFile.close();
        mLog.clear();
    }


}


void MainWindow::on_checkersen_textEdited(const QString &arg1)
{
    ui->IMUOutText->setPlainText(mLog[ arg1.toInt()]);
}
