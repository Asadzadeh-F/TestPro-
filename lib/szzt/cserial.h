#ifndef __CSERIAL_H__
#define __CSERIAL_H__

/* C�ӿڵĴ���ͨ�ÿ��Ʒ���
   ���ߣ�coon 2011-10-28
*/

#define  CS_SUCCESS              (0)                   //�ɹ�
#define  ERR_CS_BASE             (-10150)              //����������
#define  ERR_CS_OPEN_COM         (ERR_CS_BASE + (-1))  //�򿪴���ʧ��
#define  ERR_CS_NOT_OPEN         (ERR_CS_BASE + (-2))  //����δ��
#define  ERR_CS_READLN           (ERR_CS_BASE + (-3))  //������ָ������
#define  ERR_CS_READ             (ERR_CS_BASE + (-4))  //����������
#define  ERR_CS_WRITELN          (ERR_CS_BASE + (-5))  //д����ָ������
#define  ERR_CS_WRITE            (ERR_CS_BASE + (-6))  //д��������
#define  ERR_CS_BPS              (ERR_CS_BASE + (-7))  //������ָ������
#define  ERR_CS_SETATTR          (ERR_CS_BASE + (-8))  //��������ʧ��
#define  ERR_CS_DEVLEN           (ERR_CS_BASE + (-9))  //�豸������

typedef struct
{
    int  com_fd;         //�ļ�������,-1��ʾû�й����豸
    int  com_bps;        //���ڲ�����
    int  com_parity;     //������żУ��
    int  com_databit;    //����λ
    int  com_stopbit;    //ֹͣλ
    char com_port[64];   //�����豸�ļ�
}CSerial;

int  cserial_init(CSerial *cs, char *dev, int bps);      //��ʼ��������Ϣ
int  cserial_open(CSerial *cs);                          //��ָ������
void cserial_close(CSerial *cs);                         //�ر�ָ������
void cserial_clear(CSerial *cs);                         //�������
int  cserial_readData(CSerial *cs, char *s, int ln);     //����������
int  cserial_writeData(CSerial *cs, char *s, int ln);    //���ʹ�������

#endif //__CSERIAL_H__
