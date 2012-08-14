TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/

MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj

LIBS += -L../../bin -lqrkernel -lqrutils

HEADERS += \
	diffPlugin.h \

SOURCES += \
	diffPlugin.cpp \
