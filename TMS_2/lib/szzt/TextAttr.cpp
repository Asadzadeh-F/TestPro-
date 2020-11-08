#include "TextAttr.h"
#include "szztplib.h"

TextAttr::TextAttr(void)
{
    buf = new unsigned char [12];
    len = 12;

    buf[0] = 0x1b;
    buf[1] = 0x21;
    buf[2] = 0;

    buf[3] = 0x1b;
    buf[4] = 0x56;
    buf[5] = 0;

    buf[6] = 0x1d;
    buf[7] = 0x42;
    buf[8] = 0;

    buf[9] = 0x1b;
    buf[10] = 0x7b;
    buf[11] = 0;
}

TextAttr::~TextAttr(void)
{
    if(buf)
        delete []buf;
}

void TextAttr::setAttr(unsigned long attribute)
{
    buf[2] = (unsigned char)attribute;

    if(attribute & TEXT_ROTATE90)
    {
        buf[5] = 1;
    }
    else
    {
        buf[5] = 0;
    }

    if(attribute & TEXT_REVERSE)
    {
        buf[8] = 1;
    }
    else
    {
        buf[8] = 0;
    }

    if(attribute & TEXT_UPSIDE_DOWN)
    {
        buf[11] = 1;
    }
    else
    {
        buf[11] = 0;
    }

}