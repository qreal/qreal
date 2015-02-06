TARGET = robots-plugin

include(../../../../global.pri)

DESTDIR = $$DESTDIR/plugins/tools/

TEMPLATE = lib
CONFIG += plugin

QT += widgets

INCLUDEPATH += \
	$$PWD/../interpreterCore/include/ \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../../generators/generatorBase/include/ \
	$$PWD/../../utils/include/ \
	$$PWD/../../../../qrgui/ \
	$$PWD/../../../../qrtext/include \

links(qrkernel qrgui-tool-plugin-interface robots-interpreter-core)

HEADERS += \
	$$PWD/robotsPlugin.h \

SOURCES += \
	$$PWD/robotsPlugin.cpp \
