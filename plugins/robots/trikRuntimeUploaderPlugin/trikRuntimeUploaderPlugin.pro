QT += widgets
TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../bin/plugins/tools
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	trikRuntimeUploaderPlugin.h

SOURCES += \
	trikRuntimeUploaderPlugin.cpp

TRANSLATIONS += $$PWD/trikRuntimeUploaderPlugin_ru.ts

RESOURCES += $$PWD/trikRuntimeUploaderPlugin.qrc \
