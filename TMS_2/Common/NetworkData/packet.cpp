#include "packet.h"

Packet::Packet()
{
	footer	 = FooterValue;
	data	 = nullptr;
	CRC		 = 0;
	mustFree = false;
	index	 = 0;
}

Packet::~Packet()
{
	if (mustFree)
		SafeDelete(data);
}

QByteArray Packet::toBytes()
{
	Packet	   packet;
	QByteArray bytes = QByteArray::fromRawData((char*) &header, sizeof(Header));
	//if (packet.header.command == GetFile) // ToDo por shavad
	//{
	bytes += QByteArray::fromRawData((char*) &index, sizeof(uint16_t));
	//}
	bytes += QByteArray::fromRawData(data, header.sizeOfPayload);
	bytes += QByteArray::fromRawData((char*) &CRC, sizeof(CRC));
	bytes += QByteArray::fromRawData((char*) &footer, sizeof(uint8_t));
	return bytes;
}

uint8_t Packet::getCRC()
{
	QByteArray source = QByteArray::fromRawData((char*) data, header.sizeOfPayload);
	char	   crc	  = 0x00;

	foreach (auto c, source)
		crc ^= c;

	crc = 256 - crc;

	return crc;
}

QByteArray Packet::calcHash()
{
	QCryptographicHash hash(QCryptographicHash::Sha1);
	hash.addData(QByteArray::fromRawData(data, header.sizeOfPayload));
	return hash.result();
}
