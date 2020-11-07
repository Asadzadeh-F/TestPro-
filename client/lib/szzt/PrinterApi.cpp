#include "PrinterApi.h"

#include "MessengerFactory.h"
#include "Messenger.h"
#include "CommParameters.h"
#include "Barcode.h"
#include "CmdHandler.h"

#include "PrintGraphCmd.h"
#include "zterrorcode.h"
#include "PrintLogoCmd.h"
#include "TextAttr.h"

#ifndef WIN32
#include <linux/lp.h>
#endif

//#define UNICODE_SUPPORT

const unsigned short EncodeMode[] = {MODE_ANSI, MODE_UNICODE_LE, MODE_UNICODE_BE};

unsigned short encodeMode = MODE_UNICODE_LE;
unsigned long ztLastError;
static Messenger *messenger = NULL;

PrinterApi::PrinterApi(void)
{
}

PrinterApi::~PrinterApi(void)
{
}

int PrinterApi::OpenPort()
{
    if(messenger != NULL)
    {
        //already opened.
        return ZT_SUCCESS;
    }

    messenger = MessengerFactory::buildMessenger();
    //PrinterApi::messenger = messenger;
    if(messenger == NULL)
    {
        return ZT_FAIL;
    }
    else
    {
        return ZT_SUCCESS;
    }
}

void PrinterApi::ClosePort()
{
    if(messenger)
    {
        MessengerFactory::releaseMessenger(messenger);
        messenger = NULL;    
    }
}


int PrinterApi::SetPrinterCodePage(int codepage)
{
    unsigned char buf[3];
    int result;

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

#ifdef UNICODE_SUPPORT
    //force to ANSI mode
    buf[0] = 0xff;
    buf[1] = 0xff;
    result = messenger->sendBuf(buf, 2);
#endif

    //prepare data
    buf[0] = 0x1b;
    buf[1] = 't';
    buf[2] = codepage;

    //send out
    result = messenger->sendBuf(buf, 3);

    //don't know how, if we dont add this, data will not be send to printer 
    //uhder linux. anyhow, a null char will be neglected by printer.
    buf[0] = 0;
    buf[1] = 0;
    messenger->sendBuf(buf, 2);

    return result;
}

int PrinterApi::SetPrinterEncodeMode(int mode)
{
    unsigned char buf[3];
    int result;

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

    //validate 
    if(mode < 0 || mode > 2)
    {
        mode = 0;
    }

    encodeMode = EncodeMode[mode];


    //prepare data
    buf[0] = ((encodeMode >> 8) & 0xff);
    buf[1] = (encodeMode & 0xff);

    //send out
    result = messenger->sendBuf(buf, 2);

    //don't know how, if we dont add this, data will not be send to printer 
    //uhder linux. anyhow, a null char will be neglected by printer.
    buf[0] = 0;
    buf[1] = 0;
    messenger->sendBuf(buf, 2);

    return result;
}

int PrinterApi::PrintText(unsigned char *textBuf, int length)
{
    int result;
    unsigned char buf[2];

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

    //send out text
    result = messenger->sendBuf(textBuf, length);

    //don't know how, if we dont add this, data will not be send to printer 
    //uhder linux. anyhow, a null char will be neglected by printer.
    buf[0] = 0;
    buf[1] = 0;
    messenger->sendBuf(buf, 2);

    return result;
}

int PrinterApi::LineFeed()
{
    int result;
    unsigned char buf[3];

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

    //print out the text that remained in the printer buffer.
    buf[0] = 0xff;
    buf[1] = 0xff;
    buf[2] = 0x0a;
    result = messenger->sendBuf(buf, 3);

    //don't know how, if we dont add this, data will not be send to printer 
    //uhder linux. anyhow, a null char will be neglected by printer.
    buf[0] = 0;
    buf[1] = 0;
    messenger->sendBuf(buf, 2);

    return result;
}


int PrinterApi::PrintGraph(const char* imageFileName)
{
    int result;
    unsigned char buf[3];
    int steps;

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

    PrintGraphCmd cmd(imageFileName);

    result = 0;

	if(messenger){

        //force to ANSI mode
#ifdef UNICODE_SUPPORT
        buf[0] = 0xff;
        buf[1] = 0xff;
        result = messenger->sendBuf(buf, 2);
#endif
		steps = cmd.getSteps();

		int i;
		for(i=0; i<steps; i++){

			result = messenger->sendBuf(cmd.getStepNBuf(i),cmd.getStepLen(i));
			if(result) break;
		}

        buf[0] = 0x1b;
        buf[1] = 0x4a;
        buf[2] = 0x08;
	    result = messenger->sendBuf(buf, 3);

	}

    return result;
}

int PrinterApi::PrintLogo(int logoNo, int printMode)
{
    int result;

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

    if(logoNo > 2 || logoNo < 0)
    {
        logoNo = 0;
    }

    if(printMode > 3 || printMode < 0)
    {
        printMode = 0;
    }

    PrintLogoCmd logoCmd(printMode);

    logoCmd.setLogoNo(logoNo);

    result = messenger->sendBuf(logoCmd.getBuf(), logoCmd.getLen()); 

    return result; 

}

int PrinterApi::SetTextAttr(unsigned long attribute)
{
    int result;
    TextAttr textAttr;

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

    textAttr.setAttr(attribute);
    result = messenger->sendBuf(textAttr.getBuf(), textAttr.getLen()); 

    return result; 
}


int PrinterApi::SetLeftMargin(int leftMargin)
{
    int result;
    unsigned char buf[4];

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

    buf[0] = 0x1d;
    buf[1] = 0x4c;
    buf[2] = leftMargin % 256;
    buf[3] = leftMargin / 256;

    result = messenger->sendBuf(buf, 4); 

    return result; 
}


int PrinterApi::CutPaper(int cutMode)
{
    int result;
    unsigned char buf[4];
    int localcutmode;

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

    //force to ANSI mode
#ifdef UNICODE_SUPPORT
    buf[0] = 0xff;
    buf[1] = 0xff;
    result = messenger->sendBuf(buf, 2);
#endif
    //set cut mode
    if(cutMode & HALF_CUT_FLAG)
    {
        localcutmode = 1;
    }
    else
    {
        //full cut
        localcutmode = 0;
    }

    buf[0] = 0x1b;
    buf[1] = 'N';
    buf[2] = 3;
    buf[3] = localcutmode;

    result = messenger->sendBuf(buf, 4);

    if(cutMode & BLACKMARK_CUT_FLAG)
    {
        //activate black mark
        buf[0] = 0x1d;
        buf[1] = ':';
        buf[2] = 0x01;
        result = messenger->sendBuf(buf, 3);

        //send cut command
        buf[0] = 0x1d;
        buf[1] = 0x56;
        buf[2] = 0x42;
        buf[3] = 0x01;
        result = messenger->sendBuf(buf, 4);
    }
    else
    {
        //send cut command
        buf[0] = 0x1d;
        buf[1] = 0x56;
        buf[2] = 0x01;
        result = messenger->sendBuf(buf, 3);
    }

    //don't know how, if we dont add this, data will not be send to printer 
    //uhder linux. anyhow, a null char will be neglected by printer.
    buf[0] = 0;
    buf[1] = 0;
    messenger->sendBuf(buf, 2);

    return result;

}

int PrinterApi::GetStatus(unsigned long *pStatus)
{
    unsigned long status;
    int statusByte;
    CommParameters commPara;
    bool portClosed = false;

    Messenger* sender; 

    if(messenger)
    {
        ClosePort();
        portClosed = true;
    }

    status = 0xffffffff;

    commPara = MessengerFactory::getCommParameters();
    if(commPara.porttype == COM)
    {
        //set flow control to none so that printer may respond even if
        //printer's receive buffer is full.
        commPara.m_flowcontrol = FLOWCONTROL_NONE;
        MessengerFactory::setCommParameters(commPara);
    }

    sender = MessengerFactory::buildMessenger();

    int ret = ZT_FAIL; 

    //send out the cmd via the channel
    if(sender){

        ret = sender->getStatus(1, &statusByte);
        if(ret == ZT_SUCCESS)
        {
            status = statusByte;
        }
#ifndef WIN32
        //under linux there is no way to send our vender-specific commands to printer
        //so we just send a standard GET_PORT_STATUS command to get very limited infomation.
        else if(ret == ZT_GETSTATUS_VIA_CONTROL)
        {
            //LP_PBUSY     inverted busy input, active high
            //LP_PACK      unchanged acknowledge input, active low
            //LP_POUTPA    unchanged out-of-paper input, active high
            //LP_PSELECD   unchanged selected input, active high
            //LP_PERRORP   unchanged error input, active low
            if(statusByte & LP_POUTPA)
            {
                status |= STATUS_PAPER_ERROR;
            }
            else if(statusByte & (~LP_PERRORP) == 0)
            {
                status |= STATUS_OFFLINE;
            }
            ret = ZT_SUCCESS;
            return ret;
        }
#endif
        if(ret == ZT_SUCCESS)
        {
            ret = sender->getStatus(2, &statusByte);
            if(ret == ZT_SUCCESS)
            {
                status += (statusByte << 8);
            }
        }

        if(ret == ZT_SUCCESS)
        {
            ret = sender->getStatus(3, &statusByte);
            if(ret == ZT_SUCCESS)
            {
                status += (statusByte << 16);
            }
        }

        if(ret == ZT_SUCCESS)
        {
            ret = sender->getStatus(4, &statusByte);
            if(ret == ZT_SUCCESS)
            {
                status += (statusByte << 24);
            }
        }

        MessengerFactory::releaseMessenger(sender);

    }

    if(commPara.porttype == COM)
    {
        commPara.m_flowcontrol = FLOWCONTROL_HARDWARE;
        MessengerFactory::setCommParameters(commPara);
    }

    if(portClosed)
    {
        messenger = MessengerFactory::buildMessenger();
    }

    *pStatus = status;

    return ret;
}

int PrinterApi::FeedPaper(int n)
{
    int result;
    unsigned char buf[3];
    int times, remains;

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

    if(n < 0)
    {
        n = 0;
    }

    if(n > 1024)
    {
        n = 1024;
    }

    times = n / 256;
    remains = n % 256;
    for(int i = 0; i < times; i ++)
    {
        buf[0] = 0x1b;
        buf[1] = 0x4a;
        buf[2] = 255;
        result = messenger->sendBuf(buf, 3);
    }
    if(remains > 0)
    {
        buf[0] = 0x1b;
        buf[1] = 0x4a;
        buf[2] = remains;
        result = messenger->sendBuf(buf, 3);
    }

    return result;
}

//**********************************************************************

//The real function to set the barcode's argements
int PrinterApi::doSetBarcodeArg(Barcode &BarcodeArg)
{
	int ret = ZT_SUCCESS;

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

	if(messenger)
	{
		CmdHandler handler(messenger);	
		BarcodeArg.setBarcodeWidth(BarcodeArg.barcodeWidth);
		ret = handler.sendCmd(BarcodeArg);
		if(!ret)
		{
			BarcodeArg.setBarcodeHeight(BarcodeArg.barcodeHeight);
			ret = handler.sendCmd(BarcodeArg);
		}

		if(!ret)
		{
			BarcodeArg.setBarcodeFontPos(BarcodeArg.textPos);
			ret = handler.sendCmd(BarcodeArg);
		}
		if(!ret)
		{
			BarcodeArg.setBarcodeTextFontType( BarcodeArg.textFontType);
			ret = handler.sendCmd(BarcodeArg);
		}
	}

    return ret;
}

int PrinterApi::SetBarcode(int height,int width,int textPos,int textFontType)
{
	int ret=ZT_SUCCESS;
    Barcode BarcodeArg;

	BarcodeArg.setHeight(height);
	BarcodeArg.setWidth(width);
	BarcodeArg.setFontPos(textPos);
	BarcodeArg.setTextFontType(textFontType);

	if(!(BarcodeArg.barcodeHeight>=30 && BarcodeArg.barcodeHeight<=240))
	{
		//printf("The value of barcodeHeight must be set between 30 and 240!\n");
		ret = ZT_BARCODE_HEIGHT_ERR;
		return ret;
	}
	if(!(BarcodeArg.barcodeWidth>=2 && BarcodeArg.barcodeWidth<=6))
	{
		//printf("The value of barcodeWidth must be set between 2 and 6!\n");
		ret = ZT_BARCODE_WIDTH_ERR;
		return ret;
	}
	if(!(BarcodeArg.textPos >=0 && BarcodeArg.textPos<=3))
	{
		//printf("The value of textPos must be set between 0 and 3!\n");
		ret = ZT_BARCODE_FONT_POS_ERR;
		return ret;
	}
	if(!(BarcodeArg.textFontType == 0 || BarcodeArg.textFontType == 1))
	{
		//printf("The value of textFontType must be set between 0 and 1!\n");
		ret = ZT_BARCODE_TEXT_FONT_TYPE_ERR;
		return ret;
	}
	ret = doSetBarcodeArg(BarcodeArg);

	return ret;	
}

int PrinterApi::PrintBarcode(int barcodeType, char* text)
{
    Barcode barcode;
	int ret;

    if(messenger == NULL)
    {
        return ZT_FAIL;
    }

	ret = barcode.BarcodeArgChk(barcodeType, text);
	if(ret == ZT_SUCCESS)
	{
		CmdHandler handler(messenger);	

		ret = handler.sendCmd(barcode);//��ӡ����
	}

    return ret;
}
