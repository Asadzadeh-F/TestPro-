#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include "iostream"
using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_hasGetOneConnection = false;

    server = new QTcpServer(this);
    m_packetData = new FilePacket();

    //connect(ui->startServer, SIGNAL(clicked()), this, SLOT(StartConnection()));
    connect(server, SIGNAL(newConnection()),this, SLOT(NewGetConnection()));
    connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this,SLOT(onAcceptedError(QAbstractSocket::SocketError)));
    //connect(ui->Clear, SIGNAL(clicked()), this , SLOT(ClearBrowser()));
    //connect(ui->SendTo, SIGNAL(clicked()), this, SLOT (SendData()));
    Greading = "Hi ...";
    m_packetData->fileBytesSize=0;
    m_packetData->totalFileBytesSize=0;
    ui->SendTo->setEnabled(ui->IP->count() != 0);


   //server->setProxy(QNetworkProxy::NoProxy);

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::StartConnection()
{
    qDebug() << "\nlistening...";
    ui->LogBrowser->setText("listening...");

    if(server->listen(QHostAddress::Any,20))
    {

     qDebug() << "Start Up, The Server Has Started";

    }
    else
    {

     qDebug() << "Start Up, The Server failed to Start up";
    }


    if (server -> hasPendingConnections() )
        qDebug()<< "\nserver has Pending Connections ";

}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}

void MainWindow::NewGetConnection()
{
    qDebug() << "\nnewConnection";
    QTcpSocket* client = server-> nextPendingConnection();
    ui->LogBrowser->setText("Hi Hi...");
    connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnect()));            //connect(client, SIGNAL(Disconnected()), this, SLOT(onDisconnect()));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(ErrorInClinet(QAbstractSocket::SocketError)));
    connect(client, SIGNAL(readyRead()), this, SLOT(ReadData()));
    //client-> write(Greading.toLatin1());
    qDebug() << "\n\nHI Client...";
    QString ip = client->peerAddress().toString();
            //m_clients.insert({ip, client});
    clients.insert(pair<QString, QTcpSocket*>(ip, client));
    allConnectedIP.push_back(ip);
    //int int_ip = ip.toInt();
    ui->IP->addItem(ip);
    ui->SendTo->setEnabled(ui->IP->count() != 0);


}

void MainWindow::ReadData()
{

        qDebug() << "\n\nreading data from client...";


        //Read Buffer Data
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        buffer += sender->readAll();
        ui->LogBrowser->setText(buffer);

}

void MainWindow::ClearBrowser()
{
    qDebug() << "\nRequest for Clear.";
    buffer = "";
    ui->LogBrowser->setText(buffer);
    qDebug() << "\nClear...";
}

void MainWindow::onDisconnect()
{
     QTcpSocket* client = static_cast<QTcpSocket*>(QObject::sender());
     QString ip = client->peerAddress().toString();

    for ( int a = 0; a < ui->IP->count() + 1; a++ )
    {
     if ( ui->IP->item(a)!= nullptr && ui->IP->item(a)->text() == ip  )

         ui->IP->takeItem(a);
    }

    clients.erase(client->peerAddress().toString());
    ui->SendTo->setEnabled(ui->IP->count() != 0);
}



void MainWindow::MainWindow::ErrorInClinet(QAbstractSocket::SocketError error)
{
    qDebug() << "Client has error: " << error;
}

void MainWindow::on_startServer_clicked()
{
    qDebug() << "\nlistening...";
    ui->LogBrowser->setText("listening...");
    ui->status->setText("start");

    if(server->listen(QHostAddress::Any,20))
    {
        qDebug() << "Start Up, The Server Has Started";

    }
    else
    {
        qDebug() << "Start Up, The Server failed to Start up";
    }

    if (server -> hasPendingConnections() )
        qDebug()<< "\nserver has Pending Connections ";
}

void MainWindow::on_SendTo_clicked()
{
    uint32_t count = 0;
    //qDebug() << "farzaneh";
    QListWidgetItem *Item = ui->IP->currentItem();
    Item->setTextColor(Qt::white);
    Item->setBackgroundColor(Qt::blue);
    QString ip;
    ip = ui->IP->currentItem()->text();

    m_FileForSend = "/yaffs/CashLess/temp.dot";
    QFile file(m_FileForSend);

    if (file.exists())

        {
            qDebug() << "totalReadSize";
            qDebug(QString::number(file.size()).toLatin1());
            if (file.open(QIODevice::ReadOnly))
            {
                m_packetData->totalFileBytesSize = file.size();
                qDebug() << "totalFileBytesSize";
                qDebug(QString::number(m_packetData->totalFileBytesSize).toLatin1());

                while (!file.atEnd())
                {
                    quint32 readSize			= file.read((char*) m_packetData->fileBytes, sizeof(m_packetData->fileBytes) - 1);
                    m_packetData->fileBytesSize = readSize;
                    qDebug() << "fileBytesSize";
                    qDebug(QString::number(m_packetData->fileBytesSize).toLatin1());

                    clients[ip]->write((char*) m_packetData, sizeof(FilePacket));

                    //int * a;
                    qApp->processEvents();
                }
            }
        }

}

void MainWindow::on_Clear_clicked()
{
    qDebug() << "\nRequest for Clear.";
    buffer = "";
    ui->LogBrowser->setText(buffer);
    qDebug() << "\nClear...";
}
