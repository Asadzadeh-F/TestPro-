#-------------------------------------------------
#
# Project created by QtCreator 2020-08-29T00:02:15
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
unix: QMAKE_CXXFLAGS += -std=c++0x
unix:!macx:QMAKE_CXXFLAGS += -Wno-psabi
unix: DEFINES += QT_MESSAGELOGCONTEXT

TARGET = client
TEMPLATE = app

unix: target.path = /yaffs/CashLess
unix: INSTALLS += target

SOURCES += \
    ../Common/NetworkData/packet.cpp \
    MainWindow.cpp \
    NetworkProcess.cpp \
    ../Common/Container/memoryBlockManagement.cpp \
    Handler.cpp \
    NetworkManager.cpp \
    Logger.cpp \
    main.cpp

HEADERS += \
    ../Common/NetworkData/packet.h \
    MainWindow.h \
    NetworkProcess.h \
    ../Common/global.h \
    ../Common/Container/memoryBlockManagement.h \
    Handler.h \
    NetworkManager.h \
    Logger.h

FORMS += \
    MainWindow.ui

INCLUDEPATH += ../Common/ \
    ../Common/NetworkData/ \
    ../Common/Container

unix:!macx: LIBS += -L$$PWD/../lib/newlib/qttslib/lib/ -lts

INCLUDEPATH += $$PWD/../lib/newlib/qttslib/include
DEPENDPATH += $$PWD/../lib/newlib/qttslib/include

unix:!macx: LIBS += -L$$PWD/../lib/szzt/ -lzt598

INCLUDEPATH += $$PWD/../lib/szzt
DEPENDPATH += $$PWD/../lib/szzt

unix:!macx: LIBS += -L$$PWD/../lib/szzt/ -lztmisc

INCLUDEPATH += $$PWD/../lib/szzt
DEPENDPATH += $$PWD/../lib/szzt

unix:!macx: LIBS += -L$$PWD/../lib/szzt/ -lzttap

INCLUDEPATH += $$PWD/../lib/szzt
DEPENDPATH += $$PWD/../lib/szzt

unix: QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN'"
