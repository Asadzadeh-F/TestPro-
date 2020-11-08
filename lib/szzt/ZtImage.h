#ifndef __ZT__IMAGE__
#define __ZT__IMAGE__

#include "EasyBMP.h"

#ifdef WIN32
	#include <windows.h>
#else
	typedef unsigned long   COLORREF;
#define LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((long)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

#define RGB(r,g,b)          ((COLORREF)(((unsigned char)(r)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))

#endif


class ZtImage
{
public:
    ZtImage(void);
    ~ZtImage(void);

    long Load(const char* pszFileName);
	int GetWidth();
	int GetHeight() ;
	COLORREF GetPixel( int x, int y );

private:
    BMP m_bmp;
    bool loaded;
};


#endif //__ZT__IMAGE__

