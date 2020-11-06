#ifndef __ZT_ERROR_CODE__
#define __ZT_ERROR_CODE__




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


extern unsigned long ztLastError;


#endif //__ZT_ERROR_CODE__
