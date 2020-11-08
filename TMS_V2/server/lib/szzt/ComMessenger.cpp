#include "ComMessenger.h"
#include "CommParameters.h"
#include "zterrorcode.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
 
using namespace std;

#ifdef WIN32
#include <tchar.h>

#define DEFAULT_BAUD_RATE 5
#define RECEIVE_TIMEOUT 5000

static const int BAUDRATE[] = {CBR_4800, CBR_9600, CBR_19200, CBR_38400, CBR_57600, CBR_115200};


extern VOID WINAPI CompletedWriteRoutine(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap);
extern VOID WINAPI CompletedReadRoutine(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap);

ComMessenger::ComMessenger(int portNo, int baudRate)
{
    timeout = 2000; //2 seconds
	openChannel(portNo, baudRate, 1);
}

ComMessenger::ComMessenger(int portNo, int baudRate, int flowControl)
{
    timeout = 2000; //2 seconds
	openChannel(portNo, baudRate, flowControl);
}


ComMessenger::~ComMessenger(void)
{
	closeChannel();
}


int ComMessenger::openChannel(int portNo, int baudrate, int flowControl)
{
#ifdef UNICODE
    wstring portName;
#else
    string portName;
#endif
    TCHAR strPortNo[3];

    if(portNo > 99)
    {
        return FAIL;
    }
    portName.append(TEXT("COM"));
    _stprintf(strPortNo, _T("%d"), portNo);
    portName.append(strPortNo);
    portName.append(TEXT(":"));

    BOOL ret;
    COMMCONFIG  config;
    DWORD configSize;
    configSize = sizeof(COMMCONFIG);
    ret = GetDefaultCommConfig(portName.substr(0, 4).c_str(), &config, &configSize);

    if(ret == FALSE) {
        ztLastError = GetLastError();

        return ZT_FAIL;
    }

    config.dcb.DCBlength = sizeof(DCB);
    config.dcb.fBinary = 1;

    config.dcb.BaudRate = BAUDRATE[baudrate];
    config.dcb.ByteSize = 8;
    config.dcb.Parity = NOPARITY;
    config.dcb.StopBits = ONESTOPBIT;
    if(flowControl == FLOWCONTROL_HARDWARE)
    {
        config.dcb.fOutxDsrFlow = 0;
        config.dcb.fDtrControl = 0;
        config.dcb.fOutxCtsFlow = 1;
        config.dcb.fRtsControl = 1;
    }
    else
    {
        config.dcb.fOutxDsrFlow = 0;
        config.dcb.fDtrControl = 0;
        config.dcb.fOutxCtsFlow = 0;
        config.dcb.fRtsControl = 0;
    }
    config.dcb.fOutX = 0;
    config.dcb.fInX = 0;
    config.dcb.EofChar = 0;
    config.dcb.EvtChar = 0;
    config.dcb.XonChar = 0;
    config.dcb.XoffChar = 0;
    config.dcb.XoffLim = 0;
    config.dcb.XonLim = 0;
    config.dcb.fErrorChar = 0;
    config.dcb.ErrorChar = 0;
    config.dcb.fNull = 0;

    ret = SetDefaultCommConfig(portName.substr(0, 4).c_str(), &config, configSize);
    if(ret == FALSE) {
        ztLastError = GetLastError();

        return ZT_FAIL;
    }

    hCom = CreateFile(portName.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);

    PurgeComm(hCom, PURGE_RXABORT | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR);

    if(INVALID_HANDLE_VALUE != hCom) 
    {
	    DCB dcbSerialParams = {0};

	    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
	    if (GetCommState(hCom, &dcbSerialParams)) {
		    dcbSerialParams.BaudRate = BAUDRATE[baudrate];
		    dcbSerialParams.ByteSize = 8;
		    dcbSerialParams.Parity = NOPARITY;
		    dcbSerialParams.StopBits = ONESTOPBIT;

            if(flowControl == FLOWCONTROL_HARDWARE)
            {
                dcbSerialParams.fOutxDsrFlow = 0;
                dcbSerialParams.fDtrControl = 0;
                dcbSerialParams.fOutxCtsFlow = 1;
                dcbSerialParams.fRtsControl = 1;
            }
            else
            {
                dcbSerialParams.fOutxDsrFlow = 0;
                dcbSerialParams.fDtrControl = 0;
                dcbSerialParams.fOutxCtsFlow = 0;
                dcbSerialParams.fRtsControl = 0;
            }
            dcbSerialParams.fOutX = 0;
            dcbSerialParams.fInX = 0;
            dcbSerialParams.EofChar = 0;
            dcbSerialParams.EvtChar = 0;
            dcbSerialParams.XonChar = 0;
            dcbSerialParams.XoffChar = 0;
            dcbSerialParams.XoffLim = 0;
            dcbSerialParams.XonLim = 0;
            dcbSerialParams.fErrorChar = 0;
            dcbSerialParams.ErrorChar = 0;
            dcbSerialParams.fNull = 0;

		    if(!SetCommState(hCom, &dcbSerialParams))
            {
                //error setting serial port state
    		    CloseHandle(hCom);
                hCom = INVALID_HANDLE_VALUE;
            }

	    }

        //SetCommMask(hCom, EV_RXCHAR);
    }

    COMMTIMEOUTS commTimeouts;

    if(INVALID_HANDLE_VALUE != hCom) 
    {
        GetCommTimeouts(hCom, &commTimeouts);
        commTimeouts.ReadIntervalTimeout = 0;
        commTimeouts.ReadTotalTimeoutMultiplier = 0;
        commTimeouts.ReadTotalTimeoutConstant = RECEIVE_TIMEOUT;
        commTimeouts.WriteTotalTimeoutMultiplier = 0;
        commTimeouts.WriteTotalTimeoutConstant = timeout;
        SetCommTimeouts(hCom, &commTimeouts);

    }

    if(INVALID_HANDLE_VALUE != hCom) 
    {
    	return ZT_SUCCESS;
    }
    else
    {
        ztLastError = GetLastError();
        return ZT_FAIL;
    }
}

int ComMessenger::closeChannel()
{
	if(hCom != INVALID_HANDLE_VALUE){
		CloseHandle(hCom);
	}
	return 0;
}

unsigned int  ComMessenger::recvBuf(unsigned char *buf, int maxlen)
{
	int ret;
    int loopTimes = 0;

	if(hCom == INVALID_HANDLE_VALUE)
    {
		return ZT_PORT_UNAVAILABLE;
	}

    memset(buf, 0,maxlen);
    DWORD nBytesRead;

    BOOL  bResult;
    
    bResult = ReadFile(hCom, buf, maxlen, &nBytesRead, NULL) ;
    
    /*DWORD dwEvtMask;
    DWORD error;
    COMSTAT comStat;

    oReadFile.Internal = 0;
    oReadFile.InternalHigh = 0;
    oReadFile.Offset = 0;
    oReadFile.OffsetHigh = 0;

    oReadFile.hEvent = CreateEvent(
        NULL,   // default security attributes 
        TRUE,   // manual-reset event 
        FALSE,  // not signaled 
        NULL    // no name
		);

    WaitCommEvent(hCom, &dwEvtMask, &oReadFile);
    if(dwEvtMask & EV_RXCHAR)
    {
        int i;
        i = oReadFile.Internal;
        i = oReadFile.Offset;
        ReadFile(hCom, buf, maxlen, &nBytesRead, NULL) ;
    }

    ClearCommError(hCom, &error, &comStat);
    ClearCommError(hCom, &error, &comStat);

    //WaitCommEvent(hCom, &dwEvtMask, &oReadFile);
    */

    if(bResult && nBytesRead > 0)
    {
		ret = ZT_SUCCESS;
    }
    else 
    {
		ret = ZT_READ_PORT_FAIL;
	}
	
	return ret;
}

unsigned int ComMessenger::sendBuf(unsigned char *buf, int len)
{
	int ret = ZT_FAIL;

	if(hCom == INVALID_HANDLE_VALUE) {
		ret = ZT_PORT_UNAVAILABLE;
		goto exit1;
	}

    DWORD nByteWritten;
    BOOL bRes = WriteFile(hCom, (const char *)buf, len, &nByteWritten, NULL);
    FlushFileBuffers(hCom);

    if(bRes && nByteWritten == len)
    {
        DWORD error;
        COMSTAT comStat; 
        //ClearCommError(hCom, &error, &comStat);
        //ClearCommError(hCom, &error, &comStat);
		ret = ZT_SUCCESS;
    }
    else
    {
        ret = ZT_WRITE_PORT_FAIL;
    }

exit1:
	return ret;
}


unsigned int ComMessenger::getStatus(int statusId, int *pStatus)
{
    UCHAR buf[4];
    DWORD ret;

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
        ztLastError = GetLastError();
    }

    return ret;
}

unsigned int  ComMessenger::recoverPrinterStatus(int commandId)
{
    UCHAR buf[4];
    DWORD ret;

    buf[0] = 0x10;
    buf[1] = 0x05;
    buf[2] = commandId;

    ret = sendBuf(buf, 3);

    if(ret != ZT_SUCCESS)
    {
        ztLastError = GetLastError();
    }

    return ret;
}

void ComMessenger::setTimeOut(int timeout)
{
    this->timeout = timeout;
}

#else

#include     <unistd.h>     
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <sys/ioctl.h>
#include     <fcntl.h>      
#include     <termios.h>    
#include     <errno.h>      
#include     <termios.h>


static const int BAUDRATE[] = {B9600, B19200, B38400, B57600, B115200};


ComMessenger::ComMessenger(int portNo, int baudRate)
    : timeoutInterval(5) //5 seconds timeout
    , sessionError(0)
{
	openChannel(portNo, baudRate, 1);
}

ComMessenger::ComMessenger(int portNo, int baudRate, int flowControl)
    : timeoutInterval(5) //5 seconds timeout
    , sessionError(0)
{
	openChannel(portNo, baudRate, flowControl);
}


ComMessenger::~ComMessenger(void)
{
    closeChannel();
}


int ComMessenger::openChannel(int portNo, int baudrate, int flowControl)
{
    string portName;
    char strPortNo[3];
    struct termios options;

    ztLastError = 0;

    if(portNo < 1 || portNo > 99)
    {
        return ZT_FAIL;
    }
    portName.append("/dev/ttyUSB");
    sprintf(strPortNo, "%d", portNo - 1);
    portName.append(strPortNo);

    fd = open(portName.c_str(), O_RDWR | O_NOCTTY /*| O_NDELAY*/);

    if (fd == -1)
    {
        //Could not open the port.
        return ZT_FAIL;
    }

    //fcntl(fd, F_SETFL, O_NDELAY);

    //Get the current options for the port...
    tcgetattr(fd, &options);

    //Set the baud rates
    if(baudrate < 0 || baudrate > 4)
    {
        baudrate = 0;
    }
    cfsetispeed(&options, BAUDRATE[baudrate]);
    cfsetospeed(&options, BAUDRATE[baudrate]);

    //Enable the receiver and set local mode...
    options.c_cflag |= (CLOCAL | CREAD);

    //8 data bits
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    //CSTOPB -- 2 stop bits (1 otherwise)
    //1 stop bit
    options.c_cflag &= ~CSTOPB;

    //no parity
    options.c_cflag &= ~PARENB;

    //use rts/cts flow control    
    if(flowControl)
    {
        options.c_cflag |= CRTSCTS;
    }
    else
    {
        options.c_cflag &= (~CRTSCTS);
    }

    //disable software flow control
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    //raw type input
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    //raw type output
    options.c_oflag &= ~OPOST;

    //disable 0d->0xa mapping
    options.c_iflag &= ~ (INLCR | ICRNL | IGNCR);
    options.c_oflag &= ~(ONLCR | OCRNL);

    options.c_cc[VTIME] = 40; /* Set timeout of 4.0 seconds */
    options.c_cc[VMIN] = 0;
    //Set the new options for the port...
    tcsetattr(fd, TCSANOW, &options);
    //tcsetattr(fd, TCSAFLUSH, &options);

    return ZT_SUCCESS;
}

int ComMessenger::closeChannel()
{
    if(fd != -1)
    {
        if(sessionError)
        {
            tcflush(fd, TCIOFLUSH); 
        }
        close(fd);
    }
    return ZT_SUCCESS;
}

unsigned int ComMessenger::sendBuf(unsigned char *buf, int len)
{
    int sentLen;
    int totalSentLen;
    int ret;

    ret = ZT_SUCCESS;

    if(fd == -1)
    {
        return ZT_PORT_UNAVAILABLE;
    }

    totalSentLen = 0;
    while(totalSentLen < len)
    {
        sentLen = write(fd, buf + totalSentLen, len);
        if(sentLen > 0)
        {
            totalSentLen += sentLen;
        }
        else
        {
            ztLastError = errno;
            sessionError = 1;
            ret = ZT_FAIL;
            break;
        }
    }

    return ret;
}

unsigned int  ComMessenger::recvBuf(unsigned char *buf, int maxlen)
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

#if 0
    FD_ZERO(&set); /* clear the set */
    FD_SET(fd, &set); /* add our file descriptor to the set */

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    totalReadLen = 0;
    while(totalReadLen < maxlen)
    {
        rv = select(fd + 1, &set, NULL, NULL, &timeout);
        if(rv == -1)
        {
            ztLastError = errno;
            sessionError = 1;
            ret = ZT_READ_PORT_FAIL;
            break;
        }
        else if(rv == 0)
        {
            sessionError = 1;
            ret = ZT_READ_TIME_OUT;
            break;
        }
        else
        {
            readLen = read(fd, buf, maxlen - totalReadLen);
            if(readLen > 0)
            {
                totalReadLen += readLen;
            }
            else
            {
                ztLastError = errno;
                sessionError = 1;
                ret = ZT_READ_PORT_FAIL;
                break;
            }
        }
    }
#endif
    readLen = read(fd, buf, maxlen);
    if(readLen <= 0)
    {
        ztLastError = errno;
        sessionError = 1;
        ret = ZT_READ_PORT_FAIL;
    }

    return ret;
}


unsigned int ComMessenger::getStatus(int statusId, int *pStatus)
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
        ztLastError = errno;
    }

    return ret;
}

unsigned int  ComMessenger::recoverPrinterStatus(int commandId)
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

void ComMessenger::setTimeOut(int timeout)
{
}

#endif
