TARGET = qrgui-facade

include(../../global.pri)

TEMPLATE = lib

QT += widgets

links(qrkernel qslog qrgui-models qrgui-plugin-manager qrgui-text-editor qrgui-tool-plugin-interface)

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_system_facade_ru.ts

DEFINES += QRGUI_SYSTEM_FACADE_LIBRARY

HEADERS += \
	$$PWD/systemFacadeDeclSpec.h \
	$$PWD/systemFacade.h \
	$$PWD/components/nullErrorReporter.h \
	$$PWD/components/consoleErrorReporter.h \
	$$PWD/components/nullTextManager.h \
	$$PWD/components/projectManager.h \
	$$PWD/components/autosaver.h \
	$$PWD/components/nullMainWindow.h \

SOURCES += \
	$$PWD/systemFacade.cpp \
	$$PWD/components/nullErrorReporter.cpp \
	$$PWD/components/consoleErrorReporter.cpp \
	$$PWD/components/nullTextManager.cpp \
	$$PWD/components/projectManager.cpp \
	$$PWD/components/autosaver.cpp \
	$$PWD/components/nullMainWindow.cpp \
