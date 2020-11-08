#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QTcpSocket>
#include "QMainWindow"
#include "QTableWidget"
#include "Logger.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

enum class LogType
{
    Data,
    Info,
    Warning,
    Error
};

#ifndef nullptr
#define nullptr 0
#endif

class NetworkProcess;
class Handler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addLog(QString log, LogType logType = LogType::Info);

public slots:
    void onStart();
    void onClear();
    void onClearLogBrowser();
	void onAllFilePartsReady();
	//void onConnected();
    //void onDisconnected();
    //void onErrorInConnection(QAbstractSocket::SocketError error);
    //void onReadData();
    //void onSendDataToServer();
    //void onGetNewPacket(PacketData packet);

signals:
    //void readyPacket(PacketData);

private:
    Ui::MainWindow *				ui;
    QString							m_hiWord;
    QString							m_bufferData;
    //NetworkProcess*                 m_networkProcess;
    Handler*                        m_handler;
    Logger*                         m_logger;
    QString                         IP_Add;

    /*
     * QTcpSocket* m_clientTcp = nullptr; // not recommended
     * QTcpSocket m_clientTcp;
     *
     */
};
#endif // MAINWINDOW_H
