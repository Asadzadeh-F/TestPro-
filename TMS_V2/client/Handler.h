#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include "packet.h"
#include "NetworkProcess.h"
#include "NetworkManager.h"
#include "Logger.h"

enum UpdateState
{
    InitState,
    DetectionMessage,
    GetFileParts
};

class Handler : public QObject
{
    Q_OBJECT
public:
    Handler(QObject *parent = 0);
    UpdateState Updatestatus();
    void setState(UpdateState state);

private:
    NetworkProcess*       m_networkProcess;
    NetworkManager*       m_networkManager;
    UpdateState           m_updatestatus;
    uint16_t              m_Index;
    uint16_t              lastIndex;
    uint16_t              totalIndex;
    uint16_t              currentIndex;
    QTcpSocket*           m_client;
    bool                  isPacketLost;
    Packet                m_RCVpacket;
    QByteArray            data;
    uint16_t              notmatchIndexs;
    QString               m_address;
    uint32_t              m_port;
    Handler*              m_handler;
    //Logger                m_logger;
    QSettings             m_settings;
    uint64_t              m_hash;



    void processPacket(bool isPacketLost = false);

public slots:
    void onConnectedToServer();
    void onGetPacket(QVariant packet);
    void onPacketLost();
    //void onStartUpdate();
    bool startUpdate(QString serverAdress, uint32_t port);

signals:
    void readyAllFileParts();
    void failedUpdate();
    void statusChanged();


};

#endif // HANDLER_H
