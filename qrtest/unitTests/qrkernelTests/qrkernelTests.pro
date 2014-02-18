TARGET = qrkernel_unittests

CONFIG += c++11

include(../common.pri)

include(../../../qrkernel/qrkernel.pri)

SOURCES += \
	idsTest.cpp \
	exception/exceptionTest.cpp \
	settingsManagerTest.cpp \

HEADERS += \
	settingsManagerTest.h \
