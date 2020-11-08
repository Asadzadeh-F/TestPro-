#include "MessengerFactory.h"
#include "Messenger.h"
#include "CommParameters.h"
#include "PrintGraphCmd.h"
#include "Barcode.h"
#include "CmdHandler.h"
#include "szztplib.h"


const char NumTable[]="0123456789";
// У������ȷ���� 0.���󷵻� 1
static int VerifyChkCode(string str)
{
	int ret = 0;
	int i = 0;
	int ChkCode = 0;
	int ChkSum = 0;
	int StrLength = str.length();
	while(NumTable[i]!= (str.at(StrLength -1))) i++;
	ChkCode = i;//ȡ��У����
	string ChkCodeStr = str.substr(0,StrLength -1);
	for(i = 1;i<StrLength;i+=2)//����֮��
	{
		ChkSum += ChkCodeStr[StrLength -1 -i]-'0';
	}
	ChkSum *=3;
	for(i = 2;i < StrLength;i+=2)//��������֮�ͼ���ż��֮��
	{
		ChkSum += ChkCodeStr[StrLength -1 -i]-'0';
	}
	//int n=ChkSum;
	//while((n%10)!=0) n++;//�õ����ڻ�����ChkSum����С��10�ı���
	//if((n - ChkSum) == ChkCode)
	int n=0;
	if((10 - ((n=ChkSum%10)==0?10:n)) == ChkCode)
		return 0;//У������ȷ
	else return 1;
}
static int isDigitChk(string str,int startPos ,int Length)
{
	//if(str.length()==17 ||str.length() == 18)
	//{
		for(int i=startPos-1; i<Length;i++)
		{
			if(!isdigit(str.at(i)))
			{
				return 0;//�������֣�����0
			}	
		}
		return 1;//�����֣�����1

	//}else
	//	return -1;

}
//�����Ƿ��и����룬�У�����1���ޣ�����0
static int isExtraCodeChk(string str)
{
	int i=0;
	while(i<str.length())
	{
		if(str[i] == ' ')
			return 1;
		i++;
	}
	return 0;
}
/*
int Barcode::doPrintBarcode(int barcodeType, char* barcode,char *extracode)
{
	BOOL dataValid;

	dataValid =BarcodeArgChk(barcodeType,barcode,extracode);
	if(dataValid == ZT_SUCCESS)
	{
		int ret = -1;

		Messenger* sender = MessengerFactory::buildMessenger();

		CmdHandler handler(sender);	

		ret = handler.sendCmd(BarcodeArg);//��ӡ����

		MessengerFactory::releaseMessenger(sender);
		return ZT_SUCCESS;
	}
	else
	{
		return ZT_FAIL;
	}
}*/




//3 extra chars to hold the command itself, ie. 0x1d, 0x6b, 0x7.
//3 extra chars to hold pdf417-row, pdf417-column, and a null char.
static unsigned char BarcodeCmdBuf[256 + 3 + 3] = {0x1d, 0x6b, 7,  }; 

Barcode::Barcode()
	: barcodeType(EAN13)
	, barcodeText("0123456789012")
	, barcodeHeight(65)
	, barcodeWidth(3)
	, textPos(2)
	, textFontType(0)
{
	buf = BarcodeCmdBuf;
	len = 0;
}
Barcode::~Barcode()
{

}
/*
int Barcode::BarcodeArgChk(int barcodeType, char* barcode, char* extracode)
{
	int err = ZT_SUCCESS;
	string BcAndEc;
	string BarCodeStr;
	string ExtraCodeStr;
	int BarCodeLength=0;
	int ExtraCodeLength=0;	

	BarCodeStr = barcode;
	BarCodeLength = BarCodeStr.length();
	ExtraCodeStr = extracode;
	
	switch (barcodeType)
	{
	case PDF417:setRows(0);setColumns(0);break;
	case 10:barcodeType = CODE93;break;
	case 11:barcodeType = CODE128;break;
	default:break;
	}
	//this must be set first, because setBuf depend on barcode type
	setType(barcodeType);
	if( barcodeType == EAN13 
		||  barcodeType == EAN8 
		||  barcodeType == UPCA 
		||  barcodeType == UPCE 
		||  barcodeType == JAN13 
		||  barcodeType == JAN8 
		)
	{
		if (extracode != "")//�и�����
		{
			ExtraCodeLength = ExtraCodeStr.length();
			if(!(ExtraCodeLength ==2 || ExtraCodeLength==5))
			{
				//printf("��������2λ��5λ������!\n");
				err |= ZT_BARCODE_DATA_FORMAT_ERR;
			}
			if(!isDigitChk(ExtraCodeStr,1,ExtraCodeLength))
			{
				//printf("����������������!\n"); 
				err |= ZT_BARCODE_DATA_TYPE_ERR;		
			}
			BcAndEc =BarCodeStr + '+'+ ExtraCodeStr;//
		}
		else//�и����������룬��δ��д������
		{
			BcAndEc = BarCodeStr;
		}

		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("�������ֱ���������!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;
		}
	}else//�޸�����������
	{
		BcAndEc = BarCodeStr;
	}

	if(barcodeType == EAN13||  barcodeType == JAN13)
	{
		if(BarCodeLength != 12 && BarCodeLength != 13)
		{
			//printf("EAN13 JAN13����������12λ����!\n"); 
			err |= ZT_BARCODE_DATA_FORMAT_ERR;
		}
		if (BarCodeLength == 13)
		{

			if(!VerifyChkCode(BarCodeStr))
			{//У������ȷ
				//printf("ChkCode ok!\n"); 
			}
			else
			{//У���벻��ȷ
				//printf("ChkCode error!\n"); 
				err |= ZT_BARCODE_CHKCODE_ERR;
			}
		}

		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("EAN13 JAN13�������ֱ���������!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;
		}
	}else if (barcodeType == EAN8 ||  barcodeType == JAN8)
	{
		if(BarCodeLength != 7 && BarCodeLength != 8)
		{
			//printf("EAN8 JAN8����������7λ����!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;
		}
		if (BarCodeLength == 8)
		{
			if(!VerifyChkCode(BarCodeStr))
			{//У������ȷ
				//printf("ChkCode ok!\n"); 
			}
			else
			{//У���벻��ȷ
				//printf("ChkCode error!\n"); 
				err |= ZT_BARCODE_CHKCODE_ERR;
			}
		}
		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("EAN8 JAN8�������ֱ���������!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;
		}
	}else if (barcodeType == UPCA)
	{
		if(BarCodeLength != 11 && BarCodeLength != 12)
		{
			//printf("UPCA����������11λ����!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;
		}
		if (BarCodeLength == 12)
		{
			if(!VerifyChkCode(BarCodeStr))
			{//У������ȷ
				//printf("ChkCode ok!\n"); 
			}
			else
			{//У���벻��ȷ
				//printf("ChkCode error!\n"); 
				err |= ZT_BARCODE_CHKCODE_ERR;
			}
		}
		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("UPCA�������ֱ���������!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;

		}		
	}else if (barcodeType == UPCE)
	{
		if(BarCodeLength != 6 && BarCodeLength != 7)
		{
			//printf("UPCE����������6λ����!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;
		}
		if (BarCodeLength == 7)
		{
			if(!VerifyChkCode(BarCodeStr))
			{//У������ȷ
				//printf("ChkCode ok!\n"); 
			}
			else
			{//У���벻��ȷ
				//printf("ChkCode error!\n"); 
				err |= ZT_BARCODE_CHKCODE_ERR;
			}
		}

		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("UPCE�������ֱ���������!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;			
		}	
	}else if(barcodeType == ITF)
	{		
		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("ITF�������ֱ���������!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;

		}
		if(!(BarCodeLength == 13 || BarCodeLength == 14))
		{
			//printf("ITF����������13λ����!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;
		}else if(BarCodeLength == 14)//����Ϊ14��˵����У��λ
		{
			if(!VerifyChkCode(BarCodeStr))
			{//У������ȷ
				//printf("ChkCode ok!\n"); 
			}
			else
			{//У���벻��ȷ
				//printf("ChkCode error!\n"); 
				err |= ZT_BARCODE_CHKCODE_ERR;
			}
		}

	}else if( barcodeType == CODABAR)
	{
		if(
			( BarCodeStr[0] != 'A'
			&&  BarCodeStr[0] != 'B'
			&&  BarCodeStr[0] != 'C' 
			&&  BarCodeStr[0] != 'D' 
			)
			||
			( BarCodeStr[BarCodeLength - 1] != 'A' 
			&&  BarCodeStr[BarCodeLength - 1] != 'B' 
			&&  BarCodeStr[BarCodeLength - 1] != 'C' 
			&&  BarCodeStr[BarCodeLength - 1] != 'D' 
			)
			)
		{
			//printf("��һ���ַ�������һ���ַ�������A��B��C��D!\n"); 
			err |= ZT_BARCODE_DATA_TYPE_ERR;
		}

		for(int i=1; i < BarCodeLength - 1; i++)
		{
			if(!isdigit( BarCodeStr.at(i)) 
				&&  BarCodeStr.at(i) != '-'
				&&  BarCodeStr.at(i) != '$'
				&&  BarCodeStr.at(i) != ':'
				&&  BarCodeStr.at(i) != '/'
				&&  BarCodeStr.at(i) != '+'
				&&  BarCodeStr.at(i) != '.'
				)
			{
				//printf("�����ַ����������ֻ򡰨C$:/+.���е�һ��\n"); 
				err |= ZT_BARCODE_DATA_TYPE_ERR;
				break;
			}
		}
	}
	setBuf(BcAndEc);
	return err;
}*/

int Barcode::BarcodeArgChk(int barcodeType, char* barcodeAndExtracode)
{
	int err = ZT_SUCCESS;
	string BcAndEc= barcodeAndExtracode;

	string BarCodeStr;
	string ExtraCodeStr;
	int BarCodeLength=0;
	int ExtraCodeLength=0;	
	if( barcodeType == EAN13 
		||  barcodeType == EAN8 
		||  barcodeType == UPCA 
		||  barcodeType == UPCE 
		||  barcodeType == JAN13 
		||  barcodeType == JAN8 
		)
	{
		if(isExtraCodeChk(BcAndEc))//�и�����
		{
			while(BcAndEc[BarCodeLength]!=' ')//���������������ֱ����ÿո��ֿ�
			{ 
				BarCodeLength++;
			};//���������븽�����ÿո��ֿ�
			ExtraCodeLength = BcAndEc.length()-BarCodeLength -1;//�õ������볤��
			//printf("ExtraCodeLength=%d \n BarCodeLength=%d\n",ExtraCodeLength,BarCodeLength);
		}
        else//û�и�����
        {
		    BarCodeStr = BcAndEc;
		    BarCodeLength = BcAndEc.length();
		    ExtraCodeLength = 0;
        }
	}
    else
    {
    	BarCodeStr = BcAndEc;
		BarCodeLength = BcAndEc.length();
    }

    if(BarCodeLength>=1)//��ֹ��������������������
	{
		BarCodeStr = BcAndEc.substr(0,BarCodeLength);//ȡ�����������ַ��Ӵ�

	}
	else
	{
		err = ZT_BARCODE_NO_BARCODE_ERR;
		return err;
	}

	switch (barcodeType)
	{
		case PDF417:setRows(0);setColumns(0);break;
		case 10:barcodeType = CODE93;break;
		case 11:barcodeType = CODE128;break;
		default:break;
	}
	//this must be set first, because setBuf depend on barcode type
	setType(barcodeType);
	if( barcodeType == EAN13 
		||  barcodeType == EAN8 
		||  barcodeType == UPCA 
		||  barcodeType == UPCE 
		||  barcodeType == JAN13 
		||  barcodeType == JAN8 
		)
	{
		if(0 != ExtraCodeLength)//�и�����
		{

			if(!(ExtraCodeLength ==2 || ExtraCodeLength==5))
			{
				//printf("��������2λ��5λ������!\n");
				err = ZT_BARCODE_DATA_FORMAT_ERR;
				return err;
			}
			
			ExtraCodeStr = BcAndEc.substr(BarCodeLength + 1,BarCodeLength + ExtraCodeLength + 1);//ȡ�ø������ַ��Ӵ�
		
			if(!isDigitChk(ExtraCodeStr,1,ExtraCodeLength))
			{
				//printf("����������������!\n"); 
				err = ZT_BARCODE_DATA_TYPE_ERR;	
				return err;
			}
			BcAndEc[BarCodeLength]='+';//�ո�λ�ӱ�Ϊ'+'
		}else//�и����������룬��δ��д������
		{
			BcAndEc = BarCodeStr;
		}

		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("�������ֱ���������!\n"); 
			err = ZT_BARCODE_DATA_TYPE_ERR;
			return err;
		}
	}else//�޸�����������
	{
		BcAndEc = BarCodeStr;
	}

	if(barcodeType == EAN13||  barcodeType == JAN13)
	{
		if(BarCodeLength != 12 && BarCodeLength != 13)
		{
			//printf("EAN13 JAN13����������12λ����!\n"); 
			err = ZT_BARCODE_DATA_FORMAT_ERR;
			return err;
		}
		if (BarCodeLength == 13)
		{
		
			if(!VerifyChkCode(BarCodeStr))
			{//У������ȷ
				//printf("ChkCode ok!\n"); 
			}
			else
			{//У���벻��ȷ
				//printf("ChkCode error!\n"); 
				err = ZT_BARCODE_CHKCODE_ERR;
			}
		}

		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("EAN13 JAN13�������ֱ���������!\n"); 
			err = ZT_BARCODE_DATA_TYPE_ERR;
			return err;
		}
	}else if (barcodeType == EAN8 ||  barcodeType == JAN8)
	{
		if(BarCodeLength != 7 && BarCodeLength != 8)
		{
			//printf("EAN8 JAN8����������7λ����!\n"); 
			err = ZT_BARCODE_DATA_TYPE_ERR;
			return err;
		}
		if (BarCodeLength == 8)
		{
			if(!VerifyChkCode(BarCodeStr))
			{//У������ȷ
				//printf("ChkCode ok!\n"); 
			}
			else
			{//У���벻��ȷ
				//printf("ChkCode error!\n"); 
				err = ZT_BARCODE_CHKCODE_ERR;
				return err;
			}
		}
		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("EAN8 JAN8�������ֱ���������!\n"); 
			err = ZT_BARCODE_DATA_TYPE_ERR;
			return err;
		}
	}else if (barcodeType == UPCA)
	{
		if(BarCodeLength != 11 && BarCodeLength != 12)
		{
			//printf("UPCA����������11λ����!\n"); 
			err = ZT_BARCODE_DATA_TYPE_ERR;
			return err;
		}
		if (BarCodeLength == 12)
		{
			if(!VerifyChkCode(BarCodeStr))
			{//У������ȷ
				//printf("ChkCode ok!\n"); 
			}
			else
			{//У���벻��ȷ
				//printf("ChkCode error!\n"); 
				err = ZT_BARCODE_CHKCODE_ERR;
				return err;
			}
		}
		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("UPCA�������ֱ���������!\n"); 
			err = ZT_BARCODE_DATA_TYPE_ERR;
			return err;			
		}		
	}else if (barcodeType == UPCE)
	{
		if(BarCodeLength != 6 && BarCodeLength != 7)
		{
			//printf("UPCE����������6λ����!\n"); 
			err = ZT_BARCODE_DATA_TYPE_ERR;
			return err;
		}
		
        /*if (BarCodeLength == 7)
		{
			if(!VerifyChkCode(BarCodeStr))
			{//У������ȷ
				//printf("ChkCode ok!\n"); 
			}
			else
			{//У���벻��ȷ
				//printf("ChkCode error!\n"); 
				err = ZT_BARCODE_CHKCODE_ERR;
				return err;
			}
		}
        */

		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
		{
			//printf("UPCE�������ֱ���������!\n"); 
			err = ZT_BARCODE_DATA_TYPE_ERR;	
			return err;
		}	
	}
    else if(barcodeType == ITF)
	{		
		if(!isDigitChk(BarCodeStr,1,BarCodeLength))
			{
				//printf("ITF�������ֱ���������!\n"); 
				err = ZT_BARCODE_DATA_TYPE_ERR;
				return err;
			}

        /*if(!VerifyChkCode(BarCodeStr))
				{//У������ȷ
					//printf("ChkCode ok!\n"); 
				}
				else
				{//У���벻��ȷ
					//printf("ChkCode error!\n"); 
					err = ZT_BARCODE_CHKCODE_ERR;
					return err;
	    }
    	*/
	}
    else if( barcodeType == CODABAR)
	{
			if(
				( BarCodeStr[0] != 'A'
				&&  BarCodeStr[0] != 'B'
				&&  BarCodeStr[0] != 'C' 
				&&  BarCodeStr[0] != 'D' 
				)
				||
				( BarCodeStr[BarCodeLength - 1] != 'A' 
				&&  BarCodeStr[BarCodeLength - 1] != 'B' 
				&&  BarCodeStr[BarCodeLength - 1] != 'C' 
				&&  BarCodeStr[BarCodeLength - 1] != 'D' 
				)
				)
			{
				//printf("��һ���ַ�������һ���ַ�������A��B��C��D!\n"); 
				err = ZT_BARCODE_DATA_TYPE_ERR;
				return err;
			}

			for(int i=1; i < BarCodeLength - 1; i++)
			{
				if(!isdigit( BarCodeStr.at(i)) 
					&&  BarCodeStr.at(i) != '-'
					&&  BarCodeStr.at(i) != '$'
					&&  BarCodeStr.at(i) != ':'
					&&  BarCodeStr.at(i) != '/'
					&&  BarCodeStr.at(i) != '+'
					&&  BarCodeStr.at(i) != '.'
					)
				{
					//printf("�����ַ����������ֻ򡰨C$:/+.���е�һ��\n"); 
					err = ZT_BARCODE_DATA_TYPE_ERR;
					return err;
					//break;
				}
			}
	}
	else if(barcodeType == CODE39)
	{
			for(int i = 0; i < BarCodeLength - 1; i++)
			{
				if(!isdigit( BarCodeStr.at(i)) 
                    && !isalpha(BarCodeStr.at(i))
					&&  BarCodeStr.at(i) != '-'
					&&  BarCodeStr.at(i) != '.'
					&&  BarCodeStr.at(i) != '$'
					&&  BarCodeStr.at(i) != '/'
					&&  BarCodeStr.at(i) != '+'
					&&  BarCodeStr.at(i) != '%'
					&&  BarCodeStr.at(i) != ' '
					)
				{
					//printf("�����ַ����������ֻ򡰨C.$/+%���е�һ��\n"); 
					err = ZT_BARCODE_DATA_TYPE_ERR;
					return err;
					//break;
				}
			}
	}

	setBuf(BcAndEc);
	return err;
}

void Barcode::releaseBarcode(Barcode *barcode)
{
	delete barcode;
}

void Barcode::setLen(int length)
{
	 len = length;    
}

void Barcode::setRows(int rows)
{
	BarcodeCmdBuf[3] = rows;
}

void Barcode::setColumns(int cols)
{
	BarcodeCmdBuf[4] = cols;
}

void Barcode::setBuf(string barcodeAndExtracode)
{
	unsigned int i;
	int startLen;

	startLen = 3;
	if(barcodeType == PDF417)
	{
		startLen = 5;
	}

	for(i = 0; i < barcodeAndExtracode.length(); i++)
	{
		BarcodeCmdBuf[startLen + i] = barcodeAndExtracode.at(i);
	}
	BarcodeCmdBuf[startLen + i] = '\0';
	setLen(startLen + barcodeAndExtracode.length() + 1);
}



static unsigned char TextFontTypeCmdBuf[] = {0x1d, 0x66, 2}; 
static unsigned char FontPosCmdBuf[]	  =	{0x1d, 0x48, 2}; 
static unsigned char HeightCmdBuf[]		  =	{0x1d, 0x68, 162}; 
static unsigned char WidthCmdBuf[]		  =	{0x1d, 0x77, 2}; 

void Barcode::setType(int  type)
{
	barcodeType = type;
	BarcodeCmdBuf[2] = type;
	buf= BarcodeCmdBuf;
}

void Barcode::setHeight(int height)
{
	barcodeHeight = height;
}
void Barcode::setWidth(int width)
{
	barcodeWidth = width;
}
void Barcode::setFontPos(int pos)
{
	textPos = pos;
}
void Barcode::setTextFontType(int type)
{
	textFontType =type;
}



void Barcode::setBarcodeHeight(int height)
{
	buf = HeightCmdBuf;
	HeightCmdBuf[2] = height;
	len =sizeof(HeightCmdBuf);
}
void Barcode::setBarcodeWidth(int width)
{
	buf = WidthCmdBuf;
	WidthCmdBuf[2] = width;
	len =sizeof(WidthCmdBuf);
}
void Barcode::setBarcodeFontPos(int pos)
{
	buf = FontPosCmdBuf;
	FontPosCmdBuf[2] = pos;
	len =sizeof(FontPosCmdBuf);
}
void Barcode::setBarcodeTextFontType(int type)
{
	buf = TextFontTypeCmdBuf;
	TextFontTypeCmdBuf[2] = type;
	len =sizeof(TextFontTypeCmdBuf);
}

