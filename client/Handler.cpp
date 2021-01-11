#include "hdr/TMS/Handler.h"
#include "hdr/TMS/NetworkManager.h"
#include "hdr/TMS/NetworkProcess.h"
#include "hdr/TMS/memoryBlockManagement.h"
#include <QApplication>
#include "QTcpSocket"
#include <string>
#include <QMessageBox>
#include "iostream"
#include "QVariant"
using namespace std;

Handler::Handler(QObject* parent) : QObject(parent)
{
    m_Index			 = 0;
    m_networkManager = nullptr;
    m_networkProcess = nullptr;
    m_settings		 = nullptr;
    m_networkManager = new NetworkManager(this);
    m_networkProcess = new NetworkProcess(this); //Free With Qt
    QCoreApplication::setOrganizationName("SEA");
    QCoreApplication::setApplicationName("TMSClient");
    m_settings		 = new Settings();
    m_logger         = new Logger(this);

    connect(m_networkProcess, SIGNAL(detectedPacket(QVariant)), this, SLOT(onGetPacket(QVariant)));
    connect(m_networkProcess, SIGNAL(packetLost()), this, SLOT(onPacketLost()));
    //	connect(m_networkProcess, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
    //			SLOT(tcpSocketState(QAbstractSocket::SocketState)), Qt::UniqueConnection);
    //	connect(m_networkProcess, SIGNAL(error(QAbstractSocket::SocketError)),
    //			SLOT(connectionError(QAbstractSocket::SocketError)), Qt::UniqueConnection);
    //uncommentttttttttttt
    connect(m_networkManager, SIGNAL(readyData(QByteArray)), m_networkProcess,
            SLOT(processData(QByteArray)));
    connect(m_networkManager, SIGNAL(connected()), this, SLOT(onConnectedToServer()));
    connect(m_networkManager, SIGNAL(disconnected()), this, SLOT(onDisconnectedFromServer()));
    connect(m_networkManager, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,
            SLOT(connectionWasClosed(QAbstractSocket::SocketState)));

   }

bool Handler::startUpdate(QString serverAdress, uint32_t port)
{
    m_address = serverAdress;
    m_port	  = port;
    //m_settings->setParameters(0, 1, 0);
    //m_logger->writeLog(QtDebugMsg, "start connecting...");
    m_networkManager->connectToHost(m_address, m_port);
    setState(UpdateState::DetectionMessage);
     memset(serialtest, 0, sizeof(serialtest));
    sprintf(serialtest, "936755008098", sizeof("936755008098"));
    return true;
}

void Handler::processPacket(bool isPacketLost)
{
    qDebug() << "processPacket start...";
    switch (m_updatestatus)
    {
        case UpdateState::InitState: {
            //log to please call startUpdate();
        }
        break;
        case UpdateState::DetectionMessage: {
            lastIndex = 0;
            lastIndex =
                     m_settings->containInKeys("LastGetIndex")
                     ? m_settings->readValue("UpdateData", "LastGetIndex", lastIndex).toUInt() + 1
                     : 0;

            //check hash for last receive parts.

            //test first packet

            //QString serialtest_stirng= "DSG936755008098";
            //serialtest = serialtest_stirng.toUInt();

            if (m_networkManager->sendDetectionMessage(serialtest, lastIndex))
            {
                qDebug() << "sendDetectionMessage...";
                setState(UpdateState::GetFileParts);
                qDebug() << "State::GetFileParts";
            }
        }
        break;
        case UpdateState::GetFileParts: {
            lastIndex  = m_settings->readValue("UpdateData", "LastGetIndex", lastIndex).toUInt();
            totalIndex = m_settings->readValue("UpdateData", "TotalIndex", totalIndex).toUInt();
            notmatchIndexs = (lastIndex + 1 != m_RCVpacket.index);



            if (((m_RCVpacket.header.command != Command::GetFileInfo) &&  notmatchIndexs ) || isPacketLost)
            {
                m_networkManager->sendErrorCode(notmatchIndexs ? ErrorCode::IndexMismatch
                                                               : ErrorCode::PacketLost);
                qDebug() << "Error";
                m_data.clear();
            }//incorrect packet
            else //packet correct
            {
                if(m_RCVpacket.header.command == Command::GetFileInfo)
                {
                    m_hash =
                         (m_settings->containInKeys("Hash")
                         ? m_settings->readValue("UpdateData", "Hash", m_hash).toUInt()
                         : -1);
                    uint32_t tempHash = QByteArray::fromRawData((char *) m_RCVpacket.data,
                                                                m_RCVpacket.header.sizeOfPayload).toInt();
                    if (tempHash != m_hash )
                    {
                        if(m_networkManager->sendAckMessage(serialtest, 1))
                        {
                            //cout<< "notmatch Hash::last Packet update failed. Restart sending agein."<< endl;
                            qDebug() << "sendFirstAckMessage(1)...";
                            setState(UpdateState::GetFileParts);
                            qDebug() << "State::GetFileParts...";
                        }
                    }
                    else
                    {
                        m_networkManager->sendAckMessage(serialtest, m_RCVpacket.index + 1);
                        setState(UpdateState::GetFileParts);
                    }

                }//GetFileInfo
                else
                {
                    if(m_RCVpacket.header.command == Command::GetFile)
                    {
                        if( m_RCVpacket.index == totalIndex)
                        {
                            m_networkManager->sendAckMessage(serialtest,0);
                            setState(UpdateState::InitState);

                        }
                        else
                           if(m_RCVpacket.index < totalIndex)
                            {
                               m_networkManager->sendAckMessage(serialtest,m_RCVpacket.index + 1);
                               setState(UpdateState::GetFileParts);
                            }
                            else
                           {
                               cout << "m_RCVpacket.index < totalIndex ::" << "m_RCVpacket.index:"
                                    << m_RCVpacket.index << "  " << "totalIndex:" << totalIndex <<endl;
                               m_networkManager->sendErrorCode(ErrorCode::IndexMismatch);
                               setState(UpdateState::InitState);
                           }



                    }
                }//elseGetInfo

            }
            //{
            //    //receive reply of detection // get hash and total index
            //    //m_settings->writeValue("UpdateData", "LastGetIndex", m_RCVpacket.index);
            //    m_data += QByteArray::fromRawData(m_RCVpacket.data,
            //                                      m_RCVpacket.header.sizeOfPayload);
            //    //if (m_RCVpacket.header.command == EndOfFile)
            //    //{
            //        if (m_RCVpacket.index == totalIndex)
            //        {
            //            m_hash =
            //            (m_settings->containInKeys("Hash")
            //            ? m_settings->readValue("UpdateData", "Hash", m_hash).toUInt()
            //            : -1);
            //            cout << "Hash : " << m_hash << endl ;
            //            //compare hash
            //            if (m_RCVpacket.calcHash().toUInt() == m_hash)
            //            {
            //                setState(UpdateState::InitState);
            //                m_networkManager->sendAckMessage(serialtest,0);
            //                emit readyAllFileParts(m_data);
            //                //m_logger.writeLog(QtDebugMsg,"GerFileParts state: ReadyAllFiles");
            //                m_data.clear();
            //            }
            //         }
            //        //m_RCVpacket.index != totalIndex
            //        else
            //            if(m_RCVpacket.index < totalIndex)
            //        {
            //            m_networkManager->sendErrorCode(ErrorCode::HashMismatch);
            //        }
            //      }//Command ::EndOfFile
            //    else{
            //    if (m_RCVpacket.header.command == Command::GetFileInfo)
            //    {
            //        //uint32_t hash     = m_settings->readValue("UpdateData", "Hash", hash).toUInt();
            //        m_hash =
            //        (m_settings->containInKeys("Hash")
            //        ? m_settings->readValue("UpdateData", "Hash", m_hash).toUInt()
            //        : -1);
            //        uint32_t tempHash = QByteArray::fromRawData((char *) m_RCVpacket.data,
            //                                               m_RCVpacket.header.sizeOfPayload).toInt();
            //
            //        if (tempHash != m_hash )
            //        {
            //            if(m_networkManager->sendAckMessage(serialtest, 1))
            //            {
            //                //cout<< "last Packet failed. Restart sending agein."<< endl;
            //                qDebug() << "sendFirstAckMessage(1)...";
            //                setState(UpdateState::GetFileParts);
            //                qDebug() << "State::GetFileParts...";
            //            }
            //        }//tempHash == hash
            //        else
            //        {
            //            m_networkManager->sendAckMessage(serialtest, m_RCVpacket.index + 1);
            //            setState(UpdateState::GetFileParts);
            //        }
            //      }//Command::GetFileInfo
            //       //m_data.clear();
            //    }
            // }//correct packet

        }//GetFileParts
        break;
    }//switch
}

void Handler::onGetPacket(QVariant packet)
{
     m_RCVpacket = packet.value<Packet>();
    if ( m_RCVpacket.header.command == Command::GetFileInfo )
    {
        QByteArray data = QByteArray::fromRawData(m_RCVpacket.data,
                                          m_RCVpacket.header.sizeOfPayload);
        if(m_settings->containInKeys("Hash"))
            m_settings->writeValue("UpdateData", "Hash", data);
        else
            qDebug() << "Error : write Hash in setting file";
        if(m_settings->containInKeys("TotalIndex"))
            m_settings->writeValue("UpdateData", "TotalIndex", m_RCVpacket.index);
        else
            qDebug() << "Error : write TotalIndex in setting file";
    }
    else
    if(m_settings->containInKeys("LastGetIndex"))
        m_settings->writeValue("UpdateData", "LastGetIndex", m_RCVpacket.index);
    else
        qDebug() << "Error : write LastGetIndex in setting file";

    m_RCVpacket.mustFree = true;

    processPacket();
}

void Handler::onPacketLost()
{
    processPacket(true);
}

void Handler::onConnectedToServer()
{
    qDebug() << "Connected to Server";
    //m_logger->writeLog(QtDebugMsg, "Client Connected...");
    processPacket();
}

void Handler::onDisconnectedFromServer()
{
    //m_logger->writeLog(QtDebugMsg, "Client Disconnected...");
    startUpdate(m_address, m_port);
}

UpdateState Handler::Updatestatus()
{
    m_logger->writeLog(QtDebugMsg, "State Changed...");
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

void Handler::connectionWasClosed(QAbstractSocket::SocketState state)
{

    qDebug() << "!!! SocketState changed !!!" << state;
    //m_logger->writeLog(QtDebugMsg, "connectionWasClosed...");
}
