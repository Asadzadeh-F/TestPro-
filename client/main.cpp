#include "MainWindow.h"

#include <QApplication>
#include <QSettings>
#include <QtGlobal>
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}

MainWindow* mainWindow = nullptr;

#if QT_VERSION > 0x049999
#define qInstallMsgHandler qInstallMessageHandler
void messageHandler(QtMsgType type, const QMessageLogContext&, const QString& msg)
{
	Q_UNUSED(type)
	QString tmp = msg;
	if (mainWindow != nullptr)
		qDebug(tmp.toStdString().c_str());
}
#else
void messageHandler(QtMsgType type, const char* msg)
{
	Q_UNUSED(type)
	QString tmp = msg;
	if (mainWindow != nullptr)
		qDebug(tmp.toStdString().c_str());
}
#endif

int main(int argc, char* argv[])
{
	QCoreApplication::setOrganizationName("MySoft");
	QCoreApplication::setOrganizationDomain("mysoft.com");
	QCoreApplication::setApplicationName("Star Runner");

	QApplication a(argc, argv);
	qInstallMsgHandler(messageHandler);
	mainWindow = new MainWindow();
	mainWindow->show();
	return a.exec();
}
