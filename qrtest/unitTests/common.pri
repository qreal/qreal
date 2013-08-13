TEMPLATE = app
CONFIG += console

DESTDIR = $$PWD/../../bin

macx {
	CONFIG -= app_bundle
}

SOURCES = $$PWD/mainTest.cpp

!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,."
}

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

if (equals(QMAKE_CXX, "g++") : !macx) {
	QMAKE_LFLAGS += -Wl,-E
}

INCLUDEPATH += \
	$$PWD/../thirdparty/gmock-1.6.0/include \
	$$PWD/../thirdparty/gmock-1.6.0/gtest/include \

LIBS += -L$$PWD/../../bin -lgmock -lpthread
