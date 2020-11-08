#ifndef __VPMISC_INTERFACE_H__
#define __VPMISC_INTERFACE_H__

#define VPMISC_VERSION "DS4500 MISC V1.02"

#define VPRET_MISC_SUCCESS           0
#define VPRET_MISC_ERROR             -200
#define VPRET_MISC_FUNC_UNSUPPORT    VPRET_MISC_ERROR-1
#define VPRET_MISC_OPENDEV_ERROR	 VPRET_MISC_ERROR-2
#define VPRET_MISC_PARA_ERROR	     VPRET_MISC_ERROR-3
#define VPRET_MISC_TIMEOUT		     VPRET_MISC_ERROR-4

#define  VPRET_MISC_INIT_GPS_SERIAL_FAIL	 VPRET_MISC_ERROR - 10
#define	 VPRET_MISC_GPS_GET_ERROR			 VPRET_MISC_ERROR - 12
#define  VPRET_MISC_GPS_OPEN_ERROR			 VPRET_MISC_ERROR - 13
#define  VPRET_MISC_GPS_CLOSE_ERROR			 VPRET_MISC_ERROR - 14
#define  VPRET_MISC_GPS_MALLOC_ERROR		 VPRET_MISC_ERROR - 15
#define  VPRET_MISC_GPS_CREATE_PTHREAD_ERROR VPRET_MISC_ERROR - 16
#define  VPRET_MISC_GPS_DET_PTHREAD_ERROR	 VPRET_MISC_ERROR - 17	
#define  VPRET_MISC_GPS_MUTEX_INIT_ERROR	 VPRET_MISC_ERROR - 18

typedef struct gps_time
{
	
	int  nHour;  		 
	int  nMinute;		 
	int  nSecond;        
	int  nMs;		 	
	int  nYear;
	int  nMonth;
	int  nDate;	
}TIME_T;  // 北京时间

typedef struct gps_data
{ 
	char szLatitude[20];    //纬度(度)
	char cLatDirection;    //纬度方向 N --- 北， S --- 南
	char szLongitude[20];  //经度(度) E---东, W ---西
	char cLonDirection;    //经度方向	
	char cPosSta;		 //定位位置的有效性: V --无效， A---有效
	double dSpeed;		 //地面速度(节)  1节=1.852km/h
	double dCourse;      //地面航向(度, 从北起顺时针计算)	
	char satellite_num;  //参与定位的卫星的数目
	TIME_T time;
}GPS_DATA_T;			//GPS数据结构体

extern "C"
{
extern char * vpmisc_version(void);
extern int vpmisc_wcdma_control(unsigned char code ); //0x0=OFF, 0x1=ON, 0x2=Reset 
extern int vpmisc_buzzer_control(unsigned int to_ms);
extern int vpmisc_zt606_reset(void);
extern int vpmisc_light_enable(bool enable);					//True: enable 			False: Disable
extern int vpmisc_audio_output_enable(bool enable);   //True: enable 			False: Disable
extern int vpmisc_audio_chanal_select(bool enable); 	//True: Sound card  False: 3G moudule
extern int vpmisc_backlight_enable(bool enable);  		//True: enable 			False: Disable
extern int vpmisc_barcode_scan(unsigned char * barcode,int to_s);
extern int vpmisc_sys_ver(char * pbt,char * pos ,char * pfs);
extern int vpmisc_adapter_status(void);

/* 
 * 函数原型: int vpmisc_gps_get(GPS_DATA_T *pOutGps)
 * 功能描述: 获取GPS数据
 * 参数: pOutGps --- output,  指向存储GPS数据的存储空间
 * 返回值: 成功返回0， 失败返回错误码: 负值 
 */
extern int vpmisc_gps_get(GPS_DATA_T *pOutGps);
/* 
 * 函数原型: int vpmisc_gps_open(void)
 * 功能描述: 打开GPS
 * 参数:
 * 返回值: 成功返回0， 失败返回错误码:负值
 * 备注: 有打开必须得有close关闭gps
 */
extern int vpmisc_gps_open(void);
/* 
 * 函数原型: int vpmisc_gps_close(void)
 * 功能描述: 关闭GPS
 * 参数:
 * 返回值: 成功返回0， 失败返回错误码: 负值
 */
extern int vpmisc_gps_close(void);
};
#endif  
