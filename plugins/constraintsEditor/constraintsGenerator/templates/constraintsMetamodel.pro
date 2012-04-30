TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	constraints@@metamodelName@@Plugin.h \
	../../../qrgui/pluginManager/editorManagerInterface.h \
@@constraintsDiagramHFiles@@

SOURCES += \
	constraints@@metamodelName@@Plugin.cpp \
	../../../qrgui/constraintsPluginInterface/checkStatus.cpp \
@@constraintsDiagramCppFiles@@
