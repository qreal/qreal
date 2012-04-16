TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	constraintsPlugin.h \
@@constraintsDiagramHFiles@@

SOURCES += \
	constraintsPlugin.cpp \
	../../../qrgui/constraintsPluginInterface/checkStatus.cpp \
@@constraintsDiagramCppFiles@@
