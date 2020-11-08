#include "client/MainWindow.h"
#include "../Tests/ClientTests/ClientTests.h"

#include <QApplication>
#include "QtGlobal"
#include "QSettings"
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}

MainWindow* mainWindow = nullptr;

void messageHandler(QtMsgType type,const char* msg)
{
    Q_UNUSED(type)
    QString tmp = msg;
    if(mainWindow!=nullptr)
        qDebug(tmp.toStdString().c_str());
}


int main(int argc, char *argv[])
{
    QSettings settings("SEA", "TMS");

    //QSettings::QSettings(const QString &fileName, QSettings::Format format, QObject *parent = nullptr)  //On Unix, fileName is the name of an INI file.
    settings.beginGroup("TMS");
    settings.setValue("Totalndex",0);
    settings.setValue("lastGetIndex",0);
    settings.setValue("Hash",0);
    settings.endGroup();

    QApplication a(argc, argv);
    ClientTests *testApp = new ClientTests();
    testApp->initTestCase();
    testApp->testIndexNetworkProcessing();
    testApp->testSimpleNetworkProcessing();
    //qInstallMsgHandler(messageHandler);
    //mainWindow= new MainWindow();
    //mainWindow->show();
	return a.exec();
}
