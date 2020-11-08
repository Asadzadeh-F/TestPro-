#include "CmdHandler.h"
CmdHandler::CmdHandler(Messenger *messenger):sender(messenger)
{

}

CmdHandler::~CmdHandler(void)
{
}

int CmdHandler::sendCmd(Cmd &cmd)
{
	return sender->sendBuf(cmd.getBuf(), cmd.getLen());
}

int CmdHandler::sendObject(MObject &o)
{
	return sender->sendBuf(o.getBuf(), o.getLen());
}

int CmdHandler::readCmdResult(Cmd  &o)
{
	return sender->recvBuf(o.getBuf(), o.getLen());
}
