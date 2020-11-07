#ifndef PACKET_H
#define PACKET_H
#include <stdint.h>
#include <QObject>
#include <QMetaType>

#define FILE_SIZE (1024*1024*8)

#pragma pack(push, 1)

struct FilePacket
{
    uint32_t	totalFileBytesSize	   ;
    uint32_t	fileBytesSize 		   ;
    uint8_t     fileBytes[FILE_SIZE+1] ;
};

#pragma pack(pop)


//fffe5512....

Q_DECLARE_METATYPE(FilePacket)

Q_DECLARE_METATYPE(FilePacket*)

#endif // PACKET_H
