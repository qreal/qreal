TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-facade

QT += widgets

LIBS += -L$$PWD/../../bin -lqrkernel -lqslog -lqrgui-models -lqrgui-plugin-manager -lqrgui-text-editor \
		-lqrgui-tool-plugin-interface \

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

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
