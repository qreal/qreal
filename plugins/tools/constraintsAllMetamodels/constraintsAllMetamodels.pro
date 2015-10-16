include(../../../global.pri)

TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = $$DESTDIR/plugins/tools/
MOC_DIR = .moc
RCC_DIR = .moc

links(qrkernel qrutils qrrepo qrgui-constraints-plugin-interface)

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

HEADERS += \
	constraintsAllMetamodelsPlugin.h \
	../../../qrgui/plugins/pluginManager/editorManagerInterface.h \
	../../../qrgui/plugins/constraintsPluginInterface/checkStatus.h \
	constraintsAllLanguages.h \


SOURCES += \
	constraintsAllMetamodelsPlugin.cpp \
	constraintsAllLanguages.cpp \
