QT       += core gui network
QMAKE_CXXFLAGS += -std=c++0x
QT += testlib


#TARGET = Tests
#TEMPLATE = app

#target.path = /yaffs/CashLess
#INSTALLS += target



HEADERS += \
	../../Common/NetworkData/packet.h \
	../../Common/Container/memoryBlockManagement.h \
	../../client/NetworkManager.h \
	../../client/Handler.h \
	../../client/NetworkProcess.h \
	ClientTests.h

SOURCES += \
	../../Common/NetworkData/packet.cpp \
	../../Common/Container/memoryBlockManagement.cpp\
	../../client/NetworkManager.cpp \
	../../client/Handler.cpp \
	../../client/NetworkProcess.cpp \
	ClientTests.cpp

INCLUDEPATH +=../../ \
	      ../../Common/ \
	      ../../Common/NetworkData \
	      ../../Common/Container \
	      ../../client/


