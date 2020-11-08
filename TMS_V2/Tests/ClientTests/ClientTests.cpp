#include "ClientTests.h"
#include "NetworkProcess.h"
#include "QDebug"

void ClientTests::testIndexNetworkProcessing()
{
	Packet packet;
	//SimplePacket                 packet;
	packet.header.command		= GetFile;
	packet.header.sizeOfPayload = sizeof(uint8_t);
	packet.index				= 0;
	packet.data					= (char *) new uint64_t();
	*packet.data				= 100;
	m_networkProcess->processData(packet.toBytes());
	qDebug() << (int) m_networkProcess->currentProcessState();
	QCOMPARE(m_networkProcess->currentProcessState(),
			 NetworkProcess::ProcessPacketState::CleanProcess);
	SafeDelete(packet.data);
}

void ClientTests::initTestCase()
{
	m_networkProcess = new NetworkProcess(this);
	connect(m_networkProcess, SIGNAL(detectedPacket(QVariant)), this, SLOT(onGetPacket(QVariant)));

	qDebug() << "init Client TestCase";
}

void ClientTests::testStreamNetworkProcessing()
{
	Packet packet;
	packet.header.command		= Detection;
	packet.header.sizeOfPayload = sizeof(uint8_t);
	packet.data					= (char *) new (uint64_t);
	*packet.data				= 80;
	QByteArray bytes			= packet.toBytes();

    for (char* byte = bytes.begin(); byte != bytes.end(); ++byte)
	{
		QByteArray getBytes;
		getBytes += byte;
		m_networkProcess->processData(getBytes);
	}
	qDebug() << (int) m_networkProcess->currentProcessState();
	QCOMPARE(m_networkProcess->currentProcessState(),
			 NetworkProcess::ProcessPacketState::CleanProcess);
	SafeDelete(packet.data);
}

QTEST_MAIN(ClientTests)
