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
}TIME_T;  // ����ʱ��

typedef struct gps_data
{ 
	char szLatitude[20];    //γ��(��)
	char cLatDirection;    //γ�ȷ��� N --- ���� S --- ��
	char szLongitude[20];  //����(��) E---��, W ---��
	char cLonDirection;    //���ȷ���	
	char cPosSta;		 //��λλ�õ���Ч��: V --��Ч�� A---��Ч
	double dSpeed;		 //�����ٶ�(��)  1��=1.852km/h
	double dCourse;      //���溽��(��, �ӱ���˳ʱ�����)	
	char satellite_num;  //���붨λ�����ǵ���Ŀ
	TIME_T time;
}GPS_DATA_T;			//GPS���ݽṹ��

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
 * ����ԭ��: int vpmisc_gps_get(GPS_DATA_T *pOutGps)
 * ��������: ��ȡGPS����
 * ����: pOutGps --- output,  ָ��洢GPS���ݵĴ洢�ռ�
 * ����ֵ: �ɹ�����0�� ʧ�ܷ��ش�����: ��ֵ 
 */
extern int vpmisc_gps_get(GPS_DATA_T *pOutGps);
/* 
 * ����ԭ��: int vpmisc_gps_open(void)
 * ��������: ��GPS
 * ����:
 * ����ֵ: �ɹ�����0�� ʧ�ܷ��ش�����:��ֵ
 * ��ע: �д򿪱������close�ر�gps
 */
extern int vpmisc_gps_open(void);
/* 
 * ����ԭ��: int vpmisc_gps_close(void)
 * ��������: �ر�GPS
 * ����:
 * ����ֵ: �ɹ�����0�� ʧ�ܷ��ش�����: ��ֵ
 */
extern int vpmisc_gps_close(void);
};
#endif  
