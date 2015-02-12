TEMPLATE = app
CONFIG += console

include(../../global.pri)

QT += widgets

SOURCES = $$PWD/mainTest.cpp

if (equals(QMAKE_CXX, "g++") : !macx) {
	QMAKE_LFLAGS += -Wl,-E
	# For gmock
	QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
}

INCLUDEPATH += \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../thirdparty/gmock-1.6.0/include \
	$$PWD/../thirdparty/gmock-1.6.0/gtest/include \

links(gmock)

include($$PWD/utils/utils.pri)