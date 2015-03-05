include(../../../global.pri)

QT += xml widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qrutils qrmc qrgui-preferences-dialog)

HEADERS += \
	metaEditorSupportPlugin.h \
	editorGenerator.h \
	xmlParser.h \
	compilerPage.h \

SOURCES += \
	metaEditorSupportPlugin.cpp \
	editorGenerator.cpp \
	xmlParser.cpp \
	compilerPage.cpp \

FORMS += \
	compilerPage.ui \

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/metaEditorSupport_ru.ts

RESOURCES = metaEditorSupport.qrc
