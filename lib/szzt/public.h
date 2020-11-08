#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <iconv.h>
#include <stdarg.h>

///////////////////////////////////////////////////////////////////////
/*
  ��һ���ֽڵ�8��λ����ʾ��־��¼����
  bit1: ������־, �κ�ʱ����Ҫ��¼
  bit2: ������ʾ��Ϣ
  bit3: �û���������1
  bit4: �û���������2
  bit5: �û���������3
  bit6: �û���������4
  bit7: Ӳ��ָ����Ϣ,һ������²������¼
  bit8: ��־��Ϣ�������׼���,ͬʱ��¼�ļ�
*/
#define LOG_FLAG_ERR             (0x01)         //������־,�κ�ʱ����Ҫ��¼
#define LOG_FLAG_BASE            (0x02)         //������ʾ��Ϣ
#define LOG_FLAG_USER1           (0x04)         //�û���������
#define LOG_FLAG_USER2           (0x08)         //�û���������
#define LOG_FLAG_USER3           (0x10)         //�û���������
#define LOG_FLAG_USER4           (0x20)         //�û���������
#define LOG_FLAG_HDCMD           (0x40)         //Ӳ��ָ����Ϣ,һ������²�Ҫ�����¼
#define LOG_FLAG_STDIO           (0x80)         //��־ͬʱ�������׼���
///////////////////////////////////////////////////////////////////////
/*
#define LOG_LEV_ERR              0            //������־�ȼ�
#define LOG_LEV_NORMAL           1            //������ʾ��Ϣ
#define LOG_LEV_IMPORTANT        2            //��Ҫ��ʾ��Ϣ
#define LOG_LEV_USER_3           3            //�û�����ȼ�1
#define LOG_LEV_USER_4           4            //�û�����ȼ�2
#define LOG_LEV_USER_5           5            //�û�����ȼ�3
#define LOG_LEV_USER_6           6            //�û�����ȼ�4
#define LOG_LEV_DEVCMD           7            //��¼һ���豸ָ��
#define LOG_LEV_ALL              8            //��¼������Ϣ
#define LOG_LEV_ALL_STDOUT       9            //��¼������Ϣ,ͬʱ�������׼���
*/

#define PUBLIC_SUCCESS           (0)
#define ERR_PUBLIC_BASE          (-10050)
#define ERR_PUBLIC_HEXLN         (ERR_PUBLIC_BASE + (-1))   //�����16�����ַ�������
#define ERR_PUBLIC_HEXCH         (ERR_PUBLIC_BASE + (-2))   //�Ƿ���16�����ַ�
#define ERR_PUBLIC_OPEN_FILE     (ERR_PUBLIC_BASE + (-3))   //���ļ�ʧ��
#define ERR_PUBLIC_FILENULL      (ERR_PUBLIC_BASE + (-4))   //�ļ�ָ��Ϊ��
#define ERR_PUBLIC_SPLIT_NUM     (ERR_PUBLIC_BASE + (-5))   //ָ���ķָ����Ŵ���
#define ERR_PUBLIC_LAST_POS      (ERR_PUBLIC_BASE + (-6))   //û���ҵ���Ľ���λ��
#define ERR_PUBLIC_FILE_CTL      (ERR_PUBLIC_BASE + (-7))   //�ļ�����ʧ��

void FormatStr(char *instr,char *outstr,int len,int filldirection,int fillchar);
void Yuan2Fen(char *instr,int len);
void Fen2Yuan(char *instr);
void loc_trim(const char *instr, char *outstr);
int  readIniFile(char *filename, char *key, char *value, int *len, char *def);
int  writeIniFile(char *filename, char *key, char *value);
int  getNField(char *instr, char *outstr,int n,int divch);
int  Hex2Dec(unsigned char *in, unsigned char *out, int flag);
void Dec2Hex(unsigned char *in, unsigned char *out, int ln);
void addEnter(char *src, int ln);
int  is_legal_ip(char *ip);
int  is_legal_ip2(char *ipstr, char ch);
void getDateTime(char *strdate,char *strtime);
unsigned char CRC(unsigned char *buffer, int count);
/////////////////////////////////////////////////////////////////////////



int  GetFileLength(char *filename);


void get_preNday(char *buf, int ndays, int format);
int  file_isexist(char *fname);
char *gSplitBcd(char *soustr, short len, char *desstr);
char *gCompressAsc(char *soustr, short len, char *desstr);
void getDateTime2(char *strdate,char *strtime);
void getDateTime3(char *strdate,char *strtime, char *strmsec);
int  get_cur_msec();

int  replaceStr(char* src, char* find_str, char* rep_str, int src_len, int find_len, int rep_len);

FILE *create_log_file(FILE *fp, char *dir, char *pre_name, int log_days);
//int  WriteLog(FILE *fp, int app_log_lev, int cur_log_lev, const char *format, ...);
int  WriteLog2(FILE *fp, char app_log_flag, char cur_log_flag, const char *format, ...);

#endif  //__PUBLIC_H__
