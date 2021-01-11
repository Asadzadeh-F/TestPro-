//#pragma once
//
//#include <QSettings>
//
///// Singleton! Create only one instance!
//class Settings
//{
//public:
//    Settings();
//    ~Settings();
//
//    static bool contains(const QString &key);
//    static QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
//    static void setValue(const QString &key, const QVariant &value);
//
//private:
//    static Settings *s_instance;
//    QSettings m_settings;
//};

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
     Settings(QObject *parent = 0);
     Settings(QString sIniFileName);
     ~Settings();
     void setParameters(uint16_t lastIndex, uint16_t totalIndex, uint32_t m_hash);
     QVariant readValue(const QString & group, const QString & key, const QVariant & defValue);
     void writeValue(const QString& group, const QString& key, const QVariant& value);

     bool containInKeys(QString keyName);
     QString		m_totalIndex;
     QString		m_currectIndex;
     uint32_t		m_hash;


 private:
    QSettings*     m_setting;

};

#endif // SETTINGS_H
//

