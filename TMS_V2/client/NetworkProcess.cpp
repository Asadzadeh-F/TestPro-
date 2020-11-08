#include "NetworkProcess.h"
#include "NetworkManager.h"
#include <QCoreApplication>
#include <QDebug>
#include "QVariant"
#include "Logger.h"
NetworkProcess::NetworkProcess(QObject *parent) : QObject(parent) {
    m_currentProcessState   = DetectHeader;
    m_packetType            = Simple;
    m_index                 = 0;
    m_startHeader           = 0;
    m_memoryBlockManagement = nullptr;
    m_memoryBlockManagement = new memoryBlockManagement;

}


NetworkProcess::~NetworkProcess()
{
    SafeDelete(m_memoryBlockManagement);
}


void NetworkProcess::processData(QByteArray data)
{
    m_bufferData += data;

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
        case ProcessPacketState::ReadIndex: {
            minNeededSize = MinimumPacketSize - sizeof(m_packet.header);
        }
        break;
        case ProcessPacketState::ReadPayload: {
            minNeededSize = MinimumPacketSize - sizeof(m_packet.header);
        }
    case ProcessPacketState::ReadCRC: {
            minNeededSize = MinimumPacketSize - sizeof(m_packet.header)
                                              - sizeof(m_packet.index);
        }
        break;
        case ProcessPacketState::ReadFooter: {
        }
        break;
    }

    if ((m_bufferData.size() - m_index) < minNeededSize)
        return;

    qDebug() << m_bufferData.size();

    while ((m_bufferData.size() - m_index) > 0)
    {
        switch (m_currentProcessState)
        {
            case ProcessPacketState::CleanProcess: {
                //delete[] m_packet.data;
                //m_simplePacket.data = nullptr;
                //m_bufferData.remove(0,m_index);
                //m_index = 0;
                m_currentProcessState           = ProcessPacketState::DetectHeader;
            }
            case ProcessPacketState::DetectHeader: {
                if ((m_bufferData.size() - m_index) >= sizeof(m_packet.header.header))
                {
                    if (uint8_t(m_bufferData.at(m_index)) == HeaderValue)
                    {
                        m_startHeader           = m_index;
                        m_packet.header.header  = m_bufferData.at(m_index);
                        m_index++;
                        m_currentProcessState   = ProcessPacketState::ReadPayLoadSize;
                    }
                    else
                    {
                        m_currentProcessState   = ProcessPacketState::CleanProcess;
                        m_index++;
                        emit packetLost();
                        return;
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
                    memcpy(&m_packet.header.sizeOfPayload, (m_bufferData.data() + m_index),
                           sizeof(m_packet.header.sizeOfPayload));
                    m_index              += sizeof(m_packet.header.sizeOfPayload);
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
                    m_index              += sizeof(m_packet.header.command);
                    /*m_currentProcessState = (m_packet.header.command == Command::GetFile)
                                                ? ProcessPacketState::ReadIndex
                                                : ProcessPacketState::ReadPayload;*/
                    m_currentProcessState   = ProcessPacketState::ReadIndex;
                    /*
                    m_packetType          = (m_packet.header.command == Command::GetFile)
                                       ? PacketType::Index : PacketType::Simple;
                    auto size = ((m_packet.header.command == Command::GetFile)
                                     ? sizeof(m_packet.index) : 0)
                                + m_packet.header.sizeOfPayload + sizeof(m_packet.footer);*/
                    auto size  = m_packet.index + m_packet.header.sizeOfPayload + sizeof(m_packet.CRC) + sizeof(m_packet.footer);
                    qDebug() << (m_bufferData.size() - m_index) << size;
                    if ((m_bufferData.size() - m_index) < size)
                    {
                        return;
                    }
                }
                else
                {
                    return;
                }
            }
            break;

            case ProcessPacketState::ReadPayload: {
                if ((m_bufferData.size() - m_index) >= m_packet.header.sizeOfPayload)
                {
                    m_packet.data        = (char *) m_memoryBlockManagement->getMemory(
                        m_packet.header.sizeOfPayload);

                    memcpy(m_packet.data, (m_bufferData.data() + m_index),
                           m_packet.header.sizeOfPayload);
                    m_index              += m_packet.header.sizeOfPayload;
                    m_currentProcessState = ProcessPacketState::ReadFooter;
                    if(m_packet.header.command == Detection)
                    {
                        int temp = QByteArray::fromRawData((char *)m_packet.data,  m_packet.header.sizeOfPayload).toInt();
                        m_setting.setValue("Hash",temp);
                    }
                }
                else
                {

                    return;
                }
            }
            break;
            case ProcessPacketState::ReadIndex: {
                if ((m_bufferData.size() - m_index) >= sizeof(m_packet.index))
                {
                    memcpy(&m_packet.index, m_bufferData.data() + m_index, sizeof(m_packet.index));
                    m_index              += sizeof(m_packet.index);
                    m_currentProcessState = ProcessPacketState::ReadPayload;
                    if(m_packet.header.command == Detection) //the first msg from server containing the total number
                    {
                        int TotalIndex = m_packet.index;
                        m_setting.setValue("Totalndex", TotalIndex);
                    }
                else
                {

                    return;
                }
            }
            }
            break;
            case ProcessPacketState::ReadCRC:
            {

            if ((m_bufferData.size() - m_index) >= sizeof(m_packet.CRC))
            {
                memcpy(&m_packet.CRC,(m_bufferData.data() + m_index),sizeof(m_packet.CRC));
                m_index              += sizeof(m_packet.CRC);
                if(m_packet.getCRC() != m_packet.CRC)
                {
                    m_currentProcessState = ProcessPacketState::CleanProcess;
                    emit packetLost();
                    //log
                    return;
                }

            }
            else
            {
                m_currentProcessState = ProcessPacketState::ReadFooter;

            }
             }
            break;

            case ProcessPacketState::ReadFooter: {
                if ((m_bufferData.size() - m_index) >= sizeof(m_packet.footer))
                {
                    if (uint8_t(m_bufferData.at(m_index)) == FooterValue)
                    {
                        m_packet.footer          = m_bufferData.at(m_index);
                        m_currentProcessState    = ProcessPacketState::CleanProcess;
                        m_index++;
                        auto tempMem             = m_memoryBlockManagement->getNewMemory(m_packet.header.sizeOfPayload);
                        memcpy(tempMem, m_packet.data, m_packet.header.sizeOfPayload);
                        m_packet.data			 = (char *)tempMem;
                        m_packet.mustFree        = true;
                        emit detectedPacket(QVariant::fromValue<Packet>(m_packet));
                        m_packet.data            = nullptr;
                        m_packet.mustFree        = false;
                        qApp->processEvents();
                        qDebug() << m_index;
                    }
                    else
                    {
                        m_currentProcessState = ProcessPacketState::CleanProcess;
                        m_index++;
                        emit packetLost();
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
