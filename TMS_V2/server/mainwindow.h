#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTcpServer"
#include "qmap.h"
#include "map"
#include "QFile"
#include <QDataStream>
#include <memory>

using namespace std;


namespace Ui {
class MainWindow;
}

#ifndef nullptr
#define nullptr 0
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void StartConnection();
    void NewGetConnection();
    void ErrorInClinet(QAbstractSocket::SocketError);
    void displayError(QAbstractSocket::SocketError socketError);
    void ReadData();
    void ClearBrowser();
    void onDisconnect();



private slots:
    void on_startServer_clicked();

    void on_SendTo_clicked();

    void on_Clear_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    bool m_hasGetOneConnection;
    QString Greading;
    QByteArray buffer;
    QTcpSocket* client;
    //packet PacketData;
    map<QString, QTcpSocket *>	clients;
    vector<QString>	allConnectedIP;
    //FilePacket  m_packetData;
    FilePacket *m_packetData;
    QString DataFile;
    QString  m_FileForSend;

};

#endif // MAINWINDOW_H
