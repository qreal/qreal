TEMPLATE = lib
CONFIG += plugin
DESTDIR = @@pathToQReal@@/bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L@@pathToQReal@@/bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	constraints@@metamodelName@@Plugin.h \
	@@pathToQReal@@/qrgui/pluginManager/editorManagerInterface.h \
@@constraintsDiagramHFiles@@

SOURCES += \
	constraints@@metamodelName@@Plugin.cpp \
	@@pathToQReal@@/qrgui/constraintsPluginInterface/checkStatus.cpp \
@@constraintsDiagramCppFiles@@
