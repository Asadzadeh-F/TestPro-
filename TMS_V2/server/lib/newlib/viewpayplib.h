#ifndef _VIEWPAY_PRINTER_LIB_H
#define _VIEWPAY_PRINTER_LIB_H

#define VP_FAIL -1
#define VP_SUCCESS 0
#define VP_PORT_UNAVAILABLE 1
#define VP_RECOVER_PRINTER_FAIL 2
#define VP_GET_PRINTER_STATUS_FAIL 3
#define VP_READ_PORT_FAIL 4
#define VP_READ_TIME_OUT 5
#define VP_WRITE_PORT_FAIL 6
#define VP_WRITE_TIME_OUT 7
#define VP_OTHER_ERROR 8
#define VP_GETSTATUS_VIA_CONTROL 9 /*only used in USB interface while get printer's status*/

#define VP_BARCODE_WIDTH_ERR			10//������ȳ���
#define VP_BARCODE_HEIGHT_ERR			11//����߶ȳ���
#define VP_BARCODE_FONT_POS_ERR			12//����λ�ô���
#define VP_BARCODE_TEXT_FONT_TYPE_ERR	13//�������
#define VP_BARCODE_DATA_TYPE_ERR		14//�������ʹ��������˷���������
#define VP_BARCODE_DATA_FORMAT_ERR		15//���ݸ�ʽ�����磺����λ������
#define VP_BARCODE_CHKCODE_ERR			16//У�������
#define VP_BARCODE_NO_SPACE_ERR			17//���������븽����֮��û�пո�
#define VP_BARCODE_NO_BARCODE_ERR		18//û����������

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

extern unsigned long vpLastError;


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



#ifdef WIN32
#define VIEWPAY_API __declspec(dllexport) 
#else
#define VIEWPAY_API 
#endif


#ifdef __cplusplus
extern "C" {
#endif
VIEWPAY_API int SetCommSettings(int portType, int portNo, int baudRate, int flowControl);
VIEWPAY_API int OpenPort();
VIEWPAY_API void ClosePort();
VIEWPAY_API int GetStatus(unsigned long *pStatus);
VIEWPAY_API int CutPaper(int cutMode);
VIEWPAY_API int PrintText(unsigned char *textBuf, int length);
VIEWPAY_API int PrintGraph(const char* imageFileName);
VIEWPAY_API int LineFeed();
VIEWPAY_API int FeedPaper(int n);
VIEWPAY_API int PrintLogo(int logoNo, int printMode);
VIEWPAY_API int SetTextAttr(unsigned long attribute);
VIEWPAY_API int SetLeftMargin(int leftMargin);
VIEWPAY_API int SetPrinterEncodeMode(int mode);
VIEWPAY_API int SetPrinterCodePage(int codepage);
VIEWPAY_API int SetBarcode(int barHeight,int barWidth, enum BarcodeTextPos textPos, enum BarcodeTextType textFontType);
VIEWPAY_API int PrintBarcode(enum BarcodeType barcodeType, char* text );

#ifdef __cplusplus
}
#endif


#endif 
 