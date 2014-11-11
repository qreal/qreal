TARGET = qrkernel_unittests

CONFIG += c++11

include(../common.pri)

include(../../../qrkernel/qrkernel.pri)

INCLUDEPATH += $$PWD/../../../

SOURCES += \
	$$PWD/idsTest.cpp \
	$$PWD/exception/exceptionTest.cpp \
	$$PWD/settingsManagerTest.cpp \
	$$PWD/versionTest.cpp \

HEADERS += \
	$$PWD/settingsManagerTest.h \
