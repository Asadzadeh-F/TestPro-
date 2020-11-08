#ifndef __VPSTRING_H__
#define __VPSTRING_H__

#include "vpmutex.h"

/*线程安全的*/

typedef enum
{
    TRIM_LEFT        = 1,    //去前导0x20, 0x09
    TRIM_RIGHT       = 2,    //去末尾0x20, 0x09, 0x0D, 0x0A
    TRIM_LEFT_RIGHT  = 3     //去前后空字符
}TTrimMode;

class VPString
{
public:
    VPString(const char* p_caName);
    virtual ~VPString();

    static int   hex2dec(const char* p_caHex, char* p_caDec);
           int   hex2dec(char* p_caDec);
           void  hex2dec();
    static void  dec2hex(const char* p_caDec, int p_iLn, char* p_caHex);
           void  dec2hex(char* p_caHex);
           void  dec2hex();
    static void  trim(char* p_caInStr, char* p_caOutStr, TTrimMode p_stMode);
           void  trim(char* p_caOutStr, TTrimMode p_stMode);
           void  trim(TTrimMode p_stMode);
    static bool  is_numeric(const char* p_caStr);
           bool  is_numeric();
    static void  exec_time(const char *p_caCaption);
    static int   fieldn(const char *p_caSrc, int p_iSrcLn, char p_cCh, int p_iN, char *p_caOut);
    static int   fieldn(const char *p_caSrc, int p_iSrcLn, char p_cCh);
    static void  show_menu(const char *p_caMenuStr);

private:
    int       c_iLn;
    char*     c_cpName;
    char*     c_cpValue;
    VPMutex*  c_mutex;

};



#endif  //__VPSTRING_H__
