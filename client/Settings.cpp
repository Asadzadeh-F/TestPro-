#include "hdr/TMS/Settings.h"
#include <QCoreApplication>
#include "hdr/TMS/global.h"
#include <QStringList>
#include "QString"
#include "QDebug"

Settings::Settings(QObject *parent) : QObject(parent)
{
    m_setting   = nullptr;
    m_setting   = new QSettings(this);
    //QCoreApplication::setOrganizationName("SEA");
    //QCoreApplication::setApplicationName("TMSClient");
    qDebug() << m_setting->fileName();
}

Settings::Settings(QString sIniFileName)
{
    m_setting = new QSettings(sIniFileName,QSettings::IniFormat);
}

Settings::~Settings()
{
    m_setting->clear();
}

void Settings::setParameters(uint16_t lastIndex, uint16_t totalIndex, uint32_t hash)
{
    m_setting->beginGroup("UpdateData");
    m_setting->setValue("Hash", hash);
    m_setting->setValue("LastGetIndex", lastIndex);
    m_setting->setValue("TotalIndex", totalIndex);
    m_setting->endGroup();
}


QVariant Settings::readValue(const QString &group, const QString &key, const QVariant &defValue)
{
    //Settings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

    m_setting->beginGroup(group);
    QVariant value = m_setting->value(key, defValue);
    m_setting->endGroup();

    return value;

}

void Settings::writeValue(const QString &group,const QString &key,const QVariant &value)
{
    m_setting->beginGroup( group );
    m_setting->setValue( key, value );
    m_setting->endGroup();
}

bool Settings::containInKeys(QString keyName)
{
    return m_setting->childKeys().contains(keyName);
}


//Settings *Settings::s_instance = nullptr;
//
//Settings::Settings()
//{
//    Q_ASSERT(s_instance == nullptr);
//    s_instance = this;
//}
//
//Settings::~Settings()
//{
//    Q_ASSERT(s_instance != nullptr);
//    s_instance = nullptr;
//}
//
//bool Settings::contains(const QString &key)
//{
//    return s_instance->m_settings.contains(key);
//}
//
//QVariant Settings::value(const QString &key, const QVariant &defaultValue)
//{
//    return s_instance->m_settings.value(key, defaultValue);
//}
//
//void Settings::setValue(const QString &key, const QVariant &value)
//{
//    s_instance->m_settings.setValue(key, value);
//}
//
