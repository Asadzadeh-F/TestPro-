#ifndef __PRINT_LOGO_CMD__
#define __PRINT_LOGO_CMD__

#include "Cmd.h"

class PrintLogoCmd :
	public Cmd
{
public:
	PrintLogoCmd(int mode);
	~PrintLogoCmd(void);

	virtual unsigned char* getBuf();
	virtual int getLen();

    void setLogoNo(int option);
private:

};

#endif //__PRINT_LOGO_CMD__
