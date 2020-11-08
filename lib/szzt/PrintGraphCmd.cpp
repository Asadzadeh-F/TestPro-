#include "PrintGraphCmd.h"

struct GraphCmdHead{
	char cmdId[2];
	char mode;
	char length[2];
};

#define LINEHEIGHT 24

PrintGraphCmd::PrintGraphCmd(const char *imageFileName):
	lineheight(LINEHEIGHT),
	mode(33)
{
	struct GraphCmdHead cmdhead;

    try{
        buf = NULL;
        m_image.Load(imageFileName);

	    cmdhead.cmdId[0] = 0x1b;
	    cmdhead.cmdId[1] = 0x2a;
	    cmdhead.mode = mode;
	    cmdhead.length[0] = m_image.GetWidth() % 256;
	    cmdhead.length[1] = m_image.GetWidth() / 256;

	    //allocate mem for buffer;
	    len = sizeof(struct GraphCmdHead) + m_image.GetWidth() * (lineheight/8);

	    buf = new unsigned char[len];

    }catch(...)
    {
        if(buf != NULL)
        {
            delete []buf;
            buf = NULL;
        }
    }

    if(buf != NULL){
	    memcpy(buf, &cmdhead, sizeof(struct GraphCmdHead));
    }
}

PrintGraphCmd::PrintGraphCmd(const char *imageFileName, int mode)
{
	struct GraphCmdHead cmdhead;

    try{
        buf = NULL;

        m_image.Load(imageFileName);

	    cmdhead.cmdId[0] = 0x1b;
	    cmdhead.cmdId[1] = 0x2a;
	    cmdhead.mode = mode;
	    cmdhead.length[0] = m_image.GetWidth() % 256;
	    cmdhead.length[1] = m_image.GetWidth() / 256;

        if(mode == 0 || mode == 1)
        {
            lineheight = LINEHEIGHT / 3;
        }
        else
        {
            lineheight = LINEHEIGHT;
        }

	    //allocate mem for buffer;
	    len = sizeof(struct GraphCmdHead) + m_image.GetWidth() * (lineheight/8);

	    buf = new unsigned char[len];
    }catch(...)
    {
        if(buf != NULL)
        {
            delete []buf;
            buf = NULL;
        }
    }

    if(buf != NULL){
	    memcpy(buf, &cmdhead, sizeof(struct GraphCmdHead));
    }
}


PrintGraphCmd::PrintGraphCmd(const char *imageFileName, int height, int m):
	lineheight(height),
	mode(m)
{
	struct GraphCmdHead cmdhead;

    try{
        buf = NULL;
        m_image.Load(imageFileName);

	    cmdhead.cmdId[0] = 0x1b;
	    cmdhead.cmdId[1] = 0x2a;
	    cmdhead.mode = m;
	    cmdhead.length[0] = m_image.GetWidth() % 256;
	    cmdhead.length[1] = m_image.GetWidth() / 256;

	    //allocate mem for buffer;
	    len = sizeof(struct GraphCmdHead) + m_image.GetWidth() * (lineheight/8);

	    buf = new unsigned char[len];
    }catch(...)
    {
        if(buf != NULL)
        {
            delete []buf;
            buf = NULL;
        }
    }

    if(buf != NULL){
	    memcpy(buf, &cmdhead, sizeof(struct GraphCmdHead));
    }
}


PrintGraphCmd::~PrintGraphCmd(void)
{
    if(buf)
    {
        delete []buf;
    }
}



int PrintGraphCmd::getSteps(){
    return (m_image.GetHeight() + lineheight - 1) / lineheight;
}

unsigned char* PrintGraphCmd::getStepNBuf(int i){
    int offset = sizeof(struct GraphCmdHead);
    if(buf)
    {
        memset(buf + offset, 0, len - offset);
        convertBmpToData(buf + offset, i);
    }
    return buf;
}

int PrintGraphCmd::getStepLen(int i)
{
    return len;
}

unsigned char* PrintGraphCmd::getBuf()
{
    return buf;
}

int PrintGraphCmd::getLen()
{
    return len;
}


void PrintGraphCmd::convertBmpToData(unsigned char *buf, int line)
{
	int ptr = 0;

	for(int i=0; i<m_image.GetWidth(); i++){
		unsigned char v;
		COLORREF color;
		COLORREF bwcolor;

		int j, k;
		
		v = 0;

		int start = line * lineheight;
		int actualH = (start + lineheight) <= m_image.GetHeight() ? lineheight : (m_image.GetHeight() - start);
		int end = start + actualH;

		for(j = start,k=0; j < end; j++,k++){
			color = m_image.GetPixel(i,j);
			bwcolor = GetRValue(color)*0.299 + GetGValue(color)*0.587 + GetBValue(color)*0.114;
			v <<= 1;
            //if one of the color is predominant
            if(GetRValue(color) < 127 || GetGValue(color) < 127 || GetBValue(color) < 127 )
            {
                v++;
			}
            //or the overall color looks dark enough
            else if(bwcolor < 127)
            {
                v++;
            }   
			if((k & 7) == 7){
				if(ptr < (int)(len - sizeof(struct GraphCmdHead))){
					buf[ptr++] = v;
				}
				v = 0;
			}
		}
		if(k%8){
			v <<= (8-k%8);
			if(ptr < (int)(len - sizeof(struct GraphCmdHead))){
				buf[ptr++] = v;
			}
		}
		if(k <= lineheight - 8){
			buf[ptr++] = 0;
		}
		if(k <= lineheight - 16){
			buf[ptr++] = 0;
		}
	}
}


