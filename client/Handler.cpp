#include "Handler.h"
#include "NetworkManager.h"
#include "NetworkProcess.h"
#include "QTcpSocket"
#include "memoryBlockManagement.h"
#include <string>
#include <QMessageBox>

class NetworkManager;
class NetworkProcess;

Handler::Handler(QObject* parent) : QObject(parent)
{
    m_Index              = 0;
    m_networkManager     = 0;
    m_networkProcess     = 0;
    m_networkManager     = new NetworkManager(this);
    m_networkProcess     = new NetworkProcess(this); //Free With Qt
    //m_logger           = new Logger(this);

	connect(m_networkProcess, SIGNAL(detectedPacket(QVariant)), this, SLOT(onGetPacket(QVariant)));
	connect(m_networkProcess, SIGNAL(packetLost()), this, SLOT(onPacketLost()));
    connect(m_networkProcess, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(tcpSocketState(QAbstractSocket::SocketState)), Qt::UniqueConnection);
    connect(m_networkProcess, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(connectionError(QAbstractSocket::SocketError)), Qt::UniqueConnection);
    //uncommentttttttttttt
    connect(m_networkManager, SIGNAL(readyData(QByteArray)), m_networkProcess,SLOT(processData(QByteArray)));
	connect(m_networkManager, SIGNAL(connected()), this, SLOT(onConnectedToServer()));
	connect(m_networkManager, SIGNAL(disconnected()), this, SLOT(onDisconnectedFromServer()));
    connect(m_networkManager, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(connectionWasClosed(QAbstractSocket::SocketState)));
	//m_settings = new QSettings ("SEA", "TMS");
	QSettings m_settings("SEA", "TMS");
	m_settings.beginGroup("UpdateData");
	//settings.childKeys();
	//settings.setValue("Totalndex", 0);
	//settings.setValue("lastGetIndex", 0);
	//settings.setValue("Hash", 0);
	//settings.endGroup();
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
            lastIndex = m_settings.childKeys().contains("lastGetIndex")
							? m_settings.value("lastGetIndex").toUInt() + 1
                            : 0;

			//check hash for last receive parts.

			if (m_networkManager->sendDetectionMessage(SerialNumber, lastIndex))
			{
				setState(GetFileParts);
			}
		}
		break;
		case UpdateState::GetFileParts: {
			Packet packet;

			lastIndex	   = m_settings.value("lastGetIndex").toUInt();
			totalIndex	   = m_settings.value("Totalndex").toUInt();
			notmatchIndexs = (lastIndex + 1 != m_RCVpacket.index);
			if (isPacketLost || notmatchIndexs)
				m_networkManager->sendErrorCode(notmatchIndexs ? IndexMismatch : PacketLost);
			else //packet correct
			{
                //receive reply of detection // get hash and total index

				m_settings.setValue("lastGetIndex", m_RCVpacket.index);
				if (m_RCVpacket.header.command == EndOfFile)
				{
					if (m_RCVpacket.index == totalIndex)
					{
						m_hash = m_settings.value("Hash").toUInt();
						//compare hash
						if (m_RCVpacket.calcHash().toUInt() == m_hash)
						{
							setState(InitState);
							m_networkManager->sendAckMessage(0);
							emit readyAllFileParts();
                            //m_logger.writeLog(QtDebugMsg,"GerFileParts state: ReadyAllFiles");
						}
                        else
                            m_networkManager->sendErrorCode(ErrorCode::HashMismatch);
					}
				}
                else
                    if (m_RCVpacket.header.command == 0xD1)
                     {
                         m_settings.setValue("Hash",m_RCVpacket.data);
                         m_settings.setValue("TotalIndex",m_RCVpacket.index);
                         m_networkManager->sendAckMessage(1);
                    }

                    else
                    {

                        m_networkManager->sendAckMessage(m_RCVpacket.index + 1);
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
    qDebug() << "Connected to Server";
}

void Handler::onDisconnectedFromServer()
{
	startUpdate(m_address, m_port);
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

void Handler::connectionWasClosed(QAbstractSocket::SocketState state){

    qDebug() << "!!! SocketState changed !!!" << state ;

}
/*
void Handler::connectionError(QAbstractSocket::SocketError socketError)
{
    std::string errorStr = "ERROR: " + m_handler->errorString().toStdString();
    QMessageBox::information(this, tr("Tcp Server Client"), tr(errorStr.c_str()));
}

void Handler::tcpSocketState(QAbstractSocket::SocketState socketState)
{
    switch (socketState) {
        case QAbstractSocket::UnconnectedState:
            QMessageBox::information(this, tr("Tcp Server Client"), tr("The socket is not connected."));
            break;
        case QAbstractSocket::HostLookupState:
            QMessageBox::information(this, tr("Tcp Server Client"), tr("The socket is performing a hostname lookup."));
            break;
        case QAbstractSocket::ConnectedState:
            QMessageBox::information(this, tr("Tcp Server Client"), tr("A connection is established."));
            break;
        case QAbstractSocket::ConnectingState:
            QMessageBox::information(this, tr("Tcp Server Client"), tr("The socket has started establishing a connection."));
            break;
        case QAbstractSocket::BoundState:
            QMessageBox::information(this, tr("Tcp Server Client"), tr("The socket is bound to an address and port."));
            break;
        case QAbstractSocket::ClosingState:
            QMessageBox::information(this, tr("Tcp Server Client"), tr("The socket is about to close."));
            break;
        case QAbstractSocket::ListeningState:
            QMessageBox::information(this, tr("Tcp Server Client"), tr("The socket is listening."));
            break;
        default:
            QMessageBox::information(this, tr("Tcp Server Client"), tr("Unknown State."));
    }
}
*/
