/******************************************************************************
  文 件 名:  ztmisc_interface.h
  当前版本:  V1.00.00  2013/04/07    
  功能描述:  ZT8120产品检测控制类设备驱动头文件，包括函数声明，错误
             返回值的宏定义，数据结构定义等内容。
  修改记录:  
  					 V1.00  Billy  2013/04/07  第一次编写		
  					 V1.01	Billy  2013/07/23  增加键盘声音控制接口。		
  Copyright (C), 2001-2008, Shenzhen Zhengtong Electronics Co.LTD
******************************************************************************/
#ifndef __ZTMISC_INTERFACE_H__
#define __ZTMISC_INTERFACE_H__

#define ZTMISC_VERSION "ZT2312D MISC V1.00"

#define ZTRET_MISC_SUCCESS           0
#define ZTRET_MISC_ERROR             -200
#define ZTRET_MISC_FUNC_UNSUPPORT    ZTRET_MISC_ERROR-1
#define ZTRET_MISC_OPENDEV_ERROR	 ZTRET_MISC_ERROR-2
#define ZTRET_MISC_PARA_ERROR	     ZTRET_MISC_ERROR-3
#define ZTRET_MISC_TIMEOUT		     ZTRET_MISC_ERROR-4

#ifdef __cplusplus
extern "C" {
#endif

extern char *ztmisc_version(void);
extern int ztmisc_wcdma_control(unsigned char code ); //0x0=OFF, 0x1=ON, 0x2=Reset 
extern int ztmisc_buzzer_control(unsigned int to_ms);
extern int ztmisc_zt606_reset(void);
extern int ztmisc_light_enable(bool enable);					//True: enable 			False: Disable
extern int ztmisc_audio_output_enable(bool enable);   //True: enable 			False: Disable
extern int ztmisc_audio_chanal_select(bool enable); 	//True: Sound card  False: 3G moudule
extern int ztmisc_backlight_enable(bool enable);  		//True: enable 			False: Disable
extern int ztmisc_barcode_scan(unsigned char * barcode,int to_s);
extern int ztmisc_sys_ver(char * pbt,char * pos ,char * pfs);
extern int ztmisc_adapter_status(void);

#ifdef __cplusplus
}
#endif

#endif  //_ZTLCD_INTERFACE_H__
