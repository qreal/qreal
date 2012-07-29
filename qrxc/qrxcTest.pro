include (qrxc.pro)

OBJECTS_DIR = .unittestobj
MOC_DIR = .unittestmoc

INCLUDEPATH += ../thirdparty/gmock-1.6.0/include
INCLUDEPATH += ../thirdparty/gmock-1.6.0/gtest/include
LIBS += -L../thirdparty/ -lgmock_main -lpthread

TARGET = qrxc_unittests
DESTDIR = ../bin/unittests
DEFINES += UNITTEST
