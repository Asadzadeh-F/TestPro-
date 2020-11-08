#ifndef __TEXT_ATTR_H__
#define __TEXT_ATTR_H__

#include "Cmd.h"

class TextAttr :
    public Cmd
{
public:
    TextAttr(void);
    ~TextAttr(void);
    void setAttr(unsigned long attribute);
};

#endif //__TEXT_ATTR_H__
