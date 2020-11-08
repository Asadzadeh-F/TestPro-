#include "MessengerFactory.h"
#include "UsbMessenger.h"
#include "ComMessenger.h"
#include "EthernetMessenger.h"

#include <typeinfo>
#include <stdio.h>

CommParameters MessengerFactory::commPara;

MessengerFactory::MessengerFactory(void)
{
}

MessengerFactory::~MessengerFactory(void)
{
}

void MessengerFactory::setCommParameters(CommParameters commpara)
{
    MessengerFactory::commPara = commpara;
}

CommParameters MessengerFactory::getCommParameters()
{
    return commPara;
}

Messenger* MessengerFactory::buildMessenger()
{
    CommPortType type;
    int portno;
    int baudrate;
    int flowcontrol;

    type = MessengerFactory::commPara.porttype; 

	if(COM == type )
    {
        portno = commPara.portno;
        baudrate = commPara.m_baudrate;
        flowcontrol = commPara.m_flowcontrol;

        ComMessenger* messenger = NULL;
        messenger = new ComMessenger(portno, baudrate, flowcontrol);
        return messenger;
    }
    else if(USB == type)
    {
    	portno = commPara.portno;
        UsbMessenger* messenger = new UsbMessenger(portno);
        return messenger; 
    }
    else if(ETHERNET == type)
    {
        //ip -- 127.0.0.1, port = 9100
        EthernetMessenger  *messenger = new EthernetMessenger(0x7f000001, 9100);
        return messenger; 
    }


        return NULL;
}


void MessengerFactory::releaseMessenger(Messenger *messenger)
{
    if(messenger != NULL)
    {
		delete messenger;
	}
}
