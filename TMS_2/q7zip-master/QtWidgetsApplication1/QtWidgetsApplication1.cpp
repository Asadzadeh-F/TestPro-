#include "QtWidgetsApplication1.h"
#include <QFileDialog>
#include "ui_QtWidgetsApplication1.h"

#include <q7z_facade.h>
#include <q7z_list.h>
#include <q7z_create.h>
#include <q7z_extract.h>
#include <QTemporaryFile>
#include <q7z_extract.h>
#include <QProcess>

class MyCallBack :public Q7z::ExtractCallback
{


    // ExtractCallback interface
protected:
    LONG setCompleted(quint64 completed, quint64 total)
    {
        qDebug() << 100 * float(completed) / float(total);
        qApp->processEvents();
        return S_OK;
    }
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(onCompress()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCompress()
{
    /*Q7z::initSevenZ();

    auto path=QFileDialog::getOpenFileName(nullptr,"Browse File");

    QFile file(path); // embedded resource
    file.open(QIODevice::ReadOnly);

    QVector<Q7z::File> files = Q7z::listArchive(&file);
    qDebug()<<"Get List Files";*/

    /*Q7z::initSevenZ();
    QFile target("/tmp/compress.7z");
    target.open(QIODevice::ReadWrite|QIODevice::Truncate);
    Q7z::createArchive(&target, QStringList() <<
                               "/home/abbas/Documents/Project/other/testQ7zip/build-Debug/ui_mainwindow.h" <<
                                    "/home/abbas/Documents/Project/other/testQ7zip/build-Debug/out1.7z");*/
    Q7z::initSevenZ();
    bool ok = Q7z::isSupportedArchive("D:\\Projects\\Temp\\SimpleQt\\q7zip\\QtWidgetsApplication1\\App3dViewer.0.0.22.Win32.7z");
    if (ok)
    {
        QFile source("D:\\Projects\\Temp\\SimpleQt\\q7zip\\QtWidgetsApplication1\\App3dViewer.0.0.22.Win32.7z"); // embedded resource
        source.open(QIODevice::ReadOnly);
        auto myCallback = new MyCallBack();
        Q7z::extractArchive(&source, "D:\\Projects\\Temp\\SimpleQt\\q7zip\\QtWidgetsApplication1\\outZip", myCallback);
        QString appName = "/home/abbas/Documents/Project/other/testQ7zip/build-Debug/outZip/etcher-1.1.1-x86_64.AppImage";
        system((QString("chmod +x ") + appName).toStdString().data());
        QProcess::startDetached(appName);

    }

}

