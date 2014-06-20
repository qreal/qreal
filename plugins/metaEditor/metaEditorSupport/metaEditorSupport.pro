QT += xml widgets

TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrmc

TRANSLATIONS = metaEditorSupport_ru.ts

RESOURCES = metaEditorSupport.qrc

HEADERS += \
	metaEditorSupportPlugin.h \
	editorGenerator.h \
	xmlParser.h \
	compilerPage.h \
	../../../qrgui/dialogs/preferencesPages/preferencesPage.h

SOURCES += \
	metaEditorSupportPlugin.cpp \
	editorGenerator.cpp \
	xmlParser.cpp \
	compilerPage.cpp \

FORMS += \
	compilerPage.ui \

