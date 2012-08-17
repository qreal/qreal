include (qrxc.pro)

OBJECTS_DIR = .unittestobj
MOC_DIR = .unittestmoc

INCLUDEPATH += \
	../thirdparty/gmock-1.6.0/include \
	../thirdparty/gmock-1.6.0/gtest/include \

LIBS += -L../bin/thirdparty/ -lgmock -lpthread

TARGET = qrxc_unittests
DESTDIR = ../bin/unittests
DEFINES += UNITTEST
