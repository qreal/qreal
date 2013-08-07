DEFINES += UNITTEST

TEMPLATE = app
CONFIG += console

QT += xml

DESTDIR = ../../../bin
TARGET = qrrepo_unittests

!macx {
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../bin
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../bin/plugins
}

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	../../../thirdparty/gmock-1.6.0/include \
	../../../thirdparty/gmock-1.6.0/gtest/include \

LIBS += -L../../../bin/ -lqrkernel -lqrutils
LIBS += -L../../../bin/thirdparty/ -lgmock -lpthread

include(../../../qrrepo/qrrepo.pri)

include(qrrepoTests.pri)

OTHER_FILES += \
	../unittestDeclaration.cfg \
