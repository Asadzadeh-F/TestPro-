
#include "public.h"

/*[method*******************************************************************************
 *def:  public void getDateTime(char *strdate,char *strtime)
 *func:
        ȡ������ʱ�䵽ָ���ı�����.
 *output:
        #strdate ȡ�õ�����, YYYYMMDD��ʽ
        #strtime ȡ�õ��¼�, HHMMSS��ʽ
*************************************************************************************]*/
void getDateTime(char *strdate,char *strtime)
{
    time_t t;
    struct tm * tm;

    //ȡ������ʱ��
    time(&t);
    tm = localtime(&t);

	//������ʱ���ʽ��
    sprintf(strdate, "%04d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);
    sprintf(strtime, "%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
}

//**:**********************************************************************************
//**:���ܣ��ж����������Ƿ�Ϸ�
//**:���أ�0:��ȷ,  <0:����
//**:**********************************************************************************
int is_legal_ip(char *ip)
{
	char s1[16],s2[16],s3[16],s4[16],tmpip[17];
	int i=0, j=0, dotcnt=0;

	if(15<strlen(ip)) return -1;  //���ȴ���
	memset(s1, 0x00, sizeof(s1));
	memset(s2, 0x00, sizeof(s2));
	memset(s3, 0x00, sizeof(s3));
	memset(s4, 0x00, sizeof(s4));

	memset(tmpip, 0x00, sizeof(tmpip));
	strcpy(tmpip, ip);
	tmpip[strlen(tmpip)] = '.';   //���ڴ���,��������һ��'.'

	for(i=0; i<(int)strlen(tmpip); i++)
	{
		if(tmpip[i]=='.')   //�ҵ�һ��'.'
		{
			dotcnt++;
			switch(dotcnt)
			{
			case 1: memcpy(s1, tmpip+j, i-j);  break;
			case 2: memcpy(s2, tmpip+j, i-j);  break;
			case 3: memcpy(s3, tmpip+j, i-j);  break;
			case 4: memcpy(s4, tmpip+j, i-j);  break;
			default: return -2;  //��ʽ����
			}
			j = i+1;
		}
	}

	if(dotcnt!=4) return -3;  //��ʽ����
	if(strlen(s1)==0 || 255<atoi(s1)) return -4;   //���ݴ���
	if(strlen(s2)==0 || 255<atoi(s2)) return -5;   //���ݴ���
	if(strlen(s3)==0 || 255<atoi(s3)) return -6;   //���ݴ���
	if(strlen(s4)==0 || 255<atoi(s4)) return -7;   //���ݴ���

	return 0;
}

/*[method*******************************************************************************
 *def:  public int is_legal_ip2(char *ipstr, char ch)
 *func:
        �жϵ�ǰ������֮ǰ��������Ƿ�Ϸ���IP
 *args
 		ipstr	��������ַ���
 		ch		��ǰ����
 *ret
 		<0 ����, =0 �ǺϷ���IP
*************************************************************************************]*/
int is_legal_ip2(char *ipstr, char ch)
{
	char tmp[12],tmpip[20];
	int i=0, dotcnt=0;



    memset(tmpip, 0, sizeof(tmpip));
    sprintf(tmpip, "%s%c", ipstr, ch);

    //ͳ��С�������
    for(i=0; i<(int)strlen(tmpip); i++)
        if( tmpip[i] == '.' ) dotcnt++;
    if( 3 < dotcnt ) return -1;
    if( ch == '.' ) strcat(tmpip, "0");   //�����һ��С�����,��0�����жϴ���

    for(i=0; i<(int)strlen(tmpip); i++) //ͳ��С�������
        if( tmpip[i] == '.' )
			dotcnt++;
    if( 3 < dotcnt )
		return -1;
    if( ch == '.' )
		strcat(tmpip, "0");   //�����һ��С�����,��0�����жϴ���

    strcat(tmpip, ".");
    dotcnt++;

    for(i=0; i<dotcnt; i++)
    {
        memset(tmp, 0, sizeof(tmp));
        getNField(tmpip, tmp, i+1, '.');

        if( strlen(tmp) == 0 ) return -2;

		if(strlen(tmp)>3)
			return -3;

        if( i== 0 && (233 <= atoi(tmp) || atoi(tmp)==0) ) return -4;

        if( 255 < atoi(tmp) ) return -5;
    }
	if(dotcnt==4 && ch == 0)	//��ȷ�ϼ�,������ǰ�������,����С��������4��(��Ϊǰ�������һ��)
		return 1;
	return 0;
}

/*[method*******************************************************************************
 *def:  public void FormatStr(char *instr,char *outstr,int len,int filldirection,int fillchar)
 *func:
        ��ʽ���ַ���,��ָ���Ĳ��뷽ʽ���ַ������뵽ָ������.
        �ڲ���������ʱ��������,Դ��Ŀ�Ŀ�����ͬһ������.
        Դ��Ŀ�Ĳ��ܳ���1024�ֽڳ���.
 *args:
        #instr ��Ҫ������ַ���
        #divch �����Ľ��
        #len   �������Ҫ�ﵽ���ַ�������
        #filldirection ��䷽��. 1:ǰ��ָ���ַ�, 2:��ָ���ַ�, 3:ǰ��
        #fillchar �����ַ�
*************************************************************************************]*/
void FormatStr(char *instr,char *outstr,int len,int filldirection,int fillchar)
{
	char tmp[1024];
	int i,j,start=0;

	if(len > ((int)sizeof(tmp)-1)) return;

	for(j=0;j<len;j++)      //ȡ���ַ�������
		if(instr[j]==0) break;

	if(filldirection==1)    //ǰ��
		start = len-j;
	else if(filldirection==3)  //���Ҳ�
        start = (len-j+1)/2;

	memset(tmp, 0, sizeof(tmp));
	for(i=0;i<len;i++)      //��������ַ�
		tmp[i] = fillchar;
	    //outstr[i] = fillchar;

	for(i=0;i<j;i++)
		tmp[i+start] = instr[i];
		//outstr[i+start] = instr[i];
	memcpy(outstr, tmp, len);
	outstr[len]=0;

}

/*[method*******************************************************************************
 *def:  public void Fen2Yuan(char *instr)
 *func:
        ����תΪԪ��ʹ��ʱ�����뱣֤�����instr�������Ŀռ�����
        �ַ�����������������ܷ����ڴ�й©���⡣
 *args:
        #instr ת��ǰ��ֵ
 *output:
        #instr ת����Ľ��
 ע:ʹ��atoiת��strΪint ���ɱ�ʾ2147483648,zt579Ŀ¼�´˺���δ���޸�--��
*************************************************************************************]*/
void Fen2Yuan(char *instr)
{
	long int money1=0;
	int money2=0;
	int str_num;
	char *pos;
	char str_tmp[5]={0};
	int i;

	pos = strstr(instr,".");
	if(pos != NULL)  //�Ѿ���ԪΪ��λ
	{
		pos[0] = 0;
		if(pos[1] == 0) pos[1] = '0';
		if(pos[2] == 0) pos[2] = '0';
		pos[3] = 0;
		for(i=0;i<((int)strlen(instr)-2);i++)
		{
			memcpy(str_tmp,instr+i,1);
			str_num = atoi(str_tmp);
			money1 = money1*10+str_num;
		}
			money2 = atoi(pos+1);	//�Ƿ�
	}
	else
	{
		for(i=0;i<((int)strlen(instr)-2);i++)
		{
			memcpy(str_tmp,instr+i,1);
			str_num = atoi(str_tmp);
			money1 = money1*10+str_num;
		}
		if((int)strlen(instr)>=2)//Ԫ
		{
			memcpy(str_tmp,instr+(strlen(instr)-2),2);
			money2 = atoi(str_tmp);  //�Ƿ�
		}
		if((int)strlen(instr)==1)//Ԫ
		{
			memcpy(str_tmp,instr+(strlen(instr)-1),1);
			money2 = atoi(str_tmp);  //�Ƿ�
		}
	}
	sprintf(instr, "%ld.%02d",money1, money2);
}


/*[method*******************************************************************************
 *def:  public void Yuan2Fen(char *instr,int len)
 *func:
        ��ԪתΪ�֡����뱣֤�����㹻.
 *args:
        #instr ת��ǰ��ֵ
        #len   ����ֵ�ĳ���
 *output:
        #instr ת����Ľ��
*************************************************************************************]*/
void Yuan2Fen(char *instr,int len)
{
	int i;

	strcat(instr, "00");   //���2��0

	for(i=0;i<len;i++)      //�����תΪ��
    {
		if(instr[i]==46)  //�ҵ�С����.������λ��ǰ�ƣ�����������ֽ���Ϊ0
		{
			instr[i] = instr[i+1];
			instr[i+1] = instr[i+2];
			instr[i+2] = 0;
			break;
		}
	}
}

/*[method*******************************************************************************
 *def:  public void Hex2Dec(unsigned char *in, unsigned char *out)
 *func:
        ��16�����ַ���תΪ10�����ַ�: ��30313233תΪ0123,���㷨����ͬʱ����BCD��ѹ�㷨.
 *args:
        #in    16�����ַ���
        #ln    16�����ַ�������
        #flag  flag=1 ��߲���, flag=2 �ұ߲���
 *output:
        #out �������ַ���
*************************************************************************************]*/
int  Hex2Dec(unsigned char *in, unsigned char *out, int flag)
{
	int ch, i, ln;
	char *src;

	ln = strlen((char*)in);
	src = (char*)malloc(ln+2);
	memset(src, 0, ln+2);
	if( (ln % 2) != 0 )
	{
		if( flag == 2 )   //�Ҳ�
		{
			memcpy(src, in ,ln);
			src[ln] = '0';
			ln = ln + 1;
		}
		else
		{
			src[0] = '0';
			memcpy(src+1, in, ln);
			ln = ln + 1;
		}
	}
	//else
		//strcpy(src, in); //��ʾreturn commu(csZT8120, chSendBuf, 3, chRecvBuf, &iRcvLn, 0); ��ע�͵�

	ln = strlen((char*)src);
	for(i=0; i<ln; i++)
	{
		ch = (src[i]) & 0xFF;
		if('0'<=ch && ch<='9') continue;
		if('a'<=ch && ch<='f') continue;
		if('A'<=ch && ch<='F') continue;
		return 0;  //�зǷ����ַ�
	}
	for(i=0; i<ln/2; i++)
	{
		sscanf( (char*)(src+i*2), "%02X", &ch);
		out[i] = ch & 0xFF;
	}

	out[ln/2] = 0;   //����ַ�����������
	return (ln / 2);
}

/*[method*******************************************************************************
 *def:  public void Dec2Hex(unsigned char *in, unsigned char *out, int ln)
 *func:
        ��10�����ַ���תΪ16�����ַ�: ��0123תΪ30313233.,���㷨����ͬʱ����BCDѹ���㷨.
        Դ��Ŀ�Ŀ���ʹ��ͬһ������.
 *args:
        #in  10�����ַ���
        #ln  10�����ַ�������
 *output:
        #out �������ַ���
*************************************************************************************]*/
void Dec2Hex(unsigned char *in, unsigned char *out, int ln)
{
	int i;
	char *tmp_out;

	tmp_out = (char*)malloc((ln+1)*2);

	memset(tmp_out, 0, (ln+1)*2);
	for(i=0; i<ln; i++)
		sprintf(tmp_out+(i*2), "%02X", in[i] & 0xFF);

	memset(out, 0, ln*2+1);
	memcpy(out, tmp_out, ln*2);
	free(tmp_out);
}

/*[method*******************************************************************************
 *def:  public int getNField(char *instr, char *outstr,int n,int divch)
 *func:
        ��ָ���ַ������ҵ�ָ���ָ���ָ���ĵ�n�����ֵ,n��1��ʼ
 *args:
        #instr ����ǰ��ֵ
        #n     Ҫ�ҵ�����
        #divch ��ָ��ַ�
 *output:
        #outstr �����Ľ��
 *ret:
        >=0 �ҵ����ַ����ĳ���
        <0 δ�ҵ�ָ����
*************************************************************************************]*/
int getNField(char *instr, char *outstr,int n,int divch)
{
	int find_i=0;  //��¼�ҵ��ָ����Ĵ���
	int start_i=0,stop_i=-1;     //��¼�ҵ������ʼ��ַ
	int i;

	if(n<1) return ERR_PUBLIC_SPLIT_NUM;

	for(i=0;i<(int)(strlen(instr));i++)
	{
		if(instr[i]==divch)     find_i++;    //�ҵ�һ���ָ���
		if(find_i==n)    //���ҵ���Ҫȡ�õ���
		{
			stop_i = i-1;       //�������λ��
			break;
		}
		//��û��ָ����
		if(instr[i]==divch) //��ÿһ���ָ������������������ʼ��ַ
		{
			start_i = i+1;
			stop_i = i;     //��ʼʱ��ʼ���ý���λ��С����ʼλ��,���ڴ���
		}
		if(n==1 && (i+1)==(int)strlen(instr) && instr[i]!=divch)  //���û�зָ��,ȡ��һ��������
		{
			stop_i = i;
			break;
		}
	}

	//�������һ����֮��û�зָ��������
	if( find_i == n-1 )
        stop_i = strlen(instr);

	if((stop_i - start_i)<0)     return ERR_PUBLIC_LAST_POS;
	memcpy(outstr,instr+start_i,stop_i-start_i+1);
	return stop_i-start_i+1;
}

/*[method*******************************************************************************
 *def:  public void loc_trim(const char *instr, char *outstr)
 *func:
        ȥ��ָ���ַ���ǰ��Ŀո��TAB��,�س����з�,������ַ�������������.
        ��Ϊ�ڲ������˾ֲ�����ת��,����Դ��Ŀ�Ŀ�����ͬһ������
 *args:
        #instr ����ǰ��ֵ.
 *output:
        #outstr �����Ľ��.
*************************************************************************************]*/
void loc_trim(const char *instr, char *outstr)
{
    int start_i,stop_i, k, ln;
	char *tmp;

	ln = strlen(instr) + 2;
	tmp = (char*)malloc(ln);

	memset(tmp, 0x00, ln);
	strcpy(tmp,instr);

	start_i=0;  stop_i=strlen(tmp);
	//Ѱ�ҵ�һ���ǿ��ַ�
	for( k=0;k<(int)strlen(tmp);k++)
	{
		if(tmp[k]==0x20 || tmp[k]==0x09 || tmp[k]==0x0d || tmp[k]==0x0a)
			start_i = k+1;
		else
			break;
	}
	//�Ӻ���ǰѰ�ҵ�һ���ǿ��ַ�
	for( k=(int)strlen(tmp)-1; start_i<=k;k--)
		if(tmp[k]==0x20 || tmp[k]==0x09 || tmp[k]==0x0d || tmp[k]==0x0a)
			stop_i = k-1;
		else
			break;

	memcpy(outstr,tmp+start_i,stop_i-start_i+1);
	outstr[stop_i-start_i+1] = 0;

	free(tmp);
}

/*[method*******************************************************************************
 *def:  public int readIniFile(char *filename, char *key, char *value, int *len, char *def)
 *func:
        �������ļ��ض�ȡָ���ؼ��ֶ�Ӧ��ֵ.
        ÿ���ؼ��ֵ�ֵ���ܴ���500�ֽ�
 *args:
        #filename �����ļ�·��
        #key      �ؼ���
        #def      �ؼ��ֲ�����ʱ��Ĭ��ֵ
 *output:
        #value    ȡ����ֵ
        #len      ֵ�ĳ���
 *ret:
        =0 ����ɹ�
        ���� ����ʧ��
*************************************************************************************]*/
int readIniFile(char *filename, char *key, char *value, int *len, char *def)
{
		int ln1, ln2;
	char line[1024], tmp_key[64];
	char *pos;
	FILE *f;

	if((f = fopen(filename,"r"))==NULL)
	{
		memset(line, 0, sizeof(line));
		return ERR_PUBLIC_OPEN_FILE;      //���ļ�ʧ��
	}

	memset(line, 0x00, sizeof(line));
	while( fgets(line, sizeof(line)-1, f)!=0 )
	{
		//ȥ��ע��
		pos = strstr(line, "//");
		if(pos!=NULL) pos[0] = 0;

		//ȥ�����з�
		loc_trim(line, line);

		//�Ա�key
		memset(tmp_key, 0, sizeof(tmp_key));
		getNField(line, tmp_key, 1, '=');
		ln1 = strlen(tmp_key);
		loc_trim(tmp_key, tmp_key);
		if(strcmp(tmp_key, key)==0)
		{
			ln2 = strlen(line);
			*len = ln2 - ln1 - 1;
			memset(value, 0x00, *len+1);
			strcpy(value, line+ln1+1);
			fclose(f);
			return 0;
		}
	}
	fclose(f);

	//Ĭ��ֵ
	*len = strlen(def);
	strcpy(value, def);
	return 0;
}

/*[method*******************************************************************************
 *def:  public int writeIniFile(char *filename, char *key, char *value)
 *func:
        д�����ļ�, ˵��,�����ļ����ݲ��ܳ���4096���ֽ�
 *args:
        #filename �����ļ�·��
        #key      �ؼ���
        #value    �ؼ��ֵ�ֵ
 *ret:
        =0 ����ɹ�
        ���� ����ʧ��
*************************************************************************************]*/
int writeIniFile(char *filename, char *key, char *value)
{
	int ln1, ln2, buf_ln, flag=0;
	char line[1024], buf[4096], remark[1024];
	FILE *f;
	char *pos;

	if((f = fopen(filename,"r"))==NULL)
	{
		memset(line, 0, sizeof(line));
		return ERR_PUBLIC_OPEN_FILE;      //���ļ�ʧ��
	}

	ln1 = strlen(key);
	memset(line, 0x00, sizeof(line));
	memset(buf, 0x00, sizeof(buf));
	buf_ln = 0;

	while(fgets(line, sizeof(line)-1, f)!=NULL)
	{
		//ȥ�����з�
		ln2 = strlen(line);
		if(line[ln2-2]==0x0d && line[ln2-1]==0x0a)
		{
			line[ln2-2] = 0x00;
			line[ln2-1] = 0x00;
		}
		if(line[ln2-1]==0x0a) line[ln2-1]=0x00;

		//����ע��
		memset(remark, 0, sizeof(remark));
		pos = strstr(line, " ");
		if(pos!=NULL)
		{
			strcpy(remark, pos);
			pos[0] = 0;
		}

		//�Ա�key
		if(memcmp(line, key, ln1)==0 && line[strlen(key)]=='=')  //�����ָ���ؼ���
		{
			//��ӿ���key
			memcpy(buf+buf_ln, key, strlen(key));
			buf_ln = buf_ln + strlen(key);
			memcpy(buf+buf_ln, "=", 1);
			buf_ln++;
			//���value
			memcpy(buf+buf_ln, value, strlen(value));
			buf_ln = buf_ln + strlen(value);
			flag = 1;   //�����
		}
		else{    //����ָ���ؼ���,���뻺������
			memcpy(buf+buf_ln, line, strlen(line));
			buf_ln = buf_ln + strlen(line);
		}

		//���ע��
		memcpy(buf+buf_ln, remark, strlen(remark));
		buf_ln = buf_ln + strlen(remark);

		memcpy(buf+buf_ln, "\n", 1);
		buf_ln++;
	}
	fclose(f);

	if(flag==0)
	{
		memcpy(buf+buf_ln, key, strlen(key));
		buf_ln = buf_ln + strlen(key);
		memcpy(buf+buf_ln, "=", 1);
		buf_ln++;
		memcpy(buf+buf_ln, value, strlen(value));
		buf_ln = buf_ln + strlen(value);
		memcpy(buf+buf_ln, "\n", 1);
		buf_ln++;
	}

	if((f = fopen(filename,"w"))==NULL)    return ERR_PUBLIC_OPEN_FILE;      //���ļ�ʧ��
	fwrite(buf, 1, buf_ln, f);
	fflush(f);
	fclose(f);

	return 0;
}

/*[method*******************************************************************************
 *def:  public void addEnter(char *src, int ln)
 *func:
        ���ַ�����ָ��������ӻس����з�,Ҫ��ȷ������
*************************************************************************************]*/
void addEnter(char *src, int ln)
{
	int  chs_cnt=0, cnt=0, i, j;
	char tmp[1024]={0};
	unsigned char ch;

	i = 0;   //��¼Դ���ƶ�λ��
	j = 0;   //��¼��ǰ���ַ���
	cnt = 0; //��¼Ŀ���������ַ���

	while( i < (int)strlen(src))
	{
		ch = src[i++] & 0xff;
		tmp[cnt++] = ch;
		j++;
		if( 127 < ch )  chs_cnt++;   //�����ַ�
		if( j % ln == 0)   //��Ҫ��ӻ��з�
		{
			if( chs_cnt % 2 != 0)    //�������
			{
				ch = src[i++] & 0xff;
				tmp[cnt++] = ch;
			}
			tmp[cnt++] = '\r';
			tmp[cnt++] = '\n';
			j = 0;
		}
	}

	strcpy(src, tmp);
}

//////////////////////////////����Ϊ�����������//////////////////////////////////////


/*[method*******************************************************************************
 *def:  public int  WriteLog2(FILE *fp, char app_log_flag, char cur_log_flag, const char *format, ...)
 *func:
        ���ݸ�ʽ�ַ�����¼һ����־��ÿ���Զ��������ʱ�䡣
        ���з��ڸ�ʽ�ַ�����ָ��.
        �� cur_log_lev <= log_lev ʱ,���ܼ���־.
        �� cur_log_lev �� log_lev��Ϊ9ʱ,�����׼����ϴ����־��Ϣ.
 *args:
        #app_log_lev: Ӧ�õ���־�ȼ���־
        #fp:      ��־�ļ�ָ��
        #cur_log_lev: ��ǰ��Ϣ����־����
        #format:      ��ʽ�ַ���
 *ret:
        0   �ɹ�,  <0 ʧ��
*************************************************************************************]*/
int  WriteLog2(FILE *fp, char app_log_flag, char cur_log_flag, const char *format, ...)
{
    int  n;
    char dt[32]={0}, tmp[12];     //�������ʱ��
    unsigned char log_flag;                //��Ϻ����־��־
	va_list arg_ptr;              //�����������

	//��־�ļ�����,�򷵻�
	if(fp == NULL) return ERR_PUBLIC_FILENULL;

	//������־��־
	log_flag = ((app_log_flag & cur_log_flag) & 0xff);

	if( log_flag == 0x00) return 0;    //��ǰ��־�������¼

	//ȡ�ñ������,��ʼ�����������
    va_start(arg_ptr, format);

	//�����������׼���,�����������׼���
	if ((log_flag & LOG_FLAG_STDIO) == LOG_FLAG_STDIO)
	{
		vprintf(format, arg_ptr);        //������printf**
		if(log_flag == LOG_FLAG_STDIO)   //ֻ��Ҫ��ʾ,��ֻ��ʾ
		{
			va_end(arg_ptr);
			return 0;
		}
	}

	//ȡ������ʱ��
	getDateTime3(dt, (char*)(dt+11), (char*)(dt+20));
    //getDateTime2(dt, (char*)(dt+11));
    dt[10] = 32;   dt[19] = 32;   dt[23] = 32;

	//д����ʱ��
    fwrite(dt, 1, strlen(dt), fp);
    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "[%02X,%02X]", app_log_flag&0xff, cur_log_flag&0xff);
    fwrite(tmp, 1, strlen(tmp), fp);

    //����־����д����־�ļ�
    n = vfprintf(fp, format, arg_ptr);  //����ֵΪд����ֽ���

	//���������������
    va_end(arg_ptr);

	//ˢ���ļ�
    fflush(fp);

    return 0;
}

/*[method*******************************************************************************
 *def:  public FILE *create_log_file(char *dir, char *pre_name)
 *func:
        ����һ����־�ļ�,�����־�ļ��Ѿ�����,�򱣳�,����򿪻򴴽���־�ļ�,
        ��������־�ļ���Ϊdir/pre_nameyyyymmdd.log��ʽ
 *args:
        dir       ��־�ļ�Ŀ¼
        pre_name  ��־�ļ�ǰ����
        log_days  ��Ҫ���ֵ���־�ļ�����
*************************************************************************************]*/
FILE *create_log_file(FILE *fp, char *dir, char *pre_name, int log_days)
{
	char tmp[64];

	if(fp == 0)
	{
		//������ǰ��־�ļ�
		memset(tmp, 0,sizeof(tmp));
		sprintf(tmp, "%s/%s", dir, pre_name);
		get_preNday(tmp+strlen(tmp), 0, 2);    //ȡ�õ�ǰ����ʱ��
		strcat(tmp, ".log");

		fp = fopen(tmp, "a");  //��׷�ӷ�ʽ�򿪻򴴽�һ���ļ�,ȡ���ļ����
		//ɾ����ʷ��־�ļ�
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s/%s", dir, pre_name);
		get_preNday(tmp+strlen(tmp), log_days, 2);
		strcat(tmp, ".log");
		if(file_isexist(tmp) == 1)	remove(tmp);  //�ļ�����
	}

	return fp;
}



/*[method*******************************************************************************
 *def:  public unsigned char CRC(unsigned char *buffer, int count)
 *func:
        ����У����,���
*************************************************************************************]*/
unsigned char CRC(unsigned char *buffer, int count)
{
    unsigned char ch=0;
    int  i;

    for(i = 0; i < count; i++ )
	    ch = ch ^ (buffer[i]);

    return ch;
}


////
int get_cur_msec()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000 + tv.tv_usec / 1000;
}
/*[method*******************************************************************************
 *def:  public void getDateTime3(char *strdate,char *strtime, char *strmsec)
 *func:
        ȡ������ʱ�䵽ָ���ı�����.
 *output:
        #strdate ȡ�õ�����, YYYY-MM-DD��ʽ
        #strtime ȡ�õ��¼�, HH:MM:SS��ʽ
*************************************************************************************]*/
void getDateTime3(char *strdate,char *strtime, char *strmsec)
{
    time_t t;
    struct tm * tm;

    //ȡ������ʱ��
    time(&t);
    tm = localtime(&t);

	//������ʱ���ʽ��
    if(strdate != NULL) sprintf(strdate, "%04d-%02d-%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);
    if(strtime != NULL) sprintf(strtime, "%02d:%02d:%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	if(strmsec != NULL) sprintf(strmsec, "%03d", get_cur_msec() % 1000);
}

/*[method*******************************************************************************
 *def:  public void getDateTime2(char *strdate,char *strtime)
 *func:
        ȡ������ʱ�䵽ָ���ı�����.
 *output:
        #strdate ȡ�õ�����, YYYY-MM-DD��ʽ
        #strtime ȡ�õ��¼�, HH:MM:SS��ʽ
*************************************************************************************]*/
void getDateTime2(char *strdate,char *strtime)
{
    time_t t;
    struct tm * tm;

    //ȡ������ʱ��
    time(&t);
    tm = localtime(&t);

	//������ʱ���ʽ��
    sprintf(strdate, "%04d-%02d-%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);
    sprintf(strtime, "%02d:%02d:%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
}

//format: 1:yyyy-mm-ddģʽ, 2:yyyymmddģʽ
void get_preNday(char *buf, int ndays, int format)
{
	int year, mon, day;
	time_t          t;
	struct tm      *m;

	t = time(NULL) - ndays * 24 * 3600;
	m = localtime(&t);

	year = m->tm_year + 1900;
	mon  = m->tm_mon + 1;
	day  = m->tm_mday;

	if(format == 1)
		 sprintf(buf, "%04d-%02d-%02d", year, mon, day);
	else sprintf(buf, "%04d%02d%02d", year, mon, day);
}

//**:***************************************************************************
//**:���ܣ����ָ���ļ��Ƿ����
//**:���أ����ڣ�����1�������ڷ���0
//**:***************************************************************************
int file_isexist(char *fname)
{
	FILE *fp;
	fp = fopen(fname, "r+");
	if(fp==NULL)
		return 0;
	else
		return 1;
}

//**:***************************************************************************
//**:���ܣ�ȡ��ָ���ļ��Ĵ�С
//**:**************************************************************************/
int GetFileLength(char *filename)
{
	long len=0;
	int ret;
	FILE *fp;

	fp = fopen(filename, "r");
	if(fp == NULL)  return ERR_PUBLIC_OPEN_FILE;

	ret = fseek(fp, 0, SEEK_END);
	if(ret<0) return ERR_PUBLIC_FILE_CTL;

	len = ftell(fp);

	fclose(fp);
	return len;
}






//��һ��10�����ַ���չΪ2��16�����ַ���ʾ
char *gSplitBcd(char *soustr, short len, char *desstr)
{
    int i;
    char *pdes;

    strcpy(desstr,"");
    pdes = desstr;
    for( i=0; i<len; i++)
    {
        sprintf(pdes, "%02X", soustr[i]&0XFF );
        pdes += 2;
    }
    return desstr;
}
//��2��16���Ƶ��ַ�ѹ����һ��10�����ַ�
char *gCompressAsc(char *soustr, short len, char *desstr)
{
    int i, ch;
    char tmpstr[2049];

    sprintf(tmpstr, "%*.*sF",len,len,soustr);
    for( i=0;i<(len+1)/2;i++)
    {
        sscanf( tmpstr+i*2, "%02X", &ch);
        desstr[i] = ch & 0xFF;
    }
    desstr[i] = '\0';
    return desstr;
}

/*[method*******************************************************************************
 *def:  public int replaceStr(char* src, char* find_str, char* rep_str, int src_len, int find_len, int rep_len)
 *func:
        �������ַ����е�ָ���ַ���,��ָ�����ַ����滻.
        �ַ������Ȳ��ܴ���1024�ֽ�.
 *args:
        #src           Դ�ַ���
        #fine_str      ��Ҫ���ҵ��ַ���
        #rep_str       �����滻���ַ���
        #src_ln        Դ�ַ�������
        #find_len      ��Ҫ���ҵ��ַ����ĳ���
        #rep_len       �����滻���ַ����ĳ���
 *output:
        #src           �滻����ַ���
 *ret:
        >=0  �������ַ�������
*************************************************************************************]*/
int  replaceStr(char* src, char* find_str, char* rep_str, int src_len, int find_len, int rep_len)
{
	char tmp_buf[1024], tmp_buf1[1024];
	char* tmp_pos;
	int ret_ln, ln1, ln2, i_pos;

	memcpy(tmp_buf, src, src_len);
	ret_ln = src_len;

    i_pos = 0;
	while(1==1)
	{
		tmp_pos = strstr(tmp_buf+i_pos, find_str);
		if(tmp_pos == NULL) break;
		memset(tmp_buf1, 0, sizeof(tmp_buf1));
		ln1 = tmp_pos - tmp_buf;        //���Ҳ���֮ǰ�ĳ���
		ln2 = src_len - ln1 - strlen(find_str);   //���Ҳ���֮��ĳ���

		memcpy(tmp_buf1, tmp_buf, ln1);
		memcpy(tmp_buf1+ln1, rep_str, rep_len);
		memcpy(tmp_buf1+ln1+rep_len, tmp_buf+ln1+find_len, ln2);
		i_pos = ln1 + rep_len;    //�����Ѽ�ⲿ��,���������滻�Ĵ��а����б��滻�Ĵ��Ӷ������ڴ�����Ĵ���

		ret_ln = ln1+rep_len+ln2;
		memset(tmp_buf, 0,sizeof(tmp_buf));
		memcpy(tmp_buf, tmp_buf1, ret_ln);
		src_len = strlen(tmp_buf);     //�ǳ���Ҫ
	}

	memset(src, 0, ret_ln+1);
	memcpy(src, tmp_buf, ret_ln);
	return ret_ln;
}
