#include "zttap_interface.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/i2c.h>

#define ADXL345_ADDRESS		0x53  
#define I2C_RETRIES         0x0701
#define I2C_TIMEOUT         0x0702
#define I2C_SLAVE           0x0703       //IIC从器件的地址设置
#define I2C_BUS_MODE        0x0780
#define IO_CTL_TAP_CLERA 0x0

//ADXL345 reg
#define ADXL345_RA_DEVID            0x00
#define ADXL345_RA_RESERVED1        0x01
#define ADXL345_RA_THRESH_TAP       0x1D
#define ADXL345_RA_OFSX             0x1E
#define ADXL345_RA_OFSY             0x1F
#define ADXL345_RA_OFSZ             0x20
#define ADXL345_RA_DUR              0x21
#define ADXL345_RA_LATENT           0x22
#define ADXL345_RA_WINDOW           0x23
#define ADXL345_RA_THRESH_ACT       0x24
#define ADXL345_RA_THRESH_INACT     0x25
#define ADXL345_RA_TIME_INACT       0x26
#define ADXL345_RA_ACT_INACT_CTL    0x27
#define ADXL345_RA_THRESH_FF        0x28
#define ADXL345_RA_TIME_FF          0x29
#define ADXL345_RA_TAP_AXES         0x2A
#define ADXL345_RA_ACT_TAP_STATUS   0x2B
#define ADXL345_RA_BW_RATE          0x2C
#define ADXL345_RA_POWER_CTL        0x2D
#define ADXL345_RA_INT_ENABLE       0x2E
#define ADXL345_RA_INT_MAP          0x2F
#define ADXL345_RA_INT_SOURCE       0x30
#define ADXL345_RA_DATA_FORMAT      0x31
#define ADXL345_RA_DATAX0           0x32
#define ADXL345_RA_DATAX1           0x33
#define ADXL345_RA_DATAY0           0x34
#define ADXL345_RA_DATAY1           0x35
#define ADXL345_RA_DATAZ0           0x36
#define ADXL345_RA_DATAZ1           0x37
#define ADXL345_RA_FIFO_CTL         0x38
#define ADXL345_RA_FIFO_STATUS      0x39
static int gFd = -1;
static int tap_fd=-1;

/*
 *打开设备
 */
int adxl345_open()
{
    int iRet;
    
    tap_fd = open("/dev/tap", 0);
		if (tap_fd < 0) 
		{
			printf("open tap detect error\n");		
			return -1;
		}
    
    gFd = open("/dev/i2c/1", O_RDWR);
    if(gFd < 0)
    {
        printf("[%s] error\n", __func__);   
        return ERR_ADXL345_OPEN;
    }    
    
    iRet = ioctl(gFd, I2C_SLAVE, ADXL345_ADDRESS); 
    if(iRet < 0)
   	{
   		 close(gFd);
       return  ERR_ADXL345_OPEN_IOCTL;    
   	}
   	
    return 0;
}

/*
 *关闭设备
 */
int adxl345_close()
{
		close(tap_fd);
    close(gFd);
    return 0;
}

/*
 *写寄存器地址
 */
static int adxl345_i2c_write_reg(unsigned char p_cReg)
{
    int i;

    for(i = 0; i < 5; i++)
    {
        if(write(gFd, &p_cReg, 1) == 1)
        {
              return 0;
        }   
        usleep(100);              //延时100us     
    }     
    
    return ERR_WRITE_REG;
}

/*
 *从I2C器件读数据 
 */
static int adxl345_i2c_read(unsigned char p_cReg, unsigned char *p_cpData, unsigned int p_iNum)
{    
    if(adxl345_i2c_write_reg(p_cReg) != 0)
        return ERR_WRITE_REG;  
    
  
    if(read(gFd, p_cpData, p_iNum) != p_iNum)
    {    
        return ERR_READ_DATA;        
    }
    
    return 0;
}

/*
 *往I2C器件写数据 
 */
static int adxl345_i2c_write(unsigned char p_cReg, unsigned char p_cpData, unsigned int p_uiNum)
{
	
    int retries;
    unsigned char  data[2];

    data[0] = p_cReg;
    data[1] = p_cpData;
	
    for(retries=5; retries; retries--) 
	{
		if(write(gFd, data, p_uiNum)==p_uiNum)
		{
			return 0;
		}
        usleep(1000*10);
    }
    return ERR_WRITE_DATA;   
}

/*
 *读ID寄存器的值
 */
int adxl345_read_id(unsigned char *p_ucReg)
{
    int iRet;
    
    iRet = adxl345_i2c_read(ADXL345_RA_DEVID, p_ucReg, 1);
    if(iRet != 0)
    {
        printf("[%s] ret (%d). \n", __func__, iRet);
        return ERR_ADXL_READ_DEVID;
        
    }
    else
    {   
        printf("[%s]DEVID(0x00): 0x%02X. \n", __func__, *p_ucReg);   
        return 0;
    }
}

/*
 *读中断使能控制寄存器的值
 */
int adxl345_read_int_enable(unsigned char *p_ucReg)
{   
    if(!adxl345_i2c_read(ADXL345_RA_INT_ENABLE, p_ucReg, 1))
    {
        printf("[%s](0x2E): 0x%02X.\n", __func__,*p_ucReg); 
        return 0;  
    }
    else
    {
        return ERR_ADXL_READ_INT_ENABLE;
    }           
}

/*
 *读中断源寄存器的值
 */
int adxl345_read_int_source(unsigned char *p_ucpReg)
{   
    if(!adxl345_i2c_read(ADXL345_RA_INT_SOURCE, p_ucpReg, 1))
    {
        printf("[%s](0x30): 0x%02X.\n", __func__, *p_ucpReg); 
        return 0;  
    }
    else
    {
        return ERR_ADXL_READ_INT_SOURCE;
    }       
}

/*
 *读轴使能控制活动和静止检测寄存器的值
 */
int adxl345_read_act_inact_ctl(unsigned char *p_ucpReg)
{
    if(!adxl345_i2c_read(ADXL345_RA_ACT_INACT_CTL, p_ucpReg, 1))
    {
        printf("[%s](0x27): 0x%02X.\n", __func__, *p_ucpReg); 
        return 0;  
    }
    else
    {
        return ERR_ADXL_READ_ACT_INACT_CTL;
    }   
}

/*
 *写轴使能控制活动和静止检测寄存器的值
 */
int adxl345_write_act_inact_ctl(unsigned char p_ucData)
{   
    int iRet;
    
    iRet = adxl345_i2c_write(ADXL345_RA_ACT_INACT_CTL, p_ucData, 2);
    if(!iRet)
    {
        return 0;
    }   
    else
    {
        return ERR_ADXL_WRITE_ACT_INACT_CTL;
    }
}

int adxl345_read_thresh_tap(unsigned char *p_ucReg)
{
    int iRet;
    
    iRet = adxl345_i2c_read(ADXL345_RA_THRESH_TAP, p_ucReg, 1);
    if(iRet != 0)
    {
        printf("[%s] ret (%d). \n", __func__, iRet);
        return ERR_ADXL_READ_DEVID;
        
    }
    else
    {   
        printf("[%s]DEVID(0x00): 0x%02X. \n", __func__, *p_ucReg);   
        return 0;
    }
}

int adxl345_write_thresh_tap(unsigned char p_ucData)
{   
    int iRet;
    
    iRet = adxl345_i2c_write(ADXL345_RA_THRESH_TAP, p_ucData, 2);
    if(!iRet)
    {
        return 0;
    }   
    else
    {
        return ERR_ADXL_WRITE_ACT_INACT_CTL;
    }
}

int adxl345_tap_init(void)
{
	int ret;
	unsigned char p_ucpReg=0x0;
	
	printf("======================================================\n");
	
	ret = adxl345_i2c_write(ADXL345_RA_POWER_CTL, 0x00, 2);
  if(ret)
  {
      printf("ADXL345_RA_POWER_CTL ERROR!!\n");
  }   
  
  ret = adxl345_i2c_write(ADXL345_RA_POWER_CTL, 0x08, 2);
  if(ret)
  {
      printf("ADXL345_RA_POWER_CTL ERROR!!\n");
  }   	
	
  ret = adxl345_i2c_write(ADXL345_RA_THRESH_TAP, 0xFF, 2);
  if(ret)
  {
      printf("ADXL345_RA_THRESH_TAP ERROR!!\n");
  }   
  
  ret = adxl345_i2c_write(ADXL345_RA_DUR, 0x80, 2);
  if(ret)
  {
      printf("ADXL345_RA_DUR ERROR!!\n");
  }   
	
	ret = adxl345_i2c_write(ADXL345_RA_TAP_AXES, 0x0F, 2);
  if(ret)
  {
      printf("ADXL345_RA_TAP_AXES ERROR!!\n");
  }  
 
  	ret = adxl345_i2c_write(ADXL345_RA_INT_MAP, 0x00, 2);
  if(ret)
  {
      printf("ADXL345_RA_INT_MAP ERROR!!\n");
  }  
  
    	ret = adxl345_i2c_write(ADXL345_RA_INT_ENABLE, 0x40, 2);
  if(ret)
  {
      printf("ADXL345_RA_INT_ENABLE ERROR!!\n");
  }  
  
  
  sleep(1);
  
  if(!adxl345_i2c_read(ADXL345_RA_POWER_CTL, &p_ucpReg, 1))
  {
      printf("ADXL345_RA_POWER_CTL: 0x%02X.\n", p_ucpReg); 
  }
  
  
	  if(!adxl345_i2c_read(ADXL345_RA_THRESH_TAP, &p_ucpReg, 1))
  {
      printf("ADXL345_RA_THRESH_TAP: 0x%02X.\n", p_ucpReg); 
  }

  if(!adxl345_i2c_read(ADXL345_RA_DUR, &p_ucpReg, 1))
  {
      printf("ADXL345_RA_DUR: 0x%02X.\n", p_ucpReg); 
  }
  
  if(!adxl345_i2c_read(ADXL345_RA_TAP_AXES, &p_ucpReg, 1))
  {
      printf("ADXL345_RA_TAP_AXES: 0x%02X.\n", p_ucpReg); 
  } 
  
  if(!adxl345_i2c_read(ADXL345_RA_INT_ENABLE, &p_ucpReg, 1))
  {
      printf("ADXL345_RA_INT_ENABLE: 0x%02X.\n", p_ucpReg); 
  }
  
  if(!adxl345_i2c_read(ADXL345_RA_INT_MAP, &p_ucpReg, 1))
  {
      printf("ADXL345_RA_INT_MAP: 0x%02X.\n", p_ucpReg);  
  }
  
  printf("======================================================\n");
  
  return 0;
}

int adxl345_get_status(void)
{
	unsigned char p_ucpReg=0x0;
	
 	if(!adxl345_i2c_read(ADXL345_RA_INT_SOURCE, &p_ucpReg, 1))
  {
      //printf("ADXL345_RA_INT_SOURCE: 0x%02X.\n", p_ucpReg); 
  }
  
  if(!adxl345_i2c_read(ADXL345_RA_ACT_TAP_STATUS, &p_ucpReg, 1))
  {
      //printf("ADXL345_RA_ACT_TAP_STATUS: 0x%02X.\n", p_ucpReg); 
  }
  return 0;
}

int adxl345_tap_clear(void)
{
	int ret=0x0;
	unsigned char p_ucpReg=0x0;
	
	ret=ioctl(tap_fd,IO_CTL_TAP_CLERA,0x0 );
	if ( ret < 0) 
	{
		printf("read buttons error");
		return -1;
	}
	
 	if(!adxl345_i2c_read(ADXL345_RA_INT_SOURCE, &p_ucpReg, 1))
  {
      //printf("ADXL345_RA_INT_SOURCE: 0x%02X.\n", p_ucpReg); 
  }
  
  if(!adxl345_i2c_read(ADXL345_RA_ACT_TAP_STATUS, &p_ucpReg, 1))
  {
      //printf("ADXL345_RA_ACT_TAP_STATUS: 0x%02X.\n", p_ucpReg); 
  }
  
  return 0; 
}

int adxl345_tap_detect(void)
{
	int ret;
	int tapstate=0x0;
	
	ret=read(tap_fd, &tapstate, sizeof(tapstate)) ;
	if ( ret < 0) 
	{
		printf("read tapstate error");
		return -1;
	}
	
	return tapstate;
}

int adxl345_tap_thresh(char thresh)
{
	int ret;
  ret = adxl345_i2c_write(ADXL345_RA_THRESH_TAP, thresh, 2);
  if(ret)
  {
      printf("ADXL345_RA_THRESH_TAP ERROR!!\n");
  }   
	return ret;
}