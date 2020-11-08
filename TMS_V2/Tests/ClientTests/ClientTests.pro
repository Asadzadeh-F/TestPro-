QT       += core gui network
QMAKE_CXXFLAGS += -std=c++0x
QT += testlib


#TARGET = Tests
#TEMPLATE = app

#target.path = /yaffs/CashLess
#INSTALLS += target



HEADERS += \
        ../client/Common/NetworkData/packet.h \
        ../client/Common/Container/memoryBlockManagement.h \
        ../client/NetworkManager.h \
        ../client/Handler.h \
        ../client/NetworkProcess.h \
        ClientTests.h

SOURCES += \
        ../client/Common/NetworkData/packet.cpp \
        ../client/Common/Container/memoryBlockManagement.cpp\
        ../client/NetworkManager.cpp \
        ../client/Handler.cpp \
        ../client/NetworkProcess.cpp \
        ClientTests.cpp

INCLUDEPATH +=../client/ \
              ../client/Common/ \
              ../client/Common/NetworkData \
              ../client/Common/Container


