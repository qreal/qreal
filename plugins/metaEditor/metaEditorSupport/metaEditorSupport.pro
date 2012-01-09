QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils

TRANSLATIONS = metaEditorSupport_ru.ts

RESOURCES = metaEditorSupport.qrc

HEADERS += \
	metaEditorSupportPlugin.h \
	editorGenerator.h \

SOURCES += \
	metaEditorSupportPlugin.cpp \
	editorGenerator.cpp \
