TARGET = robots-plugin

include(../../../../global.pri)

DESTDIR = $$DESTDIR/plugins/tools/

TEMPLATE = lib
CONFIG += plugin

QT += widgets

includes(plugins/robots/interpreters/interpreterCore \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrgui-tool-plugin-interface robots-interpreter-core)

HEADERS += \
	$$PWD/robotsPlugin.h \

SOURCES += \
	$$PWD/robotsPlugin.cpp \
