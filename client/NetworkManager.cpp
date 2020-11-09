#include "NetworkManager.h"
#include "QObject"
#include "QVariant"
#include "packet.h"

NetworkManager::NetworkManager(QObject* parent) : QTcpSocket(parent)
{

	//qRegisterMetaType(&PacketData);
	//client       = nullptr;
	//client       = new QTcpSocket(this);

	//connect(client,SIGNAL(connected()),this,SIGNAL(connected()));
	//connect(client,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
	//connect(client,SIGNAL(QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error)),this,&NetworkManager:: onErrorInConnection(QAbstractSocket::SocketError error));
	//connect(client,SIGNAL(&QTcpSocket::readyRead()),this,);
    connect(this, SIGNAL(readyRead()), this, SLOT(onReceiveData()));

}

NetworkManager::~NetworkManager()
{
	//SafeDelete(client) ;
}

/*
uint64_t NetworkManager::write(char* data, uint64_t size_t)
{
    return client->write(data,size_t);
}

bool NetworkManager::connectToServer(QString serverAddress, uint32_t port)
{
    client->connectToHost(serverAddress,port);
    return true;
}
*/

bool NetworkManager::sendErrorCode(ErrorCode errorcode)
{
	//log message depend on error type
	Packet packet;
	packet.header.command		= Error;
	packet.header.sizeOfPayload = sizeof(uint16_t);
	packet.data					= (char*) m_memoryBlock.getMemory(packet.header.sizeOfPayload);
	*packet.data				= errorcode;
	auto data					= packet.toBytes();
	return write(data);
}

bool NetworkManager::sendDetectionMessage(uint64_t serial, uint16_t thelastRCVIndex)
{
	Packet packet;
	packet.header.command		= Detection;
	packet.header.sizeOfPayload = sizeof(serial);
	packet.data					= (char*) m_memoryBlock.getMemory(packet.header.sizeOfPayload);
	*packet.data				= serial;
	packet.index				= thelastRCVIndex; //send the last Index received
	//packet.CRC
	auto data = packet.toBytes();
	return writePacket(data);
}

bool NetworkManager::sendAckMessage(int16_t index)
{
	Packet packet;
	if (index > 0)
	{
		packet.header.sizeOfPayload = sizeof(uint16_t);
		packet.header.command		= Acknowledgement;
		//packet.data                            = (char*)m_memoryBlock.getMemory(sizeof(uint16_t));
		//*packet.data                           = index;
		packet.data	 = 0;
		packet.index = index;
		//packet.CRC                             =  0;
	}
	else
	{
		if (index == 0)
		{
			packet.header.command		= FinalAcknowledgement;
			packet.header.sizeOfPayload = index;
			packet.data					= nullptr;
		}
	}
	auto data = packet.toBytes();
	return writePacket(data);
}

/*
void NetworkManager::onConnected()
{
    //emit connected();
}

void NetworkManager::onDisconnected()
{
    //emit disconnected();
}


void NetworkManager::onErrorInConnection(QAbstractSocket::SocketError error)
{
    emit errorOccurred();
}
*/

void NetworkManager::onReceiveData()
{
	emit readyData(readAll());
}

bool NetworkManager::writePacket(QByteArray& data)
{
	bool result = write(data) == data.size();
	return waitForBytesWritten() && result;
}
