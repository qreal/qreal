include(../../../global.pri)

TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qrutils qrrepo qrgui-constraints-plugin-interface)

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

HEADERS += \
	constraintsAllMetamodelsPlugin.h \
	constraintsAllLanguages.h \

SOURCES += \
	constraintsAllMetamodelsPlugin.cpp \
	constraintsAllLanguages.cpp \
