#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Handler.h"

#include <NetworkProcess.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //qRegisterMetaType<PacketData>();
    ui->setupUi(this);
    connect(this, SIGNAL(readyPacket(PacketData)), this, SLOT(onGetNewPacket(PacketData)));

    connect(ui->connect, SIGNAL(clicked()), this, SLOT(onStart()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(onClearLogBrowser()));
    connect(ui->send, SIGNAL(clicked()), this, SLOT(onSendDataToServer()));

    //connect(m_networkProcess->get(), SIGNAL(detectedPacket(QVariant)), m_handler, SLOT(onGetPacket(QVariant)));

    //m_networkProcess.reset(new NetworkProcess(this), [](NetworkProcess* p) { Q_UNUSED(p) });
    m_handler = new Handler();
    m_hiWord = "Hi...";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addLog(QString log, LogType logType)
{
    QString logTypeString="";
    switch(logType)
    {
        case LogType::Data:
        {
            logTypeString="Data:";
        }
        break;
        case LogType::Info:
        {
            logTypeString="Info:";
        }
        break;
        case LogType::Warning:
        {
            logTypeString="Warning:";
        }
        break;
        case LogType::Error:
        {
            logTypeString="Error:";
        }
        break;
    }

    m_bufferData += logTypeString+log + "\n";
    ui->LogBrowser->setText(m_bufferData);
}

void MainWindow::onStart()
{
    m_handler->startUpdate(ui->IP->text(), ui->Port->value());
}
/*
void MainWindow::onConnected()
{
    qDebug() << "Connected.";
}

void MainWindow::onDisconnected()
{
    qDebug() << "Disconnected.";
}

void MainWindow::onErrorInConnection(QAbstractSocket::SocketError error)
{
    qDebug() << error;
}


void MainWindow::onReadData()
{
    m_networkProcess->onProcessData(m_clientTcp->readAll());
}

void MainWindow::onSendDataToServer()
{
    m_clientTcp->write(ui->dataToServer->toPlainText().toLatin1());
}
*/
void MainWindow::onClearLogBrowser()
{
    m_bufferData = "";
    ui->LogBrowser->setText(m_bufferData);
}
void MainWindow::onClear() {}

/*
void MainWindow::onGetNewPacket(PacketData packet)
{
    qDebug() << "Get Packet";

    switch (packet.type) {
    case PacketType::Data:
        qDebug() << "Get Data Packet type";
        break;
    case PacketType::File:
    {
        qDebug() << "Get File Packet type";
        if(packet.totalFileBytesSize<packet.fileBytesSize)
            packet.fileBytes[packet.totalFileBytesSize]=0;
        else
            packet.fileBytes[packet.fileBytesSize]=0;
        QString data=QString::fromLatin1((char*)packet.fileBytes);
        qDebug()<<"File Data in length:"<< packet.fileBytesSize<<"\n"<<data;
    }
        break;
    case PacketType::Command:
        qDebug() << "Get Command Packet type";
        break;
    default:
        break;
    }
}
*/
