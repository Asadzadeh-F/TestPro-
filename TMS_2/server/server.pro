#-------------------------------------------------
#
# Project created by QtCreator 2020-08-29T01:31:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app

target.path = /yaffs/CashLess
INSTALLS += target


SOURCES += \
    ../Common/NetworkData/packet.cpp \
    main.cpp \
    mainwindow.cpp \
    packet.cpp

HEADERS += \
   ../Common/NetworkData/packet.h \
    mainwindow.h

FORMS += \
        mainwindow.ui







unix:!macx: LIBS += -L$$PWD/lib/newlib/qttslib/lib/ -lts

INCLUDEPATH += $$PWD/lib/newlib/qttslib/include
DEPENDPATH += $$PWD/lib/newlib/qttslib/include

unix:!macx: LIBS += -L$$PWD/lib/szzt/ -lzt598

INCLUDEPATH += $$PWD/lib/szzt
DEPENDPATH += $$PWD/lib/szzt

unix:!macx: LIBS += -L$$PWD/lib/szzt/ -lztcard

INCLUDEPATH += $$PWD/lib/szzt
DEPENDPATH += $$PWD/lib/szzt

unix:!macx: LIBS += -L$$PWD/lib/szzt/ -lztmisc

INCLUDEPATH += $$PWD/lib/szzt
DEPENDPATH += $$PWD/lib/szzt

unix:!macx: LIBS += -L$$PWD/lib/szzt/ -lzttap

INCLUDEPATH += $$PWD/lib/szzt
DEPENDPATH += $$PWD/lib/szzt

QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN'"

