#include "Logger.h"

#include <QMutex>

Logger::Logger(QString fileName, QObject* parent) : QObject(parent)
{
	m_showDate = true;
    if (!fileName.isEmpty()) {
     file = new QFile;
     file->setFileName(fileName);
     file->open(QIODevice::Append | QIODevice::Text);
    }
}

Logger::~Logger()
{
    if (file != 0)
        file->close();
}

void Logger::writeLog(QtMsgType type, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    //QString text = value;// + "";
    QString msgStr;
    QString text;

    switch (type) {
    case QtDebugMsg:
        msgStr.append("Debug:").append(msg);
        break;
    case QtWarningMsg:
        msgStr.append("Warning:").append(msg);
        break;
    case QtCriticalMsg:
        msgStr.append("Critical:").append(msg);
        break;
    case QtFatalMsg:
        msgStr.append("Fatal:").append(msg);
        break;
    default:
        break;
    }
    if (m_showDate)
     text = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + msgStr;
    QTextStream out(file);
    out.setCodec("UTF-8");
    if (file != 0) {
     out << text;
    }

}


void Logger::setShowDateTime(bool value)
{
    m_showDate = value;
}





