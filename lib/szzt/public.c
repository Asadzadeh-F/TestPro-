
#include "public.h"

/*[method*******************************************************************************
 *def:  public void getDateTime(char *strdate,char *strtime)
 *func:
        取得日期时间到指定的变量中.
 *output:
        #strdate 取得的日期, YYYYMMDD格式
        #strtime 取得的事件, HHMMSS格式
*************************************************************************************]*/
void getDateTime(char *strdate,char *strtime)
{
    time_t t;
    struct tm * tm;

    //取得日期时间
    time(&t);
    tm = localtime(&t);

	//将日期时间格式化
    sprintf(strdate, "%04d%02d%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);
    sprintf(strtime, "%02d%02d%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
}

//**:**********************************************************************************
//**:功能：判断网络设置是否合法
//**:返回：0:正确,  <0:错误
//**:**********************************************************************************
int is_legal_ip(char *ip)
{
	char s1[16],s2[16],s3[16],s4[16],tmpip[17];
	int i=0, j=0, dotcnt=0;

	if(15<strlen(ip)) return -1;  //长度错误
	memset(s1, 0x00, sizeof(s1));
	memset(s2, 0x00, sizeof(s2));
	memset(s3, 0x00, sizeof(s3));
	memset(s4, 0x00, sizeof(s4));

	memset(tmpip, 0x00, sizeof(tmpip));
	strcpy(tmpip, ip);
	tmpip[strlen(tmpip)] = '.';   //便于处理,在最后添加一个'.'

	for(i=0; i<(int)strlen(tmpip); i++)
	{
		if(tmpip[i]=='.')   //找到一个'.'
		{
			dotcnt++;
			switch(dotcnt)
			{
			case 1: memcpy(s1, tmpip+j, i-j);  break;
			case 2: memcpy(s2, tmpip+j, i-j);  break;
			case 3: memcpy(s3, tmpip+j, i-j);  break;
			case 4: memcpy(s4, tmpip+j, i-j);  break;
			default: return -2;  //格式错误
			}
			j = i+1;
		}
	}

	if(dotcnt!=4) return -3;  //格式错误
	if(strlen(s1)==0 || 255<atoi(s1)) return -4;   //数据错误
	if(strlen(s2)==0 || 255<atoi(s2)) return -5;   //数据错误
	if(strlen(s3)==0 || 255<atoi(s3)) return -6;   //数据错误
	if(strlen(s4)==0 || 255<atoi(s4)) return -7;   //数据错误

	return 0;
}

/*[method*******************************************************************************
 *def:  public int is_legal_ip2(char *ipstr, char ch)
 *func:
        判断当前输入与之前输入组合是否合法的IP
 *args
 		ipstr	已输入的字符串
 		ch		当前输入
 *ret
 		<0 错误, =0 是合法的IP
*************************************************************************************]*/
int is_legal_ip2(char *ipstr, char ch)
{
	char tmp[12],tmpip[20];
	int i=0, dotcnt=0;



    memset(tmpip, 0, sizeof(tmpip));
    sprintf(tmpip, "%s%c", ipstr, ch);

    //统计小数点个数
    for(i=0; i<(int)strlen(tmpip); i++)
        if( tmpip[i] == '.' ) dotcnt++;
    if( 3 < dotcnt ) return -1;
    if( ch == '.' ) strcat(tmpip, "0");   //在最后一个小数点后,加0便于判断处理

    for(i=0; i<(int)strlen(tmpip); i++) //统计小数点个数
        if( tmpip[i] == '.' )
			dotcnt++;
    if( 3 < dotcnt )
		return -1;
    if( ch == '.' )
		strcat(tmpip, "0");   //在最后一个小数点后,加0便于判断处理

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
	if(dotcnt==4 && ch == 0)	//是确认键,以满足前面的条件,并且小数点已是4个(因为前面添加了一个)
		return 1;
	return 0;
}

/*[method*******************************************************************************
 *def:  public void FormatStr(char *instr,char *outstr,int len,int filldirection,int fillchar)
 *func:
        格式化字符串,按指定的补齐方式将字符串补齐到指定长度.
        内部采用了临时变量处理,源和目的可以是同一个变量.
        源和目的不能超过1024字节长度.
 *args:
        #instr 需要处理的字符串
        #divch 处理后的结果
        #len   处理后需要达到的字符串长度
        #filldirection 填充方向. 1:前补指定字符, 2:后补指定字符, 3:前后补
        #fillchar 填充的字符
*************************************************************************************]*/
void FormatStr(char *instr,char *outstr,int len,int filldirection,int fillchar)
{
	char tmp[1024];
	int i,j,start=0;

	if(len > ((int)sizeof(tmp)-1)) return;

	for(j=0;j<len;j++)      //取得字符串长度
		if(instr[j]==0) break;

	if(filldirection==1)    //前补
		start = len-j;
	else if(filldirection==3)  //左右补
        start = (len-j+1)/2;

	memset(tmp, 0, sizeof(tmp));
	for(i=0;i<len;i++)      //设置填充字符
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
        将分转为元。使用时，必须保证传入的instr在其分配的空间中有
        字符串结束符，否则可能发生内存泄漏问题。
 *args:
        #instr 转换前的值
 *output:
        #instr 转换后的结果
 注:使用atoi转换str为int 最大可表示2147483648,zt579目录下此函数未做修改--金
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
	if(pos != NULL)  //已经是元为单位
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
			money2 = atoi(pos+1);	//角分
	}
	else
	{
		for(i=0;i<((int)strlen(instr)-2);i++)
		{
			memcpy(str_tmp,instr+i,1);
			str_num = atoi(str_tmp);
			money1 = money1*10+str_num;
		}
		if((int)strlen(instr)>=2)//元
		{
			memcpy(str_tmp,instr+(strlen(instr)-2),2);
			money2 = atoi(str_tmp);  //角分
		}
		if((int)strlen(instr)==1)//元
		{
			memcpy(str_tmp,instr+(strlen(instr)-1),1);
			money2 = atoi(str_tmp);  //角分
		}
	}
	sprintf(instr, "%ld.%02d",money1, money2);
}


/*[method*******************************************************************************
 *def:  public void Yuan2Fen(char *instr,int len)
 *func:
        将元转为分。必须保证长度足够.
 *args:
        #instr 转换前的值
        #len   传入值的长度
 *output:
        #instr 转换后的结果
*************************************************************************************]*/
void Yuan2Fen(char *instr,int len)
{
	int i;

	strcat(instr, "00");   //添加2个0

	for(i=0;i<len;i++)      //将金额转为分
    {
		if(instr[i]==46)  //找到小数点.将后两位数前移，并将后面的字节置为0
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
        将16进制字符串转为10进制字符: 如30313233转为0123,此算法可以同时用做BCD解压算法.
 *args:
        #in    16进制字符串
        #ln    16进制字符串长度
        #flag  flag=1 左边补齐, flag=2 右边补齐
 *output:
        #out 处理后的字符串
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
		if( flag == 2 )   //右补
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
		//strcpy(src, in); //提示return commu(csZT8120, chSendBuf, 3, chRecvBuf, &iRcvLn, 0); 先注释掉

	ln = strlen((char*)src);
	for(i=0; i<ln; i++)
	{
		ch = (src[i]) & 0xFF;
		if('0'<=ch && ch<='9') continue;
		if('a'<=ch && ch<='f') continue;
		if('A'<=ch && ch<='F') continue;
		return 0;  //有非法的字符
	}
	for(i=0; i<ln/2; i++)
	{
		sscanf( (char*)(src+i*2), "%02X", &ch);
		out[i] = ch & 0xFF;
	}

	out[ln/2] = 0;   //添加字符串结束符号
	return (ln / 2);
}

/*[method*******************************************************************************
 *def:  public void Dec2Hex(unsigned char *in, unsigned char *out, int ln)
 *func:
        将10进制字符串转为16进制字符: 如0123转为30313233.,此算法可以同时用做BCD压缩算法.
        源和目的可以使用同一个变量.
 *args:
        #in  10进制字符串
        #ln  10进制字符串长度
 *output:
        #out 处理后的字符串
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
        从指定字符串中找到指定分割符分割出的第n个域的值,n从1开始
 *args:
        #instr 处理前的值
        #n     要找的域编号
        #divch 域分割字符
 *output:
        #outstr 处理后的结果
 *ret:
        >=0 找到的字符串的长度
        <0 未找到指定域
*************************************************************************************]*/
int getNField(char *instr, char *outstr,int n,int divch)
{
	int find_i=0;  //记录找到分隔符的次数
	int start_i=0,stop_i=-1;     //记录找到域的起始地址
	int i;

	if(n<1) return ERR_PUBLIC_SPLIT_NUM;

	for(i=0;i<(int)(strlen(instr));i++)
	{
		if(instr[i]==divch)     find_i++;    //找到一个分隔符
		if(find_i==n)    //已找到需要取得的域
		{
			stop_i = i-1;       //置域结束位置
			break;
		}
		//还没到指定域
		if(instr[i]==divch) //在每一个分隔符处，重新置域的起始地址
		{
			start_i = i+1;
			stop_i = i;     //初始时，始终让结束位置小于起始位置,便于处理
		}
		if(n==1 && (i+1)==(int)strlen(instr) && instr[i]!=divch)  //针对没有分割符,取第一个域的情况
		{
			stop_i = i;
			break;
		}
	}

	//处理最后一个域之后没有分隔符的情况
	if( find_i == n-1 )
        stop_i = strlen(instr);

	if((stop_i - start_i)<0)     return ERR_PUBLIC_LAST_POS;
	memcpy(outstr,instr+start_i,stop_i-start_i+1);
	return stop_i-start_i+1;
}

/*[method*******************************************************************************
 *def:  public void loc_trim(const char *instr, char *outstr)
 *func:
        去除指定字符串前后的空格和TAB键,回车换行符,处理的字符串长度无限制.
        因为内部采用了局部变量转换,所以源和目的可以是同一个变量
 *args:
        #instr 处理前的值.
 *output:
        #outstr 处理后的结果.
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
	//寻找第一个非空字符
	for( k=0;k<(int)strlen(tmp);k++)
	{
		if(tmp[k]==0x20 || tmp[k]==0x09 || tmp[k]==0x0d || tmp[k]==0x0a)
			start_i = k+1;
		else
			break;
	}
	//从后向前寻找第一个非空字符
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
        从配置文件重读取指定关键字对应的值.
        每个关键字的值不能大于500字节
 *args:
        #filename 配置文件路径
        #key      关键字
        #def      关键字不存在时的默认值
 *output:
        #value    取到的值
        #len      值的长度
 *ret:
        =0 处理成功
        其它 处理失败
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
		return ERR_PUBLIC_OPEN_FILE;      //打开文件失败
	}

	memset(line, 0x00, sizeof(line));
	while( fgets(line, sizeof(line)-1, f)!=0 )
	{
		//去掉注释
		pos = strstr(line, "//");
		if(pos!=NULL) pos[0] = 0;

		//去除换行符
		loc_trim(line, line);

		//对比key
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

	//默认值
	*len = strlen(def);
	strcpy(value, def);
	return 0;
}

/*[method*******************************************************************************
 *def:  public int writeIniFile(char *filename, char *key, char *value)
 *func:
        写配置文件, 说明,配置文件数据不能超过4096个字节
 *args:
        #filename 配置文件路径
        #key      关键字
        #value    关键字的值
 *ret:
        =0 处理成功
        其它 处理失败
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
		return ERR_PUBLIC_OPEN_FILE;      //打开文件失败
	}

	ln1 = strlen(key);
	memset(line, 0x00, sizeof(line));
	memset(buf, 0x00, sizeof(buf));
	buf_ln = 0;

	while(fgets(line, sizeof(line)-1, f)!=NULL)
	{
		//去除换行符
		ln2 = strlen(line);
		if(line[ln2-2]==0x0d && line[ln2-1]==0x0a)
		{
			line[ln2-2] = 0x00;
			line[ln2-1] = 0x00;
		}
		if(line[ln2-1]==0x0a) line[ln2-1]=0x00;

		//保留注释
		memset(remark, 0, sizeof(remark));
		pos = strstr(line, " ");
		if(pos!=NULL)
		{
			strcpy(remark, pos);
			pos[0] = 0;
		}

		//对比key
		if(memcmp(line, key, ln1)==0 && line[strlen(key)]=='=')  //如果是指定关键字
		{
			//添加可用key
			memcpy(buf+buf_ln, key, strlen(key));
			buf_ln = buf_ln + strlen(key);
			memcpy(buf+buf_ln, "=", 1);
			buf_ln++;
			//添加value
			memcpy(buf+buf_ln, value, strlen(value));
			buf_ln = buf_ln + strlen(value);
			flag = 1;   //已添加
		}
		else{    //不是指定关键字,存入缓冲区中
			memcpy(buf+buf_ln, line, strlen(line));
			buf_ln = buf_ln + strlen(line);
		}

		//添加注释
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

	if((f = fopen(filename,"w"))==NULL)    return ERR_PUBLIC_OPEN_FILE;      //打开文件失败
	fwrite(buf, 1, buf_ln, f);
	fflush(f);
	fclose(f);

	return 0;
}

/*[method*******************************************************************************
 *def:  public void addEnter(char *src, int ln)
 *func:
        将字符串按指定长度添加回车换行符,要正确处理汉字
*************************************************************************************]*/
void addEnter(char *src, int ln)
{
	int  chs_cnt=0, cnt=0, i, j;
	char tmp[1024]={0};
	unsigned char ch;

	i = 0;   //记录源串移动位置
	j = 0;   //记录当前行字符数
	cnt = 0; //记录目标中已有字符数

	while( i < (int)strlen(src))
	{
		ch = src[i++] & 0xff;
		tmp[cnt++] = ch;
		j++;
		if( 127 < ch )  chs_cnt++;   //汉字字符
		if( j % ln == 0)   //需要添加换行符
		{
			if( chs_cnt % 2 != 0)    //半个汉字
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

//////////////////////////////以上为已完成整理函数//////////////////////////////////////


/*[method*******************************************************************************
 *def:  public int  WriteLog2(FILE *fp, char app_log_flag, char cur_log_flag, const char *format, ...)
 *func:
        根据格式字符串记录一行日志，每次自动添加日期时间。
        换行符在格式字符串中指定.
        当 cur_log_lev <= log_lev 时,才能记日志.
        当 cur_log_lev 和 log_lev都为9时,将会标准输出上打出日志信息.
 *args:
        #app_log_lev: 应用的日志等级标志
        #fp:      日志文件指针
        #cur_log_lev: 当前信息的日志级别
        #format:      格式字符串
 *ret:
        0   成功,  <0 失败
*************************************************************************************]*/
int  WriteLog2(FILE *fp, char app_log_flag, char cur_log_flag, const char *format, ...)
{
    int  n;
    char dt[32]={0}, tmp[12];     //存放日期时间
    unsigned char log_flag;                //组合后的日志标志
	va_list arg_ptr;              //变体参数变量

	//日志文件错误,则返回
	if(fp == NULL) return ERR_PUBLIC_FILENULL;

	//计算日志标志
	log_flag = ((app_log_flag & cur_log_flag) & 0xff);

	if( log_flag == 0x00) return 0;    //当前日志不允许记录

	//取得变体参数,开始变体参数处理
    va_start(arg_ptr, format);

	//允许输出到标准输出,则先输出到标准输出
	if ((log_flag & LOG_FLAG_STDIO) == LOG_FLAG_STDIO)
	{
		vprintf(format, arg_ptr);        //不能用printf**
		if(log_flag == LOG_FLAG_STDIO)   //只需要显示,则只显示
		{
			va_end(arg_ptr);
			return 0;
		}
	}

	//取得日期时间
	getDateTime3(dt, (char*)(dt+11), (char*)(dt+20));
    //getDateTime2(dt, (char*)(dt+11));
    dt[10] = 32;   dt[19] = 32;   dt[23] = 32;

	//写日期时间
    fwrite(dt, 1, strlen(dt), fp);
    memset(tmp, 0, sizeof(tmp));
    sprintf(tmp, "[%02X,%02X]", app_log_flag&0xff, cur_log_flag&0xff);
    fwrite(tmp, 1, strlen(tmp), fp);

    //将日志内容写入日志文件
    n = vfprintf(fp, format, arg_ptr);  //返回值为写入的字节数

	//结束变体参数处理
    va_end(arg_ptr);

	//刷新文件
    fflush(fp);

    return 0;
}

/*[method*******************************************************************************
 *def:  public FILE *create_log_file(char *dir, char *pre_name)
 *func:
        创建一个日志文件,如果日志文件已经存在,则保持,否则打开或创建日志文件,
        创建的日志文件名为dir/pre_nameyyyymmdd.log形式
 *args:
        dir       日志文件目录
        pre_name  日志文件前导名
        log_days  需要保持的日志文件天数
*************************************************************************************]*/
FILE *create_log_file(FILE *fp, char *dir, char *pre_name, int log_days)
{
	char tmp[64];

	if(fp == 0)
	{
		//创建当前日志文件
		memset(tmp, 0,sizeof(tmp));
		sprintf(tmp, "%s/%s", dir, pre_name);
		get_preNday(tmp+strlen(tmp), 0, 2);    //取得当前日期时间
		strcat(tmp, ".log");

		fp = fopen(tmp, "a");  //以追加方式打开或创建一个文件,取得文件句柄
		//删除历史日志文件
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s/%s", dir, pre_name);
		get_preNday(tmp+strlen(tmp), log_days, 2);
		strcat(tmp, ".log");
		if(file_isexist(tmp) == 1)	remove(tmp);  //文件存在
	}

	return fp;
}



/*[method*******************************************************************************
 *def:  public unsigned char CRC(unsigned char *buffer, int count)
 *func:
        计算校验字,异或。
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
        取得日期时间到指定的变量中.
 *output:
        #strdate 取得的日期, YYYY-MM-DD格式
        #strtime 取得的事件, HH:MM:SS格式
*************************************************************************************]*/
void getDateTime3(char *strdate,char *strtime, char *strmsec)
{
    time_t t;
    struct tm * tm;

    //取得日期时间
    time(&t);
    tm = localtime(&t);

	//将日期时间格式化
    if(strdate != NULL) sprintf(strdate, "%04d-%02d-%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);
    if(strtime != NULL) sprintf(strtime, "%02d:%02d:%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	if(strmsec != NULL) sprintf(strmsec, "%03d", get_cur_msec() % 1000);
}

/*[method*******************************************************************************
 *def:  public void getDateTime2(char *strdate,char *strtime)
 *func:
        取得日期时间到指定的变量中.
 *output:
        #strdate 取得的日期, YYYY-MM-DD格式
        #strtime 取得的事件, HH:MM:SS格式
*************************************************************************************]*/
void getDateTime2(char *strdate,char *strtime)
{
    time_t t;
    struct tm * tm;

    //取得日期时间
    time(&t);
    tm = localtime(&t);

	//将日期时间格式化
    sprintf(strdate, "%04d-%02d-%02d",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);
    sprintf(strtime, "%02d:%02d:%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
}

//format: 1:yyyy-mm-dd模式, 2:yyyymmdd模式
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
//**:功能：检查指定文件是否存在
//**:返回：存在，返回1，不存在返回0
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
//**:功能：取得指定文件的大小
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






//将一个10进制字符扩展为2个16进制字符表示
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
//将2个16进制的字符压缩成一个10进制字符
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
        将给定字符串中的指定字符串,用指定的字符串替换.
        字符串长度不能大于1024字节.
 *args:
        #src           源字符串
        #fine_str      需要查找的字符串
        #rep_str       用来替换的字符串
        #src_ln        源字符串长度
        #find_len      需要查找的字符串的长度
        #rep_len       用于替换的字符串的长度
 *output:
        #src           替换后的字符串
 *ret:
        >=0  处理后的字符串长度
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
		ln1 = tmp_pos - tmp_buf;        //查找部分之前的长度
		ln2 = src_len - ln1 - strlen(find_str);   //查找部分之后的长度

		memcpy(tmp_buf1, tmp_buf, ln1);
		memcpy(tmp_buf1+ln1, rep_str, rep_len);
		memcpy(tmp_buf1+ln1+rep_len, tmp_buf+ln1+find_len, ln2);
		i_pos = ln1 + rep_len;    //跳过已检测部分,避免用于替换的串中包含有被替换的串从而导致内存溢出的错误

		ret_ln = ln1+rep_len+ln2;
		memset(tmp_buf, 0,sizeof(tmp_buf));
		memcpy(tmp_buf, tmp_buf1, ret_ln);
		src_len = strlen(tmp_buf);     //非常重要
	}

	memset(src, 0, ret_ln+1);
	memcpy(src, tmp_buf, ret_ln);
	return ret_ln;
}
