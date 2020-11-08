#ifndef __PRINT_GRAPH_CMD__
#define __PRINT_GRAPH_CMD__

#include "Cmd.h"

#include "ZtImage.h"


class PrintGraphCmd :
	public Cmd
{
public:

    PrintGraphCmd(const char *imageFileName);
	PrintGraphCmd(const char *imageFileName, int mode);
	PrintGraphCmd(const char *imageFileName, int height, int m);

	virtual ~PrintGraphCmd(void);

	virtual unsigned char* getBuf();
	virtual int getLen();

	int getSteps();
	unsigned char* getStepNBuf(int i);
	int getStepLen(int i);

    ZtImage m_image;
private:
	void convertBmpToData(unsigned char *buf, int line);
	int lineheight;
	int mode;
};


#endif //__PRINT_GRAPH_CMD__

