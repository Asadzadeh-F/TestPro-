#ifndef __CSERIAL_H__
#define __CSERIAL_H__

/* C接口的串口通用控制方法
   作者：coon 2011-10-28
*/

#define  CS_SUCCESS              (0)                   //成功
#define  ERR_CS_BASE             (-10150)              //错误号码基数
#define  ERR_CS_OPEN_COM         (ERR_CS_BASE + (-1))  //打开串口失败
#define  ERR_CS_NOT_OPEN         (ERR_CS_BASE + (-2))  //串口未打开
#define  ERR_CS_READLN           (ERR_CS_BASE + (-3))  //读长度指定错误
#define  ERR_CS_READ             (ERR_CS_BASE + (-4))  //读动作错误
#define  ERR_CS_WRITELN          (ERR_CS_BASE + (-5))  //写长度指定错误
#define  ERR_CS_WRITE            (ERR_CS_BASE + (-6))  //写动作错误
#define  ERR_CS_BPS              (ERR_CS_BASE + (-7))  //波特率指定错误
#define  ERR_CS_SETATTR          (ERR_CS_BASE + (-8))  //设置属性失败
#define  ERR_CS_DEVLEN           (ERR_CS_BASE + (-9))  //设备名超长

typedef struct
{
    int  com_fd;         //文件描述符,-1表示没有关联设备
    int  com_bps;        //串口波特率
    int  com_parity;     //串口奇偶校验
    int  com_databit;    //数据位
    int  com_stopbit;    //停止位
    char com_port[64];   //串口设备文件
}CSerial;

int  cserial_init(CSerial *cs, char *dev, int bps);      //初始化控制信息
int  cserial_open(CSerial *cs);                          //打开指定串口
void cserial_close(CSerial *cs);                         //关闭指定串口
void cserial_clear(CSerial *cs);                         //清除串口
int  cserial_readData(CSerial *cs, char *s, int ln);     //读串口数据
int  cserial_writeData(CSerial *cs, char *s, int ln);    //发送串口数据

#endif //__CSERIAL_H__
