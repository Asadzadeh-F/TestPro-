#ifndef _PRINTBARCODE_H_
#define _PRINTBARCODE_H_

#include "Cmd.h"
#include "szztplib.h"

#include <string>
#include <iostream>
using namespace std;



class Barcode : public Cmd
{
public:
	  int barcodeType;
	  string barcodeText;
	  int barcodeHeight;
	  int barcodeWidth;
	  int textPos;
	  int textFontType;

	Barcode();   // standard constructor
	virtual ~Barcode();
	void releaseBarcode(Barcode *barcode);
	virtual void setLen(int len);
	void setBuf(string barcodeAndExtracode);
	void setRows(int rows);
	void setColumns(int cols);

	//int BarcodeArgChk(int barcodeType, char* barcode,char *extracode);
	//int doPrintBarcode(int barcodeType, char* barcode,char *extracode);
	int BarcodeArgChk(int barcodeType, char* barcodeAndExtracode);

	void setType(int type);

	void setWidth(int n);
	void setHeight(int n);
	void setFontPos(int pos);
	void setTextFontType(int type);

	void setBarcodeWidth(int n);
	void setBarcodeHeight(int n);
	void setBarcodeFontPos(int pos);
	void setBarcodeTextFontType(int type);

};

#endif

