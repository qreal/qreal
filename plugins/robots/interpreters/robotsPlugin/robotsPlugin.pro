TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

DESTDIR = ../../../../bin/plugins/tools/
TARGET = robots-plugin

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui

QT += widgets

INCLUDEPATH += \
	$$PWD/ \
	$$PWD/../interpreterCore/include/ \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../../generators/generatorBase/include/ \
	$$PWD/../../utils/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \
	$$PWD/../../../../qrtext/include \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrgui-tool-plugin-interface -lrobots-interpreter-core

HEADERS += \
	$$PWD/robotsPlugin.h \

SOURCES += \
	$$PWD/robotsPlugin.cpp \
