TEMPLATE =  lib
CONFIG += plugin qtestlib

HEADERS += \
	qrguiTests/idsTest.h \
	suite.h \

SOURCES += \
	suite.cpp \

DESTDIR = ../testRunner/suites/

include(qrguiTests/qrgui.pri)

