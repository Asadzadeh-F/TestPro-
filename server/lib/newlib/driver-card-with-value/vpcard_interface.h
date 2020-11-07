
#ifndef __VPCARD_H__
#define __VPCARD_H__

#define VPCARD_VERSION "VP606 V1.06.01"

#define VPCARD_USRCARD1    (0x11)      //�û��Ӵ���1
#define VPCARD_USRCARD2    (0x12)      //�û��Ӵ���2, 606��֧��
#define VPCARD_USRCARD3    (0x13)      //�û��Ӵ���3, 606��֧��
#define VPCARD_RFID1       (0x21)      //�û��ǽӿ�1
#define VPCARD_RFID2       (0x22)      //�û��ǽӿ�2, 606��֧��
#define VPCARD_RFID3       (0x23)      //�û��ǽӿ�3, 606��֧��
#define VPCARD_PSAM1       (0x31)      //PSAM1
#define VPCARD_PSAM2       (0x32)      //PSAM2
#define VPCARD_PSAM3       (0x33)      //PSAM3, 606��֧��
#define VPCARD_PSAM4       (0x34)      //PSAM4, 606��֧��

#define VPCARD_AT_ST       (0x01)      //AT24C01��AT24C02��AT24C04��AT24C08��AT24C16��ST14C02C��ST14C04C��GPM1K��GPM2K��GPM4K
#define VPCARD_45D041      (0x02)      //45D041
#define VPCARD_SLE1        (0x03)      //SLE4418��SLE4428
#define VPCARD_SLE2        (0x04)      //SLE4432��SLE4442
#define VPCARD_AT2         (0x05)      //AT24C32/64
#define VPCARD_AT3         (0x06)      //93C46
#define VPCARD_AT4         (0x07)      //AT153
#define VPCARD_AT5         (0x08)      //AT102/AT1604
#define VPCARD_AT6         (0x09)      //AT1608

#define ERR_CARD_SUCCESS 	0x00
#define ERR_CARD_BASE   	(-1000)
#define ERR_CARD_OPEN            (ERR_CARD_BASE + (-1))   //�򿪴���ʧ��
#define ERR_CARD_SENDCMD_FAIL    (ERR_CARD_BASE + (-2))   //����ʧ��
#define ERR_CARD_RHEAD           (ERR_CARD_BASE + (-3))   //����ʼ��־ʧ��
#define ERR_CARD_DATA            (ERR_CARD_BASE + (-4))   //������ʧ��
#define ERR_CARD_MAG23           (ERR_CARD_BASE + (-5))   //�����ŵ�����
#define ERR_CARD_ICRET           (ERR_CARD_BASE + (-6))   //IC���ش���״̬
#define ERR_CARD_UNKNOWN         (ERR_CARD_BASE + (-7))   //δ֪��Ƭ
#define ERR_CARD_TYPE            (ERR_CARD_BASE + (-8))   //δ֪��Ƭ
#define ERR_CARD_ACTIVE          (ERR_CARD_BASE + (-9))   //�ǽӴ��û�������ʧ��
#define ERR_CARD_SLOT            (ERR_CARD_BASE + (-10))  //����Ŀ�����
#define ERR_CARD_CMD             (ERR_CARD_BASE + (-11))  //����ִ��ʧ��
#define ERR_CARD_SETBPS          (ERR_CARD_BASE + (-12))  //���ò�����ʧ��
#define ERR_CARD_HALT_ITMEOUT    (ERR_CARD_BASE + (-13))  //Halt��ʱ
#define ERR_CARD_END             (ERR_CARD_BASE + (-14))  //δ���ֽ������
#define VPCARD_RFID_HALT         (ERR_CARD_BASE + (-15))  //Haltʧ��
#define ERR_CARD_MAG123			 (ERR_CARD_BASE + (-16))  // 123�ŵ����ݴ���

#define VPCARD_NOT_SURPORT_CONTACT  (0x1)     //��֧�ֽӴ�ʽ�û���
#define VPCARD_NOT_POSITION         (0x2)     //�Ӵ�ʽ�û���δ�嵽λ
#define VPCARD_CONTACT_POWERON      (0x3)     //�Ӵ�ʽ�û������ϵ�
#define VPCARD_CONTACT_POWEROFF     (0x4)     //�Ӵ�ʽ�û���δ�ϵ�
#define VPCARD_NOT_SURPORT_PSAM     (0x5)     //��֧��PSAM��
#define VPCARD_PSAM_POWERON         (0x6)     //PSAM�����ϵ�
#define VPCARD_PSAM_POWEROFF        (0x7)     //PSAM��δ�ϵ�
#define VPCARD_CONTACT_POWERFAIL    (0x8)     //�Ӵ�ʽ�û����ϵ�ʧ��
#define VPCARD_PSAM_POWERFAIL       (0x9)     //PSAM���ϵ�ʧ��
#define VPCARD_NOT_SURPORT_RFID     (0x10)    //��֧�ַǽ�
#define VPCARD_RFID_WAIT_TIMEOUT    (0x11)    //�ȴ��������Ӧ����ʱ
#define VPCARD_RFID_MORE            (0x12)    //�ж��ſ��ڸ�Ӧ��


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

extern int  vpcard_contact_sts(int card_no);   //��ȡ�Ӵ�ʽ�û���״̬
extern int  vpcard_contact_poweron(int card_no, int delay_ms, char *ATR);
extern int  vpcard_contact_poweroff(int card_no);
extern int  vpcard_contact_set_cardtype(int card_no, int card_type);
extern int  vpcard_contact_get_cardtype(int card_no);
extern int  vpcard_contact_apdu(int card_no, char *inapdu, int inapdu_ln, char *outapdu, int *outapdu_ln);
extern int 	vpcard_ic_commu(char *cmd, char *out, int *out_ln, int timeout);

extern int  vpcard_psam_sts(int card_no);   //��ȡPSAM��״̬
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
