#include "szztplib.h"

#include "CommParameters.h"
#include "MessengerFactory.h"
#include "PrinterApi.h"

#include <stdexcept>

int SetCommSettings(int portType, int portNo, int baudRate, int flowControl)
{
	int localPortType;
	int localPortNo;
	int localBaudRate;
	int localFlowControl;

	localPortType = COM;
	localPortNo = 1;
	localBaudRate = BAUD_RATE_115200;
	localFlowControl = FLOWCONTROL_NONE;	
	
	if(portType == USB_PORT)
	{
		localPortType = USB;
	}
	if(portType == NETWORK_PORT)
	{
		localPortType = ETHERNET;
	}
	else if(portType == SERIAL_PORT)
	{
		localPortType = COM;

		if(portNo >=1 && portNo <= 99)
		{
			localPortNo = portNo;
		}
		else
		{
			localPortNo = 1;
		}
	
		//retrieve baud rate
		if(baudRate == BAUD_RATE_9600)
		{
			localBaudRate = BAUD_RATE_9600;
		}
		else if(baudRate == BAUD_RATE_19200)
		{
			localBaudRate = BAUD_RATE_19200;
		}
		else if(baudRate == BAUD_RATE_38400)
		{
			localBaudRate = BAUD_RATE_38400;
		}
		else if(baudRate == BAUD_RATE_57600)
		{
			localBaudRate = BAUD_RATE_57600;
		}
		else if(baudRate == BAUD_RATE_115200)
		{
			localBaudRate = BAUD_RATE_115200;
		}
	
		//retrieve flow control
		if(flowControl == FLOW_CONTROL_DSRDTR || flowControl == FLOW_CONTROL_RTSCTS)
		{
			localFlowControl = FLOWCONTROL_HARDWARE;
		}
		else if(flowControl == FLOW_CONTROL_NONE)
		{
			localFlowControl = FLOWCONTROL_NONE;
		}
	}
	
	CommParameters commPara;
	
	//commPara
	commPara.porttype = (CommPortType)localPortType;
	commPara.portno = localPortNo;
	commPara.m_baudrate = (BaudRate)localBaudRate;
	commPara.m_flowcontrol = (FlowControl)localFlowControl;
	commPara.m_parity = MY_PARITY_NONE;
	
	MessengerFactory::setCommParameters(commPara);
    return 1;
}

int GetStatus(unsigned long *status)
{
	return PrinterApi::GetStatus(status);
}

int CutPaper(int cutMode)
{
	return PrinterApi::CutPaper(cutMode);
}

int PrintText(unsigned char *textBuf, int length)
{
	return PrinterApi::PrintText(textBuf, length);
}

int PrintGraph(const char* imageFileName)
{
	return PrinterApi::PrintGraph(imageFileName);
}

int LineFeed()
{
	return PrinterApi::LineFeed();
}

int FeedPaper(int n)
{
	return PrinterApi::FeedPaper(n);
}

int SetPrinterEncodeMode(int mode)
{
	return PrinterApi::SetPrinterEncodeMode(mode);
}

int SetPrinterCodePage(int codepage)
{
	return PrinterApi::SetPrinterCodePage(codepage);
}

int PrintLogo(int logoNo, int printMode)
{
	return PrinterApi::PrintLogo(logoNo, printMode);
}

int SetTextAttr(unsigned long attribute)
{
	return PrinterApi::SetTextAttr(attribute);
}

int SetLeftMargin(int leftMargin)
{
	return PrinterApi::SetLeftMargin(leftMargin);
}

int OpenPort()
{
	return PrinterApi::OpenPort();
}

void ClosePort()
{
	return PrinterApi::ClosePort();
}

int sendBuf(char *buf, int length)
{
    Messenger *messenger;
    int result;

    messenger = MessengerFactory::buildMessenger();

    result = messenger->sendBuf((unsigned char *)buf, length);

    MessengerFactory::releaseMessenger(messenger);

    return result;
}

int recvBuf(char *buf, int length)
{
    Messenger *messenger;
    int result;

    messenger = MessengerFactory::buildMessenger();

    result = messenger->recvBuf((unsigned char *)buf, length);

    MessengerFactory::releaseMessenger(messenger);

    return result;
}
 
int SetBarcode(int barHeight,int barWidth, BarcodeTextPos textPos, BarcodeTextType textFontType)
{
	return PrinterApi::SetBarcode(barHeight, barWidth, textPos, textFontType);
}

int PrintBarcode(BarcodeType barcodeType, char* text)
{
	return PrinterApi::PrintBarcode(barcodeType, text);
}
