#ifndef __PRINTER_API__
#define __PRINTER_API__

#include "CommParameters.h"
#include "Barcode.h"

#define CODE_PAGE_DEFAULT     0    //  0 PC437 [IBM437 OEM United States]
#define CODE_PAGE_JAPANESE    1    //  1 Japanese katakana
#define CODE_PAGE_WEST_EUROPE 2    //  2 PC850 [Western European (DOS)]
#define CODE_PAGE_PORTUGUESE  3    //  3 PC860 [Portuguese (DOS)]
#define CODE_PAGE_CAN_FRENCH  4    //  4 PC863 [French Canadian (DOS)]
#define CODE_PAGE_NORDIC      5    //  5 PC865 [Nordic (DOS)]
#define CODE_PAGE_WEST_EUROPE_W 6  //  6 WPC1252 [Western European (Windows)]
#define CODE_PAGE_RUSSIAN     7    //  7 PC866 [Russian MS-DOS]
#define CODE_PAGE_EAST_EUROPE 8    //  8 PC852 [Eastern European MS-DOS]
#define CODE_PAGE_LATIN1      9    //  9 PC858 [IBM00858 OEM Multilingual Latin I]
#define CODE_PAGE_CHINESE     255   //  10 PC936 [Chinese Simplified]

#define ENCODE_MODE_ANSI 0
#define ENCODE_MODE_UNICODE_LE 1
#define ENCODE_MODE_UNICODE_BE 2

#define MODE_ANSI 0xffff
#define MODE_UNICODE_LE 0xfffe
#define MODE_UNICODE_BE 0xfeff

#define FULL_CUT_FLAG 0
#define HALF_CUT_FLAG 1
#define BLACKMARK_CUT_FLAG 2

#define STATUS_BYTE1_BIT0 1 
#define STATUS_BYTE1_BIT1 2
#define STATUS_BYTE1_BIT2 4
#define STATUS_BYTE1_OFFLINE 8
#define STATUS_BYTE1_BIT4 0x10
#define STATUS_BYTE1_BIT5 0x20
#define STATUS_BYTE1_FEED_BUTTON_PRESSED 0x40
#define STATUS_BYTE1_BIT7 0x80

#define STATUS_BYTE2_BIT0 1
#define STATUS_BYTE2_BIT1 2
#define STATUS_BYTE2_PLATEN_OPENED 4
#define STATUS_BYTE2_FEEDING_PAPER_BY_FEED_BUTTON 8
#define STATUS_BYTE2_BIT4 0x10
#define STATUS_BYTE2_PAPER_END_STOP 0x20
#define STATUS_BYTE2_HAVE_ERROR 0x40
#define STATUS_BYTE2_BIT7 0x80

#define STATUS_BYTE3_BIT0 1
#define STATUS_BYTE3_BIT1 2
#define STATUS_BYTE3_MACHANICAL_ERROR 4
#define STATUS_BYTE3_AUTOCUTTER_ERROR 8
#define STATUS_BYTE3_BIT4 0x10
#define STATUS_BYTE3_UNRECOVERABLE_ERROR 0x20
#define STATUS_BYTE3_RECOVERABLE_ERROR 0x40
#define STATUS_BYTE3_BIT7 0x80

//#define STATUS_BYTE4_BIT0 1
//#define STATUS_BYTE4_BIT1 2
#define STATUS_BYTE4_EJECTOR_HAVE_PAPER 1
#define STATUS_BYTE4_EJECTOR_JAMMING 2
#define STATUS_BYTE4_PAPER_NEAR_END 0xc
//#define STATUS_BYTE4_BIT3 8
#define STATUS_BYTE4_BIT4 0x10
#define STATUS_BYTE4_PAPER_END 0x60
//#define STATUS_BYTE4_BIT6 0x40
#define STATUS_BYTE4_BIT7 0x80

#define STATUS_OFFLINE     STATUS_BYTE1_OFFLINE
#define STATUS_PAPER_ERROR     (STATUS_BYTE4_PAPER_END << 24)
#define STATUS_PAPER_NE     (STATUS_BYTE4_PAPER_NEAR_END << 24)
#define STATUS_CUTTER_ERROR     (STATUS_BYTE3_AUTOCUTTER_ERROR << 16)
#define STATUS_PLATEN_ERROR     (STATUS_BYTE2_PLATEN_OPENED << 8)
#define STATUS_BLACK_MARK_ERROR     (STATUS_BYTE3_MACHANICAL_ERROR << 16)
#define STATUS_RECOVERABLE_ERROR     (STATUS_BYTE3_RECOVERABLE_ERROR << 16)
#define STATUS_UNRECOVERABLE_ERROR     (STATUS_BYTE3_UNRECOVERABLE_ERROR << 16)

class PrinterApi
{
public:
    PrinterApi(void);
    ~PrinterApi(void);

    static int OpenPort();
    static void ClosePort();
	static int SetPrinterEncodeMode(int mode);
	static int SetPrinterCodePage(int codepage);
	static int PrintText(unsigned char *textBuf, int length);
	static int LineFeed();
	static int PrintGraph(const char* imageFileName);
	static int GetStatus(unsigned long *pStatus);
	static int CutPaper(int cutMode);
    static int FeedPaper(int n);
    static int PrintLogo(int logoNo, int printMode);
    static int SetTextAttr(unsigned long attribute);
    static int SetLeftMargin(int leftMargin);

    static int doSetBarcodeArg(Barcode &BarcodeArg);
    static int PrintBarcode(int barcodeType, char* text);
    static int SetBarcode(int height,int width,int textPos,int textFontType);
};

#endif //__PRINTER_API__
