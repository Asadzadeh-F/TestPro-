#include "ZtImage.h"

using namespace std;

ZtImage::ZtImage(void)
{
    loaded = false;
}

ZtImage::~ZtImage(void)
{
}


long ZtImage::Load(const char* pszFileName)
{
    loaded = m_bmp.ReadFromFile(pszFileName);

    return loaded;
}

int ZtImage::GetWidth() 
{
    if(loaded)
    {
        return m_bmp.TellWidth();
    }
    else
    {
        return 0;
    }
}

int ZtImage::GetHeight()
{
    if(loaded)
    {
        return m_bmp.TellHeight();
    }
    else
    {
        return 0;
    }
}

COLORREF ZtImage::GetPixel( int x, int y )
{
    RGBApixel pixel;
    
    if(loaded)
    {
        pixel = m_bmp.GetPixel(x, y);
    }
    else
    {
        pixel.Red = 255;
        pixel.Green = 255;
        pixel.Blue = 255;
    }

    return RGB(pixel.Red, pixel.Green, pixel.Blue);
}
