#ifndef _CMDHANDLER_H_
#define _CMDHANDLER_H_
#include "Messenger.h"
#include "Cmd.h"
#include "MObject.h"
#include <string>
//*******************************************

class CmdHandler
{
public:
	CmdHandler(Messenger *messenger);
	~CmdHandler(void);

	int sendCmd(Cmd &cmd);
	int sendObject(MObject &o);
	int readCmdResult(Cmd &o);

private:
	Messenger *sender;
};

#endif
