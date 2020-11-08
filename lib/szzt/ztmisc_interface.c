/******************************************************************************
 	File		:  ztmisc_interface.c   
  Descript:  Misc devices driver code of ZT2312D platform.
  Modify	:  
  					 V1.00 Billy 14/01/15  draft.			
  Copyright (C), 2001-2008, Shenzhen Zhengtong Electronics Co.LTD
******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>
#include <termios.h> 
#include <linux/input.h>
#include "ztmisc_interface.h"

//arg
#define IO_CTL_LCD_BACKLIGHT 						0x0
#define IO_CTL_3G_RESET									0x1
#define IO_CTL_MCU_RESET                0x3
#define IO_CTL_3G_POWERON								0x4
#define IO_CTL_BUZZER										0x5
#define IO_CTL_USBHUB                   0x6
#define IO_CTL_LED                      0x7
#define IO_CTL_AUDIO_LEFT               0x8
#define IO_CTL_AUDIO_RIGHT              0x9
#define IO_CTL_LCD_DISP 								0xa
#define IO_CTL_AUDIO_ENABLE_LEFT 				0xb
#define IO_CTL_ZT606_RESET	 						0xc
#define IO_CTL_BARSCANNER_TRIG	 				0xd
#define IO_CTL_ADUIO_3G_SWITCH	 				0xe
#define IO_CTL_ADAPTER_STATUS		 				0xf

#define SET_IO_STATUS_LOW       				0x0
#define SET_IO_STATUS_HIGH              0x1 


/**********************************************************
Function£ºchar * ztmisc_version(void)
Descrip	£ºget driver version 
Return	£ºdriver version 
***********************************************************/
char * ztmisc_version(void)
{
	return ZTMISC_VERSION;
}

/**********************************************************
Function£ºint ztmisc_buzzer_control(unsigned int to_ms)
Descrip	£ºenable the Buzzer to beep.
Paramter£º
	to_ms : input, beep time in millisecond.
Return	£º
			=0: success
  		<0: fail
***********************************************************/
int ztmisc_buzzer_control(unsigned int to_ms)
{
	int ret    = -1;
	int fd     = -1;
	
	fd = open("/dev/gpio", O_RDWR);
	if (fd < 0)
	{
		return ZTRET_MISC_OPENDEV_ERROR;
	}
	
	ret=ioctl(fd,IO_CTL_BUZZER,1);
	usleep(1000*to_ms);
	ret=ioctl(fd,IO_CTL_BUZZER,0);
	
  close(fd);
	
	if(ret!=0)
	{
		return ret;	
	}
	
	return ZTRET_MISC_SUCCESS;		
}  


/**********************************************************
Function£ºint ztmisc_wcdma_control(unsigned int to_ms)
Descrip	£ºenable the Buzzer to beep.
Paramter£º
		code: input, operater code, 0x0=OFF, 0x1=ON, 0x2=Reset  
Return	£º
			=0: success
  		<0: fail
***********************************************************/
int ztmisc_wcdma_control(unsigned char code)
{
    return ZTRET_MISC_FUNC_UNSUPPORT;
}


/**********************************************************
Function£ºint ztmisc_zt606_reset(void)
Descrip	£ºreset ZT606 card reader module
Return	£º
			=0: success
  		<0: fail
***********************************************************/
int ztmisc_zt606_reset(void)
{
	int ret;
	int fd_gpio=-1;
	
	fd_gpio = open("/dev/gpio", O_RDWR);
	if (fd_gpio < 0)
	{
		return ZTRET_MISC_OPENDEV_ERROR;
	}
	
	ret=ioctl(fd_gpio,IO_CTL_MCU_RESET,0);
	if(ret!=0)
	{
		goto ERR_ZT606;
	}
	
	ret=ioctl(fd_gpio,IO_CTL_MCU_RESET,1);
	if(ret!=0)
	{
		goto ERR_ZT606;
	}
	usleep(10000);
	
	ret=ioctl(fd_gpio,IO_CTL_MCU_RESET,0);
	if(ret!=0)
	{
		goto ERR_ZT606;
	}
	
	
	close(fd_gpio);
	
	return ZTRET_MISC_SUCCESS;	
	
ERR_ZT606:
  close(fd_gpio);
	return ret;
}

/**********************************************************
Function£ºint ztmisc_light_enable(unsigned char enable)
Descrip	£ºenable the lights on the keyboard.
Paramter£º
	enable: input, 0x0 disable,0x1 enable;
Return	£º
			=0: success
  		<0: fail
***********************************************************/
int ztmisc_light_enable(bool enable)
{
	int ret    = -1;
	int fd     = -1;
	
	fd = open("/dev/gpio", O_RDWR);
	if (fd < 0)
	{
		return ZTRET_MISC_OPENDEV_ERROR;
	}
	
	if(enable)
		ret=ioctl(fd,IO_CTL_LED,1);
	else 
		ret=ioctl(fd,IO_CTL_LED,0);
	
  close(fd);
	
	if(ret!=0)
	{
		return ret;	
	}
	
	return ZTRET_MISC_SUCCESS;		
}

/**********************************************************
Function£ºint ztmisc_audio_output_enable(bool enable)
Descrip	£ºenable the lights on the keyboard.
Paramter£º
	enable: input, false disable,true enable;
Return	£º
			=0: success
  		<0: fail
***********************************************************/
int ztmisc_audio_output_enable(bool enable)  //0x0=audio disable 0x1=enable telephone audio 0x2=enable soundcard audio 
{
	int ret    = -1;
	int fd     = -1;
	
	fd = open("/dev/gpio", O_RDWR);
	if (fd < 0)
	{
		return ZTRET_MISC_OPENDEV_ERROR;
	}

	if(enable)
	{
		ret=ioctl(fd,IO_CTL_AUDIO_LEFT,1);
		ret=ioctl(fd,IO_CTL_AUDIO_RIGHT,1);
	}
	else
	{
		ret=ioctl(fd,IO_CTL_AUDIO_LEFT,0);
		ret=ioctl(fd,IO_CTL_AUDIO_RIGHT,0);
	}
	
  close(fd);
  
	return ZTRET_MISC_SUCCESS;
}

/**********************************************************
Function£ºint ztmisc_audio_output_enable(bool enable)
Descrip	£ºenable the lights on the keyboard.
Paramter£º
	enable: input, false disable,true enable;
Return	£º
			=0: success
  		<0: fail
***********************************************************/
int ztmisc_audio_chanal_select(bool enable)  //True: Sound card  False: 3G moudule
{
	int ret    = -1;
	int fd     = -1;
	
	fd = open("/dev/gpio", O_RDWR);
	if (fd < 0)
	{
		return ZTRET_MISC_OPENDEV_ERROR;
	}

	if(enable)  //sound card
	{
		ret=ioctl(fd,IO_CTL_AUDIO_ENABLE_LEFT,0);
		ret=ioctl(fd,IO_CTL_ADUIO_3G_SWITCH,1);
	}
	else				//3g module
	{
		ret=ioctl(fd,IO_CTL_AUDIO_ENABLE_LEFT,1);
		ret=ioctl(fd,IO_CTL_ADUIO_3G_SWITCH,0);
	}
	
  close(fd);
  
	return ZTRET_MISC_SUCCESS;
}

/**********************************************************
Function£ºint ztmisc_backlight_enable(bool enable)
Descrip	£ºenable the lights on the keyboard.
Paramter£º
	enable: input, 0x0 disable,0x1 enable;
Return	£º
			=0: success
  		<0: fail
***********************************************************/
int ztmisc_backlight_enable(bool enable)
{
	int ret    = -1;
	int fd     = -1;
	
	fd = open("/dev/gpio", O_RDWR);
	if (fd < 0)
	{
		return ZTRET_MISC_OPENDEV_ERROR;
	}
	
	if(enable)
		ret=ioctl(fd,IO_CTL_LCD_DISP,1);
	else 
		ret=ioctl(fd,IO_CTL_LCD_DISP,0);
	
  close(fd);
	
	if(ret!=0)
	{
		return ret;	
	}
	
	return ZTRET_MISC_SUCCESS;		
}  

/**********************************************************
Function£ºint ztmisc_sys_ver(char * pbt,char * pos ,char * pfs)
Descrip	£ºenable the lights on the keyboard.
Paramter£º
		 pbt: output, version of bootloader
		 pos: output, version of kernel
		 pfs: output, version of rootfs 
Return	£º
			=0: success
  		<0: fail
***********************************************************/
int ztmisc_sys_ver(char * pbt,char * pos ,char * pfs)
{
	FILE * stream;
	char tempbuff[256];
	char * ptrversion=NULL;
	
	//get bootloader version
	if(pbt != NULL)
	{
		memset(tempbuff,0x0,sizeof(tempbuff));
		stream = fopen("/proc/cmdline","r");
		fread(tempbuff,sizeof(tempbuff),sizeof(char),stream);
		ptrversion=strstr(tempbuff,"ZT2312D");
		if(ptrversion)
		{
				memcpy(pbt,ptrversion,18);
		}
		fclose(stream);
	}
	
	//get kernel os version
	if(pos != NULL)
	{
		memset(tempbuff,0x0,sizeof(tempbuff));
		stream = fopen("/proc/cpuinfo","r");
		fread(tempbuff,sizeof(tempbuff),sizeof(char),stream);
		ptrversion=strstr(tempbuff,"ZT2312D");
		if(ptrversion)
		{
				memcpy(pos,ptrversion,19);
		}
		fclose(stream);
	}
	
	//get rootfs version
	if(pfs != NULL)
	{
		memset(tempbuff,0x0,sizeof(tempbuff));
		stream = fopen("/etc/version","r");
		fread(tempbuff,sizeof(tempbuff),sizeof(char),stream);
		memcpy(pfs,tempbuff,strlen(tempbuff));
		fclose(stream);
	}

	return ZTRET_MISC_SUCCESS;
}

/**********************************************************
Function£ºint ztmisc_backlight_enable(bool enable)
Descrip	£ºenable the lights on the keyboard.
Paramter£º
	enable: input, 0x0 disable,0x1 enable;
Return	£º
			=1: Adapter insert
			=0: Adapter not insert 
  		<0: fail
***********************************************************/
int ztmisc_adapter_status(void)
{
 	int ret;
	unsigned int status=0x0;
	int fd_gpio=-1;

	fd_gpio = open("/dev/gpio", O_RDWR);
	if (fd_gpio < 0)
	{
		return ZTRET_MISC_OPENDEV_ERROR;
	}
	ret=ioctl(fd_gpio,IO_CTL_ADAPTER_STATUS,&status);
	if(ret<0)
	{
		return ret;	
	}
	
	close(fd_gpio);
	return !status;	
}

int set_com_attr(int fd, int bps)
{
	struct termios tios;  //????????
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
			return -1;
	}

	tcgetattr(fd, &tios);										        		//1???????(?????????)

 	tios.c_cflag     &= ~(CSIZE | CSTOPB | PARENB);			//3??????????
	tios.c_cflag     |= CS8 | CREAD | HUPCL | CLOCAL;		//4???????LINUX???????
	tios.c_iflag      = IGNBRK | IGNPAR;
	//tios.c_iflag      = 0;
	
	tios.c_oflag      = 0;
	tios.c_lflag      = 0;
	
	tios.c_cc[VMIN]   = 1;	//??????
	tios.c_cc[VTIME]  = 1;	//

	//speed= B19200;										                      //5??????

  cfsetospeed (&tios, speed);							                  //6????????
	cfsetispeed (&tios, speed);							                  //7????????

	if (tcsetattr(fd, TCSAFLUSH, &tios) < 0) 
		return -1;         //8?????????????

 	//??1-8??,1-4?????????,???????,
	//5-8?????

	modemcontrl = TIOCM_DTR;
	ioctl(fd,TIOCMBIS,&modemcontrl);
	modemcontrl = TIOCM_RTS;
	ioctl(fd,TIOCMBIS,&modemcontrl);

	return 0;
}
/**********************************************************
Function£ºint ztmisc_barcode_scan(unsigned char * barcode)
Descrip	£ºget barcode from the 2D scanner.
Paramter£º
barcode	: OUTPUT, barcode;
	to_ms	: INPUT, time out in millisecond
Return	£º
			=0: success
  		<0: fail
***********************************************************/
int ztmisc_barcode_scan(unsigned char * barcode,int to_ms)
{
	int ret    = -1;
	int fd_trig= -1;
	int fd_serial=-1;
	int nread;
	int counter=0x0;
	bool firsttime=true;
	char buff[512];
	int index=0;
	char temp;
	char command[2];
	
	struct termios opt;
	
	fd_trig = open("/dev/gpio", O_RDWR);
	if (fd_trig < 0)
	{
		return ZTRET_MISC_OPENDEV_ERROR;
	}
	
	fd_serial = open("/dev/ttyS3", O_RDWR|O_NONBLOCK);
 	if (-1 == fd_serial) 
 	{
 		return ZTRET_MISC_ERROR;	
	} 
	else
	{
		//get old config
		ret = set_com_attr(fd_serial,9600);
	}
	
	command[0]=0x1b;
	command[1]=0x31;
	write(fd_serial,command,2);

	while(1)
	{
		nread = read(fd_serial, &temp,1);
		if (nread > 0) 
		{
			printf("recv data = %02x\n",temp);
			break;
		}
		else
		{
			usleep(10000);
		}
		
		if((counter++) > 20)
		{
			printf("Hardware error!!!\n");
			return ZTRET_MISC_ERROR;
		}
	}
	
	memset(buff,0x0,sizeof(buff));
	
	while(1)
	{
		if(counter % 3000 == 0)
		{
			if(firsttime)
			{
				firsttime=false;
			}
			else
			{
				ret=ioctl(fd_trig,IO_CTL_BARSCANNER_TRIG,1);
				ret=ioctl(fd_trig,IO_CTL_BARSCANNER_TRIG,0);
			}
		} 
		
		usleep(1000);

		nread = read(fd_serial,&temp,1);
		if (nread > 0) 
		{
			buff[index]=temp;
			//printf("buff = %s\n",buff);
			index++;		
		}
		
		if(index>2)
		{
			if((buff[index-1]==0x0a)&&(buff[index-2]==0x0d))
			{
				memcpy(barcode,buff,index);
				break;
			}
		}

		if((counter++) >= to_ms)
		{
			break;		
		}
	}
	
	ioctl(fd_trig,IO_CTL_BARSCANNER_TRIG,1); //trun off the scanner
	close(fd_trig);
	close(fd_serial);
	
	if(strlen(buff) == 0x0)
		return ZTRET_MISC_TIMEOUT;
	else
		return ZTRET_MISC_SUCCESS;	
}
	

	

	
	
	