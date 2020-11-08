#include "UsbMessenger.h"
#include "zterrorcode.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#ifdef WIN32

#include <windows.h> 
#include <tchar.h>
#include <strsafe.h>

#define usbHandle hCom

#pragma comment(lib, "setupapi.lib")

extern VOID WINAPI CompletedWriteRoutine(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap); 
extern VOID WINAPI CompletedReadRoutine(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap);

/* This define is required so that the GUID_DEVINTERFACE_USBPRINT variable
 * is declared an initialised as a static locally, since windows does not include it
 * in any of its libraries
 */

#define SS_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
static const GUID name \
= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

SS_DEFINE_GUID(GUID_DEVINTERFACE_USBPRINT, 0x28d78fad, 0x5a12, 0x11D1, 
				0xae, 0x5b, 0x00, 0x00, 
				0xf8, 0x03, 0xa8, 0xc2);



#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

#define USBPRINT_IOCTL_INDEX  0x0000
#define FILE_DEVICE_UNKNOWN             0x00000022
#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1
#define METHOD_OUT_DIRECT               2
#define METHOD_NEITHER                  3
#define FILE_ANY_ACCESS                 0
#define FILE_SPECIAL_ACCESS    (FILE_ANY_ACCESS)
#define FILE_READ_ACCESS          ( 0x0001 )    // file & pipe
#define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe



#define IOCTL_USBPRINT_GET_LPT_STATUS  CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   USBPRINT_IOCTL_INDEX+12,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)                                                           

#define IOCTL_USBPRINT_GET_1284_ID     CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   USBPRINT_IOCTL_INDEX+13,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)                                                           

#define IOCTL_USBPRINT_VENDOR_SET_COMMAND CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   USBPRINT_IOCTL_INDEX+14,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)                                                           

#define IOCTL_USBPRINT_VENDOR_GET_COMMAND CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   USBPRINT_IOCTL_INDEX+15,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)                                                           

#define IOCTL_USBPRINT_SOFT_RESET         CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   USBPRINT_IOCTL_INDEX+16,\
                                                   METHOD_BUFFERED,  \
                                                   FILE_ANY_ACCESS)                                                           

UsbMessenger::UsbMessenger(int portno)
	:devs(INVALID_HANDLE_VALUE) 
{

	openChannel();
}

UsbMessenger::~UsbMessenger(void)
{
	closeChannel();

}


int UsbMessenger::openChannel()
{
	int ret;

    if(usbHandle != INVALID_HANDLE_VALUE) {
		ret = 0;
		goto exit1;
	}

	intfce = GUID_DEVINTERFACE_USBPRINT;
	devs = SetupDiGetClassDevs(&intfce, 0, 0, DIGCF_PRESENT |
								DIGCF_DEVICEINTERFACE);
	if (devs == INVALID_HANDLE_VALUE) 
	{
		ret = -1;
		goto exit1;
	}
	devcount = 0;
	devinterface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	while (SetupDiEnumDeviceInterfaces(devs, 0, &intfce, devcount, &devinterface)) 
	{
	    /* The following buffers would normally be malloced to he correct size
	     * but here we just declare them as large stack variables
	     * to make the code more readable
	     */
	    //char driverkey[2048];
	    //wchar_t interfacename[2048];
	    //char location[2048];
	    //char description[2048];
		
	    /* If this is not the device we want, we would normally continue onto the
	     * next one or so something like
	     * if (!required_device) continue; would be added here
	     */
	    devcount++;
	    size = 0;
	    /* See how large a buffer we require for the device interface details */
	    SetupDiGetDeviceInterfaceDetail(devs, &devinterface, 0, 0, &size, 0);
	    devinfo.cbSize = sizeof(SP_DEVINFO_DATA);
	    interface_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)calloc(1, size);
	    if (interface_detail) 
	    {
			interface_detail->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
			devinfo.cbSize = sizeof(SP_DEVINFO_DATA);
			if (!SetupDiGetDeviceInterfaceDetail(devs, &devinterface, 
												interface_detail, size, 0, &devinfo)) 
			{
				free(interface_detail);
				SetupDiDestroyDeviceInfoList(devs);
				devs = INVALID_HANDLE_VALUE;
				ret = -1;
				goto exit1;
			}
			/* Make a copy of the device path for later use */
			_tcscpy(interfacename, interface_detail->DevicePath);
			free(interface_detail);
			/* And now fetch some useful registry entries */
			size = sizeof(driverkey);
			driverkey[0] = 0;
            DWORD dataType;
			if (!SetupDiGetDeviceRegistryProperty(devs, &devinfo, SPDRP_DRIVER, 
												  &dataType, (LPBYTE)driverkey, size, 0)) 
			{
				SetupDiDestroyDeviceInfoList(devs);
				devs = INVALID_HANDLE_VALUE;
				ret = -1;
				goto exit1;
			}
			size = sizeof(location);
			location[0] = 0;
			if (!SetupDiGetDeviceRegistryProperty(devs, &devinfo, SPDRP_LOCATION_INFORMATION,
													&dataType, (LPBYTE)location, size, 0)) 
			{
				SetupDiDestroyDeviceInfoList(devs);
				devs = INVALID_HANDLE_VALUE;
				ret = -1;
				goto exit1;
			}
			usbHandle = CreateFile(interfacename, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,
									NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED,
									NULL);
			if (usbHandle == INVALID_HANDLE_VALUE) {
				ret = -1;
				goto exit1;
			}

		}
	}

	ret = 0;
exit1:
	return ret;
}



int UsbMessenger::closeChannel()
{
	if (usbHandle != INVALID_HANDLE_VALUE) {
		CloseHandle(usbHandle);
		usbHandle = INVALID_HANDLE_VALUE;
	}
	if (devs != INVALID_HANDLE_VALUE) {
		SetupDiDestroyDeviceInfoList(devs);
		devs = INVALID_HANDLE_VALUE;
	}

	return 0;
}

unsigned int UsbMessenger::sendBuf(unsigned char *buf, int len)
{
	int ret = ZT_FAIL;

	if(usbHandle == INVALID_HANDLE_VALUE || devs == INVALID_HANDLE_VALUE) {
		ret = ZT_PORT_UNAVAILABLE;
		goto exit1;
	}

	if (hEvent == NULL) {
		ret = ZT_OTHER_ERROR;
		goto exit1;
	}

	if(WriteFileEx(usbHandle, (const char *)buf, len, &oWriteFile, CompletedWriteRoutine) == 0){
		ret = ZT_WRITE_PORT_FAIL;
		goto exit1;
	}

	DWORD dwWait;
	DWORD exitCode = 0;
	while(1){
		if(exitCode) break;

		dwWait = WaitForSingleObjectEx( 
			 hEvent,  // event object to wait for 
			5000,       // waits indefinitely 
			TRUE);          // alertable wait enabled 
		switch(dwWait){
			case WAIT_OBJECT_0:
				break;
			case WAIT_IO_COMPLETION:
				exitCode = WAIT_IO_COMPLETION;
				break;
			case WAIT_TIMEOUT:
				exitCode = WAIT_TIMEOUT;
				break;
			default:
				break;
		}
	}
	if(exitCode == WAIT_IO_COMPLETION){
		ret = ZT_SUCCESS;
		goto exit1;
	}else {
		CancelIo(usbHandle);
		ret = ZT_WRITE_TIME_OUT;
	}

    ztLastError = GetLastError();

exit1:
	return ret;
}

unsigned int UsbMessenger::recvBuf(unsigned char *buf, int maxlen)
{
	int ret = ZT_FAIL;

	if(usbHandle == INVALID_HANDLE_VALUE || devs == INVALID_HANDLE_VALUE) {
		ret = ZT_PORT_UNAVAILABLE;
		goto exit1;
	}

	if (hEvent == NULL) {
		ret = ZT_OTHER_ERROR;
		goto exit1;
	}

	if(ReadFileEx(usbHandle, buf, maxlen, &oReadFile, CompletedReadRoutine) == 0){
		ret = ZT_READ_PORT_FAIL;
		goto exit1;
	}

	DWORD dwWait;
	DWORD exitCode = 0;
	while(1){
		if(exitCode) break;

		dwWait = WaitForSingleObjectEx( 
			 hEvent,  // event object to wait for 
			5000,       // waits indefinitely 
			TRUE);          // alertable wait enabled 
		switch(dwWait){
			case WAIT_OBJECT_0:
				break;
			case WAIT_IO_COMPLETION:
				exitCode = WAIT_IO_COMPLETION;
				break;
			case WAIT_TIMEOUT:
				exitCode = WAIT_TIMEOUT;
				break;
			default:
				break;
		}
	}
	if(exitCode == WAIT_IO_COMPLETION){
		ret = ZT_SUCCESS;
		goto exit1;
	}else {
		CancelIo(usbHandle);
		ret = ZT_READ_TIME_OUT;
	}
	
    ztLastError = GetLastError();

exit1:
	return ret;
}

unsigned int UsbMessenger::getStatus(int statusId, int *pStatus)
{
    UCHAR status[16];
    int nbyte;
    BOOL bResult;
    DWORD dwIoControlCode;

    if(statusId == 0)
    {
        dwIoControlCode = IOCTL_USBPRINT_GET_LPT_STATUS;
    }
    else
    {
        dwIoControlCode = IOCTL_USBPRINT_VENDOR_GET_COMMAND;
    }

    bResult = DeviceIoControl(
    (HANDLE)usbHandle, 
    dwIoControlCode, // dwIoControlCode 
    &statusId, // lpInBuffer
    1, // nInBufferSize
    (PUCHAR)status,
    (DWORD)4, 
    (LPDWORD)&nbyte,
    (LPOVERLAPPED) NULL 
  );

    *pStatus = status[0];

    if (!bResult) 
    { 
        // Deal with the error code. 
        ztLastError = GetLastError() ;
        return ZT_GET_PRINTER_STATUS_FAIL;
    }

    return ZT_SUCCESS;
}

unsigned int UsbMessenger::recoverPrinterStatus(int commandId)
{
    UCHAR status[16];
    int nbyte;
    BOOL bResult;
    DWORD dwIoControlCode;

    dwIoControlCode = IOCTL_USBPRINT_VENDOR_GET_COMMAND;
    commandId += 0x80;

    bResult = DeviceIoControl(
    (HANDLE)usbHandle, 
    dwIoControlCode, // dwIoControlCode 
    &commandId, // lpInBuffer
    1, // nInBufferSize
    (PUCHAR)status,
    (DWORD)4, 
    (LPDWORD)&nbyte,
    (LPOVERLAPPED) NULL 
  );

    if (!bResult) 
    { 
        // Deal with the error code. 
         ztLastError = GetLastError() ;
        return ZT_RECOVER_PRINTER_FAIL;
    }

    return ZT_SUCCESS;
}

#else
/*************************linux definition **********************************************************/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <sys/ioctl.h>
#include     <unistd.h>     
#include     <fcntl.h>      
#include     <termios.h>    
#include     <errno.h>      

#include     <linux/lp.h>      

#include "UsbMessenger.h"

UsbMessenger::UsbMessenger(int portNo)
    : timeoutInterval(5) 
{
    openChannel(portNo);
}

UsbMessenger::~UsbMessenger(void)
{
    closeChannel();
}


int UsbMessenger::openChannel(int portNo)
{
    string portName;
    char strPortNo[3];

    ztLastError = 0;

    if(portNo < 1 || portNo > 16)
    {
        return ZT_FAIL;
    }
    portName.append("/dev/usb/lp");
    sprintf(strPortNo, "%d", portNo - 1);
    portName.append(strPortNo);

    ztLastError = 0;

    fd = open(portName.c_str(), O_RDWR|O_NONBLOCK);

    if (fd == -1)
    {
        //Could not open the port.
        return ZT_FAIL;
    }

    return ZT_SUCCESS;
}



int UsbMessenger::closeChannel()
{
    if(fd != -1)
    { 
        usleep(5000);
        close(fd);
    }
    return ZT_SUCCESS;
}

unsigned int UsbMessenger::sendBuf(unsigned char *buf, int len)
{
    int sentLen;
    int totalSentLen;
    int ret;
    fd_set set;
    struct timeval timeout;
    int rv;

    ret = ZT_SUCCESS;

    if(fd == -1)
    {
        return ZT_PORT_UNAVAILABLE;
    }

    FD_ZERO(&set); /* clear the set */
    FD_SET(fd, &set); /* add our file descriptor to the set */

    timeout.tv_sec = timeoutInterval;
    timeout.tv_usec = 0;

    totalSentLen = 0;
    while(totalSentLen < len)
    {
        rv = select(fd + 1, NULL, &set, NULL, &timeout);
        if(rv == -1)
        {
            ztLastError = errno;
            ret = ZT_READ_PORT_FAIL;
            break;
        }
        else if(rv == 0)
        {
            ret = ZT_READ_TIME_OUT;
            break;
        }
        else
        {
            sentLen = write(fd, buf + totalSentLen, len - totalSentLen);
            if(sentLen > 0)
            {
                totalSentLen += sentLen;
            }
            else
            {
                ztLastError = errno;
                ret = ZT_READ_PORT_FAIL;
                break;
            }
        }
    }

    return ret;
}

unsigned int UsbMessenger::recvBuf(unsigned char *buf, int maxlen)
{
    int readLen;
    int totalReadLen;
    int ret;
    fd_set set;
    struct timeval timeout;
    int rv;

    ret = ZT_SUCCESS;

    if(fd == -1)
    {
        return ZT_PORT_UNAVAILABLE;
    }

    FD_ZERO(&set); /* clear the set */
    FD_SET(fd, &set); /* add our file descriptor to the set */

    timeout.tv_sec = timeoutInterval;
    timeout.tv_usec = 0;


    totalReadLen = 0;
    while(totalReadLen < maxlen)
    {
        rv = select(fd + 1, &set, NULL, NULL, &timeout);
        if(rv == -1)
        {
            ztLastError = errno;
            ret = ZT_READ_PORT_FAIL;
            break;
        }
        else if(rv == 0)
        {
            ret = ZT_READ_TIME_OUT;
            break;
        }
        else
        {
            readLen = read(fd, buf + totalReadLen, maxlen - totalReadLen);
            if(readLen > 0)
            {
                totalReadLen += readLen;
            }
            else
            {
                ztLastError = errno;
                ret = ZT_READ_PORT_FAIL;
                break;
            }
        }
    }

    return ret;
}

unsigned int UsbMessenger::getStatus(int statusId, int *pStatus)
{
    unsigned char buf[4];
    unsigned long ret;

    buf[0] = 0x10;
    buf[1] = 0x04;
    buf[2] = statusId;

    ret = sendBuf(buf, 3);
    if(ret == ZT_SUCCESS)
    {
        ret = recvBuf(buf, 1);
    }

    if(ret == ZT_SUCCESS)
    {
        *pStatus = buf[0];
    }
    else
    {
        //if fails, try another way
        if(statusId == 1)
        {
            //LP_PBUSY     inverted busy input, active high
            //LP_PACK      unchanged acknowledge input, active low
            //LP_POUTPA    unchanged out-of-paper input, active high
            //LP_PSELECD   unchanged selected input, active high
            //LP_PERRORP   unchanged error input, active low
            if(ioctl(fd, LPGETSTATUS, pStatus) >= 0)
            {
                ret = ZT_GETSTATUS_VIA_CONTROL;
            }
           else
            {
                ret = ZT_FAIL;
                ztLastError = errno;
            }
        }
        else
        {
            pStatus = 0;
        }
    }

    return ret;
}

unsigned int UsbMessenger::recoverPrinterStatus(int commandId)
{
    unsigned char buf[4];
    unsigned long ret;

    buf[0] = 0x10;
    buf[1] = 0x05;
    buf[2] = commandId;

    ret = sendBuf(buf, 3);

    if(ret != ZT_SUCCESS)
    {
        ztLastError = errno;
    }

    return ret;
}

void UsbMessenger::setTimeOut(int timeout)
{
}

#endif //WIN32
