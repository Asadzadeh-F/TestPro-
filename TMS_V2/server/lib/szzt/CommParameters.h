#ifndef __COMM_PARAMETERS__
#define __COMM_PARAMETERS__

enum CommPortType
{
    COM,
    USB,
    ETHERNET
};

enum BaudRate
{
    BAUD_RATE_4800,
    BAUD_RATE_9600,
    BAUD_RATE_19200,
    BAUD_RATE_38400,
    BAUD_RATE_57600,
    BAUD_RATE_115200
};

enum Parity
{
    MY_PARITY_ODD,
    MY_PARITY_EVEN,
    MY_PARITY_NONE
};


enum FlowControl
{
    FLOWCONTROL_NONE,
    FLOWCONTROL_HARDWARE
};


class CommParameters
{
public:
    CommPortType porttype;
    int portno;
    BaudRate m_baudrate;
    Parity m_parity;
    FlowControl m_flowcontrol;

    CommParameters();
    ~CommParameters();
};



#endif //' __COMM_PARAMETERS__
