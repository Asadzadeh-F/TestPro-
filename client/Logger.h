#ifndef LOGGER_H
#define LOGGER_H
#include "QObject"
#include "QFile"
#include "QDateTime"
#include "QTextStream"
#include <mutex>



class Logger :public QObject
{
    Q_OBJECT
public:
	explicit Logger(QString fileName, QObject *parent = nullptr);
	~Logger();
    void setShowDateTime(bool value);
private:
    QFile *file;
    bool m_showDate;
private slots:
    void writeLog(QtMsgType type,const QString &value);

};

#endif // LOGGER_H
