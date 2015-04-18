TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-versioning-plugins-manager

QT += widgets

LIBS += -L$$PWD/../../bin -lqrkernel -lqrutils -lqrrepo -lqrgui-tool-plugin-interface

DEFINES += QRGUI_VERSIONING_PLUGINS_MANAGER_LIBRARY

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/qrgui_versioningPluginsManager_ru.ts

HEADERS += \
	$$PWD/versioningPluginsManagerDeclSpec.h \
	$$PWD/versioningPluginsManager.h \
	$$PWD/versioningPluginInterface.h \
	$$PWD/diffPluginInterface.h \

SOURCES += \
	$$PWD/versioningPluginsManager.cpp \

