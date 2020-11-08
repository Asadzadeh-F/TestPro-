/*
* 串口
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
        设置串口属性
 *args:
        #fd:       需要设置的串口文件描述符
        #bps:      指定的串口波特率
 *ret:
        0   成功,  <0 失败
 *auth: coon 2011-10-28
*************************************************************************************]*/
int set_com_attrib(int fd, int bps)
{
	struct termios tios;  //定义终端属性结构
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

	tcgetattr(fd, &tios);										        		//1、获取串口属性（默认的或以前设置的）

 	tios.c_cflag     &= ~(CSIZE | CSTOPB | PARENB);			//3、以下为设置串口属性
	tios.c_cflag     |= CS8 | CREAD | HUPCL | CLOCAL;		//4、具体意义参见LINUX的终端属性设置

	tios.c_iflag      = IGNBRK | IGNPAR;
	tios.c_oflag      = 0;
	tios.c_lflag      = 0;
	tios.c_cc[VMIN]   = 0;	//最少可读数据
	tios.c_cc[VTIME]  = 1;	//等待数据时间,10秒的倍数

	//speed= B19200;										                      //5、新的波特率

  	cfsetospeed (&tios, speed);							                  //6、设置输出波特率
	cfsetispeed (&tios, speed);							                  //7、设置输入波特率

	if (tcsetattr(fd, TCSAFLUSH, &tios) < 0) return ERR_CS_SETATTR;         //8、使以上设置的属性立即生效

 	//以上1-8步中，1－4大多情况下可以不用，即用默认的设置，
	//5－8一般要使用

	modemcontrl = TIOCM_DTR;
	ioctl(fd,TIOCMBIS,&modemcontrl);
	modemcontrl = TIOCM_RTS;
	ioctl(fd,TIOCMBIS,&modemcontrl);

	return 0;
}

/*[method*******************************************************************************
 *def:  public int cserial_init(CSerial *cs, char *dev, int bps)
 *func:
        打开指定的串口
 *args:
        #cs:      串口控制结构
 *ret:
        0   成功,  <0 失败
 *auth: coon 2011-10-28
*************************************************************************************]*/
int cserial_init(CSerial *cs, char *dev, int bps)      //初始化控制信息
{
	if( strlen(dev) > (sizeof(cs->com_port)-1) ) return ERR_CS_DEVLEN;

	cs->com_fd = -1;              //文件描述符,-1表示没有关联设备
    cs->com_bps = bps;            //串口波特率
    cs->com_parity = 0;      //串口奇偶校验
    cs->com_databit = 8;    //数据位
    cs->com_stopbit = 1;    //停止位
    memset(cs->com_port, 0 , sizeof(cs->com_port));   //串口设备文件
    strcpy(cs->com_port, dev);

    return CS_SUCCESS;
}

/*[method*******************************************************************************
 *def:  public int cserial_open(CSerial *cs)
 *func:
        打开指定的串口
 *args:
        #cs:      串口控制结构
 *ret:
        0   成功,  <0 失败
 *auth: coon 2011-10-28
*************************************************************************************]*/
int cserial_open(CSerial *cs)
{
	int ret;

	//如果串口已经打开，则先关闭，再打开
	if( cs->com_fd != -1) close(cs->com_fd);

	//打开指定的串口
	cs->com_fd = open(cs->com_port, O_RDWR | O_SYNC);  //// | O_NDELAY |O_SYNC

	if( cs->com_fd < 0 )  return ERR_CS_OPEN_COM;

	//设置属性
    ret = set_com_attrib( cs->com_fd, cs->com_bps);

	return ret;
}

/*[method*******************************************************************************
 *def:  public void cserial_close(CSerial *cs)
 *func:
        关闭指定的串口
 *args:
        #cs:      串口控制结构
 *ret:
        0   成功,  <0 失败
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
        清除读写缓存
 *args:
        #cs:      串口控制结构
 *auth: coon 2011-10-28
*************************************************************************************]*/
void cserial_clear(CSerial *cs)
{
	tcflush ( cs->com_fd, TCIOFLUSH);
}

/*[method*******************************************************************************
 *def:  public int cserial_readData(CSerial *cs, char *s, int ln)
 *func:
        从串口读取指定长度数据, 返回实际读到的数据长度。此方法采用阻塞方式读取串口数据,
        如果串口当前没有数据,则立即返回。
 *args:
        #cs:      串口控制结构
        #s:       读取到的数据存储空间
        #ln:      需要读取的数据长度
 *ret:
        <0        失败
        0<=       读到的数据长度
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
        向串口发送数据,分段发送,每次发送100字节数据,每发100字节数据后,等待100ms
 *args:
        #cs:      串口控制结构
        #s:       读取到的数据存储空间
        #ln:      需要读取的数据长度
 *ret:
        <0        失败
        0<=       实际发送的数据长度
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
