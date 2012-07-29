include (qrgui.pro)

OBJECTS_DIR = .unittestobj
UI_DIR = .unittestui
MOC_DIR = .unittestmoc
RCC_DIR = .unittestmoc

INCLUDEPATH += ../thirdparty/gmock-1.6.0/include
INCLUDEPATH += ../thirdparty/gmock-1.6.0/gtest/include
LIBS += -L../thirdparty/ -lgmock_main -lpthread

TARGET = qrgui_unittests
DESTDIR = ../bin/unittests
DEFINES += UNITTEST
