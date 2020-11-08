#ifndef __COM_MESSENGER__
#define __COM_MESSENGER__

#ifdef WIN32
#include "WinMessenger.h"

class ComMessenger :
	public WinMessenger

{
public:
    ComMessenger(int portNo,  int baudRate);
    ComMessenger(int portNo, int baudRate, int flowControl);
    ~ComMessenger(void);

    unsigned int sendBuf(unsigned char *buf, int len);
    unsigned int recvBuf(unsigned char *buf, int maxlen);
    unsigned int getStatus(int statusId, int *pStatus);
    unsigned int recoverPrinterStatus(int commandId);
    void setTimeOut(int timeout);

private:
    int openChannel(int i,int baudrate, int flowControl);
    int closeChannel();
};

#else
#include "Messenger.h"

class ComMessenger :
        public Messenger
{
public:
    ComMessenger(int portNo,  int baudRate);
    ComMessenger(int portNo, int baudRate, int flowControl);
    ~ComMessenger(void);

    unsigned int sendBuf(unsigned char *buf, int len);
    unsigned int recvBuf(unsigned char *buf, int maxlen);
    unsigned int getStatus(int statusId, int *pStatus);
    unsigned int recoverPrinterStatus(int commandId);
    virtual void setTimeOut(int timeout);

private:
    int openChannel(int i,int baudrate, int flowControl);
    int closeChannel();
    int sessionError;
    int fd;
    int timeoutInterval;
};
#endif

#endif //__COM_MESSENGER__
