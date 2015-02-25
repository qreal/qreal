TEMPLATE = lib
CONFIG += plugin

QT += widgets

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

DESTDIR = $$ROOT/bin/plugins/
!isEmpty(DESTINATION) {
	DESTDIR = $$ROOT$$DESTINATION
}

isEmpty(QREAL_EDITOR_NAME) {
	error(Please set QREAL_EDITOR_NAME variable in a .pro file of your editor as a folder name of that editor)
}

if (equals(QMAKE_CXX, "g++")) {
	QMAKE_LFLAGS += -Wl,-E
}

QRMC_ROOT = $$ROOT

include(pluginsSdk.pri)
