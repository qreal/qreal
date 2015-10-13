TEMPLATE = lib

include($$PWD/../../../global.pri)

DESTDIR = $$PWD/../../../bin

TARGET = qrgui-constraints-plugin-interface

links(qrkernel)

DEFINES += QRGUI_CONSTRAINTS_PLUGIN_INTERFACE_LIBRARY

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../../../ \

HEADERS += \
	$$PWD/constraintsPluginInterfaceDeclSpec.h \
	$$PWD/constraintsPluginInterface.h \
	$$PWD/checkStatus.h

SOURCES += \
	$$PWD/checkStatus.cpp


