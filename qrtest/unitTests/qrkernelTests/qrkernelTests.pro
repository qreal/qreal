TARGET = qrkernel_unittests

include(../common.pri)

include(../../../qrkernel/qrkernel.pri)

SOURCES += \
	idsTest.cpp \
	exception/exceptionTest.cpp \
	settingsManagerTest.cpp \

HEADERS += \
	settingsManagerTest.h \
