DESTDIR = ../bin

CONFIG += c++11

CONFIG += rpath_libdirs
macx {
	CONFIG -= app_bundle
}

SOURCES = main.cpp

TRANSLATIONS = qrgui_ru.ts

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../bin/thirdparty/
}

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

if (equals(QMAKE_CXX, "g++") : !macx) {
	QMAKE_LFLAGS += -Wl,-E
}

include(qrgui.pri)
