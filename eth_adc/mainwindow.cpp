#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 40);
    connect(udpSocket, SIGNAL(readyRead()), SLOT(read()));
}


void MainWindow::read()
{
    QByteArray Datagram;
    QFile datafile("data.bin");

    do {
        Datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(Datagram.data(), Datagram.size());
    } while(udpSocket->hasPendingDatagrams());

    QDataStream in(&datafile);
    in << Datagram;
}
