#ifndef SIMPLEPACKET_H
#define SIMPLEPACKET_H
#include "../Common/global.h"
#include <stdint.h>
#include <QMetaType>
#include <QObject>
#include "QCryptographicHash"

#pragma pack(push, 1)


#define nullptr 0
#define MinimumPacketSize 10
#define HeaderValue 0x7E
#define FooterValue 0x9E
#define SerialNumber 10

enum ErrorCode
{
	PacketLost = 1,
	HashMismatch,
	IndexMismatch,
	CompressCorrepted,
	OutOfSizeDisk,
	CantCopyFiles,
	UpdateError,
};

enum Command : uint8_t
{
    Detection            = 0xD0,
    GetFile              = 0xF0,
	//HashRequest          = 0xA0,
	EndOfFile			 = 0xF1,
	Acknowledgement		 = 0xA0,
	FinalAcknowledgement = 0xA1,
	Error				 = 0xE0,
	UpdateSuccess		 = 0xFF
};

struct Header
{
    uint8_t              header;
    uint32_t             sizeOfPayload;
    Command              command;
	Header()
	{
		header = HeaderValue;
	}
};

/*struct PacketWithIndexData
{
    Header	 header;
    char*	 data;
    uint8_t	 footer;

PacketWithIndexData()
{
	footer = FooterValue;
	data   = nullptr;
	index  = nullptr;
}

QByteArray toBytes()
{
	QByteArray bytes = QByteArray::fromRawData((char*) &header, sizeof(Header));
	bytes += QByteArray::fromRawData((char*) &index, sizeof (uint16_t));
	bytes += QByteArray::fromRawData(data, header.sizeOfPayload);
	bytes + QByteArray::fromRawData((char*) &footer, sizeof (uint8_t));
	return bytes;
}
};*/

class Packet
{

public:
	Packet();
    ~Packet();
    QByteArray  toBytes();
    uint8_t     getCRC();
    QByteArray  calcHash();


    Header      header;
    char*       data;
    uint16_t    index;
    uint8_t     CRC;
    uint8_t     footer;
    //PacketWithIndexData m_indexPacket;
    bool        mustFree;



};



#pragma pack(pop)
Q_DECLARE_METATYPE(Packet)

#endif // SIMPLEPACKET_H
