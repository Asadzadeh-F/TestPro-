#include "Cmd.h"

Cmd::Cmd(void) : buf(0)
	,len(0)

{
}

Cmd::~Cmd(void)
{
}

unsigned char* Cmd::getBuf()
{
	return buf;
}

int Cmd::getLen()
{
	return len;
}

void Cmd::setBuf(unsigned char *buf)
{
	this->buf = buf;
}

void Cmd::setLen(int len)
{
	this->len = len;
}

