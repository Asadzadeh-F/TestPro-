#ifndef LIBSAM_H
#define LIBSAM_H

#include "libsam_global.h"
#define UINT8 unsigned char
#define UINT16 unsigned int
#define UINT32 unsigned long int
#define INT32 signed long int
class LIBSAMSHARED_EXPORT Libsam
{

public:
    Libsam();
    #include "Active_Header.h"

    enum Type_Protocol_SAM
    {
        SAM_Type_1 = 1,//ÔíÑÇÒ
        SAM_Type_2 = 2,//ÏÑæÏ
        SAM_Type_3 = 3,//íÒÏ
        SAM_Type_4 = 4,// ÓÇÑí Ïãæ
        SAM_Type_5 = 5,// Yazd Store
        SAM_Type_6 = 6,// Shiraz Store
        SAM_Type_7 = 7,// Yazd ATM
        SAM_Type_8 = 8,//  Shiraz ATM
    };

    enum Type_Key
    {
        Key_KeyA=0xAA,
        Key_KeyB=0xBB,
    };


    #define DoRud 0
    #define Sndj 1
    #define Yazd 2
    #define Shiraz 3

    // ICC_SAM1	0x11
    // ICC_SAM2	0x12
    // ICC_SAM3	0x13
    // ICC_SAM4	0x14

    // long ACK_INT32=0;
    // unsigned char Generate_Random[8];
    // int i_SAM=0;
    //////////////////////////////////////////////////////////ÊÚÑíÝ ÊæÇÈÚ
    INT32 Initial_SAM(INT32 CardSlot);
    INT32 Get_Key_A_Form_SAM(INT32 CardSlot,UINT8 SN_SAM[4],UINT8 Sector_Sam,UINT8 Cryptogram_SAM[8],UINT8 *Key_Out,UINT8 Type_City_In);
    INT32 Get_Key_B_Form_SAM(INT32 CardSlot,UINT8 SN_SAM[4],UINT8 Sector_Sam,UINT8 Cryptogram_SAM[8],UINT8 *Key_Out,UINT8 Type_City_In);


    int SAMInit(int CardSlot_In,unsigned char* _active_code_in);
    int SAMGetKey(int CardSlot_In,unsigned char SN_In[4],int Sector_In,unsigned char Cryptogram_In[8],unsigned char *Key_Out,enum Type_Protocol_SAM _Type_Protocol_SAM_In,enum Type_Key _Type_Key_In);
    int SAMGetKeyA(int CardSlot_In,unsigned char SN_In[4],unsigned char Sector_In,unsigned char Cryptogram_In[8],unsigned char *Key_Out,enum Type_Protocol_SAM _Type_Protocol_SAM_In);
    int SAMGetKeyB(int CardSlot_In,unsigned char SN_In[4],unsigned char Sector_In,unsigned char Cryptogram_In[8],unsigned char *Key_Out,enum Type_Protocol_SAM _Type_Protocol_SAM_In);
    int SAMGetKeyB_test(int CardSlot_In,unsigned char SN_In[4],unsigned char Sector_In,unsigned char Cryptogram_In[8],unsigned char *Key_Out,enum Type_Protocol_SAM _Type_Protocol_SAM_In);


};

#endif // LIBSAM_H
