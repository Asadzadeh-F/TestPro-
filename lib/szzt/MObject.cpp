#include "MObject.h"
#include <string>
MObject::MObject(void):m_buf(NULL),m_buflen(0)
{
}

MObject::~MObject(void)
{
}


unsigned char* MObject::getBuf()
{
	return NULL;
}

int MObject::getLen()
{
	return 0;
}
