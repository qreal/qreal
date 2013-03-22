TEMPLATE = lib

CONFIG += plugin
DESTDIR = ../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj
UI_DIR = .ui

HEADERS += \
	testInvocationPlugin.h \

SOURCES += \
	testInvocationPlugin.cpp \
