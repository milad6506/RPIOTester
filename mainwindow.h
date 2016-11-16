#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_i2tcheck_clicked();
    void showIMUData();
    void separateYPR(QString idata);

    void on_IMUSPIRadio_clicked();

    void on_sentence_clicked();

    void on_sentenceIn_textEdited(const QString &arg1);


    void on_filename_textEdited(const QString &arg1);

    void on_saveCheck_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QSerialPort* IMU;
    bool IMUButton;
    QString d;
    int count;
    QByteArray imudata;
    QList <QString> IMUResults;
    QString defcom;
    bool saveCheckbox = false;
    QFile logFile;
    // variables
    QString ax= "a",ay= "a",az= "a",magx= "a",magy= "a",magz= "a",gx= "a",gy= "a",gz = "a";

    // variables

};

#endif // MAINWINDOW_H
