#include "hdr/TMS/NetworkProcess.h"
#include "hdr/TMS/Logger.h"
#include "hdr/TMS/NetworkManager.h"
#include "QVariant"
#include <QCoreApplication>
#include <QDebug>
#include "iostream"
#include "stdio.h"

using namespace std;

NetworkProcess::NetworkProcess(QObject *parent) : QObject(parent)
{
    m_currentProcessState	= DetectHeader;
	m_packetType			= Simple;
    m_index					=  0;
    m_startHeader			=  0;
    m_logger                =  0;
    //m_settings              = nullptr;
    //m_settings              = new Settings(this);
	m_memoryBlockManagement = nullptr;
	m_memoryBlockManagement = new memoryBlockManagement;
    m_logger                = new Logger(this);
}

NetworkProcess::~NetworkProcess()
{
    SafeDelete(m_memoryBlockManagement);
}

void NetworkProcess::processData(QByteArray data)
{  
    m_bufferData += data;
    //m_bufferData.append(data);

    //test : RAW
    QByteArray alakiorginal = m_bufferData;
    QString filePath0 = "/yaffs/CashLess/TMStestfile_orginal";
    QFile TMStest0(filePath0);
    if ( TMStest0.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        TMStest0.write(alakiorginal);
        TMStest0.close();
    }
    //test : convert to Hex
    QByteArray alaki2 = m_bufferData;
    alaki2 = alaki2.toHex().toUpper();
    QString filePath = "/yaffs/CashLess/TMStestfile_toHex";
    QFile TMStest(filePath);
    if ( TMStest.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        TMStest.write(alaki2);
        TMStest.close();
    }

    qDebug() << "NetworkProcess::processData";
    //m_logger->writeLog(QtDebugMsg , "NetworkProcess::processData readyData emited!");

    uint8_t minNeededSize = 0;
    switch (m_currentProcessState)
    {
        case ProcessPacketState::CleanProcess: {
        }
        case ProcessPacketState::DetectHeader: {
            minNeededSize = MinimumPacketSize;
        }
        break;
        case ProcessPacketState::ReadPayLoadSize: {
            minNeededSize = MinimumPacketSize - sizeof(m_packet.header.header);
        }
        break;
        case ProcessPacketState::ReadCommand: {
            minNeededSize = MinimumPacketSize - sizeof(m_packet.header.header)
                            - sizeof(m_packet.header.sizeOfPayload);
        }
        break;
        case ProcessPacketState::ReadPayload: {
            minNeededSize = MinimumPacketSize - sizeof(m_packet.header);
        }
        break;
        case ProcessPacketState::ReadIndex: {
            minNeededSize = MinimumPacketSize - sizeof(m_packet.header);
        }
        break;

        case ProcessPacketState::ReadCRC: {
            minNeededSize = MinimumPacketSize - sizeof(m_packet.header) - sizeof(m_packet.index);
        }
        break;
        case ProcessPacketState::ReadFooter: {
        }
        break;
    }

    if ((m_bufferData.size() - m_index) < minNeededSize)
    {
        //m_logger->writeLog(QtWarningMsg , "NetworkProcess::processData did not receive enough Data!");
        return;
    }

    qDebug() << "m_bufferData.size :: " << m_bufferData.size();

    while ((m_bufferData.size() - m_index) > 0)
    {
        switch (m_currentProcessState)
        {
            case ProcessPacketState::CleanProcess: {
                m_bufferData.remove(0, m_index);
                m_bufferData.clear();
                m_index				  = 0;
                m_currentProcessState = ProcessPacketState::DetectHeader;
            }
            case ProcessPacketState::DetectHeader: {
                if ((m_bufferData.size() - m_index) >= sizeof(m_packet.header.header))
                {
                    cout << "DetectHeader::m_index: " << m_index << endl;
                    cout << "uint8_t(m_bufferData.at(m_index)): "
                         << uint8_t(m_bufferData.at(m_index)) << endl;
                    if (uint8_t(m_bufferData.at(m_index)) == HeaderValue)
                    {
                        m_startHeader		   = m_index;
                        m_packet.header.header = m_bufferData.at(m_index);
                        m_index                =+ sizeof(m_packet.header.header);
                        m_currentProcessState  = ProcessPacketState::ReadPayLoadSize;

                    }
                    else
                    {
                        m_currentProcessState = ProcessPacketState::CleanProcess;
                        m_index               =+ sizeof(m_packet.header.header);
                        //emit packetLost();
                        //m_networkManager->sendErrorCode(ErrorCode::PacketLost);
                        //return;
                    }
                }
                else
                {
                    return;
                }
            }
            break;
            case ProcessPacketState::ReadPayLoadSize: {
                if ((m_bufferData.size() - m_index) >= sizeof(m_packet.header.sizeOfPayload))
                {
                    
                    //memcpy(&m_packet.header.sizeOfPayload, (m_bufferData.data() + m_index),
                           //sizeof(m_packet.header.sizeOfPayload));
                    memcpy(&m_packet.header.sizeOfPayload, (m_bufferData.data() + m_index), sizeof(m_packet.header.sizeOfPayload));
                    
                    m_index += sizeof(m_packet.header.sizeOfPayload);
                    cout << "ReadPayLoadSize::m_index: " << m_index << endl;

                    if (uint8_t(m_bufferData.at(m_index)) == HeaderValue)
                        qDebug() << "test***";

                    if (m_bufferData.size() > m_index)
                        cout << "uint8_t(m_bufferData.at(m_index)): "
                             << uint8_t(m_bufferData.at(m_index)) << endl;

                    //test
                    cout << "#DetectHeader::ReadPayLoadSize: " << sizeof(m_packet.header.sizeOfPayload) << endl;
                    cout << "#DetectHeader::ReadPayLoadSize::value " << m_packet.header.sizeOfPayload << endl;
                    qDebug() << "Value of PayLoadsize : " << hex << m_packet.header.sizeOfPayload;
                    qDebug() << (void *) m_packet.header.sizeOfPayload;
                    qDebug() << QString::number(m_packet.header.sizeOfPayload, 16);
                    qDebug() << "Value of PayLoad : " << m_packet.header.sizeOfPayload;



                    m_currentProcessState = ProcessPacketState::ReadCommand;
                }
                else
                {
                    return;
                }
            }
            break;
            case ProcessPacketState::ReadCommand: {
                if ((m_bufferData.size() - m_index) >= sizeof(m_packet.header.command))
                {
                    memcpy(&m_packet.header.command, (m_bufferData.data() + m_index),
                           sizeof(m_packet.header.command));
                    m_index += sizeof(m_packet.header.command);
                    cout << "ReadCommand::m_index: " << m_index << endl;
                    if (m_bufferData.size() > m_index)
                        cout << "uint8_t(m_bufferData.at(m_index)): "
                             << uint8_t(m_bufferData.at(m_index)) << endl;
                    /*m_currentProcessState = (m_packet.header.command == Command::GetFile)
                                                    ? ProcessPacketState::ReadIndex
                                                    : ProcessPacketState::ReadPayload;*/
                    m_currentProcessState = ProcessPacketState::ReadPayload;
                    /*
                        m_packetType          = (m_packet.header.command == Command::GetFile)
                                           ? PacketType::Index : PacketType::Simple;
                        auto size = ((m_packet.header.command == Command::GetFile)
                                         ? sizeof(m_packet.index) : 0)
                                    + m_packet.header.sizeOfPayload + sizeof(m_packet.footer);*/
                    auto size = sizeof(m_packet.index) + m_packet.header.sizeOfPayload
                                + sizeof(m_packet.CRC) + sizeof(m_packet.footer);

                    //test
                    qDebug() << "(m_bufferData.size() - m_index) : " << (m_bufferData.size() - m_index) <<"size variable:" <<size;
                    qDebug() << "sizeof header :" << sizeof(m_packet.header.header);
                    qDebug() << "sizeof sizepart :" << sizeof(m_packet.header.sizeOfPayload);
                    qDebug() << "sizeof command :" << sizeof(m_packet.header.command);
                    qDebug() << "sizeof data :" << sizeof(m_packet.data);
                    qDebug() << "sizeof index :" << sizeof(m_packet.index);
                    qDebug() << "sizeof crc :" << sizeof(m_packet.CRC);
                    qDebug() << "sizeof footer :" << sizeof(m_packet.footer);

                    //qDebug() << "sizeof  :" << sizeof(m_packet.header) + sizeof(m_packet.index) + m_packet.header.sizeOfPayload
                               // + sizeof(m_packet.CRC) + sizeof(m_packet.footer);

                    if ((m_bufferData.size() - m_index) < size)
                    {
                        qDebug() << "m_index:" << m_index;
                        qDebug() << "m_bufferData.size: " << m_bufferData.size();
                        qDebug("!!m_bufferData.size() - m_index) < size!!");
                        return;
                    }
                }
                else
                {
                    cout << "ReadCommand::m_bufferData.size() - m_index) < "
                            "sizeof(m_packet.header.command"
                         << endl;
                    return;                    
                }
            }
            break;

            case ProcessPacketState::ReadPayload: {
                if ((m_bufferData.size() - m_index) >= m_packet.header.sizeOfPayload)
                {
                    m_packet.data = (char *) m_memoryBlockManagement->getMemory(
                        m_packet.header.sizeOfPayload);

                    memcpy(m_packet.data, (m_bufferData.data() + m_index),
                           m_packet.header.sizeOfPayload);
                    m_index += m_packet.header.sizeOfPayload;
                    cout << "ReadPayload::m_index: " << m_index << endl;
                    if (m_bufferData.size() > m_index)
                        cout << "uint8_t(m_bufferData.at(m_index)): "
                             << uint8_t(m_bufferData.at(m_index)) << endl;
                    m_currentProcessState = ProcessPacketState::ReadIndex;
                    //set setting file
                }
                else
                {
                    cout << "m_bufferData.size() - m_index) >= m_packet.header.sizeOfPayload"<< endl;
                    return;                    
                }
            }
            break;
            case ProcessPacketState::ReadIndex: {
                if ((m_bufferData.size() - m_index) >= sizeof(m_packet.index))
                {
                    memcpy(&m_packet.index, m_bufferData.data() + m_index, sizeof(m_packet.index));
                    m_index += sizeof(m_packet.index);
                    cout << "ReadIndex::m_index: " << m_index << endl;
                    if (m_bufferData.size() > m_index)
                        cout << "uint8_t(m_bufferData.at(m_index)): "
                             << uint8_t(m_bufferData.at(m_index)) << endl;
                    m_currentProcessState = ProcessPacketState::ReadCRC;
                    //the first msg from server containing the total number
                    /*
                    if (m_packet.header.command == Detection)
                    {
                        uint16_t TotalIndex = m_packet.index;
                        m_settings->writeValue("UpdateData", "Hash", m_packet.data);
                        m_settings->writeValue("UpdateData", "Total Index", TotalIndex);
                    }
                    */
                }//if
                else
                {

                    return;
                }

            }
           break;
            case ProcessPacketState::ReadCRC: {
                if ((m_bufferData.size() - m_index) >= sizeof(m_packet.CRC))
                {
                    memcpy(&m_packet.CRC, (m_bufferData.data() + m_index), sizeof(m_packet.CRC));
                    m_index += sizeof(m_packet.CRC);
                    qDebug() << "Value of CRC : " << hex << m_packet.CRC;
                    cout << "CRC: " << m_packet.CRC << endl;
                    cout << "ReadCRC::m_index: " << m_index << endl;
                    if (m_bufferData.size() > m_index)
                        cout << "uint8_t(m_bufferData.at(m_index)): "
                             << uint8_t(m_bufferData.at(m_index)) << endl;
                    if (m_packet.header.command == GetFileInfo && m_packet.CRC == 0 )
                    {
                        m_currentProcessState = ProcessPacketState::ReadFooter;
                    }
                    else{
                            if (m_packet.getCRC() != m_packet.CRC)
                            {
                                m_currentProcessState = ProcessPacketState::CleanProcess;
                                cout << "not match CRC";
                                //m_logger->writeLog(QtWarningMsg , "NetworkProcess::processData did not receive enough Data!");
                                emit packetLost();
                                //m_networkManager->sendErrorCode(ErrorCode::PacketLost);
                                //log
                                return;
                            }
                            else
                            {
                                m_currentProcessState = ProcessPacketState::ReadFooter;
                            }
                       }
                }
                else
                {
                    return;
                }
            }
            break;

            case ProcessPacketState::ReadFooter: {
                if ((m_bufferData.size() - m_index) >= sizeof(m_packet.footer))
                {
                    if (uint8_t(m_bufferData.at(m_index)) == FooterValue)
                    {
                        m_packet.footer		  = m_bufferData.at(m_index);
                        m_currentProcessState = ProcessPacketState::CleanProcess;
                        m_index               += sizeof(m_packet.footer);
                        qDebug() << "ReadFooter::m_index:" << m_index;
                        auto tempMem = m_memoryBlockManagement->getNewMemory(
                            m_packet.header.sizeOfPayload);
                        memcpy(tempMem, m_packet.data, m_packet.header.sizeOfPayload);

                        m_packet.data = (char *) tempMem;
                        m_bufferData.remove(0, m_index);
                        emit detectedPacket(QVariant::fromValue<Packet>(m_packet));
                        m_packet.data = nullptr;

                        qApp->processEvents();
                        //m_currentProcessState = ProcessPacketState::CleanProcess;

                    }
                    else
                    {
                        m_currentProcessState = ProcessPacketState::CleanProcess;
                        m_index++;
                        emit packetLost();
                        //m_networkManager->sendErrorCode(ErrorCode::PacketLost);
                        return; //header size command payload(header size command payload) crc footer header
                    }
                }
                else
                {
                    return;
                }
            }
            break;
        }
    }
}

NetworkProcess::ProcessPacketState NetworkProcess::currentProcessState() const
{
    return m_currentProcessState;
}
