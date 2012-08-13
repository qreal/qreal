TEMPLATE =  lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

isEmpty(QREAL_EDITOR_NAME) {
	error(Please set QREAL_EDITOR_NAME variable in a .pro file of your editor as a folder name of that editor)
}

if (equals(QMAKE_CXX, "g++")) {
	QMAKE_LFLAGS += -Wl,-E
}

QRMC_ROOT = ..

include(pluginsSdk.pri)
