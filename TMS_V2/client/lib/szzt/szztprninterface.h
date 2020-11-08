#ifndef SZZTPRNINTERFACE_H
#define SZZTPRNINTERFACE_H

#include <QObject>
#include "szztplib.h"
#ifdef SZZT_DEF

#define ZT_FAIL -1
#define ZT_SUCCESS 0
#define ZT_PORT_UNAVAILABLE 1
#define ZT_RECOVER_PRINTER_FAIL 2
#define ZT_GET_PRINTER_STATUS_FAIL 3
#define ZT_READ_PORT_FAIL 4
#define ZT_READ_TIME_OUT 5
#define ZT_WRITE_PORT_FAIL 6
#define ZT_WRITE_TIME_OUT 7
#define ZT_OTHER_ERROR 8
#define ZT_GETSTATUS_VIA_CONTROL 9 /*only used in USB interface while get printer's status*/

#define ZT_BARCODE_WIDTH_ERR			10//条码宽度超限
#define ZT_BARCODE_HEIGHT_ERR			11//条码高度超限
#define ZT_BARCODE_FONT_POS_ERR			12//文字位置错误
#define ZT_BARCODE_TEXT_FONT_TYPE_ERR	13//字体错误
#define ZT_BARCODE_DATA_TYPE_ERR		14//数据类型错误，输入了非数字数据
#define ZT_BARCODE_DATA_FORMAT_ERR		15//数据格式错误，如：数据位数错误
#define ZT_BARCODE_CHKCODE_ERR			16//校验码错误
#define ZT_BARCODE_NO_SPACE_ERR			17//条码文字与附加码之间没有空格
#define ZT_BARCODE_NO_BARCODE_ERR		18//没有输入条码

#define STATUS_BYTE1_BIT0 1
#define STATUS_BYTE1_BIT1 2
#define STATUS_BYTE1_BIT2 4
#define STATUS_BYTE1_OFFLINE 8
#define STATUS_BYTE1_BIT4 0x10
#define STATUS_BYTE1_BIT5 0x20
#define STATUS_BYTE1_FEED_BUTTON_PRESSED 0x40
#define STATUS_BYTE1_BIT7 0x80

#define STATUS_BYTE2_BIT0 1
#define STATUS_BYTE2_BIT1 2
#define STATUS_BYTE2_PLATEN_OPENED 4
#define STATUS_BYTE2_FEEDING_PAPER_BY_FEED_BUTTON 8
#define STATUS_BYTE2_BIT4 0x10
#define STATUS_BYTE2_PAPER_END_STOP 0x20
#define STATUS_BYTE2_HAVE_ERROR 0x40
#define STATUS_BYTE2_BIT7 0x80

#define STATUS_BYTE3_BIT0 1
#define STATUS_BYTE3_BIT1 2
#define STATUS_BYTE3_MACHANICAL_ERROR 4
#define STATUS_BYTE3_AUTOCUTTER_ERROR 8
#define STATUS_BYTE3_BIT4 0x10
#define STATUS_BYTE3_UNRECOVERABLE_ERROR 0x20
#define STATUS_BYTE3_RECOVERABLE_ERROR 0x40
#define STATUS_BYTE3_BIT7 0x80

//#define STATUS_BYTE4_BIT0 1
//#define STATUS_BYTE4_BIT1 2
#define STATUS_BYTE4_EJECTOR_HAVE_PAPER 1
#define STATUS_BYTE4_EJECTOR_JAMMING 2
#define STATUS_BYTE4_PAPER_NEAR_END 0xc
//#define STATUS_BYTE4_BIT3 8
#define STATUS_BYTE4_BIT4 0x10
#define STATUS_BYTE4_PAPER_END 0x60
//#define STATUS_BYTE4_BIT6 0x40
#define STATUS_BYTE4_BIT7 0x80

#define STATUS_OFFLINE     STATUS_BYTE1_OFFLINE
#define STATUS_PAPER_ERROR     (STATUS_BYTE4_PAPER_END << 24)
#define STATUS_PAPER_NE     (STATUS_BYTE4_PAPER_NEAR_END << 24)
#define STATUS_CUTTER_ERROR     (STATUS_BYTE3_AUTOCUTTER_ERROR << 16)
#define STATUS_PLATEN_ERROR     (STATUS_BYTE2_PLATEN_OPENED << 8)
#define STATUS_MECHANICAL_ERROR     (STATUS_BYTE3_MACHANICAL_ERROR << 16)
#define STATUS_AUTO_RECOVERABLE_ERROR     (STATUS_BYTE3_RECOVERABLE_ERROR << 16)
#define STATUS_UNRECOVERABLE_ERROR     (STATUS_BYTE3_UNRECOVERABLE_ERROR << 16)
#define STATUS_EJECTOR_JAMMING_ERROR  (STATUS_BYTE4_EJECTOR_JAMMING << 24)
#define STATUS_HAVE_ERROR     (STATUS_BYTE2_HAVE_ERROR << 8)

extern unsigned long ztLastError;


#define SERIAL_PORT 0
#define USB_PORT 1
#define NETWORK_PORT 2

#define FULL_CUT_FLAG 0
#define HALF_CUT_FLAG 1
#define BLACKMARK_CUT_FLAG 2

#define BAUDRATE_4800 4800
#define BAUDRATE_9600 9600
#define BAUDRATE_19200 19200
#define BAUDRATE_38400 38400
#define BAUDRATE_57600 57600
#define BAUDRATE_115200 115200

#define FLOW_CONTROL_NONE 0
#define FLOW_CONTROL_DSRDTR 1
#define FLOW_CONTROL_RTSCTS 2
#define FLOW_CONTROL_XONXOFF 3

#define PRINT_LOGO_ORIGNAL_SIZE 0
#define PRINT_LOGO_DOUBLE_WIDTH 1
#define PRINT_LOGO_DOUBLE_HEIGHT 2

#define TEXT_SMALL_FONT 1
#define TEXT_BOLD 8
#define TEXT_DOUBLE_HEIGHT 16
#define TEXT_DOUBLE_WIDTH 32
#define TEXT_UNDERLINE 128
#define TEXT_ROTATE90 0x100
#define TEXT_REVERSE 0x200
#define TEXT_UPSIDE_DOWN 0x400


enum BarcodeType {
    UPCA    ,
    UPCE    ,
    JAN13   ,
    JAN8    ,
    CODE39  ,
    ITF     ,
    CODABAR ,
    EAN13   ,
    EAN8    ,
    PDF417  ,
    CODE93  = 0x48,
    CODE128 = 0x49
};

enum BarcodeTextPos
{
    NO_TEXT,
    UP_SIDE,
    DOWN_SIDE,
    UP_DOWN_SIDE
};

enum BarcodeTextType
{
    NORMAL_FONT,
    SMALL_FONT
};
#endif



class SzztPrnInterface : public QObject
{
    Q_OBJECT
    
public:
    explicit SzztPrnInterface(QObject *parent = 0);
    ~SzztPrnInterface();
public:
    void PrnPrintArabic(const char *text,int size);

    //int SetCommSettings(int portType, int portNo, int baudRate, int flowControl);
    int PrnOpenPort();
    void PrnClosePort();
    int PrnGetStatus(unsigned long *pStatus);
    int PrnCutPaper(int cutMode);
    int PrnPrintText(unsigned char *textBuf, int length);
    int PrnPrintGraph(const char* imageFileName);
    int PrnLineFeed();
    int PrnFeedPaper(int n);
    int PrnPrintLogo(int logoNo, int printMode);
    int PrnSetTextAttr(unsigned long attribute);
    int PrnSetLeftMargin(int leftMargin);
    int PrnSetPrinterEncodeMode(int mode);
    int PrnSetPrinterCodePage(int codepage);
    int PrnSetBarcode(int barHeight,int barWidth, enum BarcodeTextPos textPos, enum BarcodeTextType textFontType);
    int PrnPrintBarcode(enum BarcodeType barcodeType, char* text );
    void PrnSetLineWidth(int linewidth);
private:
    unsigned long text_flag;
    int rightmargin;
    int _linewidth;
};

#endif // SZZTPRNINTERFACE_H
