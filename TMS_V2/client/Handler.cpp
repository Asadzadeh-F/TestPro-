#include "Handler.h"
#include "QTcpSocket"
#include "NetworkManager.h"
#include "NetworkProcess.h"
#include "memoryBlockManagement.h"


class NetworkManager;
class NetworkProcess;


Handler::Handler(QObject* parent) : QObject(parent)
{
    m_Index             =  0;
    m_networkManager    = new NetworkManager(this);
    m_networkProcess    = new NetworkProcess(this); //Free With Qt

    connect(m_networkProcess, SIGNAL(detectedPacket(QVariant)), this, SLOT(onGetPacket(QVariant)));
    connect(m_networkProcess, SIGNAL(packetLost()), this, SLOT(onPacketLost()));

    connect(m_networkManager, SIGNAL(readyData(QByteArray)), m_networkProcess,
            SLOT(processData(QByteArray)));
    connect(m_networkManager, SIGNAL(connected()), this, SLOT(onConnectedToServer()));
    //m_settings = new QSettings ("SEA", "TMS");
    QSettings m_settings("SEA", "TMS");

}

bool Handler::startUpdate(QString serverAdress, uint32_t port)
{
    m_address = serverAdress;
    m_port	  = port;

    m_networkManager->connectToHost(m_address, m_port);
    setState(DetectionMessage);
    return true;
}

void Handler::processPacket(bool isPacketLost)
{
    switch (m_updatestatus)
    {
        case UpdateState::InitState: {
            //log to please call startUpdate();
        }
        break;
        case UpdateState::DetectionMessage: {
            //lastIndex = 0;
            lastIndex = m_settings.value("lastGetIndex").toUInt();

            if(m_networkManager->sendDetectionMessage(SerialNumber,lastIndex + 1))
            {
                setState(GetFileParts);
            }
        }
        break;
        case UpdateState::GetFileParts: {
            Packet packet;

            lastIndex  = m_settings.value("lastGetIndex").toUInt();
            totalIndex = m_settings.value("Totalndex").toUInt();
            notmatchIndexs = (lastIndex + 1 != m_RCVpacket.index);
            if (isPacketLost || notmatchIndexs)
                m_networkManager->sendErrorCode(notmatchIndexs ? WrongIndex:PacketLost);
            else //packet correct
            {
                if (m_RCVpacket.header.command != FinalAcknowledgement)
                {
                    m_settings.setValue("lastGetIndex",m_RCVpacket.index);
                    m_networkManager->sendAckMessage((m_RCVpacket.header.command == EndOfFile && lastIndex == totalIndex ? 0
                                                       : lastIndex + 1));
                }
                else //Final Acknowledgement,check Hash

                    if (m_RCVpacket.header.command == FinalAcknowledgement)
                    {
                        m_hash = m_settings.value("Hash").toUInt();
                        //compare hash
                        if (m_RCVpacket.calcHash().toUInt() == m_hash)
                        {
                            setState(InitState);
                            emit readyAllFileParts();
                            //m_logger.writeLog("GerFileParts state: ReadyAllFiles");
                            m_networkManager->sendAckMessage(-1);

                        }
                    }

            }
        }
        break;
    }
}

void Handler::onGetPacket(QVariant packet)
{
    m_RCVpacket = packet.value<Packet>();
    processPacket();
}

void Handler::onPacketLost()
{
    processPacket(true);
}

void Handler::onConnectedToServer()
{
    processPacket();
}

UpdateState Handler::Updatestatus()
{
    return m_updatestatus;
}

void Handler::setState(UpdateState status)
{
    if (m_updatestatus != status)
    {
        m_updatestatus = status;
        //emit statusChanged();
    }
}
