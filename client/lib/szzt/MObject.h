#ifndef _MOBJECT_H_
#define _MOBJECT_H_

class MObject
{
public:
	MObject(void);
	virtual ~MObject(void);

	virtual unsigned char* getBuf();
	virtual int getLen();
protected:
	unsigned char *m_buf;
	int m_buflen;
};
#endif