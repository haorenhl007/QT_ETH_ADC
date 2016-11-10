#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stateProc=false;
    udpSocket = new QUdpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(stateProc==true)
    {
        datafile.flush();
        datafile.close();
    }
}

void MainWindow::on_pushButton_clicked()
{

    if(stateProc==false)
    {
        datafile.setFileName("data.bin");
        datafile.open(QFile::Append);
        udpSocket->bind(QHostAddress::Any, 1000);
        connect(udpSocket, SIGNAL(readyRead()), SLOT(read()));
        stateProc=true;
        ui->pushButton->setText("Stop");//stop
    }
    else
    {
        disconnect(udpSocket, SIGNAL(readyRead()),this, SLOT(read()));
        datafile.flush();
        datafile.close();
        stateProc=false;
        ui->pushButton->setText("Start");
    }
}


void MainWindow::read()
{
    QByteArray Datagram;
    QDataStream in(&datafile);

    do
    {
        Datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(Datagram.data(), Datagram.size());
        in << Datagram;
    }
    while(udpSocket->hasPendingDatagrams());
}
