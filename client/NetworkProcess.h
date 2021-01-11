#ifndef NETWORKPROCESS
#define NETWORKPROCESS

#include "packet.h"
#include "stdint.h"
#include "QVariant"
#include "Logger.h"
#include "Settings.h"
#include "NetworkManager.h"
#include "memoryBlockManagement.h"


#include <QtCore/QObject>



class NetworkProcess : public QObject
{
    Q_OBJECT
public:
    enum ProcessPacketState
    {
        DetectHeader,
        ReadPayLoadSize,
        ReadCommand,
        ReadIndex,
        ReadPayload,
        ReadCRC,
        ReadFooter,
        CleanProcess
    };

    enum PacketType
    {
        Simple,
        Index
    };

public:
    explicit NetworkProcess(QObject *parent = 0);
    NetworkProcess::ProcessPacketState currentProcessState () const;
    ~NetworkProcess();
    bool checkCRC(QVariant *packet);
public slots:
    void processData(QByteArray);


signals:
   void  detectedPacket(QVariant);
   void  packetLost();


public:
    ProcessPacketState     m_currentProcessState;
    QByteArray             m_bufferData;
    PacketType             m_packetType ;
    uint64_t               m_index ;
    uint64_t               m_startHeader;
    //SimplePacket         m_simplePacket;
    //Settings*              m_settings;
    uint16_t               m_hash;
    uint32_t               tempHash;




    memoryBlockManagement* m_memoryBlockManagement;
    NetworkManager*        m_networkManager;




private:
    Packet                 m_packet;
    Logger*                m_logger;
};

#endif // NETWORKPROCESS
