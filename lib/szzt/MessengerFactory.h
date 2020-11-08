#pragma once

#include "Messenger.h"
#include "CommParameters.h"

class MessengerFactory
{
private:
    static CommParameters commPara;
public:
    MessengerFactory(void);
    ~MessengerFactory(void);

    static void setCommParameters(CommParameters commpara);
    static CommParameters getCommParameters();
    
    static Messenger* buildMessenger();

    static void releaseMessenger(Messenger *messenger);
};
