TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = @@pathToQReal@@/bin/plugins/tools
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L@@pathToQReal@@/bin -lqrkernel -lqrutils -lqrrepo

INCLUDEPATH += \
	$$PWD/@@pathToQReal@@ \
	$$PWD/@@pathToQReal@@/qrgui/ \

HEADERS += \
	constraints@@metamodelName@@Plugin.h \
	@@pathToQReal@@/qrgui/pluginManager/editorManagerInterface.h \
@@constraintsDiagramHFiles@@

SOURCES += \
	constraints@@metamodelName@@Plugin.cpp \
	@@pathToQReal@@/qrgui/constraintsPluginInterface/checkStatus.cpp \
@@constraintsDiagramCppFiles@@
