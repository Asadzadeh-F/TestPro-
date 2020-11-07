/*
* ����
*/
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cserial.h"



/*[method*******************************************************************************
 *def:  private int set_com_attr(int fd, int bps)
 *func:
        ���ô�������
 *args:
        #fd:       ��Ҫ���õĴ����ļ�������
        #bps:      ָ���Ĵ��ڲ�����
 *ret:
        0   �ɹ�,  <0 ʧ��
 *auth: coon 2011-10-28
*************************************************************************************]*/
int set_com_attrib(int fd, int bps)
{
	struct termios tios;  //�����ն����Խṹ
	int speed;
	int modemcontrl;

	memset(&tios, 0, sizeof(tios));

	switch (bps) {
		case 300:
			speed = B300;
			break;
		case 600:
			speed = B600;
			break;
		case 1200:
			speed = B1200;
			break;
		case 1800:
			speed = B1800;
			break;
		case 2400:
			speed = B2400;
			break;
		case 4800:
			speed = B4800;
			break;
		case 9600:
			speed = B9600;
			break;
		case 19200:
			speed = B19200;
			break;
		case 38400:
			speed = B38400;
			break;
		case 57600:
			speed = B57600;
			break;
		case 115200:
			speed = B115200;
			break;
		default:
			return ERR_CS_BPS;
	}

	tcgetattr(fd, &tios);										        		//1����ȡ�������ԣ�Ĭ�ϵĻ���ǰ���õģ�

 	tios.c_cflag     &= ~(CSIZE | CSTOPB | PARENB);			//3������Ϊ���ô�������
	tios.c_cflag     |= CS8 | CREAD | HUPCL | CLOCAL;		//4����������μ�LINUX���ն���������

	tios.c_iflag      = IGNBRK | IGNPAR;
	tios.c_oflag      = 0;
	tios.c_lflag      = 0;
	tios.c_cc[VMIN]   = 0;	//���ٿɶ�����
	tios.c_cc[VTIME]  = 1;	//�ȴ�����ʱ��,10��ı���

	//speed= B19200;										                      //5���µĲ�����

  	cfsetospeed (&tios, speed);							                  //6���������������
	cfsetispeed (&tios, speed);							                  //7���������벨����

	if (tcsetattr(fd, TCSAFLUSH, &tios) < 0) return ERR_CS_SETATTR;         //8��ʹ�������õ�����������Ч

 	//����1-8���У�1��4�������¿��Բ��ã�����Ĭ�ϵ����ã�
	//5��8һ��Ҫʹ��

	modemcontrl = TIOCM_DTR;
	ioctl(fd,TIOCMBIS,&modemcontrl);
	modemcontrl = TIOCM_RTS;
	ioctl(fd,TIOCMBIS,&modemcontrl);

	return 0;
}

/*[method*******************************************************************************
 *def:  public int cserial_init(CSerial *cs, char *dev, int bps)
 *func:
        ��ָ���Ĵ���
 *args:
        #cs:      ���ڿ��ƽṹ
 *ret:
        0   �ɹ�,  <0 ʧ��
 *auth: coon 2011-10-28
*************************************************************************************]*/
int cserial_init(CSerial *cs, char *dev, int bps)      //��ʼ��������Ϣ
{
	if( strlen(dev) > (sizeof(cs->com_port)-1) ) return ERR_CS_DEVLEN;

	cs->com_fd = -1;              //�ļ�������,-1��ʾû�й����豸
    cs->com_bps = bps;            //���ڲ�����
    cs->com_parity = 0;      //������żУ��
    cs->com_databit = 8;    //����λ
    cs->com_stopbit = 1;    //ֹͣλ
    memset(cs->com_port, 0 , sizeof(cs->com_port));   //�����豸�ļ�
    strcpy(cs->com_port, dev);

    return CS_SUCCESS;
}

/*[method*******************************************************************************
 *def:  public int cserial_open(CSerial *cs)
 *func:
        ��ָ���Ĵ���
 *args:
        #cs:      ���ڿ��ƽṹ
 *ret:
        0   �ɹ�,  <0 ʧ��
 *auth: coon 2011-10-28
*************************************************************************************]*/
int cserial_open(CSerial *cs)
{
	int ret;

	//��������Ѿ��򿪣����ȹرգ��ٴ�
	if( cs->com_fd != -1) close(cs->com_fd);

	//��ָ���Ĵ���
	cs->com_fd = open(cs->com_port, O_RDWR | O_SYNC);  //// | O_NDELAY |O_SYNC

	if( cs->com_fd < 0 )  return ERR_CS_OPEN_COM;

	//��������
    ret = set_com_attrib( cs->com_fd, cs->com_bps);

	return ret;
}

/*[method*******************************************************************************
 *def:  public void cserial_close(CSerial *cs)
 *func:
        �ر�ָ���Ĵ���
 *args:
        #cs:      ���ڿ��ƽṹ
 *ret:
        0   �ɹ�,  <0 ʧ��
 *auth: coon 2011-10-28
*************************************************************************************]*/
void cserial_close(CSerial *cs)
{
	if( cs->com_fd != -1) close(cs->com_fd);
	cs->com_fd = -1;
}

/*[method*******************************************************************************
 *def:  public void cserial_clear(CSerial *cs)
 *func:
        �����д����
 *args:
        #cs:      ���ڿ��ƽṹ
 *auth: coon 2011-10-28
*************************************************************************************]*/
void cserial_clear(CSerial *cs)
{
	tcflush ( cs->com_fd, TCIOFLUSH);
}

/*[method*******************************************************************************
 *def:  public int cserial_readData(CSerial *cs, char *s, int ln)
 *func:
        �Ӵ��ڶ�ȡָ����������, ����ʵ�ʶ��������ݳ��ȡ��˷�������������ʽ��ȡ��������,
        ������ڵ�ǰû������,���������ء�
 *args:
        #cs:      ���ڿ��ƽṹ
        #s:       ��ȡ�������ݴ洢�ռ�
        #ln:      ��Ҫ��ȡ�����ݳ���
 *ret:
        <0        ʧ��
        0<=       ���������ݳ���
 *auth: coon 2011-10-28
*************************************************************************************]*/
int cserial_readData(CSerial *cs, char *s, int ln)
{
	//tcflush ( cs->com_fd, TCIOFLUSH);	//modify by huanghao 2012.05.04
	int ready_read_ln=0, cur_read_ln;

	//if( cs->com_fd < 0) return ERR_CS_NOT_OPEN;
	//if( ln< 1)          return ERR_CS_READLN;
	if( cs->com_fd < 0) return 0;
	if( ln< 1)          return 0;

	while(1==1)
	{
		usleep(10);
		cur_read_ln = read( cs->com_fd, s+ready_read_ln, ln-ready_read_ln);
		//if( cur_read_ln < 0) return ERR_CS_READ;
		if( cur_read_ln<0) return 0;

		if(cur_read_ln == 0) return ready_read_ln;
		ready_read_ln = ready_read_ln + cur_read_ln;
	}
	return ready_read_ln;
}

/*[method*******************************************************************************
 *def:  public int cserial_writeData(CSerial *cs, char *s, int ln)
 *func:
        �򴮿ڷ�������,�ֶη���,ÿ�η���100�ֽ�����,ÿ��100�ֽ����ݺ�,�ȴ�100ms
 *args:
        #cs:      ���ڿ��ƽṹ
        #s:       ��ȡ�������ݴ洢�ռ�
        #ln:      ��Ҫ��ȡ�����ݳ���
 *ret:
        <0        ʧ��
        0<=       ʵ�ʷ��͵����ݳ���
 *auth: coon 2011-10-28
*************************************************************************************]*/
int cserial_writeData(CSerial *cs, char *s, int ln)
{
	//tcflush ( cs->com_fd, TCIOFLUSH);	//modify by huanghao 2012.05.04
	int ready_write_ln=0, cur_write_ln, tmp_ln;

	if( cs->com_fd < 0) return ERR_CS_NOT_OPEN;
	if( ln< 1)          return ERR_CS_WRITELN;

	while(1==1)
	{
		if( (ln-ready_write_ln) >100 )
			 cur_write_ln = 100;
		else cur_write_ln = ln - ready_write_ln;

		tmp_ln = write( cs->com_fd, s+ready_write_ln, cur_write_ln);
		if( tmp_ln < 0) return ERR_CS_WRITE;

		ready_write_ln = ready_write_ln + tmp_ln;
		if(ready_write_ln == ln) break;
		//usleep(100000);
		//usleep(1000);
	}
	return ready_write_ln;
}
