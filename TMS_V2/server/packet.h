#ifndef PACKET
#define PACKET
#include "stdint.h"

enum CommonType
{
    Detection = 0xD0, GetFile = 0xF0, Ack= 0xA0, EoFile = 0xF1, TransError = 0xE0, FinalAck = 0xA1
};
#pragma pack(push, 1)
struct packet
{
    uint8_t startByte ;
    uint16_t size;
    uint8_t CommonCode;
    uint8_t Footer ;
    uint8_t Payload[20];
};


#pragma pack(pop)

//Q_DECLARE_METATYPE(PacketData)

//Q_DECLARE_METATYPE(PacketData*)

#endif // PACKET

