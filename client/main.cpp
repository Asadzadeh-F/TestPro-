//#include "hdr/BaseForms/BaseFormSplash.h"
#include "hdr/FrmMain.h"
#include <QApplication>
#include <QWSServer>
#include "hdr/TMS/Handler.h"
#include "hdr/TMS/packet.h"
#include "hdr/TMS/UnitTest.h"
#include "hdr/TMS/Settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #ifdef Q_WS_QWS
        QWSServer::setCursorVisible( false );
    #endif

   //
   // BaseWaitForm *baseWaitForm = new BaseWaitForm();
   // Global::setWaitStatus(FRM_WAIT_WAITING);
   // baseWaitForm->setupUi();
   // baseWaitForm->show();
   //
   //
   //FrmMain *main;
   // main = FrmMain::getInstance();
    //main->show();

        //Packet *pac = new Packet();
        //pac->getCRC();;
    //UnitTest *test = new UnitTest;
    QCoreApplication::setOrganizationName("SEA");
    QCoreApplication::setApplicationName("TMSClient");
    Settings* setting = new Settings();

    setting->setParameters(0,0,4);
    QString serverAdress = "172.45.6.202" ;
    uint32_t port = 15007;
    Handler *handler;
    handler = new Handler();
    handler->startUpdate(serverAdress, port);

    return a.exec();
}
