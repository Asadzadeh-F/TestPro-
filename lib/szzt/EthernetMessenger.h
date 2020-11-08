#ifndef __ETHERNET_MESSENGER__
#define __ETHERNET_MESSENGER__

#ifdef WIN32
#include "WinMessenger.h"

class EthernetMessenger :
	public WinMessenger
{
public:
    EthernetMessenger(unsigned long tIp, int tPort);
    virtual ~EthernetMessenger(void);
    virtual int openChannel();
    virtual int closeChannel();

    int port;
    unsigned long ip;

    virtual unsigned int sendBuf(unsigned char *buf, int len);

private:
    SOCKET sock;
    bool channelopened;
};

#else
#include "Messenger.h"
#include <sys/socket.h>
class EthernetMessenger :
	public Messenger
{
public:
    EthernetMessenger(unsigned long tIp, int tPort);
    virtual ~EthernetMessenger(void);
    virtual int openChannel();
    virtual int closeChannel();

    int port;
    unsigned long ip;

    unsigned int sendBuf(unsigned char *buf, int len);
    unsigned int recvBuf(unsigned char *buf, int maxlen);
    unsigned int getStatus(int statusId, int *pStatus);
    unsigned int recoverPrinterStatus(int commandId);
    void setTimeOut(int timeout);	

private:
    int sock;
    bool channelopened;
};
#endif


#endif //__ETHERNET_MESSENGER__
