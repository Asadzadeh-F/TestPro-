#ifndef __PRINTER_CMD__
#define __PRINTER_CMD__


class Cmd
{
public:
	Cmd(void);
	virtual ~Cmd(void);

	virtual int getLen();
	virtual unsigned char* getBuf();
	virtual void setLen(int len);
	virtual void setBuf(unsigned char *buf);

protected:
	unsigned char *buf;
	int len;
};



#endif //__PRINTER_CMD__
