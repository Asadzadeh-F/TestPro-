#include "hdr/TMS/Updater.h"
#include "QDir"
#include <QProcess>

Updater::Updater(QObject *parent) : QObject(parent)
{
	m_handler = nullptr;
	m_handler = new Handler(this);
	connect(m_handler, SIGNAL(readyAllFileParts(QByteArray packet)), this,
			SLOT(extractFile(QByteArray packet)));
	connect(this, SIGNAL(packageReady(QString)), this, SLOT(UpdateFile(QString)));
}

void Updater::extractFile(QByteArray buffer)
{
	qDebug() << buffer.size();

    //unzip(buffer);
    gUnzip(buffer);
}

void Updater::UpdateFile(QString extractDir)
{
	QString defaultCashLessPath = "/yaffs/CashLess";
	QDir	dir(extractDir);
	//QFile::copy()
	QStringList filters;
    filters << "*.so.*";
	auto fileInfoList = dir.entryInfoList(filters);
	copyFilesIntoPath(fileInfoList, defaultCashLessPath);
	//        Process *proc = new QProcess(this);
	//        proc->start("cp ......");

    filters << "CashLess";
    fileInfoList = dir.entryInfoList(filters);
    copyFilesIntoPath(fileInfoList, defaultCashLessPath);

    filters << "VASClient";
    fileInfoList = dir.entryInfoList(filters);
    copyFilesIntoPath(fileInfoList, defaultCashLessPath);

    filters << "*.config";
    fileInfoList = dir.entryInfoList(filters);
    copyFilesIntoPath(fileInfoList, defaultCashLessPath);

    defaultCashLessPath = "/yaffs/CashLess/images";
    filters << "*.png"
            << "*.PNG";
    fileInfoList = dir.entryInfoList(filters);
    copyFilesIntoPath(fileInfoList, defaultCashLessPath);

    defaultCashLessPath = "/yaffs/";
    filters << "*.sh";
    fileInfoList = dir.entryInfoList(filters);
    copyFilesIntoPath(fileInfoList, defaultCashLessPath);

    filters << "*sysConf";
    fileInfoList = dir.entryInfoList(filters);
    copyFilesIntoPath(fileInfoList, defaultCashLessPath);



}

void Updater::unzip(QByteArray &buffer)
{
	QString	   packagePath = "";
	QByteArray uncompresed = qUncompress(buffer);
	emit	   packageReady(packagePath);
}

bool Updater::copyFilesIntoPath(QFileInfoList filesList, QString destinationPath)
{
	bool result = true;
    foreach (auto fileInfo , filesList)
	{
		if (copyRecursively(fileInfo.absoluteFilePath(),
							destinationPath + "/" + fileInfo.fileName()))
		{
			//copy successfully.
            qDebug("copy successfully.");
		}
		else
		{
			//copy failed.
            qDebug("copy failed");
			result = false;
		}
	}
	return result;
}

bool Updater::copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
	QFileInfo srcFileInfo(srcFilePath);
	if (srcFileInfo.isDir())
	{
		QDir targetDir(tgtFilePath);
		targetDir.cdUp();
		if (!QFileInfo(tgtFilePath).exists() && !targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
			return false;
		QFile file(tgtFilePath);
		file.setPermissions(QFile::Permission(0x7777));
        QDir sourceDir(srcFilePath);
		QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot
													| QDir::Hidden | QDir::System);
		foreach (const QString &fileName, fileNames)
		{
			const QString newSrcFilePath = srcFilePath + QLatin1Char('/') + fileName;
			const QString newTgtFilePath = tgtFilePath + QLatin1Char('/') + fileName;
			if (!copyRecursively(newSrcFilePath, newTgtFilePath))
				return false;
		}
	}
	else
	{
		if (!QFileInfo(tgtFilePath).exists() && !QFile::copy(srcFilePath, tgtFilePath))
			return false;
		QFile file(tgtFilePath);
		file.setPermissions(QFile::Permission(0x7777));
	}
    return true;
}

void Updater::gUnzip(QByteArray &buffer)
{
    QString filePath = "/yaffs/CashLess/RCVTMS";
    QFile RCVTMS(filePath);
    if ( RCVTMS.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        RCVTMS.write(buffer);
        RCVTMS.close();
    }
    else
        qDebug("Updater::gUnzip--> open error.");
    QProcess process;
    QStringList params;
    params << filePath;
    process.start("gunzip", params);
    if(!process.waitForStarted()) //default wait time 30 sec
        qWarning() << " cannot start process ";


    int waitTime = 60000 ; //60 sec
    if (!process.waitForFinished(waitTime))
             qWarning() << "timeout .. ";

   //QProcess::execute("gunzip /yaffs/CashLess/RCVTMS"));
    emit	   packageReady(filePath);

}
