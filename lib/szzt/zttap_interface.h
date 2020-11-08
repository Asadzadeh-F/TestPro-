#ifndef __ZTTAP_INTERFACE_H__
#define __ZTTAP_INTERFACE_H__

#define ERR_ADXL345_BASE			(-10000)
#define ERR_ADXL345_OPEN            (ERR_ADXL345_BASE - 1)
#define ERR_ADXL345_OPEN_IOCTL      (ERR_ADXL345_BASE - 2)
#define ERR_WRITE_REG               (ERR_ADXL345_BASE - 3)
#define ERR_WRITE_DATA              (ERR_ADXL345_BASE - 4)
#define ERR_READ_DATA               (ERR_ADXL345_BASE - 5)
#define ERR_ADXL_READ_DEVID         (ERR_ADXL345_BASE - 6)
#define ERR_ADXL_READ_INT_ENABLE         (ERR_ADXL345_BASE - 7)
#define ERR_ADXL_READ_INT_SOURCE         (ERR_ADXL345_BASE - 8)
#define ERR_ADXL_READ_ACT_INACT_CTL       (ERR_ADXL345_BASE - 9)  
#define ERR_ADXL_WRITE_ACT_INACT_CTL       (ERR_ADXL345_BASE - 10) 

extern int adxl345_open();
extern int adxl345_close();
extern int adxl345_tap_init(void);
extern int adxl345_tap_adjust(char thresh);
extern int adxl345_tap_clear(void);
extern int adxl345_tap_detect(void);

#endif //__ZTTAP_INTERFACE_H__