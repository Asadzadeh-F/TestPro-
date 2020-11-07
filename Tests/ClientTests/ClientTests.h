#ifndef CLIENTTESTS_H
#define CLIENTTESTS_H

#include <NetworkProcess.h>
#include "Handler.h"
#include <QtTest/QTest>
#include <QObject>

class ClientTests : public QObject
{
    Q_OBJECT

public slots:
    void testStreamNetworkProcessing();
    void testIndexNetworkProcessing();
    void testSimpleNetworkProcessing();
    void initTestCase();

public:
    ClientTests();
    NetworkProcess* m_networkProcess;
    Handler*        m_handler;
};

#endif // CLIENTTESTS_H
