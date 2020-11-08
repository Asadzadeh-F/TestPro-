#ifndef __MESSENGER__H
#define __MESSENGER__H

class Messenger
{
public:
    Messenger(void);
    virtual ~Messenger(void);

    virtual unsigned int sendBuf(unsigned char *buf, int len) = 0;
    virtual unsigned int recvBuf(unsigned char *buf, int maxlen) = 0;
    virtual unsigned int getStatus(int statusId, int *pStatus) = 0;
    virtual unsigned int recoverPrinterStatus(int statusId) = 0;
    virtual void setTimeOut(int timeout) = 0;

    int timeout;
};

#endif //__MESSENGER__H

