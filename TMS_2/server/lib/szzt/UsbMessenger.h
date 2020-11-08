#ifndef __USB_MESSENGER__
#define __USB_MESSENGER__

#ifdef WIN32
#include "WinMessenger.h"

#include <setupapi.h>
#include <devguid.h>
//#include <wdmguid.h>


class UsbMessenger :
	public WinMessenger
{
public:
    UsbMessenger(int portno);
    virtual ~UsbMessenger(void);

	virtual unsigned int sendBuf(unsigned char *buf, int len);
	virtual unsigned int recvBuf(unsigned char *buf, int maxlen);
    virtual unsigned int getStatus(int statusId, int *pStatus);
    virtual unsigned int recoverPrinterStatus(int commandId);

private:
	HDEVINFO devs;

	DWORD devcount;
	SP_DEVINFO_DATA devinfo;
	SP_DEVICE_INTERFACE_DATA devinterface;
	DWORD size;
	GUID intfce;
	PSP_DEVICE_INTERFACE_DETAIL_DATA interface_detail;

    TCHAR driverkey[2048];
    TCHAR interfacename[2048];
    TCHAR location[2048];
    TCHAR description[2048];

	virtual int openChannel();
	virtual int closeChannel();
};
#else

#include "Messenger.h"

class UsbMessenger :
	public Messenger
{
public:
    UsbMessenger(int portNo);
    virtual ~UsbMessenger(void);

    virtual unsigned int sendBuf(unsigned char *buf, int len);
    virtual unsigned int recvBuf(unsigned char *buf, int maxlen);
    virtual unsigned int getStatus(int statusId, int *pStatus);
    virtual unsigned int recoverPrinterStatus(int commandId);
    virtual void setTimeOut(int timeout);

private:
    virtual int openChannel(int portNoss);
    virtual int closeChannel();
    int fd;
    int timeoutInterval;
};

#endif //WIN32


#endif //__USB_MESSENGER__
