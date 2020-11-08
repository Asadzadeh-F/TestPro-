#include <QImage>
#include <QPainter>
#include <QPen>
#include <QFontDatabase>
#include <QDebug>
#include <QByteArray>
#include <cstdio>
#include "szztplib.h"
#include "szztprninterface.h"

SzztPrnInterface::SzztPrnInterface(QObject *parent) :
    QObject(parent)
{
    //use the following settings for USB port.
    SetCommSettings(USB_PORT, 0, 0, 0);
    text_flag=0;
    rightmargin =0;
    _linewidth = 40;
}


SzztPrnInterface::~SzztPrnInterface()
{

}

int SzztPrnInterface::PrnOpenPort()
{
    return OpenPort();
}

int SzztPrnInterface::PrnGetStatus(unsigned long *pStatus)
{
    return GetStatus(pStatus);
}

void SzztPrnInterface::PrnClosePort()
{
    ClosePort();
}
int SzztPrnInterface::PrnCutPaper(int cutMode)
{
    return CutPaper(cutMode);
}
int SzztPrnInterface::PrnPrintText(unsigned char *textBuf, int length)
{
    return PrintText(textBuf,length);
}
int SzztPrnInterface::PrnPrintGraph(const char* imageFileName)
{
    return PrintGraph(imageFileName);
}
int SzztPrnInterface::PrnLineFeed()
{
    return LineFeed();
}
int SzztPrnInterface::PrnFeedPaper(int n)
{
    return FeedPaper(n);
}
int SzztPrnInterface::PrnPrintLogo(int logoNo, int printMode)
{
    return PrintLogo(logoNo,printMode);
}
int SzztPrnInterface::PrnSetTextAttr(unsigned long attribute)
{
    text_flag = attribute;
    return SetTextAttr(attribute);
}
int SzztPrnInterface::PrnSetLeftMargin(int leftMargin)
{
    rightmargin = leftMargin;
    return SetLeftMargin(leftMargin);
}
int SzztPrnInterface::PrnSetPrinterEncodeMode(int mode)
{
    return SetPrinterEncodeMode(mode);
}
int SzztPrnInterface::PrnSetPrinterCodePage(int codepage)
{
    return SetPrinterCodePage(codepage);
}
int SzztPrnInterface::PrnSetBarcode(int barHeight,int barWidth, enum BarcodeTextPos textPos, enum BarcodeTextType textFontType)
{
    return SetBarcode(barHeight,barWidth,textPos,textFontType);
}
int SzztPrnInterface::PrnPrintBarcode(enum BarcodeType barcodeType, char* text )
{
    return PrintBarcode(barcodeType,text);
}
void SzztPrnInterface::PrnSetLineWidth(int linewidth)
{
    _linewidth = linewidth;
}

void SzztPrnInterface::PrnPrintArabic(const char *text,int fontsize)
{

//    QString Arabicstr=QString::fromUtf8(text);
//    char * tmp = "؟ يفضل الغرفة لإعطاء الشمس أو العكس؟";
    QString Arabicstr = QString::fromUtf8(text);

    QSize size(576,_linewidth); //指定图片大小;
    QImage image(size,QImage::Format_ARGB32); //以ARGB32格式构造一个QImage,
    QRectF rect(0-rightmargin,0,576,_linewidth);

     if( text_flag ==  TEXT_BOLD)
     {
        image.fill(qRgba(255,255,255,0));//
         //填充图片背景,120/250为透明度
         QPainter painter(&image); //为这个QImage构造一个QPainter
         painter.setCompositionMode(QPainter::CompositionMode_DestinationOver); //设置画刷的组合模式


         QFont font = painter.font();
         font.setPixelSize(fontsize);
         font.setWeight(QFont::Bold);

         painter.setPen(QColor(Qt::black));
         painter.setFont(font);
         painter.drawText(rect,Qt::AlignRight,Arabicstr);

     }else if(text_flag == TEXT_UNDERLINE)
     {
         image.fill(qRgba(255,255,255,0));
         QPainter painter(&image); //为这个QImage构造一个QPainter
         painter.setCompositionMode(QPainter::CompositionMode_DestinationOver); //设置画刷的组合模式

         QFont font = painter.font();
         font.setPixelSize(fontsize);
         font.setWeight(QFont::Light);
         font.setUnderline(true);

         painter.setPen(QColor(Qt::black));
         painter.setFont(font);
         painter.drawText(rect,Qt::AlignRight,Arabicstr);


     }else if(text_flag == TEXT_REVERSE)
     {
         image.fill(qRgba(0,0,0,0));
         //填充图片背景,120/250为透明度
         QPainter painter(&image); //为这个QImage构造一个QPainter
         painter.setCompositionMode(QPainter::CompositionMode_DestinationOver); //设置画刷的组合模式


         QFont font = painter.font();
         font.setPixelSize(fontsize);
         font.setWeight(QFont::Light);

         painter.setPen(QColor(Qt::white));
         painter.setFont(font);
         painter.drawText(rect,Qt::AlignRight,Arabicstr);

     }else if(  text_flag == TEXT_SMALL_FONT){
         image.fill(qRgba(255,255,255,0));//填充图片背景,120/250为透明度

          QPainter painter(&image); //为这个QImage构造一个QPainter
          painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
          //设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
          //改变组合模式和上面的填充方式可以画出透明的图片。


          QFont font = painter.font();
          font.setPixelSize(20);
          font.setWeight(QFont::Light);

          painter.setPen(QColor(Qt::black));
          painter.setFont(font);
          painter.drawText(rect,Qt::AlignRight,Arabicstr);

     }else if( rightmargin ==  TEXT_UPSIDE_DOWN){
         SetTextAttr(0);
         PrintText((unsigned char *)text,strlen(text));

         return;
     } else{
            image.fill(qRgba(255,255,255,0));//填充图片背景,120/250为透明度

             QPainter painter(&image); //为这个QImage构造一个QPainter
             painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
             //设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
             //改变组合模式和上面的填充方式可以画出透明的图片。


             QFont font = painter.font();
             font.setPixelSize(fontsize);
             font.setWeight(QFont::Light);

             painter.setPen(QColor(Qt::black));
             painter.setFont(font);
             painter.drawText(rect,Qt::AlignRight,Arabicstr);
     }

     image.save("alabo.BMP","BMP");
     PrintGraph("alabo.BMP");
}

