TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../../bin/plugins/kitPlugins/

QT += widgets

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui

LIBS += -L../../../../bin/plugins -lrobotsInterpreterCore

INCLUDEPATH += \
	../robotsInterpreterCore/include \
	../../../../ \

HEADERS += \
	nullKitInterpreterInterface.h \

SOURCES += \
	nullKitInterpreterInterface.cpp \
