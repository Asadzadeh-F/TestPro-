#ifndef HANDLER_H
#define HANDLER_H

#include "Logger.h"
#include "hdr/TMS/packet.h"
#include "QStringList"
#include "hdr/TMS/Settings.h"
#include <QAbstractSocket>
#include <QObject>

class NetworkManager;
class NetworkProcess;

enum class UpdateState
{
    InitState,
    DetectionMessage,
    GetFileParts
};

class Handler : public QObject
{
    Q_OBJECT
public:
    Handler(QObject* parent = 0);
    UpdateState         Updatestatus();
    void                setState(UpdateState state);
    bool                startUpdate(QString serverAdress, uint32_t port);

private:
    NetworkProcess*     m_networkProcess;
    NetworkManager*     m_networkManager;
    UpdateState         m_updatestatus;
    uint16_t            m_Index;
    uint16_t            lastIndex;
    uint16_t            totalIndex;
    uint16_t            currentIndex;
    bool                isPacketLost;
    Packet              m_RCVpacket;
    QByteArray          m_data;
    uint16_t            notmatchIndexs;
    QString             m_address;
    uint32_t            m_port;
    Handler*            m_handler;
    Logger*             m_logger;
    Settings*           m_settings;
    uint64_t            m_hash;
    char                serialtest[13];

    void processPacket(bool isPacketLost = false);

public slots:
    void onConnectedToServer();
    void onDisconnectedFromServer();

    void onGetPacket(QVariant packet);
    void onPacketLost();
    void connectionWasClosed(QAbstractSocket::SocketState state = QAbstractSocket::UnconnectedState);
    //void connectionError(QAbstractSocket::SocketError);
    //void tcpSocketState(QAbstractSocket::SocketState);
    //void onStartUpdate();

signals:
    void readyAllFileParts(QByteArray packet);
    void failedUpdate();
    void statusChanged();
};

#endif // HANDLER_H
