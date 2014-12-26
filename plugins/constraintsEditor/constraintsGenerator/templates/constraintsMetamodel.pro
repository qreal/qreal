TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = @@pathToQReal@@/bin/plugins/tools
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L@@pathToQReal@@/bin -lqrkernel -lqrutils -lqrrepo -lqrgui-constraints-plugin-interface

INCLUDEPATH += \
	@@pathToQReal@@ \
	@@pathToQReal@@/qrgui/ \

HEADERS += \
	constraints@@metamodelName@@Plugin.h \
	@@pathToQReal@@/qrgui/pluginManager/editorManagerInterface.h \
	@@pathToQReal@@/qrgui/plugins/pluginManager/editorManagerInterface.h \
	@@pathToQReal@@/qrgui/plugins/constraintsPluginInterface/checkStatus.h \
@@constraintsDiagramHFiles@@

SOURCES += \
	constraints@@metamodelName@@Plugin.cpp \
@@constraintsDiagramCppFiles@@
