#ifndef __VPEPP_INTERFACE_H__
#define __VPEPP_INTERFACE_H__

#define VPRET_EPP_SUCCESS           0                       //成功
#define VPRET_EPP_BASE          (-1100)                 //错误基数
#define VPRET_EPP_FAIL          (ERR_KBD_BASE + (-1))   //打开设备失败
#define VPRET_EPP_KEYSINFO      (ERR_KBD_BASE + (-2))   //打开密钥文件失败
#define VPRET_EPP_NOTSUPPORT    (ERR_KBD_BASE + (-3))   //不支持的功能

#define VERSION "VP598B V1.00"

#define WFS_MAC_X99      1    //X9.9/X9.19
#define WFS_MAC_SAM      2    //SAM卡算法
#define WFS_MAC_PBOC     3    //PBOC算法
#define WFS_MAC_UNIONPAY 4    //银联算法
#define WFS_MAC_CBC      5    //CBC算法

extern "C"
{
extern int  vpepp_open(char * dev);
extern int  vpepp_close(void);
extern int  vpepp_reset(char flag);
extern int  vpepp_enable(int p_iOpen, int p_iVoice);
extern int  vpepp_getkey(char *p_caKey, char *p_caDamage);
extern int  vpepp_version(char *ver);
extern int  vpepp_load_keymap(int mode,char *keys);
extern int  vpepp_clearkeys();
extern int  vpepp_setting (int p,int f);           
extern int  vpepp_load_masterkey(int mk_idx, char *mk,int mk_ln, char *chk_str);           
extern int  vpepp_load_workkey(int mk_idx, int wk_idx, char *wk, int wk_ln,char *chk_str);
extern int  vpepp_encrypt(char *instr, int inlen,char *outstr, int mk_idx, int wk_idx, int ecb_or_cbc,int key_ln);
extern int  vpepp_decrypt(char *instr, int inlen,char *outstr, int mk_idx, int wk_idx, int ecb_or_cbc,int key_ln);
extern int  vpepp_datamac(char *indata, int inlen, char *outData,int mk_idx, int wk_idx,int mac_type, int key_ln);
extern int  vpepp_start_pwdinput(int p_iMkIdx, int p_iWkIdx, int p_iFormat,int p_iMinln, int p_iMaxln, const char *p_caCardNo, int p_iSecond,int p_iKeyln);
extern int  vpepp_get_password(char *pwd);
extern int  vpepp_term_accredit( const char *p_caRand );
};
#endif  //__VPMATRIX_H__
