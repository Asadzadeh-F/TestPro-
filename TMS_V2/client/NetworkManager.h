#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include "QObject"
#include "QTcpSocket"
#include "memoryBlockManagement.h"
#include "packet.h"

class NetworkManager :public QTcpSocket
{
    Q_OBJECT
public:
    NetworkManager(QObject* parent = nullptr);
    //QString Logmessage(QString log,LogType logtype =info );
    ~NetworkManager();
    //uint64_t write(char* data, uint64_t size_t);


    bool sendAckMessage(int16_t index);
    bool sendErrorCode(ErrorCode errorcode);
    bool sendDetectionMessage(uint64_t serial, uint16_t thelastRCVIndex);
    //bool connectToServer(QString serverAddress, uint32_t port);

signals:
    void readyData(QByteArray data);
    //void errorOccurred();
    //void connected();
    //void disconnected();

public slots:
    //void onConnected();
    //void onDisconnected();
    //void onErrorInConnection(QAbstractSocket::SocketError error);
    void onReceiveData();

private:
    bool writePacket(QByteArray& data);
    //QTcpSocket            *client;
    memoryBlockManagement   m_memoryBlock;
    //uint16_t                m_lastRCVIndex;

};

#endif // NETWORKMANAGER_H
