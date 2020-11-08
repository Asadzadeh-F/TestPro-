#ifndef __ZTEPP_INTERFACE_H__
#define __ZTEPP_INTERFACE_H__

#define ZTRET_EPP_SUCCESS           0                       //成功
#define ZTRET_EPP_BASE          (-1100)                 //错误基数
#define ZTRET_EPP_FAIL          (ERR_KBD_BASE + (-1))   //打开设备失败
#define ZTRET_EPP_KEYSINFO      (ERR_KBD_BASE + (-2))   //打开密钥文件失败
#define ZTRET_EPP_NOTSUPPORT    (ERR_KBD_BASE + (-3))   //不支持的功能

#define VERSION "ZT598B V1.00"

#define WFS_MAC_X99      1    //X9.9/X9.19
#define WFS_MAC_SAM      2    //SAM卡算法
#define WFS_MAC_PBOC     3    //PBOC算法
#define WFS_MAC_UNIONPAY 4    //银联算法
#define WFS_MAC_CBC      5    //CBC算法


extern int  ztepp_open(char * dev);
extern int  ztepp_close(void);
extern int  ztepp_reset(char flag);
extern int  ztepp_enable(int p_iOpen, int p_iVoice);
extern int  ztepp_getkey(char *p_caKey, char *p_caDamage);
extern int  ztepp_version(char *ver);
extern int  ztepp_load_keymap(int mode,char *keys);
extern int  ztepp_clearkeys();
extern int  ztepp_setting (int p,int f);           
extern int  ztepp_load_masterkey(int mk_idx, char *mk,int mk_ln, char *chk_str);           
extern int  ztepp_load_workkey(int mk_idx, int wk_idx, char *wk, int wk_ln,char *chk_str);
extern int  ztepp_encrypt(char *instr, int inlen,char *outstr, int mk_idx, int wk_idx, int ecb_or_cbc,int key_ln);
extern int  ztepp_decrypt(char *instr, int inlen,char *outstr, int mk_idx, int wk_idx, int ecb_or_cbc,int key_ln);
extern int  ztepp_datamac(char *indata, int inlen, char *outData,int mk_idx, int wk_idx,int mac_type, int key_ln);
extern int  ztepp_start_pwdinput(int p_iMkIdx, int p_iWkIdx, int p_iFormat,int p_iMinln, int p_iMaxln, const char *p_caCardNo, int p_iSecond,int p_iKeyln);
extern int  ztepp_get_password(char *pwd);
extern int  ztepp_term_accredit( const char *p_caRand );

#endif  //__ZTMATRIX_H__
