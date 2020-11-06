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

#define ZT_BARCODE_WIDTH_ERR			10//�����ȳ���
#define ZT_BARCODE_HEIGHT_ERR			11//����߶ȳ���
#define ZT_BARCODE_FONT_POS_ERR			12//����λ�ô���
#define ZT_BARCODE_TEXT_FONT_TYPE_ERR	13//�������
#define ZT_BARCODE_DATA_TYPE_ERR		14//�������ʹ��������˷���������
#define ZT_BARCODE_DATA_FORMAT_ERR		15//���ݸ�ʽ�����磺����λ������
#define ZT_BARCODE_CHKCODE_ERR			16//У�������
#define ZT_BARCODE_NO_SPACE_ERR			17//���������븽����֮��û�пո�
#define ZT_BARCODE_NO_BARCODE_ERR		18//û����������


extern unsigned long ztLastError;


#endif //__ZT_ERROR_CODE__
