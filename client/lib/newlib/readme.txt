for nfc reader the following API can be use:

extern int vpcard_rfmem_active(int delay_ms, char *card_type, char *uid, char *ATR);
extern int vpcard_rfmem_halt(int delay_ms);
extern int vpcard_rfmem_pinauth( unsigned char sector,unsigned char pintype, unsigned char *pindata);
extern int vpcard_rfmem_read_block(unsigned char block,unsigned char * data);
extern int vpcard_rfmem_write_block(unsigned char block,unsigned char * data);

see step==6 in main.c file for the detail information. 