#include "PrintLogoCmd.h"

static unsigned char cmdbuf[] = {0x1c, 'p', 0, 0}; 

PrintLogoCmd::PrintLogoCmd(int mode) 
{
	buf = cmdbuf;
	len = sizeof(cmdbuf);
    cmdbuf[3] = mode;
}

PrintLogoCmd::~PrintLogoCmd(void)
{
}

unsigned char* PrintLogoCmd::getBuf()
{
	return buf;
}

int PrintLogoCmd::getLen()
{
	return len;
}

void PrintLogoCmd::setLogoNo(int no)
{
    cmdbuf[2] = no;
}