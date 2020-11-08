
#ifndef __VPCARD_H__
#define __VPCARD_H__

#define VPCARD_VERSION "VP606 V1.06.01"

#define VPCARD_USRCARD1    (0x11)      //用户接触卡1
#define VPCARD_USRCARD2    (0x12)      //用户接触卡2, 606不支持
#define VPCARD_USRCARD3    (0x13)      //用户接触卡3, 606不支持
#define VPCARD_RFID1       (0x21)      //用户非接卡1
#define VPCARD_RFID2       (0x22)      //用户非接卡2, 606不支持
#define VPCARD_RFID3       (0x23)      //用户非接卡3, 606不支持
#define VPCARD_PSAM1       (0x31)      //PSAM1
#define VPCARD_PSAM2       (0x32)      //PSAM2
#define VPCARD_PSAM3       (0x33)      //PSAM3, 606不支持
#define VPCARD_PSAM4       (0x34)      //PSAM4, 606不支持

#define VPCARD_AT_ST       (0x01)      //AT24C01、AT24C02、AT24C04、AT24C08、AT24C16、ST14C02C、ST14C04C、GPM1K、GPM2K、GPM4K
#define VPCARD_45D041      (0x02)      //45D041
#define VPCARD_SLE1        (0x03)      //SLE4418、SLE4428
#define VPCARD_SLE2        (0x04)      //SLE4432、SLE4442
#define VPCARD_AT2         (0x05)      //AT24C32/64
#define VPCARD_AT3         (0x06)      //93C46
#define VPCARD_AT4         (0x07)      //AT153
#define VPCARD_AT5         (0x08)      //AT102/AT1604
#define VPCARD_AT6         (0x09)      //AT1608

#define ERR_CARD_SUCCESS 	0x00
#define ERR_CARD_BASE   	(-1000)
#define ERR_CARD_OPEN            (ERR_CARD_BASE + (-1))   //打开串口失败
#define ERR_CARD_SENDCMD_FAIL    (ERR_CARD_BASE + (-2))   //发送失败
#define ERR_CARD_RHEAD           (ERR_CARD_BASE + (-3))   //读开始标志失败
#define ERR_CARD_DATA            (ERR_CARD_BASE + (-4))   //读数据失败
#define ERR_CARD_MAG23           (ERR_CARD_BASE + (-5))   //二三磁道错误
#define ERR_CARD_ICRET           (ERR_CARD_BASE + (-6))   //IC返回错误状态
#define ERR_CARD_UNKNOWN         (ERR_CARD_BASE + (-7))   //未知卡片
#define ERR_CARD_TYPE            (ERR_CARD_BASE + (-8))   //未知卡片
#define ERR_CARD_ACTIVE          (ERR_CARD_BASE + (-9))   //非接触用户卡激活失败
#define ERR_CARD_SLOT            (ERR_CARD_BASE + (-10))  //错误的卡座号
#define ERR_CARD_CMD             (ERR_CARD_BASE + (-11))  //命令执行失败
#define ERR_CARD_SETBPS          (ERR_CARD_BASE + (-12))  //设置波特率失败
#define ERR_CARD_HALT_ITMEOUT    (ERR_CARD_BASE + (-13))  //Halt超时
#define ERR_CARD_END             (ERR_CARD_BASE + (-14))  //未发现结束标记
#define VPCARD_RFID_HALT         (ERR_CARD_BASE + (-15))  //Halt失败
#define ERR_CARD_MAG123			 (ERR_CARD_BASE + (-16))  // 123磁道数据错误

#define VPCARD_NOT_SURPORT_CONTACT  (0x1)     //不支持接触式用户卡
#define VPCARD_NOT_POSITION         (0x2)     //接触式用户卡未插到位
#define VPCARD_CONTACT_POWERON      (0x3)     //接触式用户卡已上电
#define VPCARD_CONTACT_POWEROFF     (0x4)     //接触式用户卡未上电
#define VPCARD_NOT_SURPORT_PSAM     (0x5)     //不支持PSAM卡
#define VPCARD_PSAM_POWERON         (0x6)     //PSAM卡已上电
#define VPCARD_PSAM_POWEROFF        (0x7)     //PSAM卡未上电
#define VPCARD_CONTACT_POWERFAIL    (0x8)     //接触式用户卡上电失败
#define VPCARD_PSAM_POWERFAIL       (0x9)     //PSAM卡上电失败
#define VPCARD_NOT_SURPORT_RFID     (0x10)    //不支持非接
#define VPCARD_RFID_WAIT_TIMEOUT    (0x11)    //等待卡进入感应区超时
#define VPCARD_RFID_MORE            (0x12)    //有多张卡在感应区


extern char vpcard_version(char *version);
extern int  vpcard_open(int bps, char *port_str);
extern void vpcard_close();
extern int  vpcard_reset();
extern int  vpcard_firmware_version(char *cup_ver, char *bank_ver, char *fact_ver);
extern int  vpmag_card_check_open(int param);
extern int  vpmag_card_check_close();
extern int  vpmag_card_get_status();
extern int  vpmag_read2(char *mag2, char *mag3);
extern int  vpmag_read3(char *mag1,char *mag2, char *mag3);

extern int  vpcard_contact_sts(int card_no);   //获取接触式用户卡状态
extern int  vpcard_contact_poweron(int card_no, int delay_ms, char *ATR);
extern int  vpcard_contact_poweroff(int card_no);
extern int  vpcard_contact_set_cardtype(int card_no, int card_type);
extern int  vpcard_contact_get_cardtype(int card_no);
extern int  vpcard_contact_apdu(int card_no, char *inapdu, int inapdu_ln, char *outapdu, int *outapdu_ln);
extern int 	vpcard_ic_commu(char *cmd, char *out, int *out_ln, int timeout);

extern int  vpcard_psam_sts(int card_no);   //获取PSAM卡状态
extern int  vpcard_psam_poweron(int card_no, int delay_ms, char *ATR);
extern int  vpcard_psam_poweroff(int card_no);
extern int  vpcard_psam_set_cardtype(int card_no, int card_type);
extern int  vpcard_psam_get_cardtype(int card_no);
extern int  vpcard_psam_apdu(int card_no, char *inapdu, int inapdu_ln, char *outapdu, int *outapdu_ln);

extern int  vpcard_rfid_active(int delay_ms, char *card_type, char *uid, char *ATR);
extern int  vpcard_rfid_halt(int delay_ms);
extern int  vpcard_rfid_apdu(int card_no, char *inapdu, int inapdu_ln, char *outapdu, int *outapdu_ln);

extern void vpcard_rfid_antenna_close();
extern int vpcard_ic_read(int card_no, unsigned short addr, unsigned short len, unsigned char * data);
extern int vpcard_ic_pinauth(int card_no, unsigned char pinlen, unsigned char *pindata);
extern int vpcard_ic_write(int card_no, unsigned short addr, unsigned short len, unsigned char * data);

extern int vpcard_rfmem_active(int delay_ms, char *card_type, char *uid, char *ATR);
extern int vpcard_rfmem_halt(int delay_ms);
extern int vpcard_rfmem_pinauth( unsigned char sector,unsigned char pintype, unsigned char *pindata);
extern int vpcard_rfmem_read_block(unsigned char block,unsigned char * data);
extern int vpcard_rfmem_write_block(unsigned char block,unsigned char * data);

/* only been used for value block */
/* when use the following function to operation rfid memory card, 
 * you need to use "vpcard_rfmem_write_valueblock" to set the block to value block.
 */
extern int vpcard_rfmem_read_valueblock(unsigned char *block, unsigned char *data);
extern int vpcard_rfmem_write_valueblock(unsigned char *block, unsigned char *data);
extern int vpcard_rfmem_increment(unsigned char * block, unsigned char * data);
extern int vpcard_rfmem_decrement(unsigned char * block, unsigned char * data);
/* only been used for value block */
#endif  //__VPCARD_H__
